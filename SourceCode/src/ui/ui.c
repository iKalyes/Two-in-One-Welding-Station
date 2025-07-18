// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.5.1
// LVGL version: 8.3.11
// Project name: UI

#include "ui.h"
#include "ui_helpers.h"

///////////////////// VARIABLES ////////////////////
void Start_Animation( lv_obj_t *TargetObject, int delay);

// SCREEN: ui_PowerON
void ui_PowerON_screen_init(void);
lv_obj_t *ui_PowerON;
lv_obj_t *ui_ImagePowerON;
lv_obj_t *ui_InitializeStatusBar;
lv_obj_t *ui_InitializeStatus;
lv_obj_t *ui_FWVersion;
// CUSTOM VARIABLES

// SCREEN: ui_MainScreen
void ui_MainScreen_screen_init(void);
lv_obj_t *ui_MainScreen;
void ui_event_Setting( lv_event_t * e);
lv_obj_t *ui_Setting;
void ui_event_Buzzer( lv_event_t * e);
lv_obj_t *ui_Buzzer;
void ui_event_CoolingFan( lv_event_t * e);
lv_obj_t *ui_CoolingFan;
lv_obj_t *ui_TMP102Dock;
lv_obj_t *ui_TMP102Temp;
lv_obj_t *ui_ImageTMP102Temp;
lv_obj_t *ui_INA226Voltage;
lv_obj_t *ui_INA226Current;
lv_obj_t *ui_INA226Power;
lv_obj_t *ui_SystemStatusDock;
lv_obj_t *ui_BuzzerStatus;
lv_obj_t *ui_SleepStatus;
lv_obj_t *ui_CoolingStatus;
lv_obj_t *ui_Soldering;
lv_obj_t *ui_TextSoldering;
lv_obj_t *ui_ImageSoldering;
lv_obj_t *ui_SolderingDock;
lv_obj_t *ui_SolderingTempDock;
lv_obj_t *ui_BarSolderingDuty;
lv_obj_t *ui_SolderingTemp;
lv_obj_t *ui_TextSolderingTemp;
lv_obj_t *ui_SolderingSettingDock;
lv_obj_t *ui_TextSolderingTragetTemp;
lv_obj_t *ui_SolderingTargetTemp;
void ui_event_SolderingSet( lv_event_t * e);
lv_obj_t *ui_SolderingSet;
void ui_event_SolderingSwitch( lv_event_t * e);
lv_obj_t *ui_SolderingSwitch;
lv_obj_t *ui_Heatgun;
lv_obj_t *ui_TextHeatgun;
lv_obj_t *ui_ImageHeatgun;
lv_obj_t *ui_HeatgunDock;
lv_obj_t *ui_HeatgunTempDock;
lv_obj_t *ui_BarHeatgunDuty;
lv_obj_t *ui_HeatgunTemp;
lv_obj_t *ui_TextHeatgunTemp;
lv_obj_t *ui_HeatgunSettingDock;
lv_obj_t *ui_TextHeatgunTargetTemp;
lv_obj_t *ui_HeatgunTargetTemp;
lv_obj_t *ui_TextHeatgunWindSpeed;
lv_obj_t *ui_HeatgunWindSpeed;
void ui_event_HeatgunSet( lv_event_t * e);
lv_obj_t *ui_HeatgunSet;
void ui_event_HeatgunSwitch( lv_event_t * e);
lv_obj_t *ui_HeatgunSwitch;
// CUSTOM VARIABLES

// SCREEN: ui_SolderingTempSetScreen
void ui_SolderingTempSetScreen_screen_init(void);
lv_obj_t *ui_SolderingTempSetScreen;
lv_obj_t *ui_SolderingKeyboard;
void ui_event_SolderingConfirm( lv_event_t * e);
lv_obj_t *ui_SolderingConfirm;
void ui_event_SolderingDelete( lv_event_t * e);
lv_obj_t *ui_SolderingDelete;
void ui_event_SolderingNum0( lv_event_t * e);
lv_obj_t *ui_SolderingNum0;
lv_obj_t *ui_SolderingNumPanel;
void ui_event_SolderingNum1( lv_event_t * e);
lv_obj_t *ui_SolderingNum1;
void ui_event_SolderingNum2( lv_event_t * e);
lv_obj_t *ui_SolderingNum2;
void ui_event_SolderingNum3( lv_event_t * e);
lv_obj_t *ui_SolderingNum3;
void ui_event_SolderingNum4( lv_event_t * e);
lv_obj_t *ui_SolderingNum4;
void ui_event_SolderingNum5( lv_event_t * e);
lv_obj_t *ui_SolderingNum5;
void ui_event_SolderingNum6( lv_event_t * e);
lv_obj_t *ui_SolderingNum6;
void ui_event_SolderingNum7( lv_event_t * e);
lv_obj_t *ui_SolderingNum7;
void ui_event_SolderingNum8( lv_event_t * e);
lv_obj_t *ui_SolderingNum8;
void ui_event_SolderingNum9( lv_event_t * e);
lv_obj_t *ui_SolderingNum9;
lv_obj_t *ui_SolderingTempSetDock;
lv_obj_t *ui_TextSolderingTempSetting;
lv_obj_t *ui_SolderingTempSetting;
lv_obj_t *ui_SolderingSetTemp;
lv_obj_t *ui_TextSolderingCurrentTemp;
lv_obj_t *ui_SolderingCurrentTemp;
void ui_event_SolderingTempSetBack( lv_event_t * e);
lv_obj_t *ui_SolderingTempSetBack;
// CUSTOM VARIABLES

// SCREEN: ui_SystemSettingScreen
void ui_SystemSettingScreen_screen_init(void);
lv_obj_t *ui_SystemSettingScreen;
lv_obj_t *ui_SystemSettingDock;
lv_obj_t *ui_TextSystemSetting;
void ui_event_SystemSettingBack( lv_event_t * e);
lv_obj_t *ui_SystemSettingBack;
lv_obj_t *ui_SystemSetting;
void ui_event_PIDSetting( lv_event_t * e);
lv_obj_t *ui_PIDSetting;
lv_obj_t *ui_TextPIDSetting;
void ui_event_SolderingMaxTemp( lv_event_t * e);
lv_obj_t *ui_SolderingMaxTemp;
lv_obj_t *ui_TextSolderingMaxTemp;
lv_obj_t *ui_NameSolderingMaxTemp;
void ui_event_SolderingMinTemp( lv_event_t * e);
lv_obj_t *ui_SolderingMinTemp;
lv_obj_t *ui_TextSolderingMinTemp;
lv_obj_t *ui_NameSolderingMinTemp;
void ui_event_SolderingStandbyTemp( lv_event_t * e);
lv_obj_t *ui_SolderingStandbyTemp;
lv_obj_t *ui_TextSolderingStandbyTemp;
lv_obj_t *ui_NameSolderingStandbyTemp;
void ui_event_SolderingStandbyTime( lv_event_t * e);
lv_obj_t *ui_SolderingStandbyTime;
lv_obj_t *ui_TextSolderingStandbyTime;
lv_obj_t *ui_NameSolderingStandbyTime;
void ui_event_HeatgunMaxTemp( lv_event_t * e);
lv_obj_t *ui_HeatgunMaxTemp;
lv_obj_t *ui_TextHeatgunMaxTemp;
lv_obj_t *ui_NameHeatgunMaxTemp;
void ui_event_HeatgunMinTemp( lv_event_t * e);
lv_obj_t *ui_HeatgunMinTemp;
lv_obj_t *ui_TextHeatgunMinTemp;
lv_obj_t *ui_NameHeatgunMinTemp;
void ui_event_SystemBrightness( lv_event_t * e);
lv_obj_t *ui_SystemBrightness;
lv_obj_t *ui_ImageBrightness;
lv_obj_t *ui_TextBrightness;
void ui_event_SettingSave( lv_event_t * e);
lv_obj_t *ui_SettingSave;
// CUSTOM VARIABLES

// SCREEN: ui_HeatgunTempSetScreen
void ui_HeatgunTempSetScreen_screen_init(void);
lv_obj_t *ui_HeatgunTempSetScreen;
lv_obj_t *ui_HeatgunKeyboard;
void ui_event_HeatgunConfirm( lv_event_t * e);
lv_obj_t *ui_HeatgunConfirm;
void ui_event_HeatgunDelete( lv_event_t * e);
lv_obj_t *ui_HeatgunDelete;
void ui_event_HeatgunNum0( lv_event_t * e);
lv_obj_t *ui_HeatgunNum0;
lv_obj_t *ui_HeatgunNumPanel;
void ui_event_HeatgunNum1( lv_event_t * e);
lv_obj_t *ui_HeatgunNum1;
void ui_event_HeatgunNum2( lv_event_t * e);
lv_obj_t *ui_HeatgunNum2;
void ui_event_HeatgunNum3( lv_event_t * e);
lv_obj_t *ui_HeatgunNum3;
void ui_event_HeatgunNum4( lv_event_t * e);
lv_obj_t *ui_HeatgunNum4;
void ui_event_HeatgunNum5( lv_event_t * e);
lv_obj_t *ui_HeatgunNum5;
void ui_event_HeatgunNum6( lv_event_t * e);
lv_obj_t *ui_HeatgunNum6;
void ui_event_HeatgunNum7( lv_event_t * e);
lv_obj_t *ui_HeatgunNum7;
void ui_event_HeatgunNum8( lv_event_t * e);
lv_obj_t *ui_HeatgunNum8;
void ui_event_HeatgunNum9( lv_event_t * e);
lv_obj_t *ui_HeatgunNum9;
lv_obj_t *ui_HeatgunSetDock;
lv_obj_t *ui_TextHeatgunSetting;
lv_obj_t *ui_HeatgunSetting;
void ui_event_HeatgunSetTemp( lv_event_t * e);
lv_obj_t *ui_HeatgunSetTemp;
lv_obj_t *ui_TextHeatgunCurrentTemp;
lv_obj_t *ui_HeatgunCurrentTemp;
void ui_event_HeatgunSetWindSpeed( lv_event_t * e);
lv_obj_t *ui_HeatgunSetWindSpeed;
lv_obj_t *ui_TextHeatgunCurrentWindSpeed;
lv_obj_t *ui_HeatgunCurrentWindSpeed;
void ui_event_HeatgunSetBack( lv_event_t * e);
lv_obj_t *ui_HeatgunSetBack;
// CUSTOM VARIABLES

// SCREEN: ui_PIDSettingScreen
void ui_PIDSettingScreen_screen_init(void);
lv_obj_t *ui_PIDSettingScreen;
lv_obj_t *ui_PIDSettingHeader;
lv_obj_t *ui_TextPIDSettingHeader;
void ui_event_PIDSettingBack( lv_event_t * e);
lv_obj_t *ui_PIDSettingBack;
lv_obj_t *ui_PIDSettingDock;
lv_obj_t *ui_TextSolderingPID;
void ui_event_SolderingKP( lv_event_t * e);
lv_obj_t *ui_SolderingKP;
lv_obj_t *ui_TextPIDKP;
void ui_event_SolderingKI( lv_event_t * e);
lv_obj_t *ui_SolderingKI;
lv_obj_t *ui_TextPIDKI;
void ui_event_SolderingKD( lv_event_t * e);
lv_obj_t *ui_SolderingKD;
lv_obj_t *ui_TextPIDKD;
void ui_event_SolderingKPDown( lv_event_t * e);
lv_obj_t *ui_SolderingKPDown;
void ui_event_SolderingKPUp( lv_event_t * e);
lv_obj_t *ui_SolderingKPUp;
void ui_event_SolderingKIDown( lv_event_t * e);
lv_obj_t *ui_SolderingKIDown;
void ui_event_SolderingKIUp( lv_event_t * e);
lv_obj_t *ui_SolderingKIUp;
void ui_event_SolderingKDDown( lv_event_t * e);
lv_obj_t *ui_SolderingKDDown;
void ui_event_SolderingKDUp( lv_event_t * e);
lv_obj_t *ui_SolderingKDUp;
lv_obj_t *ui_TextHeatgunPID;
void ui_event_HeatgunKP( lv_event_t * e);
lv_obj_t *ui_HeatgunKP;
lv_obj_t *ui_TextPIDKP2;
void ui_event_HeatgunKI( lv_event_t * e);
lv_obj_t *ui_HeatgunKI;
lv_obj_t *ui_TextPIDKI2;
void ui_event_HeatgunKD( lv_event_t * e);
lv_obj_t *ui_HeatgunKD;
lv_obj_t *ui_TextPIDKD2;
void ui_event_HeatgunKPDown( lv_event_t * e);
lv_obj_t *ui_HeatgunKPDown;
void ui_event_HeatgunKPUp( lv_event_t * e);
lv_obj_t *ui_HeatgunKPUp;
void ui_event_HeatgunKIDown( lv_event_t * e);
lv_obj_t *ui_HeatgunKIDown;
void ui_event_HeatgunKIUp( lv_event_t * e);
lv_obj_t *ui_HeatgunKIUp;
void ui_event_HeatgunKDDown( lv_event_t * e);
lv_obj_t *ui_HeatgunKDDown;
void ui_event_HeatgunKDUp( lv_event_t * e);
lv_obj_t *ui_HeatgunKDUp;
// CUSTOM VARIABLES

// EVENTS
void ui_event____initial_actions0( lv_event_t * e);
lv_obj_t *ui____initial_actions0;

// IMAGES AND IMAGE SETS

///////////////////// TEST LVGL SETTINGS ////////////////////
#if LV_COLOR_DEPTH != 16
    #error "LV_COLOR_DEPTH should be 16bit to match SquareLine Studio's settings"
#endif
#if LV_COLOR_16_SWAP !=0
    #error "LV_COLOR_16_SWAP should be 0 to match SquareLine Studio's settings"
#endif

///////////////////// ANIMATIONS ////////////////////
void Start_Animation( lv_obj_t *TargetObject, int delay)
{
ui_anim_user_data_t *PropertyAnimation_0_user_data = lv_mem_alloc(sizeof(ui_anim_user_data_t));
PropertyAnimation_0_user_data->target = TargetObject;
PropertyAnimation_0_user_data->val = -1;
lv_anim_t PropertyAnimation_0;
lv_anim_init(&PropertyAnimation_0);
lv_anim_set_time(&PropertyAnimation_0, 1000);
lv_anim_set_user_data(&PropertyAnimation_0, PropertyAnimation_0_user_data);
lv_anim_set_custom_exec_cb(&PropertyAnimation_0, _ui_anim_callback_set_opacity );
lv_anim_set_values(&PropertyAnimation_0, 0, 255 );
lv_anim_set_path_cb( &PropertyAnimation_0, lv_anim_path_linear);
lv_anim_set_delay( &PropertyAnimation_0, delay + 0 );
lv_anim_set_deleted_cb( &PropertyAnimation_0, _ui_anim_callback_free_user_data );
lv_anim_set_playback_time(&PropertyAnimation_0, 0);
lv_anim_set_playback_delay(&PropertyAnimation_0, 0);
 lv_anim_set_repeat_count(&PropertyAnimation_0, 0);
lv_anim_set_repeat_delay(&PropertyAnimation_0, 0);
lv_anim_set_early_apply( &PropertyAnimation_0, false );
 lv_anim_set_get_value_cb(&PropertyAnimation_0, &_ui_anim_callback_get_opacity );
lv_anim_start(&PropertyAnimation_0);

}

///////////////////// FUNCTIONS ////////////////////

void ui_event____initial_actions0( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);

if ( event_code == LV_EVENT_SCREEN_LOAD_START) {
      Start_Animation(ui_PowerON, 0);
}
}

///////////////////// SCREENS ////////////////////

void ui_init( void )
{
lv_disp_t *dispp = lv_disp_get_default();
lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), false, LV_FONT_DEFAULT);
lv_disp_set_theme(dispp, theme);
ui_PowerON_screen_init();
ui_MainScreen_screen_init();
ui_SolderingTempSetScreen_screen_init();
ui_SystemSettingScreen_screen_init();
ui_HeatgunTempSetScreen_screen_init();
ui_PIDSettingScreen_screen_init();
ui____initial_actions0 = lv_obj_create(NULL);
lv_obj_add_event_cb(ui____initial_actions0, ui_event____initial_actions0, LV_EVENT_ALL, NULL);

lv_disp_load_scr(ui____initial_actions0);
lv_disp_load_scr( ui_PowerON);
}
