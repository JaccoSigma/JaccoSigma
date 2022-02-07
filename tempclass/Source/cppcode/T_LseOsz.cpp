#include "T_IPC.H"
#include "T_NUC.H"


_BOOL GetChannelData(_DWORD*, _DWORD*, _COLOR*, _LSEOSZI*, _THEURGE*, _UWORD);
void DrawScale(_THEURGE*, _ROOM*);


_BOOL LseOsziAddChannel(_LSEOSZI *LseOszi, _UDWORD EntryNo, _ASCII *name, _UWORD station, _DWORD YMin, _DWORD YMax, _COLOR color)
{
  _BOOL retcode = _FALSE;
  _LSEOSZI_CHANNEL *pc;

  PushMemColor(MEMORY_PRG);
  if(Memory2((void**)&LseOszi->channels.ptr, (LseOszi->channels.no + 1) * sizeof(_LSEOSZI_CHANNEL)) != 0)
  {
    pc = &LseOszi->channels.ptr[LseOszi->channels.no];
    init_LSEOSZI_CHANNEL(pc);
    if(Memory2((void**)&pc->data, EntryNo * sizeof(*pc->data)) != 0)
    {
      pc->vl.no = 1;
      pc->vl.info[0].state = VAR_VAL;
      pc->vl.info[0].value = VarList.GetVarNo(name, station);
      pc->YMin = YMin;
      pc->YMax = YMax;
      pc->color = color;
      pc->ringbuffer.no = EntryNo;
      LseOszi->channels.no++;
      retcode = _TRUE;
    }
  }
  PopMemColor();

  return(retcode);

}

_BOOL LseOsziConfigChannels(_LSEOSZI *LseOszi, _UDWORD EntryNo, _THEURGE *ptu)
{

  return(LseOsziConfigChannelsNew(LseOszi, EntryNo, _FALSE, ptu));
}

_BOOL LseOsziConfigChannelsNew(_LSEOSZI *LseOszi, _UDWORD EntryNo, _BOOL datetime, _THEURGE *ptu)
{
  _BOOL retcode = _FALSE;
  _LSEOSZI_CHANNEL *pc;
  _UWORD i, j;

  PushMemColor(MEMORY_PRG);
  if(Memory2((void**)&LseOszi->channels.ptr, OSZICHANNELS * sizeof(_LSEOSZI_CHANNEL)) != 0)
  {
    pc = LseOszi->channels.ptr;

    for(i=0,j=0; i<OSZICHANNELS; i++,j=i*3)
    {
      init_LSEOSZI_CHANNEL(pc);
      retcode = _FALSE;
      if(Memory2((void**)&pc->data, EntryNo * sizeof(*pc->data)) != 0)
      {
        retcode = _TRUE;
        LseOszi->channels.no ++;

        MemCpy(&pc->vl, &ptu->memvarlabel.ptr[j], sizeof(_VARLABEL));
        pc->color = ptu->memcolor.ptr[i];
        pc->ringbuffer.no = EntryNo;
      }
      pc++;
    }

    if((datetime == _TRUE) && (retcode == _TRUE))
    {
      retcode = _FALSE;
      if(Memory((void**)&LseOszi->channels.datim, EntryNo * sizeof(*LseOszi->channels.datim)) != 0)
      {
        LseOszi->channels.ringbuffer.no = EntryNo;
        retcode = _TRUE;
      }
    }
  }
  PopMemColor();


  return(retcode);
}

_BOOL LseOsziCyclic(_LSEOSZI *LseOszi)
{
  _BOOL retcode = _TRUE;
  _LSEOSZI_CHANNEL *pc;
  _RESULT          result;
  _UWORD           i, no;
  _VARIABLE        variable;


  no = LseOszi->channels.no;
  pc = LseOszi->channels.ptr;
  for(i=0; i<no; i++)
  {
    if(pc->vl.no != 0)
    {
//    if(get_dat(&result, pc->varno) == _TRUE)
      if(VarList.GetSystemData(&result, &variable, &pc->vl, _FALSE, _FALSE) == _TRUE)
      {
        pc->data[wpos_RINGBUFFER(&pc->ringbuffer)] = result.value;
//        hpos->no = no_RINGBUFFER(&pc->ringbuffer);
        LseOszi->change++;
      }
    }

    pc++;
  }

  if(LseOszi->channels.datim != NULL)
  {
    _DATIM datim;

    get_DATIM(&datim);
    LseOszi->channels.datim[wpos_RINGBUFFER(&LseOszi->channels.ringbuffer)] = datim;
  }

  return(retcode);
}

/*
_BOOL LseOsziInit(_LSEOSZI *LseOszi)
{
  _BOOL retcode = _FALSE;



  return(retcode);
}
*/

_DWORD LseOsziDraw(_LSEOSZI *LseOszi, _POSITION *hpos, _IO *pio, _ROOM *proom, _UWORD XZoom, _UWORD YZoom, _UWORD channel)
{

  return(LseOsziDrawNew(NULL, LseOszi, hpos, pio, proom, XZoom, YZoom, channel));
}

_DWORD LseOsziDrawNew(_DATIM *datim, _LSEOSZI *LseOszi, _POSITION *hpos, _IO *pio, _ROOM *proom, _UWORD XZoom, _UWORD YZoom, _UWORD channel)
{
  _DWORD retcode = 0;
  _KOORD width, height;
  _ROOM  room;
  _LSEOSZI_CHANNEL *pc;
  _KOORD data[1500];
  _DWORD YMin, YMax, YDiff;
  _UWORD i, k, XReduce, CurOffs;
//  _UWORD FrOffs;
  _UDWORD j, norb, index;
  _COLOR color;

  XZoom = 1000;
//  if(XZoom == 0)
//    XZoom = 1000;
  if(YZoom == 0)
    YZoom = 1000;

  pc = LseOszi->channels.ptr;

  GetTextRoom(&room, proom, pio->attrib, pio->frametype, _FALSE);
  width  = room.xy2.x - room.xy1.x + 1;
  height = room.xy2.y - room.xy1.y + 1;

//  FrOffs = get_framewidth(pio->frametype, pio->attrib);
//  width = proom->xy2.x - proom->xy1.x + 1 - (FrOffs * 2);
//  height = proom->xy2.y - proom->xy1.y + 1 - (FrOffs * 2);
//  room.xy1.x = proom->xy1.x + FrOffs;
//  room.xy1.y = proom->xy1.y + FrOffs;
//  room.xy2.x = proom->xy2.x - FrOffs;
//  room.xy2.y = proom->xy2.y - FrOffs;

  DrawButton(proom, pio->attrib, pio->frametype, pio->colback, pio->colframe);

  if(pio->theurge->info >= 2)
  {
    DrawScale(pio->theurge, &room);
  }

  for(i=0; i<LseOszi->channels.no; i++)
  {

    if(pc->vl.no != 0)
    {
      if(GetChannelData(&YMin, &YMax, &color, LseOszi, pio->theurge, i) == _TRUE)
      {
        YMin = (YMin * YZoom) / 1000;
        YMax = (YMax * YZoom) / 1000;

        YDiff = YMax - YMin;
        if(YDiff == 0)
          YDiff = 1;
        XReduce = XZoom / 1000;
        norb = no_RINGBUFFER(&pc->ringbuffer);

        j = hpos->begin;
        for(k=0; k<width&&j<norb; k++)
        {
          index = rpos_RINGBUFFER(&pc->ringbuffer, j);
          data[k] = (_KOORD)(room.xy2.y - ((pc->data[index] - YMin) * height / YDiff));
          if(data[k] > room.xy2.y)
            data[k] = room.xy2.y;
          else if(data[k] < room.xy1.y)
            data[k] = room.xy1.y;

          j += XReduce;
        }

        OutXPixelPool(room.xy1.x, data, k, 1, T_SOLID, color);
      }
    }
    pc++;
  }

  if((channel != 0xffff) && (channel < LseOszi->channels.no))
  {
    CurOffs = room.xy1.x + hpos->pos - hpos->begin;
    if(CurOffs >= room.xy2.x)
      CurOffs = room.xy2.x;
    Line(CurOffs, room.xy1.y, CurOffs, room.xy2.y, T_SOLID, pio->theurge->memdyncolor.ptr[channel].color);

    pc = &LseOszi->channels.ptr[channel];
    if(no_RINGBUFFER(&pc->ringbuffer) >= hpos->pos)
    {
      retcode = pc->data[rpos_RINGBUFFER(&pc->ringbuffer, hpos->pos)];
    }
    if((LseOszi->channels.datim != NULL) && (datim != NULL))
    {
      if(no_RINGBUFFER(&LseOszi->channels.ringbuffer) >= hpos->pos)
      {
        *datim = LseOszi->channels.datim[rpos_RINGBUFFER(&LseOszi->channels.ringbuffer, hpos->pos)];
      }
    }
  }

  return(retcode);
}

void DrawScale(_THEURGE *ptu, _ROOM *proom)
{
  _RESULT result;
  _VARIABLE variable;
  _UWORD xDiv, yDiv;
  _COLOR color;
  _UWORD i;
  _KOORD act, tmp;


  color = ptu->memdyncolor.ptr[8].color;

  if(VarList.GetSystemData(&result, &variable, &ptu->memvarlabel.ptr[24], _FALSE, _TRUE) == _TRUE)
  {
    xDiv = result.value;

    if(xDiv > 1)
    {
      tmp = proom->xy2.x - proom->xy1.x + 1;
      xDiv--;
 
      for(i=0; i<xDiv; i++)
      {
        act = (tmp * (i + 1) / (xDiv + 1)) + proom->xy1.x;
        Line(act, proom->xy1.y, act, proom->xy2.y, T_DOTTED, color);
      }
    }
  }

  if(VarList.GetSystemData(&result, &variable, &ptu->memvarlabel.ptr[25], _FALSE, _TRUE) == _TRUE)
  {
    yDiv = result.value;

    if(yDiv > 1)
    {
      tmp = proom->xy2.y - proom->xy1.y + 1;
      yDiv--;

      for(i=0; i<yDiv; i++)
      {
        act = (tmp * (i + 1) / (yDiv + 1)) + proom->xy1.y;
        Line(proom->xy1.x, act, proom->xy2.x, act, T_DOTTED, color);
      }
    }
  }

}

_BOOL GetChannelData(_DWORD *min, _DWORD *max, _COLOR *color, _LSEOSZI *p, _THEURGE *ptu, _UWORD no)
{
  _BOOL retcode = _FALSE;
  _UWORD idx;
  _VARIABLE variable;
  _RESULT result;

  idx = no * 3;
  if(ptu != NULL)
  {
    if(VarList.GetSystemData(&result, &variable, &ptu->memvarlabel.ptr[idx+1], _FALSE, _TRUE) == _TRUE)
    {
      *min = result.value;
      if(VarList.GetSystemData(&result, &variable, &ptu->memvarlabel.ptr[idx+2], _FALSE, _TRUE) == _TRUE)
      {
        *max = result.value;
        *color = ptu->memdyncolor.ptr[no].color;

        if(is_invisible(*color, T_SOLID) == _FALSE)
          retcode = _TRUE;
      }
    }

  }
  else
  {
    *min = p->channels.ptr[no].YMin;
    *max = p->channels.ptr[no].YMax;
    *color = p->channels.ptr[no].color;

    if(is_invisible(*color, T_SOLID) == _FALSE)
      retcode = _TRUE;
  }

  return(retcode);
}
