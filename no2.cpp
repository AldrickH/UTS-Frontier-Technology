/*
Run on ESPectro32 Board
The below code run 5 tasks : 
1. Blink LED every x seconds
2. Send a String to UART every y seconds
3. Check for RGB character received from UART and turn on/off Neo Pixel RGB respectively
4. Check Button A State : Pressed or Released
5. Check Button B State : Pressed or Released

Ensure the libraries are installed : pio lib --global install [LIB NUMBER/NAME]
*/

#include <Arduino.h>

long LEDmillis = 0;
long Serialmillis = 0;
int LED_Period = 1000;
int Serial_Period = 5000;
bool LED_State = HIGH;
int previousButton_A, previousButton_B = 1;
#define Button_A 0 //GPIO0
#define Button_B 4 //GPIO4

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(Button_A, INPUT);
    pinMode(Button_B, INPUT);
    Serial.begin(115200);
}

void loop()
{
    // Task 1 : Blink LED Every LED_Period
    if (millis() - LEDmillis >= LED_Period)
    {
        LEDmillis = millis();
        LED_State = !LED_State;
        digitalWrite(LED_BUILTIN, LED_State);
    }

    // Task 4 : Check Button_A State
    int currentButton_A = digitalRead(Button_A);
    if (currentButton_A != previousButton_A)
    {
        if (currentButton_A == 0)
        {
            if (LED_Period < 2500)
            {
                LED_Period += 100;
                Serial.println("LED period increased 100ms to: " + String(LED_Period) + "ms");
            }
        }

        previousButton_A = currentButton_A;
    }

    // Task 5 : Check Button_B State
    int currentButton_B = digitalRead(Button_B);
    if (currentButton_B != previousButton_B)
    {
        if (currentButton_B == 0)
        {
            if (LED_Period > 100)
            {
                LED_Period -= 100;
                Serial.println("LED period decreased 100ms to: " + String(LED_Period) + "ms");
            }
        }

        previousButton_B = currentButton_B;
    }
}