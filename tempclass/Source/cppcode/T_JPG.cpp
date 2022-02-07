#include "T_IPC.H"
#include "T_NUC.H"

_JpgFunctLoad    JpgFunctLoad; // pointer to function jpgLoad
_JpgFunctSave    JpgFunctSave; // pointer to function jpgSave

void JpgFunctSetup(void *fpload, void *fpsave)
{
  JpgFunctLoad = (_JpgFunctLoad)fpload; // pointer to function jpgLoad
  JpgFunctSave = (_JpgFunctSave)fpsave; // pointer to function jpgSave
}

_BOOL load_jpg(_BMP *bmp, _ASCII *dpne, _UWORD *p_errorcode, _FUDWORD_UDWORD_UDWORD fp_callback)
{
  _BOOL   retcode = _FALSE;
  _UWORD  width, height;
  _BMPDAT *bmpdat;

  gfxerror_set(p_errorcode, GFXERROR_NONE);

  if(JpgFunctLoad != NULL)
  {
    if(sizeof(_BMPDAT) != 3)
    {
     #ifdef PGTEST
      Annahme("T_JPG.CPP", __LINE__);
     #endif
      return(_FALSE);
    }

    dpne = ClearRamFile(dpne);

    init_BMP_Makro(bmp);

    //bmpdat = (_BMPDAT*)jpg_load_jpg(&width, &height, dpne, p_errorcode, fp_callback);
    bmpdat = (_BMPDAT*)JpgFunctLoad(&width, &height, dpne, p_errorcode, (void*)fp_callback);
    if(bmpdat != NULL)
    {
      bmp->ptr    = bmpdat;
      bmp->datano = width * height;
      bmp->width  = width;
      bmp->height = height;
      bmp->state  = (BMP_HICOLOR | BMP_NO_TRANSPARENT);

      compress_BMP(bmp);
      retcode = _TRUE;
    }
  }
	return(retcode);
}

_BOOL save_jpg(_ASCII *dpne, _KOORD x0, _KOORD y0, _KOORD x1, _KOORD y1, _UWORD compr_rate, _BOOL grayscale)
{
	_BOOL retcode = _FALSE;
	_UBYTE *rgbbuf = NULL;
	_UBYTE *tmp;
  _UWORD width, height;
	_UDWORD size;
	_COLOR color;
	_UWORD i, j;
	_RGB rgb;
  _UWORD colbit = 1;

  if(JpgFunctSave != NULL)
  {
    dpne = ClearRamFile(dpne);

    if(grayscale == _TRUE)
      colbit = 0;

    width = x1 - x0 + 1;
    height = y1 - y0 + 1;
  	size =  width * height * 3;

    if((width > 0) && (height > 0))
    {
      if(Memory((void**)&rgbbuf, size) != 0)
      {
        tmp = rgbbuf;
        
        ReadPixelStart(x0, y0, x1, y1);
        
        for(i=y0; i<=y1; i++)
        {
          for(j=x0; j<=x1; j++)
          {
            color = ReadPixel(j, i);
            Make_RGB(&rgb, color);
            *tmp = rgb.cred;
            tmp++;
            *tmp = rgb.cgreen;
            tmp++;
            *tmp = rgb.cblue;
            tmp++;
          }
        }
        
        ReadPixelStop();

        //if(jpg_save_jpg(dpne, rgbbuf, width, height, compr_rate, colbit) == _TRUE)
        if(JpgFunctSave(dpne, rgbbuf, width, height, compr_rate, colbit) == _TRUE)
          retcode = _TRUE;

        Memory((void**)&rgbbuf, 0);
      }
    }
  }
  
  return(retcode);
}

_BOOL save_jpg_BMP(_ASCII *dpne, _BMP *bmp, _UWORD compr_rate, _BOOL grayscale)
{
	_BOOL retcode = _FALSE;
	_UBYTE *rgbbuf = NULL;
	_UBYTE *tmp;
	_UDWORD size;
	_UDWORD i, j;
	_RGB rgb;
  _UWORD colbit = 1;
  _BMPDAT *bmpdat;
  _KOORD x, y;
  _BOOL turn = _FALSE;

  if(JpgFunctSave != NULL)
  {
    if(grayscale == _TRUE)
      colbit = 0;

    size = bmp->width * bmp->height * 3;

    bmpdat = bmp->ptr;

    if((bmp->state & BMP_TURN) == BMP_TURN)
    {
      turn = _TRUE;
      x = 0;
      y = bmp->height - 1;
    }

    if((bmp->width > 0) && (bmp->height > 0))
    {
      if(Memory((void**)&rgbbuf, size) != 0)
      {
        tmp = rgbbuf;
        for(i=0; i<bmp->datano; i++)
        {
          Make_RGB(&rgb, bmpdat->col);
          for(j=0; j<bmpdat->no; j++)
          {
            if(turn == _TRUE)
            {
              if(x >= bmp->width)
              {
                x = 0;
                y--;
              }
              tmp = rgbbuf + (((bmp->width * y) + x) * 3);

              x++;
            }

            *tmp = rgb.cred;
            tmp++;
            *tmp = rgb.cgreen;
            tmp++;
            *tmp = rgb.cblue;
            tmp++;
          }

          bmpdat++;
        }

        //if(jpg_save_jpg(dpne, rgbbuf, bmp->width, bmp->height, compr_rate, colbit) == _TRUE)
        if(JpgFunctSave(dpne, rgbbuf, bmp->width, bmp->height, compr_rate, colbit) == _TRUE)
          retcode = _TRUE;

        Memory((void**)&rgbbuf, 0);
      }
    }
  }
  
  return(retcode);
}

/*
_BOOL save_jpg_BMP(_ASCII *dpne, _BMP *bmp, _UWORD compr_rate, _BOOL grayscale)
{
	_BOOL retcode = _FALSE;
	_UBYTE *rgbbuf = NULL;
	_UBYTE *tmp;
	_UDWORD size;
	_UDWORD i, j;
	_RGB rgb;
  _UWORD colbit = 1;
  _BMPDAT *bmpdat;
  _KOORD x, y;
  _BOOL turn = _FALSE;


  if(grayscale == _TRUE)
    colbit = 0;

	size = bmp->width * bmp->height * 3;

  bmpdat = bmp->ptr;

  if((bmp->state & BMP_TURN) == BMP_TURN)
  {
    turn = _TRUE;
    x = 0;
    y = bmp->height - 1;
  }

  if((bmp->width > 0) && (bmp->height > 0))
  {
    if(Memory((void**)&rgbbuf, size) != 0)
    {
      tmp = rgbbuf;
      for(i=0; i<bmp->height; i++)
      {
        for(j=0; j<bmp->width; j++)
        {
          Make_RGB(&rgb, get_pixel(bmp, j, i));
          *tmp = rgb.cred;
          tmp++;
          *tmp = rgb.cgreen;
          tmp++;
          *tmp = rgb.cblue;
          tmp++;
        }

      }

      if(jpg_save_jpg(dpne, rgbbuf, bmp->width, bmp->height, compr_rate, colbit) == _TRUE)
        retcode = _TRUE;

      Memory((void**)&rgbbuf, 0);
    }

  }

  return(retcode);
}
*/

