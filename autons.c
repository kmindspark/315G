#include "helperfunctions.c"

string autons[11] = {"No Auton", "4 in 20", "2 in 20", "1 in 20", "4 in 5", "2 in 5", "1 in 5", "Stackalot", "Stat + 2 in 5", "Stat + Def", "iM a DeFeNsE bOt"};

void autonomousStationary(bool reverse, bool extraCones){
	motor[rollers] = 35;
	assignArmMotors(127);
	assignMogoMotors(127);
	forwardDistance(127, 120, false, true);
	assignArmMotors(127);
	forwardDistance(127, 300, false, false);
	while (SensorValue[potArm] < STATIONARYARMPOS)
	{

	}
	assignArmMotors(0);
	assignMogoMotors(10);
	startTask(maintainArmPos);
	//assignMogoMotors(-127);
	//wait1Msec(400);
	//assignMogoMotors(0);
	forwardDistance(80, 490, true, false);
	//assignArmMotors(-10);

	/**
	openRollers();
	assignArmMotors(127);
	wait1Msec(75);
	assignArmMotors(0);
	assignFlipFlop(127);
	wait1Msec(30);
	assignFlipFlop(0);
	assignArmMotors(-127);
	wait1Msec(30);
	assignArmMotors(0);
	*/
	wait1Msec(200);
	assignFlipFlop(-127);
	//assignArmMotors(-40);
	wait1Msec(500);
	stopTask(maintainArmPos);
	assignArmMotors(0);
	openRollers();
	assignArmMotors(50);
	wait1Msec(200);
	assignFlipFlop(127);
	wait1Msec(400);
	assignFlipFlop(20);
	assignArmMotors(0);

	if (extraCones){
		assignArmMotors(-127);
		backwardDistance(80, 565);//555
		assignArmMotors(-15);
		wait1Msec(300);
		turnLeft(127, 90, reverse, false);
		forwardDistance(40, 200, false, true);
		forwardDistance(80, 400, false, false);
		assignArmMotors(50);
		assignMogoMotors(-127);
		wait1Msec(100);
		assignArmMotors(0);
		wait1Msec(200);
		assignArmMotors(0);
		forwardDistance(127, 1300, false, false);
		assignMogoMotors(-10);
		assignArmMotors(50);
		wait1Msec(200);
		assignArmMotors(0);
		forwardDistance(127, 1430, false, false);
		assignMogoMotors(127);
		assignArmMotors(0);
		motor[rollers] = 127;
		assignFlipFlop(-127);
		wait1Msec(1000);

		forwardDistance(127, 1660, true, false); //240
		assignMogoMotors(30);
		//forwardDistance(127, 1650, true, false);
		assignArmMotors(-127);
		//wait1Msec(350);
		wait1Msec(675);
		assignFlipFlop(-10);
		numCones = 1;
		assignArmMotors(-10);
		wait1Msec(10);
		startTask(autoStack);
		wait1Msec(700);

		forwardDistance(127, 1800, true, false);//1810
		while (autoStackingInProgress){
			wait1Msec(20);
		}
		numCones = 2;
		wait1Msec(200);

		endAutoStackEarly = true;
		startTask(autoStack);

		backwardDistance(127, 1400);
		assignArmMotors(-127);
		wait1Msec(200);
		openRollers();
		wait1Msec(200);
		assignArmMotors(60);
		anticipateTurn = EARLYBRAKEDEGREESMOGO;
		turnLeft(127, 120, reverse);
		assignMogoMotors(-127);
		wait1Msec(500);
		assignArmMotors(-70);
		backwardTime(127,1000);
		assignMogoMotors(0);
		assignArmMotors(0);
		assignMogoMotors(0);
	}
	else {
		assignMogoMotors(-127);
		backwardDistance(127, 400);
		assignArmMotors(-100);
		turnLeft(127, 78, reverse);
		assignMogoMotors(-10);
		assignArmMotors(0);
		forwardTime(127, 2100);
		backwardDistance(127, 300);
		turnRight(127, 135, reverse);
		forwardDistance(127, 500);
		forwardDistance(50, 100, false, true);
		assignMogoMotors(127);
		wait1Msec(1100);
		assignMogoMotors(0);
	}
}

/**
void autonomousConeIn20Pt(bool reverse, bool stopAfterTime, bool fivept, bool tenpt, int extraCones)
{
forwardDistance(127, 1200, false, false);
assignArmMotors(50);o


}
*/

void autonomousConeIn20Pt(bool reverse, bool stopAfterTime, bool fivept, bool tenpt, int extraCones)
{
	anticipateTurn = EARLYBRAKEDEGREESMOGO;
	endAutoStackEarly = false;
	clearTimer(T1);
	motor[rollers] = 20; //Drive forward to pick up mobile goal and score the cone on it
	//assignFlipFlop(15);
	assignArmMotors(70);
	assignFlipFlop(-127);
	forwardDistance(127, 160, false, true);
	assignFlipFlop(127);
	assignArmMotors(0);
	assignMogoMotors(-127);
	forwardDistance(127, 1000, false, false);
	assignFlipFlop(10);
	assignMogoMotors(-10);
	//forwardDistance(80, 1340, false, false);
	forwardDistance(80, 1370, false, false);
	assignMogoMotors(127);
	wait1Msec(850);
	assignArmMotors(-127);
	//wait1Msec(200);
	wait1Msec(450);
	assignMogoMotors(20);
	assignArmMotors(0);
	wait1Msec(200);
	openRollers(); //At this point the one cone is scored
	wait1Msec(250);
	if (extraCones > 0){
		assignArmMotors(20);
		motor[rollers] = 127;
		assignFlipFlop(-127);
		//forwardDistance(127, 1600, true, false); //240
		forwardDistance(127, 1670, true, false); //1650
		assignArmMotors(-127);
		//wait1Msec(350);
		wait1Msec(675);
		assignFlipFlop(-20);
		numCones = 1;
		assignArmMotors(-10);
		wait1Msec(10);
		//endAutoStackEarly = true;
		startTask(autoStack);
		wait1Msec(700);
		if (extraCones > 1){
			forwardDistance(127, 1840, true, false);//1810
			while (autoStackingInProgress){
				wait1Msec(20);
			}
			numCones = 2;
			wait1Msec(200);

			startTask(autoStack);
			wait1Msec(600);
			forwardDistance(127, 1940, true, false); //1990 //245
			while (autoStackingInProgress){
				wait1Msec(20);
			}
			numCones = 3;
			endAutoStackEarly = true;
			wait1Msec(500);
			startTask(autoStack);
			wait1Msec(200);
			if (stopAfterTime){
				backwardDistance(127, 600, false);
			}
			else {
				backwardDistance(127, 300, false);
				backward(127,2360);//2300
			}
		}
		else {
			endAutoStackEarly = true;
			backwardDistance(127, 1600);
		}
	}
	else {
		backwardDistance(127, 1200);
	}



	if (stopAfterTime){
		numCones = 4;
		backwardDistance(127, 400);
		stopTask(autoStack);
		endAutoStackEarly = false;
		openRollers();
		assignArmMotors(50);
		turnLeft(127, 45, reverse);
		assignArmMotors(0);
		assignFlipFlop(-127);
		backwardDistance(127, 400);
		wait1Msec(200);
		assignFlipFlop(-10);
		turnRight(127, 45, reverse);
		forwardDistance(127, 100, false, true);
		assignArmMotors(-127);
		forwardDistance(127, 250);
		wait1Msec(200);
		assignArmMotors(0);
		assignFlipFlop(0);
		startTask(autoStack);
		wait1Msec(600);
		forwardDistance(127, 55);
		//turnLeft(127, 45, reverse);
		while (autoStackingInProgress){
			wait1Msec(20);
		}
		numCones = 5;
		startTask(autoStack);
		wait1Msec(600);
		forwardDistance(127, 245);
		while (autoStackingInProgress){
			wait1Msec(20);
		}
		numCones = 6;
		startTask(autoStack);
		wait1Msec(600);
		forwardDistance(127, 245);
		while (autoStackingInProgress){
			wait1Msec(20);
		}
		numCones = 7;
		return;
	}

	if (fivept) //Code to run for 5 pt auton
	{
		assignArmMotors(-50);
		turnLeft(127,125,reverse);
		assignArmMotors(0);
		//backwardDistance(127, 300);
		if (tenpt){
			forwardDistance(127, 800);
		}
		//assignArmMotors(-127);
		//wait1Msec(200);

		//wait1Msec(200);
		assignMogoMotors(-127);
		assignFlipFlop(-127);
		assignArmMotors(-10);
		backwardDistance(127, 300);
		wait1Msec(400);
		assignMogoMotors(0);
		assignFlipFlop(0);
		motor[rollers] = -127;
		assignArmMotors(50);
		forwardDistance(127, 200);
		assignArmMotors(20);
		backward(127,1000);
		motor[rollers] = 0;
		return; //Exit from autonomous since 5 point auton is complete
		} else {
		//turnLeft(127,45,reverse);
	}

	// backwardDistance(127, 670); //655 //640 //Navigate to the 20 point zone
	turnLeft(127,45,reverse);
	backward(127,750);
	turnLeft(127,90, reverse);

	stopTask(maintainArmPos);
	motor[rollers] = -30;
	assignArmMotors(30);
	forwardTime(127, 1000);
	//forwardTime(127, 900);
	assignArmMotors(10);
	assignMogoMotors(-127); //Begin deposit sequence
	forwardTime(127, 550); //50
	assignMogoMotors(0);
	backwardTime(127, 300);
	assignMogoMotors(127);
	assignArmMotors(-50);
	/*forwardTime(127, 300);
	wait1Msec(200);*/
	backwardTime(127,800);
	assignMogoMotors(0);
	displayLCDNumber(1, 1, time1[T1]);
}

void autonDefense(){
	closeRollers();
	backwardTime(127, 4000);
}


void programmingSkills(){
	closeRollers();
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
	openRollers();
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
