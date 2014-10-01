********************************************
Probotics Open Source Release
Version 2.1x
1/20/2000
********************************************


This is the first release of Probotics Inc's Map-N-Zap application
source code. It is a mirror of Version 2.1, with minor changes to remove
our use of proprietary software which could not be released. Also, some
preliminary code to interface to Video For Windows devices has been added,
which is not in V2.1.

This release is under the Gnu Public License, version 2.0. Please read 
License.txt, which is included, for details.

This source code is known to compile under Microsoft Visual C++ 6.0. It
has not been ported to any other platform.

Contents
-------------------------------------------------------------------------
 RobotWorld - This directory contains the source code for the 
              Map-N-Zap application
 RRRCom     - This directory contains the source code for the Cye OCX
 RRRSrv     - This directory contains the source code for the Cye radio 
              interface and communications server


Compiliation and execution under VC++:
-------------------------------------------------------------------------
1) Load in the VC workspace file, RobotWorld\RobotWorld.dsw. This will
   load in 4 projects: FlowChart, RobotWorld, RRRCom, and RRRSrv.
2) Set RRRSrv as the active project, and compile it. Then run RRRSrv.
   This will bring up an empty window. After it appears, just exit the 
   program. 
3) Set RRRCom as the active project, and compile it. This installs the 
   Cye OCX, which Map-N-Zap uses.
4) Set RobotWorld as the active project, and compile. This can take a 
   while. On our setup, we get a number of errors the first time 
   we compile. All these errors relate to the use of AfxCollection 
   classes. Let the compilation proceed. After it is finished, load
   StdAfx.cpp, make a small change to it to force it to be re-saved, and
   re-compile. At this point, re-compilation of the previously 
   uncompiled files should occur without an error. We have no idea why
   this happens. If you figure it out, please let us know. 
5) You're done! Just run Map-N-Zap, and go. 

Support:
-------------------------------------------------------------------------
 Basically, we can't offer any. We have set up a developer's mailing list,
 which you can join by going to [REF]. We will monitor the list as time
 permits, but it is mainly for you to talk with other developers who are
 using Cye. 

