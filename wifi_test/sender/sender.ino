#include <WiFi.h>
#include <esp_now.h>

// this should be receiver mac address
uint8_t broadcastAddress[] = { 0x18, 0x8b, 0x0e, 0x91, 0xae, 0xb8 };

typedef struct messageStruct {
  float x;
  float y;
  float z;
} message_t;

message_t  myData;

esp_now_peer_info_t peerInfo;

void OnDataSent(const uint8_t *mac, esp_now_send_status_t status){
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
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

  // resgister the callback function called after
  // sending data
  esp_now_register_send_cb(OnDataSent);

  memcpy(peerInfo.peer_addr, broadcastAddress, 6);

  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if( esp_now_add_peer(&peerInfo) != ESP_OK ){
    Serial.println("Error addinng peer");
    return;
  }
  Serial.printf("Successfully added receiver: %02x:%02x:%02x:%02x:%02x:%02x\n", broadcastAddress[0], broadcastAddress[1], broadcastAddress[2], broadcastAddress[3], broadcastAddress[4], broadcastAddress[5]);
}

void loop() {
  // put your main code here, to run repeatedly:
  myData.x = 1.2;
  myData.y = 3.4;
  myData.z = 5.6;

  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));

  if( result == ESP_OK ){
    Serial.println("message sent successfully");
  } else {
    Serial.println("error sending message");
  }

  delay(5000);
}
