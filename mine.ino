#include <Math.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#define SUM 5 //舵机数量
int pin[SUM] = {D2, D3, D0, D5, D8};
double control_width[SUM] = {1500, 1500, 1500, 1500, 1500};   //单位us，周期为25000us，90°为500us高电平，90°为2500us高电平
int low_width = 500, high_width = 2500;
double width[SUM] = {1500, 1500, 1500, 1500, 1500};
double pos[3] = {0, 5.5, 1};
double angle[SUM] = {0.3, 0.3, 0.3, 0.3, 0.1};

String AP_SSID = "ESP8266";                //ESP8266自已创建的热点名称
String AP_PSK  = "12345678";               //热点密码

ESP8266WebServer server(80);

String mainHtml =
R"EOF(<!DOCTYPE html>
<html>

<head>
    <title>web control robotic arm </title>
    <style>
        input {
            width: 50px;
            margin: 5px;
        }

        .main {
            text-align: center;
        }

        .container {
            width: 400px;
            height: 400px;
            margin: auto;
            border: 1px solid black;
            border-radius: 30px;
        }

        .visableArea {
            width: 380px;
            height: 380px;
            margin: auto;
        }

        #bottom {
            width: 75px;
            height: 25px;
            border: 1px solid black;
            background-color: gray;
            top: 355px;
            left: 0px;
            position: relative;
            border-radius: 10px;
        }

        #base {
            width: 25px;
            height: 70px;
            border: 1px solid black;
            background-color: gray;
            top: 260px;
            left: 25px;
            position: relative;
            border-radius: 10px;
        }

        #l1 {
            width: 25px;
            height: 123px;
            border: 1px solid black;
            background-color: gray;
            top: 207px;
            left: 25px;
            position: relative;
            transform-origin: top center;
            border-radius: 10px;
        }

        #l2 {
            width: 25px;
            height: 98px;
            border: 1px solid black;
            background-color: gray;
            top: 103px;
            left: 0px;
            position: relative;
            transform-origin: top center;
            border-radius: 10px;
        }

        #l3 {
            width: 25px;
            height: 70px;
            border: 1px solid black;
            background-color: gray;
            top: 78px;
            left: 0px;
            position: relative;
            transform-origin: top center;
            border-radius: 10px;
        }

        #l4 {
            width: 25px;
            height: 75px;
            border: 1px solid black;
            background-color: gray;
            top: 50px;
            left: 0px;
            position: relative;
            transform-origin: top center;
            border-radius: 10px;
        }

        #button {
            background-color: #4CAF50;
            color: white;
            padding: 10px 20px;
            border: none;
            border-radius: 5px;
            cursor: pointer;
        }
    </style>
</head>

<body>
    <div class="main">
        <div class="container">
            <div class="visableArea" id="control">
                <div id="bottom"></div>
                <div id="base"></div>
                <div id="l1">
                    <div id="l2">
                        <div id="l3">
                            <div id="l4"></div>
                        </div>
                    </div>
                </div>

            </div>
        </div>
        <div class="indexes area">
            <div>
                <button id="button">send</button>
                <h4>parameters:</h4>
            </div>
            <div>
                width:&nbsp&nbsp&nbsp&nbsp
                <input typed="text" id="w0">
                <input typed="text" id="w1">
                <input typed="text" id="w2">
                <input typed="text" id="w3">
                <input typed="text" id="w4">
            </div>
            <div>
                position:
                <input typed="text" id="p0">
                <input typed="text" id="p1">
                <input typed="text" id="p2">
                <input typed="text" id="p3">
                <input typed="text" id="p4">
            </div>
            <div>
                angle:&nbsp&nbsp&nbsp&nbsp
                <input typed="text" id="a0">
                <input typed="text" id="a1">
                <input typed="text" id="a2">
                <input typed="text" id="a3">
                <input typed="text" id="a4">
            </div>
            <div>
                velocity:
                <input typed="text" id="v0">
                <input typed="text" id="v1">
                <input typed="text" id="v2">
                <input typed="text" id="v3">
                <input typed="text" id="v4">
            </div>

        </div>
    </div>

    <script>
        const w0 = document.getElementById("w0");
        const w1 = document.getElementById("w1");
        const w2 = document.getElementById("w2");
        const w3 = document.getElementById("w3");
        const w4 = document.getElementById("w4");

        const p0 = document.getElementById("p0");
        const p1 = document.getElementById("p1");
        const p2 = document.getElementById("p2");
        const p3 = document.getElementById("p3");
        const p4 = document.getElementById("p4");

        const a0 = document.getElementById("a0");
        const a1 = document.getElementById("a1");
        const a2 = document.getElementById("a2");
        const a3 = document.getElementById("a3");
        const a4 = document.getElementById("a4");

        const v0 = document.getElementById("v0");
        const v1 = document.getElementById("v1");
        const v2 = document.getElementById("v2");
        const v3 = document.getElementById("v3");
        const v4 = document.getElementById("v4");

        const l1 = document.getElementById("l1");
        const l2 = document.getElementById("l2");
        const l3 = document.getElementById("l3");
        const l4 = document.getElementById("l4");

        var button = document.getElementById("button");

        function sendData() {
            var xhttp = new XMLHttpRequest();
            xhttp.open("GET", "command?" + w0.value + "," + w1.value + "," + w2.value + "," + w3.value + "," + w4.value);
            xhttp.send();
        }


        var L1 = 4.4;
        var L2 = 3.1;
        var low_width = 500, high_width = 2500;
        var W = [1500, 1500, 1500, 1500, 1500];
        var P = [0., 0., 7.5, 90, 0.5];
        var A = [0., 0., 0., 0., 0.5];
        var V = [1, 0.1, 0.1, 1, 0.01];
        function update(from) {
            var bad_value = false;
            switch (from) {
                case 0:
                    A[0] = (W[0] - (low_width + high_width) / 2) / (high_width - low_width) * Math.PI;
                    A[1] = (W[1] - (low_width + high_width) / 2) / (high_width - low_width) * Math.PI;
                    A[2] = (W[2] - (low_width + high_width) / 2) / (high_width - low_width) * Math.PI;
                    A[3] = (W[3] - (low_width + high_width) / 2) / (high_width - low_width) * Math.PI;
                    A[4] = (W[4] - 1000) / 1000;

                    P[0] = A[0] / Math.PI * 180;
                    P[3] = (A[3] + A[1] - A[2]) / Math.PI * 180;
                    P[4] = A[4];

                    P[1] = Math.cos(Math.PI / 2 - A[1]) * L1 + Math.cos(Math.PI / 2 - A[1] + A[2]) * L2;
                    P[2] = Math.sin(Math.PI / 2 - A[1]) * L1 + Math.sin(Math.PI / 2 - A[1] + A[2]) * L2;

                case 1:
                    A[0] = P[0] / 180 * Math.PI;
                    A[3] = P[3] / 180 * Math.PI - A[1] + A[2];
                    A[4] = P[4];

                    var L = Math.pow(P[1] * P[1] + P[2] * P[2], 1. / 2);
                    if (L > L1 + L2 - 1e-5) {
                        L = L1 + L2 - 1e-5;
                        bad_value = true;
                    }

                    if (L < L1 - L2 + 1e-5) {
                        L = L1 - L2 + 1e-5;
                        bad_value = true;
                    }
                    A[1] = Math.PI / 2 - Math.atan(P[2] / P[1]) - Math.acos((L1 * L1 + L * L - L2 * L2) / (2. * L1 * L));
                    A[2] = Math.acos((L1 * L1 + L2 * L2 - L * L) / (2. * L1 * L2)) - Math.PI;

                    W[0] = A[0] / Math.PI * (high_width - low_width) + (low_width + high_width) / 2;
                    W[1] = A[1] / Math.PI * (high_width - low_width) + (low_width + high_width) / 2;
                    W[2] = A[2] / Math.PI * (high_width - low_width) + (low_width + high_width) / 2;
                    W[3] = A[3] / Math.PI * (high_width - low_width) + (low_width + high_width) / 2;
                    W[4] = A[4] * 1000 + 1000;
                    break;

                case 2:
                    W[0] = A[0] / Math.PI * (high_width - low_width) + (low_width + high_width) / 2;
                    W[1] = A[1] / Math.PI * (high_width - low_width) + (low_width + high_width) / 2;
                    W[2] = A[2] / Math.PI * (high_width - low_width) + (low_width + high_width) / 2;
                    W[3] = A[3] / Math.PI * (high_width - low_width) + (low_width + high_width) / 2;
                    W[4] = A[4] * 1000 + 1000;

                    P[0] = A[0] / Math.PI * 180;
                    P[3] = (A[3] + A[1] - A[2]) / Math.PI * 180;
                    P[4] = A[4];

                    P[1] = Math.cos(Math.PI / 2 - A[1]) + Math.cos(Math.PI / 2 - A[1] + A[2]);
                    P[2] = Math.sin(Math.PI / 2 - A[1]) + Math.sin(Math.PI / 2 - A[1] + A[2]);
                    break;

                case 3:
                    break;
            }

            for (let i = 0; i < 5; i++) {
                if (W[i] < 500) {
                    bad_value = true;
                    W[i] = 500;
                }
                if (W[i] > 2500) {
                    bad_value = true;
                    W[i] = 2500;
                }
            }
            if (bad_value) update(0);




            w0.value = Math.trunc(W[0]);
            w1.value = Math.trunc(W[1]);
            w2.value = Math.trunc(W[2]);
            w3.value = Math.trunc(W[3]);
            w4.value = Math.trunc(W[4]);

            p0.value = P[0];
            p1.value = P[1];
            p2.value = P[2];
            p3.value = P[3];
            p4.value = P[4];

            a0.value = A[0];
            a1.value = A[1];
            a2.value = A[2];
            a3.value = A[3];
            a4.value = A[4];

            v0.value = V[0];
            v1.value = V[1];
            v2.value = V[2];
            v3.value = V[3];
            v4.value = V[4];


            l1.style.transform = `rotate(${Math.PI + A[1]}rad)`;
            l2.style.transform = `rotate(${- A[2]}rad)`;
            l3.style.transform = `rotate(${+ A[3]}rad)`;
            l4.style.transform = `rotate(${0}rad)`;
        }

        document.addEventListener('keydown', function (event) {
            var change = false;
            switch (event.key) {
                case 'W':
                case 'w':
                    P[2] += V[2];
                    change = true;
                    break;
                case 'S':
                case 's':
                    P[2] -= V[2];
                    change = true;
                    break;
                case 'A':
                case 'a':
                    P[1] -= V[1];
                    if (P[1] < 0) P[1] = 0;
                    change = true;
                    break;
                case 'D':
                case 'd':
                    P[1] += V[1];
                    change = true;
                    break;
                case 'Q':
                case 'q':
                    P[0] += V[0];
                    change = true;
                    break;
                case 'E':
                case 'e':
                    P[0] -= V[0];
                    change = true;
                    break;
                case 'U':
                case 'u':
                    P[3] -= V[3];
                    change = true;
                    break;
                case 'I':
                case 'i':
                    P[3] += V[3];
                    change = true;
                    break;
                case 'J':
                case 'j':
                    P[4] -= V[4];
                    change = true;
                    break;
                case 'K':
                case 'k':
                    P[4] += V[4];
                    change = true;
                    break;
            }
            if (change) {
                update(1);
                sendData();
            }
        })

        for (let i = 0; i < 5; i++) {
            document.getElementById("w" + i).addEventListener('change', function (event) {
                W[i] = parseInt(event.target.value);
                update(0);
            })
        }

        for (let i = 0; i < 5; i++) {
            document.getElementById("p" + i).addEventListener('change', function (event) {
                P[i] = parseFloat(event.target.value);
                update(1);
            })
        }

        for (let i = 0; i < 5; i++) {
            document.getElementById("a" + i).addEventListener('change', function (event) {
                A[i] = parseFloat(event.target.value);
                update(2);
            })
        }

        for (let i = 0; i < 5; i++) {
            document.getElementById("v" + i).addEventListener('change', function (event) {
                V[i] = parseFloat(event.target.value);
                update(3);
            })
        }

        var isDraggingArea = false;
        var isDraggingHand = false;
        document.getElementById("control").addEventListener('mousedown', function (e) {
            isDraggingArea = true;
        });

        document.getElementById("l3").addEventListener('mousedown', function (e) {
            isDraggingHand = true;
        });

        document.getElementById("control").addEventListener('mouseup', function (e) {
            isDraggingArea = false;
            isDraggingHand = false;
        });

        document.getElementById("control").addEventListener('mousemove', function (e) {
            if (isDraggingHand){
                const rect = document.getElementById("base").getBoundingClientRect();
                const X = (e.clientX - rect.x);
                const Y = -(e.clientY - rect.y);
                A[3] = - Math.atan(Y/X);
                update(2);
                sendData();
            }
            else if (isDraggingArea) {
                const rect = document.getElementById("base").getBoundingClientRect();
                P[1] = (e.clientX - rect.x) / 25;
                P[2] = -(e.clientY - rect.y) / 25;
                update(1);
                sendData();
            }
        });
        button.onclick = sendData;
        update(1);
    </script>
</body>

</html>)EOF";

void handleRoot() {
  server.send(200, "text/html", mainHtml);
}

void command(){
  if(server.args() != 0){
    String t = server.argName(0);
    for (char i = 0; i < 5; i++) {
    String substr = t.substring(0, t.indexOf(','));
    int number = 0;
    for (char j = 0; j < substr.length(); j++){
      number *= 10;
      number += substr[j] - '0';
    }
    control_width[i] = number;
    t = t.substring(t.indexOf(',') + 1);
  }
    Serial.println("command success!");
    server.send(200);
  }
}

void handleNotFound(){
  digitalWrite(LED_BUILTIN, 1);
  String message = "File Not Foundnn";
  message += "URI: ";
  message += server.uri();
  message += "nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "nArguments: ";
  message += server.args();
  message += "n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(LED_BUILTIN, 0);
}

void setup() {
  for (char i = 0; i < SUM; i++) {
    pinMode(pin[i], OUTPUT);
    digitalWrite(pin[i], LOW);
    
  }

  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  
  WiFi.mode(WIFI_AP);          
  WiFi.softAPConfig({192, 168, 1, 1}, {192, 168, 1, 1}, {255, 255, 255, 0}); //配置开发板IP,网关与子网掩码.
  if (WiFi.softAP(AP_SSID, AP_PSK)) {                             //开启WIFI_AP无线网络,热点 与 密码
    Serial.printf("\n创建WIFI.SSID:%S", WiFi.softAPSSID());
    Serial.printf(" 密码:%S", WiFi.softAPPSK());
    Serial.printf(" IP:%s", WiFi.softAPIP().toString().c_str());
    //Serial.print("MAC );Serial.println(WiFi.softAPmacAddress());
  }

  
  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }
 
  server.on("/", handleRoot);
  server.on("/command", HTTP_GET, command);
  server.on("/inline", [](){
    server.send(200, "text/plain", "this works as well");
  });
 
  server.onNotFound(handleNotFound);
 
  server.begin();
  Serial.println("HTTP server started");
}

double dL;
void loop() {
  server.handleClient();

  static uint64_t current_time = 0;
  if ((micros() - current_time) >= 2500){
    current_time = micros();
    for (char i = 0; i < SUM; i++){
      digitalWrite(pin[i], HIGH);
    }

    bool closed[5] = {0,0,0,0,0};
    while (not (closed[0] && closed[1] && closed[2] && closed[3] && closed[4])){
      for (char i = 0; i < SUM; i++){
        if ((micros()- current_time) >= control_width[i] and not closed[i]){
          digitalWrite(pin[i], LOW);
          closed[i] |= 1;
        }
      }
    }
  }
}
