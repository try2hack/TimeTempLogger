#include <ESP8266WiFi.h>
#include <time.h>
#include <TridentTD_LineNotify.h>
#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "Jaidee_Home"; //ใส่ชื่อ SSID Wifi
const char* password = ""; //ใส่รหัสผ่าน
const char* LINE_TOKEN = "RqQMo817zLgTp9SHqqNQ2nERdGgP0e3x1MoXpGBlNZQ";

int timezone = 7 * 3600; 
int dst = 0; //กำหนดค่า Date Swing Time

void setup()
{
Serial.begin(115200);
LINE.setToken(LINE_TOKEN);
WiFi.mode(WIFI_STA); //เชื่อมต่อ Wifi
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) {
Serial.print("*");
delay(1000);
}
configTime(timezone, dst, "pool.ntp.org", "time.nist.gov"); //ดึงเวลาจาก Server
Serial.println("\nLoading time");
while (!time(nullptr)) {
Serial.print("*");
delay(1000);
}
Serial.println("");
dht.begin();
}

void loop()
{
//configTime(timezone, dst, "pool.ntp.org", "time.nist.gov"); //แสดงเวลาปัจจุบัน
time_t now = time(nullptr);
struct tm* p_tm = localtime(&now);

Serial.print(p_tm->tm_hour);
Serial.print(":");
Serial.print(p_tm->tm_min);
Serial.print(":");
Serial.print(p_tm->tm_sec);
if((p_tm->tm_min) == 0 && (p_tm->tm_sec)== 0)
  {  
  // Wait a few seconds between measurements.
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }  
  Serial.print(F("ความชื้นสัมพัทธ์:"));
  Serial.print(h);
  Serial.print(F("%  อุณหภูมิ:"));
  Serial.print(t);
  Serial.println(F("°C ")); 
  String temp = String(t);
  h=h-15;
  String hud = String(h);
  
  LINE.notify("อุณหภูมิ : "+temp+" C"+" ความชื้นสัมพัทธ์ : "+hud+" %");  
  }
  
Serial.println("");
delay(1000);

}
