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
#ifndef __MHCOMM_H__
#define __MHCOMM_H__

// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


// Dispatch interfaces referenced by this interface
class COleFont;

/////////////////////////////////////////////////////////////////////////////
// CMhComm wrapper class

class CMhComm : public CWnd
{
    protected:
        DECLARE_DYNCREATE(CMhComm)
    public:
        CLSID const& GetClsid()
        {
            static CLSID const clsid
                = { 0x8bc1991e, 0x3889, 0x11ce, { 0x87, 0x48, 0x52, 0x41, 0x53, 0x48, 0x0, 0x4 } };
            return clsid;
        }
        virtual BOOL Create(LPCTSTR lpszClassName,
                            LPCTSTR lpszWindowName, DWORD dwStyle,
                            const RECT& rect,
                            CWnd* pParentWnd, UINT nID,
                            CCreateContext* pContext = NULL)
        {
            return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID);
        }

        BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle,
                    const RECT& rect, CWnd* pParentWnd, UINT nID,
                    CFile* pPersist = NULL, BOOL bStorage = FALSE,
                    BSTR bstrLicKey = NULL)
        {
            return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID,
                                 pPersist, bStorage, bstrLicKey);
        }

        // Attributes
    public:
        short GetQBBackColor();
        void SetQBBackColor(short);
        short GetQBForeColor();
        void SetQBForeColor(short);
        CString GetFontName();
        void SetFontName(LPCTSTR);
        float GetFontSize();
        void SetFontSize(float);
        BOOL GetFontBold();
        void SetFontBold(BOOL);
        BOOL GetFontUnderline();
        void SetFontUnderline(BOOL);
        BOOL GetFontItalic();
        void SetFontItalic(BOOL);
        long GetHandshaking();
        void SetHandshaking(long);
        BOOL GetCDHolding();
        void SetCDHolding(BOOL);
        long GetProcessInput();
        void SetProcessInput(long);
        BOOL GetBreak();
        void SetBreak(BOOL);
        long GetCDTimeout();
        void SetCDTimeout(long);
        short GetCommID();
        void SetCommID(short);
        short GetCommPort();
        void SetCommPort(short);
        BOOL GetCTSHolding();
        void SetCTSHolding(BOOL);
        long GetCTSTimeout();
        void SetCTSTimeout(long);
        BOOL GetDSRHolding();
        void SetDSRHolding(BOOL);
        long GetDSRTimeout();
        void SetDSRTimeout(long);
        BOOL GetDTREnable();
        void SetDTREnable(BOOL);
        short GetCommEvent();
        void SetCommEvent(short);
        short GetInBufferSize();
        void SetInBufferSize(short);
        short GetInBufferCount();
        void SetInBufferCount(short);
        CString GetInput();
        void SetInput(LPCTSTR);
        short GetInputLen();
        void SetInputLen(short);
        long GetInTimeOut();
        void SetInTimeOut(long);
        CString GetLineInput();
        void SetLineInput(LPCTSTR);
        BOOL GetNullDiscard();
        void SetNullDiscard(BOOL);
        short GetOutBufferSize();
        void SetOutBufferSize(short);
        short GetOutBufferCount();
        void SetOutBufferCount(short);
        CString GetOutput();
        void SetOutput(LPCTSTR);
        CString GetParityReplace();
        void SetParityReplace(LPCTSTR);
        BOOL GetPortOpen();
        void SetPortOpen(BOOL);
        short GetRThreshold();
        void SetRThreshold(short);
        BOOL GetRTSEnable();
        void SetRTSEnable(BOOL);
        CString GetSettings();
        void SetSettings(LPCTSTR);
        short GetSThreshold();
        void SetSThreshold(short);
        short GetColumns();
        void SetColumns(short);
        short GetRows();
        void SetRows(short);
        short GetCursorRow();
        void SetCursorRow(short);
        short GetCursorColumn();
        void SetCursorColumn(short);
        CString GetOutputDisplay();
        void SetOutputDisplay(LPCTSTR);
        CString GetText();
        void SetText(LPCTSTR);
        long GetTerminalEmulation();
        void SetTerminalEmulation(long);
        BOOL GetEcho();
        void SetEcho(BOOL);
        long GetAutoSizeTerminal();
        void SetAutoSizeTerminal(long);
        long GetAutoScroll();
        void SetAutoScroll(long);
        CString GetCaptureFilename();
        void SetCaptureFilename(LPCTSTR);
        long GetCaptureMode();
        void SetCaptureMode(long);
        short GetScrollRows();
        void SetScrollRows(short);
        CString GetScrollText();
        void SetScrollText(LPCTSTR);
        long GetColorFilter();
        void SetColorFilter(long);
        CString GetDownload();
        void SetDownload(LPCTSTR);
        CString GetUpload();
        void SetUpload(LPCTSTR);
        CString GetTransferDestFilename();
        void SetTransferDestFilename(LPCTSTR);
        short GetTransferStatus();
        void SetTransferStatus(short);
        CString GetTransferSourceFilename();
        void SetTransferSourceFilename(LPCTSTR);
        long GetTransferTransferred();
        void SetTransferTransferred(long);
        long GetTransferFileSize();
        void SetTransferFileSize(long);
        long GetTransferProtocol();
        void SetTransferProtocol(long);
        long GetTransferStatusDialog();
        void SetTransferStatusDialog(long);
        long GetSelLength();
        void SetSelLength(long);
        long GetSelStart();
        void SetSelStart(long);
        short GetIntegerTag();
        void SetIntegerTag(short);
        CString GetSelText();
        void SetSelText(LPCTSTR);
        CString GetTransferMessage();
        void SetTransferMessage(LPCTSTR);
        long GetBackspace();
        void SetBackspace(long);
        BOOL GetAutoPortOpen();
        void SetAutoPortOpen(BOOL);
        long GetCursorStyle();
        void SetCursorStyle(long);
        long GetAutoDownload();
        void SetAutoDownload(long);
        long GetDialType();
        void SetDialType(long);
        CString GetPhoneNumber();
        void SetPhoneNumber(LPCTSTR);
        BOOL GetDial();
        void SetDial(BOOL);
        CString GetModemInit();
        void SetModemInit(LPCTSTR);
        CString GetModemType();
        void SetModemType(LPCTSTR);
        CString GetUploadPath();
        void SetUploadPath(LPCTSTR);
        CString GetDownloadPath();
        void SetDownloadPath(LPCTSTR);
        long GetTransferOptimization();
        void SetTransferOptimization(long);
        BOOL GetZModemDownloadRecovery();
        void SetZModemDownloadRecovery(BOOL);
        short GetTransferCPS();
        void SetTransferCPS(short);
        BOOL GetTransferIgnoreCD();
        void SetTransferIgnoreCD(BOOL);
        BOOL GetInstantScrollBars();
        void SetInstantScrollBars(BOOL);
        unsigned long GetForeColor();
        void SetForeColor(unsigned long);
        unsigned long GetBackColor();
        void SetBackColor(unsigned long);
        CString GetModemInitString();
        void SetModemInitString(LPCTSTR);
        unsigned long GetScrollSeparatorColor();
        void SetScrollSeparatorColor(unsigned long);
        long GetScrollSeparatorStyle();
        void SetScrollSeparatorStyle(long);
        COleFont GetFont();
        void SetFont(LPDISPATCH);
        short GetBorderStyle();
        void SetBorderStyle(short);
        BOOL GetEnabled();
        void SetEnabled(BOOL);
        BOOL GetTransferComBarUpdate();
        void SetTransferComBarUpdate(BOOL);
        CString Get_PhoneNumber();
        void Set_PhoneNumber(LPCTSTR);
        OLE_HANDLE GetHWnd();
        void SetHWnd(OLE_HANDLE);
        long GetMhComBarHWnd();
        void SetMhComBarHWnd(long);

        // Operations
    public:
        void Clear();
        void AboutBox();
};

#endif // __MHCOMM_H__
