#include <LittleFS.h>

const bool FORMAT = false;
bool format_successful = false;
bool data_read_successful = false;
int delay_ = 50;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  LittleFS.begin();
  pinMode(LED_BUILTIN, OUTPUT);

  if (FORMAT){
    format_successful = LittleFS.format();
  }
  else{
    Serial.println("reading");
    File f = LittleFS.open("/usersettings.TXT", "r");
    if (!f) {
      Serial.println("file open failed");
    }
    else{
      while(f.available()){
        Serial.println(f.readString());
      }
      f.close();
      data_read_successful = true;
      
    }
    LittleFS.end();
  }
  
}

void loop() {
  // put your main code here, to run repeatedly:
  if (format_successful and FORMAT){
    delay_ = 1000;
  }
  else if(data_read_successful and (FORMAT == false)){
    delay_ = 500;
  }
  digitalWrite(LED_BUILTIN, HIGH);
  delay(delay_);
  digitalWrite(LED_BUILTIN, LOW);
  delay(delay_);
}
