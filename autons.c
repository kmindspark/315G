
#include "helperfunctions.c"

string autons[7] = {"No Auton", "20 Pt Cone", "5 Pt Cone", "1 Cone", "10 Pt Cone", "Stationary", "iM a DeFeNsE bOt"};

void autonomousStationary(){
	motor[claw] = 20;
	assignArmMotors(127);
	assignFlipFlop(-127);
	while (SensorValue[potArm] < STATIONARYARMPOS){

	}
	assignArmMotors(-20);
	wait1Msec(200);
	assignArmMotors(10);
	assignFlipFlop(0);
	forward(50, 700);
	assignArmMotors(-20);
	wait1Msec(300);
	backwardTime(30, 800);
	openClaw();
	backwardTime(127, 500);
	assignArmMotors(0);
	motor[claw] = 0;
}

void autonomousConeIn20Pt(bool reverse, bool stopAfterTime, bool fivept, bool tenpt)
{
	clearTimer(T1);
	motor[claw] = 20;
	//turnRight(127,250,reverse);
	//turnLeft(127,250,reverse);
	assignArmMotors(60);
	assignMogoMotors(-127);
	forward(127,600);
	assignArmMotors(10);
	forward(127,1400);
	assignMogoMotors(127);
	wait1Msec(1300);
	assignMogoMotors(0);
	assignArmMotors(-127);
	wait1Msec(200);
	assignArmMotors(0);
	wait1Msec(200);
	openClaw();
	if (stopAfterTime){
		return;
	}
	//backward(127,1150);
	backward(127,1850);
	turnRight(127,90,reverse);
	backwardTime(127,500);
	backwardTime(80,400);
	if (fivept)
	{
		turnRight(127,90,reverse);
		if (tenpt){
			forwardTime(127, 800);
		}
		assignArmMotors(127);
		wait1Msec(300);
		assignArmMotors(10);
		assignMogoMotors(-127);
		wait1Msec(1000);
		assignMogoMotors(0);
		backward(127,1000);
		return;
}
	forward(127, 200);
	turnRight(127,60,reverse);
	forward(127, 1000);//880
	turnRight(127, 75, reverse);
	forward(100, 700);
	assignArmMotors(127);
	wait1Msec(300);
	assignArmMotors(10);
	assignMogoMotors(-127);
	forwardTime(100, 900);
	wait1Msec(500);
	assignMogoMotors(0);
	backwardTime(127, 300);
	assignMogoMotors(127);
	/*forwardTime(127, 300);
	wait1Msec(200);*/
	backwardTime(127,700);
	assignMogoMotors(0);
	displayLCDNumber(1, 1, time1[T1]);
}

void autonDefense(){
	closeClaw();
	backwardTime(127, 4000);
}
