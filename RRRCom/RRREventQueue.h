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
// RRRCList.h : interface of the RRRClistEntry & RRRClist classes
//
/////////////////////////////////////////////////////////////////////////////

#ifndef RRREVENTQUEUE
#define RRREVENTQUEUE

class RRREventQueueEntry : public CObject
{
        DECLARE_DYNAMIC(RRREventQueueEntry)

    public:
        RRREventQueueEntry(UINT msg, WPARAM wparam, LPARAM lparam);
        ~RRREventQueueEntry();

        // Attributes
    public:

        // Operations
    public:

        // Overrides

        // Implementation
    private:

    public:
        UINT Msg;
        WPARAM wParam;
        LPARAM lParam;
};

class RRREventQueue : public CObArray
{
        DECLARE_DYNAMIC(RRREventQueue)

    public:
        RRREventQueue();
        ~RRREventQueue();

        // Attributes
    public:

        // Operations
    public:
        void AddEntry(UINT Msg, WPARAM wParam, LPARAM lParam);
        void RepostEvents(HWND hWnd);

        // Overrides

        // Implementation
    private:
        CSemaphore m_oSem;                  // multithreading lock support

    public:
};

#endif
/////////////////////////////////////////////////////////////////////////////