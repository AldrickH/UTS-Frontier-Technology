#include <WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_NeoPixel.h>


// Replace the next variables with your SSID/Password combination
const char *ssid = "Rey-Wifi";
const char *password = "reyreynaldo002";

// Add your MQTT Broker IP address, example
const char *mqtt_server = "52.87.183.206";
#define MQTT_TOPIC_ADC "IOT/KELOMPOK3/ADC"
#define MQTT_TOPIC_RGB "IOT/KELOMPOK3/RGB"

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
//char msg[50];
int value = 0;

#define LED_PIN 27
#define LED_COUNT 1
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);



void setColor(int r ,int g  ,int b ){
    strip.setPixelColor(0,r,g,b);
    strip.show();
}

void setup_wifi()
{
    delay(10);
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void callback(char *topic, byte *message, unsigned int length)
{
    Serial.print("Message arrived on topic: ");
    Serial.print(topic);
    Serial.print(". Message: ");
    String messageTemp;

    for (int i = 0; i < length; i++)
    {
        Serial.print((char)message[i]);
        messageTemp += (char)message[i];
    }
    Serial.println();

    // Feel free to add more if statements to control more GPIOs with MQTT
    // If a message is received on the topic esp32/output, we can check if the message is either "on" or "off".
    if (String(topic) == MQTT_TOPIC_RGB)
    {
        Serial.print("Changing output to ");
        if (messageTemp == "RED")
        {
            Serial.println("RED");
            setColor(255,0,0);
        }
        else if (messageTemp == "GREEN")
        {
            Serial.println("GREEN");
            setColor(0,255,0);
        }
        else if (messageTemp == "BLUE")
        {
            Serial.println("BLUE");
            setColor(0,0,255);
        }
    }
}

void reconnect()
{
    // Loop until we're reconnected
    while (!client.connected())
    {
        Serial.print("Attempting MQTT connection...");
        // Attempt to connect
        // Change to your username and password
        if (client.connect("ESP32 MQTT Client", "reynaldo", "12345"))
        {
            Serial.println("connected");
            // Subscribe
            client.subscribe(MQTT_TOPIC_RGB);
        }
        else
        {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 3 seconds");
            // Wait 3 seconds before retrying
            delay(3000);
        }
    }
}

void setup()
{
    Serial.begin(115200);
    setup_wifi();
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
    pinMode(LED_PIN, OUTPUT);
    strip.begin();
    strip.clear();
}

void loop()
{
    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.print("Reconnecting WiFi: ");
        Serial.println(ssid);
        WiFi.mode(WIFI_STA);
        WiFi.begin(ssid, password);
        while (WiFi.status() != WL_CONNECTED)
        {
            Serial.print(".");
            delay(500);
        }
        Serial.println("WiFi Connected");
    }

    while (WiFi.status() == WL_CONNECTED){

        if (!client.connected())
        {
            reconnect();
        }
        client.loop();

        long now = millis();
        if (now - lastMsg > 5000){
            lastMsg = now;
            int temp= analogRead(36);
            Serial.println("Phototransistor: "+String(temp));
            client.publish(MQTT_TOPIC_ADC, String(temp).c_str());
        }
    }
}