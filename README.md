## Probotics' Map-N-Zap Cye Robot Control Software

![A view of the Cye robot on the Map-N-Zap grid showing areas the robot can navigate to.](https://raw.github.com/dogshoes/map-n-zap/screenshots/masthead.png)

### An Explanation is in Order

Probotics Inc. released the Cye Personal Robot in 1999.  It's a little two-wheeled remote-controlled robot which can map and traverse a house.  The robot itself has no intelligence and is intrinsically tied to a computer running this, the Map-N-Zap software, to locomote.  The Cye has an innovative way of sensing its environment.  Rather than using tactile or optical ranging sensors to detect obstacles it instead has a sensitive rotary encoder on one of the wheels used to determine its position and whether it has run into something.

### Overview

This is the first and only open source release of Probotics Inc's Map-N-Zap application.  It is a mirror of version 2.1 from January 20th, 2000.   Minor changes have been made to remove use of proprietary software which could not be released.  In addition, some preliminary code to interface to Video For Windows devices has been added, which is not in the shipped version of 2.1.

This release is provided under the GNU General Public License, version 2.0.  Please read [LICENSE.txt](LICENSE.txt), which is included, for details.

### Development Environment

This source code was originally developed under Microsoft Visual C++ 6.  It has since been migrated to Visual C++ 2008 Standard Edition.  To compile the project you will need the following:

* Microsoft Visual C++ 2008 Standard Edition (and possibly later)
* The Microsoft [DirectX Software Development Kit](http://www.microsoft.com/en-us/download/details.aspx?id=6812)

### Contents

The **[RobotWorld](RobotWorld)** directory contains the source code for Map-N-Zap.exe.  This is the main GUI and command software for the Cye robot.

The **[RobotWorld/FlowZap](RobotWorld/FlowZap)** directory contains the source code for the Flow Zap script editor used in Map-N-Zap.exe and in the the free-standing Flow Zap script editor FlowChart.exe.  This free-standing editor does not compile at this time.

The **[RRRCom](RRRCom)** directory contains the source code for the CyeCom.ocx control which provides an API to control the Cye robot.

The **[RRRSrv](RRRSrv)** directory contains the source code for the CyeSrv.exe radio interface and communications server.