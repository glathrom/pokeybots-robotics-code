#include <WiFi.h>
#include <esp_now.h>

// this should be the sender mac address
uint8_t broadcastAddress[] = { 0x98, 0x3d, 0xae, 0xaa, 0x0b, 0xfc };

typedef struct messageStruct {
  float x;
  float y;
  float z;
} message_t;

message_t myData;

void OnDataReceive(const uint8_t *mac, const uint8_t *incomingData, int len){
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Bytes Received: ");
  Serial.println(len);
  Serial.print("X: ");
  Serial.println(myData.x);
  Serial.print("Y: ");
  Serial.println(myData.y);
  Serial.print("Z: ");
  Serial.println(myData.z);
  Serial.println();
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(2000);
  WiFi.mode(WIFI_MODE_STA);
  
  if( esp_now_init() != ESP_OK ){
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  Serial.println("ESP-NOW initialized");

  // esp_now callback function to process message
  esp_now_register_recv_cb(OnDataReceive);
  Serial.println("ESP-NOW received message callback registered");
}

void loop() {
  // put your main code here, to run repeatedly:
}
