#include "helperfunctions.c"

string autons[8] = {"No Auton", "20 Pt 4 C.", "1 Cone", "5 Pt Cone", "10 Pt Cone", "20 Pt Cone", "Stationary", "iM a DeFeNsE bOt"};

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
	endAutoStackEarly = false;
	clearTimer(T1);
	motor[claw] = 20; //Drive forward to pick up mobile goal and score the cone on it
	assignArmMotors(45);
	assignMogoMotors(-127);
	forward(127,1400);
	assignArmMotors(10);
	assignMogoMotors(-10);
	forward(60,600);
	assignMogoMotors(127);
	wait1Msec(650);
	assignArmMotors(-127);
	wait1Msec(200);
	assignMogoMotors(15);
	assignArmMotors(0);
	wait1Msec(200);
	openClaw(); //At this point the one cone is scored
	if (extraCones){
		assignArmMotors(40);
		assignFlipFlop(-127);
		wait1Msec(100);
		assignArmMotors(-50);
		forwardDistance(127, 240);
		assignArmMotors(-10);
		assignFlipFlop(0);
		numCones = 1;
		startTask(autoStack);
		wait1Msec(600);
		forwardDistance(127, 290);
		while (autoStackingInProgress){
			wait1Msec(20);
		}
		numCones = 2;
		wait1Msec(200);
		startTask(autoStack);
		wait1Msec(600);
		forwardDistance(127, 245);
		while (autoStackingInProgress){
			wait1Msec(20);
		}
		numCones = 3;
		endAutoStackEarly = true;
		startTask(autoStack);
		wait1Msec(200);
		backwardDistance(127, 300, false);
	}

	if (stopAfterTime){
		return;
	}

	backward(127,2350);
	turnLeft(127,45,reverse);//Drive backwards into the wall to align
	if (fivept) //Code to run for 5 pt auton
	{
		turnRight(127,90,reverse);
		if (tenpt){
			forwardTime(127, 800);
		}
		assignArmMotors(60);
		wait1Msec(300);
		assignArmMotors(10);
		assignMogoMotors(-127);
		wait1Msec(1000);
		assignMogoMotors(0);
		backward(127,1000);
		return; //Exit from autonomous since 5 point auton is complete
	}

	backwardDistance(127, 600); //Navigate to the 20 point zone
	turnLeft(127,90,reverse);
	assignArmMotors(60);
	forwardTime(127, 500);
	assignArmMotors(10);
	assignMogoMotors(-127); //Begin deposit sequence
	forwardTime(50, 750);
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
	closeClaw();
	assignMogoMotors(-127);
	wait1Msec(500);
	assignMogoMotors(-10);
	forwardDistance(120, 1200, false);
	forwardDistance(40, 200, false);
	wait1Msec(200);
	assignMogoMotors(127);
	wait1Msec(800);
	assignMogoMotors(10);
  backwardDistance(120, 1330);
  turnRight(50, 45, false);
  backwardDistance(120, 545); //575
  turnRight(50, 90, false);
  assignMogoMotors(-100);
  forwardTime(80, 1200);
  assignMogoMotors(80);
  backwardDistance(60, 450);
  assignMogoMotors(0);
  turnLeft(45, 90, false);
  assignMogoMotors(-127);
  forwardDistance(120, 380);
  assignMogoMotors(0);
  turnLeft(45, 90, false);
  forwardDistance(80, 770, false);
  forwardDistance(20, 100, false);
  assignMogoMotors(127);
  wait1Msec(800);
  assignMogoMotors(10); //have picked up second mogo
  //backwardDistance(80, 185);
	openClaw();
  turnLeft(50, 185, false); //190
  stopTask(brakeWheels);
  forwardDistance(120, 500, false); //deposit second mogo
  assignMogoMotors(-80);
  forwardDistance(70, 350, false);
  wait1Msec(200);
  backwardDistance(120, 140);
  assignMogoMotors(100);
  wait1Msec(400);
  assignMogoMotors(0);
  turnRight(45, 90, false);
  forwardDistance(127, 785);//785
  turnRight(45, 90, false);
  assignMogoMotors(-127);
  wait1Msec(500);
  assignMogoMotors(0); //go for third mogo
  forwardDistance(100, 600, false);
  forwardDistance(30, 100, false);
  assignMogoMotors(127);
  wait1Msec(800);
  assignMogoMotors(15);
  turnLeft(50, 185, false);
  forwardDistance(120, 500, false);
  assignMogoMotors(-127);
  forwardDistance(80, 360, false);
  wait1Msec(200);
  assignMogoMotors(0); //deposit third mogo
  backwardDistance(127, 475);
  assignMogoMotors(70);
  turnRight(45, 180, false);
  assignMogoMotors(-127);
  wait1Msec(800);
  assignMogoMotors(-20);
  forwardDistance(120, 1500, false);
  forwardDistance(60, 200, false);
  assignMogoMotors(127);
  wait1Msec(900);
  forwardDistance(120, 450);
  assignMogoMotors(60);
  turnRight(50, 60, false);
  forwardDistance(120, 540);//650
  turnLeft(50, 60, false);
  assignMogoMotors(-127);
  forwardTime(100, 1300);
  assignMogoMotors(80);
  backwardDistance(60, 450); //deposit 20 pt on other side
  assignMogoMotors(0);
  turnLeft(45, 90, true);
  assignMogoMotors(-127);
  forwardDistance(120, 420);
  assignMogoMotors(0);
  turnRight(45, 90, false);
  forwardDistance(80, 770, false);
  forwardDistance(20, 100, false);
  assignMogoMotors(127);
  wait1Msec(800);
  assignMogoMotors(10); //have picked up second mogo
  backwardDistance(80, 185);
  turnLeft(50, 185, true);
  stopTask(brakeWheels);
  forwardDistance(120, 790, false); //deposit second mogo
  assignMogoMotors(-127);
  wait1Msec(500);
  backwardDistance(120, 150);
  assignMogoMotors(100);
  backwardDistance(120, 400);
  assignMogoMotors(0);
  turnRight(45, 45, false);
  forwardTime(127, 800);
  wait1Msec(100);
  backwardDistance(50, 100);
  assignMogoMotors(-127);
  turnRight(50, 85, false);
	forwardDistance(120, 850, false);
	assignMogoMotors(127);
	wait1Msec(1000);
	assignMogoMotors(30);
  backwardDistance(127, 1300);
  turnRight(127, 150, false);
  forwardDistance(127, 250, false);
  assignMogoMotors(-120);
  turnRight(127, 35, false);
  forwardDistance(127, 200, false);
  wait1Msec(200);
  backwardDistance(127, 200, false);
  turnLeft(127, 30, false);
  backwardDistance(127, 2000);
}
