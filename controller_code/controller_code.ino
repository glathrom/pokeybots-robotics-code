#include <esp_now.h>
#include <WiFi.h>
#include <Wire.h>

// six byte mac address for the controller board obtained
// from the get_mac_address program
uint8_t broadcastAddress[] = {0x94, 0xa9, 0x90, 0x77, 0x6a, 0x64};

// float PositionX;
// float PositionY;
// float PositionZ;

float incomingPosX;
float incomingPosY;
float incomingPosZ;

int ch0;
int ch1;
int ch2;

String success;

typedef struct {
  float PosX;
  float PosY;
  float PosZ;
} struct_message_t;

struct_message_t PosReadings;
struct_message_t incomingReadings;

esp_now_peer_info_t peerInfo;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status){
  Serial.printf("\r\nLast Packet Send Status:\t");
  Serial.printf(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  if( status == 0 ){
    success = "Delivery Success :)";
  } else {
    success = "Delivery Failure :(";
  }
}

void OnDataReceived(const uint8_t *mac_addr, const uint8_t *incomingData, int len){
  memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));
  Serial.printf("Bytes Received: %d\n", len);
  Serial.printf("PosX: %f\n", incomingReadings.PosX);
  Serial.printf("PosY: %f\n", incomingReadings.PosY);
  Serial.printf("PosZ: %f\n", incomingReadings.PosZ);
  Serial.printf("********************\n");
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  pinMode(D2, INPUT_PULLUP);
  Serial.printf("Pin D2 set high\n");

  WiFi.mode(WIFI_STA);
  if( esp_now_init() != ESP_OK){
    Serial.printf("Error Initializing ESP-NOW\n");
    return;
  }
  Serial.printf("esp_now is OK!\n");

  esp_now_register_send_cb((esp_now_send_cb_t) OnDataSent);

  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if( esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.printf("Failed to add peer\n");
    return ;
  }
  Serial.printf("peer added\n");

  esp_now_register_recv_cb((esp_now_recv_cb_t) OnDataReceived);
}

void loop() {
  // put your main code here, to run repeatedly:



  ch0 = analogRead(D0);
  PosReadings.PosX = map(ch0, 0, 4096, 0, 180);
  ch1 = analogRead(D1);
  PosReadings.PosY = map(ch1, 0, 4096, 0, 180);
  ch2 = analogRead(D2);
  PosReadings.PosZ = map(ch2, 0, 4096, 0, 180);

  // PosReadings.PosX = PositionX;
  // PosReadings.PosY = PositionY;
  // PosReadings.PosZ = PositionZ;

  Serial.printf("********************\n");
  Serial.printf("PosX: %f\n", PosReadings.PosX);
  Serial.printf("PosY: %f\n", PosReadings.PosY);
  Serial.printf("PosZ: %f\n", PosReadings.PosZ);

  // esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &PosReadings, sizeof(PosReadings));

  // if( result == ESP_OK){
  //   Serial.printf("Send with success\n");
  // } else {
  //   Serial.printf("Error sending data\n");
  // }

  delay(100);

}
