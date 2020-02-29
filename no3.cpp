#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Adafruit_NeoPixel.h>

#define BLYNK_PRINT Serial

char auth[] = "heGliaRBcKw07pTpo5dsYNfT3E6935Sy";

char ssid[] = "Aldrick's Asus";
char pass[] = "aldrick1";
int r, g, b;

#define NeoPixel 27 //GPIO27
#define NumPixels 1 //Only one RGB LED

Adafruit_NeoPixel pixels(NumPixels, NeoPixel, NEO_GRB + NEO_KHZ800);

BLYNK_READ(V9)
{
  int value = analogRead(36);
  Blynk.virtualWrite(V9, value);

  if (value > 0.8 * 1023)
  {
    Blynk.notify("Sudah melewati 80%");
  }
}

BLYNK_WRITE(V1)
{
  r = param[0].asInt();
  g = param[1].asInt();
  b = param[2].asInt();
}

void setup()
{
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);

  pinMode(NeoPixel, OUTPUT);
  pixels.begin();
  pixels.clear();
}

void loop()
{
  Blynk.run();
  int value = analogRead(36);
  Serial.println(value);

  pixels.setPixelColor(0, pixels.Color(r, g, b));
  pixels.show();
}