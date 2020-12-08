// 와이파이와 웹서버 연결을 위한 헤더파일
#include <ESP8266WiFi.h> 
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h> 
#include <Servo.h> // 서보모터 헤더파일

//칩 자동생성 필요 변수
String sChipId=""; 
char cChipId[40]="";
ESP8266WebServer server(80); // 서버 80번포트 연결

String s;
int i;
int ledOn=0;
int Moteron=0;
const int servoPin = 5; // 서보모터 5번 핀 연결

int pos = 0;
Servo servome; // 서보모터 변수지정


void setup() {

  pinMode(servoPin, OUTPUT);
  servome.attach(servoPin);
  // put your setup code here, to run once:
  Serial.begin(19200);
  Serial1.begin(19200);
  
  

  // AP 이름 자동으로 만듬 i2r-chipid
  sChipId = "i2r-"+String(ESP.getChipId(),HEX);
  sChipId.toCharArray(cChipId,sChipId.length()+1);
  Serial.println(sChipId);

  //WiFiManager
  WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
  WiFiManager wm;
  // wm.resetSettings(); //reset settings - for testing

  if (!wm.autoConnect(cChipId)) {
    Serial.println("failed to connect and hit timeout");
    //reset and try again, or maybe put it to deep sleep
    ESP.restart();
    delay(1000);
  }

  //if you get here you have connected to the WiFi
  Serial.println("connected... :)");
  // 서버클라이언트 동작
  server.on("/", handleRoot);
  server.on("/on", handleOn);
  server.on("/off", handleOff);
  server.on("/Mon", handleMON);
  server.on("/wifi", handleWifi);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient(); // 핸들클라이언트 루프동작
}
