/*******************************************************************************
//Serial Communications.cpp
//Created: 03/20/96 S.R.
********************************************************************************
//Description:
//
*******************************************************************************/

/*Function Header template*/
/*******************************************************************************
//Created: 04/10/96 S.R.
//Last Revision: 04/10/96 S.R.
//Parameters:
//
//Return Value:
//
********************************************************************************
//Description:
//
//Method:
//
*******************************************************************************/
#include "stdafx.h"
#include "Serial Communications.h"

void CCOMPort::SendSerialString(CString* String)
/*******************************************************************************
//Created: 03/20/96 S.R.
//Last Revision: 12/11/96 S.R.
//Parameters:
//	String	The string to be put out through the serial port.
//Return Value:
//	none
********************************************************************************
//Description:
//		This function sends a string through the MHComm communications control.
//Method:
//		If the string is of 0 length, the function just returns.  Otherwise, it
//	goes through the string and breaks it into pieces that do not contain
//	embedded NULL characters.
//		The SetOutput member function of the MHComm control is unable to output
//	NULL characters.  It is expecting a NULL terminated string as an argument
//	so giving it a string with an embedded NULL cuts off all characters after
//	the first NULL.
//		To break up the string, the SpanExcluding CString member function is used
//	to get the first string of characters not containing a NULL character.  This
//	string is output by the SetOutput function.  Then, if there was a NULL
//	character in the original string, it is output using the WriteFile Win32 API
//	function.  The COM port's windows device handle is obtained by the GetCommID
//	MHComm member function.
//		To output the rest of the string, the remaining characters of the string
//	are output by recursively calling SendSerialString.  The remaining characters
//	are obtained using the Right member function of CString.
*******************************************************************************/
{
    /*make sure there's a string to output*/
    if (String == NULL)
    {
        return;
    }

    if (String->GetLength() == 0)
    {
        return;
    }

    /*output the first string of characters not including a NULL*/
    CString tempString = String->SpanExcluding("\0");

    if (tempString.GetLength() != 0)
    {
        SetOutput(tempString);
    }

    /*if there was an embedded null character then the length of tempStr != nChars*/
    int temp = String->GetLength() - tempString.GetLength();

    if (temp != 0)
    {
        /*Output a NULL char using WriteFile Win32API function*/
        DWORD temp1;
        OVERLAPPED tOverlapped;
        WriteFile((void*)(GetCommID()), "\0", 1, &temp1, &tOverlapped);
        /*Recurse on the remaining characters*/
        tempString = String->Right(temp - 1);
        SendSerialString(&tempString);
    }

    return;
}

char CCOMPort::GetSerialChar()
/*******************************************************************************
//Created: 04/09/96 S.R.
//Last Revision: 12/11/96 S.R.
//Parameters:
//	none.
//Return Value:
//		The character received or NULL if no character was in the buffer.  Note!!
//	This function will return NULL if no character was in the buffer OR if a NULL
//	character was in the buffer.
********************************************************************************
//Description:
//	This function gets a single character from the serial port's input buffer.
//***NOTE!!! InputLen needs to be set to 1 for this function to work properly!****
//Method:
//		The GetInput member function of MHComm does not handle NULL characters
//	very well.  To be able to handle NULL characters, the input buffer needs to
//	read one character at a time, and if the GetInput function doesn't return
//	a character then there was a NULL at the head of the buffer.
//		First the function checks to see if there are characters in the buffer.
//	If there aren't, NULL is returned.  If there are, GetInput is used
//	to read in the lead character.  If no string is returned, there was a NULL
//	character at the head.  Otherwise, the first character of the string is
//	returned.
*******************************************************************************/
{
#ifdef _DEBUG

    if (GetInputLen() != 1)
    {
        AfxMessageBox("MhComm InputLen != 1.  Fix This.", MB_OK, 0);
    }

#endif


    if (GetInBufferCount() > 0)
    {
        CString tString = GetInput();

        if (tString.GetLength() < 1)
        {
            return NULL;
        }
        else
        {
            return tString[0];
        }
    }

    return NULL;
}
