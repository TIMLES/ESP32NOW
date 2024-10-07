#include <WiFi.h>
#include <esp_now.h>

// 设置数据结构体
typedef struct struct_message {
  double shuzi;
} struct_message;

struct_message myData;//数据对象为myData

// 接收设备的 MAC 地址
uint8_t OtheAddrees[] = { 0x64, 0xE8, 0x33, 0x7E, 0xB3, 0x0C };
//本机地址
uint8_t myAddrees[] = { 0x64, 0xE8, 0x33, 0x89, 0xCA, 0xC8 };

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);
void setup() {
  Serial.begin(9600);

  // 初始化 ESP-NOW
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {//ESP_OK表示无错误函数返回
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  Serial.print("ESP32 Board MAC Address:  ");
  Serial.println(WiFi.macAddress());
  
  // 设置发送数据回调函数
  esp_now_register_send_cb(OnDataSent);

  // 绑定数据接收端
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, OtheAddrees, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;//发送数据是否加密

  // 检查设备是否配对成功
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
}

int i = 1;
void loop() {
  // 设置要发送的数据
  myData.shuzi = i;

  // 发送数据
  esp_err_t result = esp_now_send(OtheAddrees, (uint8_t *)&myData, sizeof(myData));

  // 检查数据是否发送成功
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  } else {
    Serial.println("Error sending the data");
  }
  delay(5000);
  i++;
}

// 数据发送回调函数
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  char macStr[18];
  Serial.print("Packet to: ");
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.println(macStr);
  Serial.print("Send status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  Serial.println();
}