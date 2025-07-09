#ifndef _LVGL_GROUP_H
#define _LVGL_GROUP_H

#include <lvgl.h>
#include <Encoder_Service.h>
#include "ui/ui.h"

void lvgl_group_init();
void lvgl_group_to_setting();
void lvgl_group_to_main();
void lvgl_group_to_soldering();
void lvgl_group_to_heatgun();
void lvgl_group_to_pid_setting();

#endif