#include "christmas_sweater_settings.h"

const char page_404[] PROGMEM = R"=====(
<html>
  <head>
    <meta name="viewport" content="width=device-width, initial-scale=1" />
    <title>404 Not Found</title>
  </head>
  <body>
    <style>body{ background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000000; }</style>
    <h1>404 Not Found!</h1>
    <p>The website you are looking for cannot be found,<br/>the christmas sweater only has a few websites available,<br/>go to the christmas sweater home page below.</p>
    <p><a href="/"><button>Go Home</button></a></p>
  </body>
</html>
)=====";


const char page_home[] PROGMEM = R"=====(
<html>
  <head>
    <meta name="viewport" content="width=device-width, initial-scale=1" />
    <title>LED Xmas Sweater</title>
  </head>
  <body>
    <style>body{ background-color: #945454; font-family: Arial, Helvetica, Sans-Serif; Color: #000000; }</style>
    <a href="/"><button>HOME</button></a>
    <h1>Christmas Sweater Home Page</h1>
    <p>Lighting Pattern Page  <a href="/lighting"><button>GO</button></a></p>
    <p>Administrative Details <a href="/admin"><button>GO</button></a></p>
  </body>
</html>
)=====";


const char page_lighting[] PROGMEM = R"=====(
<html>
  <head>
    <meta name="viewport" content="width=device-width, initial-scale=1" />
    <title>LED Xmas Sweater</title>
    <script>
      var Socket;
      function init() {
        Socket = new WebSocket('ws://' + window.location.hostname + ':81/');
      }
    </script>
  </head>
  <body onload="javascript:init()">
    <style>body{ background-color: #537846; font-family: Arial, Helvetica, Sans-Serif; Color: #000000; }</style>
    <a href="/"><button>HOME</button></a>
    <h1>Lighting Pattern Page</h1>
    <p>Manual Control Page  <a href="/lighting/manual"><button>GO</button></a></p>
  </body>
</html>
)=====";


const char page_lighting_manual[] PROGMEM = R"=====(
<html>
  <head>
    <meta name="viewport" content="width=device-width, initial-scale=1" />
    <title>LED Xmas Sweater</title>
    <script>
    var Socket;
    function init() {
      Socket = new WebSocket('ws://' + window.location.hostname + ':81/');
      Socket.onmessage = function(event){
        console.log("Socket Event!");
        var message = event.data;
        var includesOn = (message.indexOf("On") != -1);
        var includesOff = (message.indexOf("Off") != -1);
        var newText;
        if (includesOn){
          newText = "On";
        }else if (includesOff){
          newText = "Off";
        }
        
        if (event.data.indexOf("BI") != -1){
          document.getElementById("BuiltInButton").innerHTML = newText;
        }else if (event.data.indexOf("AM") != -1){
          document.getElementById("AutoButton").innerHTML = newText;
        }
      }
    }
    function sendBuiltinOn(){
      console.log("Sending Built-in State!");
      var buttonState = document.querySelector('#BuiltInButton').innerHTML;
      var isOn = (buttonState.indexOf("On") != -1);
      var isOff = (buttonState.indexOf("Off") != -1);
      if (isOn){
        document.getElementById("BuiltInButton").innerHTML = "Off";
        Socket.send("builtin off");
      }else if (isOff){
        document.getElementById("BuiltInButton").innerHTML = "On";
        Socket.send("builtin on");
        
      }
    }
    function sendAuto(){
      console.log("Sending Auto State!");
      var buttonState = document.querySelector('#AutoButton').innerHTML;
      var isOn = (buttonState.indexOf("On") != -1);
      var isOff = (buttonState.indexOf("Off") != -1);
      if (isOn){
        document.getElementById("AutoButton").innerHTML = "Off";
        Socket.send("auto off");
      }else if (isOff){
        document.getElementById("AutoButton").innerHTML = "On";
        Socket.send("auto on");
      }
    }
    function sendManData(){
      const AofColors = ['R', 'G', 'B', 'W'];
      console.log("Sending Manual Data!");
      if (document.querySelector('#AutoButton').innerHTML == "On"){
        sendAuto();
      }
      var message = "Man ";
      for (let color of AofColors){
        message += document.querySelector('#myRange' + color).value;
        if (color != AofColors[3]){
          message += ":";
        }
      }
      message += ".";
      console.log(message);
      Socket.send(message);
    }
    function sendAutoData(){
      const AofColors = ['R', 'G', 'B', 'W'];
      console.log("Sending Auto Data!");
      if (document.querySelector('#AutoButton').innerHTML == "Off"){
        sendAuto();
      }
      var C1 = document.querySelector('#myRangeStep').value;
      var C2 = document.querySelector('#myRangeSmooth').value;
      var message = "Auto " +C1+":"+C2;
      for (let color of AofColors){
        message += ":" + document.querySelector('#myRange' + color + 'Max').value;
        message += ":" + document.querySelector('#myRange' + color + 'Min').value;
      }
      message += ".";
      console.log(message);
      Socket.send(message);
    }
    function checkMinOrMax(color, fromMaxBar){
      const  begin = 'myRange';
      var max = begin + color + 'Max';
      var min = begin + color + 'Min';
      var maxNum = Number(document.querySelector('#' + max).value);
      var minNum = Number(document.querySelector('#' + min).value);
      var maxMinus10 = (maxNum - 10);
      var minPlus10 = (minNum + 10);
      console.log("Max: " + max + " " + maxNum + " " + maxMinus10);
      console.log("Min: " + min + " " + minNum + " " + minPlus10);
      console.log(fromMaxBar);
      if ((fromMaxBar) && (minNum>maxMinus10) && (maxMinus10>=0)){
        console.log("fromMax " + maxMinus10);
        document.getElementById(min).value = maxMinus10;
      } 
      if ((fromMaxBar==false) && (maxNum<minPlus10) && (minPlus10<=255)){
        console.log("fromMin " + minPlus10);
        document.getElementById(max).value = minPlus10;
      }     
      sendAutoData();
    }
    
    
    </script>
  <style>
    .slidecontainer {
      width: 50%;
    }
    .slider {
      -webkit-appearance: none;
      width: 50%;
      height: 20px;
      background: #d3d3d3;
      outline: none;
      opacity: 0.7;
      -webkit-transition: .5s;
      transition: opacity .5s;
    }
    .slider:hover {
      opacity: 1;
    }
    .slider::-webkit-slider-thumb {
      -webkit-appearance: none;
      appearance: none;
      width: 20px;
      height: 20px;
      border-radius: 50%;
      background: #000000;
      cursor: pointer;
    }
    .slider::-moz-range-thumb {
      width: 20px;
      height: 20px;
      border: 0;
      border-radius: 50%;
      background: #04AA6D;
      cursor: pointer;
    }
  </style>
  </head>
  <body onload="javascript:init()">
    <style>body{ background-color: #537846; font-family: Arial, Helvetica, Sans-Serif; Color: #000000; }</style>
    <a href="/"><button>HOME</button></a>
    <h1>Manual Lighting Page</h1>
    <div>Built-in LED&emsp;<button id="BuiltInButton" onclick="sendBuiltinOn()">Off</button>
  <br>Automatic&emsp;&emsp;<button id="AutoButton" onclick="sendAuto()">On</button>
  <br>Manual brightness:
  <br><input oninput="sendManData()" style="background: #AE452E" type="range" min="0" max="255" value="127" class="slider" id="myRangeR">
  <br><input oninput="sendManData()" style="background: #23551D" type="range" min="0" max="255" value="127" class="slider" id="myRangeG">
  <br><input oninput="sendManData()" style="background: #272D72" type="range" min="0" max="255" value="127" class="slider" id="myRangeB">
  <br><input oninput="sendManData()" style="background: #ffffff" type="range" min="0" max="255" value="127" class="slider" id="myRangeW">
  <br><br><br>
  <div>Step Time&emsp;<input oninput="sendAutoData()" style="background: #f5f5f5" type="number" min="2000" max="10000" value="2000" step="10" id="myRangeStep">&emsp;&emsp;
  Smooth Time&emsp;<input oninput="sendAutoData()" style="background: ##f5f5f5" type="number" min="5" max="100" value="35" id="myRangeSmooth">
  </div>
  <br>Maximum and Minimum Automatic Brightness:
  <br><input oninput="checkMinOrMax('R', true)" style="background: #AE452E" type="range" min="0" max="255" value="255" class="slider" id="myRangeRMax"> &emsp;Max
  <br><input oninput="checkMinOrMax('R', false)" style="background: #AE452E" type="range" min="0" max="255" value="0" class="slider" id="myRangeRMin"> &emsp;Min
  <br><input oninput="checkMinOrMax('G', true)" style="background: #23551D" type="range" min="0" max="255" value="255" class="slider" id="myRangeGMax"> &emsp;Max
  <br><input oninput="checkMinOrMax('G', false)" style="background: #23551D" type="range" min="0" max="255" value="0" class="slider" id="myRangeGMin"> &emsp;Min
  <br><input oninput="checkMinOrMax('B', true)" style="background: #272D72" type="range" min="0" max="255" value="255" class="slider" id="myRangeBMax"> &emsp;Max
  <br><input oninput="checkMinOrMax('B', false)" style="background: #272D72" type="range" min="0" max="255" value="0" class="slider" id="myRangeBMin"> &emsp;Min
  <br><input oninput="checkMinOrMax('W', true)" style="background: #ffffff" type="range" min="0" max="255" value="255" class="slider" id="myRangeWMax"> &emsp;Max
  <br><input oninput="checkMinOrMax('W', false)" style="background: #ffffff" type="range" min="0" max="255" value="0" class="slider" id="myRangeWMin"> &emsp;Min
  </div>
  </body>
</html>
)=====";


const char page_admin[] PROGMEM = R"=====(
<html>
  <head>
    <meta name="viewport" content="width=device-width, initial-scale=1" />
    <title>LED Xmas Sweater</title>
    <script>
  var Socket;
  var prev_passjs;
  var new_ssidjs;
  var new_passjs;
  var expecting_pf = false;
  
  function init() {
    Socket = new WebSocket('ws://' + window.location.hostname + ':81/');
    Socket.onmessage = function(event){
      var socketData = event.data;
      if ((expecting_pf == true) and (socketData.indexOf("pass check") != -1)){
        if (socketData.indexOf("pass check good") != -1){
          sendSettings();
        }else if (socketData.indexOf("pass check bad") != -1){
          
        }
      }
    }
  }
  function sendPassCheck(){
    prev_passjs = document.querySelector('#old_password').value;
    new_ssidjs = document.querySelector('#new_ssid').value;
    new_passjs = document.querySelector('#new_password').value;
    Socket.send("password check(" + prev_passjs + ")");
    expecting_pf = true;
  }
  
  function sendSettings(){
    Socket.send("new ssid: " + new_ssidjs + ", new pass: " + new_passjs);
    expecting_pf = false;
  }

    </script>
  </head>
  <body onload="javascript:init()">
    <style>body{ background-color: #945454; font-family: Arial, Helvetica, Sans-Serif; Color: #000000; }</style>
    <a href="/"><button>HOME</button></a>
    <h1>Administrative Page</h1>
  <hr>
  <h3>Change WIFI settings:</h3>
  <form>
    <label for="old_password">Previous Password:&ensp;&nbsp;</label>
    <input type="password" id="old_password" name="old_password"><br/>
    <sub>(must be entered in order for any changes to be made, even reset)</sub>
    <br/>
    <br/><label for="new_ssid">New WIFI name:&emsp;&emsp;</label>
    <input type="text" id="new_ssid" name="new_ssid" value="ESP-test"><br/>
    <sub>(e.g., Johns Xmas Sweater, Liz's Jacket, etc.)</sub>   
    <br/><label for="new_password">New Password:&emsp;&emsp;&ensp;</label>
    <input type="text" id="new_password" name="new_password"><br/>
    <sub>(no password for open access to anyone, otherwise must be 8 characters long)</sub>
    <br/><br/><button id="send_settings" onclick="sendPassCheck()">Send settings</button>
  </form>
    <p>&emsp;</p>
  <p>Press the reset button to <strong>Reset all</strong> settings to default,<br/>
  even WIFI name and password. This action cannot be undone.<br/>
  <br/><br/><button id="reset_settings" onclick=""><strong>Reset all</strong></button></p>
  
  </body>
</html>
)=====";


void show_page_home(){
  server.send(200, "text/html", page_home);
  man_open = false;
}

void show_page_lighting(){
  server.send(200, "text/html", page_lighting);
  man_open = false;
}

void show_page_lighting_manual(){
  server.send(200, "text/html", page_lighting_manual);
  man_open = true;
}

void show_page_admin(){
  server.send(200, "text/html", page_admin);
  man_open = false;
}

void show_page_404(){
  server.send(200, "text/html", page_404);
  man_open = false;
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t leng){
  bool includesOn;
  bool includesOff;
  char p[leng];
  for(int i = 0; i < leng; i++){
    //Serial.print((char) payload[i]);
    p[i] = ((char) payload[i]);
  }
  Serial.println(p);
  String load = String(p);
  includesOn = (load.indexOf("on") != -1);
  includesOff = (load.indexOf("off") != -1);
  Serial.print("includesOn: ");
  Serial.println(includesOn);
  Serial.print("includesOff: ");
  Serial.println(includesOff);

  Serial.print("man_open: ");
  Serial.println(man_open);
  if ((includesOn or includesOff)){
    if (load.indexOf("builtin") != -1) builtin = (includesOn and not includesOff);
    else if (load.indexOf("auto") != -1){
      automatic_mode = (includesOn and not includesOff);
      if (automatic_mode) auto_max_change = true;
    }
  }
  else if ((load.indexOf("Man") != -1)){
    if (automatic_mode) automatic_mode = false;
    man_led_change = false;
    int start_value = (load.indexOf("Man ") + 4);
    int end_value = load.indexOf(":");
    int value_ = int(load.substring(start_value, end_value).toInt());
    for (int led = 0; led <= 3; led++){
      if (AofLEds[led]->LED_value != value_){
        man_led_change = true;
        AofLEds[led]->LED_value = value_;
      }
      if (led == 3)
        break;
      start_value = (end_value + 1);
      if (led != 2) 
        end_value = load.indexOf(":", start_value);
      else 
        end_value = load.indexOf(".", start_value);
      value_ = int(load.substring(start_value, end_value).toInt());
    }
    /*if (LED_value_1.LED_value != value_){
      man_led_change = true;
      LED_value_1.LED_value = value_;
    }
    start_value = (end_value + 1);
    end_value = load.indexOf(":", start_value);
    value_ = int(load.substring(start_value, end_value).toInt());
    //Serial.println(load.substring(start_value, end_value));
    //Serial.println(value);
    if (LED_value_2.LED_value != value_){
      man_led_change = true;
      LED_value_2.LED_value = value_;
    }
    start_value = (end_value + 1);
    end_value = load.indexOf(".", start_value);
    value_ = int(load.substring(start_value, end_value).toInt());
    //Serial.println(load.substring(start_value, end_value));
    //Serial.println(value);
    if (LED_value_3.LED_value != value_){
      man_led_change = true;
      LED_value_3.LED_value = value_;
    }*/
  }
  else if ((load.indexOf("Auto") != -1)){
    if (not automatic_mode) automatic_mode = true;
    int start_value = (load.indexOf("Auto ") + 5);
    int end_value = load.indexOf(":");
    int value_ = int(load.substring(start_value, end_value).toInt());
    for (int led = -1; led <= 3; led++){
      if (led == -1){
        if (automatic_timer.time != value_){
          automatic_timer.time = value_;
        }
        start_value = (end_value + 1);
        end_value = load.indexOf(":", start_value);
        value_ = int(load.substring(start_value, end_value).toInt());
        if (smooth_interval.time != value_){
          smooth_interval.time = value_;
        }
      }
      else{
        if (AofLEds[led]->Max != value_){
          auto_max_change = true;
          AofLEds[led]->Max = value_;
        }
        start_value = (end_value + 1);
        if (led != 3) 
          end_value = load.indexOf(":", start_value);
        else 
          end_value = load.indexOf(".", start_value);
        value_ = int(load.substring(start_value, end_value).toInt());
        if (AofLEds[led]->Min != value_){
          auto_min_change = true;
          AofLEds[led]->Min = value_;
        }
      }
      if (led == 3)
        break;
      start_value = (end_value + 1);
      end_value = load.indexOf(":", start_value);
      value_ = int(load.substring(start_value, end_value).toInt());
    }
    /*if (automatic_timer.time != value_){
      automatic_timer.time = value_;
    }
    start_value = (end_value + 1);
    end_value = load.indexOf(":", start_value);
    value_ = int(load.substring(start_value, end_value).toInt());
    if (smooth_interval.time != value_){
      smooth_interval.time = value_;
    }
    start_value = (end_value + 1);
    end_value = load.indexOf(":", start_value);
    value_ = int(load.substring(start_value, end_value).toInt());
    //Serial.println(load.substring(start_value, end_value));
    //Serial.println(value);
    if (LED_value_1.Max != value_){
      auto_max_change = true;
      LED_value_1.Max = value_;
    }
    start_value = (end_value + 1);
    end_value = load.indexOf(":", start_value);
    value_ = int(load.substring(start_value, end_value).toInt());
    //Serial.println(load.substring(start_value, end_value));
    //Serial.println(value);
    if (LED_value_2.Max != value_){
      auto_max_change = true;
      LED_value_2.Max = value_;
    }
    start_value = (end_value + 1);
    end_value = load.indexOf(".", start_value);
    value_ = int(load.substring(start_value, end_value).toInt());
    //Serial.println(load.substring(start_value, end_value));
    //Serial.println(value);
    if (LED_value_3.Max != value_){
      auto_max_change = true;
      LED_value_3.Max = value_;
    }*/
  }
  else{
    if (load.indexOf("password check(") != -1){
      load.remove(0,15);
      int load_len = load.length();
      load.remove(load_len,1);
      Serial.println(load);
      if (load == password){
        Serial.println("pass excepted");
        char c[] = "pass check good";
        webSocket.broadcastTXT(c, sizeof(c));
      }
      else {
        Serial.println("pass failed");
        char c[] = "pass check bad";
        webSocket.broadcastTXT(c, sizeof(c));
      }
    }
  }
  pulse = true;
}
