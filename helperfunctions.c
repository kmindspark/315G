#define FLIPFLOPDOWN 2600
#define FLIPFLOPUP 1230
#define BOTTOMARMPOS 500
#define LOADERARMPOS 1800
#define STATIONARYARMPOS 2650

int currentDownPos=BOTTOMARMPOS;
bool clawOpen = false;

int positions[13]={120, 260, 470, 540, 700, 890, 1010, 1140, 1270, 1400, 1530, 1660, 1760};

void turnLeft(int power, int degrees, bool reverse);
void turnRight(int power, int degrees, bool reverse);

int encoderAverage(int one, int two){
	return (abs(one) + abs(two))/2;
}

void closeClaw()
{
	clawOpen = false;
	motor[claw] = 127;
	wait1Msec(500);
	motor[claw] = 25;
}

void openClaw()
{
	clawOpen = true;
	motor[claw] = -127;
	wait1Msec(300);
	motor[claw] = -20;
}


void assignArmMotors(int power){
	motor[armL] = power;
	motor[armR] = power;
}

void assignDriveMotors(int lp, int rp){
	motor[df] = lp;
	motor[db] = lp;
	motor[pf] = rp;
	motor[pb] = rp;
}

task brakeWheels(){
	int encoderValue;
	int gyroValue;
	int forwardPower;
	int turnPower;
	int kTurn;
	while (true){
		encoderValue = (-SensorValue[leftEncoder] + SensorValue[rightEncoder])/2; //Signing correct
		forwardPower = (goalDriveValue - encoderValue)*KP_WHEELS_FORWARD - autonForwardBrake*15;

		gyroValue = SensorValue[gyro];

		if (brake){
			kTurn = KP_WHEELS_LOCK_ANGLE;
		} else {
			kTurn = KP_WHEELS_ANGLE;
		}
		turnPower = (goalDriveAngle - gyroValue)*KP_WHEELS_ANGLE - autonAngleBrake*5;
		assignDriveMotors(forwardPower + turnPower, forwardPower - turnPower);
	}
}

void forwardDistance(int power, int distance){
	clearTimer(T2);
	SensorValue[leftEncoder] = 0;
	SensorValue[rightEncoder] = 0;
	assignDriveMotors(power, power);
	int difference;
	while (encoderAverage(SensorValue[leftEncoder], SensorValue[rightEncoder]) < distance - 50 && time1[T2] < distance + 2000){
		difference = KP_CORRECTDRIVE*(abs(SensorValue[leftEncoder]) - abs(SensorValue[rightEncoder]));
		assignDriveMotors(power - difference, power + difference);
		//keep going
	}
	goalDriveValue = distance;
	autonForwardBrake = 0;//1 - skills;
	startTask(brakeWheels);
	wait1Msec(500);
	stopTask(brakeWheels);
	assignDriveMotors(0, 0);
}

void backwardDistance(int power, int distance){
	clearTimer(T2);
	SensorValue[leftEncoder] = 0;
	SensorValue[rightEncoder] = 0;
	assignDriveMotors(-power, -power);
	int difference;
	while (encoderAverage(SensorValue[leftEncoder], SensorValue[rightEncoder]) < distance - 50 && time1[T2] < distance + 2000){
		difference = KP_CORRECTDRIVE*(abs(SensorValue[leftEncoder]) - abs(SensorValue[rightEncoder]));
		assignDriveMotors(-power + difference, -power + difference);
		//keep going
	}
	goalDriveValue = -distance;
	autonForwardBrake = 0;//-1 + skills;
	startTask(brakeWheels);
	wait1Msec(500);
	stopTask(brakeWheels);
	assignDriveMotors(0, 0);
}

void forwardTime(int power, int time){
	assignDriveMotors(power, power);
	wait1Msec(time);
	assignDriveMotors(-10, -10);
	wait1Msec(100);
	assignDriveMotors(0, 0);
}

void backwardTime(int power, int time){
	assignDriveMotors(-power, -power);
	wait1Msec(time);
	assignDriveMotors(10, 10);
	wait1Msec(100);
	assignDriveMotors(0, 0);
}

void forward(int power, int time){
	forwardDistance(power, time*0.65);
	return;
	assignDriveMotors(power, power);
	wait1Msec(time);
	assignDriveMotors(-10, -10);
	wait1Msec(100);
	assignDriveMotors(0, 0);
}

void backward(int power, int time)
{
	backwardDistance(power, time*0.65);
	return;
	assignDriveMotors(-power,-power);
	wait1Msec(time);
	assignDriveMotors(10,10);
	wait1Msec(100);
	assignDriveMotors(0,0);
}

void turnRight(int power, int degrees, bool reverse)
{
	if (reverse)
	{
		turnLeft(power,degrees,false);
		return;
	}
	SensorValue[gyro] = 0;
	assignDriveMotors(power,-power);
	degrees = degrees*10;
	while (abs(SensorValue[gyro]) < degrees - 500){

	}
	autonAngleBrake = 0;//1 - skills;
	goalDriveAngle = degrees;
	startTask(brakeWheels);
	wait1Msec(1000);
	stopTask(brakeWheels);
	assignDriveMotors(0,0);
}

void turnLeft(int power, int degrees, bool reverse)
{
	if (reverse)
	{
		turnRight(power,degrees,false);
		return;
	}
	SensorValue[gyro] = 0;
	assignDriveMotors(-power,power);
	degrees = degrees*10;
	while (abs(SensorValue[gyro]) < degrees - 500){

	}
	autonAngleBrake = 0;//-1+skills;
	goalDriveAngle = -degrees;
	startTask(brakeWheels);
	wait1Msec(1000);
	stopTask(brakeWheels);
	assignDriveMotors(0,0);
}

void assignFlipFlop(int power)
{
	motor[flipflop] = power;
}

task maintainArmPos(){
	int goalPos = SensorValue[potArm];
	int diff;
	while (true){
		diff = goalPos - SensorValue[potArm];
		assignArmMotors(diff*KP_ARM+5);
		wait1Msec(10);
	}
}

task autoStack(){
	autoStackingInProgress = true;
	closeClaw();
	assignFlipFlop(127);
	assignArmMotors(127);
	wait1Msec(200);
	assignFlipFlop(0);
	int goalPos = positions[numCones];
	while(SensorValue[potArm] < goalPos){
		if (SensorValue[potArm] > goalPos - 200){
			assignFlipFlop(127);
		}
	}
	assignArmMotors(10);
	assignFlipFlop(127);
	while(SensorValue[potFlipFlop] < FLIPFLOPUP){
		//wait
	}
	assignFlipFlop(0);
	assignArmMotors(-40);
	wait1Msec(300);
	openClaw();
	assignArmMotors(127);
	wait1Msec(200);
	assignFlipFlop(-127);
	wait1Msec(200);
	assignArmMotors(-127);
	while (SensorValue[potArm] > currentDownPos){
		//wait
	}
	assignArmMotors(-10);
	if (SensorValue[potFlipFlop] >= FLIPFLOPDOWN){
		while (SensorValue[potFlipFlop] >= FLIPFLOPDOWN){
			//wait
		}
	}
	assignFlipFlop(0);
	autoStackingInProgress = false;
}

void autoStackCones(){
	startTask(autoStack);
	clearTimer(T3);
	while (time1[T3] < 4000 && autoStackingInProgress){

	}
	stopTask(autoStack);
}

void assignMogoMotors(int power){
	motor[mogoL] = power;
	motor[mogoR] = -power;
}

void waitForPress(){
	while (nLCDButtons == 0 && vexCompetitionState == competitionState){
		wait1Msec(5);
	}
}

void waitForRelease(){
	while (nLCDButtons != 0){
		wait1Msec(5);
	}
}