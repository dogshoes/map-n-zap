## CyeSrv.exe

This project compiles CyeSrv.exe, the server which handles wireless communication with the Cye robot.  A single CyeSrv.exe allows multiple programs (running [CyeCom.ocx](../RRRCom)) to communicate with one or many robots within range.

### Compiling and Registering

This program is a DCOM server that exports [CyeSrv.Document](CyeSrv.odl) allowing [CyeCom.ocx](../RRRCom) to dispatch calls to the server.  In order to allow this to happen CyeSrv.exe needs to be registered with the system.  After registration CyeSrv.exe will launch and exit automatically with any program making use of [CyeCom.ocx](../RRRCom).

Please note that the registering the program will also start it for the first time.  This is expected behavior.

    .\CyeSrv.exe /Automation /RegServer