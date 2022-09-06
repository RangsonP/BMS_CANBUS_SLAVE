/*
 * ESP32 BMS CAN communication to VCU Unit
 * Author : Rangson Pluemkamon
 * Version : 1.00
 * Date create/modified : 04-08-21
 */

#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"
#include <Arduino_JSON.h>

#include "CANID.h"
#include "CANmessage.h"
#include "DataConv.h"
#include "Function.h"
#include "Variables.h"

#include <driver/can.h>
#include <driver/gpio.h>

// Initialize LittleFS
void initSPIFFS() {
if (!SPIFFS.begin()) {
Serial.println("An error has occurred while mounting LittleFS");
}
Serial.println("LittleFS mounted successfully");
}


// Initialize WiFi
void initWiFi() {
WiFi.mode(WIFI_STA);
WiFi.begin(ssid, password);
Serial.print("Connecting to WiFi ..");
while (WiFi.status() != WL_CONNECTED) {
Serial.print('.');
delay(500);
}
Serial.println(WiFi.localIP());
}




//---------- Set up --------------------------------------------//
void setup() {
// Serial port for debugging purposes
Serial.begin(115200);
Serial1.begin(115200, SERIAL_8N1, RXD2, TXD2);
initWiFi();
initSPIFFS();

// Web Server Root URL
server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){request->send(SPIFFS, "/index.html", "text/html");});
server.serveStatic("/", SPIFFS, "/");

// Request for the latest sensor readings
server.on("/readings", HTTP_GET, [](AsyncWebServerRequest *request){
String json  = getSensorReadings();
request->send(200, "application/json", json);
json = String();
});



events.onConnect([](AsyncEventSourceClient *client){
if(client->lastId()){
Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
}
// send event with message "hello!", id current millis
// and set reconnect delay to 1 second
client->send("hello!", NULL, millis(), 10000);
});
server.addHandler(&events);
// Start server
server.begin();

setup_can_driver();

}


//--------------------------------------------//
void loop() {

//server.begin();

  if(Serial1.available()){
    data_input = Serial1.readStringUntil('\n');
     if(data_input.charAt(0) == 'B'){
        get = String(data_input.charAt(1));
        number1 = get.toInt();
        number1 = number1*10;
        get2 = String(data_input.charAt(2));
        number2 = get2.toInt();
        number1 = number1 + number2;
        for(int i=4;i<10;i++){
        get3 += data_input.charAt(i);
        A[number1] = get3.toInt();
        B[number1] = A[number1]; 
      }
    }
    if(data_input.charAt(0) == 'T'){
        get = String(data_input.charAt(1));
        number1 = get.toInt();
        number1 = number1*10;
        get2 = String(data_input.charAt(2));
        number2 = get2.toInt();
        number1 = number1 + number2;
        for(int i=4;i<10;i++){
        get3 += data_input.charAt(i);
        A[number1] = get3.toInt();
        T[number1] = A[number1]; 
      }
    }
    if(data_input.charAt(0) == 'P'){
        get = String(data_input.charAt(1));
        number1 = get.toInt();
        number1 = number1*10;
        get2 = String(data_input.charAt(2));
        number2 = get2.toInt();
        number1 = number1 + number2;
        for(int i=4;i<10;i++){
        get3 += data_input.charAt(i);
        A[number1] = get3.toInt();
        V[number1] = A[number1]; 
      }
    }
    if(data_input.charAt(0) == 'S'){
        get = String(data_input.charAt(1));
        number1 = get.toInt();
        number1 = number1*10;
        get2 = String(data_input.charAt(2));
        number2 = get2.toInt();
        number1 = number1 + number2;
        for(int i=4;i<10;i++){
        get3 += data_input.charAt(i);
        A[number1] = get3.toInt();
        S[number1] = A[number1]; 
      }
    }
    if(data_input.charAt(0) == 'C'){
        get = String(data_input.charAt(1));
        number1 = get.toInt();
        number1 = number1*10;
        get2 = String(data_input.charAt(2));
        number2 = get2.toInt();
        number1 = number1 + number2;
        for(int i=4;i<10;i++){
        get3 += data_input.charAt(i);
        A[number1] = get3.toInt();
        C[number1] = A[number1]; 
      }
    }
  }

    if ((millis() - lastTime) > timerDelay) {
      // Send Events to the client with the Sensor Readings Every 30 seconds
      events.send("ping",NULL,millis());
      events.send(getSensorReadings().c_str(),"new_readings" ,millis());
      lastTime = millis();
      //Serial.println("Data has sent");
      //Serial.println(getSensorReadings());
      Serial.print(WiFi.localIP());
      Serial.print(",");
      for(int i = 1; i<21;i++){
      Serial.print(B[i]);
      Serial.print(",");
      }
      for(int k = 1 ; k<6;k++){
      Serial.print(T[k]);
      Serial.print(",");
      }
      Serial.print(V[1]);
      Serial.print(",");
      Serial.print(C[1]);
      Serial.print(",");
      Serial.print(S[1]);  
      Serial.println("");
      
    }

    // Create BY AUY
    GetValue();

    sys_voltage = sys_voltage_conv(VP[0]); // voltage 72 v
    sys_current = sys_current_conv(BC[0]); // current 30 A
    sys_soc = sys_soc_conv(BS[0]); // Soc = 60%
    
    bms_state();
    cell_vol_1();
    cell_vol_2();
    cell_temperature();
    battery_sys_state();
    battery_spec();
    accumulate_data();
    soh();


    data_input = "";
    get = "";
    get2 = "";
    get3 = "";
    number1 = 0;
    number2 = 0;
    //A[21] = "";
    //B[21] = "";
    //C[1]  = "";
    //T[5]  = "";
    //S[1]  = "";
    //V[1]  = "";

}

