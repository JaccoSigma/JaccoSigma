#include "T_IPC.H"
#include "T_NUC.H"

#ifdef UC_IPC

#define GIF_CODESIZE   	7
#define GIF_COLOURS    	7
#define GIF_MAXCOL 		  128

#define GIF_CLEARCODE  	128
#define GIF_EOFCODE    	129
#define GIF_FIRSTFREE  	130

typedef struct
{
	_BYTE	  version[6];	  					// Version
	_UWORD	PicWidth;          			// Breite des Bildausschnittes
	_UWORD	PicHeight;	        		// Höhe des Bildausschnittes
	_UBYTE	ResolutionFlag;					// bits per pixel, resolution, color map
	_UBYTE	BackgndColor;       		// Hintergrundfarbe
	_UBYTE	PixelAspectRatio;   		// Pixel aspect ratio
	_UBYTE  ColorMap[GIF_MAXCOL*3];	// Farbpalette
	_BYTE		ImageSeperator;	    		// Image Separator Header
	_UWORD	X_Start;            		// X-Startposition
	_UWORD 	Y_Start;            		// Y-Startposition
	_UWORD  width;              		// Breite
	_UWORD	height;             	 	// Höhe
	_UBYTE  flags;              		// flags
	_UBYTE	CodeSize;	          		// code size
}_GIF_HEADER;



_UBYTE fromScreen(_UDWORD offset, _KOORD x1, _KOORD y1, _UWORD width, _UBYTE *palette)
{
	_UWORD	x, y;
	_UDWORD	color;

	x = offset % width;
	y = offset / width;

	color = MakeIndexColor(ReadPixel(x+x1, y+y1), 128, palette);

	return((_UBYTE)color);
}

_UBYTE from_BMP(_UDWORD offset, _BMP *bmp, _UBYTE *palette, _UWORD *line, _UDWORD *lastpixel)
{
	_UDWORD	color;
  _UWORD x, y;


  x = offset % bmp->width;

  if(offset >= *lastpixel)
  {
    y = offset / bmp->width;
    *lastpixel = (y * bmp->width) + (bmp->width - 1);
    get_pixel_line(line, bmp, y);
  }

	color = MakeIndexColor(line[x], 128, palette);

	return((_UBYTE)color);
}

_BOOL save_gif_intern(_ASCII *dpne, _KOORD x1, _KOORD y1, _KOORD x2, _KOORD y2, _BMP *bmp)
{
	_BOOL     retcode;
  _BOOL     InitOk = _FALSE;
	_File	    File;
	_UDWORD		PixelCount, PixelIndex, BlcSize;
	_WORD		  anz;
	_UBYTE		c;
	_UBYTE		d;
	_BOOL		  fnd;
	_UWORD		lblc;
	_UWORD		OutCount;
	_UBYTE		DateLngCnt;
	_UWORD		DateLng;
	_UBYTE		FreeCode;
	_UBYTE		BitMask;
	_UBYTE		LNG[257];
	_UBYTE		Prefix[257];
	_UBYTE		Suffix[257];
	_UBYTE		OutCode[513];
	_UBYTE		*OutData = NULL;
	_GIF_HEADER *GifHeader = NULL;
  _UBYTE    palette[130*3];
  _BOOL     screen = _TRUE;
  _UWORD    *line = NULL;
  _UDWORD   lastpixel = 0;


#ifdef UC_MOUSE
	HideMouseCursor();
#endif

  if(bmp != NULL)
    screen = _FALSE;

	if(File.Open(SAVE, dpne, 0, _TRUE) == _TRUE)
	{
    Palette.Get(palette, 0, 128);

		if(Memory2((void**)&GifHeader, sizeof(_GIF_HEADER)) != 0)
    {
  	  if(Memory2((void**)&OutData, 1024) != 0)
      {
			  MemSet(LNG, 0, sizeof(LNG));
			  MemSet(Prefix, 0, sizeof(Prefix));
			  MemSet(Suffix, 0, sizeof(Suffix));
			  MemSet(OutCode, 0, sizeof(OutCode));

			  MemSet(OutData, 0, 1024);

        if(screen == _TRUE)
        {
  			  PixelCount = (x2 - x1 + 1) * (y2 - y1 + 1);
          InitOk = _TRUE;
        }
        else
        {
          PixelCount = bmp->width * bmp->height;
          if(Memory2((void**)&line, bmp->width * sizeof(*line)) != 0)
            InitOk = _TRUE;
        }

			  PixelIndex = 0;
			  BlcSize = 125;

      }
		}
	}

	if(InitOk == _TRUE)
	{
		// GIF Header schreiben
		// Kennung und Version eintragen
		MemCpy(GifHeader->version,"GIF87a", sizeof(GifHeader->version));

		// Logical descriptor
    if(screen == _TRUE)
    {
  		GifHeader->PicWidth = x2 - x1 + 1;
	  	GifHeader->PicHeight = y2 - y1 + 1;
    }
    else
    {
      GifHeader->PicWidth = bmp->width;
      GifHeader->PicHeight = bmp->height;
    }
		GifHeader->ResolutionFlag = ((1 << 7) | (GIF_COLOURS << 4)) + 6;
		GifHeader->BackgndColor = 0;
		GifHeader->PixelAspectRatio = 0;

		//  Global color map block
		Palette.Get(GifHeader->ColorMap, 0, GIF_MAXCOL);

		// Image descriptor block
		GifHeader->ImageSeperator = ',';
		GifHeader->X_Start = 0;
		GifHeader->Y_Start = 0;
		GifHeader->width = GifHeader->PicWidth;
		GifHeader->height = GifHeader->PicHeight;
		GifHeader->flags = 0;             // No local color maps

		// ... gehört eigentlich schon zu den Daten
		GifHeader->CodeSize = GIF_CODESIZE;

		File.Write(GifHeader, sizeof(_GIF_HEADER));

		// Bilddaten
    ReadPixelStart(x1, y1, x2, y2);
		for(;;)
		{
			if(PixelIndex < PixelCount)
			{
				// ... Find possible value for BlcSize
				if((PixelCount-PixelIndex) <= BlcSize)
					BlcSize = PixelCount - PixelIndex - 1;

				OutCount = 0;		        // Counter of OUT date
//				DateLngCnt = 0;         // Counter of compressed date length (until 0)
//				DateLng = 0;     	      // Counter of compressed date length (to know)
				BitMask = 127;          // 2^(CodeSize-1)-1
				FreeCode = GIF_FIRSTFREE;

				LNG[GIF_FIRSTFREE] = 0;	// To be sure that len of code is 0


				do                      // Repeat until all bytes from INdate done
				{

					c = FreeCode;         // First possible value at table plus 1

					do                    // find value from table if next combination
					{				              // OF bytes from INdate already added TO table
						c -= 1;             // Find prev. possible value at table
						DateLngCnt = LNG[c];// length of byte combination
						fnd = _TRUE;
						d = c;
						if(d < GIF_FIRSTFREE)
							fnd = _FALSE;

						DateLng = 1;

						if((PixelIndex + 1 + DateLngCnt) > PixelCount)
							fnd = _FALSE;

						while((d >= GIF_FIRSTFREE) && (fnd == _TRUE))
						{
              if(screen == _TRUE)
              {
  							if(Prefix[d] != (fromScreen(PixelIndex + DateLngCnt, x1, y1, GifHeader->width, palette)))
		  						fnd = _FALSE;
              }
              else
              {
                if(Prefix[d] != from_BMP(PixelIndex + DateLngCnt, bmp, palette, line, &lastpixel))
                  fnd = _FALSE;
              }

							DateLng += 1;
							DateLngCnt -= 1;
							d = Suffix[d];

							if(d <= BitMask)
							{
                if(screen == _TRUE)
                {
  								if((fromScreen(PixelIndex, x1, y1, GifHeader->width, palette)) != d)
	  								fnd = _FALSE;
                }
                else
                {
  								if(from_BMP(PixelIndex, bmp, palette, line, &lastpixel) != d)
	  								fnd = _FALSE;
                }
							}
						}

					}while((fnd != _TRUE) && (c >= GIF_FIRSTFREE));

					if(fnd == _FALSE)
          {
            if(screen == _TRUE)
  						c = fromScreen(PixelIndex, x1, y1, GifHeader->width, palette);
            else
  						c = from_BMP(PixelIndex, bmp, palette, line, &lastpixel);

          }

					OutCode[OutCount] = c;
					OutCount += 1;

					PixelIndex += DateLng;

					// ... Creating new value in table:
					//     Real code for output (pixel)
          if(screen == _TRUE)
    				Prefix[FreeCode] = fromScreen(PixelIndex, x1, y1, GifHeader->width, palette);
          else
          {
    				Prefix[FreeCode] = from_BMP(PixelIndex, bmp, palette, line, &lastpixel);
          }

					Suffix[FreeCode] = c;        // Prev. code
					if (c > BitMask)
						LNG[FreeCode] = LNG[c] + 1;
					else
						LNG[FreeCode] = 1;

				}while(OutCount < BlcSize);

				if(PixelIndex >= PixelCount)
					lblc = 2;
				else
					lblc = 1;

				*(OutData + 0) = OutCount + lblc;				// Block length
				*(OutData + 1) = GIF_CLEARCODE;                 // Clear code
				MemCpy((OutData + 2), OutCode, sizeof(OutCode));

				anz = OutCount + 2;
				File.Write(OutData, anz);
			}

			else
			{
				*(OutData + 0) = GIF_EOFCODE;                   // END OF FILE
				*(OutData + 1) = 0;                             // END OF FILE
				*(OutData + 2) = 0x3b;                          // END OF FILE
				anz = 3;
				File.Write(OutData, anz);
				break;
			}
		}
    ReadPixelStop();
	}

#ifdef UC_MOUSE
	ShowMouseCursor();
#endif

	retcode = File.Close();

	Memory2((void**)&GifHeader, 0);
	Memory2((void**)&OutData, 0);
  Memory2((void**)&line, 0);

	return(retcode);
}

_BOOL save_gif(_ASCII *dpne, _KOORD x1, _KOORD y1, _KOORD x2, _KOORD y2)
{

  return(save_gif_intern(dpne, x1, y1, x2, y2, NULL));
}

_BOOL save_gif_BMP(_ASCII *dpne, _BMP *bmp)
{

  return(save_gif_intern(dpne, 0, 0, 0, 0, bmp));
}

_UDWORD pwr2[17];
_DWORD nBitsIn;
_DWORD num;
_DWORD aChar;
_DWORD blockLength;
_DWORD paletteOffset, dataOffset;

_DWORD get_bit(_File *File)
{
	nBitsIn++;
	if(nBitsIn == 9)
	{
		aChar = 0;
		File->Read(&aChar, 1);
		nBitsIn = 1;
		num++;
		if(num == blockLength)
		{
			blockLength = aChar + 1;
			aChar = 0;
			File->Read(&aChar, 1);
			num = 1;
		}
	}

	if((aChar & pwr2[nBitsIn-1]) == 0)
		return 0;
	else
		return 1;
}

_DWORD read_code(_File *File, int codeSize)
{
	_DWORD i, code=0;


	for(i=0; i<codeSize; i++)
		code += get_bit(File) * pwr2[i];

	return(code);
}

/*
_BOOL load_gif(_BMP *bmp, _ASCII *dpne, _UWORD *p_errorcode)
{
  _BOOL		 retcode = _FALSE;
	_File 	 File;
	_MakeBMP MakeBmp;
	_RGB256  *rgb = NULL;
	_BYTE		 version[6];
	_UWORD	 ScreenWidth;
	_UWORD	 ScreenHeight;
	_UBYTE   ResolutionFlag;
	_UBYTE   ImageSeperator;
	_UWORD   xStart;
	_UWORD   yStart;
	_UWORD	 PicWidth;
	_UWORD	 PicHeight;
	_UWORD   x, y;
	_DWORD   i;
	_DWORD   codeSize, code, inCode, curCode, oldCode;
	_DWORD   clearCode, eofCode, freeCode, firstFree, maxCode, initCodeSize,
					 bitMask, finChar, outCount;
	_DWORD   *prefix = NULL;
	_DWORD   *suffix = NULL;
	_DWORD   *outCode = NULL;


	if(File.Open(LOAD, dpne) == _TRUE)
	{
		retcode = _TRUE;

		if(Memory2((void**)&prefix, 4096 * sizeof(_DWORD)) == NULL)
			retcode = _FALSE;
		if(Memory2((void**)&suffix, 4096 * sizeof(_DWORD)) == NULL)
			retcode = _FALSE;
		if(Memory2((void**)&outCode, 1025 * sizeof(_DWORD)) == NULL)
			retcode = _FALSE;
		if(Memory2((void**)&rgb, 256 * 3) == NULL)
			retcode = _FALSE;

		if(retcode == _TRUE)
		{
			File.Read(&version, sizeof(version));
			if(MemCmp(&version, "GIF87a", sizeof(version)) != 0)
				retcode = _FALSE;

			File.Read(&ScreenWidth, sizeof(ScreenWidth));
			File.Read(&ScreenHeight, sizeof(ScreenHeight));
			File.Read(&ResolutionFlag, sizeof(ResolutionFlag));
			if((ResolutionFlag & 0x80) != 0x80)    // no global color map ?
				retcode = _FALSE;

			// Set file pointer to color table
			File.Seek(2, SEEK_CUR);
			File.Read(rgb, (1 << (((ResolutionFlag & 7) + 1))) * 3); // (2^col_no)*3

			File.Read(&xStart, sizeof(xStart));
			File.Read(&yStart, sizeof(yStart));
			File.Read(&ImageSeperator, sizeof(ImageSeperator));
			if((ImageSeperator & 0xc0) != 0) // Local color map ?? Interlaced ??
				retcode = _FALSE;          // Interlaced bit is not set, also not in interlaced gifs ?????

			File.Read(&PicWidth, sizeof(PicWidth));
			File.Read(&PicHeight, sizeof(PicHeight));

			// Set file pointer to picture data
			File.Seek(1, SEEK_CUR);

			nBitsIn = 8;
			num = 0;
			pwr2[0] = 1;
			for (i=1; i<17; i++)
				pwr2[i] = pwr2[i-1]*2;

			x = 0;
			y = 0;

			if(retcode == _TRUE)
				retcode = MakeBmp.Start(PicWidth, PicHeight);
		}
	}


	if(retcode == _TRUE)
	{

		codeSize = 0;
		File.Read(&codeSize, 1);
		clearCode = pwr2[codeSize];									// clear code = 2^codeSize
		eofCode = clearCode + 1;										// end of file code
		freeCode = firstFree = clearCode + 2;				// free entry code
		codeSize++;																	// actual code size
		initCodeSize = codeSize;										// which is initial too
		maxCode = pwr2[codeSize];
		bitMask = pwr2[(ResolutionFlag & 7)+1]-1;

		blockLength = 0;
		File.Read(&blockLength, 1);
		blockLength++;
		outCount = 0;

		do
		{
			code = read_code(&File, codeSize);
			if(code == eofCode)
				break;
			if(code == clearCode)
			{
				codeSize = initCodeSize;								// set to initial code size
				maxCode = pwr2[codeSize];
				freeCode = firstFree;    								// set to initial free code
				code = read_code(&File, codeSize);			// read next code
				oldCode = curCode = code;								// make it current and old
				finChar = code & bitMask;								// extract bits to plot
//				WritePixel(x, y, T_SOLID, finChar);
				MakeBmp.Add(x++, y, finChar);
//				MakeBmp.Add(finChar);
				if(x >= PicWidth)    									    // next line?
				{
					x = 0;
					y++;
				}
			}
			else
			{
				inCode = curCode = code;

				if(code >= freeCode)
				{
					curCode = oldCode;
					outCode [outCount++] = finChar;
				}

				if(curCode > bitMask)
				{
					do
					{
						outCode [outCount++] = suffix [curCode];
						curCode = prefix [curCode];
					}while (curCode > bitMask);
				}

				finChar = curCode & bitMask;
				outCode [outCount++] = finChar;

				for(i=outCount-1; i>=0; i--)
				{
//					WritePixel(x, y, T_SOLID, outCode[i]);
					MakeBmp.Add(x++, y, outCode[i]);
//					MakeBmp.Add(outCode[i]);
					if(x >= PicWidth)
					{
						x = 0;
						y++;
					}
				}

				outCount = 0;
				prefix [freeCode] = oldCode;
				suffix [freeCode++] = finChar;
				oldCode = inCode;
				if(freeCode >= maxCode && codeSize < 12)
				{
					codeSize++;
					maxCode *= 2;
				}
			}
		}while(code != eofCode);

		MemCpy(bmp, MakeBmp.Stop(), sizeof(_BMP));
		colorCorrection256(bmp, rgb, (1 << (((ResolutionFlag & 7) + 1))));

	}

	File.Close();
	Memory2((void**)&prefix, 0);
	Memory2((void**)&suffix, 0);
	Memory2((void**)&outCode, 0);
	Memory2((void**)&rgb, 0);

	return(retcode);
}
*/

_BOOL load_gif(_BMP *bmp, _ASCII *dpne, _UWORD *p_errorcode)
{
  _BOOL    retcode = _FALSE;
  _File    File;
  _MakeBMP MakeBmp;
  _RGB256  *rgb = NULL;
  _RGB256  *rgbloc = NULL;
  _BYTE    version[7];
  _UWORD   ScreenWidth, ScreenHeight;
  _UBYTE   ResolutionFlag;
  _UWORD   xStart, yStart;
  _UWORD   PicWidth, PicHeight;
  _UWORD   x, y;
  _DWORD   i;
  _DWORD   codeSize, code, inCode, curCode, oldCode;
  _DWORD   clearCode, eofCode, freeCode, firstFree, maxCode, initCodeSize, bitMask, finChar, outCount;
  _DWORD   *prefix = NULL;
  _DWORD   *suffix = NULL;
  _DWORD   *outCode = NULL;
  _UBYTE   extsig, help1, help2, gcm, lcm, interlace;
  _UWORD   *ytable;
  _UWORD   *yptr;
  _UWORD   yct;

  gfxerror_set(p_errorcode, GFXERROR_NONE);

  if(File.Open(LOAD, dpne) == _TRUE)
  {
    retcode = _TRUE;

    if(Memory((void**)&prefix,  4096 * sizeof(_DWORD)) == NULL)
      retcode = _FALSE;
    if(Memory((void**)&suffix,  4096 * sizeof(_DWORD)) == NULL)
      retcode = _FALSE;
    if(Memory((void**)&outCode, 1025 * sizeof(_DWORD)) == NULL)
      retcode = _FALSE;
    if(Memory((void**)&rgb, 256 * 3) == NULL)
      retcode = _FALSE;


    if(retcode == _TRUE)
    {
      File.Read(&version, 6);
      File.Read(&ScreenWidth,    sizeof(ScreenWidth));
      File.Read(&ScreenHeight,   sizeof(ScreenHeight));
      File.Read(&ResolutionFlag, sizeof(ResolutionFlag));
      gcm = 1;
      if((ResolutionFlag & 0x80) != 0x80)    // no global color map ?
        gcm = 0;

      // Set file pointer to color table
      File.Seek(2, SEEK_CUR);
      File.Read(rgb, (1 << (((ResolutionFlag & 7) + 1))) * 3); // (2^col_no)*3

      File.Read(&help1, sizeof(help1));
      if(help1 == 0x21)
      {
        do
        {
          File.Read(&extsig, sizeof(extsig));
          if(extsig==0xF9)
          {
            File.Seek(6, SEEK_CUR);
            File.Read(&help1, sizeof(help1));
          }
          else
          {
            File.Read(&help2, sizeof(help2));
            do
            {
              File.Seek(help2, SEEK_CUR);
              File.Read(&help2, sizeof(help2));
            }
            while(help2 != 0);

            File.Read(&help1, sizeof(help1));
          }
        }
        while(help1 == 0x21);
      }

      File.Read(&xStart,    sizeof(xStart));
      File.Read(&yStart,    sizeof(yStart));
      File.Read(&PicWidth,  sizeof(PicWidth));
      File.Read(&PicHeight, sizeof(PicHeight));

      File.Read(&help1, sizeof(help1));

      interlace = (help1>>6)&1;
      lcm = (help1>>7)&1;

      if(lcm==0 && gcm == 0)
        retcode = _FALSE;

      if(lcm!=0)
      {
        if(Memory((void**)&rgbloc, 256 * 3) != NULL)
          File.Read(rgbloc, (1 << (((help1 & 7) + 1))) * 3);
        else
        {
          retcode = _FALSE;
          gfxerror_set(p_errorcode, GFXERROR_TOOLESSMEMORY);
        }
      }

      nBitsIn = 8;
      num     = 0;
      pwr2[0] = 1;

      for(i=1; i<17; i++)
        pwr2[i] = pwr2[i-1]*2;

      x = 0;
      y = 0;

      if(retcode == _TRUE)
        retcode = MakeBmp.Start(PicWidth, PicHeight);
    }
  }
  else
  {
    gfxerror_set(p_errorcode, GFXERROR_CANTOPEN);
  }


  if(retcode == _TRUE)
  {
    codeSize = 0;
    File.Read(&codeSize, 1);
    clearCode = pwr2[codeSize];									// clear code = 2^codeSize
    eofCode   = clearCode + 1;									// end of file code
    freeCode  = firstFree = clearCode + 2;			// free entry code
    codeSize++;																	// actual code size
    initCodeSize = codeSize;										// which is initial too
    maxCode = pwr2[codeSize];
    bitMask = pwr2[(ResolutionFlag & 7)+1]-1;

    blockLength = 0;
    File.Read(&blockLength, 1);
    blockLength++;
    outCount = 0;

    if(interlace==0)    // file is stored in normal mode
    {
      do
      {
        code = read_code(&File, codeSize);
        if(code == eofCode)
          break;
        if(code == clearCode)
        {
          codeSize = initCodeSize;								// set to initial code size
          maxCode = pwr2[codeSize];
          freeCode = firstFree;    								// set to initial free code
          code = read_code(&File, codeSize);			// read next code
          oldCode = curCode = code;								// make it current and old
          finChar = code & bitMask;								// extract bits to plot
          MakeBmp.Add(x++, y, finChar);
          if(x >= PicWidth)    									    // next line?
          {
            x = 0;
            y++;
          }
        }
        else
        {
          inCode = curCode = code;

          if(code >= freeCode)
          {
            curCode = oldCode;
            outCode[outCount++] = finChar;
          }

          if(curCode > bitMask)
          {
            do
            {
              outCode[outCount++] = suffix[curCode];
              curCode = prefix[curCode];
            }
            while(curCode > bitMask);
          }

          finChar = curCode & bitMask;
          outCode[outCount++] = finChar;

          for(i=outCount-1; i>=0; i--)
          {
            MakeBmp.Add(x++, y, outCode[i]);
            if(x >= PicWidth)
            {
              x = 0;
              y++;
            }
          }

          outCount = 0;
          prefix[freeCode] = oldCode;
          suffix[freeCode++] = finChar;
          oldCode = inCode;
          if(freeCode >= maxCode && codeSize < 12)
          {
            codeSize++;
            maxCode *= 2;
          }
        }
      }
      while(code != eofCode);

      MemCpy(bmp, MakeBmp.Stop(), sizeof(_BMP));

      if(lcm==0)
        colorCorrection256(bmp, rgb, (1 << (((ResolutionFlag & 7) + 1))));
      else
      {
        colorCorrection256(bmp, rgbloc, (1 << (((help1 & 7) + 1))));
        Memory((void**)&rgbloc, 0);
      }
    }
    else                                                     // File is stored in interlace-mode
    {
      ytable = NULL;
      if(Memory((void**)&ytable, ScreenHeight*sizeof(_UWORD))!=0)
      {
        yct = 0;                                           // write ytable for interlace-mode
        yptr = ytable;
        do
        {
          *yptr = yct;
          yptr++;
          yct += 8;
        }
        while(yct<ScreenHeight);

        yct = 4;
        do
        {
          *yptr = yct;
          yptr++;
          yct += 8;
        }
        while(yct<ScreenHeight);

        yct = 2;
        do
        {
          *yptr = yct;
          yptr++;
          yct += 4;
        }
        while(yct<ScreenHeight);

        yct = 1;
        do
        {
          *yptr = yct;
          yptr++;
          yct += 2;
        }
        while(yct<ScreenHeight);

        do
        {
          code = read_code(&File, codeSize);
          if(code == eofCode)
            break;
          if(code == clearCode)
          {
            codeSize = initCodeSize;								// set to initial code size
            maxCode = pwr2[codeSize];
            freeCode = firstFree;    								// set to initial free code
            code = read_code(&File, codeSize);			// read next code
            oldCode = curCode = code;								// make it current and old
            finChar = code & bitMask;								// extract bits to plot
            MakeBmp.Add(x++, ytable[y], finChar);   // ytable-entry instead of y in interlace-mode
            if(x >= PicWidth)    									  // next line?
            {
              x = 0;
              y++;
            }
          }
          else
          {
            inCode = curCode = code;

            if(code >= freeCode)
            {
              curCode = oldCode;
              outCode[outCount++] = finChar;
            }

            if(curCode > bitMask)
            {
              do
              {
                outCode[outCount++] = suffix[curCode];
                curCode = prefix[curCode];
              }
              while(curCode > bitMask);
            }

            finChar = curCode & bitMask;
            outCode[outCount++] = finChar;

            for(i=outCount-1; i>=0; i--)
            {
              MakeBmp.Add(x++, ytable[y], outCode[i]);           // ytable-entry instead of y in interlace-mode
              if(x >= PicWidth)
              {
                x = 0;
                y++;
              }
            }

            outCount = 0;
            prefix[freeCode] = oldCode;
            suffix[freeCode++] = finChar;
            oldCode = inCode;
            if(freeCode >= maxCode && codeSize < 12)
            {
              codeSize++;
              maxCode *= 2;
            }
          }
        }
        while(code != eofCode);

        MemCpy(bmp, MakeBmp.Stop(), sizeof(_BMP));

        if(lcm == 0)
          colorCorrection256(bmp, rgb, (1 << (((ResolutionFlag & 7) + 1))));
        else
        {
          colorCorrection256(bmp, rgbloc, (1 << (((help1 & 7) + 1))));
          Memory((void**)&rgbloc, 0);
        }

        Memory((void**)&ytable, 0);
      }
    }
  }

  if(File.GetError() == _TRUE)
    gfxerror_set(p_errorcode, GFXERROR_INVALIDFILE);

  File.Close();
  Memory((void**)&prefix, 0);
  Memory((void**)&suffix, 0);
  Memory((void**)&outCode, 0);
  Memory((void**)&rgb, 0);

  return(retcode);
}

// *****************************************************************************
// *****************************************************************************
/* animiertes gif **************************************************************
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
// t_types.h *******************************************************************
// *****************************************************************************

_SINGLE_MOVIE : STRUCT
  dxx              : INT;
  dyy              : INT;
  delay            : UDINT;
  bmp              : _BMP;
  gce              : USINT;
END_STRUCT;

_MOVIE : STRUCT
  no               : udint;
  ptr              : ^_SINGLE_MOVIE;
  count            : udint;
  oldtime          : udint;
  actframe         : udint;
  width            : uint;
  height           : uint;
  backcol          : _COLOR;
END_STRUCT;

FUNCTION GLOBAL __cdecl start_MOVIE VAR_INPUT p0 : ^_MOVIE; p1 : ^_ASCII; END_VAR VAR_OUTPUT retcode : BOOL; END_VAR;
FUNCTION GLOBAL __cdecl ready_MOVIE VAR_INPUT p0 : ^_MOVIE; pr : ^_ROOM; pi : ^_IMAGE; END_VAR;
FUNCTION GLOBAL __cdecl run_MOVIE VAR_INPUT p0 : ^_MOVIE; pr : ^_ROOM; pi : ^_IMAGE; END_VAR;
FUNCTION GLOBAL __cdecl end_MOVIE VAR_INPUT p0 : ^_MOVIE; END_VAR;


// *****************************************************************************
// t_nuc.h *********************************************************************
// *****************************************************************************

typedef struct { _WORD       dx;
                 _WORD       dy;
                 _UDWORD     delay;
                 _BMP        bmp;
                 _UBYTE      gce;
               }
_SINGLE_MOVIE;

typedef struct { _UDWORD       no;
                 _SINGLE_MOVIE *ptr;
                 _UDWORD       count;
                 _UDWORD       oldtime;
                 _UDWORD       actframe;
                 _UWORD        width;
                 _UWORD        height;
                 _COLOR        backcol;
               }
_MOVIE;

extern "C" _BOOL  start_MOVIE(_MOVIE*, _ASCII*);
extern "C" void   ready_MOVIE(_MOVIE*, _ROOM*, _IMAGE*);
extern "C" void   run_MOVIE(_MOVIE*, _ROOM*, _IMAGE*);
extern "C" void   end_MOVIE(_MOVIE*);

// *****************************************************************************
// t_gfx.cpp *******************************************************************
// *****************************************************************************

_UWORD seek_color(_UWORD *p, _UWORD index)
{
  _UWORD i, col = p[index];
  for(i=0;i<256; i++, p++)
  {
    if(*p == col) // reihenfolge aus geschwindigkeitsgründen so gewählt !!
      if(i != index)
        return(1);
  }
  return(0);
}

void change_color(_UWORD *p, _UWORD index)
{
  do
  {
    p[index] += 1;
  }
  while(seek_color(p, index));
}

_UWORD colorCorrection256_transparent(_BMP *b, _RGB256 *rgb, _UWORD no, _UWORD transparentindex)
{
  _UDWORD i;
  _UWORD  index[256];

  if((rgb != NULL)&&(no <= 256))
  {
    for(i=0; i<no; i++)
      index[i] = MakeTrueColor(rgb[i].cred, rgb[i].cgreen, rgb[i].cblue);

    if(seek_color(index, transparentindex) != 0)
      change_color(index, transparentindex);

    for(i=0; i<b->datano; i++)
      b->ptr[i].col = index[b->ptr[i].col];

    return(index[transparentindex]);
  }

  return(INVISIBLE);
}

// *****************************************************************************
// t_gif.cpp *******************************************************************
// *****************************************************************************

void init_SINGLE_MOVIE(_SINGLE_MOVIE *p)
{
  p->dx    = 0;
  p->dy    = 0;
  p->delay = 0;
  p->gce   = 0;
  init_BMP(&p->bmp);
}

void free_SINGLE_MOVIE(_SINGLE_MOVIE *p)
{
  free_BMP(&p->bmp);
  init_SINGLE_MOVIE(p);
}

void init_MOVIE(_MOVIE *p)
{
  p->no         = 0;
  p->ptr        = NULL;
  p->count      = 0;
  p->oldtime    = 0;
  p->actframe   = 0;
}

void free_MOVIE(_MOVIE *p)
{
  _SINGLE_MOVIE *ptr;
  _UDWORD       i;
  for(i=0, ptr=p->ptr; i<p->no; i++, ptr++)
    free_SINGLE_MOVIE(ptr);
  Memory2((void**)&p->ptr, 0);  
  init_MOVIE(p);
}


_BOOL add_MOVIE(_MOVIE *dst, _BMP *bmp, _WORD dx, _WORD dy, _UBYTE gce, _UDWORD delay)
{
  _SINGLE_MOVIE *psm;
  
  if(Memory((void**)&dst->ptr, (dst->no + 1)*sizeof(_SINGLE_MOVIE)) != 0)
  {
    psm = &dst->ptr[dst->no];
    init_SINGLE_MOVIE(psm);
    psm->bmp   = *bmp;
    psm->dx    = dx;
    psm->dy    = dy;
    psm->delay = delay;
    psm->gce   = gce & 0xFE;
    dst->no   += 1;
    return(_TRUE);
  }
  return(_FALSE);
}

_BOOL load_MOVIE(_MOVIE *pmovie, _ASCII *dpne)
{
  _BMP     bmp;
  _BOOL    retcode = _FALSE;
  _File    File;
  _MakeBMP MakeBmp;
  _RGB256  rgb[256], rgbloc[256];
  _BYTE    version[7];
  _UWORD   ScreenWidth;
  _UWORD   ScreenHeight;
  _UBYTE   ResolutionFlag;
//  _UBYTE   ImageSeperator;
  _UWORD   xStart;
  _UWORD   yStart;
  _UWORD   PicWidth;
  _UWORD   PicHeight;
  _UWORD   x, y;
  _DWORD   i;
  _DWORD   codeSize, code, inCode, curCode, oldCode;
  _DWORD   clearCode, eofCode, freeCode, firstFree, maxCode, initCodeSize, bitMask, finChar, outCount;
  _DWORD   *prefix = NULL;
  _DWORD   *suffix = NULL;
  _DWORD   *outCode = NULL;
  _UBYTE   extsig, help1, help2;
  _UBYTE   gcm, lcm, interlace;
  _UWORD   *ytable;
  _UWORD   *yptr;
  _UWORD   yct, run, delay_time = 0;
  _UBYTE   gce_flag, transparent_color_index, back_color_index;
  _UDWORD  size;

  if(File.Open(LOAD, dpne) == _TRUE)
  {
    init_BMP(&bmp);
    retcode = _TRUE;

    if(Memory((void**)&prefix,  4096 * sizeof(_DWORD)) == NULL)
      retcode = _FALSE;
    if(Memory((void**)&suffix,  4096 * sizeof(_DWORD)) == NULL)
      retcode = _FALSE;
    if(Memory((void**)&outCode, 1025 * sizeof(_DWORD)) == NULL)
      retcode = _FALSE;

    if(retcode == _TRUE)
    {
      File.Read(&version, 6);
      File.Read(&ScreenWidth,    sizeof(ScreenWidth));
      File.Read(&ScreenHeight,   sizeof(ScreenHeight));
      File.Read(&ResolutionFlag, sizeof(ResolutionFlag));
      File.Read(&back_color_index, 1);
      // Set file pointer to color table
      File.Seek(1, SEEK_CUR);
      pmovie->backcol = INVISIBLE; //DEFCOLOR;
      pmovie->width   = ScreenWidth;
      pmovie->height  = ScreenHeight;
      
      if(ResolutionFlag & 0x80)    // global color map ?
      {
        gcm     = 1;
        size    = (1 << (((ResolutionFlag & 7) + 1))) * 3;
        if(size <= sizeof(rgb))
        {
          File.Read(rgb, size);
          pmovie->backcol = MakeTrueColor(rgb[back_color_index].cred, rgb[back_color_index].cgreen, rgb[back_color_index].cblue);
        }  
        else
        {
          retcode = _FALSE;
        }
      }
      else
      {
        gcm = 0;
      }


      do
      {
        run = 0;
        File.Read(&help1, sizeof(help1));
        if(help1 == 0x21)
        {
          run = 1;
          File.Read(&extsig, sizeof(extsig));
          if(extsig==0xF9)
          {
            File.Seek(1, SEEK_CUR);
            File.Read(&gce_flag, 1);
            File.Read(&delay_time, 2);
            File.Read(&transparent_color_index, 1);
            File.Seek(1, SEEK_CUR);
          }
          else
          {
            File.Read(&help2, sizeof(help2));
            do
            {
              File.Seek(help2, SEEK_CUR);
              File.Read(&help2, sizeof(help2));
            }
            while(help2 != 0);
          }
        }
        else if(help1 == 0x2C)
        {
          run = 1;
//          ImageSeperator = help1;

          File.Read(&xStart,    sizeof(xStart));
          File.Read(&yStart,    sizeof(yStart));
          File.Read(&PicWidth,  sizeof(PicWidth));
          File.Read(&PicHeight, sizeof(PicHeight));

          File.Read(&help1, sizeof(help1));

          interlace = (help1>>6)&1;
          lcm = (help1>>7)&1;

          if(lcm==0 && gcm == 0)
            retcode = _FALSE;

          if(lcm!=0)
          {
            size = (1 << (((help1 & 7) + 1))) * 3;
            if(size <= sizeof(rgbloc))
              File.Read(rgbloc, size);
            else
              retcode = _FALSE;
          }

          nBitsIn = 8;
          num = 0;
          pwr2[0] = 1;
          for (i=1; i<17; i++)
            pwr2[i] = pwr2[i-1]*2;

          x = 0;
          y = 0;

          if(retcode == _TRUE)
          {
            if(MakeBmp.Start(PicWidth, PicHeight) == _TRUE)
            {
              codeSize = 0;
              File.Read(&codeSize, 1);
              clearCode = pwr2[codeSize];									// clear code = 2^codeSize
              eofCode = clearCode + 1;										// end of file code
              freeCode = firstFree = clearCode + 2;				// free entry code
              codeSize++;																	// actual code size
              initCodeSize = codeSize;										// which is initial too
              maxCode = pwr2[codeSize];
              bitMask = pwr2[(ResolutionFlag & 7)+1]-1;

              blockLength = 0;
              File.Read(&blockLength, 1);
              blockLength++;
              outCount = 0;

              if(interlace==0)    // file is stored in normal mode
              {
                do
                {
                  code = read_code(&File, codeSize);
                  if(code == eofCode)
                    break;
                  if(code == clearCode)
                  {
                    codeSize = initCodeSize;								// set to initial code size
                    maxCode = pwr2[codeSize];
                    freeCode = firstFree;    								// set to initial free code
                    code = read_code(&File, codeSize);			// read next code
                    oldCode = curCode = code;								// make it current and old
                    finChar = code & bitMask;								// extract bits to plot
                    MakeBmp.Add(x++, y, finChar);
                    if(x >= PicWidth)    									    // next line?
                    {
                      x = 0;
                      y ++;
                    }
                  }
                  else
                  {
                    inCode = curCode = code;

                    if(code >= freeCode)
                    {
                      curCode = oldCode;
                      outCode[outCount++] = finChar;
                    }

                    if(curCode > bitMask)
                    {
                      do
                      {
                        outCode[outCount++] = suffix[curCode];
                        curCode = prefix[curCode];
                      }
                      while(curCode > bitMask);
                    }

                    finChar = curCode & bitMask;
                    outCode[outCount++] = finChar;

                    for(i=outCount-1; i>=0; i--)
                    {
                      MakeBmp.Add(x++, y, outCode[i]);
                      if(x >= PicWidth)
                      {
                        x = 0;
                        y++;
                      }
                    }

                    outCount = 0;
                    prefix[freeCode] = oldCode;
                    suffix[freeCode++] = finChar;
                    oldCode = inCode;
                    if((freeCode >= maxCode) && (codeSize < 12))
                    {
                      codeSize++;
                      maxCode *= 2;
                    }
                  }
                }
                while(code != eofCode);
              }
              else                                                     // File is stored in interlace-mode
              {
                ytable = NULL;
                if(Memory((void**)&ytable, ScreenHeight*sizeof(_UWORD)) != 0)
                {
                  yct = 0;                                           // write ytable for interlace-mode
                  yptr = ytable;
                  do
                  {
                    *yptr = yct;
                    yptr ++;
                    yct += 8;
                  }
                  while(yct<ScreenHeight);

                  yct = 4;
                  do
                  {
                    *yptr = yct;
                    yptr ++;
                    yct += 8;
                  }
                  while(yct<ScreenHeight);

                  yct = 2;
                  do
                  {
                    *yptr = yct;
                    yptr++;
                    yct+=4;
                  }
                  while(yct<ScreenHeight);

                  yct = 1;
                  do
                  {
                    *yptr = yct;
                    yptr ++;
                    yct += 2;
                  }
                  while(yct<ScreenHeight);

                  do
                  {
                    code = read_code(&File, codeSize);
                    if(code == eofCode)
                      break;
                    if(code == clearCode)
                    {
                      codeSize = initCodeSize;								// set to initial code size
                      maxCode = pwr2[codeSize];
                      freeCode = firstFree;    								// set to initial free code
                      code = read_code(&File, codeSize);			        // read next code
                      oldCode = curCode = code;								// make it current and old
                      finChar = code & bitMask;								// extract bits to plot
                      MakeBmp.Add(x++, ytable[y], finChar);                 // ytable-entry instead of y in interlace-mode
                      if(x >= PicWidth)    									// next line?
                      {
                        x = 0;
                        y++;
                      }
                    }
                    else
                    {
                      inCode = curCode = code;

                      if(code >= freeCode)
                      {
                        curCode = oldCode;
                        outCode[outCount++] = finChar;
                      }

                      if(curCode > bitMask)
                      {
                        do
                        {
                          outCode[outCount++] = suffix[curCode];
                          curCode = prefix[curCode];
                        }
                        while(curCode > bitMask);
                      }
                      finChar = curCode & bitMask;
                      outCode[outCount++] = finChar;

                      for(i=outCount-1; i>=0; i--)
                      {
                        MakeBmp.Add(x++, ytable[y], outCode[i]);           // ytable-entry instead of y in interlace-mode
                        if(x >= PicWidth)
                        {
                          x = 0;
                          y++;
                        }
                      }

                      outCount = 0;
                      prefix[freeCode] = oldCode;
                      suffix[freeCode++] = finChar;
                      oldCode = inCode;
                      if((freeCode >= maxCode) && (codeSize < 12))
                      {
                        codeSize++;
                        maxCode *= 2;
                      }
                    }
                  }
                  while(code != eofCode);
                  Memory((void**)&ytable, 0);
                }
              }

              bmp = *MakeBmp.Stop();
              bmp.state |= BMP_NO_TRANSPARENT; // no transparency in image

              if(lcm==0)
              {
                if(gce_flag & 1)  // use transparent colorindex
                {
                  bmp.transparent = colorCorrection256_transparent(&bmp, rgb, (1 << (((ResolutionFlag & 7) + 1))), transparent_color_index);
                  bmp.state &= ~BMP_NO_TRANSPARENT;
                }
                else
                {
                  colorCorrection256(&bmp, rgb, (1 << (((ResolutionFlag & 7) + 1))));
                }
              }
              else
              {
                if(gce_flag & 1)  // use transparent colorindex
                {
                  bmp.transparent = colorCorrection256_transparent(&bmp, rgbloc, (1 << (((help1 & 7) + 1))), transparent_color_index);
                  bmp.state &= ~BMP_NO_TRANSPARENT;
                }
                else
                {
                  colorCorrection256(&bmp, rgbloc, (1 << (((help1 & 7) + 1))));
                }
              }
            }
          }

          File.Read(&help1, sizeof(help1));
          if(retcode == _TRUE)
          {
            retcode = add_MOVIE(pmovie, &bmp, xStart, yStart, gce_flag, delay_time*10);
            // Bitmap(100+xStart, 100+yStart, &bmp, T_COPY|T_SOLID, 0);
            init_BMP(&bmp);
          }

          if(retcode == _FALSE)
            run = 0;
        }
//        else //if(help1 == 0x3B)
//        {
//          run = 0;
//        }
      }
      while(run);
    }

    File.Close();
    free_BMP(&bmp);
    Memory((void**)&prefix, 0);
    Memory((void**)&suffix, 0);
    Memory((void**)&outCode, 0);
  }
  return(retcode);
}

_BOOL start_MOVIE(_MOVIE *p, _ASCII *dpne)
{
  init_MOVIE(p);
  return(load_MOVIE(p, dpne));
}

void ready_MOVIE(_MOVIE *p, _ROOM *pr, _IMAGE *pback)
{
  if(p != NULL)
  {
    p->oldtime = 0;
    run_MOVIE(p, pr, pback);
  }
}

void draw_frame(_MOVIE *p, _UDWORD frameno, _ROOM *pr, _IMAGE *pback, _BOOL do_refresh)
{
  _ROOM         room, space;
  _NEWSCREEN    act;
  _DWORD        width, height;
  _SINGLE_MOVIE *psm;

  if(frameno < p->no)
  {
    psm    = &p->ptr[frameno];
    width  = (pr->xy2.x - pr->xy1.x + 1);
    height = (pr->xy2.y - pr->xy1.y + 1);
    room   = *pr;
    space  = room;

    if(psm->dx != 0)
      if(width == p->width)
        space.xy1.x += psm->dx;
      else
        space.xy1.x += ((psm->dx * width) + (p->width/2)) / p->width;

    if(psm->dy != 0)
      if(height == p->height)
        space.xy1.y += psm->dy;
      else
        space.xy1.y += ((psm->dy * height) + (p->height/2)) / p->height;

    if(psm->bmp.width != p->width)
      if(width == p->width)
        space.xy2.x = space.xy1.x + psm->bmp.width - 1;
      else
        space.xy2.x = space.xy1.x + (((psm->bmp.width * width) + (p->width/2)) / p->width) - 1;

    if(psm->bmp.height != p->height)
      if(height == p->height)
        space.xy2.y = space.xy1.y + psm->bmp.height - 1;
      else
        space.xy2.y = space.xy1.y + (((psm->bmp.height * height) + (p->height/2)) / p->height) - 1;

    if(do_refresh == _TRUE)
      push_screen(&act);

    switch((psm->gce >> 2) & 7)
    {
      case 1 : //Bar(space.xy1.x, space.xy1.y, space.xy2.x, space.xy2.y, T_COPY|T_SOLID, 0);
               break;
      case 2 : Bar(room.xy1.x, room.xy1.y, room.xy2.x, room.xy2.y, T_COPY|T_SOLID, p->backcol);
               break;
      case 3 : if(pback != NULL)
                 PutImage(pback, room.xy1.x, room.xy1.y);
               break;
    }

    out_zoom_BMP(&space, &psm->bmp, T_COPY|T_SOLID);
//        Rectangle(space.xy1.x, space.xy1.y, space.xy2.x, space.xy2.y, T_COPY|T_SOLID, LIGHTGREEN);
//        Bitmap(space.xy1.x, space.xy1.y, &psm->bmp, T_COPY|T_SOLID, 0);
    if(do_refresh == _TRUE)
    {
      pop_screen(&act);
      copy_screen(&act, &room);
    }
  }
}

void draw_background_MOVIE(_MOVIE *p, _ROOM *pr, _IMAGE *pback)
{
  _UDWORD    i;
  _NEWSCREEN act;

  if(p != NULL)
  {
    if(p->no > 0)
    {
      push_screen(&act);

      for(i=0; i<=p->actframe; i++)
        draw_frame(p, i, pr, pback, _FALSE);

      pop_screen(&act);
      copy_screen(&act, pr);
    }
  }
}

void run_MOVIE(_MOVIE *p, _ROOM *pr, _IMAGE *pback)
{
  _BOOL st;

  if(p != NULL)
  {
    if(p->no > 0)
    {
      if(DrawBackGround)
        draw_background_MOVIE(p, pr, pback);

      st = _FALSE;
      if(p->oldtime == 0)
      {
        st = _TRUE;
      }
      else if(timediff(p->oldtime) >= p->ptr[p->actframe].delay)
      {
        st = _TRUE;
      }

      if(st == _TRUE)
      {
        p->actframe = p->count;
        draw_frame(p, p->actframe, pr, pback, _TRUE);

        p->count += 1;
        if(p->count >= p->no)
          p->count = 0;
        p->oldtime = get_abstime();
      }
    }
  }
}

void end_MOVIE(_MOVIE *p)
{
  free_MOVIE(p);
}

*/ //animiertes gif ************************************************************


#endif




