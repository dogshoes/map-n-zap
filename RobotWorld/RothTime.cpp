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
  #include <winbase.h>
  #include "RothTime.h"
  
  #define TMAX 59999
  
  unsigned int Time_CurrentTime()
  /*******************************************************************************
  Created: 04/10/96 S.R.
  Last Revision: 04/10/96 S.R.
  Parameters:
  	none.
  Return Value:
  	The time in milliseconds since the current task started.
  ********************************************************************************
  Description:
  		This function returns the number of milliseconds since the application
  	started.
  Method:
  		The function uses the GetSystemTime function to get the system time, and
  	then uses the wSecond and wMilliseconds members to return the CurrentTime in
  	milliseconds.
  *******************************************************************************/
  {
  	SYSTEMTIME TimeStruct;
  
  	GetSystemTime(&TimeStruct);
  	return 1000*TimeStruct.wSecond + TimeStruct.wMilliseconds;
  }
  
  unsigned int Time_DT(unsigned int T1,unsigned int T2)
  /*******************************************************************************
  Created: 04/10/96 S.R.
  Last Revision: 04/10/96 S.R.
  Parameters:
  	T1,T2:	the times (in milliseconds) to calculate DT between.
  Return Value:
  	The DT (in milliseconds) calculated.
  ********************************************************************************
  Description:
  		This function calculates the difference in between times T1 and T2 in msec.
  Method:
  		if T2 >= T1 then the difference in times is just T2-T1.  Otherwise, the
  	timer rolled over and DT is T2-T1 + TMAX+1 where TMAX is the maximum time
  	measurable.
  *******************************************************************************/
  {
  	if (T2 >= T1) return T2-T1;
  	else return TMAX-T1+1+T2;
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
  
