#include "pwm.h"

int init() {
    //Setup I2C
    int i2c = wiringPiI2CSetup(ADAFRUIT_MOTORHAT);

    //Setup PWM
    setAllPWM(i2c, 0, 0);
    wiringPiI2CWriteReg8(i2c, PWM_MODE2, PWM_OUTDRV);
    wiringPiI2CWriteReg8(i2c, PWM_MODE1, PWM_ALLCALL);
    delay(5);
    int mode1 = wiringPiI2CReadReg8(i2c, PWM_MODE1) & ~PWM_SLEEP;
    wiringPiI2CWriteReg8(i2c, PWM_MODE1, mode1);
    delay(5);

    //Set PWM frequency
    int prescale = (int) (25000000.0 / 4096.0 /  PWM_FREQUENCY - 1.0);
    int oldmode = wiringPiI2CReadReg8(i2c, PWM_MODE1);
    int newmode = oldmode & 0x7F | 0x10;
    wiringPiI2CWriteReg8(i2c, PWM_MODE1, newmode);
    wiringPiI2CWriteReg8(i2c, PWM_PRESCALE, prescale);
    wiringPiI2CWriteReg8(i2c, PWM_MODE1, oldmode);
    delay(5);
    wiringPiI2CWriteReg8(i2c, PWM_MODE1, oldmode | 0x80);

    return i2c;
}

void setAllPWM(int i2c, int on, int off) {
    wiringPiI2CWriteReg8(i2c, PWM_ALL_LED_ON_L, on & 0xFF);
    wiringPiI2CWriteReg8(i2c, PWM_ALL_LED_ON_H, on >> 8);
    wiringPiI2CWriteReg8(i2c, PWM_ALL_LED_OFF_L, off & 0xFF);
    wiringPiI2CWriteReg8(i2c, PWM_ALL_LED_OFF_H, off >> 8);
}

void setPWM(int i2c, int pin, int on, int off) {
    wiringPiI2CWriteReg8(i2c, PWM_LED0_ON_L + 4 * pin, on & 0xFF);
    wiringPiI2CWriteReg8(i2c, PWM_LED0_ON_H + 4 * pin, on >> 8);
    wiringPiI2CWriteReg8(i2c, PWM_LED0_OFF_L + 4 * pin, off & 0xFF);
    wiringPiI2CWriteReg8(i2c, PWM_LED0_OFF_H + 4 * pin, off >> 8);
}

void setPin(int i2c, int pin, int value) {
    if (pin < 0 || pin > 15)
        return;

    switch (value) {
        case 0:
            setPWM(i2c, pin, 0, 4096);
            break;
        case 1:
            setPWM(i2c, pin, 4096, 0);
            break;
    }
}