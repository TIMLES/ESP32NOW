#include <WiFi.h>
#include <esp_now.h>

// 设置数据结构体
typedef struct struct_message
{
  String call;
  double data;
} struct_message;


struct_message myData;
uint8_t UpAddrees;//上位机地址
//uint8_t UpAddrees;
String theCall;
//esp_now_recv_info my_recv_info;
// 数据接收回调函数
void OnDataRecv(const uint8_t *mac_addr, const uint8_t *data, int len) {
  char macStr[18];
  Serial.print("Packet from: ");
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
   mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.println(macStr);
  UpAddrees=*mac_addr;
  memcpy(&myData, data, sizeof(myData));
  theCall=myData.call;
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
float dataT=20.00;
void loop() {
  if(theCall=="17860715233"){
    myData.call="OK!";
    Serial.println(theCall);
    esp_err_t result = esp_now_send(&UpAddrees, (uint8_t *)&myData, sizeof(myData));
  // 检查数据是否发送成功
  if (result == ESP_OK)
  {
    Serial.println("Sent with success");
  }
  else
  {
    Serial.println("Error sending the data");
  }
  
  }

  dataT=dataT+0.01;
  myData.data=dataT;
  delay(500);
}

