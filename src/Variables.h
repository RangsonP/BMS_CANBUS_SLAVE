#ifndef VARIABLES_H
#define VARIABLES_H

#include <Arduino.h>

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"
#include <Arduino_JSON.h>

// BMS State data assign
uint16_t sys_voltage, sys_current; // 16-bit data
uint8_t can_counter; // 8-bit variables
uint8_t sys_soc, sys_status, chg_link, chg_indication, err_level, bms_self_check, power_off_rqst, insu_fade; // 1-4 bit data

//------
String data_in = "";
uint32_t D[28] = {40723,40654,40676,40680,40707,40661,40661,40667,40705,40621,40672,40647,40644,40663,40658,40634,40680,40623,40657,40691,3201,3097,3130,3100,3123,81,93,25};

uint16_t BV[20]; // Battery voltage data in array
uint8_t BT[5]; // Battery tmeperature data in array
uint16_t VP[1]; // Battery pack voltage in array
uint16_t BC[1]; // Battery pack current in array
uint8_t BS[1]; // Battery pack SoC

uint16_t cell_max_vol; // cell max voltage
uint16_t cell_min_vol; // cell min voltage

uint8_t cell_max_temp, cell_max_tempF;
uint8_t cell_min_temp, cell_min_tempF;


// Cell_vol_1, Cell_Vol_2 data assign
uint16_t cell_ave_vol, cell_dev_vol, chg_current_limit, dchg_current_limit;; // 16-bit data
uint8_t max_vol_cell_num, max_vol_grp_num, min_vol_cell_num, min_vol_grp_num; // 8-bit data

// Cell Temperature data assign
uint8_t max_temp_cell_num, max_temp_grp_num, min_temp_cell_num, min_temp_grp_num, cell_ave_temp, cell_dev_temp; // 8-bit data

// Battery Sys State data assign
uint16_t max_chg_current; // 16-bit data
uint8_t cell_vol_high, cell_vol_low, tem_high, tem_low; // byte 1 data
uint8_t sum_vol_high, sum_vol_low, dis_current_over, chg_current_over; // byte 2 data
uint8_t soc_high, soc_low, det_temp, det_cell_vol; // byte 3 data
uint8_t bms_commu_fault, vcu_timeout_malf, sum_vol_diff_malf, port_temp_high; // byte 4 data
uint8_t st_pos_relay, st_chg_relay, st_neg_relay, st_prechg_relay, dc_dc_relay; // byte 5 data
uint8_t air_pump_relay, port_temp_relay, port_temp_status; // byte 6 data

// Battery Spec data assign
uint16_t bms_serial_nr, v_max_cell_spec, I_ch_max_cell_spec; // 16-bit data
uint8_t batt_vendor_nr, t_max_cell_spec; // 8-byte data

// Accumulate Spec data assign
uint32_t master_timer; // 32-bit data
uint16_t acc_charge, acc_discharge; // 16-bit data

// SOH data assign
uint16_t soh_data;

//---- String Set parameter -------------//
int string_count = 0;
String data_input = "";
String A[21] = {};
String get = "";
String get2 = "";
String get3 = "";
int number1 = 0;
int number2 = 0;

//------ Uart pin configuration ---------//
#define RXD2 16
#define TXD2 17

//---------- Battery data -------------//
String B[21] = {};
String C[2]  = {};
String T[6]  = {};
String S[2]  = {};
String V[2]  = {};


// Replace with your network credentials
const char* ssid = "Jedi_2.4G";
const char* password = "038587413";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Create an Event Source on /events
AsyncEventSource events("/events");

// Json Variable to Hold Sensor Readings
JSONVar readings;

// Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 4000;


// Get Sensor Readings and return JSON object
String getSensorReadings(){
//--------- Data sending from string
readings["volt1"] =  B[1] ;//String(random(100,120));
readings["volt2"] =  B[2] ;//String(random(100,120));
readings["volt3"] =  B[3] ;//String(random(100,120));
readings["volt4"] =  B[4] ;//String(random(100,120));
readings["volt5"] =  B[5] ;//String(random(100,120));
readings["volt6"] =  B[6] ;//String(random(100,120));
readings["volt7"] =  B[7] ;//String(random(100,120));
readings["volt8"] =  B[8] ;//String(random(100,120));
readings["volt9"] =  B[9] ;//String(random(100,120));
readings["volt10"] =  B[10] ;//String(random(100,120));
readings["volt11"] =  B[11] ;//String(random(100,120));
readings["volt12"] =  B[12] ;//String(random(100,120));
readings["volt13"] =  B[13] ;//String(random(100,120));
readings["volt14"] =  B[14] ;//String(random(100,120));
readings["volt15"] =  B[15] ;//String(random(100,120));
readings["volt16"] =  B[16] ;//String(random(100,120));
readings["volt17"] =  B[17] ;//String(random(100,120));
readings["volt18"] =  B[18] ;//String(random(100,120));
readings["volt19"] =  B[19] ;//String(random(100,120));
readings["volt20"] =  B[20] ;//String(random(100,120));
readings["temp1"] =   T[1];
readings["temp2"] =   T[2];
readings["temp3"] =   T[3];
readings["temp4"] =   T[4];
readings["temp5"] =   T[5];
readings["voltpack"] = V[1];
readings["SoC"] =      S[1];
readings["current"] =  C[1];
String jsonString = JSON.stringify(readings);
return jsonString;
}


#endif