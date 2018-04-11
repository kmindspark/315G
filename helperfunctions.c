#define FLIPFLOPDOWN 3700 //was 3650
#define FLIPFLOPUP 1920 //was 2150 before
#define BOTTOMARMPOS 1400 //was 1575 before
#define LOADERARMPOS 2060
#define REALLOADERARMPOS 1800
#define STATIONARYARMPOS 1250
#define KP_WHEELS_FORWARD 1.1 //0.8 //TODO: experiment with scaling power polynomially (perhaps quadratically) instead of linearly when braking
#define KP_WHEELS_ANGLE 0.06
#define KP_WHEELS_LOCK_ANGLE 1.2
#define KP_ARM 0.35
#define KP_CORRECTDRIVE 0.8
#define EARLYBRAKEDEGREES 80
#define STRAIGHTBRAKEPOWER 87
#define ANGLEBRAKEPOWER 87

float currentKPForward = KP_WHEELS_FORWARD;

int currentDownPos=BOTTOMARMPOS;
int goalArmHeight = BOTTOMARMPOS;
bool autoStackingInProgress;
bool endAutoStackEarly = false;
bool rollersOpen;
int anticipateTurn = EARLYBRAKEDEGREES;
int angleBrakePower = ANGLEBRAKEPOWER;
int straightBrakePower = STRAIGHTBRAKEPOWER;

int positions[18]={1400, 1490, 1560, 1630, 1700, 1770, 1840, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

void turnLeft(int power, int degrees, bool reverse, bool clear);
void turnRight(int power, int degrees, bool reverse, bool clear);

int encoderAverage(int one, int two){
	return (abs(one) + abs(two))/2;
}

task closeRollersTask(){
	motor[rollers] = 127;
	wait1Msec(250);
	rollersOpen = false;
	motor[rollers] = 10;
}

void closeRollers()
{
	startTask(closeRollersTask);
}

void openRollers()
{
	motor[rollers] = -127;
}

void assignArmMotors(int power){
	motor[armL1] = power;
	motor[armR1] = power;
	motor[armL2] = power;
	motor[armR2] = power;
}

void assignDriveMotors(int lp, int rp){
	motor[dl] = lp;
	motor[dr] = rp;
}

task brakeWheels(){
	int encoderValue;
	int gyroValue;
	int forwardPower;
	int turnPower;
	int kTurn;

	kTurn = KP_WHEELS_LOCK_ANGLE;

	clearTimer(T4);
	while (true){
		encoderValue = (SensorValue[leftEncoder] - SensorValue[rightEncoder])/2; //Signing correct
		forwardPower = (goalDriveValue - encoderValue)*currentKPForward - autonForwardBrake*10;//15

		gyroValue = SensorValue[gyro];

		turnPower = (goalDriveAngle - gyroValue)/**(2000-time1[T4])/2000*/ * kTurn - autonAngleBrake*10;
		assignDriveMotors(forwardPower + turnPower, forwardPower - turnPower);
	}
}

void forwardDistance(int power, int distance, bool brake, bool clear){
	clearTimer(T2);
	if (clear){
		SensorValue[leftEncoder] = 0;
		SensorValue[rightEncoder] = 0;
	}
	assignDriveMotors(power, power);
	int difference;
	while (encoderAverage(SensorValue[leftEncoder], SensorValue[rightEncoder]) < distance - 50 && time1[T2] < distance + 2000){
		difference = KP_CORRECTDRIVE*(abs(SensorValue[leftEncoder]) - abs(SensorValue[rightEncoder]));
		assignDriveMotors(power - difference, power + difference);
	}
	if (brake){
		assignDriveMotors(-straightBrakePower, -straightBrakePower);
		wait1Msec(100);
	}
	assignDriveMotors(0, 0);
}

void backwardDistance(int power, int distance, bool brake, bool clear){
	clearTimer(T2);
	if (clear){
		SensorValue[leftEncoder] = 0;
		SensorValue[rightEncoder] = 0;
	}
	assignDriveMotors(-power, -power);
	int difference;
	while (encoderAverage(SensorValue[leftEncoder], SensorValue[rightEncoder]) < distance - 50 && time1[T2] < distance + 2000){
		difference = KP_CORRECTDRIVE*(abs(SensorValue[leftEncoder]) - abs(SensorValue[rightEncoder]));
		assignDriveMotors(-power + difference, -power + difference);
	}
	if (brake){
		assignDriveMotors(straightBrakePower, straightBrakePower);
		wait1Msec(100);
	}
	assignDriveMotors(0, 0);
}

void forwardDistance(int power, int distance, bool brake){
	forwardDistance(power, distance, brake, true);
}

void backwardDistance(int power, int distance, bool brake){
	backwardDistance(power, distance, brake, true);
}

void forwardDistance(int power, int distance){
	forwardDistance(power, distance, true, true);
}

void backwardDistance(int power, int distance){
	backwardDistance(power, distance, true, true);
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

void turnRight(int power, int degrees, bool reverse, bool clear)
{
	if (reverse)
	{
		turnLeft(power,degrees,false, clear);
		return;
	}
	SensorValue[leftEncoder] = 0;
	SensorValue[rightEncoder] = 0;
	if (clear){
		SensorValue[gyro] = 0;
	}
	//wait1Msec(50);
	assignDriveMotors(power,-power);
	degrees = degrees*10;

	while (abs(SensorValue[gyro]) < degrees - anticipateTurn){}
	assignDriveMotors(-angleBrakePower, angleBrakePower);
	wait1Msec(150);
	assignDriveMotors(0, 0);
}

void turnLeft(int power, int degrees, bool reverse, bool clear)
{
	if (reverse)
	{
		turnRight(power,degrees,false, clear);
		return;
	}
	SensorValue[leftEncoder] = 0;
	SensorValue[rightEncoder] = 0;
	if (clear){
		SensorValue[gyro] = 0;
	}
	//wait1Msec(50);
	assignDriveMotors(-power,power);
	degrees = degrees*10;

	while (abs(SensorValue[gyro]) < degrees - anticipateTurn){}
	assignDriveMotors(angleBrakePower, -angleBrakePower);
	wait1Msec(150);
	assignDriveMotors(0, 0);
}

void turnLeft(int power, int degrees, bool reverse){
	turnLeft(power, degrees, reverse, true);
}

void turnRight(int power, int degrees, bool reverse){
	turnRight(power, degrees, reverse, true);
}

void assignFlipFlop(int power)
{
	motor[flipflop] = power;
}

task maintainArmPos{
	int diff;
	while (true){
		diff = goalArmHeight - SensorValue[potArm];
		int power = diff*KP_ARM;
		if (power < -30){
			power = -30;
		}
		if (power > 30){
			power = 30;
		}
		assignArmMotors(power);
		wait1Msec(10);
	}
}

task monitorLoaderArm(){
	while (true){
		if (SensorValue[potArm] < LOADERARMPOS){
			goalArmHeight = REALLOADERARMPOS;
			startTask(maintainArmPos);
			wait1Msec(100);
			closeRollers();
			assignFlipFlop(0);
			stopTask(maintainArmPos);
			break;
		}
		else if (SensorValue[potArm] < LOADERARMPOS + 200){
			assignArmMotors(40);
			assignFlipFlop(-127);
		}

		/**if (SensorValue[potArm] < LOADERARMPOS - 200){
		//assignArmMotors(30);
		//wait1Msec(120);
		//assignArmMotors(10);
		//closeRollers();
		//startTask(maintainArmPos);
		closeRollers();
		rollersOpen = false;
		assignFlipFlop(0);
		stopTask(maintainArmPos);
		break;
		}*/
	}
}

task monitorDownArm(){
	while (true){
		if (SensorValue[potArm] < BOTTOMARMPOS + 300){
			assignArmMotors(0);
		}
		if (SensorValue[potArm] < BOTTOMARMPOS + 150){
			assignArmMotors(-50);
			wait1Msec(300);
			assignArmMotors(-15);
			break;
		}
	}
}


task autoStack(){
	autoStackingInProgress = true;
	closeRollers();
	assignArmMotors(127);
	int goalPos = positions[numCones];
	while(SensorValue[potArm] < goalPos + 20){
		if (SensorValue[potArm] > goalPos - 200){
			assignFlipFlop(127);
		}
	}
	startTask(maintainArmPos);
	while(SensorValue[potFlipFlop] > FLIPFLOPUP + 50){
		//wait
	}
	assignFlipFlop(10);

	if (endAutoStackEarly){
		wait1Msec(1000);
	}
	assignArmMotors(-127);
	wait1Msec(300);
	openRollers();

	if (endAutoStackEarly){
		return;
	}

	stopTask(maintainArmPos);
	goalPos = SensorValue[potArm];
	assignArmMotors(127);
	clearTimer(T4);
	while(SensorValue[potArm] < goalPos + 155){
		if (SensorValue[potArm] > goalPos + 50){
			assignFlipFlop(-127);
		}
	}

	motor[rollers] = 127;
	assignArmMotors(-127);
	assignFlipFlop(-127);
	while (SensorValue[potFlipFlop] <= FLIPFLOPDOWN - 500){
		//wait
	}
	assignFlipFlop(0);

	while (SensorValue[potArm] > currentDownPos + 50){
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
