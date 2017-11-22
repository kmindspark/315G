#pragma config(Sensor, in2,    potArm,         sensorPotentiometer)
#pragma config(Sensor, in3,    potFlipFlop,    sensorPotentiometer)
#pragma config(Sensor, in4,    gyro,           sensorGyro)
#pragma config(Sensor, dgtl1,  leftEncoder,    sensorQuadEncoder)
#pragma config(Sensor, dgtl3,  rightEncoder,   sensorQuadEncoder)
#pragma config(Motor,  port1,           mogoL,         tmotorVex393_HBridge, openLoop, reversed)
#pragma config(Motor,  port2,           df,            tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port3,           db,            tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port4,           flipflop,      tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port5,           armL,          tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port6,           armR,          tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port7,           claw,          tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port8,           pf,            tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port9,           pb,            tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port10,          mogoR,         tmotorVex393_HBridge, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#pragma platform(VEX)

#pragma competitionControl(Competition)
#pragma autonomousDuration(20)
#pragma userControlDuration(120)

#include "Vex_Competition_Includes.c"   //Main competition background code...do not modify!

#define vexCompetitionState (nVexRCReceiveState & (vrDisabled | vrAutonomousMode))
#define LEFTBUTTON 1
#define CENTERBUTTON 2
#define RIGHTBUTTON 4
#define SLEWCONST 0.2

int goalDrivePowerL = 0;
int goalDrivePowerR = 0;
int currentDrivePowerL = 0;
int currentDrivePowerR = 0;

TVexReceiverState competitionState;
int numCones = 0;
int autonChoice = 0;
bool left = true;
bool brake = false;
int autonForwardBrake = 0;
int goalDriveValue;
int autonAngleBrake = 0;
int goalDriveAngle;

bool skills = false;

#include "autons.c"

int filter(int input){
	if (abs(input) > 20){
		return input;
	}
	else {
		return 0;
	}
}

task slew{
	int previousGoalPowerL = 0;
	int previousGoalPowerR = 0;
	int currentChangeL = 0;
	int currentChangeR = 0;
	int count = 0;
	while (true){
		if (previousGoalPowerL != goalDrivePowerL || previousGoalPowerR != goalDrivePowerR){
			previousGoalPowerL = goalDrivePowerL;
			previousGoalPowerR = goalDrivePowerR;
			currentChangeL = goalDrivePowerL - currentDrivePowerL;
			currentChangeR = goalDrivePowerR - currentDrivePowerR;
			count = 0;
		}
		if (count <= 5){
			count = count + 1;
			currentDrivePowerL = currentDrivePowerL + currentChangeL*SLEWCONST;
			currentDrivePowerR = currentDrivePowerR + currentChangeR*SLEWCONST;
		}
		else{
			currentDrivePowerL = goalDrivePowerL;
			currentDrivePowerR = goalDrivePowerR;
		}
		wait1Msec(20);
	}
}

task drive(){
	autonBrake = 0;
	startTask(slew);
	int forward;
	int turn;
	int motor1;
	int motor2;
	int motor3;
	int motor4;
	while (true){
		if (abs(filter(vexRT[Ch3])) > 0 || abs(filter(vexRT[Ch1])) > 0){
			brake = false;
			stopTask(brakeWheels);
			forward = filter(vexRT[Ch3]);
			turn = filter(vexRT[Ch1]);
		}
		else{
			forward = filter(vexRT[Ch3] + vexRT[Ch3Xmtr2]/2);
			turn = filter(vexRT[Ch1] + vexRT[Ch1Xmtr2]/2);
		}

		goalDrivePowerL = forward + turn;
		goalDrivePowerR = forward - turn;
		motor1 = currentDrivePowerL;
		motor2 = currentDrivePowerL;
		motor3 = currentDrivePowerR;
		motor4 = currentDrivePowerR;
		motor[df] = motor1;
		motor[db] = motor2;
		motor[pf] = motor3;
		motor[pb] = motor4;

		if (vexRT[Btn8L]){
			brake = !brake;
			if (brake){
				SensorValue[leftEncoder] = 0;
				SensorValue[rightEncoder] = 0;
				goalDriveValue = 0;
				startTask(brakeWheels);
			} else {
				stopTask(brakeWheels);
			}
			wait1Msec(200);
		}
	}
}

task arm(){
	while(true){
		if(vexRT[Btn6U] == 1){
			stopTask(maintainArmPos);
			assignArmMotors(127);
			while(vexRT[Btn6U] == 1)
			{

			}
			startTask(maintainArmPos);
		}
		if(vexRT[Btn6D] == 1){
			stopTask(maintainArmPos);
			assignArmMotors(-127);
			while(vexRT[Btn6D] == 1)
			{

			}
			assignArmMotors(0);
		}
		if (vexRT[Btn7R] == 1){
			stopTask(maintainArmPos);
			autoStackCones();
			numCones++;
		}
		if (vexRT[Btn7L] == 1){
			if (currentDownPos == BOTTOMARMPOS){
				currentDownPos = LOADERARMPOS;
			}
			if (currentDownPos == LOADERARMPOS){
				currentDownPos = BOTTOMARMPOS;
			}
			wait1Msec(300);
		}
	}
}

task mogo(){
	while(true){
		if(vexRT[Btn8U]){
			assignMogoMotors(127);
			while(vexRT[Btn8U])
			{

			}
			assignMogoMotors(5);
		}
		if(vexRT[Btn8D]){
			assignMogoMotors(-127);
			while(vexRT[Btn8D] == 1)
			{

			}
			assignMogoMotors(0);
		}
	}
}

task flipfloptask {
	while (true) {
		if (vexRT[Btn5U]){
			motor[flipflop] = 127;
			while (vexRT[Btn5U]){
			}
			motor[flipflop] = 0;
		}
		if (vexRT[Btn5D]){
			motor[flipflop] = -127;
			while (vexRT[Btn5D]){

			}
			motor[flipflop] = -5;
		}
	}
}

task clawtask {
	while (true) {
		if (vexRT[Btn7U] == 1){
			while (vexRT[Btn7U] == 1){
				motor[claw] = 127;
			}
			motor[claw] = 25;
		}
		if (vexRT[Btn7D] == 1){
			while (vexRT[Btn7D] == 1){
				motor[claw] = -127;
			}
			motor[claw] = -20;
		}
	}
}

task coneCounter(){
	while (true){
		if (vexRT[Btn8DXmtr2]){
			numCones = 0;
		}
		if (vexRT[Btn8LXmtr2]){
			numCones--;
		}
		if (vexRT[Btn8RXmtr2]){
			numCones++;
		}
	}
}

void pre_auton(){
	bLCDBacklight = true;
	displayLCDCenteredString(0, "Init. gyro");
	SensorType[gyro] = sensorNone;
	wait1Msec(2000);
	SensorType[gyro] = sensorGyro;
	wait1Msec(2000);
	competitionState = vexCompetitionState;
	clearLCDLine(0);
	clearLCDLine(1);
	bool chosen = false;
	displayLCDCenteredString(0, autons[autonChoice]);
	clearTimer(T1);
	if (nLCDButtons != LEFTBUTTON){
		return;
	}
	while (!chosen && vexCompetitionState == competitionState){
		waitForPress();
		if (nLCDButtons == RIGHTBUTTON){
			waitForRelease();
			if (autonChoice < (sizeof(autons)/2) - 1){
				autonChoice++;
			}
			clearLCDLine(0);
			displayLCDCenteredString(0, autons[autonChoice]);
		}
		else if (nLCDButtons == LEFTBUTTON){
			waitForRelease();
			if (autonChoice > 0){
				autonChoice--;
			}
			clearLCDLine(0);
			displayLCDCenteredString(0, autons[autonChoice]);
		}
		else if (nLCDButtons == CENTERBUTTON){
			waitForRelease();
			clearLCDLine(0);
			displayLCDCenteredString(0, autons[autonChoice]);
			chosen = true;
		}
	}
	chosen = false;
	clearLCDLine(0);
	displayLCDCenteredString(0, "left  or  right");
	while (!chosen && vexCompetitionState == competitionState){
		waitForPress();
		if (nLCDButtons == RIGHTBUTTON){
			waitForRelease();
			left = false;
			chosen = true;
		}
		else if (nLCDButtons == LEFTBUTTON){
			waitForRelease();
			left = true;
			chosen = true;
		}
	}
	clearLCDLine(0);
	clearLCDLine(1);
	if (left){
		displayLCDCenteredString(0, "left");
	}
	else {
		displayLCDCenteredString(0, "right");
	}
	displayLCDCenteredString(1, autons[autonChoice]);
}

task autonomous()
{
	if (left) {
		switch (autonChoice){
			case 1: autonomousConeIn20Pt(false, true, false, false, false); break;
			case 2: autonomousConeIn20Pt(false, false, true, false, false); break;
			case 3: autonomousConeIn20Pt(false, false, true, true, false); break;
			case 4: autonomousConeIn20Pt(false, false, false, false, false); break;
			case 5: autonomousConeIn20Pt(false, false, false, false, true); break;
			case 6: autonomousStationary(); break;
			case 7: autonDefense(); break;
			default: break;
		}
	} else {
		switch (autonChoice){
			case 1: autonomousConeIn20Pt(true, true, false, false, false); break;
			case 2: autonomousConeIn20Pt(true, false, true, false, false); break;
			case 3: autonomousConeIn20Pt(true, false, true, true, false); break;
			case 4: autonomousConeIn20Pt(true, false, false, false, false); break;
			case 5: autonomousConeIn20Pt(false, false, false, false, true); break;
			case 6: autonomousStationary(); break;
			case 7: autonDefense(); break;
			default: break;
		}
	}
}

task usercontrol(){
	startTask(drive);
	/*
	startTask(arm);
	startTask(clawtask);
	startTask(flipfloptask);
	startTask(mogo);
	startTask(coneCounter);*/
}
