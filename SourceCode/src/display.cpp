#include <Display.h>

TFT_eSPI tft = TFT_eSPI(screenWidth, screenHeight); /* TFT instance */
FT6336U touch_6336;

// 使用 FreeRTOS 软件定时器为 LVGL 提供毫秒 Tick
TimerHandle_t s_lvglTickTimer = NULL;
static void vLvglTickCb(TimerHandle_t /*xTimer*/)
{
  lv_tick_inc(5);
}

/* Display flushing */
void my_disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p)
{
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, w, h);
  tft.pushColors((uint16_t *)&color_p->full, w * h, true);
  tft.endWrite();

  lv_disp_flush_ready(disp_drv);
}

/*Read the touchpad*/
void my_touchpad_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data)
{
  // 更新触摸数据
  touch_6336.scan();

  // 检查是否有触摸点
  if (touch_6336.touchPoint.touch_count > 0)
  {
    // 找到第一个活动的触摸点
    for (uint8_t i = 0; i < 2; i++)
    {
      if (touch_6336.touchPoint.tp[i].status != release)
      {
        data->state = LV_INDEV_STATE_PR;
        data->point.x = touch_6336.touchPoint.tp[i].x;
        data->point.y = touch_6336.touchPoint.tp[i].y;
        return;
      }
    }
  }

  // 没有触摸点活动
  data->state = LV_INDEV_STATE_REL;
}

void display_init()
{
  tft.init(); /* TFT init */
  tft.initDMA();
  tft.setRotation(3); /* Landscape orientation, flipped */
  if (Display_Type_Configured)
  {
    ChangeDisplayType(Display_Type);
  }
  lv_init();

  // 启动 LVGL Tick 软件定时器（5ms 周期）
  if (s_lvglTickTimer == NULL)
  {
    s_lvglTickTimer = xTimerCreate("lv_tick",
                                   pdMS_TO_TICKS(5),
                                   pdTRUE,
                                   nullptr,
                                   vLvglTickCb);
    if (s_lvglTickTimer)
    {
      xTimerStart(s_lvglTickTimer, 0);
    }
  }

  // Configure touch screen pins and dimensions
  touch_6336.setTouchConfig(screenWidth, screenHeight,
                            TOUCH_INT_PIN, TOUCH_SDA_PIN, TOUCH_SCL_PIN, TOUCH_RST_PIN);
  touch_6336.begin();

  lv_disp_draw_buf_init(&draw_buf, buf_1, buf_2, screenWidth * screenHeight / 5);

  /*Initialize the display*/
  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  /*Change the following line to your display resolution*/
  disp_drv.hor_res = screenWidth;
  disp_drv.ver_res = screenHeight;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);

  /*Initialize the (dummy) input device driver*/
  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = my_touchpad_read;
  lv_indev_drv_register(&indev_drv);

  ui_init();
  lv_label_set_text(ui_FWVersion, Version.c_str()); // 设置固件版本信息

  MainScreen_init();
  SystemSettingScreen_init();
  PIDSettingScreen_init();
  Soldering_PID_Init();
  Heatgun_PID_Init();
}

void lvgl_task_handler()
{
  lv_timer_handler();
  handle_encoder_parameters_edit();
}

void ChangeDisplayType(bool Type)
{
  if (Type)
  {
    tft.invertDisplay(true); // true:IPS屏幕
  }
  else
  {
    tft.invertDisplay(false); // false:TFT屏幕
  }
}

static void pid_setting_focus_cb(lv_event_t *e)
{
  lv_event_code_t code = lv_event_get_code(e);
  if (code == LV_EVENT_FOCUSED)
  {
    lv_obj_t *obj = lv_event_get_target(e);
    lv_obj_t *panel = lv_obj_get_parent(obj);
    // 强制滚动到面板顶部
    lv_obj_scroll_to_y(panel, 0, LV_ANIM_ON);
  }
}

void MainScreen_init()
{
  lv_label_set_text_fmt(ui_SolderingTargetTemp, "%.3d℃", SolderingTargetTemp);
  lv_label_set_text_fmt(ui_HeatgunTargetTemp, "%.3d℃", HeatgunTargetTemp);
  lv_label_set_text_fmt(ui_HeatgunWindSpeed, "%.3d%%", HeatgunWindSpeed);

  lv_label_set_text_fmt(ui_SolderingCurrentTemp, "%.3d℃", SolderingTargetTemp);
  lv_label_set_text_fmt(ui_HeatgunCurrentTemp, "%.3d℃", HeatgunTargetTemp);
  lv_label_set_text_fmt(ui_HeatgunCurrentWindSpeed, "%.3d%%", HeatgunWindSpeed);

  if (Buzzer_Enabled == false)
  {
    lv_obj_clear_state(ui_Buzzer, LV_STATE_CHECKED);
    lv_img_set_src(ui_BuzzerStatus, &ui_img_185202102);
  }
  else
  {
    lv_obj_add_state(ui_Buzzer, LV_STATE_CHECKED);
    lv_img_set_src(ui_BuzzerStatus, &ui_img_1699618864);
  }

  if (CoolingFan_Enabled == false)
  {
    lv_obj_clear_state(ui_CoolingFan, LV_STATE_CHECKED);
    lv_img_set_src(ui_CoolingStatus, &ui_img_1708415670);
  }
  else
  {
    lv_obj_add_state(ui_CoolingFan, LV_STATE_CHECKED);
    lv_img_set_src(ui_CoolingStatus, &ui_img_2103744591);
  }

  if (SolderingStandbyTime == 0)
  {
    lv_img_set_src(ui_SleepStatus, &ui_img_minus_png);
  }
  else
  {
    lv_img_set_src(ui_SleepStatus, &ui_img_295618317);
  }
}

void SystemSettingScreen_init()
{
  // 为PIDSetting添加特殊的事件处理
  lv_obj_add_event_cb(ui_PIDSetting, pid_setting_focus_cb, LV_EVENT_FOCUSED, NULL);
  lv_obj_set_style_radius(ui_SystemSetting, 2, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
  lv_obj_set_style_width(ui_SystemSetting, 25, LV_PART_SCROLLBAR | LV_STATE_DEFAULT); // 设置滚动条宽度

  // 为所有控件添加滚动跟随焦点的标志
  lv_obj_add_flag(ui_PIDSetting, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
  lv_obj_add_flag(ui_SolderingMaxTemp, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
  lv_obj_add_flag(ui_SolderingMinTemp, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
  lv_obj_add_flag(ui_SolderingStandbyTemp, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
  lv_obj_add_flag(ui_SolderingStandbyTime, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
  lv_obj_add_flag(ui_HeatgunMaxTemp, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
  lv_obj_add_flag(ui_HeatgunMinTemp, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
  lv_obj_add_flag(ui_SystemBrightness, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
  lv_obj_add_flag(ui_SettingSave, LV_OBJ_FLAG_SCROLL_ON_FOCUS);

  lv_label_set_text_fmt(ui_TextSolderingMinTemp, "%d℃", SolderingTargetTempMin);
  lv_slider_set_value(ui_SolderingMinTemp, SolderingTargetTempMin, LV_ANIM_OFF);

  lv_label_set_text_fmt(ui_TextSolderingMaxTemp, "%d℃", SolderingTargetTempMax);
  lv_slider_set_value(ui_SolderingMaxTemp, SolderingTargetTempMax, LV_ANIM_OFF);

  lv_label_set_text_fmt(ui_TextSolderingStandbyTemp, "%d℃", SolderingStandbyTemp);
  lv_slider_set_value(ui_SolderingStandbyTemp, SolderingStandbyTemp, LV_ANIM_OFF);

  lv_label_set_text_fmt(ui_TextSolderingStandbyTime, "%dMin", SolderingStandbyTime);
  lv_slider_set_value(ui_SolderingStandbyTime, SolderingStandbyTime, LV_ANIM_OFF);

  lv_slider_set_value(ui_SolderingPowerLimit, SolderingMaxPower, LV_ANIM_OFF);

  lv_label_set_text_fmt(ui_TextHeatgunMaxTemp, "%d℃", HeatgunTargetTempMax);
  lv_slider_set_value(ui_HeatgunMaxTemp, HeatgunTargetTempMax, LV_ANIM_OFF);

  lv_label_set_text_fmt(ui_TextHeatgunMinTemp, "%d℃", HeatgunTargetTempMin);
  lv_slider_set_value(ui_HeatgunMinTemp, HeatgunTargetTempMin, LV_ANIM_OFF);

  lv_label_set_text_fmt(ui_TextBrightness, "%d%%", Brightness);
  lv_slider_set_value(ui_SystemBrightness, Brightness, LV_ANIM_OFF);
}

void PIDSettingScreen_init()
{
  // 为PIDSetting添加特殊的事件处理
  lv_obj_add_event_cb(ui_SolderingKPDown, pid_setting_focus_cb, LV_EVENT_FOCUSED, NULL);
  lv_obj_set_style_radius(ui_PIDSettingDock, 2, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
  lv_obj_set_style_width(ui_PIDSettingDock, 25, LV_PART_SCROLLBAR | LV_STATE_DEFAULT); // 设置滚动条宽度

  // 为所有控件添加滚动跟随焦点的标志
  lv_obj_add_flag(ui_SolderingKP, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
  lv_obj_add_flag(ui_SolderingKI, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
  lv_obj_add_flag(ui_SolderingKD, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
  lv_obj_add_flag(ui_HeatgunKP, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
  lv_obj_add_flag(ui_HeatgunKI, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
  lv_obj_add_flag(ui_HeatgunKD, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
}