//MySQL连接测试
#include <WiFi.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>

//wifi
const char ssid[]     = "RAY 0893";// change to your WIFI SSID
const char password[] = "12345678";// change to your WIFI Password

//
IPAddress server_addr(121,41,116,214);// 服务器地址，注意是逗号
int MYSQLPort =3306;   //端口
char user[] = "panray";//用户名
char pass[] = "WjCNlhdC4c07mDkb";// 密码

WiFiClient client;            
MySQL_Connection conn((Client *)&client);

void setup() {
  Serial.begin(115200);
  delay(10);

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  //连接 mysql server
  if (conn.connect(server_addr, 3306, user, pass)) {
    Serial.println("Connectio SQL OK!.");
     delay(1000);
  }
  else{
    Serial.println("Connection failed.");
  }
  delay(2000);  
  //插入数据库的名称，改变其值
  char INSERT_SQL[] = "INSERT INTO timles.datalog (temp,humd) VALUES ('35','60')";//傳入的值固定為溫度,濕度為35,60
  MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);  
  cur_mem->execute(INSERT_SQL);//发送
  delete cur_mem;
  conn.close();                  // 断开连接
  Serial.println("Data Saved.");//数据已保存
}

void loop() {
//do nothing
}