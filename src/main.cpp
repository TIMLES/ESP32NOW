
#include <WiFi.h>                  
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>


IPAddress server_addr(121,41,116,214);  // IP of the MySQL *server* here
char user[] = "panray";              // MySQL user login username
char password[] = "WjCNlhdC4c07mDkb";        // MySQL user login password

// WiFi card example
char ssid[] = "RAY 0893";    // your SSID
char pass[] = "12345678";       // your SSID Password
WiFiClient client;            // Use this for WiFi instead of EthernetClient
MySQL_Connection conn((Client *)&client);

void setup() {
Serial.begin(9600);
  delay(10);

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, pass);  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  // End WiFi section

  Serial.println("Connecting...");
  if (conn.connect(server_addr, 3306, user, password)) {
    Serial.println("OK!");
    delay(1000);
  }
  else
    Serial.println("Connection failed.");
  delay(2000);  
  //insert, change database name and values by string and char[]
  char INSERT_SQL[] = "INSERT INTO timles.test (temp,humd) VALUES ('35','60')";//傳入的值固定為溫度,濕度為35,60
  MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);  
  cur_mem->execute(INSERT_SQL);//execute SQL
  delete cur_mem;
  conn.close();                  // close the connection
  Serial.println("Data Saved.");
}

void loop() {
}
