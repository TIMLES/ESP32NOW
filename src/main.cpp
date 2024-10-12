// esp32����Զ�����ݿⲢ�ϴ�
#include <WiFi.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>

// WiFi ����������
char ssid[] = "RAY 0893"; // your SSID
char pass[] = "12345678"; // your SSID Password

IPAddress server_addr(121, 41, 116, 214); // MySQL������IP
char user[] = "panray";                   // MySQL�û���
char password[] = "WjCNlhdC4c07mDkb";     // MySQL user ��½����

WiFiClient client; // Use this for WiFi�ͻ���
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

// Ŀ��
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

  // SQL��䣬%sΪ�޷����ַ�������ʹ��sprintf()��������Ŀ�����ݣ�%fΪ����
  //char SQLdata[1024]; // Ҫд��mysql�����ݣ�����������ʽ
  char *SQLdata=new char[1024];

  char SQL[]="INSERT INTO timles.test (temp,humd) VALUES ('0','0')";
  strcat(SQLdata, SQL) ;//�����ַ���
  char format[] = ",('%f','%f')"; // ��ʽ
  char formatData[15] ; // ��ʽ
  
  for(int i=0;i<5;i++) {
  sprintf(formatData, format, i, i*2); // sprintf(���ϲ���ֵ������1������2)
  strcat(SQLdata, formatData) ;//�����ַ���
  }

  Serial.println(SQLdata);
  MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn); // ����һ��Mysqlʵ��
  cur_mem->execute(SQLdata);                       // ���ɼ�������ʪ��ֵ�������ݿ���
  Serial.println("��ȡ���������ݣ���д�����ݿ�");
  delete cur_mem; // ɾ��mysqlʵ��Ϊ�´δ�����׼��
  conn.close();   // close the connection
  Serial.println("Data Saved.");
  delete[] SQLdata;
}
