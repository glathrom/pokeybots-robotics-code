/*
  Rui Santos & Sara Santos - Random Nerd Tutorials
  Complete project details at https://RandomNerdTutorials.com/esp-now-two-way-communication-esp32/
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
  
  Modified by ABC RoboKits: abcrobokits.com

*/

#include <esp_now.h>
#include <WiFi.h>

#include <ESP32Servo.h>
#include <Wire.h>

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5;

// REPLACE WITH THE MAC Address of your controller esp 
uint8_t broadcastAddress[] = {0x64, 0xE8, 0x33, 0x86, 0xAF, 0x74}; //64:e8:33:86:af:74

float rotationLeft;
float rotationRight;
float Arm;

int servoLeft = 90;
int servoRight = 90;
int servoArm = 90;

// Define variables to store incoming readings
float incomingRotationLeft;
float incomingRotationRight;
float incomingArm;

// Variable to store if sending data was successful
String success;

//Structure example to send data

//Must match the receiver structure
typedef struct struct_message {
    float RotLeft;
    float RotRight;
    float RotArm;
} struct_message;

// Create a struct_message called RotReadings to hold sensor readings
struct_message RotReadings;

// Create a struct_message to hold incoming sensor readings
struct_message incomingReadings;

esp_now_peer_info_t peerInfo;

// Callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  if (status == 0){
    success = "Delivery Success :)";
    digitalWrite(D10, HIGH);
    
  }
  else{
    success = "Delivery Fail :(";
    servoLeft = 90;
    servoRight = 90;
    servoArm = 90;
    digitalWrite(D10, LOW);
  }
}

// Callback when data is received
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
  servoLeft=int(incomingRotationLeft);
  servoRight=int(incomingRotationRight);
  servoArm=int(incomingArm);
  
}
 
void setup() {
  // Init Serial Monitor
  Serial.begin(115200);

  servo1.attach(D1);
  servo5.attach(D2);
  servo3.attach(D3);
  servo4.attach(D4);
  servo2.attach(D5);
 
   
  
  pinMode(D10, OUTPUT);
  
 
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb((esp_now_send_cb_t)OnDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  // Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
}
 
void loop() {
 
 rotationLeft=90;
 rotationRight=90;
 Arm=90;

 
  // Set values to send
  RotReadings.RotLeft = rotationLeft;
  RotReadings.RotRight = rotationRight;
  RotReadings.RotArm = Arm;

  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &RotReadings, sizeof(RotReadings));
   
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data"); 
  }


  // Student can edit servo.write numbers to center servos (see below)
// for instance, can change 90 to 88 or 93 if there is undesired movement
//"servo#.write(90);"
//replace # with the servo number

  if (abs(90-servoLeft) > 25){
  servo1.write(servoLeft);
  servo5.write(servoLeft);
  }
  else {
    servo1.write(90);
    servo5.write(90);
  }

  
  if (abs(90-servoRight) > 25){
  servo2.write(servoRight);
  servo4.write(servoRight);
  }
  else {
    servo2.write(90);
    servo4.write(90);
  }



  
  if (servoArm == 0){
  
  servo3.write(90);

  }


  else{

    servo3.write(0);  // can change to (180) to reverse direction  
  }
        
  delay(100);
}
