#include <esp_now.h>
#include <WiFi.h>
#include <ESP32Servo.h>
#include <Wire.h>

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5;

uint8_t broadcastAddress[] = {0x94, 0xa9, 0x90, 0x7c, 0x16, 0x28};

float PositionX;
float PositionY;
float PositionZ;

String success;

typedef struct {
  float PosX;
  float PosY;
  float PosZ;
} struct_message_t;

struct_message_t PosReadings;
struct_message_t incomingReadings;

esp_now_peer_info_t peerInfo;

void OnDataSent(const uint8_t *mac, esp_now_send_status_t status ){
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Failed");
  if( status == 0 ){
    success = "Delivery Success :)";
  } else {
    success = "Delivery Fail :(";

  }
}

void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len ){
  memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));
  Serial.printf("Bytes Received: %d\n ", len);
  float incomingPosX = incomingReadings.PosX;
  float incomingPosY = incomingReadings.PosY;
  float incomingPosZ = incomingReadings.PosZ;
  Serial.printf("PosX: %d\n", incomingPosX);
  Serial.printf("PosY: %d\n", incomingPosY);
  Serial.printf("PosZ: %d\n", incomingPosZ);
  /*
  X = int(incomingPosX);
  Y = int(incomingPosY);
  Z = int(incomingPosZ);
  */
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  servo1.attach(D1);
  servo2.attach(D2);
  servo3.attach(D3);
  servo4.attach(D4);
  servo5.attach(D5);

  WiFi.mode (WIFI_STA);

  if( esp_now_init() != ESP_OK){
    Serial.println("Error initializing ESP-NOW");
    return ;
  }

  esp_now_register_send_cb(esp_now_send_cb_t (OnDataSent));
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if( esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer!");
    return ;
  }

  esp_now_register_recv_cb(esp_now_recv_cb_t (OnDataRecv));
}

void loop() {
  // put your main code here, to run repeatedly:
  PositionX = 90;
  PositionY = 90;
  PositionZ = 90;

  PosReadings.PosX = PositionX;
  PosReadings.PosY = PositionY;
  PosReadings.PosZ = PositionZ;

  esp_err_t  result = esp_now_send(broadcastAddress, (uint8_t *) &PosReadings, sizeof(PosReadings));

  if( result == ESP_OK ){
    Serial.println("Sent with Success");
  } else {
    Serial.println("Error sending data");
  }

  if( PositionY < 120 && PositionY > 70){
    if( PositionX > 120 ){
      servo1.write(0);
      servo2.write(0);
    } else if( PositionX < 70 ){
      servo1.write(180);
      servo2.write(180);
    } else {
      servo1.write(90);
      servo2.write(90);
    }
  } else if ( PositionY > 120 ){
    servo1.write(0);
    servo2.write(180);
  } else if (PositionY < 70){
    servo1.write(180);
    servo2.write(0);
  }

  if( PositionZ == 0){
    servo3.write(90);
  } else {
    servo3.write(0);
  }

  delay(100);
}
