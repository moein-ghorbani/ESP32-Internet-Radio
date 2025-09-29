#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <SPIFFS.h>
#include "AudioFileSourceICYStream.h"
#include "AudioFileSourceBuffer.h"
#include "AudioGeneratorMP3.h"
#include "AudioOutputI2S.h"
#include <TFT_eSPI.h>

const char* ssid = "SSHP";
const char* password = "13661368";

TFT_eSPI tft = TFT_eSPI();

AudioGeneratorMP3 *mp3;
AudioFileSourceICYStream *file;
AudioFileSourceBuffer *buff;
AudioOutputI2S *out;

WebServer server(80);

String currentURL = "http://streams.rsa-sachsen.de/80erhits/mp3-192/streams.rsa-sachsen.de";
const char* configFile = "/radio_url.txt";

void setupTFT() {
  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextDatum(MC_DATUM);
  pinMode(15, OUTPUT);
  digitalWrite(15, HIGH);
  Serial.println("TFT initialized");
}

void drawScreen(String status, String info = "") {
  tft.fillScreen(TFT_BLACK);
  int centerX = 120;
  
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.setFreeFont(&FreeSansBold12pt7b);
  tft.drawString("RADIO", centerX, 40, 1);
  
  tft.setFreeFont(&FreeSans12pt7b);
  if (status == "PLAYING") {
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.drawString("PLAYING", centerX, 80, 1);
  } else if (status == "ERROR") {
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.drawString("ERROR", centerX, 80, 1);
  } else {
    tft.setTextColor(TFT_ORANGE, TFT_BLACK);
    tft.drawString(status, centerX, 80, 1);
  }
  
  if (status == "PLAYING") {
    tft.fillCircle(centerX, 130, 30, TFT_GREEN);
    tft.fillTriangle(centerX-12, 115, centerX-12, 145, centerX+12, 130, TFT_BLACK);
  } else {
    tft.fillCircle(centerX, 130, 30, TFT_RED);
    tft.setTextColor(TFT_WHITE, TFT_RED);
    tft.setFreeFont(&FreeSans9pt7b);
    tft.drawString("STOP", centerX, 128, 1);
  }
  
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setFreeFont(&FreeSans9pt7b);
  
  if (WiFi.status() == WL_CONNECTED) {
    tft.drawString("WiFi: OK", centerX, 180, 1);
  } else {
    tft.drawString("WiFi: OFF", centerX, 180, 1);
  }
  
  if (!info.isEmpty()) {
    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    tft.drawString(info, centerX, 200, 1);
  }
}

void saveURLToFlash(String url) {
  File file = SPIFFS.open(configFile, "w");
  if (file) {
    file.print(url);
    file.close();
  }
}

String readURLFromFlash() {
  if (SPIFFS.exists(configFile)) {
    File file = SPIFFS.open(configFile, "r");
    String url = file.readString();
    file.close();
    return url;
  }
  return currentURL;
}

void handleRoot() {
  String html = "<!DOCTYPE html><html><head><meta name='viewport' content='width=device-width, initial-scale=1'>";
  html += "<title>Radio Config</title>";
  html += "<style>body{font-family:Arial;margin:20px;background:#f0f0f0;}";
  html += ".container{background:white;padding:20px;border-radius:10px;}";
  html += "input[type=text]{width:100%;padding:10px;margin:10px 0;}";
  html += "button{background:#4CAF50;color:white;padding:10px 20px;border:none;border-radius:5px;cursor:pointer;}";
  html += "</style></head><body>";
  html += "<div class='container'>";
  html += "<h1>Radio Configuration</h1>";
  html += "<form action='/save' method='POST'>";
  html += "<label>Stream URL:</label><br>";
  html += "<input type='text' name='url' value='" + currentURL + "'><br><br>";
  html += "<button type='submit'>Save & Restart</button>";
  html += "</form><br>";
  html += "<div><strong>Status:</strong><br>";
  html += "Radio: " + String(mp3->isRunning() ? "PLAYING" : "STOPPED") + "<br>";
  html += "WiFi: " + String(WiFi.status() == WL_CONNECTED ? "Connected" : "Disconnected") + "<br>";
  html += "IP: " + WiFi.localIP().toString() + "</div>";
  html += "</div></body></html>";
  server.send(200, "text/html", html);
}

void handleSave() {
  if (server.hasArg("url")) {
    currentURL = server.arg("url");
    saveURLToFlash(currentURL);
    String html = "<!DOCTYPE html><html><body style='font-family:Arial;margin:40px;'>";
    html += "<div style='background:#d4edda;padding:20px;border-radius:10px;'>";
    html += "<h2>Settings Saved!</h2><p>Restarting radio...</p></div>";
    html += "<script>setTimeout(()=>window.location.href='/',2000);</script>";
    html += "</body></html>";
    server.send(200, "text/html", html);
    delay(1000);
    restartPlayer();
  }
}

void connectToWiFi() {
  drawScreen("WiFi", "Connecting...");
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  for(int i=0; i<20; i++) {
    if(WiFi.status() == WL_CONNECTED) break;
    delay(500);
  }
  if(WiFi.status() == WL_CONNECTED) {
    server.on("/", handleRoot);
    server.on("/save", HTTP_POST, handleSave);
    server.begin();
    drawScreen("READY", WiFi.localIP().toString());
  } else {
    drawScreen("ERROR", "WiFi Failed");
  }
}

void restartPlayer() {
  drawScreen("RESTART", "Please wait...");
  if(mp3) { mp3->stop(); delete mp3; }
  if(buff) delete buff;
  if(file) delete file;
  file = new AudioFileSourceICYStream(currentURL.c_str());
  buff = new AudioFileSourceBuffer(file, 4096);
  mp3 = new AudioGeneratorMP3();
  if(mp3->begin(buff, out)) {
    drawScreen("PLAYING", "Stream started");
  } else {
    drawScreen("ERROR", "Start failed");
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  SPIFFS.begin(true);
  currentURL = readURLFromFlash();
  setupTFT();
  connectToWiFi();
  out = new AudioOutputI2S(0, 1); 
  out->SetGain(0.3);
  file = new AudioFileSourceICYStream(currentURL.c_str());
  buff = new AudioFileSourceBuffer(file, 4096);
  mp3 = new AudioGeneratorMP3();
  if(mp3->begin(buff, out)) {
    drawScreen("PLAYING", "Radio started");
  } else {
    drawScreen("ERROR", "Init failed");
  }
}

void loop() {
  server.handleClient();
  static unsigned long lastCheck = 0;
  if(millis() - lastCheck > 5000) {
    if(WiFi.status() != WL_CONNECTED) {
      drawScreen("ERROR", "WiFi lost");
      WiFi.reconnect();
    }
    lastCheck = millis();
  }
  if(mp3->isRunning()) {
    if(!mp3->loop()) {
      restartPlayer();
    }
  }
  delay(10);
}