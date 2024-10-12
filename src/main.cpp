// esp32连接远程数据库并上传
#include <WiFi.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>

// WiFi 密码与连接
char ssid[] = "RAY 0893"; // your SSID
char pass[] = "12345678"; // your SSID Password

IPAddress server_addr(121, 41, 116, 214); // MySQL服务器IP
char user[] = "panray";                   // MySQL用户名
char password[] = "WjCNlhdC4c07mDkb";     // MySQL user 登陆密码

WiFiClient client; // Use this for WiFi客户端
MySQL_Connection conn((Client *)&client);

void transData_SQL();
void setup()

{
  Serial.begin(9600);
  delay(10);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  // End WiFi section
  delay(100);
  transData_SQL();
}

void loop()
{
     if (Serial.available() > 0)
    {
      String AA = Serial.readString();
      delay(50);
      if (AA == "17860715233")
      {
        Serial.println(AA);
        transData_SQL();
      }
      else{Serial.println("NO!");}
}}

// 目标
void transData_SQL()
{
  Serial.println("Connecting...");

  if (conn.connect(server_addr, 3306, user, password))
  {
    Serial.println("mySQL is OK!");
    delay(1000);
  }
  else
    Serial.println("Connection failed.");
  delay(100);

  // SQL语句，%s为无符号字符串，可使用sprintf()函数插入目标数据，%f为浮点
  //char SQLdata[1024]; // 要写入mysql的数据，需满足语句格式
  char *SQLdata=new char[1024];

  char SQL[]="INSERT INTO timles.test (temp,humd) VALUES ('0','0')";
  strcat(SQLdata, SQL) ;//附加字符串
  char format[] = ",('%f','%f')"; // 格式
  char formatData[15] ; // 格式
  
  for(int i=0;i<5;i++) {
  sprintf(formatData, format, i, i*2); // sprintf(被合并的值，数据1，数据2)
  strcat(SQLdata, formatData) ;//附加字符串
  }

  Serial.println(SQLdata);
  MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn); // 创建一个Mysql实例
  cur_mem->execute(SQLdata);                       // 将采集到的温湿度值插入数据库中
  Serial.println("读取传感器数据，并写入数据库");
  delete cur_mem; // 删除mysql实例为下次传输作准备
  conn.close();   // close the connection
  Serial.println("Data Saved.");
  delete[] SQLdata;
}
