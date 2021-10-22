#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>

// Replace with your network credentials
const char ssid[]    = "ESP-test";
const char password[] = "123456789";

// Set web server port number to 80
ESP8266WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

IPAddress local_IP(10,1,1,2);
IPAddress gateway(10,1,1,1);
IPAddress subnet(255,255,255,0);

// Current time
//unsigned long currentTime, previousTime, interval = 1000;


bool builtin = false;
bool pulse = false;

#include "Pages.h"

void setup() {
  Serial.begin(115200);
  LittleFS.begin();
  delay(1000);
  //check_eeprom_for_user_settings();
  pinMode(LED_BUILTIN, OUTPUT);
  // Set outputs to LOW
  digitalWrite(LED_BUILTIN, HIGH);

  delay(500);
  // Connect to Wi-Fi network with SSID and password
  Serial.print("Setting soft-AP configuration ... ");
  Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");

  Serial.print("Setting soft-AP ... ");
  Serial.println(WiFi.softAP(ssid, password) ? "Ready" : "Failed!");
  
  Serial.print("Soft-AP IP address = ");
  Serial.println(WiFi.softAPIP());

  server.onNotFound(show_page_404);

  server.on("/", show_page_home);
  server.on("/lighting", show_page_lighting);
  server.on("/admin", show_page_admin );
  server.on("/lighting/manual", show_page_lighting_manual);
  server.begin();

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

  File f = LittleFS.open("/usersettings.TXT", "r");
  if (!f) {
    Serial.println("file open failed");
  }
  else{
    Serial.println(f.readString());
  }
  
  LittleFS.end();
}

void loop(){
  server.handleClient();
  webSocket.loop();

  if (pulse){
    if (builtin){
      digitalWrite(LED_BUILTIN, LOW);
      char c[] = "on";
      webSocket.broadcastTXT(c, sizeof(c));
    }
    else{
      digitalWrite(LED_BUILTIN, HIGH);
      char c[] = "off";
      webSocket.broadcastTXT(c, sizeof(c));
    }
    pulse = false;
  }
}
