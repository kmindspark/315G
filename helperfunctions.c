#define FLIPFLOPDOWN 1900
#define FLIPFLOPUP 3750
#define BOTTOMARMPOS 200
#define LOADERARMPOS 700

int currentDownPos=BOTTOMARMPOS;

int positions[14]={100, 200, 400, 600, 800, 1000, 1200, 1400, 1600, 1800, 2000, 2200, 2400, 2600};

void turnLeft(int power, int degrees, bool reverse);
void turnRight(int power, int degrees, bool reverse);

int encoderAverage(int one, int two){
	return (abs(one) + abs(two))/2;
}

void closeClaw()
{
	motor[claw] = 127;
	wait1Msec(500);
	motor[claw] = 30;
}

void openClaw()
{
	motor[claw] = -127;
	wait1Msec(500);
	motor[claw] = 0;
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
	assignDriveMotors(power, power);
	while (encoderAverage(SensorValue[leftEncoder], SensorValue[rightEncoder]) < distance){
		//keep going
	}
	assignDriveMotors(-10, -10);
	wait1Msec(100);
	assignDriveMotors(0, 0);
}

void backwardDistance(int power, int distance){
	assignDriveMotors(-power, -power);
	while (encoderAverage(SensorValue[leftEncoder], SensorValue[rightEncoder]) < distance){
		//keep going
	}
	assignDriveMotors(10, 10);
	wait1Msec(100);
	assignDriveMotors(0, 0);
}

void forward(int power, int time){
	assignDriveMotors(power, power);
	wait1Msec(time);
	assignDriveMotors(-10, -10);
	wait1Msec(100);
	assignDriveMotors(0, 0);
}

void backward(int power, int time)
{
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
	assignDriveMotors(-35,35);
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
	assignDriveMotors(35,-35);
	wait1Msec(180);
	assignDriveMotors(0,0);
}

void assignFlipFlop(int power)
{
	motor[flipflop] = power;
}

void autoStack(int numCones){
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
	assignFlipFlop(127);
	while(SensorValue[potFlipFlop] < FLIPFLOPUP){
		//wait
	}
	assignFlipFlop(0);
	assignArmMotors(-40);
	wait1Msec(200);
	openClaw();
	assignArmMotors(127);
	assignFlipFlop(-127);
	wait1Msec(200);
	assignArmMotors(-127);
	while (SensorValue[potArm] > currentDownPos){
		//wait
	}
	if (SensorValue[potFlipFlop] >= FLIPFLOPDOWN){
		assignFlipFlop(-127);
		while (SensorValue[potFlipFlop] >= FLIPFLOPDOWN){
			//wait
		}
	}
	assignFlipFlop(-5);
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
