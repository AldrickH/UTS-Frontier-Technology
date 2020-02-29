#include <Arduino.h>

long LEDmillis = 0;
int LED_Period = 500;
bool LED_State = HIGH;

int buttonState_A = 0;
int lastButtonState_A = 0;
bool buttonPressed_A_Limit = 0;
bool lastButtonPressed_A_Limit = 0;

int startPressed = 0;
int endPressed = 0;
int holdTime = 0;

#define Button_A 0

void updateState()
{
    if (buttonState_A == LOW)
    {
        startPressed = millis();
        holdTime = 0;
        buttonPressed_A_Limit = true;
    }
    else
    {
        endPressed = millis();
        holdTime = endPressed - startPressed;

        Serial.println("Button was pressed for: " + String(holdTime / 1000) + " seconds.");
        digitalWrite(LED_BUILTIN, HIGH);
        buttonPressed_A_Limit = false;
    }
}

void updateCounter()
{
    if (buttonPressed_A_Limit == true)
    {
        if (millis() - startPressed >= 10000)
        {
            Serial.println("Button is pressed for more than 10 seconds.");

            // buttonState_A = HIGH;
            buttonPressed_A_Limit = false;
        }
    }
}

void runLED()
{
    if (millis() - LEDmillis >= LED_Period)
    {
        LEDmillis = millis();
        LED_State = !LED_State;
        digitalWrite(LED_BUILTIN, LED_State);
    }
}

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(Button_A, INPUT);
    digitalWrite(LED_BUILTIN, LED_State);
    Serial.begin(115200);
}

void loop()
{
    buttonState_A = digitalRead(Button_A);

    if (buttonState_A != lastButtonState_A)
    {
        updateState();
    }
    else
    {
        if (buttonState_A == LOW && buttonPressed_A_Limit)
        {
            runLED();
        }

        updateCounter();
    }

    lastButtonState_A = buttonState_A;
}