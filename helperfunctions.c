#define FLIPFLOPDOWN 1600
#define FLIPFLOPUP 2850
#define BOTTOMARMPOS 0
#define LOADERARMPOS 700
#define STATIONARYARMPOS 1250
#define KP_WHEELS 0.2 //TODO: experiment with scaling power polynomially (perhaps quadratically) instead of linearly when braking

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

void forwardDistance(int power, int distance){
	clearTimer(t2);
	SensorValue[leftEncoder] = 0;
	SensorValue[rightEncoder] = 0;
	assignDriveMotors(power, power);
	while (encoderAverage(SensorValue[leftEncoder], SensorValue[rightEncoder]) < distance - 100 && time1[t2] < distance + 2000){
		//keep going
	}
	goalDriveValue = distance + 30;
	startTask(brakeWheels);
	while (encoderAverage(SensorValue[leftEncoder], SensorValue[rightEncoder]) < distance && time1[t2] < distance + 2000){
		//keep going
	}
	stopTask(brakeWheels)
	assignDriveMotors(-40, -40);
	wait1Msec(100);
	assignDriveMotors(0, 0);
}

void backwardDistance(int power, int distance){
	clearTimer(t2);
	SensorValue[leftEncoder] = 0;
	SensorValue[rightEncoder] = 0;
	assignDriveMotors(-power, -power);
	while (encoderAverage(SensorValue[leftEncoder], SensorValue[rightEncoder]) < distance - 100 && time1[t2] < distance + 2000){
		//keep going
	}
	goalDriveValue = -(distance + 30);
	startTask(brakeWheels);
	while (encoderAverage(SensorValue[leftEncoder], SensorValue[rightEncoder]) < distance && time1[t2] < distance + 2000){
		//keep going
	}
	stopTask(brakeWheels)
	assignDriveMotors(40, 40);
	wait1Msec(100);
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
	degrees = degrees * 10;
	while (abs(SensorValue[gyro]) < degrees){
		//do nothing
	}
	assignDriveMotors(-40,40);
	wait1Msec(180);
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
	while (abs(SensorValue[gyro]) < degrees){
		//do nothing
	}
	assignDriveMotors(40,-40);
	wait1Msec(180);
	assignDriveMotors(0,0);
}

void assignFlipFlop(int power)
{
	motor[flipflop] = power;
}

void autoStackCones(){
	startTask(autoStack);
	while (time1[t3] < 4000 && autoStackingInProgress){

	}
	stopTask(autoStack);
}

task autoStack(){
	autoStackingInProgress = true;
	closeClaw();
	assignFlipFlop(127);
	wait1Msec(200);
	assignFlipFlop(0);
	assignArmMotors(127);
	int goalPos = positions[numCones-1];
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
	assignArmMotors(-127);
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
		assignFlipFlop(-127);
		while (SensorValue[potFlipFlop] >= FLIPFLOPDOWN){
			//wait
		}
	}

	autoStackingInProgress = false;
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

task brakeWheels(){
	int currentValue;
	int motorPower;
	while (true){
		currentValue = (-SensorValue[leftEncoder] + SensorValue[rightEncoder])/2; //TODO: check if signing is correct
		motorPower = (goalValue - currentValue)*KP_WHEELS;
		assignDriveMotors(motorPower, motorPower);
	}
}
