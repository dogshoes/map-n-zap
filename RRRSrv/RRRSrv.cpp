/**********************************************************************
   Map-N-Zap v2.x. Interface to the CYE Personal Robot
   Copyright (C) 2000, Probotics, Inc.

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2
   of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.Furthermore, this source code
   is distributed without any promise, implied or explicit, of
   support from Probotics, Inc.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA.

   Probotics, Inc
   http://www.personalrobots.com
   700 River Ave #223
   Pittsburgh, PA 15212
   USA
 **********************************************************************/
///////////////////////////////////////////////////////////////////////////////
// Module:     RRRSrv.cpp
// Abstract:   Defines the class behaviors for the application.
// Author:     J.C.H
// Copyright 1997 Cycle Time Corp
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RRRSrv.h"
#include "MainFrame.h"
#include "ChildFrame.h"
#include "RRRSrvDoc.h"
#include "RRRSrvView.h"
#include "AboutDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CRRRSrvApp

BEGIN_MESSAGE_MAP(CRRRSrvApp, CWinApp)
    //{{AFX_MSG_MAP(CRRRSrvApp)
    ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
    // NOTE - the ClassWizard will add and remove mapping macros here.
    //    DO NOT EDIT what you see in these blocks of generated code!
    //}}AFX_MSG_MAP
    // Standard file based document commands
    ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
    ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRRRSrvApp construction

CRRRSrvApp::CRRRSrvApp()
{
    // TODO: add construction code here,
    // Place all significant initialization in InitInstance
    m_oRRRCom = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CRRRSrvApp object

CRRRSrvApp theApp;

// This identifier was generated to be statistically unique for your app.
// You may change it if you prefer to choose a specific identifier.

// {F300B090-6343-11D0-8461-707D42000000}
static const CLSID clsid = { 0xf300b090, 0x6343, 0x11d0, { 0x84, 0x61, 0x70, 0x7d, 0x42, 0x0, 0x0, 0x0 } };

/////////////////////////////////////////////////////////////////////////////
// CRRRSrvApp initialization

BOOL CRRRSrvApp::InitInstance()
{
    // Initialize OLE libraries
    if (!AfxOleInit())
    {
        AfxMessageBox(IDP_OLE_INIT_FAILED);
        return FALSE;
    }

    // Standard initialization
    // If you are not using these features and wish to reduce the size
    //  of your final executable, you should remove from the following
    //  the specific initialization routines you do not need.

    LoadStdProfileSettings();  // Load standard INI file options (including MRU)

    // Register the application's document templates.  Document templates
    //  serve as the connection between documents, frame windows and views.

    CMultiDocTemplate* pDocTemplate;
    pDocTemplate = new CMultiDocTemplate(
        IDR_RRRSRVTYPE,
        RUNTIME_CLASS(CRRRSrvDoc),
        RUNTIME_CLASS(CChildFrame), // custom MDI child frame
        RUNTIME_CLASS(CRRRSrvView));
    AddDocTemplate(pDocTemplate);

    // Connect the COleTemplateServer to the document template.
    //  The COleTemplateServer creates new documents on behalf
    //  of requesting OLE containers by using information
    //  specified in the document template.
    m_server.ConnectTemplate(clsid, pDocTemplate, FALSE);

    // Register all OLE server factories as running.  This enables the
    //  OLE libraries to create objects from other applications.
    COleTemplateServer::RegisterAll();
    // Note: MDI applications register all server objects without regard
    //  to the /Embedding or /Automation on the command line.

    m_oRRRCom = NULL;
    m_sPortName = GetProfileString(GENERAL_SECTION, PORTNAME, "COM1");
    WriteProfileString(GENERAL_SECTION, PORTNAME, m_sPortName);

    m_Encoding = GetProfileInt(GENERAL_SECTION, ENCODE, RRRCom::CHECKSUM_ENCODING);
    switch (m_Encoding)
    {
        case RRRCom::CHECKSUM_ENCODING:
        case RRRCom::CRC_ENCODING:
            break;
        default:
            m_Encoding = RRRCom::CHECKSUM_ENCODING;
            break;
    }

    WriteProfileInt(GENERAL_SECTION, ENCODE, m_Encoding);

    m_BaudRate = GetProfileInt(GENERAL_SECTION, BAUDRATE, 4800);
    switch (m_BaudRate)
    {
        case 19200:
        case 9600:
        case 4800:
        case 2400:
        case 1200:
        case 300:
            break;
        default:
            m_BaudRate = 4800;
            break;
    }

    WriteProfileInt(GENERAL_SECTION, BAUDRATE, m_BaudRate);

    CString DTREnable = GetProfileString(GENERAL_SECTION, DTRENABLE, "OFF");
    if ((DTREnable.CompareNoCase("ON") == 0) || (DTREnable.CompareNoCase("TRUE") == 0))
    {
        m_EnableDTR = TRUE;
        DTREnable = "ON";
    }
    else
    {
        m_EnableDTR = FALSE;
        DTREnable = "OFF";
    }

    WriteProfileString(GENERAL_SECTION, DTRENABLE, DTREnable);
    
    m_DTROnDelay = GetProfileInt(GENERAL_SECTION, DTRONDELAY, 3);
    WriteProfileInt(GENERAL_SECTION, DTRONDELAY, m_DTROnDelay);

    m_DTROffDelay = GetProfileInt(GENERAL_SECTION, DTROFFDELAY, 3);
    WriteProfileInt(GENERAL_SECTION, DTROFFDELAY, m_DTROffDelay);

    CString FullDuplex = GetProfileString(GENERAL_SECTION, FULLDUPLEX, "ON");
    if ((FullDuplex.CompareNoCase("ON") == 0) || (FullDuplex.CompareNoCase("TRUE") == 0))
    {
        m_FullDuplex = TRUE;
        FullDuplex = "ON";
    }
    else
    {
        m_FullDuplex = FALSE;
        FullDuplex = "OFF";
    }

    WriteProfileString(GENERAL_SECTION, FULLDUPLEX, FullDuplex);

    m_MyAddress = GetProfileInt(GENERAL_SECTION, LOCALADDRESS, DEFAULT_PC_ADDRESS);

    if ((m_MyAddress < 0) || (m_MyAddress > 15))
    {
        m_MyAddress = DEFAULT_PC_ADDRESS;
    }

    WriteProfileInt(GENERAL_SECTION, LOCALADDRESS, m_MyAddress);

    //m_RobotAddress = GetProfileInt(GENERAL_SECTION, ROBOTADDRESS, DEFAULT_ROBOT_ADDRESS);
    //if ((m_RobotAddress < 0) || (m_RobotAddress > 15))
    //   m_RobotAddress = DEFAULT_ROBOT_ADDRESS;
    //WriteProfileInt(GENERAL_SECTION, ROBOTADDRESS, m_RobotAddress);

    CString InvertDTR = GetProfileString(GENERAL_SECTION, INVERTDTR, "OFF");
    if ((InvertDTR.CompareNoCase("ON") == 0) || (InvertDTR.CompareNoCase("TRUE") == 0))
    {
        m_InvertDTR = TRUE;
        InvertDTR = "ON";
    }
    else
    {
        m_EnableDTR = FALSE;
        InvertDTR = "OFF";
    }

    WriteProfileString(GENERAL_SECTION, INVERTDTR, InvertDTR);

    m_PreFlag = GetProfileInt(GENERAL_SECTION, PREFLAGCHARS, RRRCom::PRE_FLAG);
    if ((m_PreFlag < 0) || (m_PreFlag > 1000))
    {
        m_PreFlag = RRRCom::PRE_FLAG;
    }

    WriteProfileInt(GENERAL_SECTION, PREFLAGCHARS, m_PreFlag);

    m_PostFlag = GetProfileInt(GENERAL_SECTION, POSTFLAGCHARS, RRRCom::POST_FLAG);
    if ((m_PostFlag < 0) || (m_PostFlag > 1000))
    {
        m_PostFlag = RRRCom::POST_FLAG;
    }

    WriteProfileInt(GENERAL_SECTION, POSTFLAGCHARS, m_PostFlag);

    // create main MDI Frame window
    CMainFrame* pMainFrame = new CMainFrame;
    if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
    {
        return FALSE;
    }

    m_pMainWnd = pMainFrame;

    // Parse command line for standard shell commands, DDE, file open
    CCommandLineInfo cmdInfo;
    ParseCommandLine(cmdInfo);

    // Check to see if launched as OLE server
    if (cmdInfo.m_bRunEmbedded || cmdInfo.m_bRunAutomated)
    {
        // Application was run with /Embedding or /Automation.  Don't show the
        //  main window in this case.
        return TRUE;
    }

    // When a server application is launched stand-alone, it is a good idea
    //  to update the system registry in case it has been damaged.
    m_server.UpdateRegistry(OAT_DISPATCH_OBJECT);
    COleObjectFactory::UpdateRegistryAll();

    // Dispatch commands specified on the command line
    // Ignore command line parms so no new document.
    //	if (!ProcessShellCommand(cmdInfo))
    //		return FALSE;

    // The main window has been initialized, so show and update it.
    pMainFrame->ShowWindow(m_nCmdShow);
    pMainFrame->UpdateWindow();

    return TRUE;
}


// App command to run the dialog
void CRRRSrvApp::OnAppAbout()
{
    CAboutDlg aboutDlg;
    aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CRRRSrvApp commands

// saves working parameters
void CRRRSrvApp::SaveParameters()
{
    WriteProfileString(GENERAL_SECTION, PORTNAME, m_sPortName);
    WriteProfileInt(GENERAL_SECTION, ENCODE, m_Encoding);
    WriteProfileInt(GENERAL_SECTION, BAUDRATE, m_BaudRate);

    CString DTREnable;

    if (m_EnableDTR)
    {
        DTREnable = "ON";
    }
    else
    {
        DTREnable = "OFF";
    }

    WriteProfileString(GENERAL_SECTION, DTRENABLE, DTREnable);
    WriteProfileInt(GENERAL_SECTION, DTRONDELAY, m_DTROnDelay);
    WriteProfileInt(GENERAL_SECTION, DTROFFDELAY, m_DTROffDelay);

    CString FullDuplex;

    if (m_FullDuplex)
    {
        FullDuplex = "ON";
    }
    else
    {
        FullDuplex = "OFF";
    }

    WriteProfileString(GENERAL_SECTION, FULLDUPLEX, FullDuplex);
    WriteProfileInt(GENERAL_SECTION, LOCALADDRESS, m_MyAddress);
    //WriteProfileInt(GENERAL_SECTION, ROBOTADDRESS, m_RobotAddress);

    CString InvertDTR;

    if (m_InvertDTR)
    {
        InvertDTR = "ON";
    }
    else
    {
        InvertDTR = "OFF";
    }

    WriteProfileString(GENERAL_SECTION, INVERTDTR, InvertDTR);
}