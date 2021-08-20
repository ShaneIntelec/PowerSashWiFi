#include <Arduino.h>

#include <WiFi.h>
#include <WiFiClient.h>
#include <ESP_WiFiManager.h>              //https://github.com/khoih-prog/ESP_WiFiManager
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"

#include <ArduinoJson.h>


//RX input from door with 3.3k and 1.8k divider on gpio17
//Tx to gpio2

#define TXDOOR 2
#define RXDOOR 17

#define ACCESSPOINT

const char* ssid = "SashSetup";
const char* password = "";

//#define DEBUGJSONDATA
#define DEBUGNONJSONDATA

AsyncWebServer server(80);

//nv data variables
int top, bot, sto, sbo, sld, jcl, smu, smd, sms, jit, prt, dsp;
int usp, bnt, hco, pco, loo, pho;

//run mode variable
int unl, dpo, prs, drs, pit, str, aux, pir, pce, uno, cua, cub;

StaticJsonDocument<500> door_data;
StaticJsonDocument<500> submitted_data;
bool update_data = false;


void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}


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
  server.on("/index.html", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", "text/html");
  });
  server.on("/eep.html", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/eep.html", "text/html");
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

  server.on("/runData", HTTP_GET, [](AsyncWebServerRequest *request){
    AsyncResponseStream *response = request->beginResponseStream("application/json");
    DynamicJsonDocument doc(1024);
    JsonObject root = doc.to<JsonObject>();
    
    switch(uno) {
      case 0: root["uno"] = "Top Found"; break;
      case 1: root["uno"] = "Bottom Found"; break;
      case 2: root["uno"] = "Velocity Error<br/><font size=\"-1\">(Affected by Stall Max Down)</font>"; break;
      case 3: root["uno"] = "Photo Halt"; break;
      case 4: root["uno"] = "Overcurrent"; break;
      case 5: root["uno"] = "Raise Jammed<br/><font size=\"-1\">(Affected by Jitter,Stall Max Up)</font>"; break;
      case 6: root["uno"] = "PIR Halt"; break;
      case 7: root["uno"] = "Underspeed"; break;
      case 8: root["uno"] = "Photo jammed"; break;
    }
    
    root["str"] = str;
    
    switch(dpo) {
      case 0: root["dpo"] = "At Top"; break;
      case 1: root["dpo"] = "Near Top"; break;
      case 2: root["dpo"] = "Top Half"; break;
      case 3: root["dpo"] = "Bottom Half"; break;
      case 4: root["dpo"] = "Near Bottom"; break;
      case 5: root["dpo"] = "At Bottom"; break;
      case 6: root["dpo"] = "Unknown"; break;
    }
  
    switch(prs) {
      case 0: root["prs"] = "Normal"; break;
      case 1: root["prs"] = "Set Bottom"; break;
      case 2: root["prs"] = "Set Top"; break;
    }

    switch(drs) {
      case 0: root["drs"] = "Off"; break;
      case 4: root["drs"] = "Free"; break;
      case 8: root["drs"] = "Error"; break;
      default: root["drs"] = "Fix this"; break;
    }
    root["pit"] = pit;
    root["cua"] = cua;
    root["cub"] = cub;
    root["pir"] = pir;
    root["aux"] = aux;
    root["pce"] = pce;
    if (pco)
      root["pco"] = "<button class=\"btn btn-primary\">Lower Door</button>";
    else 
      root["pco"] = "<span class=\"alert alert-warning\">Lower Door Function Disabled</span>";

    serializeJson(root, *response);
    request->send(response);
  });

  server.on("/eepData", HTTP_GET, [](AsyncWebServerRequest *request){
    AsyncResponseStream *response = request->beginResponseStream("application/json");
    DynamicJsonDocument doc(1024);
    JsonObject root = doc.to<JsonObject>();
    
    root["top"] = top;
    root["bot"] = bot;
    root["str"] = str;
    
    switch(uno) {
      case 0: root["uno"] = "Top Found"; break;
      case 1: root["uno"] = "Bottom Found"; break;
      case 2: root["uno"] = "Velocity Error<br/><font size=\"-1\">(Affected by Stall Max Down)</font>"; break;
      case 3: root["uno"] = "Photo Halt"; break;
      case 4: root["uno"] = "Overcurrent"; break;
      case 5: root["uno"] = "Raise Jammed<br/><font size=\"-1\">(Affected by Jitter,Stall Max Up)</font>"; break;
      case 6: root["uno"] = "PIR Halt"; break;
      case 7: root["uno"] = "Underspeed"; break;
      case 8: root["uno"] = "Photo jammed"; break;
    }

    switch(dpo) {
      case 0: root["dpo"] = "At Top"; break;
      case 1: root["dpo"] = "Near Top"; break;
      case 2: root["dpo"] = "Top Half"; break;
      case 3: root["dpo"] = "Bottom Half"; break;
      case 4: root["dpo"] = "Near Bottom"; break;
      case 5: root["dpo"] = "At Bottom"; break;
      case 6: root["dpo"] = "Unknown"; break;
    }
  
    switch(prs) {
      case 0: root["prs"] = "Normal"; break;
      case 1: root["prs"] = "Set Bottom"; break;
      case 2: root["prs"] = "Set Top"; break;
    }

    root["pir"] = pir ? "<i class=\"fa fa-tag text-success\"></i>" : "";
    root["aux"] = aux ? "<i class=\"fa fa-tag text-success\"></i>" : "";
    root["unl"] = unl ? "<i class=\"fa fa-tag text-success\"></i>" : "";

    root["pit"] = pit;
    root["usp"] = usp;
    root["dsp"] = dsp;
    root["sto"] = sto;
    root["sbo"] = sbo;
    root["sld"] = sld;
    root["jcl"] = jcl;
    root["smu"] = smu;
    root["smd"] = smd;
    root["pce"] = pce / 20;
    root["sms"] = sms;
    root["jit"] = jit;
    root["prt"] = prt;
    root["bnt"] = bnt * 50;
    root["pho"] = pho ? "Enabled": "Disabled";
    root["hco"] = hco ? "Enabled": "Disabled";
    root["pco"] = pco ? "Enabled": "Disabled";
    root["loo"] = loo ? "Enabled": "Disabled";
    serializeJson(root, *response);
    request->send(response);
    if (unl == 0) {
      Serial2.print("!!!");  // auto unlock
    }
  });

  server.on("/set", HTTP_GET, [](AsyncWebServerRequest *request){
    String p = request->arg("param");
    Serial.print(F("Received `set` request, param: ")); Serial.println(p);
    if (p == "pirtoggle"){
      Serial2.print("r");   // nv_photoHalt
    } else if (p == "hco") {
      Serial2.print("a");   // nv_pirHaltCloseOption
    } else if (p == "pco") {
      Serial2.print("y");   // nv_pushCloseOption
    } else if (p == "loo") {
      Serial2.print("l");   // nv_liftOpenOption
    } else if (p == "raise") {
      Serial2.print(">");
    } else if (p == "lower") {
      Serial2.print("<");
    }
    request->send(200, "text/plain", "OK");
  });

  server.onRequestBody([](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total){
    if ((request->url() == "/submit") && (request->method() == HTTP_POST)) {
      if (DeserializationError::Ok == deserializeJson(submitted_data, (const char*)data)){
        update_data = true;
      }
      request->send(200, "text/plain", "OK");
    }
  });

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


void handle_param(int16_t cur_val, int16_t new_val, String down_val, String up_val){
  if (cur_val > new_val){
    for (uint16_t i = 0; i < cur_val - new_val; i++){
      Serial2.print(down_val);
      delay(5);
    }
  } else if (cur_val < new_val){
    for (uint16_t i = 0; i < new_val - cur_val; i++){
      Serial2.print(down_val);
      delay(5);
    }
  }
}


void loop(void){

  String rawDoorData;

  if (update_data){
    update_data = false;
    handle_param(usp, submitted_data["usp"], "u", "U");
    handle_param(usp, submitted_data["dsp"], "d", "D");
    handle_param(usp, submitted_data["sto"], "t", "T");
    handle_param(usp, submitted_data["sbo"], "b", "B");
    handle_param(usp, submitted_data["sld"], "x", "X");
    handle_param(usp, submitted_data["jcl"], "c", "C");
    handle_param(usp, submitted_data["smu"], "z", "Z");
    handle_param(usp, submitted_data["smd"], "s", "S");
    handle_param(usp, submitted_data["sms"], "m", "m");
    handle_param(usp, submitted_data["jit"], "j", "J");
    handle_param(usp, submitted_data["prt"], "p", "P");
    handle_param(usp, submitted_data["bnt"], "o", "O");
  }

  while (Serial2.available()) {
    rawDoorData = Serial2.readString();
    DeserializationError error = deserializeJson(door_data, rawDoorData);

    if (error) {
#ifdef DEBUGNONJSONDATA
      Serial.print(F("deserializeJson() failed: "));  Serial.println(error.f_str());
      Serial.print("NJ="); Serial.println(rawDoorData);
#endif
      break;
    }
    
    if (door_data["type"] == 1) {
      #ifdef DEBUGJSONDATA
      Serial.print("RD="); Serial.println(rawDoorData);
      #endif
      
      unl = door_data["unl"];
      dpo = door_data["dpo"];
      prs = door_data["prs"];
      drs = door_data["drs"];
      pit = door_data["pit"];
      str = door_data["str"];
      aux = door_data["aux"];
      pir = door_data["pir"];
      pce = door_data["pce"];
      uno = door_data["uno"];
      cua = door_data["cua"];
      cub = door_data["cub"];
    } else if (door_data["type"] == 2) {
      #ifdef DEBUGJSONDATA
      Serial.print("EE="); Serial.println(rawDoorData);
      #endif
      
      top = door_data["top"];
      bot = door_data["bot"];
      sto = door_data["sto"]; // 20
      sbo = door_data["sbo"]; // 20
      sld = door_data["sld"]; // 10
      jcl = door_data["jcl"]; // 200
      smu = door_data["smu"]; // 2
      smd = door_data["smd"]; // 47
      sms = door_data["sms"]; // 2
      jit = door_data["jit"]; // 0
      prt = door_data["prt"]; // 60
      dsp = door_data["dsp"]; // 10
      usp = door_data["usp"]; // 10
      bnt = door_data["bnt"]; // 10
      hco = door_data["hco"]; // 0
      pco = door_data["pco"]; // 0
      loo = door_data["loo"]; // 0
      pho = door_data["pho"]; // 0
    }
  }
  delay(1);
}
