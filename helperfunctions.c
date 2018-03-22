#define FLIPFLOPDOWN 3650
#define FLIPFLOPUP 2150
#define BOTTOMARMPOS 1575
#define LOADERARMPOS 2100
#define STATIONARYARMPOS 1250
#define KP_WHEELS_FORWARD 1.1 //0.8 //TODO: experiment with scaling power polynomially (perhaps quadratically) instead of linearly when braking
#define KP_WHEELS_ANGLE 0.06
#define KP_WHEELS_LOCK_ANGLE 1.2
#define KP_ARM 0.05
#define KP_CORRECTDRIVE 0.8
#define EARLYBRAKEDEGREES 80
#define STRAIGHTBRAKEPOWER 87
#define ANGLEBRAKEPOWER 87

float currentKPForward = KP_WHEELS_FORWARD;

int currentDownPos=BOTTOMARMPOS;
bool autoStackingInProgress;
bool endAutoStackEarly = false;
int anticipateTurn = EARLYBRAKEDEGREES;
int angleBrakePower = ANGLEBRAKEPOWER;
int straightBrakePower = STRAIGHTBRAKEPOWER;

int positions[18]={0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

void turnLeft(int power, int degrees, bool reverse, bool clear);
void turnRight(int power, int degrees, bool reverse, bool clear);

int encoderAverage(int one, int two){
	return (abs(one) + abs(two))/2;
}

void closeClaw()
{
	motor[claw] = 127;
	wait1Msec(250);
	motor[claw] = 55;
}

void openClaw()
{
	motor[claw] = -127;
	wait1Msec(250);
	motor[claw] = -20;
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
	else {
		assignDriveMotors(-10, -10);
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
	else {
		assignDriveMotors(-10, -10);
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
	while (SensorValue[potArm] < goalPos + 20){
		if (SensorValue[potArm] > goalPos - 200){
			assignFlipFlop(127);
		}
	}
	
	while (SensorValue[potFlipFlop] > FLIPFLOPUP + 150){
		//wait
	}

    assignArmMotors(-127);
	assignFlipFlop(5);
    while (SensorValue[potArm] > goalPos){

    }
    assignClawMotor(-10);
    assignFlipFlop(-127);

	if (endAutoStackEarly){
        assignArmMotors(0);
		wait1Msec(1000);
        return;
	}

    bool armExit = false;
    bool flipFlopExit = false;
	while (!armExit && !flipFlopExit){
		if (SensorValue[potFlipFlop] >= FLIPFLOPDOWN - 100){
            flipFlopExit = true;
            assignFlipFlop(0);
        }
        if (SensorValue[potArm] < currentDownPos + 100)){
            armExit = true;
            assignArmMotors(0);
        }
	}
    
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