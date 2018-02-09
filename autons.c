#include "helperfunctions.c"

string autons[8] = {"No Auton", "1 Cone", "5 Pt Cone", "10 Pt Cone", "20 Pt Cone", "20 Pt 2 Cones", "Stationary", "iM a DeFeNsE bOt"};

void autonomousStationary(bool reverse){
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

	turnLeft(127, 90, reverse);
	forwardTime(127, 1000);
}

void autonomousConeIn20Pt(bool reverse, bool stopAfterTime, bool fivept, bool tenpt, bool extraCones)
{
	clearTimer(T1);
	motor[claw] = 20; //Drive forward to pick up mobile goal and score the cone on it
	assignArmMotors(60);
	assignMogoMotors(-127);
	wait1Msec(200);
	forward(127,1200);
	assignArmMotors(10);
	assignMogoMotors(0);
	forward(127,800);
	assignMogoMotors(127);
	wait1Msec(1300);
	assignMogoMotors(15);
	assignArmMotors(-127);
	wait1Msec(400);
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
	forwardTime(50, 500);
	assignMogoMotors(0);
	backwardTime(127, 300);
	assignMogoMotors(127);
	assignArmMotors(-25);
	/*forwardTime(127, 300);
	wait1Msec(200);*/
	backwardTime(127,800);
	assignMogoMotors(0);
	displayLCDNumber(1, 1, time1[T1]);
}

void autonDefense(){
	closeClaw();
	backwardTime(127, 4000);
}


void programmingSkills(){
	assignMogoMotors(-127);
	wait1Msec(500);
	forwardDistance(127, 700);
	assignMogoMotors(127);
    forwardDistance(40, 50)
	wait1Msec(500);
	assignMogoMotors(10);
    backwardDistance(127, 600);
    turnRight(80, 45, false);
    backwardDistance(127, 300);
    turnRight(80, 127, false);
    assignMogoMotors(-80);
    forwardTime(127, 1000);
    assignMogoMotors(127);
    backwardDistance(127, 300);
    assignMogoMotors(0);
    turnLeft(80, 90, false);
    forwardDistance(127, 300);
    turnLeft(80, 90, false);
    assignMogoMotors(-127);
    wait1Msec(500);
    assignMogoMotors(0);
    forwardDistance(127, 400);
    forwardDistance(40, 50)
    assignMogoMotors(127);
    wait1Msec(500);
    assignMogoMotors(10);
    turnRight(80, 180, false);
    forwardDistance(127, 400);
    assignMogoMotors(-127);
    wait1Msec(500);
    assignMogoMotors(0);
    backwardDistance(127, 200);
    turnRight(80, 90, false);
    forwardDistance(127, 300);

}