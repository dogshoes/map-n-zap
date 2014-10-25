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

// DlgTestRobot.cpp : implementation file
//

#include "stdafx.h"
#include "robotworld.h"
#include "DlgTestRobot.h"
#include "Robot.h"
#include <fstream>

/////////////////////////////////////////////////////////////////////////////
// CDlgTestRobot dialog


CDlgTestRobot::CDlgTestRobot(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgTestRobot::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgTestRobot)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}


void CDlgTestRobot::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgTestRobot)
    // NOTE: the ClassWizard will add DDX and DDV calls here
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgTestRobot, CDialog)
    //{{AFX_MSG_MAP(CDlgTestRobot)
    ON_BN_CLICKED(IDC_BUT_FREE_WHEEL_TEST, OnButFreeWheelTest)
    ON_BN_CLICKED(IDC_BUT_M1BACKWARD, OnButM1Backward)
    ON_BN_CLICKED(IDC_BUT_M1FORWARD, OnButM1Forward)
    ON_BN_CLICKED(IDC_BUT_STOP_MOTORS, OnButStopMotors)
    ON_BN_CLICKED(IDC_BUT_M0FORWARD, OnButM0Forward)
    ON_BN_CLICKED(IDC_BUT_M0BACKWARD, OnButM0Backward)
    ON_BN_CLICKED(IDC_BUT_AGAINST_WALL_TEST, OnButAgainstWallTest)
    ON_BN_CLICKED(IDC_BUT_ROTATE_TEST, OnButRotateTest)
    ON_BN_CLICKED(IDC_TEST_MICROPHONE, OnTestMicrophone)
    ON_BN_CLICKED(IDC_TEST_INPUT, OnTestInput)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgTestRobot message handlers

void CDlgTestRobot::OnButFreeWheelTest()
{
    FreeWheelTest();
}

bool CDlgTestRobot::FreeWheelTest()
{
    std::ofstream output("Testing.txt");
    output << "Hello There Ho'wre your doing?";

    FILE* out = fopen("FreeWheelTest.txt", "w");
    const short cTestPwm = 555;
    bool TestSuccessful = true;
    CTime WaitUntilTime;
    const int nSamples = 5;
    double TestM0V[nSamples], TestM1V[nSamples];

    m_Robot->EStop();
    m_Robot->SetObstacleSensitivity(osNone);
    m_Robot->m_MotorObstacle.ValidObstacle = false;
    m_Robot->SetState(stExecuting);
    ::AfxMessageBox("Place Robot so it can rotate without interference");
    //	m_Robot->SetInformationToTransmit(true, false, true, false);
    bool M0Test1Results[nFreeWheelTestResults], M1Test1Results[nFreeWheelTestResults];
    bool M0Test2Results[nFreeWheelTestResults], M1Test2Results[nFreeWheelTestResults];
    /*Go one direction*/
    m_Robot->SetPwm(cTestPwm, -cTestPwm);

    /*Wait for motors to get up to speed*/
    WaitUntilTime = CTime::GetCurrentTime();
    WaitUntilTime += CTimeSpan(0, 0, 0, 2);
    bool done = false;

    while (!done)
    {
        m_Robot->DoWindowMessages();
        CTimeSpan TimeDiff = WaitUntilTime - CTime::GetCurrentTime();

        if ((TimeDiff.GetHours() == 0) && (TimeDiff.GetMinutes() == 0) &&
                (TimeDiff.GetSeconds() <= 0))
        {
            done = true;
        }
    }

    for (int i = 0; i < nSamples; i++)
    {
        /*Wait for a while*/
        WaitUntilTime = CTime::GetCurrentTime();
        WaitUntilTime += CTimeSpan(0, 0, 0, 2);
        done = false;

        while (!done)
        {
            m_Robot->DoWindowMessages();
            CTimeSpan TimeDiff = WaitUntilTime - CTime::GetCurrentTime();

            if ((TimeDiff.GetHours() == 0) && (TimeDiff.GetMinutes() == 0) &&
                    (TimeDiff.GetSeconds() <= 0))
            {
                done = true;
            }
        }

        TestM0V[i] = m_Robot->m_M0VelocityMeasured;
        TestM1V[i] = m_Robot->m_M1VelocityMeasured;
    }

    fprintf(out, "FreeWheelTest1 Results********:\n");
    fprintf(out, "M0 Test Results\n");
    TestSuccessful &= FreeWheelCheck(true, TestM0V, nSamples, M0Test1Results, out);

    if (m_Robot->GetState() != stExecuting)
    {
        TestSuccessful = false;
        goto END;
    }

    /*Analyze Motor 0*/
    if (!M0Test1Results[nFreeWheelTestZeroV])
    {
        m_Robot->SetPwm(cTestPwm, 0);

        if (AfxMessageBox("Is the robot's right wheel moving?", MB_YESNO) == IDYES)
        {
            AfxMessageBox("FreeWheelTest Error 1: The robot's right interrupt encoder is malfunctioning");
            fprintf(out, "****FWTERROR 1****The robot's right interrupt encoder is malfunctioning\n");
        }
        else
        {
            AfxMessageBox("FreeWheelTest Error 2: The robot's right driver is malfunctioning");
            fprintf(out, "****FWTERROR 2****The robot's right driver is malfunctioning\n");
        }

        m_Robot->SetPwm(0, 0);
    }

    if (!M0Test1Results[nFreeWheelTestDir])
    {
        if (!CheckBackAndForth(0))
        {
            AfxMessageBox("FreeWheelTest Error 3: The robot's right pwm driver is malfunctioning");
            fprintf(out, "****FWTERROR 3****The robot's right pwm driver is malfunctioning\n");
        }
        else
        {
            AfxMessageBox("FreeWheelTest Error 4: The robot's right direction  encoder is malfunctioning");
            fprintf(out, "****FWTERROR 4****The robot's right direction encoder is malfunctioning\n");
        }
    }

    fprintf(out, "M1 Test Results\n");
    TestSuccessful &= FreeWheelCheck(false, TestM1V, nSamples, M1Test1Results, out);

    if (m_Robot->GetState() != stExecuting)
    {
        TestSuccessful = false;
        goto END;
    }

    /*Analyze Motor 1*/
    if (!M1Test1Results[nFreeWheelTestZeroV])
    {
        m_Robot->SetPwm(0, -cTestPwm);

        if (AfxMessageBox("Is the robot's left wheel moving?", MB_YESNO) == IDYES)
        {
            AfxMessageBox("FreeWheelTest Error 1: The robot's left interrupt encoder is malfunctioning");
            fprintf(out, "****FWTERROR 1****The robot's left interrupt encoder is malfunctioning\n");
        }
        else
        {
            AfxMessageBox("FreeWheelTest Error 2: The robot's left driver is malfunctioning");
            fprintf(out, "****FWTERROR 2****The robot's left driver is malfunctioning\n");
        }

        m_Robot->SetPwm(0, 0);
    }

    if (!M1Test1Results[nFreeWheelTestDir])
    {
        if (!CheckBackAndForth(1))
        {
            AfxMessageBox("FreeWheelTest Error 3: The robot's left pwm driver is malfunctioning");
            fprintf(out, "****FWTERROR 3****The robot's left pwm driver is malfunctioning\n");
        }
        else
        {
            AfxMessageBox("FreeWheelTest Error 4: The robot's left direction  encoder is malfunctioning");
            fprintf(out, "****FWTERROR 4****The robot's left direction encoder is malfunctioning\n");
        }
    }

    /*Check for obstacle*/
    if (m_Robot->m_MotorObstacle.ValidObstacle)
    {
        AfxMessageBox("FreeWheelTest Error 5: Analog Channel Error");
        fprintf(out, "****FWTERROR 5**** Analog Channel Error\n");
        TestSuccessful = false;
    }

    if (m_Robot->GetState() != stExecuting)
    {
        TestSuccessful = false;
        goto END;
    }

    /*Wait for things to stop*/
    m_Robot->SetPwm(0, 0);
    /*Wait for a while*/
    WaitUntilTime = CTime::GetCurrentTime();
    WaitUntilTime += CTimeSpan(0, 0, 0, 2);
    done = false;

    while (!done)
    {
        m_Robot->DoWindowMessages();
        CTimeSpan TimeDiff = WaitUntilTime - CTime::GetCurrentTime();

        if ((TimeDiff.GetHours() == 0) && (TimeDiff.GetMinutes() == 0) &&
                (TimeDiff.GetSeconds() <= 0))
        {
            done = true;
        }
    }

    if (m_Robot->GetState() != stExecuting)
    {
        TestSuccessful = false;
        goto END;
    }

    /*Go the other*/
    m_Robot->SetPwm(-cTestPwm, cTestPwm);
    /*Wait for motors to get up to speed*/
    WaitUntilTime = CTime::GetCurrentTime();
    WaitUntilTime += CTimeSpan(0, 0, 0, 2);
    done = false;

    while (!done)
    {
        m_Robot->DoWindowMessages();
        CTimeSpan TimeDiff = WaitUntilTime - CTime::GetCurrentTime();

        if ((TimeDiff.GetHours() == 0) && (TimeDiff.GetMinutes() == 0) &&
                (TimeDiff.GetSeconds() <= 0))
        {
            done = true;
        }
    }

    for (int i = 0; i < nSamples; i++)
    {
        /*Wait for a while*/
        WaitUntilTime = CTime::GetCurrentTime();
        WaitUntilTime += CTimeSpan(0, 0, 0, 2);
        bool done = false;

        while (!done)
        {
            m_Robot->DoWindowMessages();
            CTimeSpan TimeDiff = WaitUntilTime - CTime::GetCurrentTime();

            if ((TimeDiff.GetHours() == 0) && (TimeDiff.GetMinutes() == 0) &&
                    (TimeDiff.GetSeconds() <= 0))
            {
                done = true;
            }
        }

        TestM0V[i] = m_Robot->m_M0VelocityMeasured;
        TestM1V[i] = m_Robot->m_M1VelocityMeasured;
    }

    fprintf(out, "FreeWheelTest2 Results********:\n");
    fprintf(out, "M0 Test Results\n");
    TestSuccessful &= FreeWheelCheck(false, TestM0V, nSamples, M0Test2Results, out);

    if (m_Robot->GetState() != stExecuting)
    {
        TestSuccessful = false;
        goto END;
    }

    /*Analyze Motor 0*/
    if (!M0Test2Results[nFreeWheelTestZeroV])
    {
        m_Robot->SetPwm(-cTestPwm, 0);

        if (AfxMessageBox("Is the robot's right wheel moving?", MB_YESNO) == IDYES)
        {
            AfxMessageBox("FreeWheelTest Error 1: The robot's right interrupt encoder is malfunctioning");
            fprintf(out, "****FWTERROR 1****The robot's right interrupt encoder is malfunctioning\n");
        }
        else
        {
            AfxMessageBox("FreeWheelTest Error 2: The robot's right driver is malfunctioning");
            fprintf(out, "****FWTERROR 2****The robot's right driver is malfunctioning\n");
        }

        m_Robot->SetPwm(0, 0);
    }

    if (!M0Test2Results[nFreeWheelTestDir])
    {
        if (!CheckBackAndForth(0))
        {
            AfxMessageBox("FreeWheelTest Error 3: The robot's right pwm driver is malfunctioning");
            fprintf(out, "****FWTERROR 3****The robot's right pwm driver is malfunctioning\n");
        }
        else
        {
            AfxMessageBox("FreeWheelTest Error 4: The robot's right direction  encoder is malfunctioning");
            fprintf(out, "****FWTERROR 4****The robot's right direction encoder is malfunctioning\n");
        }
    }

    fprintf(out, "M1 Test Results\n");
    TestSuccessful &= FreeWheelCheck(true, TestM1V, nSamples, M1Test2Results, out);

    if (m_Robot->GetState() != stExecuting)
    {
        TestSuccessful = false;
        goto END;
    }

    /*Analyze Motor 1*/
    if (!M1Test2Results[nFreeWheelTestZeroV])
    {
        m_Robot->SetPwm(0, cTestPwm);

        if (AfxMessageBox("Is the robot's left wheel moving?", MB_YESNO) == IDYES)
        {
            AfxMessageBox("FreeWheelTest Error 1: The robot's left interrupt encoder is malfunctioning");
            fprintf(out, "****FWTERROR 1****The robot's left interrupt encoder is malfunctioning\n");
        }
        else
        {
            AfxMessageBox("FreeWheelTest Error 2: The robot's left driver is malfunctioning");
            fprintf(out, "****FWTERROR 2****The robot's left driver is malfunctioning\n");
        }

        m_Robot->SetPwm(0, 0);
    }

    if (!M1Test2Results[nFreeWheelTestDir])
    {
        if (!CheckBackAndForth(1))
        {
            AfxMessageBox("FreeWheelTest Error 3: The robot's left pwm driver is malfunctioning");
            fprintf(out, "****FWTERROR 3****The robot's left pwm driver is malfunctioning\n");
        }
        else
        {
            AfxMessageBox("FreeWheelTest Error 4: The robot's left direction  encoder is malfunctioning");
            fprintf(out, "****FWTERROR 4****The robot's left direction encoder is malfunctioning\n");
        }
    }

END:
    //	m_Robot->SetInformationToTransmit(false, false, false, true);
    m_Robot->SetPwm(0, 0);
    m_Robot->SetObstacleSensitivity(osMediumSensitivity);
    m_Robot->SetState(stWaiting);
    m_Robot->EStop();
    fflush(out);
    fclose(out);

    if (TestSuccessful)
    {
        AfxMessageBox("Test Successful");
    }
    else
    {
        AfxMessageBox("Test Failed");
    }

    return TestSuccessful;
}

bool CDlgTestRobot::FreeWheelCheck(bool Forward, double* Velocities, int nMeasurements, bool TestResults[nFreeWheelTestResults], FILE* out)
{
    bool TestSuccessful = true;
    double AvgV, StdV, MinV, MaxV;

    if (Forward)
    {
        AvgV = Average(Velocities, nMeasurements, &StdV);
        MinV = Min(Velocities, nMeasurements);
        MaxV = Max(Velocities, nMeasurements);
    }
    else
    {
        AvgV = Average(Velocities, nMeasurements, &StdV);
        MinV = Max(Velocities, nMeasurements);
        MaxV = Min(Velocities, nMeasurements);
    }

    /*Check results*/
    const float cVelocityTestAllowablePercentError = 0.20f;
    const float cExpectedAvgVelocity = 250;
    const float cExpectedMaxVelocity = 360;
    const float cExpectedMinVelocity = 200;
    const float cAllowableSTD = 5;
    fprintf(out, "Raw Velocity Data\n");

    for (int i = 0; i < nMeasurements; i++)
    {
        fprintf(out, "%f\n", Velocities[i]);
    }

    /*Check for the motor going the right way*/
    TestResults[nFreeWheelTestDir] = true;

    if (Forward)
    {
        if (AvgV < 0)
        {
            TestResults[nFreeWheelTestDir] = false;
            TestSuccessful = false;
            fprintf(out, "Motor Going Wrong Way!!!\n");
        }
    }
    else
    {
        if (AvgV > 0)
        {
            TestResults[nFreeWheelTestDir] = false;
            TestSuccessful = false;
            fprintf(out, "Motor Going Wrong Way!!!\n");
        }
    }

    /*Check for zero velocities*/
    TestResults[nFreeWheelTestZeroV] = true;

    if ((AvgV == 0) || (MinV == 0) || (MaxV == 0))
    {
        TestResults[nFreeWheelTestZeroV] = false;
        TestSuccessful = false;
        fprintf(out, "Wheel Not Moving\n");
    }

    //	fprintf(out, "Average Velocity = %f, Velocity Expected = %f\n",AvgV, cExpectedAvgVelocity);
    //	if ( (AvgV <= 0) || (ABS(AvgV - cExpectedAvgVelocity)/cExpectedAvgVelocity > cVelocityTestAllowablePercentError) ) {
    //		fprintf(out, "Average Velocity test failed\n");
    //		TestSuccessful = false;
    //	}
    /*Check Max Velocity*/
    TestResults[nFreeWheelTestMax] = true;

    if (Forward)
    {
        if (MaxV > cExpectedMaxVelocity)
        {
            fprintf(out, "Max Velocity test failed\n");
            fprintf(out, "Max Velocity = %f, Velocity Allowed = %f\n", MaxV, cExpectedMaxVelocity);
            TestResults[nFreeWheelTestMax] = false;
            TestSuccessful = false;
        }
    }
    else
    {
        if (MaxV < -cExpectedMaxVelocity)
        {
            fprintf(out, "Max Velocity test failed\n");
            fprintf(out, "Max Velocity = %f, Velocity Allowed = %f\n", MaxV, -cExpectedMaxVelocity);
            TestResults[nFreeWheelTestMax] = false;
            TestSuccessful = false;
        }
    }

    /*Check Min Velocity*/
    if (Forward)
    {
        TestResults[nFreeWheelTestMin] = true;

        if (MinV < cExpectedMinVelocity)
        {
            fprintf(out, "Min Velocity test failed\n");
            fprintf(out, "Min Velocity = %f, Velocity Allowed = %f\n", MinV, cExpectedMinVelocity);
            TestResults[nFreeWheelTestMin] = false;
            TestSuccessful = false;
        }

        if (MinV < 0)
        {
            TestResults[nFreeWheelTestDir] = false;
            TestSuccessful = false;
            fprintf(out, "Motor Going Wrong Way!!!\n");
        }
    }
    else
    {
        TestResults[nFreeWheelTestMin] = true;

        if (MinV > -cExpectedMinVelocity)
        {
            fprintf(out, "Min Velocity test failed\n");
            fprintf(out, "Min Velocity = %f, Velocity Allowed = %f\n", MinV, -cExpectedMinVelocity);
            TestResults[nFreeWheelTestMin] = false;
            TestSuccessful = false;
        }

        if (MinV > 0)
        {
            TestResults[nFreeWheelTestDir] = false;
            TestSuccessful = false;
            fprintf(out, "Motor Going Wrong Way!!!\n");
        }
    }

    /*Check Standard Deviation*/
    if (StdV > cAllowableSTD)
    {
        fprintf(out, "Standard Deviation test failed\n");
        fprintf(out, "Standard Deviation = %f, Maximum Standard Deviation Allowed = %f\n", StdV, cAllowableSTD);
        TestResults[nFreeWheelTestStd] = false;
        TestSuccessful = false;
    }

    return TestSuccessful;
}


void CDlgTestRobot::OnButM1Backward()
{
    m_Robot->SetPwm(0, -555);
}

void CDlgTestRobot::OnButM1Forward()
{
    m_Robot->SetPwm(0, 555);
}

void CDlgTestRobot::OnButStopMotors()
{
    m_Robot->SetPwm(0, 0);
}

void CDlgTestRobot::OnButM0Forward()
{
    m_Robot->SetPwm(555, 0);
}

void CDlgTestRobot::OnButM0Backward()
{
    m_Robot->SetPwm(-555, 0);
}

bool CDlgTestRobot::CheckBackAndForth(unsigned char Motor)
{
    m_Robot->SetPwm(0, 0);
    CTime WaitUntilTime;

    if (Motor == 0)
    {
        AfxMessageBox("Press OK then see if the robot's right\nmotor rotates back and forth");
    }
    else
    {
        AfxMessageBox("Press OK then see if the robot's left\nmotor rotates back and forth");
    }

    /*Wait for a while to get times in sync*/
    WaitUntilTime = CTime::GetCurrentTime();
    WaitUntilTime += CTimeSpan(0, 0, 0, 2);
    bool done = false;

    while (!done)
    {
        m_Robot->DoWindowMessages();
        CTimeSpan TimeDiff = WaitUntilTime - CTime::GetCurrentTime();

        if ((TimeDiff.GetHours() == 0) && (TimeDiff.GetMinutes() == 0) &&
                (TimeDiff.GetSeconds() <= 0))
        {
            done = true;
        }
    }

    short Pwm = 555;

    for (int i = 0; i < 4; i++)
    {
        if (Motor == 0)
        {
            m_Robot->SetPwm(Pwm, 0);
        }
        else
        {
            m_Robot->SetPwm(0, Pwm);
        }

        if (m_Robot->GetState() != stExecuting)
        {
            goto END;
        }

        Pwm = -Pwm;
        /*Wait for a second*/
        WaitUntilTime = CTime::GetCurrentTime();
        WaitUntilTime += CTimeSpan(0, 0, 0, 2);
        done = false;

        while (!done)
        {
            m_Robot->DoWindowMessages();
            CTimeSpan TimeDiff = WaitUntilTime - CTime::GetCurrentTime();

            if ((TimeDiff.GetHours() == 0) && (TimeDiff.GetMinutes() == 0) &&
                    (TimeDiff.GetSeconds() <= 0))
            {
                done = true;
            }
        }
    }

END:
    m_Robot->SetPwm(0, 0);
    return (AfxMessageBox("Did the robot move back and forth?", MB_YESNO) == IDYES);
}

bool CDlgTestRobot::AgainstWallTest()
{
    const float cMinStuckAD = 0.49f;
    const float cMaxStuckAD = 0.98f;
    bool TestSuccessful = true;
    CTime WaitUntilTime;
    std::ofstream out("Against Wall Test.txt");
    AfxMessageBox("Please place the robot against a flat section of wall");

    m_Robot->SetObstacleSensitivity(osNone);
    m_Robot->m_MotorObstacle.ValidObstacle = false;
    //	m_Robot->SetInformationToTransmit(true, true, true, false);
    m_Robot->SetPwm(0, 0);
    m_Robot->SetState(stExecuting);

    /*Checking the battery*/
    /*Wait for 5 seconds for ADs to get transmitted*/
    for (int i = 0; i < 5; i++)
    {
        WaitUntilTime = CTime::GetCurrentTime();
        WaitUntilTime += CTimeSpan(0, 0, 0, 1);
        bool done = false;

        while (!done)
        {
            m_Robot->DoWindowMessages();
            CTimeSpan TimeDiff = WaitUntilTime - CTime::GetCurrentTime();

            if ((TimeDiff.GetHours() == 0) && (TimeDiff.GetMinutes() == 0) &&
                    (TimeDiff.GetSeconds() <= 0))
            {
                done = true;
            }
        }
    }

    if (m_Robot->m_BatteryVoltageMeasured < 12.0f)
    {
        if (AfxMessageBox("Have you fully charged the battery?", MB_YESNO) == IDYES)
        {
            AfxMessageBox("****AWTERROR 1**** Battery Level Low");
            out << "****AWTERROR 1**** Battery Level Low\n";
            out << "Battery Level = " << m_Robot->m_BatteryVoltageMeasured << "\n";
            TestSuccessful = false;
        }
        else
        {
            AfxMessageBox("Please charge the battery before repeating this test");
        }
    }

    /*Checking M0, M1 ADs*/
    if (m_Robot->m_M0CurrentMeasured > 0)
    {
        AfxMessageBox("****AWTERROR 2**** M0AD Level High");
        out << "****AWTERROR 2**** M0AD Level High\n";
        out << "M0AD Level = " << m_Robot->m_M0CurrentMeasured << "\n";
        TestSuccessful = false;
    }

    if (m_Robot->m_M1CurrentMeasured > 0)
    {
        AfxMessageBox("****AWTERROR 3**** M1AD Level High");
        out << "****AWTERROR 3**** M1AD Level High\n";
        out << "M1AD Level = " << m_Robot->m_M1CurrentMeasured << "\n";
        TestSuccessful = false;
    }

    if (m_Robot->GetState() != stExecuting)
    {
        TestSuccessful = false;
        goto END;
    }

    /*Give motors a little juice and repeat the tests*/
    m_Robot->SetPwm(333, 333);

    /*Wait a while for ADs to get updated*/
    for (int i = 0; i < 3; i++)
    {
        WaitUntilTime = CTime::GetCurrentTime();
        WaitUntilTime += CTimeSpan(0, 0, 0, 1);
        bool done = false;

        while (!done)
        {
            m_Robot->DoWindowMessages();
            CTimeSpan TimeDiff = WaitUntilTime - CTime::GetCurrentTime();

            if ((TimeDiff.GetHours() == 0) && (TimeDiff.GetMinutes() == 0) &&
                    (TimeDiff.GetSeconds() <= 0))
            {
                done = true;
            }
        }
    }

    float M0AD, M1AD, BatteryLevel;
    M0AD = m_Robot->m_M0CurrentMeasured;
    M1AD = m_Robot->m_M1CurrentMeasured;
    BatteryLevel = m_Robot->m_BatteryVoltageMeasured;
    m_Robot->SetPwm(0, 0);

    if (M0AD  < cMinStuckAD)
    {
        AfxMessageBox("****AWTERROR 4**** M0AD Level low");
        out << "****AWTERROR 4**** AD Level low\n";
        out << "M0AD = " << M0AD << "\n";
        TestSuccessful = false;
    }

    if (M0AD > cMaxStuckAD)
    {
        AfxMessageBox("****AWTERROR 5**** M0AD Level high");
        out << "****AWTERROR 5**** M0AD Level high\n";
        out << "M0AD = " << M0AD << '\n';
        TestSuccessful = false;
    }

    if (M1AD < cMinStuckAD)
    {
        AfxMessageBox("****AWTERROR 6**** M1AD Level low");
        out << "****AWTERROR 6**** M1AD Level low\n";
        out << "M1AD = " << M1AD << '\n';
        TestSuccessful = false;
    }

    if (M1AD > cMaxStuckAD)
    {
        AfxMessageBox("****AWTERROR 7**** M1AD Level high");
        out << "****AWTERROR 7**** M1AD Level high\n";
        out << "M1AD = " << M1AD << '\n';
        TestSuccessful = false;
    }

    if (BatteryLevel < 10)
    {
        AfxMessageBox("****AWTERROR 8**** Battery Level Low");
        out << "****AWTERROR 8**** Battery Level Low\n";
        out << "Battery Level = " << BatteryLevel << '\n';
        TestSuccessful = false;
    }

    if (m_Robot->GetState() != stExecuting)
    {
        TestSuccessful = false;
        goto END;
    }

END:

    if (TestSuccessful)
    {
        AfxMessageBox("Test Successful");
        out << "Test Successful\n";
    }
    else
    {
        AfxMessageBox("Test Failed");
    }

    //	m_Robot->SetInformationToTransmit(false, false, false, true);
    m_Robot->SetObstacleSensitivity(osMediumSensitivity);
    m_Robot->EStop();
    return TestSuccessful;
}

void CDlgTestRobot::OnButAgainstWallTest()
{
    AgainstWallTest();
}


bool CDlgTestRobot::RotationTest()
{
    std::ofstream out("Rotation Test.txt");
    bool TestSuccessful = true;
    const int nSamples = 5;
    const double cMaxSTD = 3;
    AfxMessageBox("Please place the robot against a wall");
    //	m_Robot->SetInformationToTransmit(false, false, false, true);
    int TicksPerRotation[nSamples];
    double AverageTicks = 0;
    double std = 0.0;
    int i;
    /*Do it counterclockwise*/
    m_Robot->AddVelocitiesToInstructions(cObstacleDetectionSpeed, cObstacleDetectionSpeed, 0);

    if (m_Robot->FollowInstructions(TRUE, TRUE) == IR_HitObstacle)
    {
        if (m_Robot->SquareToWall(m_Robot->m_MotorObstacle.MotorObstacle) != TR_TaskComplete)
        {
            TestSuccessful = false;
            goto END;
        }
    }
    else
    {
        TestSuccessful = false;
        goto END;
    }

    m_Robot->FindTicksPerRotation((int)AverageTicks, TicksPerRotation, true);
    AverageTicks = TicksPerRotation[0];
    TTaskResult TaskResult;

    for (i = 0; i < nSamples; i++)
    {
        TaskResult = m_Robot->FindTicksPerRotation((int)AverageTicks, TicksPerRotation + i, true);

        if (TaskResult != TR_TaskComplete)
        {
            AfxMessageBox("***ROTATION ERROR*** Task Incomplete");
            out << "***ROTATION ERROR*** CCW Ticks Per Rotation Task Incomplete.  TaskResult = " << TaskResult << "\n";
            TestSuccessful = false;
            goto END;
        }

        AverageTicks = Average(TicksPerRotation, i, &std);
    }

    if (std > cMaxSTD)
    {
        AfxMessageBox("****ROTATION ERROR 1**** Std out of range");
        out << "****ROTATION ERROR 1**** Std out of range\n";
        TestSuccessful = false;
    }
    else
    {
        out << "Rotation Test successful\n";
    }

    out << "Raw CCW Rotation Data\n";

    for (i = 0; i < nSamples; i++)
    {
        out << TicksPerRotation[i] << '\n';
    }

    out << "Average = " << AverageTicks << '\n';
    out << "std = " << std << '\n';
    /*Do it clockwise*/
    m_Robot->FindTicksPerRotation((int)AverageTicks, TicksPerRotation, false);
    AverageTicks = TicksPerRotation[0];

    for (i = 0; i < nSamples; i++)
    {
        TaskResult = m_Robot->FindTicksPerRotation((int)AverageTicks, TicksPerRotation + i, false);

        if (TaskResult != TR_TaskComplete)
        {
            AfxMessageBox("***ROTATION ERROR*** Task Incomplete");
            out << "***ROTATION ERROR*** CW Ticks Per Rotation Task Incomplete.  TaskResult = " << TaskResult << "\n";
            TestSuccessful = false;
            goto END;
        }

        AverageTicks = Average(TicksPerRotation, i + 1, &std);
    }

    if (std > cMaxSTD)
    {
        AfxMessageBox("****ROTATION ERROR 2**** Std out of range");
        out << "****ROTATION ERROR 2**** Std out of range\n";
        TestSuccessful = false;
    }
    else
    {
        out << "Rotation Test successful\n";
    }

    out << "Raw CW Rotation Data\n";

    for (i = 0; i < nSamples; i++)
    {
        out << TicksPerRotation[i] << '\n';
    }

    out << "Average = " << AverageTicks << '\n';
    out << "std = " << std << '\n';
END:
    m_Robot->EStop();
    //	m_Robot->SetInformationToTransmit(false, false, false, true);
    m_Robot->SetObstacleSensitivity(osMediumSensitivity);
    return TestSuccessful;
}

void CDlgTestRobot::OnButRotateTest()
{
    RotationTest();
}

void CDlgTestRobot::OnTestMicrophone()
{
    m_Robot->SetBuzzerFrequency(1000);
    m_Robot->AddWaitToInstructions(1000);
    m_Robot->FollowInstructions();
    m_Robot->SetBuzzerFrequency(0);
    //	if (m_Robot->GetMicrophoneVolume() < 1) {
    //	} else {
    //	}
}

void CDlgTestRobot::OnTestInput()
{
    //	m_Robot->SetExternalOutput(false);
    //	m_Robot->SetExternal>GetAnalog();
}
