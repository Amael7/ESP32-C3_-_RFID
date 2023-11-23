// Define for all PINS
#define GREEN 2 // The pin's number of the Green led
#define ORANGE 3 // The pin's number of the Orange led
#define RED 4 // The pin's number of the Red led


// Include for RFID
#include <SPI.h>
#include <MFRC522.h>

// Define for RFID
#define SS_PIN 6 // Number of the Slave Select Pin
#define RST_PIN 7 // Number of the Reset Pin
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

// Wifi DATA
#include "WiFi.h"
const char* ssid = "ssid"; // Replace "ssid" with the wifi name
const char* password = "password"; // Replace "password" with the wifi's password
const char* serverIP = "serverIP"; // Replace "serverIP" with the IP address of the server
const int serverPort = "serverPort"; // Replace "serverPort" with the server port

WiFiClient client;
// Wifi DATA

// Class LEDS to interact with leds
class LEDs {
public:
    void changeStatus(int led);
    bool checkStatus(int led);

private:
    void writeLed(int led);
    bool red = false;
    bool orange = false;
    bool green = false;

};

// Private function to write LEDS
void LEDs::writeLed(int led) {

    // Check which led has to turn on/off
    switch (led) {
    case GREEN:
        if (this->green) {
            digitalWrite(GREEN, LOW);
        }
        else {
            digitalWrite(GREEN, HIGH);
        }

        this->green = !this->green;
        break;
    case ORANGE:
        if (this->orange) {
            digitalWrite(ORANGE, LOW);
        }
        else {
            digitalWrite(ORANGE, HIGH);
        }

        this->orange = !this->orange;
        break;
    case RED:
        if (this->red) {
            digitalWrite(RED, LOW);
        }
        else {
            digitalWrite(RED, HIGH);
        }

        this->red = !this->red;
        break;
    }

};

// Function to check status of a led
bool LEDs::checkStatus(int led) {

    switch (led) {
    case GREEN:
        return this->green;
    case ORANGE:
        return this->orange;
    case RED:
        return this->red;
    default:
        return false;
    }
}

// Function to start changeStatus of a Led
void LEDs::changeStatus(int led) {

    // Check which led
    switch (led) {
    case GREEN:
        writeLed(GREEN);
        break;
    case ORANGE:
        writeLed(ORANGE);
        break;
    case RED:
        writeLed(RED);
        break;
    default:
        Serial.println("Led invalide");
        break;
    }
};

// Function to read RFID
String readRFID() {
    // Check if new card is present
    if (mfrc522.PICC_IsNewCardPresent()) {
      mfrc522.PICC_ReadCardSerial();

      Serial.println(mfrc522.uid.size);

        // Show UID TAG
        String content = "";
        byte letter;
        for (byte i = 0; i < mfrc522.uid.size; i++)
        {
            content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
            content.concat(String(mfrc522.uid.uidByte[i], HEX));
        }



        // UpperCase format
        content.toUpperCase();
        return content.substring(1);
    }
    else {
        return "null";
    }

}

void setup() {

    // Serial begin
    Serial.begin(115200);

    // Initiate  SPI bus && MFRC522
    SPI.begin();
    mfrc522.PCD_Init();
    delay(4);
    mfrc522.PCD_DumpVersionToSerial();
    Serial.println("Reader READY!");

    // Connect to wifi first
    WiFi.begin(ssid, password);

    // Wait for connect
    while(WiFi.status() != WL_CONNECTED){
      Serial.println(".");
    }
    Serial.println("WIFI Connected !");

    // Pins mode
    pinMode(GREEN, OUTPUT);
    pinMode(ORANGE, OUTPUT);
    pinMode(RED, OUTPUT);
}

// Create class instance
LEDs myLEDs;

void loop() {
    String UID = readRFID();
    Serial.println(UID);
    if (UID != "null") {
        if (client.connect(serverIP, serverPort)) {
            // SEND UID to check
            client.print(UID);

            while (client.available() == 0) {
              // Wait for server to respons back
            }

            // Get response
            String response = client.readString();

            // Check response
            if (response == "ok") {
              myLEDs.changeStatus(GREEN);
              delay(1000);
              myLEDs.changeStatus(GREEN);
              delay(1000);
            } else if (response == "no") {
              myLEDs.changeStatus(RED);
              delay(4000);
              myLEDs.changeStatus(RED);
              delay(1000);
            } else {
              myLEDs.changeStatus(ORANGE);
              delay(4000);
              myLEDs.changeStatus(ORANGE);
              delay(1000);
            }
        }
    }
    delay(100);
  }