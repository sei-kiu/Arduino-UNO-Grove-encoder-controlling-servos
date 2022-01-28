#include <Arduino.h>

#include <Servo.h>
#include "rgb_lcd.h"

#define ENCODER1_SIGA 2
#define ENCODER1_SIGB 3
#define ENCODER2_SIGA 4
#define ENCODER2_SIGB 5

Servo myServo1;
Servo myServo2;
rgb_lcd lcd;

volatile int encoder1Value = 0;
volatile int encoder1Flag = 0;

int encoder2sigANew = 0;
int encoder2sigAPrev = 0;
int encoder2sigBNew = 0;
int encoder2sigBPrev = 0;
int encoder2Value = 0;

void ISR_ENCODER1(void);

void setup()
{
    // put your setup code here, to run once:
    Serial.begin(115200);

    // set up the LCD's number of columns and rows:
    lcd.begin(16, 2);

    myServo1.attach(6);
    myServo2.attach(9);

    pinMode(ENCODER1_SIGA, INPUT_PULLUP);
    pinMode(ENCODER1_SIGB, INPUT_PULLUP);
    pinMode(ENCODER2_SIGA, INPUT_PULLUP);
    pinMode(ENCODER2_SIGB, INPUT_PULLUP);

    encoder2sigAPrev = digitalRead(ENCODER2_SIGA);
    encoder2sigBPrev = digitalRead(ENCODER2_SIGB);

    attachInterrupt(digitalPinToInterrupt(ENCODER1_SIGA), ISR_ENCODER1, LOW);
}

void loop()
{
    // put your main code here, to run repeatedly:
    while (1)
    {
        Serial.print(" Encoder1Value: ");
        Serial.print(encoder1Value);
        Serial.print(" Encoder2Value: ");
        //Serial.println(encoder2Value);
        Serial.print(encoder2Value);

        // Print to LCD
        // (note: line 1 is the second row, since counting begins with 0):
        lcd.setCursor(0, 0);
        lcd.print("Encoder1 Val:   ");
        lcd.setCursor(13, 0);
        lcd.print(encoder1Value);
        lcd.setCursor(0, 1);
        lcd.print("Encoder2 Val:   ");
        lcd.setCursor(13, 1);
        lcd.print(encoder2Value);

        myServo1.write(encoder1Value);
        myServo2.write(encoder2Value);
        delay(15);

        // Encoder 1: Using interrupt: Process only once per loop
        if (encoder1Flag == 1)
        {
            encoder1Flag = 0;
        }

        // Encoder 2: Check within loop
        encoder2sigANew = digitalRead(ENCODER2_SIGA);
        encoder2sigBNew = digitalRead(ENCODER2_SIGB);
        Serial.print(" Encoder2 sigA: ");
        Serial.print(encoder2sigANew);
        Serial.print(" Encoder2 sigB: ");
        Serial.println(encoder2sigBNew);
        if (encoder2sigBPrev != encoder2sigBNew)
        {
            encoder2sigBPrev = encoder2sigBNew;
            if (encoder2sigBNew == 0)      // only chck when sigB goes LOW
            {
                if (encoder2sigANew == 1) // clockwise
                {
                    if (encoder2Value == 180)
                    {
                        encoder2Value = 180;
                    }
                    else
                    {
                        encoder2Value++;
                    }
                }
                else // anti-clockwise
                {
                    if (encoder2Value == 0)
                    {
                        encoder2Value = 0;
                    }
                    else
                    {
                        encoder2Value--;
                    }
                }
                delay(100);
            }
        }
    }
}

void ISR_ENCODER1()
{
    if (encoder1Flag == 0)
    {
        encoder1Flag = 1;

        // Clockwise
        if (digitalRead(ENCODER1_SIGA) == digitalRead(ENCODER1_SIGB))
        {
            if (encoder1Value == 180)
            {
                encoder1Value = 180;
            }
            else
            {
                encoder1Value++;
            }
        }
        // Anti-clockwise
        else
        {
            if (encoder1Value == 0)
            {
                encoder1Value = 0;
            }
            else
            {
                encoder1Value--;
            }
        }
    }
}
