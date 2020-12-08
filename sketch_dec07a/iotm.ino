String webHead="<html>\n    <head>\n        <meta name='viewport' content='width=device-width, initial-scale=1.0'/>\n        <meta http-equiv='Content-Type' content='text/html;charset=utf-8' />\n        <style>\n            .button {\n                border: none;\n                color: white;\n                padding: 20px;\n                text-align: center;\n                text-decoration: none;\n                display: inline-block;\n                font-size: 16px;\n                margin: 4px 2px;\n                cursor: pointer;\n            }\n            .button-on {border-radius: 100%; background-color: #4CAF50;}\n            .button-off {border-radius: 100%;background-color: #707070;}\n        </style>\n    </head>\n    <body>\n      <h1>밥잘주는 웹사이트</h1>";
String webTail="\n<p><a href='/wifi'>네트웍공유기변경-누른후 와이파설정부터 다시하세요</a></p></body>\n</html>";

void handleRoot() {
  String s=""; 

  if(ledOn==1) 
    s=s+"<a href=\"off\"> <button id=\"button01\" class=\"button button-on\" >밥준다</button></a>";
  else 
    s=s+"<a href=\"on\"> <button id=\"button01\" class=\"button button-off\" >밥 안준다</button></a>";

    s+="<form action='/Mon'>";
  if(Moteron==1)
    s=s+"<button type= 'submit' name='button' value='0' class='button button-on' >추가기능버튼</button></a>";
  else
    s=s+"<button type= 'submit' name='button' value='0' class='button button-on' >추가기능버튼</button></a>";
  s+="</form>";
  
  s=s+"<br><br>AP & IP :&emsp;"+sChipId+"&emsp;"+WiFi.localIP().toString();
  server.send(200, "text/html", webHead+s+webTail);
}

void handleWifi() {
  WiFiManager wm;
  wm.resetSettings();
  wm.resetSettings();
  ESP.reset();
}

void handleOn() {
  ledOn=1;
  //digitalWrite(LED, 0);
  s = "\0";
  for(pos = 0; pos < 90; pos += 1)
  {
    servome.write(pos);
  }
  Serial.println(" ");
  Serial.println(s); 
  Serial1.print(s);
  GoHome();
}

void handleOff() {
  ledOn=0;
  //digitalWrite(LED, 1);
  s = "\0";
  for(pos = 90; pos >= 1; pos -=1)
  {
    servome.write(pos);
  }
  Serial.println(" ");
  Serial.println(s); 
  Serial1.print(s);
  GoHome();
}

void handleMON(){
  int no= server.arg("button").toInt();
  if(Moteron==1)
    Moteron=1;
  else
    Moteron=0;
    
   s = "\0";
  for(i = 0; i<3; i++){
  for(pos = 0; pos < 90; pos += 1)
  {
    servome.write(pos);
  }
  delay(2000);
  for(pos = 90; pos >= 1; pos-=1 )
  {
    servome.write(pos);
  }
  delay(2000);
  }
  for(pos = 0; pos <90; pos +=1)
  {
    servome.write(pos);
  }
  Serial.println(" ");
  Serial.println(s); 
  Serial1.print(s);
  GoHome();
}


void GoHome() {
  String s,ipS;
  //IPAddress ip;
  ipS=toStringIp(WiFi.localIP());
  s="<meta http-equiv='refresh' content=\"0;url='http://"+ipS+"/'\">";
  server.send(200, "text/html", s);
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}
