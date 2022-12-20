#include <SpecialBool.h>
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
bool automatic_mode = true;
bool man_open = false;
int automatic_step_no = 0;
Timer automatic_timer(2000, 1);
Timer smooth_interval(35, 1);
SpecialBool auto_trigger;
SpecialBool smooth_trigger;

bool man_led_change = true;
bool auto_max_change;
bool auto_min_change;
struct value{
  byte LED_value = 127;
  byte LED_add = 1;
  byte Max = 255;
  byte Min = 0;
};
value LED_value_1;
value LED_value_2;
value LED_value_3;
value LED_value_4;
const int LED_pin_1 = 5;
const int LED_pin_2 = 0;
const int LED_pin_3 = 14;
const int LED_pin_4 = 13;
value *AofLEds[] = {&LED_value_1, &LED_value_2, &LED_value_3, &LED_value_4};

#include "Pages.h"

void socket_send(char c[15]){
  webSocket.broadcastTXT(c, sizeof(c));
}

void setup() {
  pinMode(LED_pin_1, OUTPUT);
  pinMode(LED_pin_2, OUTPUT);
  pinMode(LED_pin_3, OUTPUT);
  pinMode(LED_pin_4, OUTPUT);
  
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
    while(f.available()){
      Serial.write(f.read());
    }
    f.close();
  }
  
  LittleFS.end();
}

void loop(){
  server.handleClient();
  webSocket.loop();
  auto_trigger.write(automatic_timer.input(not auto_trigger.special.f));
  smooth_trigger.write(smooth_interval.input(not smooth_trigger.special.f));
  if (auto_trigger.fall()){
    auto_trigger.special.f = 0;
  }
  if (smooth_trigger.fall()){
    smooth_trigger.special.f = 0;
  }
  

  if (pulse){
    if (builtin){
      digitalWrite(LED_BUILTIN, LOW);
      //socket_send("BI on");
    }
    else{
      digitalWrite(LED_BUILTIN, HIGH);
      //socket_send("BI off");
    }
    pulse = false;
  }

  if (not automatic_mode and man_led_change){
    analogWrite(LED_pin_1, LED_value_1.LED_value);
    analogWrite(LED_pin_2, LED_value_2.LED_value);
    analogWrite(LED_pin_3, LED_value_3.LED_value);
    analogWrite(LED_pin_4, LED_value_4.LED_value);
    Serial.print(LED_value_1.LED_value);
    Serial.print(",");
    Serial.print(LED_value_2.LED_value);
    Serial.print(",");
    Serial.print(LED_value_3.LED_value);
    Serial.print(",");
    Serial.println(LED_value_4.LED_value);
    man_led_change = false;
  }
  //Write auto mode
  else if (automatic_mode){
    if ((auto_trigger.actual() and not auto_trigger.special.f) or auto_trigger.special.g){
      //Serial.println("step change");
      automatic_step_no++;
      auto_trigger.special.f = 1;
      auto_trigger.special.g = 0;
      if (automatic_step_no >= 4) {
        automatic_step_no = 0;
      }
    }
    if (smooth_trigger.actual() and not smooth_trigger.special.f){
      //Serial.println("smooth change");
      smooth_trigger.special.f = 1;
      if(auto_max_change){
        Serial.println("maximum change");
        LED_value_1 = check_if_above_max(LED_pin_1, LED_value_1);
        LED_value_2 = check_if_above_max(LED_pin_2, LED_value_2);
        LED_value_3 = check_if_above_max(LED_pin_3, LED_value_3);
        LED_value_4 = check_if_above_max(LED_pin_4, LED_value_4);
        if((LED_value_1.LED_value <= LED_value_1.Max) and 
          (LED_value_2.LED_value <= LED_value_2.Max) and 
          (LED_value_3.LED_value <= LED_value_3.Max) and
          (LED_value_4.LED_value <= LED_value_4.Max)){
          auto_max_change = false;
        }
      }
      if(auto_min_change){
        Serial.println("minimum change");
        LED_value_1 = check_if_below_min(LED_pin_1, LED_value_1);
        LED_value_2 = check_if_below_min(LED_pin_2, LED_value_2);
        LED_value_3 = check_if_below_min(LED_pin_3, LED_value_3);
        LED_value_4 = check_if_below_min(LED_pin_4, LED_value_4);
        if((LED_value_1.LED_value >= LED_value_1.Min) and 
          (LED_value_2.LED_value >= LED_value_2.Min) and 
          (LED_value_3.LED_value >= LED_value_3.Min) and
          (LED_value_4.LED_value >= LED_value_4.Min)){
          auto_min_change = false;
        }
      }
      switch (automatic_step_no){
        case 0:
          LED_value_1 = smooth_dat_boi(LED_pin_1, LED_value_1);
        break;
        case 1:
          LED_value_2 = smooth_dat_boi(LED_pin_2, LED_value_2);
        break;
        case 2:
          LED_value_3 = smooth_dat_boi(LED_pin_3, LED_value_3);
        break;
        case 3:
          LED_value_4 = smooth_dat_boi(LED_pin_4, LED_value_4);
          /*LED_value_1 = smooth_dat_boi(LED_pin_1, LED_value_1);
          LED_value_2 = smooth_dat_boi(LED_pin_2, LED_value_2);
          LED_value_3 = smooth_dat_boi(LED_pin_3, LED_value_3);
          */
        break;
        default:
          automatic_step_no = 0;
        break;
      }
      
      Serial.print(LED_value_1.LED_value);
      Serial.print(",");
      Serial.print(LED_value_2.LED_value);
      Serial.print(",");
      Serial.print(LED_value_3.LED_value);
      Serial.print(",");
      Serial.println(LED_value_4.LED_value);
      
    }
  }
}

value smooth_dat_boi(int pin, value LED_values){
  if((LED_values.LED_value >= LED_values.Max) or (LED_values.LED_value <= LED_values.Min)){
    LED_values.LED_add *= -1;
    LED_values.LED_value += LED_values.LED_add;
  }
  LED_values.LED_value += LED_values.LED_add;
  if (LED_values.Max == LED_values.Min){
    LED_values.LED_value = LED_values.Min;
    auto_trigger.special.g = 1;
  }
  analogWrite(pin, LED_values.LED_value);
  return LED_values;
}
value check_if_above_max(int pin, value LED_values){
  if(LED_values.LED_value > LED_values.Max){
    LED_values.LED_add = 1;
    LED_values.LED_value -= 5;
    analogWrite(pin, LED_values.LED_value);
  }
  return LED_values;
}
value check_if_below_min(int pin, value LED_values){
  if(LED_values.LED_value < LED_values.Min){
    LED_values.LED_add = -1;
    LED_values.LED_value += 5;
    analogWrite(pin, LED_values.LED_value);
  }
  return LED_values;
}
