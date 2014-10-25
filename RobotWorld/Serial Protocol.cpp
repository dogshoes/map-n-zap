/*******************************************************************************
//Serial Protocol.cpp
//Created: 04/10/96 S.R.
********************************************************************************
//Description:
//		This file includes the member function for the CCOMProtocol class.  These
//	functions handle outputting serial strings through the COM port using the
//	correct protocol.  They also handle reading serial strings from the COM port
//	making sure they conform to the correct protocol.
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
#include "RothTime.h"
#include "Serial Protocol.h"

#define STX			2		//The STX character to send
//#define STX			3		//The STX character to send
#define cTimeOut	150	//The time out time in milliseconds

char CCOMProtocol::GetSerialCharacter(BOOL* TimedOut)
/*******************************************************************************
//Created: 04/10/96 S.R.
//Last Revision: 04/12/96 S.R.
//Parameters:
//	TimedOut		True if the function times out before getting a char.
//					False otherwise.
//Return Value:
//		NULL if the function times out before getting a character, the character
//	read from the buffer otherwise.
********************************************************************************
//Description:
//		This function is used to get a single character from a serial buffer.  If
//	the function cannot get a character from the buffer in a given amount of time
//	it sets TimedOut to True and returns a NULL character.  When the user calls
//	this function for the first time he should set TimedOut to False.  That's
//	because if TimedOut is TRUE the function will return a NULL character.
//  		If you use this function to read in a string of characters, and one of the
//	characters causes a timeout error then the next times you call this function
//	(with the same TimedOut variable) it will stop reading from the serial buffer
//	and fill the rest of your string with NULLs.  Plus TimedOut will be True in
//	the end so you know that you have to handle a TimedOut error.
//Method:
//		The function uses the Time_CurrentTime and Time_DT functions to calculate
//	the time at which a TimeOut error should be returned.  Then, while there is
//	no character in the COM buffer and TimedOut is FALSE, the function sees if it
//	is time to give up and set TimeOut to TRUE.  Next, if the function timed out,
//	it returns #0.  Otherwise it returns the character read.
//		It is necessary to read the characters individually because the MhComm
//	control does not handle strings with embedded NULL characters.
*******************************************************************************/
{
    int nChar;
    unsigned int TStart;

    /*Get the start time or return NULL if timed out*/
    if (!*TimedOut)
    {
        TStart = Time_CurrentTime();
    }
    else
    {
        return NULL;
    }

    /*Wait for a character while you haven't timed out*/
    while (((nChar = CCOMPort::GetInBufferCount()) == 0) && (*TimedOut == FALSE))
    {
        if (Time_DT(TStart, Time_CurrentTime()) > cTimeOut)
        {
            *TimedOut = TRUE;
        }
    }

    /*Return the character if you have one or #0 if you don't*/
    if (nChar == 0)
    {
        return NULL;
    }
    else
    {
        TimedOut = FALSE;
        return CCOMPort::GetSerialChar();
    }
}

CString* CCOMProtocol::GetSerialData(unsigned char* Errors)
/*******************************************************************************
//Created: 04/10/96 S.R.
//Last Revision: 04/12/96 S.R.
//Parameters:
//	Errors:		The error code for any errors that happe.
//Return Value:
//	The string read in.
********************************************************************************
//Description:
//		This function is used to read a string from the serial port.  It assumes
//	that each string is preceded by a STX (#2) character, then the length of the
//	string followed by the string and a checksum.
//	If reading one of the characters causes a TimeOut error then the remaining
//	characters in the string are set to #0, and the function returns a
//	GSDTimedOutError.  If the checksum doesn't check out then it returns a
//	GSDCCheckSumError.  If the function doesn't read a STX as the first
//	character then it returns a GSDNoSTX error.
//	If any errors are registered, the function will return the characters read
//	from the serial buffer, and the corresponding error value.
//Method:
//		First off, the function tries to read a STX byte.  If it finds one, then
//	it tries to read the rest of the string.  If not then it stops and sets Errors
//	to GSDCheckSumError.
//		Then the function reads in the length of the string, and starts calculating
//	the checksum. The function reads the string and calculates the checksum by
//	adding the read character value to the checksum.  When the string is read the
//	function reads the correct checksum and compares it to the calculated checksum.
//		Because the function passes the same TimedOut variable to the
//	GetSerialCharacter function, if there is ever a timeout error, the function
//	stops reading from the serial buffer, and just fills the string up with NULL
//	characters.
//		After everything's read in, if any errors occurred the function returns
//	the corresponding error code in the	Errors parameter.
*******************************************************************************/
{
    unsigned char CheckSum;
    unsigned char StrLength;
    BOOL TimedOut = FALSE;
    CString* SerStr = NULL;
    *Errors = GSDNoErrors;

    /*Read in the STX character*/
    if (GetSerialCharacter(&TimedOut) == STX)
    {
        /*Get the string length and start the check sum*/
        StrLength = GetSerialCharacter(&TimedOut);
        unsigned char Sum = StrLength;
        SerStr = new CString('\000', StrLength);

        /*Read in the string and calculate the check sum*/
        for (unsigned char i = 0; i < StrLength; i++)
        {
            unsigned char temp = GetSerialCharacter(&TimedOut);
            SerStr->SetAt(i, temp);
            Sum += temp;
        }

        /*Read in the check sum and check it with the calculated value*/
        CheckSum = GetSerialCharacter(&TimedOut);

        if (Sum != CheckSum)
        {
            *Errors = GSDCheckSumError;
        }
    }
    else
    {
        *Errors = GSDNoSTX;
    }

    if (TimedOut)
    {
        *Errors = GSDTimeOutError;
    }

    return SerStr;
}

void CCOMProtocol::SendSerialData(CString* SerString)
/*******************************************************************************
//Created: 04/10/96 S.R.
//Last Revision: 04/12/96 S.R.
//Parameters:
//	SerString:		The string to send.
//Return Value:
//	none.
********************************************************************************
//Description:
//		This procedure is used to send a string through a serial port.  The string
//	is formatted so that first a start byte STX (#2) is sent then the length of the
//	string, then the string and finally a check sum.  The check sum is equal to
//	the sum of the characters in the string plus the length.  You must make sure
//	there is enough room in the serial output buffer for the string before trying
//	to send it.
//Method:
//		First the function finds the length of the SerString.  Then it creates a
//	new string that is the correct length to include the STX, Length and ChkSum
//	characters.
//		Then the function starts forming the output string.  It sticks the STX
//	character first, then the Length character.  Then the function starts putting
//	in the SerString and calculating the CheckSum.  Finally the CheckSum is put
//	on the end of the TXString, and the TXString is sent to the COM port by the
//	CCOMPort::SendSerialString() function.
//		In sending the string to the CommOutput you must be careful that the total
//	length of the string sent is less than 255 characters.
*******************************************************************************/
{
    /*Get the length of the string*/
    unsigned char nChars = SerString->GetLength();
    /*Start calculating the CheckSum*/
    unsigned char CheckSum = nChars;
    /*Initialize the transmission string*/
    CString TXString('\000', nChars + 3);
    //	CString TXString('\000',nChars + 4);
    /*Set the STX char*/
    //const unsigned char cSTX = 2;
    TXString.SetAt(0, STX);
    //	TXString.SetAt(1,STX);

    /*Set the string length char*/
    TXString.SetAt(1, nChars);

    //	TXString.SetAt(2,nChars);
    /*Calculate the check sum and set the rest of the transmission string*/
    for (unsigned char i = 0; i < nChars; i++)
    {
        CheckSum += SerString->GetAt(i);
        TXString.SetAt(i + 2, SerString->GetAt(i));
        //		TXString.SetAt(i+3,SerString->GetAt(i));
    }

    /*Set the check sum character*/
    TXString.SetAt(nChars + 2, CheckSum);
    //	TXString.SetAt(nChars+3,CheckSum);
    /*Send Out STX + String Length + string + the check sum*/
    CCOMPort::SendSerialString(&TXString);
    return;
}

/*******************************************************************************
Created: 04/10/96 S.R.
Last Revision: 04/10/96 S.R.
Parameters:

Return Value:

********************************************************************************
Description:

Method:

*******************************************************************************/