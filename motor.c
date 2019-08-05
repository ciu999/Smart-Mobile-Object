//
// Created by SNSLAB on 2019-05-09.
//

#include <stdio.h>
#include "motor.h"

int initDCMotor(int i2c, int motor, int speed) {
    //if(wiringPiSetup() == -1)
    //    return 0;
    runDCMotor(i2c, motor, MOTOR_RELEASE);
    setDCSpeed(i2c, motor, speed);
    runDCMotor(i2c, motor, MOTOR_FORWARD);
    runDCMotor(i2c, motor, MOTOR_RELEASE);
    return 1;
}

void runDCMotor(int i2c, int motor, int command) {
    int in1, in2;

    switch (motor) {
        case 1:
            in1 = PWM_M1_IN1;
            in2 = PWM_M1_IN2;
            break;
        case 2:
            in1 = PWM_M2_IN1;
            in2 = PWM_M2_IN2;
            break;
        case 3:
            in1 = PWM_M3_IN1;
            in2 = PWM_M3_IN2;
            break;
        case 4:
            in1 = PWM_M4_IN1;
            in2 = PWM_M4_IN2;
            break;
    }

    switch (command) {
        case MOTOR_FORWARD:
            setPin(i2c, in2, 0);
            setPin(i2c, in1, 1);
            break;
        case MOTOR_BACK:
            setPin(i2c, in1, 0);
            setPin(i2c, in2, 1);
            break;
        case MOTOR_RELEASE:
            setPin(i2c, in1, 0);
            setPin(i2c, in2, 0);
            break;
    }
}

void setDCSpeed(int i2c, int motor, int speed) {
    if (speed < 0 || speed > 255)
        return;

    int pwm;
    switch (motor) {
        case 1:
            pwm = PWM_M1_PWM;
            break;
        case 2:
            pwm = PWM_M2_PWM;
            break;
        case 3:
            pwm = PWM_M3_PWM;
            break;
        case 4:
            pwm = PWM_M4_PWM;
            break;
    }
    setPWM(i2c, pwm, 0, speed * 16);
}

int initServoMotor(int servoPin) {
    if(wiringPiSetupPhys() == -1)
        return 0;
    pinMode(servoPin, PWM_OUTPUT);

    pwmSetMode(PWM_MODE_MS);

    pwmSetRange(SERVO_PWM_RANGE);
    pwmSetClock(SERVO_PWM_CLOCK);
    return 1;
}

void angleServo(int servoPin, int angle) {
    int value;
    if (angle < 45 || angle > 135)
        return;
    value = (float)5 / 9 * angle + 25;
    pwmWrite(servoPin, value);
}
