#include "LedControl.h"
#include "display.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "Scoreboard";
const char* password = "12345678";
ESP8266WebServer server(80);

#define Button1 5
#define Button2 3
#define Button3 1
#define Button4 14

#define DIN 13
#define CS 12
#define CLK 15
LedControl lc = LedControl(DIN, CLK, CS, 0);

int A = 0;
int B = 0;
bool hotspotEnabled = false;

void setup() {
  Serial.begin(115200);  //Serial Monitor

  lc.shutdown(0, false);  //initialize displays
  lc.setIntensity(0, 7);
  lc.clearDisplay(0);
  lc.shutdown(1, false);
  lc.setIntensity(1, 7);
  lc.clearDisplay(1);
  printnumber(A, 0);
  printnumber(B, 1);

  WiFi.mode(WIFI_AP);  // Set up the wifi
  WiFi.softAP(ssid, password);
  Serial.println("Access point started");
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);  // Set up the web server
  server.on("/a", handleA);
  server.on("/b", handleB);
  server.on("/analysis", handleanalysis);
  server.begin();
  Serial.println("Web server started");
  WiFi.disconnect(true);
  WiFi.forceSleepBegin();
  Serial.println("and stopped.");
  delay(1000);

  pinMode(Button1, INPUT_PULLUP);
  pinMode(Button2, INPUT_PULLUP);
  pinMode(Button3, INPUT_PULLUP);
  pinMode(Button4, INPUT_PULLUP);
}

void loop() {
  server.handleClient();
  if (digitalRead(Button1) == 0) {  //Wifi Mode
    hotspotEnabled = !hotspotEnabled;
    if (hotspotEnabled) {
      WiFi.forceSleepWake();
      WiFi.mode(WIFI_AP);
      WiFi.softAP(ssid, password);
      Serial.println("Wifi ON");
      printSym(Wifi1, 0);
      printSym(Wifi2ON, 1);
      delay(3000);
      printnumber(A, 0);
      printnumber(B, 1);
    } else {
      WiFi.disconnect(true);
      WiFi.forceSleepBegin();
      delay(1);
      Serial.println("Wifi OFF");
      printSym(Wifi1, 0);
      printSym(Wifi2OFF, 1);
      delay(3000);
      printnumber(A, 0);
      printnumber(B, 1);
    }
    delay(1000);
  }
  if (digitalRead(Button2) == 0) {  //Auswertung
    analysis();
  }
  if (digitalRead(Button3) == 0) {  //Button 3 Display 1
    A += 1;
    printnumber(A, 0);
    delay(500);
  }
  if (digitalRead(Button4) == 0) {  //Button 4 Display 2
    B += 1;
    printnumber(B, 1);
    delay(500);
  }
}

void handleRoot() {
  String html = "<html><body><h1>Scoreboard</h1>";
  html += "<form action=\"/a\"><button style=\"font-size:24px; padding:20px;\">A</button></form>";
  html += "<form action=\"/b\"><button style=\"font-size:24px; padding:20px;\">B</button></form>";
  html += "<br>";
  html += "<form action=\"/analysis\"><button style=\"font-size:24px; padding:20px;\">evaluation</button></form>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}
void handleA() {
  A += 1;
  printnumber(A, 0);
  server.sendHeader("Location", "/", true);
  server.send(302, "text/plain", "");
}
void handleB() {
  B += 1;
  printnumber(B, 1);
  server.sendHeader("Location", "/", true);
  server.send(302, "text/plain", "");
}
void handleanalysis() {
  analysis();
  server.sendHeader("Location", "/", true);
  server.send(302, "text/plain", "");
}
void analysis() {
  delay(3000);
  if (A > B) {
    printSym(win, 0);
    printSym(lost, 1);
    delay(5000);
    A = 0;
    B = 0;
    printnumber(A, 0);
    printnumber(B, 1);
  } else if (B > A) {
    printSym(lost, 0);
    printSym(win, 1);
    delay(5000);
    A = 0;
    B = 0;
    printnumber(A, 0);
    printnumber(B, 1);
  } else if (A == B) {
    printSym(draw, 0);
    printSym(draw, 1);
    delay(5000);
    A = 0;
    B = 0;
    printnumber(A, 0);
    printnumber(B, 1);
  }
}
void printByte(byte character1[], byte character2[], int display) {
  int i = 0;
  for (i = 0; i < 4; i++) {
    lc.setRow(display, i, character1[i]);
  }
  for (i = 4; i < 8; i++) {
    lc.setRow(display, i, character2[i - 4]);
  }
}
void printSym(byte character[], int display) {
  int i = 0;
  int i2 = 7;
  for (i = 0; i < 8; i++) {
    lc.setColumn(display, i2, character[i]);
    i2 = i2 - 1;
  }
}
void printnumber(int number, int display) {
  switch (number) {
    case 0:
      printByte(ndefault, n0, display);
      break;
    case 1:
      printByte(ndefault, n1, display);
      break;
    case 2:
      printByte(ndefault, n2, display);
      break;
    case 3:
      printByte(ndefault, n3, display);
      break;
    case 4:
      printByte(ndefault, n4, display);
      break;
    case 5:
      printByte(ndefault, n5, display);
      break;
    case 6:
      printByte(ndefault, n6, display);
      break;
    case 7:
      printByte(ndefault, n7, display);
      break;
    case 8:
      printByte(ndefault, n8, display);
      break;
    case 9:
      printByte(ndefault, n9, display);
      break;
    case 10:
      printByte(n1, n0, display);
      break;
    case 11:
      printByte(n1, n1, display);
      break;
    case 12:
      printByte(n1, n2, display);
      break;
    case 13:
      printByte(n1, n3, display);
      break;
    case 14:
      printByte(n1, n4, display);
      break;
    case 15:
      printByte(n1, n5, display);
      break;
    case 16:
      printByte(n1, n6, display);
      break;
    case 17:
      printByte(n1, n7, display);
      break;
    case 18:
      printByte(n1, n8, display);
      break;
    case 19:
      printByte(n1, n9, display);
      break;
    case 20:
      printByte(n2, n0, display);
      break;
    case 21:
      printByte(n2, n1, display);
      break;
    case 22:
      printByte(n2, n2, display);
      break;
    case 23:
      printByte(n2, n3, display);
      break;
    case 24:
      printByte(n2, n4, display);
      break;
    case 25:
      printByte(n2, n5, display);
      break;
    case 26:
      printByte(n2, n6, display);
      break;
    case 27:
      printByte(n2, n7, display);
      break;
    case 28:
      printByte(n2, n8, display);
      break;
    case 29:
      printByte(n2, n9, display);
      break;
    case 30:
      printByte(n3, n0, display);
      break;
    case 31:
      printByte(n3, n1, display);
      break;
    case 32:
      printByte(n3, n2, display);
      break;
    case 33:
      printByte(n3, n3, display);
      break;
    case 34:
      printByte(n3, n4, display);
      break;
    case 35:
      printByte(n3, n5, display);
      break;
    case 36:
      printByte(n3, n6, display);
      break;
    case 37:
      printByte(n3, n7, display);
      break;
    case 38:
      printByte(n3, n8, display);
      break;
    case 39:
      printByte(n3, n9, display);
      break;
    case 40:
      printByte(n4, n0, display);
      break;
    case 41:
      printByte(n4, n1, display);
      break;
    case 42:
      printByte(n4, n2, display);
      break;
    case 43:
      printByte(n4, n3, display);
      break;
    case 44:
      printByte(n4, n4, display);
      break;
    case 45:
      printByte(n4, n5, display);
      break;
    case 46:
      printByte(n4, n6, display);
      break;
    case 47:
      printByte(n4, n7, display);
      break;
    case 48:
      printByte(n4, n8, display);
      break;
    case 49:
      printByte(n4, n9, display);
      break;
    case 50:
      printByte(n5, n0, display);
      break;
    case 51:
      printByte(n5, n1, display);
      break;
    case 52:
      printByte(n5, n2, display);
      break;
    case 53:
      printByte(n5, n3, display);
      break;
    case 54:
      printByte(n5, n4, display);
      break;
    case 55:
      printByte(n5, n5, display);
      break;
    case 56:
      printByte(n5, n6, display);
      break;
    case 57:
      printByte(n5, n7, display);
      break;
    case 58:
      printByte(n5, n8, display);
      break;
    case 59:
      printByte(n5, n9, display);
      break;
    case 60:
      printByte(n6, n0, display);
      break;
    case 61:
      printByte(n6, n1, display);
      break;
    case 62:
      printByte(n6, n2, display);
      break;
    case 63:
      printByte(n6, n3, display);
      break;
    case 64:
      printByte(n6, n4, display);
      break;
    case 65:
      printByte(n6, n5, display);
      break;
    case 66:
      printByte(n6, n6, display);
      break;
    case 67:
      printByte(n6, n7, display);
      break;
    case 68:
      printByte(n6, n8, display);
      break;
    case 69:
      printByte(n6, n9, display);
      break;
    case 70:
      printByte(n7, n0, display);
      break;
    case 71:
      printByte(n7, n1, display);
      break;
    case 72:
      printByte(n7, n2, display);
      break;
    case 73:
      printByte(n7, n3, display);
      break;
    case 74:
      printByte(n7, n4, display);
      break;
    case 75:
      printByte(n7, n5, display);
      break;
    case 76:
      printByte(n7, n6, display);
      break;
    case 77:
      printByte(n7, n7, display);
      break;
    case 78:
      printByte(n7, n8, display);
      break;
    case 79:
      printByte(n7, n9, display);
      break;
    case 80:
      printByte(n8, n0, display);
      break;
    case 81:
      printByte(n8, n1, display);
      break;
    case 82:
      printByte(n8, n2, display);
      break;
    case 83:
      printByte(n8, n3, display);
      break;
    case 84:
      printByte(n8, n4, display);
      break;
    case 85:
      printByte(n8, n5, display);
      break;
    case 86:
      printByte(n8, n6, display);
      break;
    case 87:
      printByte(n8, n7, display);
      break;
    case 88:
      printByte(n8, n8, display);
      break;
    case 89:
      printByte(n8, n9, display);
      break;
    case 90:
      printByte(n9, n0, display);
      break;
    case 91:
      printByte(n9, n1, display);
      break;
    case 92:
      printByte(n9, n2, display);
      break;
    case 93:
      printByte(n9, n3, display);
      break;
    case 94:
      printByte(n9, n4, display);
      break;
    case 95:
      printByte(n9, n5, display);
      break;
    case 96:
      printByte(n9, n6, display);
      break;
    case 97:
      printByte(n9, n7, display);
      break;
    case 98:
      printByte(n9, n8, display);
      break;
    case 99:
      printByte(n9, n9, display);
      break;
  }
}
