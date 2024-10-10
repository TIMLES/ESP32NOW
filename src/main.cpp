#include <Arduino.h>
#include <WiFi.h>
#include "PubSubClient.h"   ////A client library for MQTT messaging.


/***************************************** 需要更改/
/* 连接WIFI SSID和密码 */
#define WIFI_SSID         "DESKTOP-LQ2U3EJ 3891"
#define WIFI_PASSWD       "12345678"
/* 设备的三元组信息*/
#define PRODUCT_KEY       "a1W3S1waPyr"
#define DEVICE_NAME       "raypan"
#define DEVICE_SECRET     "10748431b4e6443f67385fef02b8d269"
#define REGION_ID         "cn-shanghai"
//阿里云客户端id与密码
#define CLIENT_ID         "a1W3S1waPyr.raypan|securemode=2,signmethod=hmacsha256,timestamp=1728540738182|"
#define MQTT_PASSWD       "6ae47a97b5a4b0b34a6ddaa5ffae8f7807289c85843842f67336569e7d8b9bf1"
//宏定义订阅主题
#define ALINK_BODY_FORMAT         "{\"id\":\"raypan\",\"version\":\"1.0\",\"method\":\"thing.event.property.post\",\"params\":%s}"
#define ALINK_TOPIC_PROP_POST     "/sys/" PRODUCT_KEY "/" DEVICE_NAME "/thing/event/property/post"

/* 线上环境域名和端口号，不需要改 */
#define MQTT_SERVER       PRODUCT_KEY".iot-as-mqtt."REGION_ID ".aliyuncs.com"
#define MQTT_PORT         1883
#define MQTT_USRNAME      DEVICE_NAME"&"PRODUCT_KEY

unsigned long lastMs = 0;

WiFiClient espClient;
PubSubClient  client(espClient);

float soil_data ;  
float tep;  

//连接wifi
void wifiInit()
{
    WiFi.begin(WIFI_SSID, WIFI_PASSWD);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.println("WiFi not Connect");
    }
}

//mqtt连接
void mqttCheckConnect()
{
    while (!client.connected())
    {
        Serial.println("Connecting to MQTT Server ...");
        if(client.connect(CLIENT_ID, MQTT_USRNAME, MQTT_PASSWD))
        {
          Serial.println("MQTT Connected!");
        }
        else{
           Serial.print("MQTT Connect err:");
            Serial.println(client.state());
            delay(5000);
          }
        
    }
}

//上传温湿度、二氧化碳浓度
void mqttIntervalPost()
{
    char param[32];
    char jsonBuf[128];
    
    double AA=random(0,100);
    soil_data = AA;   
    sprintf(param, "{\"temperature\":%2f}", soil_data);
    sprintf(jsonBuf, ALINK_BODY_FORMAT, param);
    Serial.println(jsonBuf);
    boolean b = client.publish(ALINK_TOPIC_PROP_POST, jsonBuf);
    if(b){
      Serial.println("publish Humidity success"); 
    }else{
      Serial.println("publish Humidity fail"); 
    }
}

void setup()
{
  Serial.begin(115200);
  //dht.begin();
  wifiInit();
  client.setServer(MQTT_SERVER, MQTT_PORT);   /* 连接MQTT服务器 */
}

void loop()
{
      if (millis() - lastMs >= 5000)
    {
        lastMs = millis();
        mqttCheckConnect(); 
        /* 上报 */
        mqttIntervalPost();
    }
    client.loop();
    delay(2000);
} 
