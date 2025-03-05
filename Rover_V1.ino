// esp8266 configuration
// 5v for S-1-RX4 (receiver)
// 12v for motors or fans
// Don't forget to ground the controller
#include<ESP8266WiFi.h>
#include<ESP8266WebServer.h>
#include<WebSocketsServer.h>

const char* ssid = "Xperia V";
const char* password = "connect.now";

// ESP8266WebServer server(80);
WebSocketsServer webSocket(81);


#define IN1 14 // D5 - fan 1 control
#define IN2 12 // D6 - fan 1 control
#define IN3 15 // D8 - Fan 2 control
#define IN4 13 // D7 - fan 2 control

#define RF_CH1 5 // D1 - RF receiver channel 1 (front)
#define RF_CH2 4 // D2 - RF receiver channel 2 (Back)
#define RF_CH3 0 // D3 - RF receiver channel 3 (right)
#define RF_CH4 2 // D4 - RF receiver channel 4 (left)

  
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  digitalWrite(IN1,LOW);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,LOW);


  pinMode(RF_CH1, INPUT);
  pinMode(RF_CH2, INPUT);
  pinMode(RF_CH3, INPUT);
  pinMode(RF_CH4, INPUT);

  // WiFi.softAP(ssid,password);
  // Serial.println("WiFi AP started");
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // web server routes
  /*
  server.on("/",handleRoot);
  server.on("/forward",[](){moveForward();
    server.send(200,"text/plain","moving forward");});
  server.on("/backward",[](){moveBackward();
    server.send(200,"text/plain","Moving backward");});
  server.on("/left",[](){turnLeft();
    server.send(200,"text/plain","Moving left");});
  server.on("/right",[](){turnRight();
    server.send(200,"text/plain","Moving right");});
  server.on("/stop",[](){stopMotors();
    server.send(200,"text/plain","stopped");});
  
  server.begin();
  */

  // web socket server
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

}

void loop() {
  // put your main code here, to run repeatedly:
  // server.handleClient();

  webSocket.loop(); // handle webSocket communication

/*
  if(digitalRead(RF_CH1) == HIGH) moveForward();  
  else if(digitalRead(RF_CH2) == HIGH) moveBackward(); 
  else if(digitalRead(RF_CH3) == HIGH) turnRight();
  else if(digitalRead(RF_CH4) == HIGH) turnLeft();
  else stopMotors();
  */

}

// handle websocket messages
void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length){
  String command = String((char*)payload);

  if (command == "forward"){ moveForward();}
  else if (command == "backward"){ moveBackward();}
  else if (command == "left"){ turnLeft();}
  else if (command == "right"){ turnRight();}
  else if (command == "stop"){ stopMotors();}
}

void moveForward(){
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
  Serial.println("moving forward");
}

void moveBackward(){
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH);
  Serial.println("moving backward");
}

void turnLeft(){
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,LOW);
  Serial.println("turning left");
}

void turnRight(){
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
  Serial.println("turning right");
}

void stopMotors(){
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,LOW);
  Serial.println("stopped");
}

/*void handleRoot(){
  server.send(200,"text/html",R"rawliteral(
    <html>
    <head>
      <style>
        button { padding: 15px; margin: 5px; font-size: 20px;}
      </style>
    </head>
    <body>
      <h1>ESP8266 Robot COntroller</h1>
      <button onmousedown="sendCommand('forward')" onmouseup="sendCommand('stop')">Forward</button><br>
      <button onmousedown="sendCommand('backward')" onmouseup="sendCommand('stop')">Backward</button><br>
      <button onmousedown="sendCommand('left')" onmouseup="sendCommand('stop')">Left</button><br>
      <button onmousedown="sendCommand('right')" onmouseup="sendCommand('stop')">Right</button><br>
      <button onclick="sendCommand('stop')">Stop</button>

      <script>
        var socket = new WebSocket("ws://192.168.4.2:81/"); //ESP's IP
        function sendCommand(command){
          socket.send(command);
        }
      </script>
    </body>
    </html>

  )rawliteral");
}
*/
