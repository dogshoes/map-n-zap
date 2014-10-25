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
#include "stdafx.h"
#include "util.h"


/* MATH FUNCTIONS */
int GetMin(int a, int b)
{
    if (a < b)
    {
        return a;
    }
    else
    {
        return b;
    }
}

int GetMax(int a, int b)
{
    if (a > b)
    {
        return a;
    }
    else
    {
        return b;
    }
}

int Power(int num, int power)
{
    int result = 1;

    while (power > 0)
    {
        result *= num;
        power --;
    }

    return result;
}

int Abs(int num)
{
    if (num > 0)
    {
        return num;
    }
    else
    {
        return num - (2 * num);
    }
}







/* PARSING FUNCTIONS */
void Split(CString key, CString line, CStringList& words)
{
    words.RemoveAll();
    CString temp = "temp";
    int length, lineLength;
    lineLength = line.GetLength();

    while (lineLength > 0)
    {
        temp = line.SpanExcluding(key);
        words.AddTail(temp);
        length = temp.GetLength();
        lineLength = line.GetLength();
        line = line.Right(lineLength - length - 1);
    }

    words.RemoveTail();
}









/* TYPE CONVERSTION FUNCTIONS */
CString DoubleToString(double value, int precision)    // precision is the number of spaces after the decimal space
{
    CString result;
    int     decimal,   sign;

    for (int i = 1; i < 7; i ++)
    {
        if (value < Power(10, i))
        {
            precision += (i);
            break;
        }
    }

    result = _ecvt(value, precision, &decimal, &sign);
    CString left = result.Left(decimal);
    CString right = result.Right(result.GetLength() - decimal);
    result = left + "." + right;


    return result;

}


CString IntToString(int value)
{

    CString result;
    BOOL negNum = FALSE;

    if (value < 0)
    {
        value = Abs(value);
        negNum = TRUE;
    }

    int decimal, precision, sign;
    precision = 0;

    for (int i = 1; i < 7; i ++)
    {
        if (value < Power(10, i))
        {
            precision = i;
            break;
        }
    }

    result = _ecvt(value, precision, &decimal, &sign);

    if (negNum)
    {
        result = "-" + result;
    }

    return result;

}


CString BoolToString(BOOL value)
{
    if (value)
    {
        return "TRUE";
    }
    else
    {
        return "FALSE";
    }
}

BOOL StringToBool(CString string)
{
    if (string == "TRUE" || string == "TRUE" || string == "t" || string == "T")
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }

}



/*FILE MANIPULATION */

CStdioFile* OpenFileForReading(CString FileName)
{
    CStdioFile * file = NULL;
    TRY
    {

        file =  new CStdioFile(FileName, CFile::modeRead  | CFile::shareExclusive | CFile::typeText);
    }
    CATCH(CFileException, fe)
    {
        file = NULL;
    }
    END_CATCH

    return file;
}


CStdioFile* OpenFileForWriting(CString FileName)
{
    CStdioFile * file = NULL;
    TRY
    {
        file =  new CStdioFile(FileName, CFile::modeCreate | CFile::modeWrite | CFile::shareExclusive | CFile::typeText);
    }
    CATCH(CFileException, fe)
    {
        file =  NULL;
    }
    END_CATCH

    return file;


    /*TRY{   CStdioFile f3( pFileName,
        CFile::modeCreate | CFile::modeWrite | CFile::typeText );}
    CATCH( CFileException, e ){   #ifdef _DEBUG
        afxDump << "File could not be opened "              << e->m_cause << "\n";
     #endif}END_CATCH
     */
}
