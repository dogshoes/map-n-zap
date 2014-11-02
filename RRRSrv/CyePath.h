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
// Machine generated IDispatch wrapper class(es) created with ClassWizard
/////////////////////////////////////////////////////////////////////////////
// ICyePath wrapper class

/// <summary>
/// A path segment the robot should follow.
/// </summary>
/// <remarks>
/// The implementation for this class does not live here, in RRRSrv.  The implementation in this project
/// dispatches over to the actual implementation in the RRRpath class in RRRCom project.
/// </remarks>
class ICyePath : public COleDispatchDriver
{
    public:
        ICyePath() {}		// Calls COleDispatchDriver default constructor
        ICyePath(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
        ICyePath(const ICyePath& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

        /// <summary>
        /// Get the number of points on this path segment.
        /// </summary>
        /// <returns>The number of points on this path segment.</returns>
        short GetNumPoints();

        /// <summary>
        /// Set the number of points on this path segment.
        /// </summary>
        /// <param name="propVal">The number of points on this path segment.</param>
        void SetNumPoints(short propVal);

        /// <summary>
        /// Get the X value for a point on this path segment.
        /// </summary>
        /// <remarks>
        /// The point index must within the range defined by GetNumPoints.
        /// </remarks>
        /// <param name="Index">The point to get the X value for.</param>
        /// <returns>The X value for the point on this path segment.</returns>
        double GetX(short Index);

        /// <summary>
        /// Set the X value for a point on this path segment.
        /// </summary>
        /// <remarks>
        /// The point index must within the range defined by SetNumPoints.
        /// </remarks>
        /// <param name="Index">The point to get the X value for.</param>
        /// <param name="newValue">The X value for the point on this path segment.</param>
        void SetX(short Index, double newValue);

        /// <summary>
        /// Get the Y value for a point on this path segment.
        /// </summary>
        /// <remarks>
        /// The point index must within the range defined by GetNumPoints.
        /// </remarks>
        /// <param name="Index">The point to get the Y value for.</param>
        /// <returns>The Y value for the point on this path segment.</returns>
        double GetY(short Index);

        /// <summary>
        /// Set the Y value for a point on this path segment.
        /// </summary>
        /// <remarks>
        /// The point index must within the range defined by SetNumPoints.
        /// </remarks>
        /// <param name="Index">The point to get the Y value for.</param>
        /// <param name="newValue">The Y value for the point on this path segment.</param>
        void SetY(short Index, double newValue);
};