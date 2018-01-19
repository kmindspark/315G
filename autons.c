#include "helperfunctions.c"

string autons[8] = {"No Auton", "1 Cone", "5 Pt Cone", "10 Pt Cone", "20 Pt Cone", "20 Pt 2 Cones", "Stationary", "iM a DeFeNsE bOt"};

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

void autonomousConeIn20Pt(bool reverse, bool stopAfterTime, bool fivept, bool tenpt, bool extraCones)
{
	clearTimer(T1);
	motor[claw] = 20; //Drive forward to pick up mobile goal and score the cone on it
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
	openClaw(); //At this point the one cone is scored

	if (stopAfterTime){
		return;
	}

	if (extraCones){
		//TODO: fill in with new autonomous subroutine
	}

	backward(127,1850);
	turnRight(127,90,reverse);
	backwardTime(127,500);
	backwardTime(80,400); //Drive backwards into the wall to align
	if (fivept) //Code to run for 5 pt auton
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
		return; //Exit from autonomous since 5 point auton is complete
	}
	forward(127, 200); //Navigate to the 20 point zone
	turnRight(127,60,reverse);
	forward(127, 1000);//880
	turnRight(127, 75, reverse);
	forward(100, 700);
	assignArmMotors(127);
	wait1Msec(300);
	assignArmMotors(10);
	assignMogoMotors(-127); //Begin deposit sequence
	forwardTime(100, 900);
	wait1Msec(500);
	assignMogoMotors(0);
	backwardTime(127, 300);
	assignMogoMotors(127);
	backwardTime(127,700);
	assignMogoMotors(0);
	displayLCDNumber(1, 1, time1[T1]);
}

void autonDefense(){
	closeClaw();
	backwardTime(127, 4000);
}