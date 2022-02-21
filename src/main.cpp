#include <Arduino.h>
#include <SoftwareSerial.h>
#include <EmonLib.h>
#include <ArduinoJson.h>

EnergyMonitor enel, pannelli;
double Ienel, Ipannelli;

SoftwareSerial mySerial(9, 8); // RX, TX

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  // Serial.begin(9600);
  mySerial.begin(9600);

  enel.current(A1, 58);
  pannelli.current(A0, 58);
}

void loop()
{
  String json;
  StaticJsonDocument<200> doc;

  Ienel = enel.calcIrms(1480) - 0.06;
  Ipannelli = pannelli.calcIrms(1480) - 0.06;

  if (Ipannelli < 0)
    Ipannelli = 0;

  if (Ienel < 0)
    Ienel = 0;

  int watEnel = (int)Ienel * 230;
  int watPannelli = (int)Ipannelli * 230;

  doc["in"] = watEnel;
  doc["out"] = watPannelli;
  serializeJson(doc, json);

  mySerial.print(json);

  delay(1000);
}
