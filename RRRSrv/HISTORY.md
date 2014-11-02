# Headers

## ProtocolDiagnostics.h

    *****************  Version 5  *****************
    User: Harnden      Date: 10/13/99   Time: 10:53p
    Updated in $/RRR/RRRSrv
    Added BAd Address counter to Protocol Diagnostics dialog.
    
    *****************  Version 4  *****************
    User: Harnden      Date: 10/12/99   Time: 12:23a
    Updated in $/RRR/RRRSrv
    Added multi robot support.
    
    *****************  Version 3  *****************
    User: Harnden      Date: 8/09/99    Time: 10:03p
    Updated in $/RRR/RRRSrv
    Merged 1P5 changes into mainstream. Included changes from Stephan in
    RRRSrvCom.cpp.
    
    *****************  Version 4  *****************
    User: Harnden      Date: 4/27/99    Time: 2:07a
    Updated in $/RRR1P5 Branch/RRRSrv
    Incorporated fixes made to 1.04 software to make work on Pony.
    
    *****************  Version 4  *****************
    User: Harnden      Date: 4/18/99    Time: 10:04p
    Updated in $/RRR1P0 Branch/RRRSrv
    Added protocol diagnostics and fixed some minor problems.
    
    *****************  Version 2  *****************
    User: Harnden      Date: 5/26/98    Time: 10:51p
    Updated in $/RRR/RRRSrv
    First radio interface release sent to Cycle Time Corp.
    
    *****************  Version 1  *****************
    User: Harnden      Date: 5/26/98    Time: 12:07a
    Created in $/RRR/RRRSrv

## robot communication.h
    *****************  Version 13  *****************
    User: Harnden      Date: 10/12/99   Time: 12:23a
    Updated in $/RRR/RRRSrv
    Added multi robot support.
    
    *****************  Version 12  *****************
    User: Harnden      Date: 8/09/99    Time: 10:03p
    Updated in $/RRR/RRRSrv
    Merged 1P5 changes into mainstream. Included changes from Stephan in
    RRRSrvCom.cpp.
    
    *****************  Version 13  *****************
    User: Harnden      Date: 4/27/99    Time: 2:07a
    Updated in $/RRR1P5 Branch/RRRSrv
    Incorporated fixes made to 1.04 software to make work on Pony.
    
    *****************  Version 11  *****************
    User: Harnden      Date: 3/07/99    Time: 12:12p
    Updated in $/RRR/RRRSrv
    Added all functionality in Stephan's document. Converted name to
    CyeSrv.exe.
    
    *****************  Version 10  *****************
    User: Harnden      Date: 12/19/98   Time: 3:04p
    Updated in $/RRR/RRRSrv
    Added message priority support.
    
    *****************  Version 9  *****************
    User: Harnden      Date: 11/16/98   Time: 12:41a
    Updated in $/RRR/RRRSrv
    Added dynamic serial port change logic.
    
    *****************  Version 8  *****************
    User: Harnden      Date: 8/03/98    Time: 10:57p
    Updated in $/RRR/RRRSrv
    Added OnHomeBase parameter. Added OnBatteryChargeStateChange. Added
    PowerDown command. Added send and receive of raw data. Added
    CommStateChange event. Purge outgoing message queue when communications
    link is lost.
    
    *****************  Version 7  *****************
    User: Harnden      Date: 7/06/98    Time: 12:44a
    Updated in $/RRR/RRRSrv
    Added new RobotStatusUpdate event and changed SendFollowPath from BOOL
    to short.
    
    *****************  Version 6  *****************
    User: Harnden      Date: 6/22/98    Time: 9:11p
    Updated in $/RRR/RRRSrv
    Added three new messages and one event.
    
    *****************  Version 5  *****************
    User: Harnden      Date: 5/25/98    Time: 10:19a
    Updated in $/RRRSrv
    Working radio interface.
    
    *****************  Version 4  *****************
    User: Harnden      Date: 3/22/98    Time: 10:01p
    Updated in $/RRRSrv
    Added new functions for Stephan.
    
    *****************  Version 3  *****************
    User: Harnden      Date: 1/12/98    Time: 12:45a
    Updated in $/RRRSrv
    First delivery of ActiveX interface.
    
    *****************  Version 2  *****************
    User: Harnden      Date: 1/05/98    Time: 12:50a
    Updated in $/RRRSrv
    Full send message protocol support (except ExecutePath).
    
    *****************  Version 1  *****************
    User: Harnden      Date: 1/05/98    Time: 12:16a
    Created in $/RRRSrv

## RRRClist.h

    *****************  Version 3  *****************
    User: Harnden      Date: 10/12/99   Time: 12:23a
    Updated in $/RRR/RRRSrv
    Added multi robot support.
    
    *****************  Version 2  *****************
    User: Harnden      Date: 1/12/98    Time: 12:45a
    Updated in $/RRRSrv
    First delivery of ActiveX interface.

## RRRComEngine.h

    *****************  Version 3  *****************
    User: Harnden      Date: 10/16/99   Time: 5:30p
    Updated in $/RRR/RRRSrv
    Fixed RetryLimit get and set.
    
    *****************  Version 2  *****************
    User: Harnden      Date: 10/12/99   Time: 12:23a
    Updated in $/RRR/RRRSrv
    Added multi robot support.
    
    *****************  Version 1  *****************
    User: Harnden      Date: 10/03/99   Time: 3:11p
    Created in $/RRR/RRRSrv

## RRRComm.h

    *****************  Version 16  *****************
    User: Harnden      Date: 11/03/99   Time: 1:55a
    Updated in $/RRR/RRRSrv
    Made sure message timer was stopped when message received...
    
    *****************  Version 15  *****************
    User: Harnden      Date: 10/17/99   Time: 3:02p
    Updated in $/RRR/RRRSrv
    Rewrote message timeout and delivery logic to avoid duplicate messages.
    
    *****************  Version 14  *****************
    User: Harnden      Date: 10/16/99   Time: 5:30p
    Updated in $/RRR/RRRSrv
    Fixed RetryLimit get and set.
    
    *****************  Version 13  *****************
    User: Harnden      Date: 10/12/99   Time: 12:23a
    Updated in $/RRR/RRRSrv
    Added multi robot support.
    
    *****************  Version 12  *****************
    User: Harnden      Date: 8/09/99    Time: 10:03p
    Updated in $/RRR/RRRSrv
    Merged 1P5 changes into mainstream. Included changes from Stephan in
    RRRSrvCom.cpp.
    
    *****************  Version 13  *****************
    User: Harnden      Date: 4/27/99    Time: 2:07a
    Updated in $/RRR1P5 Branch/RRRSrv
    Incorporated fixes made to 1.04 software to make work on Pony.
    
    *****************  Version 13  *****************
    User: Harnden      Date: 4/18/99    Time: 10:04p
    Updated in $/RRR1P0 Branch/RRRSrv
    Added protocol diagnostics and fixed some minor problems.
    
    *****************  Version 11  *****************
    User: Harnden      Date: 11/28/98   Time: 9:38a
    Updated in $/RRR/RRRSrv
    Last minute changes and fix version info for release.
    
    *****************  Version 10  *****************
    User: Harnden      Date: 11/16/98   Time: 12:41a
    Updated in $/RRR/RRRSrv
    Added dynamic serial port change logic.
    
    *****************  Version 9  *****************
    User: Harnden      Date: 9/09/98    Time: 7:14a
    Updated in $/RRR/RRRSrv
    Added RetryLimit property. Fixed buffer overrun problem in receive
    logic. Tweaked RobotStatusUpdate logic.
    
    *****************  Version 8  *****************
    User: Harnden      Date: 8/21/98    Time: 9:38p
    Updated in $/RRR/RRRSrv
    Added INVERTDTR parameter to rrrsrv.ini file to invert the DTR toggling
    logic.
    
    *****************  Version 7  *****************
    User: Harnden      Date: 8/03/98    Time: 10:57p
    Updated in $/RRR/RRRSrv
    Added OnHomeBase parameter. Added OnBatteryChargeStateChange. Added
    PowerDown command. Added send and receive of raw data. Added
    CommStateChange event. Purge outgoing message queue when communications
    link is lost.
    
    *****************  Version 6  *****************
    User: Harnden      Date: 5/26/98    Time: 10:51p
    Updated in $/RRR/RRRSrv
    First radio interface release sent to Cycle Time Corp.
    
    *****************  Version 5  *****************
    User: Harnden      Date: 5/25/98    Time: 10:19a
    Updated in $/RRRSrv
    Working radio interface.
    
    *****************  Version 4  *****************
    User: Harnden      Date: 3/22/98    Time: 7:38p
    Updated in $/RRRSrv
    Updated status to include work at CTC.
    
    *****************  Version 3  *****************
    User: Harnden      Date: 1/17/98    Time: 4:32p
    Updated in $/RRRSrv
    First working copy tested against RobotSim.
    
    *****************  Version 2  *****************
    User: Harnden      Date: 1/12/98    Time: 12:45a
    Updated in $/RRRSrv
    First delivery of ActiveX interface.
    
    *****************  Version 1  *****************
    User: Harnden      Date: 1/11/98    Time: 9:49a
    Created in $/RRRSrv
    
    *****************  Version 2  *****************
    User: Harnden      Date: 1/05/98    Time: 12:50a
    Updated in $/RRRSrv
    Full send message protocol support (except ExecutePath).

## RRRConst.h

    *****************  Version 15  *****************
    User: Harnden      Date: 10/25/99   Time: 10:56p
    Updated in $/RRR/RRRSrv
    Added MicrophoneVolume event. Implemented SetRobotAddress. Removed all
    references to RobotAdress setting in CyeSrv.ini file.
    
    *****************  Version 14  *****************
    User: Harnden      Date: 10/17/99   Time: 3:02p
    Updated in $/RRR/RRRSrv
    Rewrote message timeout and delivery logic to avoid duplicate messages.
    
    *****************  Version 13  *****************
    User: Harnden      Date: 10/12/99   Time: 12:23a
    Updated in $/RRR/RRRSrv
    Added multi robot support.
    
    *****************  Version 12  *****************
    User: Harnden      Date: 3/07/99    Time: 12:12p
    Updated in $/RRR/RRRSrv
    Added all functionality in Stephan's document. Converted name to
    CyeSrv.exe.
    
    *****************  Version 11  *****************
    User: Harnden      Date: 12/19/98   Time: 3:04p
    Updated in $/RRR/RRRSrv
    Added message priority support.
    
    *****************  Version 10  *****************
    User: Harnden      Date: 12/03/98   Time: 12:07p
    Updated in $/RRR/RRRSrv
    Added RadioLevel support and fixed about box displays.
    
    *****************  Version 9  *****************
    User: Harnden      Date: 8/03/98    Time: 10:57p
    Updated in $/RRR/RRRSrv
    Added OnHomeBase parameter. Added OnBatteryChargeStateChange. Added
    PowerDown command. Added send and receive of raw data. Added
    CommStateChange event. Purge outgoing message queue when communications
    link is lost.
    
    *****************  Version 8  *****************
    User: Harnden      Date: 7/06/98    Time: 10:28p
    Updated in $/RRR/RRRSrv
    Added Heading parameter to RobotStatusUpdate event.
    
    *****************  Version 7  *****************
    User: Harnden      Date: 7/06/98    Time: 12:44a
    Updated in $/RRR/RRRSrv
    Added new RobotStatusUpdate event and changed SendFollowPath from BOOL
    to short.
    
    *****************  Version 6  *****************
    User: Harnden      Date: 6/22/98    Time: 9:11p
    Updated in $/RRR/RRRSrv
    Added three new messages and one event.
    
    *****************  Version 5  *****************
    User: Harnden      Date: 5/25/98    Time: 10:19a
    Updated in $/RRRSrv
    Working radio interface.
    
    *****************  Version 4  *****************
    User: Harnden      Date: 3/22/98    Time: 7:38p
    Updated in $/RRRSrv
    Updated status to include work at CTC.
    
    *****************  Version 3  *****************
    User: Harnden      Date: 1/12/98    Time: 12:45a
    Updated in $/RRRSrv
    First delivery of ActiveX interface.
    
    *****************  Version 2  *****************
    User: Harnden      Date: 1/03/98    Time: 5:58p
    Updated in $/RRRSrv
    Completion of all ActiveX method interfaces for RobotWorld.
    
    *****************  Version 1  *****************
    User: Harnden      Date: 12/29/97   Time: 6:48p
    Created in $/RRRSrv

## RRRMsg.h

    *****************  Version 7  *****************
    User: Harnden      Date: 10/12/99   Time: 12:23a
    Updated in $/RRR/RRRSrv
    Added multi robot support.
    
    *****************  Version 6  *****************
    User: Harnden      Date: 12/19/98   Time: 3:04p
    Updated in $/RRR/RRRSrv
    Added message priority support.
    
    *****************  Version 5  *****************
    User: Harnden      Date: 9/09/98    Time: 7:14a
    Updated in $/RRR/RRRSrv
    Added RetryLimit property. Fixed buffer overrun problem in receive
    logic. Tweaked RobotStatusUpdate logic.
    
    *****************  Version 4  *****************
    User: Harnden      Date: 8/03/98    Time: 10:57p
    Updated in $/RRR/RRRSrv
    Added OnHomeBase parameter. Added OnBatteryChargeStateChange. Added
    PowerDown command. Added send and receive of raw data. Added
    CommStateChange event. Purge outgoing message queue when communications
    link is lost.
    
    *****************  Version 3  *****************
    User: Harnden      Date: 5/25/98    Time: 10:19a
    Updated in $/RRRSrv
    Working radio interface.
    
    *****************  Version 2  *****************
    User: Harnden      Date: 1/05/98    Time: 12:50a
    Updated in $/RRRSrv
    Full send message protocol support (except ExecutePath).
    
    *****************  Version 1  *****************
    User: Harnden      Date: 1/03/98    Time: 6:17p
    Created in $/RRRSrv

## RRRSrv.h

    *****************  Version 8  *****************
    User: Harnden      Date: 10/25/99   Time: 10:56p
    Updated in $/RRR/RRRSrv
    Added MicrophoneVolume event. Implemented SetRobotAddress. Removed all
    references to RobotAdress setting in CyeSrv.ini file.
    
    *****************  Version 7  *****************
    User: Harnden      Date: 10/12/99   Time: 12:23a
    Updated in $/RRR/RRRSrv
    Added multi robot support.
    
    *****************  Version 6  *****************
    User: Harnden      Date: 11/16/98   Time: 12:41a
    Updated in $/RRR/RRRSrv
    Added dynamic serial port change logic.
    
    *****************  Version 5  *****************
    User: Harnden      Date: 8/21/98    Time: 9:38p
    Updated in $/RRR/RRRSrv
    Added INVERTDTR parameter to rrrsrv.ini file to invert the DTR toggling
    logic.
    
    *****************  Version 4  *****************
    User: Harnden      Date: 5/25/98    Time: 10:19a
    Updated in $/RRRSrv
    Working radio interface.
    
    *****************  Version 3  *****************
    User: Harnden      Date: 3/22/98    Time: 7:38p
    Updated in $/RRRSrv
    Updated status to include work at CTC.
    
    *****************  Version 2  *****************
    User: Harnden      Date: 1/05/98    Time: 12:50a
    Updated in $/RRRSrv
    Full send message protocol support (except ExecutePath).

## RRRSrvCom.h

    *****************  Version 18  *****************
    User: Harnden      Date: 10/17/99   Time: 3:02p
    Updated in $/RRR/RRRSrv
    Rewrote message timeout and delivery logic to avoid duplicate messages.
    
    *****************  Version 17  *****************
    User: Harnden      Date: 10/16/99   Time: 5:30p
    Updated in $/RRR/RRRSrv
    Fixed RetryLimit get and set.
    
    *****************  Version 16  *****************
    User: Harnden      Date: 10/13/99   Time: 10:53p
    Updated in $/RRR/RRRSrv
    Added BAd Address counter to Protocol Diagnostics dialog.
    
    *****************  Version 15  *****************
    User: Harnden      Date: 10/12/99   Time: 12:23a
    Updated in $/RRR/RRRSrv
    Added multi robot support.
    
    *****************  Version 14  *****************
    User: Harnden      Date: 8/09/99    Time: 10:46p
    Updated in $/RRR/RRRSrv
    Added new robot status packet support for software revision 9.
    
    *****************  Version 13  *****************
    User: Harnden      Date: 8/09/99    Time: 10:03p
    Updated in $/RRR/RRRSrv
    Merged 1P5 changes into mainstream. Included changes from Stephan in
    RRRSrvCom.cpp.
    
    *****************  Version 13  *****************
    User: Harnden      Date: 4/27/99    Time: 2:07a
    Updated in $/RRR1P5 Branch/RRRSrv
    Incorporated fixes made to 1.04 software to make work on Pony.
    
    *****************  Version 11  *****************
    User: Harnden      Date: 12/19/98   Time: 3:04p
    Updated in $/RRR/RRRSrv
    Added message priority support.
    
    *****************  Version 10  *****************
    User: Harnden      Date: 12/07/98   Time: 12:22a
    Updated in $/RRR/RRRSrv
    Added IDLE support to radio level logic.
    
    *****************  Version 9  *****************
    User: Harnden      Date: 12/03/98   Time: 12:07p
    Updated in $/RRR/RRRSrv
    Added RadioLevel support and fixed about box displays.
    
    *****************  Version 8  *****************
    User: Harnden      Date: 8/21/98    Time: 9:38p
    Updated in $/RRR/RRRSrv
    Added INVERTDTR parameter to rrrsrv.ini file to invert the DTR toggling
    logic.
    
    *****************  Version 7  *****************
    User: Harnden      Date: 8/03/98    Time: 10:57p
    Updated in $/RRR/RRRSrv
    Added OnHomeBase parameter. Added OnBatteryChargeStateChange. Added
    PowerDown command. Added send and receive of raw data. Added
    CommStateChange event. Purge outgoing message queue when communications
    link is lost.
    
    *****************  Version 6  *****************
    User: Harnden      Date: 5/26/98    Time: 10:51p
    Updated in $/RRR/RRRSrv
    First radio interface release sent to Cycle Time Corp.
    
    *****************  Version 5  *****************
    User: Harnden      Date: 5/25/98    Time: 10:19a
    Updated in $/RRRSrv
    Working radio interface.
    
    *****************  Version 4  *****************
    User: Harnden      Date: 3/22/98    Time: 7:38p
    Updated in $/RRRSrv
    Updated status to include work at CTC.
    
    *****************  Version 3  *****************
    User: Harnden      Date: 1/12/98    Time: 12:45a
    Updated in $/RRRSrv
    First delivery of ActiveX interface.
    
    *****************  Version 2  *****************
    User: Harnden      Date: 1/05/98    Time: 12:50a
    Updated in $/RRRSrv
    Full send message protocol support (except ExecutePath).

## Trayicon.h

    *****************  Version 2  *****************
    User: Harnden      Date: 3/22/98    Time: 7:38p
    Updated in $/RRRSrv
    Updated status to include work at CTC.
    
    *****************  Version 1  *****************
    User: Harnden      Date: 3/22/98    Time: 9:59a
    Created in $/RRRSrv

# C++

## ProtocolDiagnostics.cpp

    *****************  Version 5  *****************
    User: Harnden      Date: 10/13/99   Time: 10:53p
    Updated in $/RRR/RRRSrv
    Added BAd Address counter to Protocol Diagnostics dialog.
    
    *****************  Version 4  *****************
    User: Harnden      Date: 10/12/99   Time: 12:23a
    Updated in $/RRR/RRRSrv
    Added multi robot support.
    
    *****************  Version 3  *****************
    User: Harnden      Date: 8/09/99    Time: 10:03p
    Updated in $/RRR/RRRSrv
    Merged 1P5 changes into mainstream. Included changes from Stephan in
    RRRSrvCom.cpp.
    
    *****************  Version 4  *****************
    User: Harnden      Date: 4/27/99    Time: 2:07a
    Updated in $/RRR1P5 Branch/RRRSrv
    Incorporated fixes made to 1.04 software to make work on Pony.
    
    *****************  Version 4  *****************
    User: Harnden      Date: 4/18/99    Time: 10:04p
    Updated in $/RRR1P0 Branch/RRRSrv
    Added protocol diagnostics and fixed some minor problems.
    
    *****************  Version 2  *****************
    User: Harnden      Date: 5/26/98    Time: 10:51p
    Updated in $/RRR/RRRSrv
    First radio interface release sent to Cycle Time Corp.
    
    *****************  Version 1  *****************
    User: Harnden      Date: 5/26/98    Time: 12:07a
    Created in $/RRR/RRRSrv

## robot communication.cpp

    *****************  Version 13  *****************
    User: Harnden      Date: 10/12/99   Time: 12:23a
    Updated in $/RRR/RRRSrv
    Added multi robot support.
    
    *****************  Version 12  *****************
    User: Harnden      Date: 8/09/99    Time: 10:03p
    Updated in $/RRR/RRRSrv
    Merged 1P5 changes into mainstream. Included changes from Stephan in
    RRRSrvCom.cpp.
    
    *****************  Version 13  *****************
    User: Harnden      Date: 4/27/99    Time: 2:07a
    Updated in $/RRR1P5 Branch/RRRSrv
    Incorporated fixes made to 1.04 software to make work on Pony.
    
    *****************  Version 11  *****************
    User: Harnden      Date: 3/07/99    Time: 12:12p
    Updated in $/RRR/RRRSrv
    Added all functionality in Stephan's document. Converted name to
    CyeSrv.exe.
    
    *****************  Version 10  *****************
    User: Harnden      Date: 12/19/98   Time: 3:04p
    Updated in $/RRR/RRRSrv
    Added message priority support.
    
    *****************  Version 9  *****************
    User: Harnden      Date: 8/03/98    Time: 10:57p
    Updated in $/RRR/RRRSrv
    Added OnHomeBase parameter. Added OnBatteryChargeStateChange. Added
    PowerDown command. Added send and receive of raw data. Added
    CommStateChange event. Purge outgoing message queue when communications
    link is lost.
    
    *****************  Version 8  *****************
    User: Harnden      Date: 7/06/98    Time: 12:44a
    Updated in $/RRR/RRRSrv
    Added new RobotStatusUpdate event and changed SendFollowPath from BOOL
    to short.
    
    *****************  Version 7  *****************
    User: Harnden      Date: 6/22/98    Time: 9:11p
    Updated in $/RRR/RRRSrv
    Added three new messages and one event.
    
    *****************  Version 6  *****************
    User: Harnden      Date: 5/25/98    Time: 10:19a
    Updated in $/RRRSrv
    Working radio interface.
    
    *****************  Version 5  *****************
    User: Harnden      Date: 3/22/98    Time: 10:01p
    Updated in $/RRRSrv
    Added new functions for Stephan.
    
    *****************  Version 4  *****************
    User: Harnden      Date: 3/22/98    Time: 7:38p
    Updated in $/RRRSrv
    Updated status to include work at CTC.
    
    *****************  Version 3  *****************
    User: Harnden      Date: 1/12/98    Time: 12:45a
    Updated in $/RRRSrv
    First delivery of ActiveX interface.
    
    *****************  Version 2  *****************
    User: Harnden      Date: 1/05/98    Time: 12:50a
    Updated in $/RRRSrv
    Full send message protocol support (except ExecutePath).
    
    *****************  Version 1  *****************
    User: Harnden      Date: 1/05/98    Time: 12:16a
    Created in $/RRRSrv

## RRRClist.cpp

    *****************  Version 4  *****************
    User: Harnden      Date: 10/12/99   Time: 12:23a
    Updated in $/RRR/RRRSrv
    Added multi robot support.
    
    *****************  Version 3  *****************
    User: Harnden      Date: 1/17/98    Time: 4:32p
    Updated in $/RRRSrv
    First working copy tested against RobotSim.
    
    *****************  Version 2  *****************
    User: Harnden      Date: 1/12/98    Time: 12:45a
    Updated in $/RRRSrv
    First delivery of ActiveX interface.

## RRRComEngine.cpp

    *****************  Version 4  *****************
    User: Harnden      Date: 10/25/99   Time: 10:56p
    Updated in $/RRR/RRRSrv
    Added MicrophoneVolume event. Implemented SetRobotAddress. Removed all
    references to RobotAdress setting in CyeSrv.ini file.
    
    *****************  Version 3  *****************
    User: Harnden      Date: 10/13/99   Time: 10:57p
    Updated in $/RRR/RRRSrv
    Removed invalid reference to RRRCom.h.
    
    *****************  Version 2  *****************
    User: Harnden      Date: 10/12/99   Time: 12:23a
    Updated in $/RRR/RRRSrv
    Added multi robot support.
    
    *****************  Version 1  *****************
    User: Harnden      Date: 10/03/99   Time: 3:10p
    Created in $/RRR/RRRSrv

## RRRComm.cpp

    *****************  Version 15  *****************
    User: Harnden      Date: 11/04/99   Time: 1:26a
    Updated in $/RRR/RRRSrv
    Changed polling mechasim so message timer always runs...
    
    *****************  Version 14  *****************
    User: Harnden      Date: 11/03/99   Time: 1:55a
    Updated in $/RRR/RRRSrv
    Made sure message timer was stopped when message received...
    
    *****************  Version 13  *****************
    User: Harnden      Date: 10/17/99   Time: 3:02p
    Updated in $/RRR/RRRSrv
    Rewrote message timeout and delivery logic to avoid duplicate messages.
    
    *****************  Version 12  *****************
    User: Harnden      Date: 10/16/99   Time: 5:30p
    Updated in $/RRR/RRRSrv
    Fixed RetryLimit get and set.
    
    *****************  Version 11  *****************
    User: Harnden      Date: 10/12/99   Time: 12:23a
    Updated in $/RRR/RRRSrv
    Added multi robot support.
    
    *****************  Version 10  *****************
    User: Harnden      Date: 8/09/99    Time: 10:03p
    Updated in $/RRR/RRRSrv
    Merged 1P5 changes into mainstream. Included changes from Stephan in
    RRRSrvCom.cpp.
    
    *****************  Version 11  *****************
    User: Harnden      Date: 4/27/99    Time: 2:07a
    Updated in $/RRR1P5 Branch/RRRSrv
    Incorporated fixes made to 1.04 software to make work on Pony.
    
    *****************  Version 11  *****************
    User: Harnden      Date: 4/18/99    Time: 10:04p
    Updated in $/RRR1P0 Branch/RRRSrv
    Added protocol diagnostics and fixed some minor problems.
    
    *****************  Version 9  *****************
    User: Harnden      Date: 9/09/98    Time: 7:14a
    Updated in $/RRR/RRRSrv
    Added RetryLimit property. Fixed buffer overrun problem in receive
    logic. Tweaked RobotStatusUpdate logic.
    
    *****************  Version 8  *****************
    User: Harnden      Date: 8/21/98    Time: 9:38p
    Updated in $/RRR/RRRSrv
    Added INVERTDTR parameter to rrrsrv.ini file to invert the DTR toggling
    logic.
    
    *****************  Version 7  *****************
    User: Harnden      Date: 8/03/98    Time: 10:57p
    Updated in $/RRR/RRRSrv
    Added OnHomeBase parameter. Added OnBatteryChargeStateChange. Added
    PowerDown command. Added send and receive of raw data. Added
    CommStateChange event. Purge outgoing message queue when communications
    link is lost.
    
    *****************  Version 6  *****************
    User: Harnden      Date: 6/22/98    Time: 9:11p
    Updated in $/RRR/RRRSrv
    Added three new messages and one event.
    
    *****************  Version 5  *****************
    User: Harnden      Date: 5/25/98    Time: 10:19a
    Updated in $/RRRSrv
    Working radio interface.
    
    *****************  Version 4  *****************
    User: Harnden      Date: 3/22/98    Time: 7:38p
    Updated in $/RRRSrv
    Updated status to include work at CTC.
    
    *****************  Version 3  *****************
    User: Harnden      Date: 1/17/98    Time: 4:32p
    Updated in $/RRRSrv
    First working copy tested against RobotSim.
    
    *****************  Version 2  *****************
    User: Harnden      Date: 1/12/98    Time: 12:45a
    Updated in $/RRRSrv
    First delivery of ActiveX interface.
    
    *****************  Version 1  *****************
    User: Harnden      Date: 1/11/98    Time: 9:49a
    Created in $/RRRSrv
    
    *****************  Version 2  *****************
    User: Harnden      Date: 1/05/98    Time: 12:50a
    Updated in $/RRRSrv
    Full send message protocol support (except ExecutePath).

## RRRMsg.cpp

    *****************  Version 6  *****************
    User: Harnden      Date: 10/12/99   Time: 12:23a
    Updated in $/RRR/RRRSrv
    Added multi robot support.
    
    *****************  Version 5  *****************
    User: Harnden      Date: 8/09/99    Time: 10:03p
    Updated in $/RRR/RRRSrv
    Merged 1P5 changes into mainstream. Included changes from Stephan in
    RRRSrvCom.cpp.
    
    *****************  Version 6  *****************
    User: Harnden      Date: 4/27/99    Time: 2:07a
    Updated in $/RRR1P5 Branch/RRRSrv
    Incorporated fixes made to 1.04 software to make work on Pony.
    
    *****************  Version 6  *****************
    User: Harnden      Date: 4/18/99    Time: 10:04p
    Updated in $/RRR1P0 Branch/RRRSrv
    Added protocol diagnostics and fixed some minor problems.
    
    *****************  Version 4  *****************
    User: Harnden      Date: 12/19/98   Time: 3:04p
    Updated in $/RRR/RRRSrv
    Added message priority support.
    
    *****************  Version 3  *****************
    User: Harnden      Date: 5/25/98    Time: 10:19a
    Updated in $/RRRSrv
    Working radio interface.
    
    *****************  Version 2  *****************
    User: Harnden      Date: 1/05/98    Time: 12:50a
    Updated in $/RRRSrv
    Full send message protocol support (except ExecutePath).
    
    *****************  Version 1  *****************
    User: Harnden      Date: 1/03/98    Time: 6:17p
    Created in $/RRRSrv

## RRRSrv.cpp

    *****************  Version 9  *****************
    User: Harnden      Date: 10/25/99   Time: 10:56p
    Updated in $/RRR/RRRSrv
    Added MicrophoneVolume event. Implemented SetRobotAddress. Removed all
    references to RobotAdress setting in CyeSrv.ini file.
    
    *****************  Version 8  *****************
    User: Harnden      Date: 10/12/99   Time: 12:23a
    Updated in $/RRR/RRRSrv
    Added multi robot support.
    
    *****************  Version 7  *****************
    User: Harnden      Date: 11/16/98   Time: 12:41a
    Updated in $/RRR/RRRSrv
    Added dynamic serial port change logic.
    
    *****************  Version 6  *****************
    User: Harnden      Date: 8/21/98    Time: 9:38p
    Updated in $/RRR/RRRSrv
    Added INVERTDTR parameter to rrrsrv.ini file to invert the DTR toggling
    logic.
    
    *****************  Version 5  *****************
    User: Harnden      Date: 5/25/98    Time: 10:19a
    Updated in $/RRRSrv
    Working radio interface.
    
    *****************  Version 4  *****************
    User: Harnden      Date: 3/22/98    Time: 7:38p
    Updated in $/RRRSrv
    Updated status to include work at CTC.
    
    *****************  Version 3  *****************
    User: Harnden      Date: 1/17/98    Time: 4:32p
    Updated in $/RRRSrv
    First working copy tested against RobotSim.
    
    *****************  Version 2  *****************
    User: Harnden      Date: 1/05/98    Time: 12:50a
    Updated in $/RRRSrv
    Full send message protocol support (except ExecutePath).

## RRRSrvCom.cpp

    *****************  Version 28  *****************
    User: Harnden      Date: 11/04/99   Time: 1:26a
    Updated in $/RRR/RRRSrv
    Changed polling mechasim so message timer always runs...
    
    *****************  Version 27  *****************
    User: Harnden      Date: 10/17/99   Time: 3:02p
    Updated in $/RRR/RRRSrv
    Rewrote message timeout and delivery logic to avoid duplicate messages.
    
    *****************  Version 26  *****************
    User: Harnden      Date: 10/16/99   Time: 5:30p
    Updated in $/RRR/RRRSrv
    Fixed RetryLimit get and set.
    
    *****************  Version 25  *****************
    User: Harnden      Date: 10/13/99   Time: 10:53p
    Updated in $/RRR/RRRSrv
    Added BAd Address counter to Protocol Diagnostics dialog.
    
    *****************  Version 24  *****************
    User: Harnden      Date: 10/12/99   Time: 12:23a
    Updated in $/RRR/RRRSrv
    Added multi robot support.
    
    *****************  Version 23  *****************
    User: Harnden      Date: 8/25/99    Time: 12:30a
    Updated in $/RRR/RRRSrv
    Changes from Stephan.
    
    *****************  Version 22  *****************
    User: Harnden      Date: 8/09/99    Time: 10:46p
    Updated in $/RRR/RRRSrv
    Added new robot status packet support for software revision 9.
    
    *****************  Version 21  *****************
    User: Harnden      Date: 8/09/99    Time: 10:03p
    Updated in $/RRR/RRRSrv
    Merged 1P5 changes into mainstream. Included changes from Stephan in
    RRRSrvCom.cpp.
    
    *****************  Version 21  *****************
    User: Harnden      Date: 4/27/99    Time: 2:07a
    Updated in $/RRR1P5 Branch/RRRSrv
    Incorporated fixes made to 1.04 software to make work on Pony.
    
    *****************  Version 19  *****************
    User: Harnden      Date: 3/07/99    Time: 8:46p
    Updated in $/RRR/RRRSrv
    Added fix for reception of unknown messages with piggy back acks.
    
    *****************  Version 18  *****************
    User: Harnden      Date: 3/07/99    Time: 12:12p
    Updated in $/RRR/RRRSrv
    Added all functionality in Stephan's document. Converted name to
    CyeSrv.exe.
    
    *****************  Version 17  *****************
    User: Harnden      Date: 12/19/98   Time: 3:04p
    Updated in $/RRR/RRRSrv
    Added message priority support.
    
    *****************  Version 16  *****************
    User: Harnden      Date: 12/07/98   Time: 12:22a
    Updated in $/RRR/RRRSrv
    Added IDLE support to radio level logic.
    
    *****************  Version 15  *****************
    User: Harnden      Date: 12/03/98   Time: 12:07p
    Updated in $/RRR/RRRSrv
    Added RadioLevel support and fixed about box displays.
    
    *****************  Version 14  *****************
    User: Harnden      Date: 11/28/98   Time: 9:38a
    Updated in $/RRR/RRRSrv
    Last minute changes and fix version info for release.
    
    *****************  Version 13  *****************
    User: Harnden      Date: 9/09/98    Time: 7:14a
    Updated in $/RRR/RRRSrv
    Added RetryLimit property. Fixed buffer overrun problem in receive
    logic. Tweaked RobotStatusUpdate logic.
    
    *****************  Version 12  *****************
    User: Harnden      Date: 8/21/98    Time: 9:38p
    Updated in $/RRR/RRRSrv
    Added INVERTDTR parameter to rrrsrv.ini file to invert the DTR toggling
    logic.
    
    *****************  Version 11  *****************
    User: Harnden      Date: 8/03/98    Time: 10:57p
    Updated in $/RRR/RRRSrv
    Added OnHomeBase parameter. Added OnBatteryChargeStateChange. Added
    PowerDown command. Added send and receive of raw data. Added
    CommStateChange event. Purge outgoing message queue when communications
    link is lost.
    
    *****************  Version 10  *****************
    User: Harnden      Date: 7/06/98    Time: 10:28p
    Updated in $/RRR/RRRSrv
    Added Heading parameter to RobotStatusUpdate event.
    
    *****************  Version 9  *****************
    User: Harnden      Date: 7/06/98    Time: 12:44a
    Updated in $/RRR/RRRSrv
    Added new RobotStatusUpdate event and changed SendFollowPath from BOOL
    to short.
    
    *****************  Version 8  *****************
    User: Harnden      Date: 6/22/98    Time: 9:11p
    Updated in $/RRR/RRRSrv
    Added three new messages and one event.
    
    *****************  Version 7  *****************
    User: Harnden      Date: 5/26/98    Time: 10:51p
    Updated in $/RRR/RRRSrv
    First radio interface release sent to Cycle Time Corp.
    
    *****************  Version 6  *****************
    User: Harnden      Date: 5/25/98    Time: 10:19a
    Updated in $/RRRSrv
    Working radio interface.
    
    *****************  Version 5  *****************
    User: Harnden      Date: 3/22/98    Time: 7:38p
    Updated in $/RRRSrv
    Updated status to include work at CTC.
    
    *****************  Version 4  *****************
    User: Harnden      Date: 1/17/98    Time: 4:32p
    Updated in $/RRRSrv
    First working copy tested against RobotSim.
    
    *****************  Version 3  *****************
    User: Harnden      Date: 1/12/98    Time: 12:45a
    Updated in $/RRRSrv
    First delivery of ActiveX interface.
    
    *****************  Version 2  *****************
    User: Harnden      Date: 1/05/98    Time: 12:50a
    Updated in $/RRRSrv
    Full send message protocol support (except ExecutePath).

## Trayicon.cpp

    *****************  Version 2  *****************
    User: Harnden      Date: 3/22/98    Time: 7:38p
    Updated in $/RRRSrv
    Updated status to include work at CTC.
    
    *****************  Version 1  *****************
    User: Harnden      Date: 3/22/98    Time: 9:59a
    Created in $/RRRSrv