#include <WiFi.h>
#include <esp_now.h>

// 设置数据结构体
typedef struct struct_message {
  double shuzi;
} struct_message;

struct_message myData;

//esp_now_recv_info my_recv_info;
// 数据接收回调函数
void OnDataRecv(const uint8_t *mac_addr, const uint8_t *data, int len) {
  //char macStr[18];
  //Serial.print("Packet from: ");
  //const uint8_t * mac_addr=my_recv_info.src_addr;
 // snprintf(macStr, sizeof(my_recv_info.src_addr), "%02x:%02x:%02x:%02x:%02x:%02x",
    //       my_recv_info.src_addr[0], my_recv_info.src_addr[1], my_recv_info.src_addr[2], my_recv_info.src_addr[3], my_recv_info.src_addr[4], my_recv_info.src_addr[5]);
  //Serial.println(macStr);

  memcpy(&myData, data, sizeof(myData));
  Serial.print("shuzi:");
  Serial.println(myData.shuzi);
  Serial.println();
}

void setup() {
  Serial.begin(9600);

  // 初始化 ESP-NOW
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  Serial.print("ESP32 Board MAC Address:  ");
  Serial.println(WiFi.macAddress());
  // 设置接收数据回调函数
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
}
