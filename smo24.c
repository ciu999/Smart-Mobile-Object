#include <pthread.h>
#include "pwm.h"
#include "motor.h"
#include "keyInput.h"
#include "ultraSonic.h"

#define MOTOR_COUNT 2
#define INIT_SPEED 70

#define SERVO_PIN 12	// GPIO 18
#define INIT_ANGLE 90
#define GAP 5

#define LEFT_ULTRA_SONIC_ECHO 32        // GPIO 12
#define LEFT_ULTRA_SONIC_TRIGGER 33     // GPIO 13

#define CENTER_ULTRA_SONIC_ECHO 36      // GPIO 16
#define CENTER_ULTRA_SONIC_TRIGGER 35   // GPIO 19

#define RIGHT_ULTRA_SONIC_ECHO 38       // GPIO 20
#define RIGHT_ULTRA_SONIC_TRIGGER 40    // GPIO 21

float ultraSonicDistance[3] = {0.0, };

void *ultraSonicThreadFunction(void *data);

int main() {
    int i, i2c, motor[MOTOR_COUNT], speed = INIT_SPEED, angle = INIT_ANGLE;
    char key;
    int leftUltraSonicCode = 0;
    int centerUltraSonicCode = 1;
    int rightUltraSonicCode = 2;

    if(wiringPiSetupGpio() == -1)
		return -1;
	
	pthread_t ultraSonicThread[3];
    pthread_create(&ultraSonicThread[0], NULL, ultraSonicThreadFunction, (void *)&leftUltraSonicCode);
    pthread_create(&ultraSonicThread[1], NULL, ultraSonicThreadFunction, (void *)&centerUltraSonicCode);
    pthread_create(&ultraSonicThread[2], NULL, ultraSonicThreadFunction, (void *)&rightUltraSonicCode);

	// init servo motor (first)
	initServoMotor(SERVO_PIN);
	
	// init DC motor
    i2c = init();
    for(i=0; i<MOTOR_COUNT; ++i) {
	    motor[i] = i+1;
        initDCMotor(i2c, motor[i], speed);
        setDCSpeed(i2c, motor[i], speed);
    }

    printf("Running~!\nExit is press 'q'\n");
	while(1) {
        key = getch();
        /*
        if (key == 'w') {
            for (i = 0; i < MOTOR_COUNT; ++i) {
                runDCMotor(i2c, motor[i], MOTOR_FORWARD);
                setDCSpeed(i2c, motor[i], speed);
			}
        } else if (key == 'a') {
            angle = angle < 180 ? angle + GAP : angle;
            angleServo(SERVO_PIN, angle);
        }
		else if(key == 'd') {
            angle = angle > 0 ? angle - GAP : angle;
            angleServo(SERVO_PIN, angle);
        }
		else if(key == 's') {
            for(i=0; i<MOTOR_COUNT; ++i) {
                runDCMotor(i2c, motor[i], MOTOR_BACK);
                setDCSpeed(i2c, motor[i], speed);
            }
		}
		else if(key == 'o') {
            speed -= 10;
            for(i=0; i<MOTOR_COUNT; ++i)
                setDCSpeed(i2c, motor[i], speed);
		}
        else if(key == 'p') {
            speed += 10;
            for(i=0; i<MOTOR_COUNT; ++i)
                setDCSpeed(i2c, motor[i], speed);
        }
		else if(key == 'q') {
			speed = 0;
			for(i=0; i<MOTOR_COUNT; ++i)
				runDCMotor(i2c, motor[i], MOTOR_RELEASE);
			break;
		}
        */
        if(key == 'q') {
            speed = 0;
            for(i=0; i<MOTOR_COUNT; ++i)
                runDCMotor(i2c, motor[i], MOTOR_RELEASE);
            break;
        }
	}
	pthread_join(ultraSonicThread[0], NULL);
	pthread_join(ultraSonicThread[1], NULL);
	pthread_join(ultraSonicThread[2], NULL);
	return 0;
}

void *ultraSonicThreadFunction(void *data) {
	int code = *((int*)data), echo, trigger;
    switch(code) {
        case 0 :
            echo = LEFT_ULTRA_SONIC_TRIGGER;
            trigger = LEFT_ULTRA_SONIC_ECHO;
            break;
        case 1 :
            echo = CENTER_ULTRA_SONIC_ECHO;
            trigger = CENTER_ULTRA_SONIC_TRIGGER;
            break;
        case 2 :
            echo = RIGHT_ULTRA_SONIC_ECHO;
            trigger = RIGHT_ULTRA_SONIC_TRIGGER;
            break;
    }
    pinMode(trigger, OUTPUT);
    pinMode(echo, INPUT);
    while(1) {
        ultraSonicDistance[code] = getDistance(trigger, echo);
        printf("code %d distance is %f\n", code, ultraSonicDistance[code]);
		sleep(1);
    }
}
