#include <WiFi.h>
#include <esp_now.h>

// �������ݽṹ��
typedef struct struct_message
{
  String call;
  double data;
} struct_message;

struct_message myData; // ���ݶ���ΪmyData

// �����豸�� MAC ��ַ
uint8_t OtheAddrees[] = {0x64, 0xE8, 0x33, 0x7E, 0xB3, 0x0C};
// ������ַ
uint8_t myAddrees[] = {0x64, 0xE8, 0x33, 0x89, 0xCA, 0xC8};

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);
void OnDataRecv(const uint8_t *mac_addr, const uint8_t *data, int len);
void setup()
{
  Serial.begin(9600);

  // ��ʼ�� ESP-NOW
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK)
  { // ESP_OK��ʾ�޴���������
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  Serial.print("ESP32 Board MAC Address:  ");
  Serial.println(WiFi.macAddress());

  // ���÷������ݻص�����
  esp_now_register_send_cb(OnDataSent);

  // �����ݽ��ն�
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, OtheAddrees, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false; // ���������Ƿ����

  // ����豸�Ƿ���Գɹ�
  if (esp_now_add_peer(&peerInfo) != ESP_OK)
  {
    Serial.println("Failed to add peer");
    return;
  }
}
String AA;bool kk=0; esp_err_t result;
int i = 1;
void loop()
{

  if (Serial.available() > 0)
  {
    String AA = Serial.readString();
    delay(50);
    result = esp_now_send(OtheAddrees, (uint8_t *)&myData, sizeof(myData));
    kk=0;
      if (result == ESP_OK)
  {
    Serial.println("Sent with success");
  }
  else
  {
    Serial.println("Error sending the data");
  }
  
  }
  // ����Ҫ���͵�����
  myData.call= AA;
  myData.data = i;
  
  delay(500);
  i++;
}

// ���ݷ��ͻص�����
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
{
  char macStr[18];
  Serial.print("Packet to: ");
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.println(macStr);
  Serial.print("Send status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  Serial.println();
}

void OnDataRecv(const uint8_t *mac_addr, const uint8_t *data, int len) {
  memcpy(&myData, data, sizeof(myData));
  if(kk){
  char macStr[18];
  Serial.print("Packet from: ");
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
  mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.println(macStr);
  Serial.print("״̬:");
  Serial.println(myData.call);
  kk=1;
  }
  Serial.println(myData.data);
}