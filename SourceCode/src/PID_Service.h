#ifndef _PID_SERVICE_H
#define _PID_SERVICE_H

#include <Arduino.h>
#include <QuickPID.h>
#include <GPIO_Service.h>
#include <MAX6675_Service.h>
#include "variables.h"
#include <lvgl.h>
#include "ui/ui.h"
#include "numberflow.h"

// 创建float类型的中间变量用于PID计算
static float soldering_temp_float = 0.0f;
static float soldering_target_temp_float = 0.0f;

// 待机休眠相关变量
static struct {
    unsigned long standby_start_time;   // 待机开始时间
    bool ui_color_changed;             // UI颜色是否已改变
    bool in_standby_mode;              // 当前是否处于待机模式 (由防抖后的sleep信号决定)
    unsigned long sleep_read_time;     // 休眠状态下的读取时间控制

    // 新增防抖相关成员
    bool prev_raw_sleep_signal;         // 上一次读取的原始 sleep 信号
    bool debounced_sleep_signal;        // 防抖处理后的 sleep 信号
    unsigned long last_raw_signal_change_time; // 原始信号上一次发生变化的时间戳
    bool last_processed_debounced_signal; // 上一次处理过的（用于边沿检测）防抖信号
} standby_state = {
    0,      // standby_start_time
    false,  // ui_color_changed
    false,  // in_standby_mode
    0,      // sleep_read_time
    false,  // prev_raw_sleep_signal (将在Init中正确设置)
    false,  // debounced_sleep_signal (将在Init中正确设置)
    0,      // last_raw_signal_change_time
    false   // last_processed_debounced_signal (将在Init中正确设置)
};

// 添加状态机枚举和静态变量
enum SolderingPIDState {
    PID_POWER_OFF,
    PID_WAIT_STABLE,
    PID_READ_TEMP,
    PID_HEATING,
    PID_SLEEP           // 休眠状态
};

static SolderingPIDState pid_state = PID_POWER_OFF;
static unsigned long state_start_time = 0;

// 蜂鸣器控制变量 - 重新组织以提高缓存效率
static struct {
    bool temperature_reached_played;    // 是否已鸣叫过到达温度
    bool short_active;                  // 短鸣叫是否激活
    bool long_active;                   // 长鸣叫是否激活
    unsigned long start_time;           // 蜂鸣器开始时间
} buzzer_state = {false, false, false, 0};

// 状态跟踪变量
static struct {
    uint16_t last_target_temp;          // 上次的目标温度
    bool was_disabled;                  // 电烙铁之前是否被禁用
} soldering_state = {0, true};

void Soldering_PID_Compute_Init();
void Soldering_PID_Update_Tunings(float Kp, float Ki, float Kd);
void Soldering_PID_Compute();

// 创建float类型的中间变量用于PID计算
static float heatgun_temp_float = 0.0f; //转换Heatgun_Temp为float类型
static float heatgun_target_temp_float = 0.0f; //转换HeatgunTargetTemp为float类型

// ------ 热风枪 PID状态定义 ------//
enum HeatgunPIDState {
    HEATGUN_PID_OFF,
    HEATGUN_PID_HEATING,
    HEATGUN_PID_SLEEP_COOLING, // 进入休眠，风扇100%降温
    HEATGUN_PID_SLEEP_IDLE     // 休眠中，已降温到安全温度，风扇维持低速运转
};
static HeatgunPIDState heatgun_pid_state = HEATGUN_PID_OFF;

struct HeatgunSleepState {
    bool prev_raw_sleep_signal;
    bool debounced_sleep_signal_is_low;
    unsigned long last_raw_signal_change_time;
    bool last_processed_debounced_signal_is_low;
    bool in_sleep_mode_active;
    bool ui_color_changed;
};
static HeatgunSleepState heatgun_sleep_state;

// 新增：热风枪蜂鸣器状态
struct HeatgunBuzzerState {
    unsigned long start_time;
    bool short_active;
    bool long_active;
    bool temperature_reached_played;
};
static HeatgunBuzzerState heatgun_buzzer_state;

// 新增：热风枪内部状态，用于辅助蜂鸣器逻辑
struct HeatgunInternalState {
    bool was_disabled;
    uint16_t last_target_temp;
};
static HeatgunInternalState heatgun_internal_state;

const unsigned long HEATGUN_SLEEP_DEBOUNCE_DURATION_MS = 200;
const uint16_t HEATGUN_SAFETY_MAX_TEMP_OFFSET = 15;
const int HEATGUN_TEMP_REACHED_THRESHOLD = 3; // 温度到达提示的阈值（+/- 3度）
const uint16_t HEATGUN_SLEEP_COOL_DOWN_TEMP_STAGE1 = 150; // 新增：冷却第一阶段阈值
const uint16_t HEATGUN_SLEEP_COOL_DOWN_TEMP_STAGE2 = 100; // 重命名/明确 HEATGUN_SLEEP_COOL_DOWN_TEMP
const uint16_t HEATGUN_SLEEP_COOL_DOWN_TEMP_STAGE3 = 60;  // 新增：冷却第三阶段阈值 (风扇关闭)

void Heatgun_PID_Compute_Init();
void HeatgunPID_Update_Tunings(float Kp, float Ki, float Kd);
void Heatgun_PID_Compute();

#endif