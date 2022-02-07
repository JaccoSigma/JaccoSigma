#include "T_IPC.H"
#include "T_NUC.H"


#define DIACRIT_NONE   0
#define DIACRIT_OBEN   1
#define DIACRIT_UNTEN  2
#define DIACRIT_RECHTS 3
#define DIACRIT_LINKS  4

#define RENDERENGINE_DEFAULT 0x00000000
#define RENDERENGINE_THAI    0x0000041E
#define RENDERENGINE_HINDI   0x00000439
//#define RENDERENGINE_ARABIC  0x00000429  // persisch


// ***************************************************************************
// ***************************************************************************
// *** HINDI TEXT-RENDER ENGINE neu neu neu neu neu neu neu neu **************
// ***************************************************************************

#define SUNSCRITPUFFERLEN 2000
_UNI SunScritPuffer[SUNSCRITPUFFERLEN+1];

_UNI IsHalfForm(_UNI chr)
{
  // halfformzeichen sind auf glyph-index - 0x100 zu finden

  switch(chr)
  {
    case 0x915:
    case 0x916:
    case 0x917:
    case 0x918:
    case 0x91A:
    case 0x91C:
    case 0x91D:
    case 0x91E:
    case 0x924:
    case 0x925:
    case 0x927:
    case 0x928:
    case 0x929:
    case 0x92A:
    case 0x92B:
    case 0x92C:
    case 0x92D:
    case 0x92E:
    case 0x92F:
    case 0x932:
    case 0x935:
    case 0x936:
    case 0x937:
    case 0x938:
    case 0x958:
    case 0x959:
    case 0x95A:
    case 0x95B:
    case 0x95E:
    case 0x95F: return(chr - 0x100);
  }

  return(0);
}

_UWORD IsLigature(_UNI *txt)
{
  // zeichen werden zu einem neuen zeichen verschmolzen
  // die "neuen zeichen" existieren nicht in der original-unicodetabelle
  // ich habe sie auf 8DB bis 8FF gelegt

  switch(txt[0])
  {
    case 0x915: switch(txt[2])
    {
      case 0x915 : return(0x8FF);
      case 0x924 : return(0x8FE);
      case 0x930 : return(0x8FD);
      case 0x937 : return(0x8FC);
    }
    break;
    case 0x919: switch(txt[2])
    {
      case 0x915 : return(0x8FB);
      case 0x916 : return(0x8FA);
      case 0x917 : return(0x8F9);
      case 0x918 : return(0x8F8);
    }
    break;
    case 0x91E: switch(txt[2])
    {
      case 0x91C : return(0x8F7);
    }
    break;
    case 0x91C: switch(txt[2])
    {
      case 0x91E : return(0x8F6);
    }
    break;
    case 0x926: switch(txt[2])
    {
      case 0x918 : return(0x8F5);
      case 0x926 : return(0x8F4);
      case 0x927 : return(0x8F3);
      case 0x92C : return(0x8F2);
      case 0x92D : return(0x8F1);
      case 0x92E : return(0x8F0);
      case 0x92F : return(0x8EF);
      case 0x935 : return(0x8EE);
    }
    break;
    case 0x91F: switch(txt[2])
    {
      case 0x91F : return(0x8ED);
      case 0x920 : return(0x8EC);
    }
    break;
    case 0x920: switch(txt[2])
    {
      case 0x920 : return(0x8EB);
    }
    break;
    case 0x921: switch(txt[2])
    {
      case 0x917 : return(0x8EA);
      case 0x921 : return(0x8E9);
      case 0x922 : return(0x8E8);
    }
    break;
    case 0x924: switch(txt[2])
    {
      case 0x924 : return(0x8E7);
      case 0x930 : return(0x8E6);
    }
    break;
    case 0x928: switch(txt[2])
    {
      case 0x928 : return(0x8E5);
    }
    break;
    case 0x92B: switch(txt[2])
    {
      case 0x930 : return(0x8E4);
    }
    break;
    case 0x936: switch(txt[2])
    {
      case 0x930 : return(0x8E3);
    }
    break;
    case 0x939: switch(txt[2])
    {
      case 0x92E : return(0x8E2);
      case 0x92F : return(0x8E1);
      case 0x932 : return(0x8E0);
      case 0x935 : return(0x8DF);
    }
    break;
  }

  return(0);
}

_UNI *SunskritHindi(_UNI *txt)
{
  _UNI   tmp, *pd;
  _UWORD len;

  // als erstes werden alle zeichen mit folgendem punkt (0x93C) verschmolzen
  // info aus unicodebuch seite 402...

  pd  = SunScritPuffer;
  len = 0;
  while(txt[0])
  {
    if(len < SUNSCRITPUFFERLEN)
    {
      len   += 1;
      pd[0] = txt[0];

      if(txt[1] == 0x93C)
      {
        switch(txt[0])
        {
          case 0x928 : pd[0] = 0x929; txt += 1; break;
          case 0x930 : pd[0] = 0x931; txt += 1; break;
          case 0x933 : pd[0] = 0x934; txt += 1; break;
          case 0x915 : pd[0] = 0x958; txt += 1; break;
          case 0x916 : pd[0] = 0x959; txt += 1; break;
          case 0x917 : pd[0] = 0x95A; txt += 1; break;
          case 0x91C : pd[0] = 0x95B; txt += 1; break;
          case 0x921 : pd[0] = 0x95C; txt += 1; break;
          case 0x922 : pd[0] = 0x95D; txt += 1; break;
          case 0x92B : pd[0] = 0x95E; txt += 1; break;
          case 0x92F : pd[0] = 0x95F; txt += 1; break;
        }
      }
      else if(txt[1] == 0x943)
      {
        switch(txt[0])
        {
          case 0x930 : pd[0] = 0x8DB; txt += 1; break; // Ligature
          case 0x939 : pd[0] = 0x8DE; txt += 1; break; // Ligature
        }
      }
      else if(txt[1] == 0x941)
      {
        if(txt[0] == 0x930) // Ligature
        {
          pd[0] = 0x8DD;
          txt += 1;
        }
      }
      else if(txt[1] == 0x942)
      {
        if(txt[0] == 0x930) // Ligature
        {
          pd[0] = 0x8DC;
          txt += 1;
        }
      }
      if(txt[0] == 0x930)
      {
        if(txt[1] == 0x94D)
        {
          // erfinde neues diacrit weil devanagari doof ist
          pd[0] = 0x8D9;
          txt += 1;
        }
      }
      pd  += 1;
    }

    txt += 1;
  }
  pd[0] = 0;

  // auf len steht jetzt die tatsächliche stringlänge

  txt = SunScritPuffer;
  pd  = SunScritPuffer;

  while(len)
  {
    pd[0] = txt[0]; // vorerst einmal zeichen eintragen

    if(txt[1] == 0x94D) // superdiacrit
    {
      tmp = IsLigature(txt);
      if(tmp != 0)
      {
        pd[0] = tmp;
        txt += 2;
        len -= 2;
      }
      else if(IsHalfForm(txt[0]) != 0)
	    {
	      txt[0] -= 0x100;
        txt += 1;
        len -= 1;
	    }
    }

    pd  += 1;
    len -= 1;
    txt += 1;
  }
  pd[0] = 0;

  pd = SunScritPuffer;
  while(pd[0]) // korrektur von neuem diacrit damit's hinterm zeichen steht
  {
    if(pd[0] == 0x8D9)
    {
      if(pd[1] != 0)
      {
        pd[0] = pd[1];
        pd[1] = 0x8D9;
        pd += 1;
      }
    }
    pd += 1;
  }

  pd = SunScritPuffer;
  while(pd[0]) // korrektur von 0x93F diacrit damit's vorm zeichen steht
  {
    if(pd[1] == 0x93F)
    {
      pd[1] = pd[0];
      pd[0] = 0x93F;
    }
    pd += 1;
  }

  return(SunScritPuffer);
}

_UWORD HindiFontSize(_UWORD fontsize)
{
  // retouniert die strichbreite der zeichen 93F, 940, 94A, 94B, 94C
  // die strichbreite muss bei diesen zeichen, innerhalb eines fonts, immer gleich sein

  switch(fontsize)
  {
    case 12 : // Arial 7
    case 15 : return(4); // Arial 8
    case 16 : return(4); // Arial 9
    case 18 : return(4); // Arial 10
    case 20 : return(5); // Arial 11
    case 21 : return(5); // Arial 12
    case 23 : return(5); // Arial 13
    case 25 : return(5); // Arial 14
    case 28 : return(6); // Arial 16
    case 33 : return(9); // Arial 18
    case 36 : return(10); // Arial 20
    case 29 : return(10); // Arial 22
    case 43 : return(11); // Arial 24
    case 46 : return(12); // Arial 26
    case 50 : return(13); // Arial 28
    case 54 : return(14); // Arial 30
    case 58 : return(14); // Arial 32
    default : break;
  }
  
  return(12); // fontsize
}

_WORD HindiChrOffset(_UWORD xx, _UNI chr, _WORD fontsize)
{

  // die zeichen R, L und M stehen für RECHTS, LINKS und MITTE
  // der erste buchstabe definiert den aufhängepunkt des diakrits über dem zeichen, der zweite die ausrichtung des diakrits


  // Beispiel: ML           LL          RR
  
  //   DDDDD              DDD            DD
  //   DDDDD Diakrit      DDD            DD
  // CCCCC                CCCCC       CCCCC
  // CCCCC   character    CCCCC       CCCCC

  switch(chr)
  {
    case 0x093F : return(0); // RL
    case 0x0901 :
    case 0x0945 : return(-xx); // LL
    case 0x093C :
    case 0x0942 :
    case 0x0943 :
    case 0x0951 :
    case 0x0952 :
    case 0x0953 :
    case 0x0954 : return(-((xx + ChrWidth(chr, sizeof(_UNI)))/2)); // MM
    case 0x0940 :
    case 0x094A :
    case 0x094B :
    case 0x094C : return(-(ChrWidth(chr, sizeof(_UNI))-HindiFontSize(fontsize))); // RR + Offset
    case 0x0941 :
    case 0x0944 :
    case 0x094D :
    case 0x0962 :
    case 0x0963 : return(-ChrWidth(chr, sizeof(_UNI))); // RR
    case 0x0946 :
    case 0x0947 :
    case 0x0948 : return(-((xx/2) + ChrWidth(chr, sizeof(_UNI)))); // MR
    case 0x0902 : 
    case 0x08D9 : return(-(xx/2)); // ML erfundenes diacrit
                  
    default     : break;
  }

  return(0);
}

_UWORD HindiChrWidth(_UNI chr, _UWORD fontsize)
{
  switch(chr)
  {
    case 0x093F :
    case 0x0940 :
    case 0x094A :
    case 0x094B :
    case 0x094C : return(HindiFontSize(fontsize));
    case 0x08D9 : // erfundenes diacrit
    case 0x0901 :
    case 0x0902 :
    case 0x093C :
    case 0x0941 :
    case 0x0942 :
    case 0x0943 :
    case 0x0944 :
    case 0x0945 :
    case 0x0946 :
    case 0x0947 :
    case 0x0948 :
    case 0x094D :
    case 0x0951 :
    case 0x0952 :
    case 0x0953 :
    case 0x0954 :
    case 0x0962 :
    case 0x0963 : return(0);
    default     : break;
  }

  return(ChrWidth(chr, sizeof(_UNI)));
}

void HindiOut(_KOORD x, _KOORD y, _UNI *txt, _ATTRIB att, _COLOR col)
{
  _UNI   puf[2];
  _UWORD xx;
  _WORD  tmp;
  _UWORD fontsize = Font.GetActualHeight();

  txt    = SunskritHindi(txt);
  puf[1] = 0;
  xx     = 0; // zeichenbreite

  while(txt[0])
  {
    puf[0] = txt[0];
    tmp    = HindiChrOffset(xx, puf[0], fontsize);
    OutTextXY(x + tmp, y, puf, att, col, sizeof(_UNI));
    tmp    = HindiChrWidth(puf[0], fontsize);
    if(tmp != 0)
    {
      xx = tmp;
      x += tmp;
    }

    txt += 1;
  }
}

_UWORD HindiStrWidth(_UNI *txt)
{
  _UWORD retcode = 0;
  _UWORD fontsize = Font.GetActualHeight();

  txt = SunskritHindi(txt);
  while(txt[0])
  {
    retcode += HindiChrWidth(txt[0], fontsize);
    txt += 1;
  }

  return(retcode);
}

// ***************************************************************************
// ***************************************************************************
// *** HINDI TEXT-RENDER ENGINE **********************************************
// ***************************************************************************
/*
#define SUNSCRITPUFFERLEN 2000
_UNI SunScritPuffer[SUNSCRITPUFFERLEN+1];

_UWORD IsDiacritHindi(_UNI chr)
{
  switch(chr)
  {
    case 0x0945:
    case 0x0946:
    case 0x0947:
    case 0x0948:
    case 0x0901:
    case 0x0951:
    case 0x0953:
    case 0x0954:
    case 0x0902: return(DIACRIT_OBEN);
//    case 0x093E: +1
//    case 0x0940: +1/3
//    case 0x0949: +1
//    case 0x094A: +1/3
//    case 0x094B: +1/3
//    case 0x094C: +1/3
//    case 0x0903: +1
//	             return(DIACRIT_RECHTS);
    case 0x08D9: // erfundenes diacrit
    case 0x0941:
    case 0x0942:
    case 0x0943:
    case 0x0944:
    case 0x094D: // superdiacrit !!
    case 0x0952:
    case 0x0962:
    case 0x0963:
    case 0x093C: return(DIACRIT_UNTEN);
    case 0x093F: return(DIACRIT_LINKS);
  }

  return(DIACRIT_NONE);
}

_WORD DiacritOffsetHindi(_UNI chr, _WORD xx)
{
  _WORD xd = ChrWidth(chr, sizeof(_UNI));

  switch(chr)
  {
    case 0x0941 :
    case 0x0945 :
    case 0x0954 :
    case 0x0901 : return(0);          // linksbündig

	case 0x0902 :
	case 0x0942 :
	case 0x0943 :
	case 0x0944 :
	case 0x094D :
	case 0x0951 :
	case 0x0952 :
	case 0x0953 : return((xx - xd)/2); // mittig

    case 0x08D9 : // erfundenes diacrit
                  return(xx);

    case 0x0946 :
    case 0x0947 :
    case 0x0948 : return(-((xd+2)/3)); // etwas links

    case 0x093F : return((xd * 3 + 2)/5); // sonderfall
  }

  return(0);
}

_WORD CharacterOffsetHindi(_UNI chr, _WORD xx)
{

  switch(chr)
  {
    case 0x0940:
    case 0x094A:
    case 0x094B:
    case 0x094C: return(-((xx*3+2)/5));
  }

  return(0);
}


_UWORD IsLigature(_UNI *txt)
{
  // zeichen werden zu einem neuen zeichen verschmolzen
  // die "neuen zeichen" existieren nicht in der original-unicodetabelle
  // ich habe sie auf 8DB bis 8FF gelegt

  switch(txt[0])
  {
    case 0x915: switch(txt[2])
    {
      case 0x915 : return(0x8FF);
      case 0x924 : return(0x8FE);
      case 0x930 : return(0x8FD);
      case 0x937 : return(0x8FC);
    }
    break;
    case 0x919: switch(txt[2])
    {
      case 0x915 : return(0x8FB);
      case 0x916 : return(0x8FA);
      case 0x917 : return(0x8F9);
      case 0x918 : return(0x8F8);
    }
    break;
    case 0x91E: switch(txt[2])
    {
      case 0x91C : return(0x8F7);
    }
    break;
    case 0x91C: switch(txt[2])
    {
      case 0x91E : return(0x8F6);
    }
    break;
    case 0x926: switch(txt[2])
    {
      case 0x918 : return(0x8F5);
      case 0x926 : return(0x8F4);
      case 0x927 : return(0x8F3);
      case 0x92C : return(0x8F2);
      case 0x92D : return(0x8F1);
      case 0x92E : return(0x8F0);
      case 0x92F : return(0x8EF);
      case 0x935 : return(0x8EE);
    }
    break;
    case 0x91F: switch(txt[2])
    {
      case 0x91F : return(0x8ED);
      case 0x920 : return(0x8EC);
    }
    break;
    case 0x920: switch(txt[2])
    {
      case 0x920 : return(0x8EB);
    }
    break;
    case 0x921: switch(txt[2])
    {
      case 0x917 : return(0x8EA);
      case 0x921 : return(0x8E9);
      case 0x922 : return(0x8E8);
    }
    break;
    case 0x924: switch(txt[2])
    {
      case 0x924 : return(0x8E7);
      case 0x930 : return(0x8E6);
    }
    break;
    case 0x928: switch(txt[2])
    {
      case 0x928 : return(0x8E5);
    }
    break;
    case 0x92B: switch(txt[2])
    {
      case 0x930 : return(0x8E4);
    }
    break;
    case 0x936: switch(txt[2])
    {
      case 0x930 : return(0x8E3);
    }
    break;
    case 0x939: switch(txt[2])
    {
      case 0x92E : return(0x8E2);
      case 0x92F : return(0x8E1);
      case 0x932 : return(0x8E0);
      case 0x935 : return(0x8DF);
    }
    break;
  }

  return(0);
}

_UNI IsHalfForm(_UNI chr)
{
  // halfformzeichen sind auf glyph-index - 0x100 zu finden

  switch(chr)
  {
    case 0x915:
    case 0x916:
    case 0x917:
    case 0x918:
    case 0x91A:
    case 0x91C:
    case 0x91D:
    case 0x91E:
    case 0x924:
    case 0x925:
    case 0x927:
    case 0x928:
    case 0x929:
    case 0x92A:
    case 0x92B:
    case 0x92C:
    case 0x92D:
    case 0x92E:
    case 0x92F:
    case 0x932:
    case 0x935:
    case 0x936:
    case 0x937:
    case 0x938:
    case 0x958:
    case 0x959:
    case 0x95A:
    case 0x95B:
    case 0x95E:
    case 0x95F: return(chr - 0x100);
  }

  return(0);
}


_UNI *SunskritHindi(_UNI *txt)
{
  _UNI   tmp, *pd;
  _UWORD len;


//  pd = txt;
//  len = 0;
//  while(pd[0])
//  {
//    Debug(pd[0], len++);
//    pd += 1;
//  }


  // als erstes werden alle zeichen mit folgendem punkt (0x93C) verschmolzen
  // info aus unicodebuch seite 402..

  pd  = SunScritPuffer;
  len = 0;
  while(txt[0])
  {
    if(len < SUNSCRITPUFFERLEN)
    {
      len   += 1;
      pd[0] = txt[0];

      if(txt[1] == 0x93C)
      {
        switch(txt[0])
        {
          case 0x928 : pd[0] = 0x929; txt += 1; break;
          case 0x930 : pd[0] = 0x931; txt += 1; break;
          case 0x933 : pd[0] = 0x934; txt += 1; break;
          case 0x915 : pd[0] = 0x958; txt += 1; break;
          case 0x916 : pd[0] = 0x959; txt += 1; break;
          case 0x917 : pd[0] = 0x95A; txt += 1; break;
          case 0x91C : pd[0] = 0x95B; txt += 1; break;
          case 0x921 : pd[0] = 0x95C; txt += 1; break;
          case 0x922 : pd[0] = 0x95D; txt += 1; break;
          case 0x92B : pd[0] = 0x95E; txt += 1; break;
          case 0x92F : pd[0] = 0x95F; txt += 1; break;
        }
      }
      else if(txt[1] == 0x943)
      {
        switch(txt[0])
        {
          case 0x930 : pd[0] = 0x8DB; txt += 1; break; // Ligature
          case 0x939 : pd[0] = 0x8DE; txt += 1; break; // Ligature
        }
      }
      else if(txt[1] == 0x941)
      {
        if(txt[0] == 0x930) // Ligature
        {
          pd[0] = 0x8DD;
          txt += 1;
        }
      }
      else if(txt[1] == 0x942)
      {
        if(txt[0] == 0x930) // Ligature
        {
          pd[0] = 0x8DC;
          txt += 1;
        }
      }
      if(txt[0] == 0x930)
      {
        if(txt[1] == 0x94D)
        {
          // erfinde neues diacrit weil devanagari doof ist
          pd[0] = 0x8D9;
          txt += 1;
        }
      }

      pd  += 1;
    }

    txt += 1;
  }
  pd[0] = 0;

  // auf len steht jetzt die tatsächliche stringlänge

  txt = SunScritPuffer;
  pd  = SunScritPuffer;

  while(len)
  {
    pd[0] = txt[0]; // vorerst einmal zeichen eintragen

    if(txt[1] == 0x94D) // superdiacrit
    {
      tmp = IsLigature(txt);
      if(tmp != 0)
      {
        pd[0] = tmp;
        txt += 2;
        len -= 2;
      }
      else if(IsHalfForm(txt[0]) != 0)
	    {
	      txt[0] -= 0x100;
        txt += 1;
        len -= 1;
	    }
    }

    pd  += 1;
    len -= 1;
    txt += 1;
  }
  pd[0] = 0;

  pd = SunScritPuffer;
  while(pd[0]) // korrektur von neuem diacrit damit's hinterm zeichen steht
  {
    if(pd[0] == 0x8D9)
    {
      if(pd[1] != 0)
      {
        pd[0] = pd[1];
        pd[1] = 0x8D9;
        pd += 1;
      }
    }
    pd += 1;
  }

  return(SunScritPuffer);
}

void HindiOut(_KOORD x, _KOORD y, _UNI *txt, _ATTRIB att, _COLOR col)
{
  _UNI    puf[2];
  _UWORD  xx;
//  _UWORD  diacrit_count_oben, diacrit_count_unten;
  _KOORD  y1, x1;
//  _WORD   h;
  _UWORD  diacrit;


//  saubauch();
  txt = SunskritHindi(txt);

  // h entspricht dem y-offset des nächsten diacrits, wenn mehrere über einem zeichen liegen
//  h      = ((Font.GetActualHeight()*7+19)/39);
  puf[1] = 0;
//  diacrit_count_oben  = 0;
//  diacrit_count_unten = 0;
  xx                  = 0;

  while(txt[0])
  {
    puf[0] = txt[0];

    diacrit = IsDiacritHindi(puf[0]);

    if(diacrit != DIACRIT_NONE) // es ist ein diacrit
    {
      x1 = x;
      y1 = y;

      if(diacrit == DIACRIT_OBEN)
      {
//        y1 -= diacrit_count_oben * h;
//        diacrit_count_oben += 1;
        x1 += DiacritOffsetHindi(puf[0], xx); // position um sonderzeichenoffset korrigieren
        OutTextXY(x1, y1, puf, att, col, sizeof(_UNI));
      }
      else if(diacrit == DIACRIT_UNTEN)
      {
//        y1 += diacrit_count_unten * h;
//        diacrit_count_unten += 1;
        x1 += DiacritOffsetHindi(puf[0], xx); // position um sonderzeichenoffset korrigieren
        OutTextXY(x1, y1, puf, att, col, sizeof(_UNI));
      }
//	  else if(diacrit == DIACRIT_LINKS)
//	  {
//	    // wurde bereits vor dem zeichen ausgegeben
//	  }
    }
    else  // es ist ein zeichen
    {
//      diacrit_count_oben  = 0;
//      diacrit_count_unten = 0;
      x  += xx;

      if(IsDiacritHindi(txt[1]) == DIACRIT_LINKS) // ist nachfolgendes Zeichen ein DIACRIT_LINKS
      {
        puf[0] = txt[1];
        OutTextXY(x, y, puf, att, col, sizeof(_UNI));
        x += DiacritOffsetHindi(txt[1], xx);
        puf[0] = txt[0];
      }

      xx  = ChrWidth(puf[0], sizeof(_UNI));

      x  += CharacterOffsetHindi(puf[0], xx); // position um sonderzeichenoffset korrigieren
      OutTextXY(x, y, puf, att, col, sizeof(_UNI));
    }

    txt += 1;
  }
}

_UWORD HindiChrWidth(_UNI chr)
{
  _UWORD xx;

//  if(IsDiacritHindi(chr) == DIACRIT_NONE) // auskommentiert, weil sonst wordwrap nit funzt
  {
    xx = ChrWidth(chr, sizeof(_UNI));
    xx += CharacterOffsetHindi(chr, xx); // position um sonderzeichenoffset korrigieren
    return(xx);
  }

//  return(0);
}

_UWORD HindiStrWidth(_UNI *txt)
{
  _UNI    puf[2];
  _UWORD  xx;
  _UWORD  diacrit;
  _UWORD  x = 0;

  txt = SunskritHindi(txt);

  puf[1] = 0;
  xx     = 0;

  while(txt[0])
  {
    puf[0] = txt[0];

    diacrit = IsDiacritHindi(puf[0]);

    if(diacrit == DIACRIT_NONE) // es ist kein diacrit
    {
      x  += xx;

      if(IsDiacritHindi(txt[1]) == DIACRIT_LINKS) // ist nachfolgendes Zeichen ein DIACRIT_LINKS
      {
        puf[0] = txt[1];
        x += DiacritOffsetHindi(txt[1], xx);
        puf[0] = txt[0];
      }

      xx = ChrWidth(puf[0], sizeof(_UNI));

      x  += CharacterOffsetHindi(puf[0], xx); // position um sonderzeichenoffset korrigieren
    }

    txt += 1;
  }

  return(x);
}
*/
// ***************************************************************************
// ***************************************************************************
// *** THAI TEXT-RENDER ENGINE ***********************************************
// ***************************************************************************

_UWORD IsDiacritThai(_UNI chr)
{
  switch(chr)
  {
    case 0x0E47:
    case 0x0E48:
    case 0x0E49:
    case 0x0E4A:
    case 0x0E4B:
    case 0x0E4C:
    case 0x0E4D:
    case 0x0E4E:
    case 0x0E31:
    case 0x0E34:
    case 0x0E35:
    case 0x0E36:
    case 0x0E37: return(DIACRIT_OBEN);
    case 0x0E38:
    case 0x0E39:
    case 0x0E3A: return(DIACRIT_UNTEN);
  }

  return(DIACRIT_NONE);
}

_WORD CharacterOffsetThai(_UNI chr, _WORD xx)
{
  if(chr == 0x0E33)
    return(-((xx*6+8)/17));

  return(0);
}

_WORD DiacritOffsetThai(_UNI chr, _WORD xx)
{
  _WORD xd;

  xd = ChrWidth(chr, sizeof(_UNI));

  switch(chr)
  {
    case 0x0E31: return((xd+1)/3); // noch weiter als rechts
    case 0x0E47:
    case 0x0E4A:
    case 0x0E4E:
    case 0x0E34:
    case 0x0E35:
    case 0x0E36:
    case 0x0E37: return((xx - xd)/2); // mittig
    case 0x0E48:
    case 0x0E49:
    case 0x0E4B:
    case 0x0E4C:
    case 0x0E4D:
    case 0x0E38:
    case 0x0E39:
    case 0x0E3A: return(xx - xd); // ganz rechts
  }

  return(0);
}

void ThaiOut(_KOORD x, _KOORD y, _UNI *txt, _ATTRIB att, _COLOR col)
{
  _UNI    puf[2];
  _UWORD  xx;
  _UWORD  diacrit_count_oben, diacrit_count_unten;
  _KOORD  y1, x1, h;
  _UWORD  diacrit;

  // h entspricht dem y-offset des nächsten diacrits, wenn mehrere über einem zeichen liegen
  h      = ((Font.GetActualHeight()*7+19)/39);
  puf[1] = 0;
  diacrit_count_oben  = 0;
  diacrit_count_unten = 0;
  xx                  = 0;

  while(txt[0])
  {
    puf[0] = txt[0];
    diacrit = IsDiacritThai(puf[0]);

    if(diacrit != DIACRIT_NONE) // es ist ein diacrit
    {
      x1 = x;
      y1 = y;

      if(diacrit == DIACRIT_OBEN)
      {
        y1 -= diacrit_count_oben * h;
        diacrit_count_oben += 1;
      }
      else
      {
        y1 += diacrit_count_unten * h;
        diacrit_count_unten += 1;
      }

      x1 += DiacritOffsetThai(puf[0], xx); // position um sonderzeichenoffset korrigieren
      OutTextXY(x1, y1, puf, att, col, sizeof(_UNI));
    }
    else  // es ist ein zeichen
    {
      diacrit_count_oben  = 0;
      diacrit_count_unten = 0;

      x  += xx;
      xx  = ChrWidth(puf[0], sizeof(_UNI));
      x  += CharacterOffsetThai(puf[0], xx); // position um sonderzeichenoffset korrigieren
      OutTextXY(x, y, puf, att, col, sizeof(_UNI));
    }

    txt += 1;
  }
}

_UWORD ThaiChrWidth(_UNI chr)
{
  _UWORD xx;

  if(IsDiacritThai(chr) == DIACRIT_NONE)
  {
    xx = ChrWidth(chr, sizeof(_UNI));
    xx += CharacterOffsetThai(chr, xx); // position um sonderzeichenoffset korrigieren
    return(xx);
  }

  return(0);
}

_UWORD ThaiStrWidth(_UNI *txt)
{
  _UWORD xx, retcode = 0;

  while(txt[0])
  {
    if(IsDiacritThai(txt[0]) == DIACRIT_NONE)
    {
      xx       = ChrWidth(txt[0], sizeof(_UNI));
      retcode += xx;
      retcode += CharacterOffsetThai(txt[0], xx); // position um sonderzeichenoffset korrigieren
    }
    txt += 1;
  }

  return(retcode);
}

// ***************************************************************************
// ***************************************************************************
// *** TEXT-RENDER ENGINE ****************************************************
// ***************************************************************************

_UDWORD SetTextRenderEngine(_UDWORD winid)
{
  _UDWORD retcode = TextRenderEngine;
  TextRenderEngine = winid;
  return(retcode);
}

_UWORD ChrWidthLse(_UNI chr, _UWORD chr_size)
{
  if((TextRenderEngine != RENDERENGINE_DEFAULT)&&(chr_size != 1))
  {
    switch(TextRenderEngine)
    {
      case RENDERENGINE_THAI  : return(ThaiChrWidth(chr)); // THAI
      case RENDERENGINE_HINDI : return(HindiChrWidth(chr, Font.GetActualHeight())); // HINDI
    }
  }

  return(ChrWidth(chr, chr_size));
}

_UWORD StrWidthLse(void *txt, _UWORD chr_size)
{
  if(txt != NULL)
  {
    if((TextRenderEngine != RENDERENGINE_DEFAULT)&&(chr_size != 1))
    {
      switch(TextRenderEngine)
      {
        case RENDERENGINE_THAI  : return(ThaiStrWidth((_UNI*)txt)); // THAI
        case RENDERENGINE_HINDI : return(HindiStrWidth((_UNI*)txt)); // HINDI
      }
    }

    return(StrWidth(txt, chr_size));
  }

  return(0);
}

void OutTextLse(_KOORD x, _KOORD y, void *txt, _ATTRIB att, _COLOR coltxt, _UWORD chr_size)
{
  _COLOR c3d, bc;
  _UWORD xx, yy;
  _KOORD y1, y2;
  _BOOL  is3d = _FALSE;

  if(txt != NULL)
  {
    if(att & T_STRIKEOUT) // 3-D Effekt
    {
      _RGB  rgb;
      _WORD r, g, b;

      is3d = _TRUE;
      att  = att & ~T_STRIKEOUT;

      Make_RGB(&rgb, coltxt);
      r = rgb.cred;
      g = rgb.cgreen;
      b = rgb.cblue;
      r = 255 - ((r + g + b + 1) / 3);
      c3d = MakeColor_Makro(INVISIBLE, MakeTrueColor(r, r, r));

/* 
      Make_RGB(&rgb, coltxt);

      r = rgb.cred;
      g = rgb.cgreen;
      b = rgb.cblue;

      r = (r+g+b+1)/3;
      if(r >= 85)
      {
        r = r - 127;
        if(r < 0) r = 0;
      }
      else
      {
        r = r + 127;
      }
      c3d = MakeColor_Makro(INVISIBLE, MakeTrueColor(r, r, r));
*/

    }

    if((TextRenderEngine != RENDERENGINE_DEFAULT)&&(chr_size != 1))
    {
	    // ggf. attribut auf linksbündig stellen
      switch(att & T_HORIZBOUND)
      {
        case T_RIGHTBOUND  : x -= StrWidthLse(txt, sizeof(_UNI))+1;     break;
        case T_CENTERBOUND : x -= (StrWidthLse(txt, sizeof(_UNI)) / 2); break;
      }
      att = (att & (~T_HORIZBOUND)) | T_LEFTBOUND;

      // ggf. hintergrund ausgeben
      bc = GetBackColor_Makro(coltxt);
      if(bc != INVISIBLE)
      {
        yy = Font.GetActualHeight();
        xx = StrWidthLse(txt, sizeof(_UNI));
        y1 = y;
        switch(att & T_VERTBOUND)
        {
          case T_MIDBOUND  : y1 = y - yy/2;   break;
          case T_DOWNBOUND : y1 = y - yy + 1; break;
        }
        y2 = y1 + yy - 1;
        Rectangle(x, y1, x+xx-1, y2, att, bc);
        coltxt = MakeColor_Makro(INVISIBLE, GetPenColor_Makro(coltxt));
      }

      switch(TextRenderEngine)
      {
        case RENDERENGINE_THAI  : if(is3d == _TRUE)
                                    ThaiOut(x+1, y+1, (_UNI*)txt, att, c3d);
                                  ThaiOut(x, y, (_UNI*)txt, att, coltxt);  return;
        case RENDERENGINE_HINDI : if(is3d == _TRUE)
                                    HindiOut(x+1, y+1, (_UNI*)txt, att, c3d);
                                  HindiOut(x, y, (_UNI*)txt, att, coltxt); return;
      }
    }

    if(is3d == _TRUE)
    {
      bc = GetBackColor_Makro(coltxt);
      if(bc != INVISIBLE) // zuerst den Hintergrund malen
      {
        switch(att & T_HORIZBOUND)
        {
          case T_RIGHTBOUND  : x -= StrWidthLse(txt, sizeof(_UNI))+1;     break;
          case T_CENTERBOUND : x -= (StrWidthLse(txt, sizeof(_UNI)) / 2); break;
        }
        att = (att & (~T_HORIZBOUND)) | T_LEFTBOUND;

        yy = Font.GetActualHeight();
        xx = StrWidthLse(txt, sizeof(_UNI));
        y1 = y;
        switch(att & T_VERTBOUND)
        {
          case T_MIDBOUND  : y1 = y - yy/2;   break;
          case T_DOWNBOUND : y1 = y - yy + 1; break;
        }
        y2 = y1 + yy - 1;
        Rectangle(x, y1, x+xx-1, y2, att, bc);
        coltxt = MakeColor_Makro(INVISIBLE, GetPenColor_Makro(coltxt));
      }

     #ifdef UC_TRANSPARENT_TEXTCOLOR
      if(GetPenColor_Makro(coltxt) != INVISIBLE) // sa29902
     #endif 
      {
       #ifdef USER_PLANATOL
        OutTextXY(x, y+1, txt, att, c3d, chr_size);
       #else
        OutTextXY(x+1, y+1, txt, att, c3d, chr_size);
       #endif
      }
    }
   
   #ifdef UC_TRANSPARENT_TEXTCOLOR
    if(GetPenColor_Makro(coltxt) != INVISIBLE) // sa29902
   #endif 
    {
      OutTextXY(x, y, txt, att, coltxt, chr_size);
    }
  }
}




