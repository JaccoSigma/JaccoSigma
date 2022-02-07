#include "T_IPC.H"
#include "T_NUC.H"

_BOOL get_slurry(_PIC_COMMAND cmd)
{
  _BOOL retcode = _FALSE;

  switch(cmd)
  {
    case CMD_NEWPICRET      : // open new (previous) picture (0)
    case CMD_EXIT           : // exit window without backgroundrefresh (0)
    case CMD_CLOSE          : // close picture or window (0)
    case CMD_EDITNO         : // open editor with input[no] (2)
    case CMD_REDRAW         : // redraw picture or window (0)
    case CMD_OUTSOFTKEY     : // redraw softkey (0)
    case CMD_NEWPICTURE     : // open new picture (2)
    case CMD_NEWTOUCHEDITOR :
    case CMD_NEWWINDOW      : // open new window (2)
    case CMD_NEWSOFTKEY     : // change to defined floor and draw (1)
    case CMD_NEWLANGUAGE    : // change to other language (1)
    case CMD_EDIT           : // seek input and open editor (1)
    case CMD_RETURN         : // return to previous (0)
    case CMD_BEEP           : // signal beep (1)
    case CMD_EXITPROGRAM    : // exit application
    case CMD_NEWPOPUP       : // open popupmenu
    case CMD_CALL_ALARM     : // open alarmwindow
    case CMD_REDRAWTEXT     : // redraw all text and io's
    case CMD_SCRNSCHEME     : // open screen via scheme
    case CMD_DIAGNOSTIC     : // open diagnostic window (0)
    case CMD_SCREENSAVE     : // open screensaver (0)
    case CMD_EDITNO_HID     : // open editor with input[no] and open HID-window (2)
    case CMD_NEXTPICTURE    : // open next picture
    case CMD_USERPOPUP      : // open userdefined popupmenu
    case CMD_IOPOPUP_HID    : // open popupmenu for input with hid
    case CMD_IOPOPUP        : // open popupmenu for input
    case CMD_FIRSTDRAW      : // first time draw picture (0)
    case CMD_RETURN_TO_SIGN : // return to sign, previous (dumpling) (0)
    case CMD_REEDIT         : // open editor for a second time
    case CMD_OPENMINMAX     : // open minmaxwindow (bubble-help)
    case CMD_CLOSEMINMAX    : retcode = _TRUE; // close minmaxwindow (bubble-help)
    default                 : break;
  }
  return(retcode);
}

_BOOL slurry(_PIC_COMMAND cmd)
{
  _Window *Pw;
  _BOOL   retcode = _FALSE;

  Pw = Root.FindTop();
  if(Pw->GetWhoAmI().ima == IMA_SCREENSAVER)
  {
    if(Pw->GetReady() == _TRUE)
      retcode = get_slurry(cmd);
  }

  return(retcode);
}

void _Stack::Free(void)
{
  free_STACK(&Stack);
  Init();
}

void _Stack::Init(void)
{
  init_STACK_Makro(&Stack);
}

_BOOL _Stack::MoveStackWithoutCompromise(_STACK *ps)
{
  _UDWORD size;
  _BOOL   retcode; // = _FALSE;

  Mutex(_TRUE, MUTEX_STACK);
  {
    retcode = _TRUE;

    if(ps != NULL)
    {
      if((ps->no > 0)&&(ps != &Stack))
      {
        retcode = _FALSE;
        size    = (Stack.no + ps->no) * sizeof(_STACK_LINE);
        if(Memory2((void**)&Stack.ptr, size) != 0)
        {
          MemCpy(&Stack.ptr[Stack.no], ps->ptr, ps->no * sizeof(_STACK_LINE));
          Stack.no += ps->no;

          Memory2((void**)&ps->ptr, 0);
          ps->no = 0;
          retcode = _TRUE;
        }
      }
    }
  }
  Mutex(_FALSE, MUTEX_STACK);

  return(retcode);
}

void _Stack::DeleteAllCmd(void)
{
  Mutex(_TRUE, MUTEX_STACK);
  Stack.no = 0;
  Mutex(_FALSE, MUTEX_STACK);
}

_BOOL _Stack::InsCmd(_PIC_COMMAND cmd, _VARLABEL *op1, _VARLABEL *op2, _BOOL lrm)
{
  _UDWORD     size;
  _BOOL       retcode = _FALSE;
  _STACK_LINE *sl;

  Mutex(_TRUE, MUTEX_STACK);
  {
    if(slurry(cmd) == _TRUE)
      Root.FindTop()->InsertCmd(CMD_RETURNQUEUE, 0, 0, lrm);

    size = (Stack.no + 1) * sizeof(_STACK_LINE);
    if(Memory2((void**)&Stack.ptr, size) != 0)
    {
      sl = &Stack.ptr[Stack.no];
      init_STACK_LINE_Makro(sl);
      sl->cmd = cmd;
      if(op1 != NULL)
        sl->op1 = *op1;
      if(op2 != NULL)
        sl->op2 = *op2;
      sl->lrm = lrm;
      Stack.no ++;
      retcode = _TRUE;
    }
  }
  Mutex(_FALSE, MUTEX_STACK);

  return(retcode);
}

_BOOL _Stack::InsertCmd(_PIC_COMMAND cmd, _DWORD v1, _DWORD v2, _BOOL lrm)
{
  _VARLABEL op1, op2;

  init_VARLABEL_Makro(&op1);
  init_VARLABEL_Makro(&op2);

  op1.no = 1;
  op1.info[0].state = CONST_VAL;
  op1.info[0].value = v1;

  op2.no = 1;
  op2.info[0].state = CONST_VAL;
  op2.info[0].value = v2;

  return(InsCmd(cmd, &op1, &op2, lrm));
}

_BOOL _Stack::GetCommand(_STACK_LINE *p)
{
  _UDWORD size;
  _BOOL   retcode = _FALSE;
  _UWORD  end, i, old;

  if(Stack.no != 0)
  {
    Mutex(_TRUE, MUTEX_STACK);
      
   START_GETCOMMAND:
      
    if(DoIfElseEnd() == _TRUE)
    {
      *p = Stack.ptr[0];
      Stack.no --;
      size = Stack.no * sizeof(_STACK_LINE);
      if(size > 0)
        MemMove(&Stack.ptr[0], &Stack.ptr[1], size);

      init_STACK_LINE_Makro(&Stack.ptr[Stack.no]);
      retcode = _TRUE;
    }
    else
    {
      old = Stack.no;
      end = LinesToEnd(&Stack.ptr[0], Stack.no) + 1;
      Stack.no -= end;
      size = Stack.no * sizeof(_STACK_LINE);
      if(size > 0)
        MemMove(&Stack.ptr[0], &Stack.ptr[end], size);

      for(i=Stack.no; i<old; i++)
        init_STACK_LINE_Makro(&Stack.ptr[i]);

      if(Stack.no != 0)
        goto START_GETCOMMAND;
    }
      
    Mutex(_FALSE, MUTEX_STACK);
  }

//  if(retcode == _FALSE)
//    init_STACK_LINE_Makro(p);

  return(retcode);
}

_BOOL _Stack::Read(_File *File)
{
  // [][] ... no of interpreterlines
  //  [][] .. command
  //  [ ..... _VARLABEL, _VARLABEL (optional)

  _BOOL       retcode = _FALSE;
  _UWORD      tmp, i;
  _UDWORD     size;
  _STACK_LINE *p, *po, tmp_line;

  Free();

  Mutex(_TRUE, MUTEX_STACK);
  {
    if(File->GetError() == _FALSE)
    {
      if(File->Read(&tmp, sizeof(_UWORD)) == _TRUE)
      {
        size = tmp * sizeof(_STACK_LINE);
        if(File->FileMemory((void**)&Stack.ptr, size) == _TRUE)
        {
          Stack.no = tmp;
          p        = Stack.ptr;
          po       = NULL;
          for(i=0, retcode=_TRUE; i<Stack.no; i++)
          {
            retcode = File->Read_STACK_LINE(p);
            // folgender programmteil dient compatibilität zu alten porgrammen
            // befehlssequenz RETURN und SAVE_EDITOR austauschen (fa.berstorff!!)
            if(po != NULL)
            {
              if((po->cmd == CMD_RETURN)&&(p->cmd == CMD_SAVEEDITOR))
              {
                tmp_line = *po;
                *po      = *p;
                *p       = tmp_line;
              }
            }
            po = p;
            p ++;
          }
        }
      }

      if(retcode == _FALSE)
        Free();
    }
  }
  Mutex(_FALSE, MUTEX_STACK);

  return(retcode);
}

void _Stack::AddLine(_STACK_LINE *pl, _BOOL lrm)
{
  _VARLABEL vl;

  switch(pl->cmd)
  {
    case CMD_RETURN      :
    case CMD_SAVEEDITOR  :
    case CMD_KILLEDITOR  : init_VARLABEL_Makro(&vl);
                           InsCmd(pl->cmd, &vl, &vl, lrm);
                           break;
    case CMD_NOP         : break;
    default              : InsCmd(pl->cmd, &pl->op1, &pl->op2, lrm);
                           break;
  }
}

_BOOL _Stack::AddStack(_Stack *Src, _BOOL lrm)
{
  _BOOL retcode = _TRUE;

  if(Src != NULL)
    retcode = AddPart(Src->GetPtr(), Src->GetNo(), lrm);

  return(retcode);
}

_UWORD _Stack::LinesToElse(_STACK_LINE *pl, _UWORD anz)
{
  _UWORD i, cnt=0, scope=0;

  for(i=0; i<anz; i++)
  {
    switch(pl[i].cmd)
    {
      case CMD_IFG  :
      case CMD_IFE  :
      case CMD_IFS  : scope ++;
                      break;
      case CMD_END  : scope --;
                      if(scope == 0)
                        return(0);
                      break;
      case CMD_ELSE : if(scope == 1)
                        return(cnt);
      default       : break;
    }
    cnt ++;
  }

  return(0);
}

_UWORD _Stack::LinesToEnd(_STACK_LINE *pl, _UWORD anz)
{
  _UWORD i, cnt=0, scope=0;

  for(i=0; i<anz; i++)
  {
    switch(pl[i].cmd)
    {
      case CMD_IFG  :
      case CMD_IFE  :
      case CMD_IFS  : scope ++;
                      break;
      case CMD_END  : scope --;
                      if(scope == 0)
                        return(cnt);
                      break;
      default       : break;
    }
    cnt ++;
  }

  return(anz-1);
}

_BOOL _Stack::AddPart(_STACK_LINE *pl, _UWORD anz, _BOOL lrm)
{
  _BOOL retcode = _TRUE;

  if(pl != NULL)
  {
    while(anz > 0)
    {
      AddLine(pl, lrm);
      pl  ++;
      anz --;
    }
  }

  return(retcode);
}

_BOOL _Stack::Add_LSEFUNCT(_LSEFUNCT *p, _BOOL lrm)
{

  if(p->no < InterBoxList.GetNo())
  {
    return(AddStack(InterBoxList.Get(p->no), lrm));
  }

  return(AddPart(p->overload.ptr, p->overload.no, lrm));
}

_BOOL _Stack::DoIfElseEnd(void)
{
  _RESULT     op1, op2;
  _VARIABLE   var;
  _BOOL       retcode = _TRUE;
  _UWORD      i, els, end;
  _BOOL       st;
  _STACK_LINE *pl;


  if(Stack.no > 0)
  {
    switch(Stack.ptr[0].cmd)
    {
      case CMD_IFS :
      case CMD_IFG :
      case CMD_IFE : pl = Stack.ptr;
        if((VarList.GetSystemData(&op1, &var, &pl->op1, _TRUE, _TRUE) == _TRUE)&&
           (VarList.GetSystemData(&op2, &var, &pl->op2, _TRUE, _TRUE) == _TRUE))
        {
          els = LinesToElse(pl, Stack.no);
          end = LinesToEnd(pl, Stack.no);

          switch(pl->cmd)
          {
            case CMD_IFE : st = (op1.value == op2.value)? _TRUE : _FALSE; break;
            case CMD_IFG : st = (op1.value > op2.value)? _TRUE : _FALSE; break;
            default      : st = (op1.value < op2.value)? _TRUE : _FALSE; break;
          }

          free_STACK_LINE(Stack.ptr);

          if(st == _TRUE) // delete else part
          {
            if(els != 0)
            {
              for(i=els; i<end; i++)
                free_STACK_LINE(&Stack.ptr[i]);
            }
          }
          else // delete if part
          {
            if(els == 0)
               els = end;

            for(i=0; i<els; i++)
              free_STACK_LINE(&Stack.ptr[i]);

          }

        }
        else
        {
          retcode = _FALSE;
        }

        break;

      default :
        break;
    }
  }

  return(retcode);
}

_UWORD _Stack::GetNo(void)
{
  return(Stack.no);
}

_STACK_LINE *_Stack::GetPtr(void)
{
  return(Stack.ptr);
}

_BOOL _Stack::LookUpCommand(_PIC_COMMAND cmd)
{
  return(lookup_STACK(&Stack, cmd));
}

_STACK *_Stack::GetStack(void)
{
  return(&Stack);
}

_BOOL _Stack::InsertStack(_Stack *p, _BOOL lrm)
{
  _BOOL  retcode = _TRUE;
  _Stack TmpStack;

  if(p != NULL)
  {
    if(p->GetNo() > 0)
    {
      TmpStack.Init();
      if(TmpStack.AddStack(p, lrm) == _TRUE)
      {
        if(merge_STACK(TmpStack.GetStack(), &Stack) == _TRUE)
        {
          free_STACK(&Stack);
          Stack   = *TmpStack.GetStack();
          init_STACK_Makro(TmpStack.GetStack());
          retcode = _TRUE;
        }
      }
      TmpStack.Free();
    }
  }

  return(retcode);
}


#ifdef XUSER_KM
#define TMPSTACKSIZE 200
_BOOL _Stack::CopyJustScreenToStack(_STACK_LINE *pdst, _UWORD *panz, _Stack *Src)
{
  _BOOL       retcode = _TRUE;
  _UWORD      no;
  _STACK_LINE *ps;
  _UWORD      anz = *panz;
  _IVALUE     value;

  no = Src->GetNo();
  ps = Src->GetPtr();

  while(no > 0)
  {
    if(anz < TMPSTACKSIZE)
    {
      switch(ps->cmd)
      {
        case CMD_SCRNSCHEME    : // open screen via scheme
        case CMD_NEWPICTURE    : // open new picture (2)
        case CMD_IFE           : // if ==
        case CMD_IFG           : // if >
        case CMD_IFS           : // if <
        case CMD_ELSE          : // else
        case CMD_END           : // end
                                 pdst[anz] = *ps;
                                 anz += 1;
                                 break;
        case CMD_FUNCTIONBLOCK : // call functionblock (1)
                                 if(VarList.GetValue(&ps->op1, &value, _TRUE) == _TRUE)
                                   retcode = CopyJustScreenToStack(pdst, &anz, InterBoxList.Get(value));
                                 break;
      }
    }
    else
    {
      retcode = _FALSE;
    }

    ps ++;
    no --;

    if(retcode == _FALSE)
      no = 0;

  }

  *panz = anz;

  return(retcode);
}

void _Stack::ScanForIf(void)
{
  _STACK tmp = Stack;

  while(Stack.no > 0)
  {
    switch(Stack.ptr[0].cmd)
    {
      case CMD_IFS :
      case CMD_IFG :
      case CMD_IFE : DoIfElseEnd();
    }

    Stack.ptr += 1;
    Stack.no  -= 1;
  }

  Stack = tmp;
}

_BOOL _Stack::GetScreenNo(_Stack *Src, _UWORD *screenno, _UWORD *softfloor)
{
  _STACK_LINE tmp[TMPSTACKSIZE], *pl;
  _UWORD      no = 0;
  _IVALUE     value;
  _MYSCHEME   myscheme;
  _UDWORD     shm;

  *softfloor = 0;
  *screenno  = 0xFFFF;

  if(Src != NULL)
  {
    if(CopyJustScreenToStack(tmp, &no, Src) == _TRUE)
    {
      Stack.ptr = tmp,
      Stack.no  = no;
      ScanForIf();
//      DoIfElseEnd();
      init_STACK_Makro(&Stack);

      pl = tmp;
      while(no)
      {
        switch(pl->cmd)
        {
          case CMD_SCRNSCHEME : // open screen via scheme
                                if(VarList.GetValue(&pl->op1, &value, _TRUE) == _TRUE)
                                {
                                  init_MYSCHEME_Makro(&myscheme);
                                  myscheme.schemeno = value;
                                  if(ScreenScheme.Call(&shm, &myscheme, NULL, 0xFFFF) == _TRUE)
                                  {
                                    if(shm != 0xFFFF)
                                    {
                                      *screenno = shm;
                                      return(_TRUE);
                                    }
                                  }
                                }
                                break;

          case CMD_NEWPICTURE : // open new picture (2)
                                if(VarList.GetValue(&pl->op1, &value, _TRUE) == _TRUE)
                                {
                                  *screenno = value;
                                  if(VarList.GetValue(&pl->op2, &value, _TRUE) == _TRUE)
                                    *softfloor = value;
                                  return(_TRUE);
                                }
                                break;
        }
        no --;
        pl ++;
      }
    }
  }

  return(_FALSE);
}
#endif

_BOOL _Stack::Overload(_VARLABEL *po)
{
  return overload_STACK(&Stack, po);
}


