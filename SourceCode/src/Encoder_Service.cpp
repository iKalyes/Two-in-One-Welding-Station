#include <Encoder_Service.h>

RotaryEncoder *encoder = NULL;

// 蜂鸣器控制变量
static unsigned long buzzer_start_time = 0;
static bool buzzer_active = false;
static const unsigned long BUZZER_DURATION = 5; // 蜂鸣器响5ms
static unsigned long last_buzzer_trigger_time = 0;  // 上次触发蜂鸣器的时间
static const unsigned long BUZZER_INTERVAL = 100;   // 两次蜂鸣之间的最小间隔(ms)

void encoder_tick()  // Interrupt Service Routine for the encoder
{
    encoder->tick();
}

void encoder_init()
{
    encoder = new RotaryEncoder(17, 16, RotaryEncoder::LatchMode::FOUR3);
    attachInterrupt(digitalPinToInterrupt(17), encoder_tick, CHANGE); // Attach interrupt to pin 17
    attachInterrupt(digitalPinToInterrupt(16), encoder_tick, CHANGE); // Attach interrupt to pin 16
    pinMode(15, INPUT_PULLUP); // Pin 15 for SW
}

// 跟踪编码器状态
static int last_encoder_dir = 0;
static bool encoder_btn_pressed = false;
static bool last_btn_state = false;
static unsigned long last_btn_change_time = 0;
static const unsigned long DEBOUNCE_DELAY = 50; // 50ms去抖动延迟
static lv_indev_t* encoder_indev = NULL;

// 编码器读取回调函数
static void encoder_read(lv_indev_drv_t* drv, lv_indev_data_t* data)
{
    // 检测旋转
    int current_dir = (int)(encoder->getDirection());
    unsigned long current_time = millis();
    
    // 只有当蜂鸣器功能启用、检测到旋转且间隔足够长，才触发蜂鸣器
    if(Buzzer_Enabled && (current_dir == -1 || current_dir == 1) && 
       (current_time - last_buzzer_trigger_time > BUZZER_INTERVAL)) {
        // 检测到旋转且满足条件，启动蜂鸣器
        Buzzer_ON();
        buzzer_active = true;
        buzzer_start_time = current_time;
        last_buzzer_trigger_time = current_time;  // 更新上次触发时间
    }
    
    // 检查是否需要关闭蜂鸣器
    if (buzzer_active && (current_time - buzzer_start_time > BUZZER_DURATION)) {
        Buzzer_OFF();
        buzzer_active = false;
    }
    
    if(current_dir == -1) {
        data->enc_diff = 1;  // 顺时针
    } else if(current_dir == 1) {
        data->enc_diff = -1; // 逆时针
    } else {
        data->enc_diff = 0;  // 无变化
    }
    last_encoder_dir = current_dir;
    
    // 检测按钮状态 (Pin 15)，增加消抖机制
    bool current_btn_state = !digitalRead(15); // 读取当前按钮状态
    
    // 检查按钮状态是否改变，并处理消抖
    if (current_btn_state != last_btn_state) {
        // 状态变化，记录时间
        last_btn_change_time = millis();
    }
    
    // 只有当状态稳定足够长时间才更新实际按钮状态
    if ((millis() - last_btn_change_time) > DEBOUNCE_DELAY) {
        encoder_btn_pressed = current_btn_state;
    }
    
    // 更新上次状态
    last_btn_state = current_btn_state;
    
    // 设置LVGL状态
    data->state = encoder_btn_pressed ? LV_INDEV_STATE_PRESSED : LV_INDEV_STATE_RELEASED;
}

// 初始化LVGL编码器输入设备
void encoder_lvgl_init()
{
    // 确保编码器已初始化
    encoder_init();
    
    // 注册LVGL输入设备
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    
    indev_drv.type = LV_INDEV_TYPE_ENCODER;
    indev_drv.read_cb = encoder_read;
    
    encoder_indev = lv_indev_drv_register(&indev_drv);
    
    // 初始化位置
    last_encoder_dir = (int)(encoder->getDirection());
}

// 获取编码器输入设备指针
lv_indev_t* get_encoder_indev()
{
    return encoder_indev;
}

int get_encoder_position() {
    if (encoder != NULL) {
        return encoder->getPosition();
    }
    return 0;
}