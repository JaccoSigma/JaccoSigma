//This file was generated by the LASAL2 CodeGenerator  -- 
//Please, do not edit this file (it might be overwritten by the next generator run)
#RE_IN_DE
#pragma define (store)
#if _LSL_COMPILERVERSION >= 42
DSComm_Interface : CLASS;
HwcLogging : CLASS;
SigCLib : CLASS;
#endif
_SerLib : CLASS_PUBLIC
	TYPE
	  CmdCommand :
	  (
	    StartUserIPR,
	    StopUserIPR,
	    SerCloseIPR,
	    SerSendIPR,
	    RecvCharIPR,
	    SetOnlineIPR,
	    GetRecvStateIPR,
	    GetSendStateIPR,
	    GetErrorIPR,
	    IsInitializedIPR,
	    ClearRecBufferIPR,
	    RtsOnOffIPR,
	    rdRtsIPR,
	    rdCtsIPR,
	    DtrOnOffIPR,
	    rdDtrIPR,
	    rdDcdIPR,
	    rdDsrIPR,
	    rdRiIPR,
	    SetRSModeIPR,
	    GetRSModeIPR,
	    GetInterfaceTypeIPR
	  )$UINT;
	END_TYPE
END_CLASS;
#pragma define (restore)
checkDirectory : CLASS_PUBLIC
	TYPE
	  taskcreation :
	  (
	    sigcl_thread_create2:=1,
	    multitask_createthread:=2
	  )$UDINT;
	END_TYPE
END_CLASS;
#pragma define (restore)
#pragma InclDefBlk Class_Names
Class_Names : CLASS_PUBLIC
	TYPE
	  Names : ARRAY [0..100] OF CHAR;
	END_TYPE
END_CLASS;
#pragma define (restore)
DSComm : CLASS_PUBLIC
	TYPE
	  _DS_AccessType :
	  (
	    VarClass,
	    VarServerNumeric,
	    VarServerString,
	    VarClient,
	    VarVariableDataAt,
	    VarConstValue
	  )$UDINT;
#pragma pack(push, 1)
	  _DS_AddLocalClientCmd : STRUCT
	    Typ : USINT;
	    pIdentifier : ^USINT;
	    ID : UDINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_AddLocalClientRangeCmd : STRUCT
	    Typ : USINT;
	    onDuplicate : USINT;
	    pFrom : ^USINT;
	    pTo : ^USINT;
	    ID : UDINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_AddLocalClientRangeResp : STRUCT
	    ID : UDINT;
	    ErrorCode : UDINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_AddLocalClientResp : STRUCT
	    ID : UDINT;
	    ErrorCode : UDINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_AlarmDeleteAllResp : STRUCT
	    ErrorCode : UDINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_AlarmDeleteResp : STRUCT
	    ErrorCode : UDINT;
	    AlarmID : UDINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_AlarmDeleteUpdate : STRUCT
	    AlarmID : UDINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_AlarmParameter : STRUCT
	    Value : DINT;
	    VarID : UDINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_AlarmTimestamps : STRUCT
	    ts_firstTime : UDINT;
	    ts_firstDate : UDINT;
	    ts_lastTime : UDINT;
	    ts_lastDate : UDINT;
	    ts_quittedTime : UDINT;
	    ts_quittedDate : UDINT;
	    ts_disappearedTime : UDINT;
	    ts_disappearedDate : UDINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_AlarmListUpdate : STRUCT
	    VarID : UDINT;
	    Group : UDINT;
	    Config : UDINT;
	    State : USINT;
	    GroupBit : USINT;
	    ParameterNo : USINT;
	    nc : USINT;
	    pParameter : ^_DS_AlarmParameter;
	    pTimestamps : ^_DS_AlarmTimestamps;
	    num_appeared : UDINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_AlarmListUpdateAddResp : STRUCT
	    ErrorCode : UDINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_AlarmListUpdateSubResp : STRUCT
	    ErrorCode : UDINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_CancelResp : STRUCT
	    HandleID : UDINT;
	    State : UDINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_UnsLongLongInt : STRUCT
	    Low32 : UDINT;
	    High32 : UDINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_ChangeRoleCmd : STRUCT
	    roleID : UDINT;
	    pRoleName : ^UINT;
	    pInfo : ^UINT;
	    pWriteAccessString : ^UINT;
	    pReadAccessString : ^UINT;
	    privileges : _DS_UnsLongLongInt;
	    noOperationTimeout : UINT;
	    AccessNumberValid : USINT;
	    AccessNumber : UDINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_ChangeRoleResp : STRUCT
	    roleID : UDINT;
	    ErrorCode : UDINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_ChangeUserSettingsCmd : STRUCT
	    UserID : UDINT;
	    pName : ^UINT;
	    pPassword : ^UINT;
	    pFirstName : ^UINT;
	    pLastName : ^UINT;
	    numRoles : UDINT;
	    pRoleID : ^UDINT;
	    pInfo : ^UINT;
	    AccessNumberValid : USINT;
	    AccessNumber : UDINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_ChangeUserSettingsResp : STRUCT
	    UserID : UDINT;
	    ErrorCode : UDINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_CheckAccessLevelCmd : STRUCT
	    UserID : UDINT;
	    AccessLevel : UINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_CheckAccessLevelResp : STRUCT
	    UserID : UDINT;
	    AccessLevel : UINT;
	    ErrorCode : UDINT;
	    writeAllowed : USINT;
	    readAllowed : USINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_ClearLocalClientListResp : STRUCT
	    ErrorCode : UDINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_CreateRoleCmd : STRUCT
	    pRoleName : ^UINT;
	    pWriteAccessString : ^UINT;
	    pReadAccessString : ^UINT;
	    privileges : _DS_UnsLongLongInt;
	    noOperationTimeout : UINT;
	    pInfo : ^UINT;
	    AccessNumberValid : USINT;
	    AccessNumber : UDINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_CreateRoleResp : STRUCT
	    RoleID : UDINT;
	    ErrorCode : UDINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_CreateUserCmd : STRUCT
	    pName : ^UINT;
	    pPassword : ^UINT;
	    pFirstName : ^UINT;
	    pLastName : ^UINT;
	    numRoles : UDINT;
	    pRoleID : ^UDINT;
	    pInfo : ^UINT;
	    AccessNumberValid : USINT;
	    AccessNumber : UDINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_CreateUserResp : STRUCT
	    UserID : UDINT;
	    ErrorCode : UDINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_DeleteRoleResp : STRUCT
	    RoleID : UDINT;
	    ErrorCode : UDINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_DeleteUserResp : STRUCT
	    UserID : UDINT;
	    ErrorCode : UDINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_DirCreateResp : STRUCT
	    ErrorCode : UDINT;
	    pDp : ^CHAR;
	    DpLen : UDINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_DirReadResp : STRUCT
	    Attribute : UDINT;
	    Length : _DS_UnsLongLongInt;
	    AccessDate : UDINT;
	    AccessTime : UDINT;
	    ModifiedDate : UDINT;
	    ModifiedTime : UDINT;
	    CreatedDate : UDINT;
	    CreatedTime : UDINT;
	    pNe : ^CHAR;
	    NeLen : UDINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_EventjournalAddCmd : STRUCT
	    Size : UDINT;
	    ID : UINT;
	    nc : UINT;
	    EvtTime : UDINT;
	    EvtDate : UDINT;
	    SessionID : UDINT;
	    pData : ^CHAR;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_EventjournalAddResp : STRUCT
	    ErrorCode : UDINT;
	    CRC : UDINT;
	    idxcnt : UDINT;
	    valid : UINT;
	    DataLen : UINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_EventjournalDelAllResp : STRUCT
	    ErrorCode : UDINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_EventjournalDelResp : STRUCT
	    ErrorCode : UDINT;
	    JournalID : UDINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_EventjournalGetResp : STRUCT
	    CRC : UDINT;
	    idxcnt : UDINT;
	    valid : UINT;
	    DataLen : UINT;
	    ID : UINT;
	    nc : UINT;
	    EvtTime : UDINT;
	    EvtDate : UDINT;
	    SessionID : UDINT;
	    pData : ^CHAR;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_EventjournalQuitAlarmResp : STRUCT
	    ErrorCode : UDINT;
	    AlarmID : UDINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_EventjournalQuitAllResp : STRUCT
	    ErrorCode : UDINT;
	    StationID : UDINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_FileCloseResp : STRUCT
	    handle : UDINT;
	    ErrorCode : UDINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_FileContentResp : STRUCT
	    ErrorCode : UDINT;
	    paketOffset : _DS_UnsLongLongInt;
	    fileLength : _DS_UnsLongLongInt;
	    dataLength : UDINT;
	    pData : ^CHAR;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_FileCopyResp : STRUCT
	    ErrorCode : UDINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_FileGetResp : STRUCT
	    handle : UDINT;
	    ErrorCode : UDINT;
	    length : UDINT;
	    pData : ^CHAR;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_FileInfoResp : STRUCT
	    ErrorCode : UDINT;
	    Attribute : UDINT;
	    Length : _DS_UnsLongLongInt;
	    AccessDate : UDINT;
	    AccessTime : UDINT;
	    ModifiedDate : UDINT;
	    ModifiedTime : UDINT;
	    CreatedDate : UDINT;
	    CreatedTime : UDINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_FileLengthResp : STRUCT
	    handle : UDINT;
	    ErrorCode : UDINT;
	    length : _DS_UnsLongLongInt;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_FileMoveResp : STRUCT
	    ErrorCode : UDINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_FileOpenResp : STRUCT
	    handle : UDINT;
	    ErrorCode : UDINT;
	    pDpne : ^CHAR;
	    DpneLen : UDINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_FilePutResp : STRUCT
	    handle : UDINT;
	    ErrorCode : UDINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_FileReadResp : STRUCT
	    handle : UDINT;
	    ErrorCode : UDINT;
	    length : UDINT;
	    pData : ^CHAR;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_FileRemoveResp : STRUCT
	    ErrorCode : UDINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_FileRenameResp : STRUCT
	    ErrorCode : UDINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_FileSeekResp : STRUCT
	    handle : UDINT;
	    ErrorCode : UDINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_FileSetAttributeResp : STRUCT
	    ErrorCode : UDINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_FileTellResp : STRUCT
	    handle : UDINT;
	    ErrorCode : UDINT;
	    offset : UDINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_FileWriteResp : STRUCT
	    handle : UDINT;
	    ErrorCode : UDINT;
	    length : UDINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_GetCRCFromVarIdResp : STRUCT
	    VarID : UDINT;
	    ErrorCode : UDINT;
	    CRC : UDINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_GetCurrentIDResp : STRUCT
	    ID : UDINT;
	    ErrorCode : UDINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_GetDpInfoResp : STRUCT
	    VarID : UDINT;
	    ErrorCode : UDINT;
	    LasalID : UDINT;
	    pName : ^CHAR;
	    NameLen : UDINT;
	    Station : UDINT;
	    Scope : UDINT;
	    AccNo : UINT;
	    LimitLowTyp : UINT;
	    LimitLow : LREAL;
	    LimitHighTyp : UINT;
	    LimitHigh : LREAL;
	    RefTime : UDINT;
	    UserDefBits : _DS_UnsLongLongInt;
	    AccessType : _DS_AccessType;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_GetDpListResp : STRUCT
	    VarID : UDINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_GetDriveListResp : STRUCT
	    pDrives : ^CHAR;
	    DrivesLen : UDINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_GetLasalIdFromNameResp : STRUCT
	    pName : ^CHAR;
	    NameLen : UDINT;
	    ErrorCode : UDINT;
	    LasalId : UDINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_GetLasalIdIdFromVarIdResp : STRUCT
	    VarID : UDINT;
	    ErrorCode : UDINT;
	    LasalID : UDINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_GetLocalClientByIDCmd : STRUCT
	    ID : UDINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_GetLocalClientDetailsResp : STRUCT
	    ID : UDINT;
	    ErrorCode : UDINT;
	    Typ : USINT;
	    onDuplicate : USINT;
	    pFrom : ^USINT;
	    FromLen : UDINT;
	    pTo : ^USINT;
	    ToLen : UDINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_GetLocalClientListResp : STRUCT
	    ID : UDINT;
	    ErrorCode : UDINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_GetNameFromLasalIdCmd : STRUCT
	    LasalID : UDINT;
	    StationNo : UDINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_GetNameFromLasalIdResp : STRUCT
	    LasalID : UDINT;
	    StationNo : DINT;
	    ErrorCode : UDINT;
	    pName : ^CHAR;
	    NameLen : UDINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_GetNameFromVarIdResp : STRUCT
	    VarID : UDINT;
	    ErrorCode : UDINT;
	    pName : ^CHAR;
	    NameLen : UDINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_GetProjectInfoResp : STRUCT
	    VersionNo : UDINT;
	    BuildNo : UDINT;
	    ProjectCRC : UDINT;
	    pProjectName : ^UINT;
	    ProjNameLen : UDINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_GetProjectNameResp : STRUCT
	    pProjectName : ^UINT;
	    ProjNameLen : UDINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_GetRoleDetailsResp : STRUCT
	    RoleID : UDINT;
	    ErrorCode : UDINT;
	    pName : ^UINT;
	    NameLen : UDINT;
	    pWriteAccessString : ^UINT;
	    WriteAccessStrLen : UDINT;
	    pReadAccessString : ^UINT;
	    ReadAccessStrLen : UDINT;
	    pInfo : ^UINT;
	    InfoLen : UDINT;
	    privileges : _DS_UnsLongLongInt;
	    noOperationTimeout : UINT;
	    CreatedDate : UDINT;
	    CreatedTime : UDINT;
	    ModifiedRoleDate : UDINT;
	    ModifiedRoleTime : UDINT;
	    WriteProtected : USINT;
	    AccessNumberValid : USINT;
	    AccessNumber : UDINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_GetRoleListResp : STRUCT
	    RoleID : UDINT;
	    pName : ^UINT;
	    NameLen : UDINT;
	  END_STRUCT;
#pragma pack(pop)
	  _DS_StateType :
	  (
	    Lasal_ID_List:=1
	  )$UDINT;
#pragma pack(push, 1)
	  _DS_GetStateCmd : STRUCT
	    Target : UDINT;
	    Typ : _DS_StateType;
	    pData : pVoid;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_GetStateCmd_1 : STRUCT
	    StationNo : UDINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_GetStateResp : STRUCT
	    Target : UDINT;
	    Typ : _DS_StateType;
	    ErrorCode : UDINT;
	    pData : pVoid;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_GetStringCmd : STRUCT
	    dpID : UDINT;
	    quality : UDINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_GetStringResp : STRUCT
	    dpID : UDINT;
	    ErrorCode : UDINT;
	    quality : UDINT;
	    pString : ^UINT;
	    StringLen : UDINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_GetUserListResp : STRUCT
	    UserID : UDINT;
	    pName : ^UINT;
	    NameLen : UDINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_GetValueCmd : STRUCT
	    dpID : UDINT;
	    quality : UDINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_GetValueResp : STRUCT
	    dpID : UDINT;
	    ErrorCode : UDINT;
	    quality : UDINT;
	    value : LREAL;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_GetVarIdFromCRCResp : STRUCT
	    CRC : UDINT;
	    ErrorCode : UDINT;
	    VarID : UDINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_GetVarIdFromLasalIdCmd : STRUCT
	    LasalID : UDINT;
	    StationNo : UDINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_GetVarIdFromLasalIdResp : STRUCT
	    LasalID : UDINT;
	    StationNo : UDINT;
	    ErrorCode : UDINT;
	    VarID : UDINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_GetVarIdFromNameResp : STRUCT
	    pName : ^CHAR;
	    NameLen : UDINT;
	    ErrorCode : UDINT;
	    VarID : UDINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_GetVersionResp : STRUCT
	    VersionNo : UDINT;
	    BuildNo : UDINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_HotAlarmAddCmd : STRUCT
	    Typ : USINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_HotAlarmAddResp : STRUCT
	    ErrorCode : UDINT;
	    Typ : USINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_HotAlarmSubCmd : STRUCT
	    Typ : USINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_HotAlarmSubResp : STRUCT
	    ErrorCode : UDINT;
	    Typ : USINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_HotAlarmUpdate : STRUCT
	    Typ : USINT;
	    VarID : UDINT;
	    Group : UDINT;
	    Config : UDINT;
	    State : USINT;
	    GroupBit : USINT;
	    ParameterNo : USINT;
	    nc : USINT;
	    pParameter : ^_DS_AlarmParameter;
	    pTimestamps : ^_DS_AlarmTimestamps;
	    num_appeared : UDINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_IncDecValueResp : STRUCT
	    dpID : UDINT;
	    ErrorCode : UDINT;
	    value : LREAL;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_Lasal_IDListState : STRUCT
	    StationNo : UDINT;
	    Status : UDINT;
	  END_STRUCT;
#pragma pack(pop)
	  _DS_Parameter :
	  (
	    _RespTimeout,
	    _RespTimeoutLong,
	    _SendTimeout,
	    _RecvTimeout
	  )$UDINT;
#pragma pack(push, 1)
	  _DS_ProgressStatus : STRUCT
	    HandleID : UDINT;
	    State : UDINT;
	    Percent : UDINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_RecipeLoadResp : STRUCT
	    ErrorCode : UDINT;
	    pDpne : ^CHAR;
	    DpneLen : UDINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_RecipeSaveResp : STRUCT
	    ErrorCode : UDINT;
	    pDpne : ^CHAR;
	    DpneLen : UDINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_RefListAddResp : STRUCT
	    dpID : UDINT;
	    ErrorCode : UDINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_RefListSubResp : STRUCT
	    dpID : UDINT;
	    ErrorCode : UDINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_RefListUpdate : STRUCT
	    dpID : UDINT;
	    Value : LREAL;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_RefListUpdateString : STRUCT
	    dpID : UDINT;
	    pString : ^UINT;
	    StringLen : UDINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_RemoveLocalClientCmd : STRUCT
	    ID : UDINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_RemoveLocalClientRangeCmd : STRUCT
	    Typ : USINT;
	    pFrom : ^USINT;
	    pTo : ^USINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_RemoveLocalClientRangeResp : STRUCT
	    ErrorCode : UDINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_RemoveLocalClientResp : STRUCT
	    ErrorCode : UDINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_RemoveThisLocalClientResp : STRUCT
	    ErrorCode : UDINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_ResetDataserviceResp : STRUCT
	    ErrorCode : UDINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_RestartDataserviceResp : STRUCT
	    ErrorCode : UDINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_SetStringCmd : STRUCT
	    dpID : UDINT;
	    pString : ^UINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_SetStringResp : STRUCT
	    dpID : UDINT;
	    ErrorCode : UDINT;
	    pString : ^UINT;
	    StringLen : UDINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_SetValueCmd : STRUCT
	    dpID : UDINT;
	    value : LREAL;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_SetValueResp : STRUCT
	    dpID : UDINT;
	    ErrorCode : UDINT;
	    value : LREAL;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_StationListResp : STRUCT
	    StationNo : UDINT;
	    StationStatus : UDINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_StationStatusResp : STRUCT
	    StationNo : UDINT;
	    StationStatus : UDINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_SubscribeStateCmd : STRUCT
	    Target : UDINT;
	    Typ : _DS_StateType;
	    pData : pVoid;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_SubscribeStateCmd_1 : STRUCT
	    StationNo : UDINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_SubscribeStateResp : STRUCT
	    ErrorCode : UDINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_UnsubscribeStateCmd : STRUCT
	    Target : UDINT;
	    Typ : _DS_StateType;
	    pData : pVoid;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_UnsubscribeStateCmd_1 : STRUCT
	    StationNo : UDINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_UnsubscribeStateResp : STRUCT
	    ErrorCode : UDINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_UserEventResp : STRUCT
	    Status : USINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_UserLoginResp : STRUCT
	    UserID : UDINT;
	    NameLength : UDINT;
	    pName : ^UINT;
	    ErrorCode : UDINT;
	    FirstNameLength : UDINT;
	    pFirstName : ^UINT;
	    LastNameLength : UDINT;
	    pLastName : ^UINT;
	    NumberOfRoles : UDINT;
	    pRoleID : ^UDINT;
	    WriteAccessLogicLength : UDINT;
	    pWriteAccessLogic : ^USINT;
	    ReadAccessLogicLength : UDINT;
	    pReadAccessLogic : ^USINT;
	    UserInfoLength : UDINT;
	    pUserInfo : ^UINT;
	    CreatedDate : UDINT;
	    CreatedTime : UDINT;
	    ModifiedUserDate : UDINT;
	    ModifiedUserTime : UDINT;
	    ModifiedPWDate : UDINT;
	    ModifiedPWTime : UDINT;
	    LastLoginDate : UDINT;
	    LastLoginTime : UDINT;
	    LastLogoutDate : UDINT;
	    LastLogoutTime : UDINT;
	    UserPrivileges : _DS_UnsLongLongInt;
	    OperationTimeout : UINT;
	    WriteProtected : USINT;
	    AccessNumberValid : USINT;
	    AccessNumber : UDINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  _DS_UserPasswordChangeResp : STRUCT
	    ErrorCode : UDINT;
	  END_STRUCT;
#pragma pack(pop)
	END_TYPE
END_CLASS;
#pragma define (restore)
DSComm_Eventjournal : CLASS_PUBLIC
	TYPE
	  CmdClassSvr :
	  (
	    NiAddMessage,
	    NiAddWarning,
	    NiAddError,
	    NiAddUserText,
	    NiAddEntry,
	    NiAddEventByID
	  )$UINT;
	END_TYPE
END_CLASS;
#pragma define (restore)
#pragma InclDefBlk Lasal32
Lasal32 : CLASS_PUBLIC
#include "..\Class\Lasal32\Lasal32Def.h"
	TYPE
#pragma pack(push, 1)
	  FILE_INFO : STRUCT
	    fileName : ARRAY [0..7] OF USINT;
	    extension : ARRAY [0..2] OF USINT;
	    attributes : USINT;
	    dateTime : UDINT;
	    fileSize : DINT;
	  END_STRUCT;
#pragma pack(pop)
	END_TYPE
END_CLASS;
#pragma define (restore)
Lasal32User : CLASS_PUBLIC
	TYPE
	  Do_MyMethods :
	  (
	    Status_Go_Online_First,
	    Do_Online,
	    Do_Offline,
	    Do_Reset,
	    Do_Run,
	    Do_Reboot,
	    Status_Still_Online,
	    Status_Its_Succesfull
	  )$UDINT;
	END_TYPE
END_CLASS;
#pragma define (restore)
MerkerEx : CLASS_PUBLIC
#include <.\lsl_st_mt.h>
	TYPE
	  Cmdm_udLength :
	  (
	    GET_DATA_PTR,
	    EXCHANGE_DATA_PTR,
	    SAVE_DATA_TO_RAMEXFILE
	  )$UINT;
	END_TYPE
END_CLASS;
#pragma define (restore)
SafetyBaseFSoE : CLASS_PUBLIC
#include "..\Source\interfaces\lsl_st_safety.h"
#include "..\Class\SafetyManager\SafetyManager.h"
#include "..\Source\BusInterface.h"
#include "..\Class\SafetyBaseFSoE\SafetyBaseFSoE.h"
#include "..\Source\interfaces\lsl_st_hardwaretree.h"
	TYPE
#pragma pack(push, 1)
	  SDOHeader : STRUCT
	    StdHeader : t_SDOStdHeader;
	    SrcAdr : HDINT;
	    DstAdr : HDINT;
	    SessId : UDINT;
	  END_STRUCT;
#pragma pack(pop)
	END_TYPE
END_CLASS;
#pragma define (restore)
SafetyCDIAS_Base : CLASS_PUBLIC
#include "..\Class\SafetyCDIAS_Base\SafetyCDIAS_Base.h"
#include "..\Source\interfaces\lsl_st_safety.h"
#include "..\Class\SafetyManager\SafetyManager.h"
	TYPE
	  FrameTypeType : BSINT
	  [
	    1 Cfg,
	    2 ToP,
	    3 Standard,
	    4 PDO,
	    5 TDO,
	    6 Reserved1,
	    7 Reserved2,
	    8 Response,
	  ];
#pragma pack(push, 1)
	  SDOStdHeader : STRUCT
	    CRC : UDINT;
	    Cmd : USINT;
	    Typ : FrameTypeType;
	    SeqNr : USINT;
	    Len : USINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  SDOHeader : STRUCT
	    StdHeader : SDOStdHeader;
	    SrcAdr : HDINT;
	    DstAdr : HDINT;
	    SessId : UDINT;
	  END_STRUCT;
#pragma pack(pop)
	END_TYPE
END_CLASS;
#pragma define (restore)
SafetyUDP : CLASS_PUBLIC
	TYPE
	  _FSM_UDP_ERROR :
	  (
	    _NO_ERROR_UDP_ERROR:=0,
	    _INIT_TCP_USER_UDP_ERROR:=4294967295,
	    _NO_IP_ADDRESS_UDP_ERROR:=4294967294,
	    _NO_MEMORY_SOCKET_UDP_ERROR:=4294967293,
	    _NO_MEMORY_SENDBUFFER_UDP_ERROR:=4294967292,
	    _INVALID_HANDLE_UDP_ERROR:=4294967291,
	    _SHUTDOWN_UDP_ERROR:=4294967290,
	    _CLOSESOCKET_UDP_ERROR:=4294967289,
	    _ALLOCATE_SOCKET_UDP_ERROR:=4294967288,
	    _SET_BIND_UDP_ERROR:=4294967287,
	    _RECV_UDP_ERROR:=4294967286,
	    _SEND_UDP_ERROR:=4294967285,
	    _NO_LOCAL_IP_UDP_ERROR:=4294967284,
	    _NO_DESTINATION_IP_UDP_ERROR:=4294967283,
	    _INVALID_UDP_PORT_UDP_ERROR,
	    _INVALID_INTERFACE_NUMBER_UDP_ERROR,
	    _NO_MEMORY_RECEIVE_BUFFER_UDP_ERROR,
	    _NO_MEMORY_CONNECTION_BUFFER_UDP_ERROR
	  )$DINT;
	  _FSM_UDP_USER :
	  (
	    _STATE_INIT_UDP,
	    _STATE_IDLE_UDP,
	    _STATE_SOCK_UDP,
	    _STATE_CONNECT_UDP,
	    _STATE_ONLY_SEND_UDP,
	    _STATE_RECV_UDP,
	    _STATE_SEND_UDP,
	    _STATE_SHUTDOWN_UDP,
	    _STATE_CLOSE_SOCK_UDP,
	    _STATE_ERROR_UDP
	  )$UDINT;
	  _INITSTEPS :
	  (
	    _NO_OPERATION,
	    _ADD_SOCKET,
	    _CHECK_IF_SOCKET_OPEN,
	    _BIND_SOCKET,
	    _CLOSE_CON
	  )$UDINT;
	END_TYPE
END_CLASS;
#pragma define (restore)
#pragma InclDefBlk SdiasBase
SdiasBase : CLASS_PUBLIC
#include "..\Class\SdiasManager\SdiasManager.h"
#include "..\Class\SdiasBase\SdiasBase.h"
#include "..\Class\HwControl\HwControl.h"
#include "..\Source\interfaces\lsl_st_hardwaretree.h"
#include "..\Source\interfaces\lsl_st_mt.h"
	TYPE
	  t_b_SdiasFWState : BINT
	  [
	    1 VoltageNotOk,
	    2 NoSync,
	    3 FlashDataCRCError,
	    4 RAMDataCRCError,
	    5 FlashUnsafe,
	    6 BusTimeNotSupported,
	    7 ToggleBit,
	  ];
#pragma pack(push, 1)
	  t_s_SdiasFWStateStandard : STRUCT
	    ErrorBits : t_b_SdiasFWState;
	    Reserved : UINT;
	  END_STRUCT;
#pragma pack(pop)
#pragma pack(push, 1)
	  t_s_SPI_AccessControl : STRUCT
	    udAccessHandle : UDINT;
	    usAccessLength : USINT;
	    usAccessOffset : USINT;
	    pControlByte : t_SDIAS_pControlByte;
	    pStateByte : t_SDIAS_pStateByte;
	  END_STRUCT;
#pragma pack(pop)
	  t_SPI_PDO_DataWrite : ARRAY [0..SDIAS_MAX_PDO_SIZE-1] OF USINT;
	  t_SPI_PDO_DataRead : ARRAY [0..SDIAS_MAX_PDO_SIZE-1] OF USINT;
	  t_e_SPIWriteSSW :
	  (
	    _SPIWrite_Idle,
	    _AllocateMemory,
	    _SetRequest,
	    _ReadGrant,
	    _ReadSectorData_SetPageAddress,
	    _ReadSectorData_Wait4SetPageAddress,
	    _ReadSectorData_SetReadPageCommand,
	    _ReadSectorData_Wait4SetReadPageCommand,
	    _ReadSectorData_ReadReceiveBufferPart1,
	    _ReadSectorData_ReadReceiveBufferPart2,
	    _EraseSector_SetPageAddress,
	    _EraseSector_Wait4SetPageAddress,
	    _EraseSector_SetEraseSectorCommand,
	    _EraseSector_Wait4SetEraseSectorCommand,
	    _WriteSectorData_SetPageAddress,
	    _WriteSectorData_Wait4SetPageAddress,
	    _WriteSectorData_WriteTransmitBufferPart1,
	    _WriteSectorData_WriteTransmitBufferPart2,
	    _WriteSectorData_SetWritePageCommand,
	    _WriteSectorData_Wait4SetWritePageCommand,
	    _VerifySectorData_SetPageAddress,
	    _VerifySectorData_Wait4SetPageAddress,
	    _VerifySectorData_SetReadPageCommand,
	    _VerifySectorData_Wait4SetReadPageCommand,
	    _VerifySectorData_ReadReceiveBufferPart1,
	    _VerifySectorData_ReadReceiveBufferPart2,
	    _VerifySectorData_CompareData,
	    _ClearRequest,
	    _DisableDataObjects,
	    _FreeMemory,
	    _FinishSPIWrite
	  )$UDINT;
	  usWriteModes : USINT;
#pragma pack(push, 1)
	  t_s_SPIWriteVars : STRUCT
	    sWriteAccess : t_s_SPI_AccessControl;
	    pWriteData : ^t_SPI_PDO_DataWrite;
	    sReadAccess : t_s_SPI_AccessControl;
	    pReadData : ^t_SPI_PDO_DataRead;
	    sReadStateAccess : t_s_SPI_AccessControl;
	    pReadStateData : ^USINT;
	    pSectorMemoryRead : ^USINT;
	    pSectorMemoryWrite : ^USINT;
	    pSectorMemoryVerify : ^USINT;
	    pUserWriteData : ^USINT;
	    udUserWriteLength : UDINT;
	    usUserSectorNumber : USINT;
	    usUserPageNumber : USINT;
	    usUserPageOffset : USINT;
	    eSPIWriteSSW : t_e_SPIWriteSSW;
	    eSPIWriteSSWError : t_e_SPIWriteSSW;
	    uiCurrentPage : UINT;
	    udCurrentSectorMemoryOffset : UDINT;
	    usRetryCnt : USINT;
	    bsFinished : BSINT
	    [
	      1 Finished,
	      2 Error,
	      3 Bit3,
	      4 Bit4,
	      5 Bit5,
	      6 Bit6,
	      7 Bit7,
	      8 Bit8,
	    ];
	    usCycleWaitCnt : USINT;
	    udTimeout : UDINT;
	    NotReadyCnt : DINT;
	    usFinishedWriteLog : USINT;
	    usWriteMode : usWriteModes;
	    udMemoryToAllocate : UDINT;
	    uiLastPage : UINT;
	    udUserDataOffset : DINT;
	  END_STRUCT;
#pragma pack(pop)
	END_TYPE
END_CLASS;
#pragma define (restore)
VirtualBaseInit : CLASS_PUBLIC
	TYPE
	  CmdData :
	  (
	    GET_ADD_INFO,
	    READ_DATA,
	    WRITE_DATA,
	    READ_DATA_OFF,
	    WRITE_DATA_OFF
	  )$UINT;
	END_TYPE
END_CLASS;
#pragma define (restore)

#RE_IN_DE
