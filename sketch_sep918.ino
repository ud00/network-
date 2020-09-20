//softAP test ok read
//WiFi 설정 AI-TINKER_xxxxxx 연결
//192.168.4.1/read
//192.168.4.1/led/1 ;on
//192.168.4.1/led/0 ;off

#include <ESP8266WiFi.h>

//////////////////////
// WiFi Definitions //
//////////////////////
const char WiFiAPPSK[] = "ydy";
//const char* ssid = ""; // wifi 이름
//const char* password = "";//패스워드

/////////////////////
// Pin Definitions //
/////////////////////
const int ledPin = D4; // Thing's onboard, green LED
const int adcPin = A0; // The only analog pin on the Thing
const int swPin = D2; // Digital pin to be read
const int pwrPin = D5; //analog power source

WiFiServer server(80);

void setup() 
{
  initHardware();
  setupWiFi();
  server.begin();
}

void loop() 
{
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();

  // Match the request
  int val = -1; // We'll use 'val' to keep track of both the
                // request type (read/set) and value if set.
  if (req.indexOf("/led/0") != -1)
    val = 0; // Will write LED low
  else if (req.indexOf("/led/1") != -1)
    val = 1; // Will write LED high
  else if (req.indexOf("/read") != -1)
    val = -2; // Will print pin reads
  // Otherwise request will be invalid. We'll say as much in HTML

  // Set GPIO2/D4 according to the request
  if (val >= 0)
    digitalWrite(ledPin, val);

  client.flush();

  // Prepare the response. Start with the common header:
  String s = "HTTP/1.1 200 OK\r\n";
  s += "Content-Type: text/html\r\n\r\n";
  s += "<!DOCTYPE HTML>\r\n<html>\r\n";

  /*Note: Uncomment the line below to refresh automatically
   *      for every 1 second. This is not ideal for large pages 
   *      but for a simple read out, it is useful for monitoring 
   *      your sensors and I/O pins. To adjust the fresh rate, 
   *      adjust the value for content. For 30 seconds, simply 
   *      change the value to 30.*/
  s += "<meta http-equiv='refresh' content='1'/>\r\n";//auto refresh page
//*
  s += "<head><meta name='viewport' content='width=device-width, initial-scale=1'>\r\n";
  s += "<link rel='icon' href='data:,'>";
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
  s += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}";
  s += ".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;";
  s += "text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}";
  s += ".button2 {background-color: #77878A;}</style></head>";
//*/  
// Web Page Heading
            s += "<body><h1>ESP8266 softAP</h1>";
  // If we're setting the LED, print out a message saying we did
  if (val >= 0)
  {
    s += "<h4>LED is now ";
    s += (val)?"on":"off</h4>";
    if(val == 1){
      s += "<p><a href='/led/1'><button class='button'>ON</button></a></p>";
    } else { 
      s += "<p><a href='/led/0'><button class='button button2'>OFF</button></a></p>"; 
    }
  }
  else if (val == -2)
  { // If we're reading pins, print out those values:
    s += "Analog Pin = ";
    s += String(analogRead(adcPin));
    s += "<br>"; // Go to the next line.
    s += "Digital Pin D2 = ";
    s += String(digitalRead(swPin));
  }
  else
  {
    s += "Invalid Request.<br> Try /led/1, /led/0, or /read.";
  }
  s += "</body></html>\n";

  // Send the response to the client
  client.print(s);
  delay(1);
  Serial.println("Client disonnected");

  // The client will actually be disconnected 
  // when the function returns and 'client' object is detroyed
}

void setupWiFi()
{
  WiFi.mode(WIFI_AP); //softAP
  //AI-THINKER_DA3BD9
  // Do a little work to get a unique-ish name. Append the
  // last two bytes of the MAC (HEX'd) to "Thing-":
  uint8_t mac[WL_MAC_ADDR_LENGTH];
  WiFi.softAPmacAddress(mac);
  String macID = String(mac[WL_MAC_ADDR_LENGTH - 2], HEX) +
                 String(mac[WL_MAC_ADDR_LENGTH - 1], HEX);
  macID.toUpperCase();
  String AP_NameString = "ESP8266 Thing " + macID;

  char AP_NameChar[AP_NameString.length() + 1];
  memset(AP_NameChar, 0, AP_NameString.length() + 1);

  for (int i=0; i<AP_NameString.length(); i++)
    AP_NameChar[i] = AP_NameString.charAt(i);

  WiFi.softAP(AP_NameChar, WiFiAPPSK);
}

void initHardware()
{
  Serial.begin(115200);
  pinMode(swPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  pinMode(pwrPin, OUTPUT);
  digitalWrite(pwrPin, HIGH);
  digitalWrite(ledPin, LOW);
  // Don't need to set ANALOG_PIN as input, 
}
