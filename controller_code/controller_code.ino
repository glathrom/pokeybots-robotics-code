/*
  Rui Santos & Sara Santos - Random Nerd Tutorials
  Complete project details at https://RandomNerdTutorials.com/esp-now-two-way-communication-esp32/
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
  esptool.py --chip esp32c3 --port /dev/ttyACM1 erase_flash

  Modified by ABC RoboKits: abcrobokits.com
*/

#include <esp_now.h>
#include <WiFi.h>
#include <Wire.h>

// REPLACE WITH THE MAC ADDRESS OF YOUR robot esp
uint8_t broadcastAddress[] = {0x9C, 0x9E, 0x6E, 0xF7, 0x4F, 0x68}; //9c:9e:6e:f7:4d:74

// Define variables to store desired servo rotations
float rotationLeft;
float rotationRight;
float Arm;

// Define variables to store current servo rotations
float incomingRotationLeft;
float incomingRotationRight;
float incomingArm;

// Define variables to store joystick readings
int joystickX;
int joystickY;
int Button;

// Variable to store if sending data was successful
String success;

//Structure example to send data
//Must match the receiver structure
typedef struct struct_message {
  float RotLeft;
  float RotRight;
  float RotArm;
} struct_message;

// Create a struct_message (following above structure) called PosReadings to hold sensor readings
struct_message RotReadings;

// Create a struct_message (following above structure) to hold incoming sensor readings
struct_message incomingReadings;

// Define variable to hold peer information
// Note: 'peer' represents the device this code communicates with
esp_now_peer_info_t peerInfo; 

// This code runs when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  if (status == 0) {
    success = "Delivery Success :)";
  }
  else {
    success = "Delivery Fail :(";
  }
}

// This code runs when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));
  Serial.print("Bytes received: ");
  Serial.println(len);
  incomingRotationLeft = incomingReadings.RotLeft;
  incomingRotationRight = incomingReadings.RotRight;
  incomingArm = incomingReadings.RotArm;
  Serial.print("RotLeft: ");
  Serial.println(incomingRotationLeft);
  Serial.print("RotRight: ");
  Serial.println(incomingRotationRight);
  Serial.print("RotArm: ");
  Serial.println(incomingArm);
}

void setup() {
  // Setup with the computer to handle troubleshooting
  Serial.begin(115200);

  pinMode(D2, INPUT_PULLUP);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Connect to robot's unique wifi (this process is called ESPNow)
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully completed, we will try to send data
  // get the status of Transmitted packet of data
  esp_now_register_send_cb((esp_now_send_cb_t)OnDataSent);

  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  // Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
  // Register for code to run when data is received
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
}

void loop() {

  joystickX = analogRead(D1);          // read the joystick
  rotationLeft = map((joystickX), 0, 4096, 0, 180); // convert joystick position into servo rotation data
  
  joystickY = analogRead(D0);
  rotationRight = map((joystickY), 0, 4096, 0, 180);

  Button = digitalRead(D2);
  Arm = map(Button, 0, 1, 0, 1); // convert joystick button into arm servo position

  // Set values to send
  RotReadings.RotLeft = rotationLeft;
  RotReadings.RotRight = rotationRight;
  RotReadings.RotArm = Arm;

  // Send data via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &RotReadings, sizeof(RotReadings));

  if (result == ESP_OK) {
    Serial.println("Sent with success"); // tell the computer if everything sent properly
  }
  else {
    Serial.println("Error sending the data"); // tell the computer there was an error
  }

  delay(100); //wait for 0.1 seconds before starting the loop over
}
