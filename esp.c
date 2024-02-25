//Define Required Libraries
#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiClientSecureBearSSL.h>
#include <AccelStepper.h>

//Setup Wifi and HTTPS
BearSSL::WiFiClientSecure httpsClient;
const char *ssid = "1819_Guest";

//Define Stepper Pins
#define IN1 5
#define IN2 4
#define IN3 14
#define IN4 12

//Declare Stepper Vars/Library
bool a = false; 
const int end = 1024;
AccelStepper stepper1(4, IN1, IN3, IN2, IN4);

//Define HTTPS Request String
String payload;

// Define NTP Client to get time (Eastern)
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

//Define 'findText' to Parse thru payload's json
String findText(String input, String keyword, int read) {
  int keywordPos = input.indexOf(keyword);
  if (keywordPos == -1) {
    return ""; // Keyword not found
  }
  int start = keywordPos + keyword.length();
  return input.substring(start, start + read);
}

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);


  //Setup Stepper 1 Attributes
  stepper1.setMaxSpeed(3500.0);
  stepper1.setAcceleration(100.0);
  stepper1.setSpeed(3500);

  // Connect to Wi-Fi
  WiFi.begin(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  httpsClient.setInsecure(); // Ignore SSL certificate errors; use with caution

// Initialize a NTPClient to get time
  timeClient.begin();
  // Set offset time in seconds to adjust for your timezone, for example:
  // GMT +1 = 3600
  timeClient.setTimeOffset(-18000);
}

//Rotate Stepper 1
void rotate(int n) {
  for(int i =0; i<n;i++)
  {
  stepper1.moveTo(end);
  //Serial.println(stepper1.currentPosition());
  if(stepper1.currentPosition()==end)
  {
  stepper1.setCurrentPosition(0);
  a=false;
  }
  }
}

void loop() {
  //Update NTP
  timeClient.update();

  //Save Surrent Hour and Minute
  int currentHour = timeClient.getHours();
  int currentMinute = timeClient.getMinutes();

  //Uses NTP to Find, Save and Print Current Time
  String formaltime = currentHour + ":" + currentMinute;
  Serial.print(currentHour);  
  Serial.print(":");
  Serial.println(currentMinute);  

  HTTPClient https;  
    https.begin(httpsClient, "https://baymax.kintone.com/k/v1/record.json?app=1&id=1"); 
    https.addHeader("X-Cybozu-API-Token", "BYJglQu3jRqJcVbGrjZ8h3Q4tWkBXZzVdPGUauOX"); 
    int httpCode = https.GET();                                       
    if (httpCode > 0) { 
      payload = https.getString();
    }
    https.end(); 

  //Declare Strings to Be Found in the Json Payload
  String ft0 = "Time_0\":{\"type\":\"TIME\",\"value\":\"";
  String ft1 = "Time_1\":{\"type\":\"TIME\",\"value\":\"";
  String ft2 = "Time_2\":{\"type\":\"TIME\",\"value\":\"";
  String fnump = "\"num\":{\"type\":\"SINGLE_LINE_TEXT\",\"value\":\"";

  //Declare Result Values
  String t0 = findText(payload, ft0, 5);
  String t1 = findText(payload, ft1, 5);
  String t2 = findText(payload, ft2, 5);
  String nump = findText (payload, fnump, 1);

  //Print all Values to Serial Console for Debugging
  Serial.println(t0);
  Serial.println(t1);
  Serial.println(t2);
  Serial.println(nump);
  Serial.println("");
  
  //Determine if medicine must dispense
  if(formaltime.equals(t0) || t1.equals(t1) || formaltime.equals(t2)){
    for (int i = 1; i <= nump.toInt(); i++){
      stepper1.setMaxSpeed(3500.0);
      stepper1.setAcceleration(100.0);
      stepper1.setSpeed(3500);
      rotate(1);
    }
  }
  else{
    stepper1.setMaxSpeed(0);
    stepper1.setAcceleration(0);
    stepper1.setSpeed(0);
    return;
  }

  //Set Delay before Looping Again
  delay(5000);
}
