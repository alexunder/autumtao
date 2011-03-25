; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CRecogdemoDlg
LastTemplate=generic CWnd
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "recogdemo.h"

ClassCount=4
Class1=CRecogdemoApp
Class2=CRecogdemoDlg
Class3=CAboutDlg

ResourceCount=7
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Class4=CImageTable
Resource3=IDD_RECOGDEMO_DIALOG
Resource4=IDR_MENU_PROCESS (Chinese (P.R.C.))
Resource5=IDD_RECOGDEMO_DIALOG (Chinese (P.R.C.))
Resource6=IDD_ABOUTBOX (Chinese (P.R.C.))
Resource7=IDR_MENU_PROCESS

[CLS:CRecogdemoApp]
Type=0
HeaderFile=recogdemo.h
ImplementationFile=recogdemo.cpp
Filter=N

[CLS:CRecogdemoDlg]
Type=0
HeaderFile=recogdemoDlg.h
ImplementationFile=recogdemoDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=AFX_PROCESS_FIND

[CLS:CAboutDlg]
Type=0
HeaderFile=recogdemoDlg.h
ImplementationFile=recogdemoDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_RECOGDEMO_DIALOG]
Type=1
Class=CRecogdemoDlg
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_BUTTON_BROWSE,button,1342242816

[CLS:CImageTable]
Type=0
HeaderFile=ImageTable.h
ImplementationFile=ImageTable.cpp
BaseClass=CWnd
Filter=W
VirtualFilter=WC
LastObject=AFX_PROCESS_BINERIFY

[MNU:IDR_MENU_PROCESS]
Type=1
Class=CRecogdemoDlg
Command1=AFX_PROCESS_TOGRAY
Command2=AFX_PROCESS_BINERIFY
Command3=AFX_PROCESS_FILTER
Command4=AFX_PROCESS_FIND
CommandCount=4

[MNU:IDR_MENU_PROCESS (Chinese (P.R.C.))]
Type=1
Class=?
Command1=AFX_PROCESS_TOGRAY
Command2=AFX_PROCESS_BINERIFY
Command3=AFX_PROCESS_FILTER
Command4=AFX_PROCESS_FIND
CommandCount=4

[DLG:IDD_ABOUTBOX (Chinese (P.R.C.))]
Type=1
Class=?
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_RECOGDEMO_DIALOG (Chinese (P.R.C.))]
Type=1
Class=?
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_BUTTON_BROWSE,button,1342242816

