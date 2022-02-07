#include "T_IPC.H"
#include "T_NUC.H"

void init_COMBOBOXLINE(_COMBOBOXLINE *p)
{
  p->index = DEFSCOPE;
  p->value = -12345;
}

void free_COMBOBOXLINE(_COMBOBOXLINE *p)
{
  init_COMBOBOXLINE(p);
}

void init_COMBOBOX(_COMBOBOX *p)
{
  p->TextPuffer.Init();
  p->no              = 0;
  p->ptr             = NULL;
  p->color           = MakeColor(LIGHTGRAY, BLACK);
  p->attrib          = T_LEFTBOUND | T_MIDBOUND;
  init_COMBOINFO(&p->info);
  p->info.frame      = _3DFRAME;
}

void free_COMBOBOX(_COMBOBOX *p)
{
  for(_UDWORD i=0; i<p->no; i++)
    free_COMBOBOXLINE(&p->ptr[i]);
  
  Memory2((void**)&p->ptr, 0);
  p->TextPuffer.Free();
  
  free_COMBOINFO(&p->info);
  init_COMBOBOX(p);
}

_COMBOBOX *Make_COMBOBOX(_COLOR colback, _COLOR coltext, _UDWORD lineheight, _ATTRIB alignment, _FRAMETYPE frame)
{
  _COMBOBOX *retcode = NULL;
  if(Memory2((void**)&retcode, sizeof(_COMBOBOX)) != 0)
  {
    init_COMBOBOX(retcode);
    
    retcode->info.frame      = frame;
    retcode->info.lineheight = lineheight;
    retcode->attrib      = alignment;
    retcode->color       = MakeColor(GetPenColor(colback), GetPenColor(coltext));
  }
  return retcode;
}

_BOOL Add_COMBOBOX(_COMBOBOX *p, _DWORD value, void *txt, _UDWORD chr_size)
{
  _COMBOBOXLINE *pcl;
  if(p != NULL)
  {
    if(Memory((void**)&p->ptr, (p->no + 1) * sizeof(_COMBOBOXLINE)) != 0)
    {
      pcl = &p->ptr[p->no];
      init_COMBOBOXLINE(pcl);
      pcl->index = p->TextPuffer.Add(txt, chr_size);
      if(pcl->index != DEFSCOPE)
      {
        p->no += 1;
        pcl->value = value;
        return _TRUE;
      }
    }
  }
  return _FALSE;
}

_CHAR *GetText_COMBOBOX(_COMBOBOX *p, _DWORD value)
{
  if(p != NULL)
  {
    _COMBOBOXLINE *pcl = p->ptr;
    for(_UDWORD i=0; i<p->no; i++, pcl++)
    {
      if(pcl->value == value)
      {
        return p->TextPuffer.GetText(pcl->index);
      }
    }
  }
  
  return NULL;
}

_BOOL DoEditorNeeds_COMBOBOX(_INPUT *pi)
{
  _COMBOBOX *pcb = pi->io.info.pcombo;
  if((pcb != NULL) && (pi->io.info.tbo.list == MENULIST))
  {
    _UDWORD cindex = MenuList.MakeComboBoxMenu(pcb);
    if(cindex != DEFSCOPE)
    {
      _WORD xx = pi->io.space.xy2.x - pi->io.space.xy1.x + 1;  // wunsch lisec+kaml, 21.11.2015
      if(xx < (LEVELBUTTON_WIDTH + 5))
      {
        xx = pi->io.room.xy2.x - pi->io.room.xy1.x + 1;
      }
      MenuList.DesignMenu(cindex, xx, pi->io.font, pcb->attrib, &pcb->info);
      
      pi->io.info.tbo.no = cindex;
    }
  }
  
  return _TRUE;
}






