#include <event_pid.h>

void ui_event_PIDSettingBack( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);

if ( event_code == LV_EVENT_RELEASED) {
      _ui_screen_change( &ui_SystemSettingScreen, LV_SCR_LOAD_ANIM_FADE_ON, 100, 0, &ui_SystemSettingScreen_screen_init);
        lvgl_group_to_setting();
        WritePID();
        Soldering_PID_Update_Tunings( Soldering_KP, Soldering_KI, Soldering_KD );
        HeatgunPID_Update_Tunings( Heatgun_KP, Heatgun_KI, Heatgun_KD );
}
}

void ui_event_SolderingKP( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);

if ( event_code == LV_EVENT_VALUE_CHANGED) {
      SolderingKpUpdate( e );
      Soldering_PID_Update();
}
}

void ui_event_SolderingKI( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);

if ( event_code == LV_EVENT_VALUE_CHANGED) {
      SolderingKiUpdate( e );
      Soldering_PID_Update();
}
}

void ui_event_SolderingKD( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);

if ( event_code == LV_EVENT_VALUE_CHANGED) {
      SolderingKdUpdate( e );
      Soldering_PID_Update();
}
}

void ui_event_SolderingKPDown( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);

if ( event_code == LV_EVENT_PRESSED) {
      SolderingKPDown( e );
        lv_spinbox_decrement(ui_SolderingKP);
}
}

void ui_event_SolderingKPUp( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);

if ( event_code == LV_EVENT_PRESSED) {
      SolderingKPUp( e );
        lv_spinbox_increment(ui_SolderingKP);
}
}

void ui_event_SolderingKIDown( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);

if ( event_code == LV_EVENT_PRESSED) {
      SolderingKIDown( e );
        lv_spinbox_decrement(ui_SolderingKI);
}
}

void ui_event_SolderingKIUp( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);

if ( event_code == LV_EVENT_PRESSED) {
      SolderingKIUp( e );
        lv_spinbox_increment(ui_SolderingKI);
}
}

void ui_event_SolderingKDDown( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);

if ( event_code == LV_EVENT_PRESSED) {
      SolderingKDDown( e );
        lv_spinbox_decrement(ui_SolderingKD);
}
}

void ui_event_SolderingKDUp( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);

if ( event_code == LV_EVENT_PRESSED) {
      SolderingKDUp( e );
        lv_spinbox_increment(ui_SolderingKD);
}
}

void ui_event_HeatgunKP( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);

if ( event_code == LV_EVENT_VALUE_CHANGED) {
      HeatgunKpUpdate( e );
      Heatgun_PID_Update();
}
}

void ui_event_HeatgunKI( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);

if ( event_code == LV_EVENT_VALUE_CHANGED) {
      HeatgunKiUpdate( e );
      Heatgun_PID_Update();
}
}

void ui_event_HeatgunKD( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);

if ( event_code == LV_EVENT_VALUE_CHANGED) {
      HeatgunKdUpdate( e );
      Heatgun_PID_Update();
}
}

void ui_event_HeatgunKPDown( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);

if ( event_code == LV_EVENT_PRESSED) {
      HeatgunKPDown( e );
        lv_spinbox_decrement(ui_HeatgunKP);
}
}

void ui_event_HeatgunKPUp( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);

if ( event_code == LV_EVENT_PRESSED) {
      HeatgunKPUp( e );
        lv_spinbox_increment(ui_HeatgunKP);
}
}

void ui_event_HeatgunKIDown( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);

if ( event_code == LV_EVENT_PRESSED) {
      HeatgunKIDown( e );
        lv_spinbox_decrement(ui_HeatgunKI);
}
}

void ui_event_HeatgunKIUp( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);

if ( event_code == LV_EVENT_PRESSED) {
      HeatgunKIUp( e );
        lv_spinbox_increment(ui_HeatgunKI);
}
}

void ui_event_HeatgunKDDown( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);

if ( event_code == LV_EVENT_PRESSED) {
      HeatgunKDDown( e );
        lv_spinbox_decrement(ui_HeatgunKD);
}
}

void ui_event_HeatgunKDUp( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);

if ( event_code == LV_EVENT_PRESSED) {
      HeatgunKDUp( e );
        lv_spinbox_increment(ui_HeatgunKD);
}
}

void Soldering_PID_Update()
{
    Soldering_KP = lv_spinbox_get_value(ui_SolderingKP) / 1000.0f;
    Soldering_KI = lv_spinbox_get_value(ui_SolderingKI) / 1000.0f;
    Soldering_KD = lv_spinbox_get_value(ui_SolderingKD) / 1000.0f;
}

void Soldering_PID_Init()
{
    uint32_t SolderingKp = (uint32_t)(Soldering_KP * 1000.0f + 0.5f);
    uint32_t SolderingKi = (uint32_t)(Soldering_KI * 1000.0f + 0.5f);
    uint32_t SolderingKd = (uint32_t)(Soldering_KD * 1000.0f + 0.5f);

    lv_spinbox_set_value(ui_SolderingKP, SolderingKp);
    lv_spinbox_set_value(ui_SolderingKI, SolderingKi);
    lv_spinbox_set_value(ui_SolderingKD, SolderingKd);
}

void Heatgun_PID_Update()
{
    Heatgun_KP = lv_spinbox_get_value(ui_HeatgunKP) / 1000.0f;
    Heatgun_KI = lv_spinbox_get_value(ui_HeatgunKI) / 1000.0f;
    Heatgun_KD = lv_spinbox_get_value(ui_HeatgunKD) / 1000.0f;
}

void Heatgun_PID_Init()
{
    uint32_t HeatgunKp = (uint32_t)(Heatgun_KP * 1000.0f + 0.5f);
    uint32_t HeatgunKi = (uint32_t)(Heatgun_KI * 1000.0f + 0.5f);
    uint32_t HeatgunKd = (uint32_t)(Heatgun_KD * 1000.0f + 0.5f);

    lv_spinbox_set_value(ui_HeatgunKP, HeatgunKp);
    lv_spinbox_set_value(ui_HeatgunKI, HeatgunKi);
    lv_spinbox_set_value(ui_HeatgunKD, HeatgunKd);
}