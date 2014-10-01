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
 /*Constant values for the MSCOMM serial communications control*/
 
 /*Handshake Constants*/
 
 #define comNone			0	/*No handshaking.*/
 #define comXonXoff			/*XOn/XOff handshaking.*/
 #define comRTS				2	/*Request-to-send/clear-to-send handshaking.*/
 #define comRTSXOnXOff	3	/*Both request-to-send and XOn/XOff handshaking.*/
 
 /*OnComm Constants*/
 
 #define comEvSend			1	/*Send event.*/
 #define comEvReceive		2	/*Receive event.*/
 #define comEvCTS			3	/*Change in clear-to-send line.*/
 #define comEvDSR			4	/*Change in data-set ready line.*/
 #define comEvCD			5	/*Change in carrier detect line.*/
 #define comEvRing			6	/*Ring detect.*/
 #define comEvEOF			7	/*End of file.*/
 
 /*Error Constants*/
 
 #define comBreak			1001	/*Break signal received.*/
 #define comCTSTO			1002	/*Clear-to-send timeout.*/
 #define comDSRTO			1003	/*Data-set ready timeout.*/
 #define comFrame			1004	/*Framing error.*/
 #define comOverrun		1006	/*Port overrun.*/
 #define comCDTO			1007	/*Carrier detect timeout.*/
 #define comRxOver			1008	/*Receive buffer overflow.*/
 #define comRxParity		1009	/*Parity error.*/
 #define comTxFull			1010	/*Transmit buffer full.*/