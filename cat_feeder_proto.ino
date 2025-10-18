#include <ESP32Servo.h>
#include "CTBot.h"
#include <NTPClient.h>
#include <WiFiUdp.h>

Servo servo1;
CTBot myBot;
scheduleStartTime = 09.00;
String ssid = "RA-ROFA";
String pass = "rarofa456";
String token = "6567467409:AAFrFE2Ll5dgdjkCd3q_qE241i-Ov4UKk2A";
uint8_t led1 = 25;
int tutup = 0;
int buka = 90;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

void setup() {
  Serial.begin(115200);
  Serial.println("Starting TelegramBot...");
  servo1.attach(24);
  myBot.wifiConnect(ssid, pass);
  myBot.setTelegramToken(token);

  if (myBot.testConnection())
    Serial.println("\ntestConnection OK");
  else
    Serial.println("\ntestConnection NOK");

  pinMode(led1, OUTPUT);
}

void loop() {
  timeClient.update();
  unsigned long currentTime = timeClient.getEpochTime();

  TBMessage msg;
  if (CTBotMessageText == myBot.getNewMessage(msg)) {

    if (msg.text.equalsIgnoreCase("LIGHT1 ON")) {
      digitalWrite(led1, HIGH);
      myBot.sendMessage(msg.sender.id, "Light1 is now ON");
    } else if (msg.text.equalsIgnoreCase("LIGHT1 OFF")) {
      digitalWrite(led1, LOW);
      myBot.sendMessage(msg.sender.id, "Light1 is now OFF");
    } else if (msg.text.equalsIgnoreCase("SCHEDULE OPEN")) {
      tutup = 0;
      buka = 90;
      myBot.sendMessage(msg.sender.id, "Scheduled opening activated");
    } else if (msg.text.equalsIgnoreCase("SCHEDULE CLOSE")) {
      tutup = 90;
      buka = 0;
      myBot.sendMessage(msg.sender.id, "Scheduled closing activated");
    } else {
      String reply = "Welcome " + msg.sender.username + ". Try LIGHT1 ON, LIGHT1 OFF, SCHEDULE OPEN, or SCHEDULE CLOSE.";
      myBot.sendMessage(msg.sender.id, reply);
    }
  }

  // Check the schedule and control the servo accordingly
  if (currentTime >= scheduleStartTime && currentTime <= scheduleEndTime) {
    // It's within the schedule, so open the servo
    servo1.write(buka);
  } else {
    // It's outside the schedule, so close the servo
    servo1.write(tutup);
  }

  delay(500);
}
