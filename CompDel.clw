; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CCompDelApp
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "compdel.h"
LastPage=0

ClassCount=11
Class1=CCompDelApp
Class2=CDlgAbout
Class3=CDlgPDel
Class4=CDlgPDisp
Class5=CDlgPOther
Class6=CDlgPDel2
Class7=CDlgPDel3
Class8=CDlgConfirm
Class9=CDlgInstall
Class10=CDlgHelpDoc
Class11=CDlgNorm

ResourceCount=20
Resource1=IDD_DLG_PROPDEL (âpåÍ (±“ÿ∂))
Resource2=IDD_DLG_HELPDOC
Resource3=IDD_DLG_CONFF
Resource4=IDD_DLG_CONFF (âpåÍ (±“ÿ∂))
Resource5=IDD_DLG_NORMAL
Resource6=IDD_DLG_NORMAL (âpåÍ (±“ÿ∂))
Resource7=IDD_DLG_PROPDEL2 (âpåÍ (±“ÿ∂))
Resource8=IDD_DLG_PROPDISP
Resource9=IDD_DLG_PROPDEL3
Resource10=IDD_DLG_PROPDEL2
Resource11=IDD_DLG_PROPOTHER
Resource12=IDD_DLG_PROPOTHER (âpåÍ (±“ÿ∂))
Resource13=IDD_DLG_ABOUT
Resource14=IDD_DLG_PROPDISP (âpåÍ (±“ÿ∂))
Resource15=IDD_DLG_HELPDOC (âpåÍ (±“ÿ∂))
Resource16=IDD_DLG_ABOUT (âpåÍ (±“ÿ∂))
Resource17=IDD_DLG_PROPDEL
Resource18=IDD_DLG_INSTALL (âpåÍ (±“ÿ∂))
Resource19=IDD_DLG_INSTALL
Resource20=IDD_DLG_PROPDEL3 (âpåÍ (±“ÿ∂))

[CLS:CCompDelApp]
Type=0
BaseClass=CWinApp
HeaderFile=CompDel.h
ImplementationFile=CompDel.cpp
LastObject=CCompDelApp
Filter=N
VirtualFilter=AC

[CLS:CDlgAbout]
Type=0
BaseClass=CDialog
HeaderFile=DlgAbout.h
ImplementationFile=DlgAbout.cpp

[CLS:CDlgPDel]
Type=0
BaseClass=CPropertyPage
HeaderFile=dlgconfig.h
ImplementationFile=dlgconfig.cpp
LastObject=CDlgPDel

[CLS:CDlgPDisp]
Type=0
BaseClass=CPropertyPage
HeaderFile=dlgconfig.h
ImplementationFile=dlgconfig.cpp

[CLS:CDlgPOther]
Type=0
BaseClass=CPropertyPage
HeaderFile=dlgconfig.h
ImplementationFile=dlgconfig.cpp
LastObject=IDC_BTN_QEXEC
Filter=D
VirtualFilter=idWC

[CLS:CDlgPDel2]
Type=0
BaseClass=CPropertyPage
HeaderFile=dlgconfig.h
ImplementationFile=dlgconfig.cpp
Filter=D
VirtualFilter=idWC
LastObject=IDC_C_NOTREMOVE

[CLS:CDlgPDel3]
Type=0
BaseClass=CPropertyPage
HeaderFile=dlgconfig.h
ImplementationFile=dlgconfig.cpp
Filter=D
VirtualFilter=idWC
LastObject=CDlgPDel3

[CLS:CDlgConfirm]
Type=0
BaseClass=CDialog
HeaderFile=DlgConfirm.h
ImplementationFile=DlgConfirm.cpp

[CLS:CDlgInstall]
Type=0
BaseClass=CDialog
HeaderFile=DlgConfirm.h
ImplementationFile=DlgConfirm.cpp

[CLS:CDlgHelpDoc]
Type=0
BaseClass=CDialog
HeaderFile=DlgHelpDoc.h
ImplementationFile=DlgHelpDoc.cpp
Filter=D
VirtualFilter=dWC
LastObject=CDlgHelpDoc

[CLS:CDlgNorm]
Type=0
BaseClass=CDialog
HeaderFile=DlgNorm.h
ImplementationFile=DlgNorm.cpp

[DLG:IDD_DLG_ABOUT]
Type=1
Class=CDlgAbout
ControlCount=3
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308352
Control3=IDOK,button,1342242817

[DLG:IDD_DLG_PROPDEL]
Type=1
Class=CDlgPDel
ControlCount=14
Control1=IDC_STATIC,button,1342177287
Control2=IDC_R1_NULL,button,1342308361
Control3=IDC_R1_RND,button,1342177289
Control4=IDC_R1_DOD,button,1476395017
Control5=IDC_EDIT_NTIMES,edit,1342185600
Control6=IDC_STATIC,static,1342308864
Control7=IDC_C_DATE,button,1342185475
Control8=IDC_STATIC,button,1342177287
Control9=IDC_R2_ZLEN,button,1342308361
Control10=IDC_R2_RLEN,button,1342177289
Control11=IDC_STATIC,button,1342177287
Control12=IDC_R3_NNAME,button,1342308361
Control13=IDC_R3_RNAME,button,1342177289
Control14=IDC_R3_SNAME,button,1342177289

[DLG:IDD_DLG_PROPDISP]
Type=1
Class=CDlgPDisp
ControlCount=6
Control1=IDC_C_DISPF,button,1342177283
Control2=IDC_STATIC,button,1342177287
Control3=IDC_C_NDISPN,button,1342177283
Control4=IDC_C_NDISPE,button,1342177283
Control5=IDC_C_DISPC,button,1342177283
Control6=IDC_C_DISPF2,button,1342177283

[DLG:IDD_DLG_PROPOTHER]
Type=1
Class=CDlgPOther
ControlCount=12
Control1=IDC_STATIC,button,1342177287
Control2=IDC_STATIC,static,1342308864
Control3=IDC_EDIT_QUICK_FNAME,edit,1350631552
Control4=IDC_BTN_QFILE_BROWSE,button,1342177280
Control5=IDC_BTN_QEXEC,button,1342177280
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,button,1476395015
Control8=IDC_EDIT_TEXTEDITOR,edit,1484849280
Control9=IDC_BTN_QFILE_BROWSE2,button,1476395008
Control10=IDC_UNINSTALL,button,1342177280
Control11=IDC_STATIC,static,1342308352
Control12=IDC_VERINFO,button,1342177280

[DLG:IDD_DLG_PROPDEL2]
Type=1
Class=CDlgPDel2
ControlCount=10
Control1=IDC_STATIC,button,1342177287
Control2=IDC_EDIT_BUFFER,edit,1342185600
Control3=IDC_STATIC,static,1342308864
Control4=IDC_STATIC,button,1342177287
Control5=IDC_EDIT_OVERRUN,edit,1342185600
Control6=IDC_STATIC,static,1342308864
Control7=IDC_STATIC,button,1476395015
Control8=IDC_EDIT_DODCHAR,edit,1476403328
Control9=IDC_STATIC,static,1476526592
Control10=IDC_C_NOTREMOVE,button,1342242819

[DLG:IDD_DLG_PROPDEL3]
Type=1
Class=CDlgPDel3
ControlCount=15
Control1=IDC_STATIC,button,1342177287
Control2=IDC_STATIC,static,1342312448
Control3=IDC_C_ANTICACHE,button,1342177283
Control4=IDC_R1_ANTIDIR,button,1342308361
Control5=IDC_R1_ANTIDIR2,button,1342177289
Control6=IDC_EDIT_ANTISIZE,edit,1342185600
Control7=IDC_STATIC,static,1342308864
Control8=IDC_C_ANTI_ONETIME,button,1342177283
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC,button,1342177287
Control11=IDC_STATIC,static,1342312448
Control12=IDC_C_DMY,button,1342177283
Control13=IDC_C_DMYSKIP,button,1342177283
Control14=IDC_EDIT_DUMMY_NFILES,edit,1342185600
Control15=IDC_STATIC,static,1342308864

[DLG:IDD_DLG_CONFF]
Type=1
Class=CDlgConfirm
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_MESSAGE,static,1342308352
Control3=IDOK,button,1342242817
Control4=IDCANCEL,button,1342242816

[DLG:IDD_DLG_INSTALL]
Type=1
Class=CDlgInstall
ControlCount=12
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342181382
Control4=IDC_INST_DESKTOP,button,1342177283
Control5=IDC_INST_CONTEXT,button,1342177283
Control6=IDC_INST_MENU,button,1342177283
Control7=IDC_STATIC,button,1342177287
Control8=IDC_RADIO_UNIN,button,1342308361
Control9=IDC_RADIO_UNIN2,button,1342177289
Control10=IDOK,button,1342242817
Control11=IDCANCEL,button,1342242816
Control12=IDC_INSTHLP,button,1342177280

[DLG:IDD_DLG_HELPDOC]
Type=1
Class=CDlgHelpDoc
ControlCount=2
Control1=IDC_EDIT_MAIN,edit,1352730628
Control2=IDOK,button,1342242817

[DLG:IDD_DLG_NORMAL]
Type=1
Class=CDlgNorm
ControlCount=5
Control1=IDC_STATIC,static,1342177283
Control2=IDC_LFNAME,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDC_MES,static,1342308352
Control5=IDC_PRGRESS,msctls_progress32,1350565888

[DLG:IDD_DLG_ABOUT (âpåÍ (±“ÿ∂))]
Type=1
Class=?
ControlCount=3
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308352
Control3=IDOK,button,1342242817

[DLG:IDD_DLG_CONFF (âpåÍ (±“ÿ∂))]
Type=1
Class=?
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_MESSAGE,static,1342308352
Control3=IDOK,button,1342242817
Control4=IDCANCEL,button,1342242816

[DLG:IDD_DLG_HELPDOC (âpåÍ (±“ÿ∂))]
Type=1
Class=?
ControlCount=2
Control1=IDC_EDIT_MAIN,edit,1352730628
Control2=IDOK,button,1342242817

[DLG:IDD_DLG_INSTALL (âpåÍ (±“ÿ∂))]
Type=1
Class=?
ControlCount=12
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342181382
Control4=IDC_INST_DESKTOP,button,1342177283
Control5=IDC_INST_CONTEXT,button,1342177283
Control6=IDC_INST_MENU,button,1342177283
Control7=IDC_STATIC,button,1342177287
Control8=IDC_RADIO_UNIN,button,1342308361
Control9=IDC_RADIO_UNIN2,button,1342177289
Control10=IDOK,button,1342242817
Control11=IDCANCEL,button,1342242816
Control12=IDC_INSTHLP,button,1342177280

[DLG:IDD_DLG_NORMAL (âpåÍ (±“ÿ∂))]
Type=1
Class=?
ControlCount=5
Control1=IDC_STATIC,static,1342177283
Control2=IDC_LFNAME,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDC_MES,static,1342308352
Control5=IDC_PRGRESS,msctls_progress32,1350565888

[DLG:IDD_DLG_PROPDEL (âpåÍ (±“ÿ∂))]
Type=1
Class=?
ControlCount=13
Control1=IDC_STATIC,button,1342177287
Control2=IDC_R1_NULL,button,1342308361
Control3=IDC_R1_RND,button,1342177289
Control4=IDC_EDIT_NTIMES,edit,1342185600
Control5=IDC_STATIC,static,1342308864
Control6=IDC_C_DATE,button,1342185475
Control7=IDC_STATIC,button,1342177287
Control8=IDC_R2_ZLEN,button,1342308361
Control9=IDC_R2_RLEN,button,1342177289
Control10=IDC_STATIC,button,1342177287
Control11=IDC_R3_NNAME,button,1342308361
Control12=IDC_R3_RNAME,button,1342177289
Control13=IDC_R3_SNAME,button,1342177289

[DLG:IDD_DLG_PROPDISP (âpåÍ (±“ÿ∂))]
Type=1
Class=?
ControlCount=6
Control1=IDC_C_DISPF,button,1342177283
Control2=IDC_STATIC,button,1342177287
Control3=IDC_C_NDISPN,button,1342177283
Control4=IDC_C_NDISPE,button,1342177283
Control5=IDC_C_DISPC,button,1342177283
Control6=IDC_C_DISPF2,button,1342177283

[DLG:IDD_DLG_PROPOTHER (âpåÍ (±“ÿ∂))]
Type=1
Class=?
ControlCount=12
Control1=IDC_STATIC,button,1342177287
Control2=IDC_STATIC,static,1342308864
Control3=IDC_EDIT_QUICK_FNAME,edit,1350631552
Control4=IDC_BTN_QFILE_BROWSE,button,1342177280
Control5=IDC_BTN_QEXEC,button,1342177280
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,button,1476395015
Control8=IDC_EDIT_TEXTEDITOR,edit,1484849280
Control9=IDC_BTN_QFILE_BROWSE2,button,1476395008
Control10=IDC_UNINSTALL,button,1342177280
Control11=IDC_STATIC,static,1342308352
Control12=IDC_VERINFO,button,1342177280

[DLG:IDD_DLG_PROPDEL2 (âpåÍ (±“ÿ∂))]
Type=1
Class=?
ControlCount=10
Control1=IDC_STATIC,button,1342177287
Control2=IDC_EDIT_BUFFER,edit,1342185600
Control3=IDC_STATIC,static,1342308864
Control4=IDC_STATIC,button,1342177287
Control5=IDC_EDIT_OVERRUN,edit,1342185600
Control6=IDC_STATIC,static,1342308864
Control7=IDC_STATIC,button,1476395015
Control8=IDC_EDIT_DODCHAR,edit,1476403328
Control9=IDC_STATIC,static,1476526592
Control10=IDC_C_NOTREMOVE,button,1342242819

[DLG:IDD_DLG_PROPDEL3 (âpåÍ (±“ÿ∂))]
Type=1
Class=?
ControlCount=15
Control1=IDC_STATIC,button,1342177287
Control2=IDC_STATIC,static,1342312448
Control3=IDC_C_ANTICACHE,button,1342177283
Control4=IDC_R1_ANTIDIR,button,1342308361
Control5=IDC_R1_ANTIDIR2,button,1342177289
Control6=IDC_EDIT_ANTISIZE,edit,1342185600
Control7=IDC_STATIC,static,1342308864
Control8=IDC_C_ANTI_ONETIME,button,1342177283
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC,button,1342177287
Control11=IDC_STATIC,static,1342312448
Control12=IDC_C_DMY,button,1342177283
Control13=IDC_C_DMYSKIP,button,1342177283
Control14=IDC_EDIT_DUMMY_NFILES,edit,1342185600
Control15=IDC_STATIC,static,1342308864

