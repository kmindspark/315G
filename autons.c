
#include "helperfunctions.c"

string autons[3] = {"No Auton", "20 Pt Cone", "Score 1 Cone"};

void autonomousConeIn20Pt(bool reverse, bool stopAfterTime)
{
	closeClaw();
	forward(127,300);
	//turnRight(127,250,reverse);
	//turnLeft(127,250,reverse);
	assignArmMotors(127);
	assignMogoMotors(-127);
	forward(127,600);
	assignArmMotors(10);
	wait1Msec(400);
	assignMogoMotors(0);
	forward(127, 650);
	assignMogoMotors(127);
	wait1Msec(1300);
	assignMogoMotors(0);
	wait1Msec(200);
	assignArmMotors(-127);
	wait1Msec(400);
	assignArmMotors(0);
	assignFlipFlop(50);
	wait1Msec(500);
	assignFlipFlop(0);
	openClaw();
	if (stopAfterTime){
		return;
	}
	assignArmMotors(127);
	wait1Msec(600);
	assignArmMotors(10);
	backward(127,1150);
	turnRight(127,135,reverse);
	forward(127, 800);
	turnRight(127, 90, reverse);
	forward(127, 700);
	assignMogoMotors(-127);
	forward(127, 600);
	wait1Msec(300);
	assignMogoMotors(0);
	backward(127, 1000);
}
