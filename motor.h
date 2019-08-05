//
// Created by SNSLAB on 2019-05-08.
//

#ifndef MOTOR_H
#define MOTOR_H

#include "pwm.h"

#define MOTOR_FORWARD   1
#define MOTOR_BACK      2
#define MOTOR_BRAKE     3
#define MOTOR_RELEASE   4

// degree 0 = 25, degree 90 = 75, degree 180 = 125
#define MID_DEGREE 75
#define LEFT_DEGREE 50
#define RIGHT_DEGREE 100

int initDCMotor(int i2c, int motor, int speed);
void runDCMotor(int i2c, int motor, int command);
void setDCSpeed(int i2c, int motor, int speed);
int initServoMotor(int servoPin);
void angleServo(int servoPin, int angle);

#endif // MOTOR_H
