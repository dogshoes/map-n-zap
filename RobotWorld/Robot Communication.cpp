/*******************************************************************************
//Robot Communication.cpp
//Created: 04/16/96 S.R.
********************************************************************************
//Description:
//		This file includes the member functions for the CRobotComm class.  This
//	object is used to handle the communications with the robot.  It deals with
//	decoding the serial input from the robot, and encoding the data to be sent
//	to the robot.
*******************************************************************************/


/*Function Header*/
/*******************************************************************************
//Created: 02/13/97 S.R.
//Last Revision: 02/13/97 S.R.
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
#include <math.h>
#include <assert.h>
#include <afxdisp.h>
#include "Robot Communication.h"

#define SET_HEADING_DEST_COMMAND_CHAR 'H'				//Command character to set the robot's heading destination
#define SET_TURN_COMMAND_CHAR 'T'						//Command character to turn the robot
#define SET_MOTORS_PWM_COMMAND_CHAR 'D'				//Command character to set the robot's PWM for both motors
#define SET_MAXPWM_COMMAND_CHAR 'M'						//Command character to set the robot's maximum PWM
#define SET_MAXSPEED_COMMAND_CHAR 's'					//Command character to set the roobt's maximum speed
#define SET_FLIPHANDLE_COMMAND_CHAR 'F'				//Command character to set the robot's flip handle parameter
#define SET_MOTORS_VELOCITY_COMMAND_CHAR 'V'			//Command character to set the robot's motor speeds
#define SET_HEADCORRECTFACT_COMMAND_CHAR 'c'			//Command character to set the robot's heading correction factor
#define SET_POSITIONVELOCITY_DESTINATION_COMMAND_CHAR 'v'
#define SET_BUNGEE_DEST_COMMAND_CHAR 'b'
#define STOP_MOTORS_COMMAND_CHAR 'S'					//Command character to stop the robot
#define REQUEST_ROBOT_POSITION_COMMAND_CHAR 'P'		//Command character to request the robot's current position
#define SET_HEADING_COMMAND_CHAR 'h'					//Command character to set the robot's heading
#define SET_POSITION_COMMAND_CHAR 'p'					//Command character to set the robot's position.
#define SET_GO_STRAIGHT_VELOCITY 'a'					//Command character to go straight at a certain velocity
#define SET_SENSITIVE_OBSTACLE_DETECTION 'o'			//Command character to set the sensitive obstacle detection
#define PURGE_RING_BUFFER_COMMAND_CHAR 'R'			//Command character to send the ring buffer
#define SET_MOTION_CONTROL_CONSTANTS_COMMAND_CHAR 'K'
#define SET_HANDLE_LENGTH_COMMAND_CHAR 'L'
#define SET_FOLLOW_PATH_COMMAND_CHAR 'P'
#define SET_PATH_SEGMENT_COMMAND_CHAR 'A'
#define SET_BUZZER_ON_COMMAND_CHAR 'B'
#define SET_BUZZER_FREQUENCY_COMMAND_CHAR 'f'
#define SET_P1_COMMAND_CHAR '1'
#define SET_P2_COMMAND_CHAR '2'
#define SET_P3_COMMAND_CHAR '3'
#define SET_P4_COMMAND_CHAR '4'
#define SET_VELOCITY_DIRECTION_COMMAND_CHAR 'd'
#define SET_INFORMATION_TO_TRANSMIT_COMMAND_CHAR 'i'
#define REQUEST_STATE_COMMAND_CHAR 'r'

void CRobotComm::SendPositionDestination(robPOINT Destination)
/*******************************************************************************
//Created: 04/16/96 S.R.
//Last Revision: 11/18/97 S.R.
//Parameters:
//	RRDest	The destination coordinate for the RugRover.
//Return Value:
//	none.
********************************************************************************
//Description:
//		This function sends the robot destination to the RugRover through the
//	serial link.
//Method:
//		The function uses the LongToBytes function to break the long int X and Y
//	coordinates into their individual bytes.  Then the string to output is formed
//	and sent out using the CCOMProtocol::SendSerialData function.  This is done
//	individually for both the X and Y coordinate.
*******************************************************************************/
{
    CString OutputString('\000', 9);
    OutputString.SetAt(0, SET_BUNGEE_DEST_COMMAND_CHAR);
    /*Output the X coordinate*/
    char* NumOutput = LongToBytes(WorldToRobotUnits(Destination.x));

    for (char i = 1; i <= 4; i++)
    {
        OutputString.SetAt(i, NumOutput[i - 1]);
    }

    /*Output the Y coordinate*/
    NumOutput = LongToBytes(WorldToRobotUnits(Destination.y));

    for (i = 5; i < 9; i++)
    {
        OutputString.SetAt(i, NumOutput[i - 5]);
    }

    CCOMProtocol::SendSerialData(&OutputString);
    return;
}

long int* CRobotComm::DecodeSerialInput(CString* InputString, unsigned char* InputCode)
/*******************************************************************************
//Created: 04/16/96 S.R.
//Last Revision: 11/18/97 S.R.
//Parameters:
//	InputString		The string received from the serial port that is to be decoded.
//	InputCode		A character signifying what command was received from the RugRover
//Return Value:
//	The long integer part of the command received.  Do not delete the returned pointer.
********************************************************************************
//Description:
//		This function decodes the serial input from the robot.  The function
//	returns a character signifying what command was recieved and a long integer
//	array that has the numbers associated with that command (if any.)
//Method:
//		The input string from the robot is made up of two parts.  The first
//	part is the command code stored as a character at the beginning of the input
//	string.  The second part is a set of optional numbers associated with that command.
//	The function strips the InputCode command from the beginning of the input
//	string.  Then the function forms the return value.  How this is formed depends
//	on the command code (the length of the number depends on the command character.)
*******************************************************************************/
{
    static long int ReturnValue[3];

    if ((InputString != NULL) && (InputString->GetLength() != 0))
    {
        switch (*InputCode = InputString->GetAt(0))
        {
        case cPositionInput:
            if (InputString->GetLength() >= 11)
            {
                ReturnValue[0] = RobotToWorldUnits(BytesToLong(InputString->GetAt(1), InputString->GetAt(2),
                                                   InputString->GetAt(3), InputString->GetAt(4)));
                ReturnValue[1] = RobotToWorldUnits(BytesToLong(InputString->GetAt(5), InputString->GetAt(6),
                                                   InputString->GetAt(7), InputString->GetAt(8)));
                ReturnValue[2] = BytesToInt(InputString->GetAt(9), InputString->GetAt(10));
            }

            break;

        /*One char input*/
        case cNudgeInput:
            ReturnValue[0] = InputString->GetAt(1);
            break;

        /*Two char input*/
        case cObstacleInput:
            ReturnValue[0] = InputString->GetAt(1);
            ReturnValue[1] = InputString->GetAt(2);
            break;

        case cEncodersInput:
            ReturnValue[0] = InputString->GetAt(1);
            ReturnValue[1] = InputString->GetAt(2);
            break;

        /*Two int input*/
        case cVelocityInput:
        case cAccelerationInput:
        case cPwmInput:
            ReturnValue[0] = BytesToInt(InputString->GetAt(1), InputString->GetAt(2));
            ReturnValue[1] = BytesToInt(InputString->GetAt(3), InputString->GetAt(4));
            break;

        /*Three int input*/
        case cAnalogInput:
            ReturnValue[0] = BytesToInt(InputString->GetAt(1), InputString->GetAt(2));
            ReturnValue[1] = BytesToInt(InputString->GetAt(3), InputString->GetAt(4));
            ReturnValue[2] = BytesToInt(InputString->GetAt(5), InputString->GetAt(6));
            break;

        /*One long input*/
        case cRingBufferInput:
        case cRobotError:
            ReturnValue[0] = BytesToLong(InputString->GetAt(1), InputString->GetAt(2), InputString->GetAt(3), InputString->GetAt(4));
            break;

        /*No data input*/
        case cSendNewPathSegment:
        case cLowBatteryMessage:
            break;

        default:
            *InputCode = cInvalidInputString;
            break;
        }
    }
    else
    {
        InputCode = cInvalidInputString;
    }

    return ReturnValue;
}

int CRobotComm::BytesToInt(unsigned char Byte0, unsigned char Byte1)
/*******************************************************************************
//Created: 04/16/96 S.R.
//Last Revision: 04/16/96 S.R.
//Parameters:
//	Byte0		The low byte of the number
//	Byte1		The high byte of the number
//Return Value:
//	The ingeger made up of the individual bytes in Byte0 and Byte 1.
********************************************************************************
//Description:
//		This function takes two individual bytes and turns them into an integer.
//Method:
//		The function uses the or and bit shift operations to glue the two bytes
//	together.
*******************************************************************************/
{
    return (((int)Byte0) | (((int)Byte1) << 8));
}

long int CRobotComm::BytesToLong(unsigned char Byte0, unsigned char Byte1,
                                 unsigned char Byte2, unsigned char Byte3)
/*******************************************************************************
//Created: 04/16/96 S.R.
//Last Revision: 04/16/96 S.R.
//Parameters:
//	Byte0..Byte3	The individual bytes to join together.  0 is the low order byte.
//Return Value:
//	The long integer made up of the individual bytes in Byte0..Byte3.
********************************************************************************
//Description:
//	This function takes four individual bytes and turns them into a long integer.
//Method:
//		The function uses the or and bit shift operations to glue the four bytes
//	together.
*******************************************************************************/
{
    long int Result = Byte0;
    Result |= ((long int)Byte1) << 8;
    Result |= ((long int)Byte2) << 16;
    Result |= ((long int)Byte3) << 24;
    return Result;
}


char* CRobotComm::LongToBytes(long int X)
/*******************************************************************************
//Created: 04/16/96 S.R.
//Last Revision: 04/16/96 S.R.
//Parameters:
//	X		The long integer to break up into individual bytes.
//Return Value:
//	an array of characters containing the individual bytes of X.
//	!!!!!!!!!!!Do not delte the resulting pointer!!!!!!!!!!!
********************************************************************************
//Description:
//	This function takes a long integer and breaks it up into its individual bytes.
//	!!!!!Do Not Delete the Pointer Returned!!!!!!
//Method:
//		This function uses the bit shift operations to strip out the individual
//	bytes of the long integer.  It then returns an array of characters containing
//	the individual characters.  This array must be deleted or memory leaks will
//	occur.
*******************************************************************************/
{
    static char Result[4];
    Result[0] = (char)X;
    Result[1] = (char)(X >> 8);
    Result[2] = (char)(X >> 16);
    Result[3] = (char)(X >> 24);
    return Result;
}

void CRobotComm::SendMotorsPWM(int M0PWM, int M1PWM)
/*******************************************************************************
//Created: 04/30/96 S.R.
//Last Revision: 04/30/96 S.R.
//Parameters:
//	M0PWM		The amount of pwm to send to motor 0.
//	M1PWM		The amount of pwm to send to motor 1.
//Return Value:
//	none.
********************************************************************************
//Description:
//		This function sends the Robot a command to change the pwm for the two
//	motors.
//Method:
//		The function forms a string with the command character to set the motors'
//	PWM at the front followed by motor0's pwm and motor1's pwm.  This string is
//	sent through the COM port by the CCOMProtocol::SendSerialData function.
*******************************************************************************/
{
    CString OutputString('\000', 5);
    /*Set the command character*/
    OutputString.SetAt(0, SET_MOTORS_PWM_COMMAND_CHAR);
    /*Set M0's pwm*/
    char* NumOutput = LongToBytes(M0PWM);
    OutputString.SetAt(1, NumOutput[0]);
    OutputString.SetAt(2, NumOutput[1]);
    /*Set M1's pwm*/
    NumOutput = LongToBytes(M1PWM);
    OutputString.SetAt(3, NumOutput[0]);
    OutputString.SetAt(4, NumOutput[1]);
    /*Send the data*/
    CCOMProtocol::SendSerialData(&OutputString);
}

void CRobotComm::SendStopMotors()
/*******************************************************************************
//Created: 04/30/96 S.R.
//Last Revision: 04/30/96 S.R.
//Parameters:
//	none.
//Return Value:
//	none.
********************************************************************************
//Description:
//	This function sends a command to the RugRover to stop the motors.
//Method:
//		The function sends a string with the command character to stop the motors.
// This string is sent through the COM port by the CCOMProtocol::SendSerialData
//	function.
*******************************************************************************/
{
    CString OutputString = STOP_MOTORS_COMMAND_CHAR;
    CCOMProtocol::SendSerialData(&OutputString);
}

void CRobotComm::SendPurgeRingBuffer()
/*******************************************************************************
//Created: 04/21/97 S.R.
//Last Revision: 04/21/97 S.R.
//Parameters:
//	none.
//Return Value:
//	none.
********************************************************************************
//Description:
//
//Method:
//
*******************************************************************************/
{
    CString OutputString = PURGE_RING_BUFFER_COMMAND_CHAR;
    CCOMProtocol::SendSerialData(&OutputString);
}

void CRobotComm::SendHeadingDestination(double Heading, short Velocity)
/*******************************************************************************
//Created: 04/30/96 S.R.
//Last Revision: 01/30/97 S.R.
//Parameters:
//	Heading:		The heading to send to the robot.
//Return Value:
//	none.
********************************************************************************
//Description:
//		This function sends a heading for the robot to turn to.
//Method:
//		The function creates a string with the SET_HEADING_DEST_COMMAND_CHAR and Heading.
// The string is sent through the COM port by the CCOMProtocol::SendSerialData
//	function.
*******************************************************************************/
{
    CString OutputString('\000', 5);
    /*Set the command character*/
    OutputString.SetAt(0, SET_HEADING_DEST_COMMAND_CHAR);
    /*Set the heading*/
    char* NumOutput = LongToBytes(RadianToEncoderHeading(Heading));
    OutputString.SetAt(1, NumOutput[0]);
    OutputString.SetAt(2, NumOutput[1]);
    NumOutput = LongToBytes(Velocity);
    OutputString.SetAt(3, NumOutput[0]);
    OutputString.SetAt(4, NumOutput[1]);
    /*Send the data*/
    CCOMProtocol::SendSerialData(&OutputString);
}

void CRobotComm::SendMaxPWM(unsigned char MaxPWM)
/*******************************************************************************
//Created: 04/30/96 S.R.
//Last Revision: 04/30/96 S.R.
//Parameters:
//	MaxPWM:	The max pwm the robot can use.
//Return Value:
//	none.
********************************************************************************
//Description:
//	This function sends the maximum pwm the robot can use.
//Method:
//	The function creates a string with the SET_MAXPWM_COMMAND_CHAR and MaxPWM.
// The string is sent through the COM port by the CCOMProtocol::SendSerialData
//	function.
*******************************************************************************/
{
    CString OutputString('\000', 2);
    /*Set the command character*/
    OutputString.SetAt(0, SET_MAXPWM_COMMAND_CHAR);
    /*Set M0's pwm*/
    OutputString.SetAt(1, MaxPWM);
    /*Send the data*/
    CCOMProtocol::SendSerialData(&OutputString);
}

void CRobotComm::SendFlipHandle(BOOL value)
/*******************************************************************************
//Created: 08/29/96 S.R.
//Last Revision: 08/29/96 S.R.
//Parameters:
//	value: the value to set FlipHandle to.
//Return Value:
//	none.
********************************************************************************
//Description:
//	This function sends the flip handle value to the robot.
//Method:
//	The function creates a string with the SET_FLIPHANDLE_COMMAND_CHAR and value.
// The string is sent through the COM port by the CCOMProtocol::SendSerialData
//	function.
*******************************************************************************/
{
    CString OutputString('\000', 2);
    /*Set the command character*/
    OutputString.SetAt(0, SET_FLIPHANDLE_COMMAND_CHAR);
    /*Set the value*/
    OutputString.SetAt(1, value);
    /*Send the data*/
    CCOMProtocol::SendSerialData(&OutputString);
}

void CRobotComm::SendHeadingCorrectionFactor(int Correction)
/*******************************************************************************
//Created: 11/12/96 S.R.
//Last Revision: 11/15/96 S.R.
//Parameters:
//	Correction:	The heading correction factor.
//Return Value:
//	none.
********************************************************************************
//Description:
//	The function sends the heading correction factor to the robot.
//Method:
//	The function creates a string with the SET_HEADCORRECTFACT_COMMAND_CHAR and Correction.
// The string is sent through the COM port by the CCOMProtocol::SendSerialData
//	function.
*******************************************************************************/
{
    CString OutputString('\000', 3);
    /*Set the command character*/
    OutputString.SetAt(0, SET_HEADCORRECTFACT_COMMAND_CHAR);
    /*Set the correction bytes*/
    char* NumOutput = LongToBytes(Correction);
    OutputString.SetAt(1, NumOutput[0]);
    OutputString.SetAt(2, NumOutput[1]);
    /*Send the data*/
    CCOMProtocol::SendSerialData(&OutputString);
}

void CRobotComm::SendMotorVelocities(int M0Velocity, int M1Velocity)
/*******************************************************************************
//Created: 11/21/96 S.R.
//Last Revision: 04/01/97 S.R.
//Parameters:
//	M0Speed, M1Speed:	The velocities the motors should go.
//Return Value:
//	none.
********************************************************************************
//Description:
//	This function sets the velocities the robot's motors should go.
//Method:
//		The function creates a string with the SET_MOTORS_VELOCITY_COMMAND_CHAR and
//	M0Velocity and M1Velocity.
//	The string is sent through the COM port by the CCOMProtocol::SendSerialData
//	function.
*******************************************************************************/
{
    CString OutputString('\000', 5);
    /*Set the command character*/
    OutputString.SetAt(0, SET_MOTORS_VELOCITY_COMMAND_CHAR);
    /*Set M0's pwm*/
    char* NumOutput = LongToBytes(M0Velocity);
    OutputString.SetAt(1, NumOutput[0]);
    OutputString.SetAt(2, NumOutput[1]);
    NumOutput = LongToBytes(M1Velocity);
    OutputString.SetAt(3, NumOutput[0]);
    OutputString.SetAt(4, NumOutput[1]);
    /*Send the data*/
    CCOMProtocol::SendSerialData(&OutputString);
}

void CRobotComm::SendPosition(robPOINT Position)
/*******************************************************************************
//Created: 12/19/96 S.R.
//Last Revision: 11/18/97 S.R.
//Parameters:
//	Position:	The robot's correct position.
//Return Value:
//	none.
********************************************************************************
//Description:
//	This function sends the robot's correct position.
//Method:
//		The function creates a string with the SET_POSITION_COMMAND_CHAR and
//	Position.  Position is transformed from world to robot coordinates.
//	The string is sent through the COM port by the CCOMProtocol::SendSerialData
//	function.
*******************************************************************************/
{
    CString OutputString('\000', 9);
    OutputString.SetAt(0, SET_POSITION_COMMAND_CHAR);
    /*Output the X coordinate*/
    char* NumOutput = LongToBytes(WorldToRobotUnits(Position.x));

    for (char i = 1; i <= 4; i++)
    {
        OutputString.SetAt(i, NumOutput[i - 1]);
    }

    /*Output the Y coordinate*/
    NumOutput = LongToBytes(WorldToRobotUnits(Position.y));

    for (i = 5; i < 9; i++)
    {
        OutputString.SetAt(i, NumOutput[i - 5]);
    }

    CCOMProtocol::SendSerialData(&OutputString);
}

void CRobotComm::SendHeading(double Heading)
/*******************************************************************************
//Created: 12/19/96 S.R.
//Last Revision: 01/30/97 S.R.
//Parameters:
//	Heading:	The robot's correct heading.
//Return Value:
//	none.
********************************************************************************
//Description:
//	This function sends the robot's correct heading.
//Method:
//		The function creates a string with the SET_HEADING_COMMAND_CHAR and
//	Heading.
//	The string is sent through the COM port by the CCOMProtocol::SendSerialData
//	function.
*******************************************************************************/
{
    CString OutputString('\000', 3);
    /*Set the command character*/
    OutputString.SetAt(0, SET_HEADING_COMMAND_CHAR);
    /*Set the heading*/
    short RobotHeading = RadianToEncoderHeading(Heading);

    if ((RobotHeading < 0) || (RobotHeading > cNHeadings))
    {
        fprintf(stderr, "ERROR!!! SendHeading: Radian Heading = %10lf Robot Heading = %d\n", Heading, RobotHeading);
        ASSERT(FALSE);
    }

    char* NumOutput = LongToBytes(RobotHeading);
    OutputString.SetAt(1, NumOutput[0]);
    OutputString.SetAt(2, NumOutput[1]);
    /*Send the data*/
    CCOMProtocol::SendSerialData(&OutputString);
}

void CRobotComm::SendMaxSpeed(short MaxSpeed)
/*******************************************************************************
//Created: 01/23/97 S.R.
//Last Revision: 04/01/97 S.R.
//Parameters:
//	MaxPWM:	The max speed the robot can use.
//Return Value:
//	none.
********************************************************************************
//Description:
//	This function sends the maximum speed the robot can use.
//Method:
//	The function creates a string with the SET_MAXSPEED_COMMAND_CHAR and MaxSpeed.
// The string is sent through the COM port by the CCOMProtocol::SendSerialData
//	function.
*******************************************************************************/
{
    CString OutputString('\000', 3);
    /*Set the command character*/
    OutputString.SetAt(0, SET_MAXSPEED_COMMAND_CHAR);
    char*NumOutput = LongToBytes(MaxSpeed);
    /*Set M0's speed*/
    OutputString.SetAt(1, NumOutput[0]);
    OutputString.SetAt(2, NumOutput[1]);
    /*Send the data*/
    CCOMProtocol::SendSerialData(&OutputString);
}

void CRobotComm::SendPositionVelocityDestination(robPOINT Destination, short Velocity)
/*******************************************************************************
//Created: 01/23/97 S.R.
//Last Revision: 11/18/97 S.R.
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
{
    CString OutputString('\000', 11);
    OutputString.SetAt(0, SET_POSITIONVELOCITY_DESTINATION_COMMAND_CHAR);
    /*Output the X coordinate*/
    char* NumOutput = LongToBytes(WorldToRobotUnits(Destination.x));

    for (char i = 1; i <= 4; i++)
    {
        OutputString.SetAt(i, NumOutput[i - 1]);
    }

    /*Output the Y coordinate*/
    NumOutput = LongToBytes(WorldToRobotUnits(Destination.y));

    for (i = 5; i < 9; i++)
    {
        OutputString.SetAt(i, NumOutput[i - 5]);
    }

    NumOutput = LongToBytes(Velocity);
    OutputString.SetAt(9, NumOutput[0]);
    OutputString.SetAt(10, NumOutput[1]);
    CCOMProtocol::SendSerialData(&OutputString);
}

void CRobotComm::SendStraightVelocity(int Velocity, double Heading)
/*******************************************************************************
//Created: 01/30/97 S.R.
//Last Revision: 04/10/97 S.R.
//Parameters:
//
//Return Value:
//
********************************************************************************
//Description:
//	!!Warning!! This command can change the value of FlipHandle and MaxSpeed in the robot!!
//Method:
//
*******************************************************************************/
{
    CString OutputString('\000', 5);
    OutputString.SetAt(0, SET_GO_STRAIGHT_VELOCITY);
    char* NumOutput = LongToBytes(Velocity);
    OutputString.SetAt(1, NumOutput[0]);
    OutputString.SetAt(2, NumOutput[1]);
    NumOutput = LongToBytes(RadianToEncoderHeading(Heading));
    OutputString.SetAt(3, NumOutput[0]);
    OutputString.SetAt(4, NumOutput[1]);
    CCOMProtocol::SendSerialData(&OutputString);
}

int CRobotComm::RadianToEncoderHeading(double Heading)
/*******************************************************************************
//Created: 01/30/97 S.R.
//Last Revision: 01/30/97 S.R.
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
{
    Heading = BringAngleInRange(Heading);
    /*Transform radian heading into a robot heading*/
    Heading = Heading * cNHeadings / (2 * pi);

    if (Heading >= cNHeadings)
    {
        Heading -= cNHeadings;
    }

    if (Heading >= cNHeadings)
    {
        AfxMessageBox("RadianToEncoderHeading:  Heading Out Of Range", MB_OK, 0);
    }

    return Round(Heading);
}

void CRobotComm::SendObstacleSensitivity(TObstacleSensitivity Sensitive)
/*******************************************************************************
//Created: 02/06/97 S.R.
//Last Revision: 02/06/97 S.R.
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
{
    CString OutputString('\000', 2);
    /*Set the command character*/
    OutputString.SetAt(0, SET_SENSITIVE_OBSTACLE_DETECTION);
    /*Set the value*/
    OutputString.SetAt(1, Sensitive);
    /*Send the data*/
    CCOMProtocol::SendSerialData(&OutputString);
}

CRobotComm::~CRobotComm()
/*******************************************************************************
//Created: 02/13/97 S.R.
//Last Revision: 02/13/97 S.R.
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
{
}

void CRobotComm::SendTurn(double Heading, short Velocity, short Diameter)
/*******************************************************************************
//Created: 09/17/97 S.R.
//Last Revision: 09/17/97 S.R.
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
{
    CString OutputString('\000', 7);
    /*Set the command character*/
    OutputString.SetAt(0, SET_TURN_COMMAND_CHAR);
    /*Set the heading*/
    char* NumOutput = LongToBytes(RadianToEncoderHeading(Heading));
    OutputString.SetAt(1, NumOutput[0]);
    OutputString.SetAt(2, NumOutput[1]);
    /*Set the velocity*/
    NumOutput = LongToBytes(Velocity);
    OutputString.SetAt(3, NumOutput[0]);
    OutputString.SetAt(4, NumOutput[1]);
    /*Set the diameter*/
    NumOutput = LongToBytes(WorldToRobotUnits((long)Diameter));
    OutputString.SetAt(5, NumOutput[0]);
    OutputString.SetAt(6, NumOutput[1]);
    /*Send the data*/
    CCOMProtocol::SendSerialData(&OutputString);
}

void CRobotComm::SendMotorControlConstants(char Kp, char Ki, char Kd, char Kb, char KPwm)
/*******************************************************************************
//Created: 10/06/97 S.R.
//Last Revision: 10/06/97 S.R.
//Parameters:
//
//Return Value:
//	none.
********************************************************************************
//Description:
//
//Method:
//
*******************************************************************************/
{
    CString OutputString('\000', 6);
    /*Set the command character*/
    OutputString.SetAt(0, SET_MOTION_CONTROL_CONSTANTS_COMMAND_CHAR);
    /*Set the string values*/
    OutputString.SetAt(1, Kp);
    OutputString.SetAt(2, Ki);
    OutputString.SetAt(3, Kd);
    OutputString.SetAt(4, Kb);
    OutputString.SetAt(5, KPwm);
    /*Send the data*/
    CCOMProtocol::SendSerialData(&OutputString);

}

void CRobotComm::SendHandleLength(short HandleLength)
/*******************************************************************************
//Created: 10/07/97 S.R.
//Last Revision: 11/18/97 S.R.
//Parameters:
//	HandleLength:	the new robot's handle length in world units.
//Return Value:
//	none.
********************************************************************************
//Description:
//	This function transforms HandleLength into robot units then sends it to the
//	robot.
//Method:
//
*******************************************************************************/
{
    CString OutputString('\000', 3);
    /*Set the command character*/
    OutputString.SetAt(0, SET_HANDLE_LENGTH_COMMAND_CHAR);
    /*Set the handle length*/
    HandleLength = WorldToRobotUnits((long)HandleLength);
    char* NumOutput = LongToBytes(HandleLength);
    OutputString.SetAt(1, NumOutput[0]);
    OutputString.SetAt(2, NumOutput[1]);
    /*Send the data*/
    CCOMProtocol::SendSerialData(&OutputString);
}

void CRobotComm::SendFollowPath(short Speed)
/*******************************************************************************
//Created: 10/07/97 S.R.
//Last Revision: 10/07/97 S.R.
//Parameters:
//	FollowPath:	Follow path or not?
//Return Value:
//	none.
********************************************************************************
//Description:
//		This function sends a command to the robot to start or stop following its
//	stored connect-the-dots path.
//Method:
//
*******************************************************************************/
{
    CString OutputString('\000', 3);
    /*Set the command character*/
    OutputString.SetAt(0, SET_FOLLOW_PATH_COMMAND_CHAR);
    /*Set Speed*/
    char* NumOutput = LongToBytes(Speed);
    OutputString.SetAt(1, NumOutput[0]);
    OutputString.SetAt(2, NumOutput[1]);
    /*Send the data*/
    CCOMProtocol::SendSerialData(&OutputString);
}

void CRobotComm::SendPathSegment(BOOL PurgePath, robPOINT* Points, unsigned char nPoints)
/*******************************************************************************
//Created: 10/14/97 S.R.
//Last Revision: 11/18/97 S.R.
//Parameters:
//	PurgePath:	TRUE if the robot should purge its previous path.
//	Points:	points to add to the robot's connect-the-dots path.
//	nPoints:	number of points in Points array.
//Return Value:
//	none.
********************************************************************************
//Description:
//		This function sends a connect-the-dots path segment to the robot.
//Method:
//		The function transforms the points from world to robot coordinates.  It
//	sends the path segment down as a segment origin point then the distance between
//	successive points.  This saves download time.
*******************************************************************************/
{
    CString OutputString('\000', 8 + 2 * (nPoints - 1) + 3);
    OutputString.SetAt(0, SET_PATH_SEGMENT_COMMAND_CHAR);
    /*Set Purge Path*/
    OutputString.SetAt(1, PurgePath);
    /*Set the number of points*/
    OutputString.SetAt(2, nPoints);

    if (nPoints != 0)
    {
        /*Output the X0 coordinate*/
        char* NumOutput = LongToBytes(WorldToRobotUnits(Points[0].x));

        for (unsigned char i = 0; i < 4; i++)
        {
            OutputString.SetAt(i + 3, NumOutput[i]);
        }

        /*Output the Y0 coordinate*/
        NumOutput = LongToBytes(WorldToRobotUnits(Points[0].y));

        for (i = 0; i < 4; i++)
        {
            OutputString.SetAt(i + 7, NumOutput[i]);
        }

        /*Output the rest of the coordinates*/
        for (i = 0; i < nPoints - 1; i++)
        {
            OutputString.SetAt(2 * i + 11, WorldToRobotUnits(Points[i + 1].x - Points[i].x));
            OutputString.SetAt(2 * i + 1 + 11, WorldToRobotUnits(Points[i + 1].y - Points[i].y));
        }
    }

    CCOMProtocol::SendSerialData(&OutputString);
}

void CRobotComm::SendBuzzerOn(BOOL On)
{
    CString OutputString('\000', 2);
    /*Set the command character*/
    OutputString.SetAt(0, SET_BUZZER_ON_COMMAND_CHAR);
    /*Set M0's pwm*/
    OutputString.SetAt(1, On);
    /*Send the data*/
    CCOMProtocol::SendSerialData(&OutputString);
}

void CRobotComm::SendBuzzerFrequency(short BuzzerFrequency)
{
    typedef enum {bf0, bf2000, bf4000, bf6000, bf8000} TBuzzerFrequency;
    CString OutputString('\000', 3);
    /*Set the command character*/
    OutputString.SetAt(0, SET_BUZZER_FREQUENCY_COMMAND_CHAR);
    /*Set Buzzer Frequency*/
    char* NumOutput = LongToBytes(BuzzerFrequency);
    OutputString.SetAt(1, NumOutput[0]);
    OutputString.SetAt(2, NumOutput[1]);
    /*Send the data*/
    CCOMProtocol::SendSerialData(&OutputString);
}

long CRobotComm::WorldToRobotUnits(long WorldUnit)
{
    return (WorldUnit * cRobotEncoderTicksPerInch / cTPI);
}

double CRobotComm::RobotToWorldUnits(long RobotUnit)
{
    return (RobotUnit * cTPI / cRobotEncoderTicksPerInch);
}

long CRobotComm::WorldToRobotUnits(double WorldUnit)
{
    return Round(WorldUnit * cRobotEncoderTicksPerInch / cTPI);
}

void CRobotComm::SendP1(long P1)
{
    CString OutputString('\000', 5);
    OutputString.SetAt(0, SET_P1_COMMAND_CHAR);
    /*Output P1*/
    char* NumOutput = LongToBytes(P1);

    for (char i = 1; i <= 4; i++)
    {
        OutputString.SetAt(i, NumOutput[i - 1]);
    }

    CCOMProtocol::SendSerialData(&OutputString);
}

void CRobotComm::SendP2(long P2)
{
    CString OutputString('\000', 5);
    OutputString.SetAt(0, SET_P2_COMMAND_CHAR);
    /*Output P2*/
    char* NumOutput = LongToBytes(P2);

    for (char i = 1; i <= 4; i++)
    {
        OutputString.SetAt(i, NumOutput[i - 1]);
    }

    CCOMProtocol::SendSerialData(&OutputString);
}

void CRobotComm::SendP3(long P3)
{
    CString OutputString('\000', 5);
    OutputString.SetAt(0, SET_P3_COMMAND_CHAR);
    /*Output P3*/
    char* NumOutput = LongToBytes(P3);

    for (char i = 1; i <= 4; i++)
    {
        OutputString.SetAt(i, NumOutput[i - 1]);
    }

    CCOMProtocol::SendSerialData(&OutputString);
}

void CRobotComm::SendP4(long P4)
{
    CString OutputString('\000', 5);
    OutputString.SetAt(0, SET_P4_COMMAND_CHAR);
    /*Output P4*/
    char* NumOutput = LongToBytes(P4);

    for (char i = 1; i <= 4; i++)
    {
        OutputString.SetAt(i, NumOutput[i - 1]);
    }

    CCOMProtocol::SendSerialData(&OutputString);
}

void CRobotComm::SendVelocityDirection(int Velocity, double Heading)
{
    CString OutputString('\000', 5);
    OutputString.SetAt(0, SET_VELOCITY_DIRECTION_COMMAND_CHAR);
    char* NumOutput = LongToBytes(Velocity);
    OutputString.SetAt(1, NumOutput[0]);
    OutputString.SetAt(2, NumOutput[1]);
    NumOutput = LongToBytes(RadianToEncoderHeading(Heading));
    OutputString.SetAt(3, NumOutput[0]);
    OutputString.SetAt(4, NumOutput[1]);
    CCOMProtocol::SendSerialData(&OutputString);
}

void CRobotComm::SendInformationToTransmit(long mask)
{
    CString OutputString('\000', 5);
    OutputString.SetAt(0, SET_INFORMATION_TO_TRANSMIT_COMMAND_CHAR);
    /*Output mask*/
    char* NumOutput = LongToBytes(mask);

    for (char i = 1; i <= 4; i++)
    {
        OutputString.SetAt(i, NumOutput[i - 1]);
    }

    CCOMProtocol::SendSerialData(&OutputString);
}

void CRobotComm::SendRequestState()
{
    CString OutputString = REQUEST_STATE_COMMAND_CHAR;
    CCOMProtocol::SendSerialData(&OutputString);
}
