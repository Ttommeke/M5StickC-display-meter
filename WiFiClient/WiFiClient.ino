/*
 *  This sketch sends data via HTTP GET requests to data.sparkfun.com service.
 *
 *  You need to get streamId and privateKey at data.sparkfun.com and paste them
 *  below. Or just customize this script to talk to other HTTP servers.
 *
 */

#include <WiFi.h>
#include <M5StickC.h>

const char* ssid     = "TMeQZEFqsdfq";
const char* password = "tomspass14863";

const char* host = "192.168.0.208";
const int port = 5846;

const char* meterName = "Initiatief";

WiFiClient client;

void setup()
{
    Serial.begin(115200);
    delay(10);

    // We start by connecting to a WiFi network

    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    M5.begin();

    // LCD display
    M5.Lcd.println(meterName);
    if (!client.connect(host, port)) {
        Serial.println("connection failed");
        return;
    }
    
    Serial.println("Connected!");
}

bool wasRed = false;
int intValue = 0;

void loop()
{
    delay(1000);

    //send what weter is requested
    client.println(meterName);
    
    unsigned long timeout = millis();
    while (client.available() == 0) {
        if (millis() - timeout > 5000) {
            Serial.println(">>> Client Timeout !");
            client.stop();

            if (!client.connect(host, port)) {
              Serial.println("connection failed");
              return;
            }
    
            Serial.println("Reconnected!");
            return;
        }
    }
    // Read all the lines of the reply from server and print them to Serial
    while(client.available()) {
        String value = client.readStringUntil('\n');
        intValue = value.toInt();
    }
    
    if (intValue < 20 && wasRed) {
      M5.Lcd.fillScreen(TFT_RED);
    } else {
      M5.Lcd.fillScreen(TFT_BLACK);

      M5.Lcd.setTextSize(6);
      M5.Lcd.setCursor(5,40);
      M5.Lcd.println(intValue);
    }
    
    wasRed = !wasRed;
}
