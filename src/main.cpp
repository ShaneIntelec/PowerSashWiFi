#include <Arduino.h>

#include <WiFi.h>
#include <WiFiClient.h>
#include <ESP_WiFiManager.h>              //https://github.com/khoih-prog/ESP_WiFiManager
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"

#include <ArduinoJson.h>
#include "index.h"
#include "eep.h"
#include "css.h"


//RX input from door with 3.3k and 1.8k divider on gpio17
//Tx to gpio2

#define TXDOOR 2
#define RXDOOR 17

#define ACCESSPOINT

const char* ssid = "SashSetup";
const char* password = "";

//#define DEBUGJSONDATA
#define DEBUGNONJSONDATA

AsyncWebServer  server(80);

bool jamclear, pushc, piren, pirhc;

//nv data variables
int top, bot, sto, sbo, sld, jcl, smu, smd, sms, jit, prt , dsp;
int usp, bnt, hco, pco, loo, pho;

//run mode variable
int unl,dpo,prs, drs , pit, str, aux, pir, pce, uno, cua, cub;


void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

/* 
void handlePIR() {
//  pho = (pho)? 0:1;
  Serial2.print("r"); //nv_photoHalt
  handleEEP();
}

void handlePCO() {
  Serial2.print("y");  //nv_pushCloseOption
  handleEEP();
}

void handleLOO() {
  Serial2.print("l");  //nv_liftOpenOption
  handleEEP();
}

void handleHCO() {
//  Serial2.print("!!!");   //toggle nv_pirHaltCloseOption
//  delay(100);
  Serial2.print("a");   //toggle nv_pirHaltCloseOption
  handleEEP();
}

void handleRaise() {
  Serial2.print(">");
  handleEEP();
//  handleRoot();
}

void handleLower() {
  Serial2.print("<");
  handleEEP();
//  handleRoot();
}

void handleSmddn() {Serial2.print("S"); handleEEP();}
void handleSmdup() {Serial2.print("s"); handleEEP();}
void handleSmudn() {Serial2.print("z"); handleEEP();}
void handleSmuup() {Serial2.print("Z"); handleEEP();}
void handleJCLdn() {Serial2.print("c"); handleEEP();}
void handleJCLup() {Serial2.print("C"); handleEEP();}
void handleUSPdn() {Serial2.print("u"); handleEEP();}
void handleUSPup() {Serial2.print("U"); handleEEP();}
void handleDSPdn() {Serial2.print("d"); handleEEP();}
void handleDSPup() {Serial2.print("D"); handleEEP();}
void handleSTOdn() {Serial2.print("t"); handleEEP();}
void handleSTOup() {Serial2.print("T"); handleEEP();}
void handleSBOdn() {Serial2.print("b"); handleEEP();}
void handleSBOup() {Serial2.print("B"); handleEEP();}
void handleSLDdn() {Serial2.print("x"); handleEEP();}
void handleSLDup() {Serial2.print("X"); handleEEP();}
void handleSMSdn() {Serial2.print("m"); handleEEP();}
void handleSMSup() {Serial2.print("M"); handleEEP();}
void handleJITdn() {Serial2.print("j"); handleEEP();}
void handleJITup() {Serial2.print("J"); handleEEP();}
void handlePRTdn() {Serial2.print("p"); handleEEP();}
void handlePRTup() {Serial2.print("P"); handleEEP();}
void handleBNTdn() {Serial2.print("o"); handleEEP();}
void handleBNTup() {Serial2.print("O"); handleEEP();}

void handleJAM() {
  jamclear = (jamclear)? 0:1;
  handleEEP();
}

void handleRunData() {

  String dataString = "<p>";

  dataString += "<br/>Status: ";
  switch(uno)
  {
    case 0:dataString += "Top Found"; break;
    case 1:dataString += "Bottom Found"; break;
    case 2:dataString += "Velocity Error<br/><font size=\"-1\">(Affected by Stall Max Down)</font>"; break;
    case 3:dataString += "Photo Halt"; break;
    case 4:dataString += "Overcurrent"; break;
    case 5:dataString += "Raise Jammed<br/><font size=\"-1\">(Affected by Jitter,Stall Max Up)</font>"; break;
    case 6:dataString += "PIR Halt"; break;
    case 7:dataString += "Underspeed"; break;
    case 8:dataString += "Photo jammed"; break;
  }
  dataString += "<br/>";

  dataString += "<br/>String Position:" + (String)str;
  
  dataString += "<br/>Sash: ";
  switch(dpo)
  {
    case 0:dataString += "At Top"; break;
    case 1:dataString += "Near Top"; break;
    case 2:dataString += "Top Half"; break;
    case 3:dataString += "Bottom Half"; break;
    case 4:dataString += "Near Bottom"; break;
    case 5:dataString += "At Bottom"; break;
    case 6:dataString += "Unknown"; break;
  }
//    dataString += "<br/>Program State:" + (String)prs;
  dataString += "<br/>Button State: ";
  switch(prs)
  {
    case 0:dataString += "Normal"; break;
    case 1:dataString += "Set Bottom"; break;
    case 2:dataString += "Set Top"; break;
  }
//    dataString += "<br/>Drive State:" + (String)drs;
  dataString += "<br/>Drive State: ";
  switch(drs)
  {
    case 0:dataString += "Off"; break;
    case 4:dataString += "Free"; break;
    case 8:dataString += "Error"; break;
    default:dataString += "fix this"; break;
  }
  dataString += "<br/>PIR Timer: " + (String)pit;
  dataString += "<br/>Current A: " + (String)cua + "mA" ;
  dataString += "<br/>Current B: " + (String)cub + "mA" ;
  dataString += "<br/>PIR Input: " + (String)pir;
  dataString += "<br/>AUX Input: " + (String)aux;
  dataString += "<br/>Pace: " + (String)pce;
  
  dataString += "</p>";

  dataString += "<p><strong>Raise Door</strong><a href=\"/raisedoor\"><button class=\"button\">UP</button></a></p>";

  if (pco)
    dataString += "<p><strong>Lower Door</strong><a href=\"/lowerdoor\"><button class=\"button\">DOWN</button></a></p>";
  else 
    dataString += "<p><br>Lower Door function disabled while Push Close option disabled</p>";

  server.send(200, "text/plain", dataString);
}

void handleEEPData() {

  String eepString = "<table>";
  eepString += "<tr><td>" + (String)top + "</td></tr>";
  eepString += "<tr><td>" + (String)bot + "</td></tr>";
  eepString += "<tr><td>" + (String)str + "</td></tr>";

  eepString += "<tr><td>";
  switch(dpo)
  {
    case 0:eepString += "At Top"; break;
    case 1:eepString += "Near Top"; break;
    case 2:eepString += "Top Half"; break;
    case 3:eepString += "Bottom Half"; break;
    case 4:eepString += "Near Bottom"; break;
    case 5:eepString += "At Bottom"; break;
    case 6:eepString += "Unknown"; break;
  }
  eepString += "</tr></td>";

  eepString += "<tr><td>";
  switch(uno)
  {
    case 0:eepString += "Top Found"; break;
    case 1:eepString += "Bottom Found"; break;
    case 2:eepString += "Velocity Error<br/><font size=\"-1\">(Adjust Down Timing)</font>"; break;
    case 3:eepString += "Beam Sensor Halt"; break;
    case 4:eepString += "Overcurrent Detected"; break;
    case 5:eepString += "Raise Jammed<br/><font size=\"-1\">(Adjust Min Up Speed)</font>"; break;
    case 6:eepString += "PIR Halt"; break;
    case 7:eepString += "Underspeed"; break;
    case 8:eepString += "Beam Sensor Covered"; break;
  }
  eepString += "</tr></td>";

  eepString += "<tr><td>";
  switch(prs)
  {
    case 0:eepString += "Normal"; break;
    case 1:eepString += "Set Bottom"; break;
    case 2:eepString += "Set Top"; break;
  }
  eepString += "</tr></td>";
  
  
  if (pir) eepString += "<tr><td><div class=\"led-green\"></div></td></tr>";
  else eepString += "<tr><td></td></tr>";

  if (aux) eepString += "<tr><td><div class=\"led-green\"></div></td></tr>";
  else eepString += "<tr><td></td></tr>";

  if (unl) eepString += "<tr><td><div class=\"led-green\"></div></td></tr>";
  else eepString += "<tr><td></td></tr>";
  
  eepString += "<tr><td>" + (String)pit + "</td></tr>";
  eepString += "<tr><td>" + (String)usp + "<font size=\"-1\"> mS/Step</font></td></tr>";
  eepString += "<tr><td>" + (String)dsp + "<font size=\"-1\"> mS/Step</font></td></tr>";
  eepString += "<tr><td>" + (String)sto + "</td></tr>";
  eepString += "<tr><td>" + (String)sbo + "</td></tr>";
  eepString += "<tr><td>" + (String)sld + "</td></tr>";
//    eepString += "<tr><td>" + (String)jcl + "</td></tr>";
  eepString += "<tr><td>" + (String)jcl + "<font size=\"-1\"> Steps</font></td></tr>";
  
  eepString += "<tr><td>" + (String)smu + "</td></tr>";
  eepString += "<tr><td>" + (String)smd + "<font size=\"-1\">Timed:" + (String)(pce/20) + "</font></td></tr>";
  
  eepString += "<tr><td>" + (String)sms + "<font size=\"-1\"> (String)</font></td></tr>";
  eepString += "<tr><td>" + (String)jit + "<font size=\"-1\"> (String)</font></td></tr>";
  eepString += "<tr><td>" + (String)prt + "<font size=\"-1\"> Seconds</font></td></tr>";
  eepString += "<tr><td>" + (String)(bnt*50) + "<font size=\"-1\"> mS</font></td></tr>";
  
  eepString += "<tr><td><a href=\"/pirtoggle\"><button class=\"button\">";
  if (pho) eepString += "Enabled";
  else eepString += "Disabled";
  eepString += "</button></a></td></tr>";

  eepString += "<tr><td><a href=\"/hco\"><button class=\"button\">";
  if (hco) eepString += "Enabled";
  else eepString += "Disabled";
  eepString += "</button></a></td></tr>";
  
  eepString += "<tr><td><a href=\"/pco\"><button class=\"button\">";
  if (pco) eepString += "Enabled";
  else eepString += "Disabled";
  eepString += "</button></a></td></tr></table>";

  eepString += "<tr><td><a href=\"/loo\"><button class=\"button\">";
  if (loo) eepString += "Enabled";
  else eepString += "Disabled";
  eepString += "</button></a></td></tr></table>";
  
  server.send(200, "text/plain", eepString);
//    handleCSS();

  if (unl == 0) {
    Serial2.print("!!!");  // auto unlock
  }

}

*/

void setup(void){

  pinMode(RXDOOR, INPUT);
  pinMode(TXDOOR, OUTPUT);

  if(!SPIFFS.begin(true)){
     Serial.println("An Error has occurred while mounting SPIFFS");
     return;
}

  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, RXDOOR, TXDOOR);
  Serial2.setTimeout(100);

#ifdef ACCESSPOINT
  WiFi.mode(WIFI_AP); //Access Point mode
  WiFi.softAP(ssid, password);
#else
  Serial.print(F("\nStarting AutoConnect_ESP32_minimal on ")); Serial.println(ARDUINO_BOARD); 
  Serial.println(ESP_WIFIMANAGER_VERSION);
  ESP_WiFiManager ESP_wifiManager("AutoConnectAP");
  ESP_wifiManager.autoConnect("AutoConnectAP");
  if (WiFi.status() == WL_CONNECTED) { Serial.print(F("Connected. Local IP: "));Serial.println(WiFi.localIP()); }
  else { Serial.println(ESP_wifiManager.getStatus(WiFi.status())); }
#endif

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", "text/html");
  });
  server.on("/src/bootstrap.bundle.min.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/src/bootstrap.bundle.min.js", "text/javascript");
  });
  
  server.on("/src/jquery-3.6.0.min.js", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(SPIFFS, "/src/jquery-3.6.0.min.js", "text/javascript");
  });
  
  server.on("/src/bootstrap.min.css", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(SPIFFS, "/src/bootstrap.min.css", "text/css");
  });

  // server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
  //   request->send_P(200, "text/html", MAIN_page);
  // });
  // server.on("/eep", HTTP_GET, [](AsyncWebServerRequest *request){
  //   request->send_P(200, "text/html", EEP_page);
  // });
  // server.on("/style2.css", HTTP_GET, [](AsyncWebServerRequest *request){
  //   request->send_P(200, "text/html", CSS_page);
  // });

  // server.on("/submit", HTTP_POST, [](AsyncWebServerRequest *request){
  //   String message;
  //   message = request->getParam(PARAM_MESSAGE, true)->value();
  //   request->send(200, "text/plain", "Hello, POST: " + message);
  // });

  // server.on("/runData", handleRunData);   // To get update of run data only 
  // server.on("/eepData", handleEEPData);     // To get update of run data only

  // server.on("/pirtoggle", handlePIR) ;
  // server.on("/pco", handlePCO) ;
  // server.on("/loo", handleLOO) ;
  // server.on("/hco", handleHCO) ;
  // server.on("/raisedoor", handleRaise) ;
  // server.on("/lowerdoor", handleLower) ;
  // server.on("/smdup", handleSmdup) ;
  // server.on("/smddn", handleSmddn) ;
  // server.on("/smudn", handleSmudn);
  // server.on("/smuup", handleSmuup);
  // server.on("/jcldn", handleJCLdn);
  // server.on("/jclup", handleJCLup);
  // server.on("/uspdn", handleUSPdn);
  // server.on("/uspup", handleUSPup);
  // server.on("/dspdn", handleDSPdn);
  // server.on("/dspup", handleDSPup);
  // server.on("/stodn", handleSTOdn);
  // server.on("/stoup", handleSTOup);
  // server.on("/sbodn", handleSBOdn);
  // server.on("/sboup", handleSBOup);
  // server.on("/slddn", handleSLDdn);
  // server.on("/sldup", handleSLDup);
  // server.on("/smsdn", handleSMSdn);
  // server.on("/smsup", handleSMSup);
  // server.on("/jitdn", handleJITdn);
  // server.on("/jitup", handleJITup);
  // server.on("/prtdn", handlePRTdn);
  // server.on("/prtup", handlePRTup);
  // server.on("/bntdn", handleBNTdn);
  // server.on("/bntup", handleBNTup);
 
  server.onNotFound(notFound);
  server.begin();
  Serial.println("Server started");
}


StaticJsonDocument<500> doc;
String rawDoorData;

void loop(void){

  while (Serial2.available()) {
    rawDoorData = Serial2.readString();
    DeserializationError error = deserializeJson(doc, rawDoorData);

    if (error) {
    #ifdef DEBUGNONJSONDATA
      Serial.print("NJ=");
      Serial.println(rawDoorData);
      #endif
//      Serial.print(F("deserializeJson() failed: "));
//      Serial.println(error.f_str());
      return;
    }
    
    if (doc["type"] == 1) {
      #ifdef DEBUGJSONDATA
      Serial.print("RD=");
      Serial.println(rawDoorData);
      #endif
      
      unl = doc["unl"];
      dpo = doc["dpo"];
      prs = doc["prs"];
      drs = doc["drs"];
      pit = doc["pit"];
      str = doc["str"];
      aux = doc["aux"];
      pir = doc["pir"];
      pce = doc["pce"];
      uno = doc["uno"];
      cua = doc["cua"];
      cub = doc["cub"];
    }
    else if (doc["type"] == 2) {
      #ifdef DEBUGJSONDATA
      Serial.print("EE=");
      Serial.println(rawDoorData);
      #endif
      
      top = doc["top"];
      bot = doc["bot"];
      sto = doc["sto"]; // 20
      sbo = doc["sbo"]; // 20
      sld = doc["sld"]; // 10
      jcl = doc["jcl"]; // 200
      smu = doc["smu"]; // 2
      smd = doc["smd"]; // 47
      sms = doc["sms"]; // 2
      jit = doc["jit"]; // 0
      prt = doc["prt"]; // 60
      dsp = doc["dsp"]; // 10
      usp = doc["usp"]; // 10
      bnt = doc["bnt"]; // 10
      hco = doc["hco"]; // 0
      pco = doc["pco"]; // 0
      loo = doc["loo"]; // 0
      pho = doc["pho"]; // 0
    }
  }
  delay(1);
}
