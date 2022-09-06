#ifndef CANMESSAGE_H
#define CANMESSAGE_H

#include <Arduino.h>

#include "CANID.h"
#include "Variables.h"
#include "DataConv.h"
#include "driver/gpio.h"
#include "driver/can.h"

// Setup CAN-BUS driver
void setup_can_driver(){
  can_general_config_t g_config = { // Create g_config for setup CAN Parameter
    .mode = CAN_MODE_NORMAL,
    .tx_io = GPIO_NUM_26, // CAN TX Pin GPIO_26
    .rx_io = GPIO_NUM_27, // CAN RX Pin GPIO_27
    .clkout_io = ((gpio_num_t)CAN_IO_UNUSED),
    .bus_off_io = ((gpio_num_t)CAN_IO_UNUSED),
    .tx_queue_len = 5,
    .rx_queue_len = 5,
    .alerts_enabled = CAN_ALERT_ALL,
    .clkout_divider = 0
  };
  can_timing_config_t t_config = CAN_TIMING_CONFIG_250KBITS(); // Set baud rate 250Kbps
  can_filter_config_t f_config = CAN_FILTER_CONFIG_ACCEPT_ALL();

  if (can_driver_install(&g_config, &t_config, &f_config) != ESP_OK) {
    Serial.println("Failed to install driver");
    return;
  }

  if (can_start() != ESP_OK) {
    Serial.println("Failed to start driver");
    return;
  }
}

/*
 * Create function for CAN message function
 * List of message state : BMS_State, Cell_Vol State, Cell Temperature State, BattSysState
 */

// Send BMS State message
void bms_state(){
  byte byte5 = (sys_status << 4) + (chg_link << 2) + (chg_indication << 0);
  byte byte6 = (err_level << 6) + (bms_self_check << 4) + (power_off_rqst << 2) + (insu_fade << 0);
  can_message_t txmessage;
  txmessage.identifier = bms_state_id; // message id for bms state
  txmessage.flags = CAN_MSG_FLAG_EXTD;
  txmessage.data_length_code = 8; // 8 Byte Data Send
  txmessage.data[0] = sys_voltage >> 8;
  txmessage.data[1] = sys_voltage;
  txmessage.data[2] = sys_current >> 8;
  txmessage.data[3] = sys_current;
  txmessage.data[4] = sys_soc; // Soc = N*x + C by x = 0.4
  txmessage.data[5] = byte5;
  txmessage.data[6] = byte6;
  txmessage.data[7] = can_counter;
  // Send can message
  if (can_transmit(&txmessage, pdMS_TO_TICKS(10)) == ESP_OK) {
    Serial.println("message queued for transmission");
  } else {
    Serial.println("Failed to queue txmessage for transmission");
  }
  can_counter ++;
  delayMicroseconds(10);
}

// Send cell voltage 1 state message
void cell_vol_1(){
  can_message_t txmessage;
  txmessage.identifier = cell_vol_1_id; // message id for cell vol 1
  txmessage.flags = CAN_MSG_FLAG_EXTD;
  txmessage.data_length_code = 8; // 8 Byte Data Send
  txmessage.data[0] = cell_max_vol >> 8;
  txmessage.data[1] = cell_max_vol;
  txmessage.data[2] = cell_min_vol >> 8;
  txmessage.data[3] = cell_min_vol;
  txmessage.data[4] = max_vol_cell_num;
  txmessage.data[5] = max_vol_grp_num;
  txmessage.data[6] = min_vol_cell_num;
  txmessage.data[7] = min_vol_grp_num;
  
  if (can_transmit(&txmessage, pdMS_TO_TICKS(10)) == ESP_OK) {
    Serial.println("message queued for transmission");
  } else {
    Serial.println("Failed to queue txmessage for transmission");
  }
  delayMicroseconds(10);
}

// Send cell voltage 2 state message
void cell_vol_2(){
  can_message_t txmessage;
  txmessage.identifier = cell_vol_2_id; // message id for cell vol 2
  txmessage.flags = CAN_MSG_FLAG_EXTD;
  txmessage.data_length_code = 8; // 8 Byte Data Send
  txmessage.data[0] = cell_ave_vol >> 8;
  txmessage.data[1] = cell_ave_vol;
  txmessage.data[2] = cell_dev_vol >> 8;
  txmessage.data[3] = cell_dev_vol;
  txmessage.data[4] = chg_current_limit >> 8;
  txmessage.data[5] = chg_current_limit;
  txmessage.data[6] = dchg_current_limit >> 8;
  txmessage.data[7] = dchg_current_limit;
  
  if (can_transmit(&txmessage, pdMS_TO_TICKS(10)) == ESP_OK) {
    Serial.println("message queued for transmission");
  } else {
    Serial.println("Failed to queue txmessage for transmission");
  }
  delayMicroseconds(10);
}

// Send cell temperature message
void cell_temperature(){
  can_message_t txmessage;
  txmessage.identifier = cell_temperature_id; // message id for cell temp
  txmessage.flags = CAN_MSG_FLAG_EXTD;
  txmessage.data_length_code = 8; // 8 Byte Data Send
  txmessage.data[0] = cell_max_temp;
  txmessage.data[1] = max_temp_cell_num;
  txmessage.data[2] = max_temp_grp_num;
  txmessage.data[3] = cell_min_temp;
  txmessage.data[4] = min_temp_cell_num;
  txmessage.data[5] = min_temp_grp_num;
  txmessage.data[6] = cell_ave_temp;
  txmessage.data[7] = cell_dev_temp;

  if (can_transmit(&txmessage, pdMS_TO_TICKS(10)) == ESP_OK) {
    Serial.println("message queued for transmission");
  } else {
    Serial.println("Failed to queue txmessage for transmission");
  }
  delayMicroseconds(10);
}

// Send battery system state message
void battery_sys_state(){
  byte byte1 = (cell_vol_high << 6) + (cell_vol_low << 4) + (tem_high << 2) + (tem_low << 0);
  byte byte2 = (sum_vol_high << 6) + (sum_vol_low << 4) + (dis_current_over << 2) + (chg_current_over << 0);
  byte byte3 = (soc_high << 6) + (soc_low << 4) + (det_temp << 2) + (det_cell_vol << 0);
  byte byte4 = (bms_commu_fault << 6) + (vcu_timeout_malf << 4) + (sum_vol_diff_malf << 2) + (port_temp_high << 0);
  byte byte5 = (st_pos_relay << 6) + (st_chg_relay << 4) + (st_neg_relay << 2) + (st_prechg_relay << 0);
  byte byte6 = (dc_dc_relay << 6) + (air_pump_relay << 4) + (port_temp_status << 2) + (0 << 0);
  can_message_t txmessage;
  txmessage.identifier = battery_sys_state_id; // message id for Battery Sys State
  txmessage.flags = CAN_MSG_FLAG_EXTD;
  txmessage.data_length_code = 8; // 8 Byte Data Send
  txmessage.data[0] = byte1;
  txmessage.data[1] = byte2;
  txmessage.data[2] = byte3;
  txmessage.data[3] = byte4;
  txmessage.data[4] = byte5;
  txmessage.data[5] = byte6;
  txmessage.data[6] = max_chg_current >> 8;
  txmessage.data[7] = max_chg_current;

  if (can_transmit(&txmessage, pdMS_TO_TICKS(10)) == ESP_OK) {
    Serial.println("message queued for transmission");
  } else {
    Serial.println("Failed to queue txmessage for transmission");
  }
  delayMicroseconds(10);
}

// Send battery spec message
void battery_spec(){
  can_message_t txmessage;
  txmessage.identifier = battery_spec_id; // message id for Battery Spec
  txmessage.flags = CAN_MSG_FLAG_EXTD;
  txmessage.data_length_code = 8; // 8 Byte Data Send
  txmessage.data[0] = batt_vendor_nr;
  txmessage.data[1] = bms_serial_nr >> 8;
  txmessage.data[2] = bms_serial_nr;
  txmessage.data[3] = v_max_cell_spec >> 8;
  txmessage.data[4] = v_max_cell_spec;
  txmessage.data[5] = t_max_cell_spec;
  txmessage.data[6] = I_ch_max_cell_spec >> 8;
  txmessage.data[7] = I_ch_max_cell_spec;

  if (can_transmit(&txmessage, pdMS_TO_TICKS(10)) == ESP_OK) {
    Serial.println("message queued for transmission");
  } else {
    Serial.println("Failed to queue txmessage for transmission");
  }
  delayMicroseconds(10);
}

// Send accumulate data message
void accumulate_data(){
  can_message_t txmessage;
  txmessage.identifier = accumulate_data_id; // message id for Battery Spec
  txmessage.flags = CAN_MSG_FLAG_EXTD;
  txmessage.data_length_code = 8; // 8 Byte Data Send
  txmessage.data[0] = acc_charge >> 8;
  txmessage.data[1] = acc_charge;
  txmessage.data[2] = acc_discharge >> 8;
  txmessage.data[3] = acc_discharge;
  txmessage.data[4] = master_timer >> 24;
  txmessage.data[5] = master_timer >> 16;
  txmessage.data[6] = master_timer >> 8;
  txmessage.data[7] = master_timer;

  if (can_transmit(&txmessage, pdMS_TO_TICKS(10)) == ESP_OK) {
    Serial.println("message queued for transmission");
  } else {
    Serial.println("Failed to queue txmessage for transmission");
  }
  delayMicroseconds(10);
}

// Send soh message
void soh(){
  byte byte1 = 0b00000000;
  byte byte2 = 0b00000000;
  byte byte3 = 0b00000000;
  byte byte4 = 0b00000000;
  byte byte7 = 0b00000000;
  byte byte8 = 0b00000000;
  can_message_t txmessage;
  txmessage.identifier = soh_id; // message id for Battery Spec
  txmessage.flags = CAN_MSG_FLAG_EXTD;
  txmessage.data_length_code = 8; // 8 Byte Data Send
  txmessage.data[0] = byte1;
  txmessage.data[1] = byte2;
  txmessage.data[2] = byte3;
  txmessage.data[3] = byte4;
  txmessage.data[4] = soh_data >> 8; // Soh data max 100
  txmessage.data[5] = soh_data;
  txmessage.data[6] = byte7;
  txmessage.data[7] = byte8;

  if (can_transmit(&txmessage, pdMS_TO_TICKS(10)) == ESP_OK) {
    Serial.println("message queued for transmission");
  } else {
    Serial.println("Failed to queue txmessage for transmission");
  }
  delayMicroseconds(10);
}




#endif