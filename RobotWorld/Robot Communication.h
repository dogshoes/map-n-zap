#ifndef ROBOTCOMMUNICATION230495lsdfjg0934ldfjb039w4ldfj

#define ROBOTCOMMUNICATION230495lsdfjg0934ldfjb039w4ldfj

#include "Serial Protocol.h"
#include "RothMath.h"
#include "rrdim.h"

const unsigned char cPositionInput = 'P';					//Serial input command character for a position packet
const unsigned char cNudgeInput = 'N';						//Serial input command character for a nudge packet
const unsigned char cObstacleInput = 'O';					//Serial input command character for an obstacle packet
const unsigned char cVelocityInput = 'V';					//Serial input command character for a velocity packet
const unsigned char cAccelerationInput = 'A';			//Serial input command character for an acceleration packet
const unsigned char cEncodersInput = 'E';					//Serial input command character for an encoders packet
const unsigned char cAnalogInput = 'a';					//Serial input command character for an analog packet
const unsigned char cPwmInput = 'p';						//Serial input command character for a pwm packet
const unsigned char cRingBufferInput = 'R';				//Serial input command character for a ring buffer dump
const unsigned char cSendNewPathSegment = 's';			//Serial input command character for sending a new robot path segment
const unsigned char cInvalidInputString = 0;				//returned when the command character from a serial string was invalid
const unsigned char cLowBatteryMessage = 'b';		//Serial input command character for low battery message
const unsigned char cRobotError = 'e';

class CRobotComm : public CCOMProtocol {
public:
	void SendRequestState(void);
	void SendInformationToTransmit(long mask);
	void SendVelocityDirection(int Velocity, double Heading);
	void SendP5(long P) {return;};
	void SendP6(long P) {return;};
	void SendP7(long P) {return;};
	void SendP8(long P) {return;};
	void SendP4(long P4);
	void SendP3(long P3);
	void SendP2(long P2);
	void SendP1(long P1);
	void SendBuzzerFrequency(short BuzzerFrequency);
	void SendBuzzerOn(BOOL On);
	void SendPathSegment(BOOL PurgePath, robPOINT* Points, unsigned char nPoints);
	void SendFollowPath(short Speed);
	void SendHandleLength(short HandleLength);
	void SendMotorControlConstants(char Kp, char Ki, char Kd, char Kb, char KPwm);
	void SendTurn(double Heading, short Velocity, short Diameter);
	~CRobotComm();
	void SendObstacleSensitivity(TObstacleSensitivity Sensitive);
	void SendStraightVelocity(int Velocity, double Heading);
	void SendPositionVelocityDestination(robPOINT Destination, short Velocity);
	void SendMaxSpeed(short MaxSpeed);
	void SendHeading(double Heading);
	void SendPosition(robPOINT Position);
	void SendMotorVelocities(int M0Velocity, int M1Velocity);
	void SendHeadingCorrectionFactor(int Correction);
	void SendFlipHandle(BOOL value);
	void SendMaxPWM(unsigned char MaxPWM);
	void SendHeadingDestination(double Heading, short Velocity);
	void SendStopMotors(void);
	void SendPurgeRingBuffer(void);
	void SendMotorsPWM(int M0PWM, int M1PWM);
	void SendPositionDestination(robPOINT Destination);
	long int* DecodeSerialInput(CString* InputString, unsigned char* InputCode);
protected:

private:
	long WorldToRobotUnits(double WorldUnit);
	double RobotToWorldUnits(long RobotUnit);
	long WorldToRobotUnits(long WorldUnit);
	int RadianToEncoderHeading(double Heading);
	char* LongToBytes(long int X);
	int BytesToInt(unsigned char Byte0, unsigned char Byte1);
	long int BytesToLong(unsigned char Byte0, unsigned char Byte1,
		unsigned char Byte2, unsigned char Byte3);
};

#endif