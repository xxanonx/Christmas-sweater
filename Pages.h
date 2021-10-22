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
          document.getElementById("BuiltInButton").value = event.data;
          document.getElementById("BuiltInButton").innerHTML=document.getElementById("BuiltInButton").value;
        }
      }
      function sendBuiltinOn(){
        var buttonState = document.querySelector('#BuiltInButton').value;
        var isOn = buttonState.indexOf("on");
        var isOff = buttonState.indexOf("off");
    if (isOn != -1){
      Socket.send("builtin off");
    }else if ((isOff != -1)){
      Socket.send("builtin on");
      
    }
    }

    </script>
  </head>
  <body onload="javascript:init()">
    <style>body{ background-color: #537846; font-family: Arial, Helvetica, Sans-Serif; Color: #000000; }</style>
    <a href="/"><button>HOME</button></a>
    <h1>Manual Lighting Page</h1>
    <p>Built-in LED&emsp;<button id="BuiltInButton" onclick="sendBuiltinOn()" value="On"></button></p>
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
}

void show_page_lighting(){
  server.send(200, "text/html", page_lighting);
}

void show_page_lighting_manual(){
  server.send(200, "text/html", page_lighting_manual);
}

void show_page_admin(){
  server.send(200, "text/html", page_lighting_manual);
}

void show_page_404(){
  server.send(200, "text/html", page_404);
}


void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t leng){
  char p[leng];
  for(int i = 0; i < leng; i++){
    Serial.print((char) payload[i]);
    p[i] = ((char) payload[i]);
  }
  Serial.println(p);
  String load = String(p);
  if (load.substring(0,10) == "builtin on") builtin = true;
  else if (load.substring(0,11) == "builtin off") builtin = false;
  else if (load.substring(0,15) == "password check("){
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
  pulse = true;
}
