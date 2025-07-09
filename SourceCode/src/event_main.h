#ifndef _EVENT_MAIN_H_
#define _EVENT_MAIN_H_

#include <Arduino.h>
#include <lvgl.h>
#include <ui/ui.h>
#include <Encoder_Service.h>
#include <variables.h>
#include <lvgl_group.h>
#include <GPIO_Service.h>
#include <MAX6675_Service.h>
#include <flash.h>

void handle_soldering_temp_encoder_edit();
void handle_encoder_parameters_edit();
void handle_parameter_change(uint16_t* parameter, int diff,
                            uint16_t min_value, uint16_t max_value,
                            lv_obj_t* ui_object, const char* format);

#endif