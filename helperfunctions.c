#define FLIPFLOPDOWN 1370
#define FLIPFLOPUP 3750

int positions[14]={100, 200, 400, 600, 800, 1000, 1200, 1400, 1600, 1800, 2000, 2200, 2400, 2600};

void turnLeft(int power, int time, bool reverse);
void turnRight(int power, int time, bool reverse);

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

void turnRight(int power, int time, bool reverse)
{
	if (reverse)
	{
		turnLeft(power,time,false);
		return;
	}
	assignDriveMotors(power,-power);
	wait1Msec(time);
	assignDriveMotors(-10,10);
	wait1Msec(100);
	assignDriveMotors(0,0);
}

void turnLeft(int power, int time, bool reverse)
{
	if (reverse)
	{
		turnRight(power,time,false);
		return;
	}
	assignDriveMotors(-power,power);
	wait1Msec(time);
	assignDriveMotors(10,-10);
	wait1Msec(100);
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
	assignArmMotors(127);
	int goalPos = positions[numCones-1];
	while(SensorValue[potArm] < goalPos){
		if (SensorValue[potArm] < positions[0] + 400  || SensorValue[potArm] > goalPos - 400){
			assignFlipFlop(127);
		}
		else if (SensorValue[potArm] < goalPos){
			assignFlipFlop(0);
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
	wait1Msec(400);
	assignArmMotors(-127);
	while (SensorValue[potArm] > positions[0]){
		if (SensorValue[potFlipFlop] < FLIPFLOPDOWN){
			assignFlipFlop(10);
		}
	}
	if (SensorValue[potFlipFlop] >= FLIPFLOPDOWN){
		assignFlipFlop(-127);
		while (SensorValue[potFlipFlop] >= FLIPFLOPDOWN){
			//wait
		}
		assignFlipFlop(10);
	}
}

void assignMogoMotors(int power){
	motor[mogoL] = power;
	motor[mogoR] = -power;
}
