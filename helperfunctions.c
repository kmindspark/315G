#define FLIPFLOPDOWN 3650
#define FLIPFLOPUP 2150
#define BOTTOMARMPOS 1575
#define LOADERARMPOS 2100
#define STATIONARYARMPOS 1250
#define KP_WHEELS_FORWARD 0.8 //TODO: experiment with scaling power polynomially (perhaps quadratically) instead of linearly when braking
#define KP_WHEELS_ANGLE 0.06
#define KP_WHEELS_LOCK_ANGLE 1.5
#define KP_ARM 0.05
#define KP_CORRECTDRIVE 0.8

float currentKPForward = KP_WHEELS_FORWARD;

int currentDownPos=BOTTOMARMPOS;
bool autoStackingInProgress;
bool endAutoStackEarly = false;

int positions[13]={1620, 1800, 1830, 1920, 2200, 2390, 2510, 2640, 2770, 2900, 3030, 3160, 3260};

void turnLeft(int power, int degrees, bool reverse);
void turnRight(int power, int degrees, bool reverse);

int encoderAverage(int one, int two){
	return (abs(one) + abs(two))/2;
}

void closeClaw()
{
	motor[claw] = 127;
	wait1Msec(250);
	motor[claw] = 45;
}

void openClaw()
{
	motor[claw] = -127;
	wait1Msec(250);
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
	clearTimer(T4);
	while (true){
		encoderValue = (SensorValue[leftEncoder] - SensorValue[rightEncoder])/2; //Signing correct
		if (skills == 1){
			encoderValue = (-SensorValue[leftEncoder] + SensorValue[rightEncoder])/2; //Signing correct
		}
		forwardPower = (goalDriveValue - encoderValue)*currentKPForward - autonForwardBrake*10;//15

		gyroValue = SensorValue[gyro];
		if (brake){
			kTurn = KP_WHEELS_LOCK_ANGLE;
			} else {
			kTurn = KP_WHEELS_ANGLE;
		}
		turnPower = (goalDriveAngle - gyroValue)/**(2000-time1[T4])/2000*/ * KP_WHEELS_ANGLE - autonAngleBrake*10;
		assignDriveMotors(forwardPower + turnPower, forwardPower - turnPower);
	}
}



void forwardDistance(int power, int distance, bool brake){
	clearTimer(T2);
	SensorValue[leftEncoder] = 0;
	SensorValue[rightEncoder] = 0;
	assignDriveMotors(power, power);
	int difference;
	while (encoderAverage(SensorValue[leftEncoder], SensorValue[rightEncoder]) < distance - 50 && time1[T2] < distance + 2000){
		difference = KP_CORRECTDRIVE*(abs(SensorValue[leftEncoder]) - abs(SensorValue[rightEncoder]));
		assignDriveMotors(power - difference, power + difference);
	}
	if (brake){
		if (skills == 1){
			goalDriveValue = distance;
			autonForwardBrake = 1;
			startTask(brakeWheels);
			wait1Msec(150);
			autonForwardBrake = 0;
			wait1Msec(400);
			stopTask(brakeWheels);
		}
		else{
			assignDriveMotors(-50, -50);
			wait1Msec(100);
		}
	}
	else {
		assignDriveMotors(-10, -10);
		wait1Msec(100);
	}
	assignDriveMotors(0, 0);
}

void backwardDistance(int power, int distance, bool brake){
	clearTimer(T2);
	SensorValue[leftEncoder] = 0;
	SensorValue[rightEncoder] = 0;
	assignDriveMotors(-power, -power);
	int difference;
	while (encoderAverage(SensorValue[leftEncoder], SensorValue[rightEncoder]) < distance - 50 && time1[T2] < distance + 2000){
		difference = KP_CORRECTDRIVE*(abs(SensorValue[leftEncoder]) - abs(SensorValue[rightEncoder]));
		assignDriveMotors(-power + difference, -power + difference);
	}
	if (brake){
		if (skills == 1){
			goalDriveValue = distance;
			autonForwardBrake = -1;
			startTask(brakeWheels);
			wait1Msec(150);
			autonForwardBrake = 0;
			wait1Msec(400);
			stopTask(brakeWheels);
		}
		else{
			assignDriveMotors(50, 50);
			wait1Msec(100);
		}
	}
	else {
		assignDriveMotors(-10, -10);
		wait1Msec(100);
	}
	assignDriveMotors(0, 0);
}

void forwardDistance(int power, int distance){
	forwardDistance(power, distance, true);
}

void backwardDistance(int power, int distance){
	backwardDistance(power, distance, true);
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
	forwardDistance(power, time*0.65, false);
}

void backward(int power, int time)
{
	backwardDistance(power, time*0.65, false);
	return;
}

void turnRight(int power, int degrees, bool reverse)
{
	if (reverse)
	{
		turnLeft(power,degrees,false);
		return;
	}
	SensorValue[leftEncoder] = 0;
	SensorValue[rightEncoder] = 0;
	SensorValue[gyro] = 0;
	wait1Msec(50);
	assignDriveMotors(power,-power);
	degrees = degrees*10;
	int compensation = 250;
	if (degrees <= 900){
		compensation = degrees*5/18;
	}
	if (skills == 1){
		while (abs(SensorValue[gyro]) < degrees - compensation /*- 250*/){

		}
		currentKPForward = 0;
		goalDriveAngle = degrees;
		goalDriveValue = 0;
		startTask(brakeWheels);
		autonAngleBrake = 1;
		wait1Msec(250);
		autonAngleBrake = 0;
		wait1Msec(75);
		stopTask(brakeWheels);
		currentKPForward = KP_WHEELS_FORWARD;
	}
	else {
		while (abs(SensorValue[gyro]) < degrees - 10){}
		assignDriveMotors(-85, 85);
		wait1Msec(150);
	}
}

void turnLeft(int power, int degrees, bool reverse)
{
	if (reverse)
	{
		turnRight(power,degrees,false);
		return;
	}
	SensorValue[leftEncoder] = 0;
	SensorValue[rightEncoder] = 0;
	SensorValue[gyro] = 0;
	wait1Msec(50);
	assignDriveMotors(-power,power);
	degrees = degrees*10;
	int compensation = 300;
	if (degrees <= 900){
		compensation = degrees/3;
	}
	if (skills == 1){
		while (abs(SensorValue[gyro]) < degrees - compensation /*- 250*/){

		}
		currentKPForward = 0;
		goalDriveAngle = degrees;
		goalDriveValue = 0;
		startTask(brakeWheels);
		autonAngleBrake = -1;
		wait1Msec(250);
		autonAngleBrake = 0;
		wait1Msec(75);
		stopTask(brakeWheels);
		currentKPForward = KP_WHEELS_FORWARD;
	}
	else {
		while (abs(SensorValue[gyro]) < degrees){}
		assignDriveMotors(85, -85);
		wait1Msec(150);
	}
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
	assignArmMotors(127);
	int goalPos = positions[numCones];
	while(SensorValue[potArm] < goalPos + 50){
		if (SensorValue[potArm] > goalPos - 200){
			assignFlipFlop(127);
		}
	}
	assignArmMotors(-30);
	while(SensorValue[potFlipFlop] > FLIPFLOPUP + 50){
		//wait
	}
	assignFlipFlop(10);

	if (endAutoStackEarly){
		wait1Msec(1000);
	}
	assignArmMotors(-100);
	wait1Msec(200);
	assignArmMotors(0);
	//wait1Msec(100);
	openClaw();

	if (endAutoStackEarly){
		return;
	}

	goalPos = SensorValue[potArm];
	assignArmMotors(80);
	clearTimer(T4);
	while(SensorValue[potArm] < goalPos + 140){
		if (time1[T4] > 150){
			assignFlipFlop(-127);
		}
	}
	assignArmMotors(-127);
	assignFlipFlop(-127);
	while (SensorValue[potFlipFlop] <= FLIPFLOPDOWN - 500){
		//wait
	}
	assignFlipFlop(-80);

	while (SensorValue[potArm] > currentDownPos){
		//wait
	}
	assignArmMotors(-10);

	assignFlipFlop(-20);
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
