#include "config.h"
#include <HardwareSerial.h>
#include <driver/uart.h>
#include <CodeScanner.h>
#include <WiFi.h>

void POSTRequest(char* code);

CodeScanner Scanner;

void prova(){
  //Scanner.setPreferredConfig();
  Scanner.setRawOutput(true);
  Scanner.setUpperCaseOutput(true);
  Scanner.setDigitsOutput(true);
}

void setup() {
  Serial.begin(115200);
  Serial2.begin(BAUD, SERIAL_8N1, RX_PIN, TX_PIN);
  while(!Serial || !Serial2){delay(100);}
  Serial.println("Seriali collegati");

  Scanner.setVerbose(true);       //per mandare in Serial cosa succede
  prova();
  //Scanner.setPreferredConfig();  //per configurare con le impostazioni usate normalmente

  Serial.print("Connessione a ");
  Serial.print(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connesso");
}

char buffer[BUF_SIZE];
char last[BUF_SIZE];
int nbytes = 0;
WiFiClient client;

void loop() {
  nbytes = Scanner.read(buffer);
  if(nbytes > 0){
    Serial.print(String("len: ") + nbytes + String(" codice: "));
    Serial.println(buffer);

    if(strcmp(last, buffer) == 0){
      return;
    }
    strcpy(last, buffer);

    POSTRequest(buffer, nbytes);
    Serial.println("POST inviata");
  }
  delay(80);
}


void POSTRequest(char* code, int nbytes){
  String req;
  int len = nbytes + 5;

  req =   "POST " + String(PATH) + " HTTP/1.1\r\n";
  req +=  "Host: " + String(HOST) + "\r\n";
  req +=  "Connection: close\r\n";
  req +=  "Content-type: application/x-www-form-urlencoded\r\n";
  req +=  "Content-length: " + String(len) + "\r\n";
  req +=  "\r\n";
  req +=  "code=" + String(code);

  while(!client.connect(HOST, PORT)){
    Serial.println("Errore di connessione");
    delay(1000);
  }

  client.print(req);
  Serial.println(req);

  unsigned long timeout = millis();
  while(client.available() == 0){
    if(millis() - timeout > 5000){
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
  String risp = client.readString();
  Serial.println(risp);
}
