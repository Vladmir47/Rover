#include <Servo.h>
#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>

#define IN1 14 // D5 - fan 1 control
#define IN2 12 // D6 - fan 1 control
#define IN3 15 // D8 - Fan 2 control
#define IN4 13 // D7 - fan 2 control


Servo myServo;

#define SERVO_PIN 2  // GPIO2 (D4 on NodeMCU/Wemos)
int currentPosition = 90;  // Start at center position (90°)

const char* ssid = "Xperia V";
const char* password = "connect.now";

WebSocketsServer webSocket(81);  // WebSocket server on port 81

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi Connected!");
    Serial.println(WiFi.localIP());

    myServo.attach(SERVO_PIN);
    myServo.write(currentPosition);  // Initialize servo position

    webSocket.begin();
    webSocket.onEvent(webSocketEvent);

    Serial.println("Servo & WebSocket Initialized");

      
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);

    digitalWrite(IN1,LOW);
    digitalWrite(IN2,LOW);
    digitalWrite(IN3,LOW);
    digitalWrite(IN4,LOW);
}

void moveRight() {
    int newPosition = currentPosition - 20;
    if (newPosition >= 0) {
        currentPosition = newPosition;
        myServo.write(currentPosition);
        Serial.print("Moved Left to: ");
        Serial.println(currentPosition);
    } else {
        Serial.println("Cannot move further left!");
    }
}

void moveLeft() {
    int newPosition = currentPosition + 20;
    if (newPosition <= 180) {
        currentPosition = newPosition;
        myServo.write(currentPosition);
        Serial.print("Moved Right to: ");
        Serial.println(currentPosition);
    } else {
        Serial.println("Cannot move further right!");
    }
}

// Handle WebSocket Messages
// void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
//     switch (type) {
//         case WStype_TEXT:
//             Serial.printf("Received WebSocket Message: %s\n", payload);
//             if (strcmp((char*)payload, "left") == 0) {
//                 moveLeft();
//             } else if (strcmp((char*)payload, "right") == 0) {
//                 moveRight();
//             }
//             break;
//     }
// }

void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
    if (type == WStype_TEXT) {
        String command = String((char*)payload);
        Serial.printf("Received WebSocket Message: %s\n", payload);

        // Movement commands
        if (command == "forward") {
            moveForward();
        } else if (command == "backward") {
            moveBackward();
        } else if (command == "stop") {
            stopMotors();
        } 

        // Turning commands
        else if (command == "left") {
            moveLeft();
        } else if (command == "right") {
            moveRight();
        }
    }
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

void stopMotors(){
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,LOW);
  Serial.println("stopped");
}


void loop() {
    webSocket.loop();  // Keep WebSocket connection alive
}
