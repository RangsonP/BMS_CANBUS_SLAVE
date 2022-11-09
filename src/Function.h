#ifndef FUNCTION_H
#define FUCNTION_H

#include <Arduino.h>
#include <stdint.h>

#include "CANID.h"
#include "Variables.h"
#include "DataConv.h"
#include "driver/gpio.h"
#include "driver/can.h"

void getCellVolData(){
  for(int i=0; i<20; i++){
    BV[i] = B[i].toInt() * 0.1;
  }
}

void getBatteryTempData(){
  for(int i=0; i<5; i++){
    // BT[i] = T[i].toInt() * 0.01;
    BT[i] = T[i].toFloat();
  }
}

void getVolPackData(){
  for(int i=0; i<1; i++){
    // VP[i] = V[i].toInt() * 10;
    VP[i] = V[i].toFloat();
  }
}

void getSoC(){
  for(int i=0; i<1; i++){
    // BS[i] = S[i].toInt();
    BS[i] = S[i].toFloat();
  }
}

void getCurrent(){
  for(int i=0; i<1; i++){
    // BC[i] = C[i].toInt() * 10;
    BC[i] = C[i].toFloat();
  }
}

// Get cell voltage min-max, ave-dev, number of cell
void GetCellVoltMinMax(){
  uint32_t sum = 0;
  int min = INT16_MAX, max = INT16_MIN;
  for(int i:BV){
    if(i < min){
      min = i;
      cell_min_vol = min;
    }
    if(i > max){
      max = i;
      cell_max_vol = max;
    }
  }
  for(int i=0; i<20 ;i++){
    if(BV[i] == cell_min_vol){
      min_vol_cell_num = i + 1;
    }
    if(BV[i] == cell_max_vol){
      max_vol_cell_num = i + 1;
    }
  }
  for(int i=0; i<20 ;i++){
    sum += BV[i];
    if(i == 19){
      cell_ave_vol = sum/20;
      sum = 0;
    }
  }
}

// Get cell temperature min-max, ave-dev, number of cell
void GetCellTempMinMax(){
  uint32_t sum = 0;
  int min = INT8_MAX, max = INT8_MIN;
  for(int i:BT){
    if(i < min){
      min = i;
      cell_min_tempF = min;
      cell_min_temp = cell_min_temp_conv(min);
    }
    if(i > max){
      max = i;
      cell_max_tempF = max;
      cell_max_temp = cell_max_temp_conv(max);
    }
  }
  for(int i=0; i<5; i++){
    if(BT[i] == cell_min_tempF){
      min_temp_cell_num = i + 1;
    }
    if(BT[i] == cell_max_tempF){
      max_temp_cell_num = i + 1;
    }
  }
  for(int i=0; i<5; i++){
    sum += BT[i];
    if(i == 4){
      cell_ave_temp = cell_ave_temp_conv(sum/5);
    }
  }
}

void readSerial(){
  for(int i=0; i<28; i++){
    data_in = Serial.readStringUntil(',');
    D[i] = data_in.toInt();
  }
}

// Get All values
void GetValue(){
  getCellVolData();
  getBatteryTempData();
  getVolPackData();
  getSoC();
  getCurrent();
  GetCellVoltMinMax();
  GetCellTempMinMax();
}






#endif