#define ARMDOWN 1600
#define ARMUP 2850
#define BOTTOMLIFTPOS 0
#define LOADERLIFTPOS 700
#define STATIONARYLIFTPOS 1250
#define KP_WHEELS_FORWARD 1 //TODO: experiment with scaling power polynomially (perhaps quadratically) instead of linearly when braking
#define KP_WHEELS_ANGLE 0.2
#define KP_WHEELS_LOCK_ANGLE 0.5
#define KP_ARM 0.05

int currentDownPos=BOTTOMARMPOS;
bool autoStackingInProgress;

int positions[13]={120, 260, 470, 540, 700, 890, 1010, 1140, 1270, 1400, 1530, 1660, 1760};

void turnLeft(int power, int degrees, bool reverse);
void turnRight(int power, int degrees, bool reverse);

int encoderAverage(int one, int two){
	return (abs(one) + abs(two))/2;
}

void closeClaw()
{
	motor[claw] = 127;
	wait1Msec(500);
	motor[claw] = 25;
}

void openClaw()
{
	motor[claw] = -127;
	wait1Msec(500);
	motor[claw] = -20;
}


void assignLiftMotors(int power){
	motor[liftL] = power;
	motor[liftR] = power;
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
	while (encoderAverage(SensorValue[leftEncoder], SensorValue[rightEncoder]) < distance - 50 && time1[T2] < distance + 2000){
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
	while (encoderAverage(SensorValue[leftEncoder], SensorValue[rightEncoder]) < distance - 50 && time1[T2] < distance + 2000){
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

void assignArmMotors(int power)
{
	motor[armL] = power;
	motor[armR] = power;
}

task maintainLiftPos(){
	int goalPos = SensorValue[potLift];
	int diff;
	while (true){
		diff = goalPos - SensorValue[potLift];
		assignLiftMotors(diff*KP_ARM+5);
		wait1Msec(10);
	}
}

task autoStack(){
	autoStackingInProgress = true;
	closeClaw();
	assignArmMotors(127);
	assignLiftMotors(127);
	wait1Msec(200);
	assignArmMotors(0);
	int goalPos = positions[numCones];
	while(SensorValue[potLift] < goalPos){
		if (SensorValue[potLift] > goalPos - 200){
			assignArmMotors(127);
		}
	}
	assignLiftMotors(10);
	assignArmMotors(127);
	while(SensorValue[potArmMotors] < ARMUP){
		//wait
	}
	assignArmMotors(0);
	assignLiftMotors(-40);
	wait1Msec(300);
	openClaw();
	assignLiftMotors(127);
	wait1Msec(200);
	assignArmMotors(-127);
	wait1Msec(200);
	assignLiftMotors(-127);
	while (SensorValue[potLift] > currentDownPos){
		//wait
	}
	assignLiftMotors(-10);
	if (SensorValue[potArmMotors] >= ARMDOWN){
		while (SensorValue[potArmMotors] >= ARMDOWN){
			//wait
		}
	}
	assignArmMotors(0);
	autoStackingInProgress = false;
}

task autoStack(){
	autoStackingInProgress = true;
	closeClaw();
	assignArmMotors(127);
	assignLiftMotors(127);
	wait1Msec(200);
	assignArmMotors(0);
	int goalPos = positions[numCones];
	while(SensorValue[potLift] < goalPos){
		if (SensorValue[potLift] > goalPos - 200){
			assignArmMotors(127);
		}
	}
	assignLiftMotors(10);
	assignArmMotors(127);
	wait1Msec(300);
	assignArmMotors(0);
	assignLiftMotors(-127);
	wait1Msec(300);
	assignArmMotors(-127);
	if (SensorValue[potArmMotors] >= ARMDOWN){
		while (SensorValue[potArmMotors] >= ARMDOWN){
			//wait
		}
	}
	assignArmMotors(-5);

	while (SensorValue[potLift] > currentDownPos){
		//wait
	}
	assignLiftMotors(-10);
	
	autoStackingInProgress = false;
}

void autoStackCones(){
	startTask(autoStack2);
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
