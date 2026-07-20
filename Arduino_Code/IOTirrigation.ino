#include <ESP8266WiFi.h>
int sensor = A0;// moisture sensor 
int value;
int output;
String apiKey = "G0KUPS8716NUMNPL"; //
const char* ssid = "Reena"; // Here you can change Username and Password of wifi Hotspot
const char* password = "password";
const char* server = "api.thingspeak.com";
const int relay = 16;
const int relayon = 1;
const int relayoff = 0;
WiFiClient client;


void setup() 
{
pinMode(relay, OUTPUT);

Serial.begin(115200);
delay(10);
WiFi.begin(ssid, password);
 
Serial.println();
Serial.println();
Serial.print("Connecting to ");
Serial.println(ssid);
 
WiFi.begin(ssid, password);
 
while (WiFi.status() != WL_CONNECTED) 
{
delay(500);
Serial.print(".");
}
Serial.println("");
Serial.println("WiFi connected");
 
}

void _relay()
{
  if(value == 1){
  digitalWrite(relay, LOW);
  output = relayon;
  Serial.println("Current Flowing");
  //delay(5000);
}
  if((value > 1)){
  digitalWrite(relay, HIGH);
  output = relayoff;
  Serial.println("Current not Flowing");
  //delay(5000);
}
}

void loop() 
{
_relay();
value = analogRead(sensor);
value = constrain(value, 400, 1023); // keep the range 400 to 1023
value = map(value, 400, 1023, 100, 0); // mapping the rrange to 0-100
if (client.connect(server,80)) {
String postStr = apiKey;
postStr +="&field1=";
postStr += String(output);// add the value to send to things peak
postStr += "\r\n\r\n";
 
client.print("POST /update HTTP/1.1\n");
client.print("Host: api.thingspeak.com\n");
client.print("Connection: close\n");
client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
client.print("Content-Type: application/x-www-form-urlencoded\n");
client.print("Content-Length: ");
client.print(postStr.length());
client.print("\n\n");
client.print(postStr);
 
//display
Serial.println("Soil mositure is:");
Serial.println(output);Serial.println(value);
Serial.println("Sending data to Thingspeak");
}
client.stop();
 
Serial.println("Waiting 20 secs");
// thingspeak needs at least a 15 sec delay between updates
// 10 seconds to be safe
delay(1000);
}
