// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.5.1
// LVGL version: 8.3.11
// Project name: UI

#include "../ui.h"

void ui_SolderingTempSetScreen_screen_init(void)
{
ui_SolderingTempSetScreen = lv_obj_create(NULL);
lv_obj_clear_flag( ui_SolderingTempSetScreen, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_bg_color(ui_SolderingTempSetScreen, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_SolderingTempSetScreen, 255, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_SolderingKeyboard = lv_obj_create(ui_SolderingTempSetScreen);
lv_obj_remove_style_all(ui_SolderingKeyboard);
lv_obj_set_width( ui_SolderingKeyboard, 282);
lv_obj_set_height( ui_SolderingKeyboard, 137);
lv_obj_set_x( ui_SolderingKeyboard, 36 );
lv_obj_set_y( ui_SolderingKeyboard, 100 );
lv_obj_clear_flag( ui_SolderingKeyboard, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_radius(ui_SolderingKeyboard, 5, LV_PART_MAIN| LV_STATE_DEFAULT);
ui_object_set_themeable_style_property(ui_SolderingKeyboard, LV_PART_MAIN| LV_STATE_DEFAULT, LV_STYLE_BG_COLOR, _ui_theme_color_Container);
ui_object_set_themeable_style_property(ui_SolderingKeyboard, LV_PART_MAIN| LV_STATE_DEFAULT, LV_STYLE_BG_OPA, _ui_theme_alpha_Container);

ui_SolderingConfirm = lv_btn_create(ui_SolderingKeyboard);
lv_obj_set_width( ui_SolderingConfirm, 77);
lv_obj_set_height( ui_SolderingConfirm, 32);
lv_obj_set_x( ui_SolderingConfirm, -15 );
lv_obj_set_y( ui_SolderingConfirm, 12 );
lv_obj_set_align( ui_SolderingConfirm, LV_ALIGN_TOP_RIGHT );
lv_obj_add_flag( ui_SolderingConfirm, LV_OBJ_FLAG_SCROLL_ON_FOCUS );   /// Flags
lv_obj_clear_flag( ui_SolderingConfirm, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_radius(ui_SolderingConfirm, 5, LV_PART_MAIN| LV_STATE_DEFAULT);
ui_object_set_themeable_style_property(ui_SolderingConfirm, LV_PART_MAIN| LV_STATE_DEFAULT, LV_STYLE_BG_COLOR, _ui_theme_color_ButtonPressed);
ui_object_set_themeable_style_property(ui_SolderingConfirm, LV_PART_MAIN| LV_STATE_DEFAULT, LV_STYLE_BG_OPA, _ui_theme_alpha_ButtonPressed);
lv_obj_set_style_bg_img_src( ui_SolderingConfirm, &ui_img_938671949, LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_shadow_color(ui_SolderingConfirm, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_shadow_opa(ui_SolderingConfirm, 0, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_SolderingDelete = lv_btn_create(ui_SolderingKeyboard);
lv_obj_set_width( ui_SolderingDelete, 76);
lv_obj_set_height( ui_SolderingDelete, 32);
lv_obj_set_x( ui_SolderingDelete, -15 );
lv_obj_set_y( ui_SolderingDelete, 52 );
lv_obj_set_align( ui_SolderingDelete, LV_ALIGN_TOP_RIGHT );
lv_obj_add_flag( ui_SolderingDelete, LV_OBJ_FLAG_SCROLL_ON_FOCUS );   /// Flags
lv_obj_clear_flag( ui_SolderingDelete, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_radius(ui_SolderingDelete, 5, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(ui_SolderingDelete, lv_color_hex(0xFF0000), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_SolderingDelete, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_img_src( ui_SolderingDelete, &ui_img_13634893, LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_shadow_color(ui_SolderingDelete, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_shadow_opa(ui_SolderingDelete, 0, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_SolderingNum0 = lv_btn_create(ui_SolderingKeyboard);
lv_obj_set_width( ui_SolderingNum0, 76);
lv_obj_set_height( ui_SolderingNum0, 32);
lv_obj_set_x( ui_SolderingNum0, -15 );
lv_obj_set_y( ui_SolderingNum0, 92 );
lv_obj_set_align( ui_SolderingNum0, LV_ALIGN_TOP_RIGHT );
lv_obj_add_flag( ui_SolderingNum0, LV_OBJ_FLAG_SCROLL_ON_FOCUS );   /// Flags
lv_obj_clear_flag( ui_SolderingNum0, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_radius(ui_SolderingNum0, 5, LV_PART_MAIN| LV_STATE_DEFAULT);
ui_object_set_themeable_style_property(ui_SolderingNum0, LV_PART_MAIN| LV_STATE_DEFAULT, LV_STYLE_BG_COLOR, _ui_theme_color_ButtonAndBack);
ui_object_set_themeable_style_property(ui_SolderingNum0, LV_PART_MAIN| LV_STATE_DEFAULT, LV_STYLE_BG_OPA, _ui_theme_alpha_ButtonAndBack);
lv_obj_set_style_bg_img_src( ui_SolderingNum0, &ui_img_num0_png, LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_shadow_color(ui_SolderingNum0, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_shadow_opa(ui_SolderingNum0, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_img_src( ui_SolderingNum0, &ui_img_num0_png, LV_PART_MAIN | LV_STATE_PRESSED );
ui_object_set_themeable_style_property(ui_SolderingNum0, LV_PART_MAIN| LV_STATE_PRESSED, LV_STYLE_BG_IMG_RECOLOR, _ui_theme_color_ButtonPressed);
ui_object_set_themeable_style_property(ui_SolderingNum0, LV_PART_MAIN| LV_STATE_PRESSED, LV_STYLE_BG_IMG_RECOLOR_OPA, _ui_theme_alpha_ButtonPressed);

ui_SolderingNumPanel = lv_obj_create(ui_SolderingKeyboard);
lv_obj_set_width( ui_SolderingNumPanel, 190);
lv_obj_set_height( ui_SolderingNumPanel, 136);
lv_obj_set_flex_flow(ui_SolderingNumPanel,LV_FLEX_FLOW_ROW_WRAP);
lv_obj_set_flex_align(ui_SolderingNumPanel, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
lv_obj_clear_flag( ui_SolderingNumPanel, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_bg_color(ui_SolderingNumPanel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_SolderingNumPanel, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_color(ui_SolderingNumPanel, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_border_opa(ui_SolderingNumPanel, 0, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_SolderingNum1 = lv_btn_create(ui_SolderingNumPanel);
lv_obj_set_width( ui_SolderingNum1, 48);
lv_obj_set_height( ui_SolderingNum1, 32);
lv_obj_set_x( ui_SolderingNum1, -20 );
lv_obj_set_y( ui_SolderingNum1, -6 );
lv_obj_set_align( ui_SolderingNum1, LV_ALIGN_TOP_RIGHT );
lv_obj_set_flex_flow(ui_SolderingNum1,LV_FLEX_FLOW_ROW);
lv_obj_set_flex_align(ui_SolderingNum1, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
lv_obj_add_flag( ui_SolderingNum1, LV_OBJ_FLAG_SCROLL_ON_FOCUS );   /// Flags
lv_obj_clear_flag( ui_SolderingNum1, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_radius(ui_SolderingNum1, 5, LV_PART_MAIN| LV_STATE_DEFAULT);
ui_object_set_themeable_style_property(ui_SolderingNum1, LV_PART_MAIN| LV_STATE_DEFAULT, LV_STYLE_BG_COLOR, _ui_theme_color_ButtonAndBack);
ui_object_set_themeable_style_property(ui_SolderingNum1, LV_PART_MAIN| LV_STATE_DEFAULT, LV_STYLE_BG_OPA, _ui_theme_alpha_ButtonAndBack);
lv_obj_set_style_bg_img_src( ui_SolderingNum1, &ui_img_num1_png, LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_shadow_color(ui_SolderingNum1, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_shadow_opa(ui_SolderingNum1, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_img_src( ui_SolderingNum1, &ui_img_num1_png, LV_PART_MAIN | LV_STATE_PRESSED );
ui_object_set_themeable_style_property(ui_SolderingNum1, LV_PART_MAIN| LV_STATE_PRESSED, LV_STYLE_BG_IMG_RECOLOR, _ui_theme_color_ButtonPressed);
ui_object_set_themeable_style_property(ui_SolderingNum1, LV_PART_MAIN| LV_STATE_PRESSED, LV_STYLE_BG_IMG_RECOLOR_OPA, _ui_theme_alpha_ButtonPressed);

ui_SolderingNum2 = lv_btn_create(ui_SolderingNumPanel);
lv_obj_set_width( ui_SolderingNum2, 48);
lv_obj_set_height( ui_SolderingNum2, 32);
lv_obj_set_x( ui_SolderingNum2, -216 );
lv_obj_set_y( ui_SolderingNum2, 7 );
lv_obj_set_align( ui_SolderingNum2, LV_ALIGN_TOP_RIGHT );
lv_obj_set_flex_flow(ui_SolderingNum2,LV_FLEX_FLOW_ROW);
lv_obj_set_flex_align(ui_SolderingNum2, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
lv_obj_add_flag( ui_SolderingNum2, LV_OBJ_FLAG_SCROLL_ON_FOCUS );   /// Flags
lv_obj_clear_flag( ui_SolderingNum2, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_radius(ui_SolderingNum2, 5, LV_PART_MAIN| LV_STATE_DEFAULT);
ui_object_set_themeable_style_property(ui_SolderingNum2, LV_PART_MAIN| LV_STATE_DEFAULT, LV_STYLE_BG_COLOR, _ui_theme_color_ButtonAndBack);
ui_object_set_themeable_style_property(ui_SolderingNum2, LV_PART_MAIN| LV_STATE_DEFAULT, LV_STYLE_BG_OPA, _ui_theme_alpha_ButtonAndBack);
lv_obj_set_style_bg_img_src( ui_SolderingNum2, &ui_img_num2_png, LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_shadow_color(ui_SolderingNum2, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_shadow_opa(ui_SolderingNum2, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_img_src( ui_SolderingNum2, &ui_img_num2_png, LV_PART_MAIN | LV_STATE_PRESSED );
ui_object_set_themeable_style_property(ui_SolderingNum2, LV_PART_MAIN| LV_STATE_PRESSED, LV_STYLE_BG_IMG_RECOLOR, _ui_theme_color_ButtonPressed);
ui_object_set_themeable_style_property(ui_SolderingNum2, LV_PART_MAIN| LV_STATE_PRESSED, LV_STYLE_BG_IMG_RECOLOR_OPA, _ui_theme_alpha_ButtonPressed);

ui_SolderingNum3 = lv_btn_create(ui_SolderingNumPanel);
lv_obj_set_width( ui_SolderingNum3, 48);
lv_obj_set_height( ui_SolderingNum3, 32);
lv_obj_set_x( ui_SolderingNum3, -151 );
lv_obj_set_y( ui_SolderingNum3, 2 );
lv_obj_set_align( ui_SolderingNum3, LV_ALIGN_TOP_RIGHT );
lv_obj_set_flex_flow(ui_SolderingNum3,LV_FLEX_FLOW_ROW);
lv_obj_set_flex_align(ui_SolderingNum3, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
lv_obj_add_flag( ui_SolderingNum3, LV_OBJ_FLAG_SCROLL_ON_FOCUS );   /// Flags
lv_obj_clear_flag( ui_SolderingNum3, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_radius(ui_SolderingNum3, 5, LV_PART_MAIN| LV_STATE_DEFAULT);
ui_object_set_themeable_style_property(ui_SolderingNum3, LV_PART_MAIN| LV_STATE_DEFAULT, LV_STYLE_BG_COLOR, _ui_theme_color_ButtonAndBack);
ui_object_set_themeable_style_property(ui_SolderingNum3, LV_PART_MAIN| LV_STATE_DEFAULT, LV_STYLE_BG_OPA, _ui_theme_alpha_ButtonAndBack);
lv_obj_set_style_bg_img_src( ui_SolderingNum3, &ui_img_num3_png, LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_shadow_color(ui_SolderingNum3, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_shadow_opa(ui_SolderingNum3, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_img_src( ui_SolderingNum3, &ui_img_num3_png, LV_PART_MAIN | LV_STATE_PRESSED );
ui_object_set_themeable_style_property(ui_SolderingNum3, LV_PART_MAIN| LV_STATE_PRESSED, LV_STYLE_BG_IMG_RECOLOR, _ui_theme_color_ButtonPressed);
ui_object_set_themeable_style_property(ui_SolderingNum3, LV_PART_MAIN| LV_STATE_PRESSED, LV_STYLE_BG_IMG_RECOLOR_OPA, _ui_theme_alpha_ButtonPressed);

ui_SolderingNum4 = lv_btn_create(ui_SolderingNumPanel);
lv_obj_set_width( ui_SolderingNum4, 48);
lv_obj_set_height( ui_SolderingNum4, 32);
lv_obj_set_x( ui_SolderingNum4, -89 );
lv_obj_set_y( ui_SolderingNum4, 7 );
lv_obj_set_align( ui_SolderingNum4, LV_ALIGN_TOP_RIGHT );
lv_obj_set_flex_flow(ui_SolderingNum4,LV_FLEX_FLOW_ROW);
lv_obj_set_flex_align(ui_SolderingNum4, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
lv_obj_add_flag( ui_SolderingNum4, LV_OBJ_FLAG_SCROLL_ON_FOCUS );   /// Flags
lv_obj_clear_flag( ui_SolderingNum4, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_radius(ui_SolderingNum4, 5, LV_PART_MAIN| LV_STATE_DEFAULT);
ui_object_set_themeable_style_property(ui_SolderingNum4, LV_PART_MAIN| LV_STATE_DEFAULT, LV_STYLE_BG_COLOR, _ui_theme_color_ButtonAndBack);
ui_object_set_themeable_style_property(ui_SolderingNum4, LV_PART_MAIN| LV_STATE_DEFAULT, LV_STYLE_BG_OPA, _ui_theme_alpha_ButtonAndBack);
lv_obj_set_style_bg_img_src( ui_SolderingNum4, &ui_img_num4_png, LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_shadow_color(ui_SolderingNum4, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_shadow_opa(ui_SolderingNum4, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_img_src( ui_SolderingNum4, &ui_img_num4_png, LV_PART_MAIN | LV_STATE_PRESSED );
ui_object_set_themeable_style_property(ui_SolderingNum4, LV_PART_MAIN| LV_STATE_PRESSED, LV_STYLE_BG_IMG_RECOLOR, _ui_theme_color_ButtonPressed);
ui_object_set_themeable_style_property(ui_SolderingNum4, LV_PART_MAIN| LV_STATE_PRESSED, LV_STYLE_BG_IMG_RECOLOR_OPA, _ui_theme_alpha_ButtonPressed);

ui_SolderingNum5 = lv_btn_create(ui_SolderingNumPanel);
lv_obj_set_width( ui_SolderingNum5, 48);
lv_obj_set_height( ui_SolderingNum5, 32);
lv_obj_set_x( ui_SolderingNum5, -212 );
lv_obj_set_y( ui_SolderingNum5, 53 );
lv_obj_set_align( ui_SolderingNum5, LV_ALIGN_TOP_RIGHT );
lv_obj_add_flag( ui_SolderingNum5, LV_OBJ_FLAG_SCROLL_ON_FOCUS );   /// Flags
lv_obj_clear_flag( ui_SolderingNum5, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_radius(ui_SolderingNum5, 5, LV_PART_MAIN| LV_STATE_DEFAULT);
ui_object_set_themeable_style_property(ui_SolderingNum5, LV_PART_MAIN| LV_STATE_DEFAULT, LV_STYLE_BG_COLOR, _ui_theme_color_ButtonAndBack);
ui_object_set_themeable_style_property(ui_SolderingNum5, LV_PART_MAIN| LV_STATE_DEFAULT, LV_STYLE_BG_OPA, _ui_theme_alpha_ButtonAndBack);
lv_obj_set_style_bg_img_src( ui_SolderingNum5, &ui_img_num5_png, LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_shadow_color(ui_SolderingNum5, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_shadow_opa(ui_SolderingNum5, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_img_src( ui_SolderingNum5, &ui_img_num5_png, LV_PART_MAIN | LV_STATE_PRESSED );
ui_object_set_themeable_style_property(ui_SolderingNum5, LV_PART_MAIN| LV_STATE_PRESSED, LV_STYLE_BG_IMG_RECOLOR, _ui_theme_color_ButtonPressed);
ui_object_set_themeable_style_property(ui_SolderingNum5, LV_PART_MAIN| LV_STATE_PRESSED, LV_STYLE_BG_IMG_RECOLOR_OPA, _ui_theme_alpha_ButtonPressed);

ui_SolderingNum6 = lv_btn_create(ui_SolderingNumPanel);
lv_obj_set_width( ui_SolderingNum6, 48);
lv_obj_set_height( ui_SolderingNum6, 32);
lv_obj_set_x( ui_SolderingNum6, -153 );
lv_obj_set_y( ui_SolderingNum6, 52 );
lv_obj_set_align( ui_SolderingNum6, LV_ALIGN_TOP_RIGHT );
lv_obj_add_flag( ui_SolderingNum6, LV_OBJ_FLAG_SCROLL_ON_FOCUS );   /// Flags
lv_obj_clear_flag( ui_SolderingNum6, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_radius(ui_SolderingNum6, 5, LV_PART_MAIN| LV_STATE_DEFAULT);
ui_object_set_themeable_style_property(ui_SolderingNum6, LV_PART_MAIN| LV_STATE_DEFAULT, LV_STYLE_BG_COLOR, _ui_theme_color_ButtonAndBack);
ui_object_set_themeable_style_property(ui_SolderingNum6, LV_PART_MAIN| LV_STATE_DEFAULT, LV_STYLE_BG_OPA, _ui_theme_alpha_ButtonAndBack);
lv_obj_set_style_bg_img_src( ui_SolderingNum6, &ui_img_num6_png, LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_shadow_color(ui_SolderingNum6, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_shadow_opa(ui_SolderingNum6, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_img_src( ui_SolderingNum6, &ui_img_num6_png, LV_PART_MAIN | LV_STATE_PRESSED );
ui_object_set_themeable_style_property(ui_SolderingNum6, LV_PART_MAIN| LV_STATE_PRESSED, LV_STYLE_BG_IMG_RECOLOR, _ui_theme_color_ButtonPressed);
ui_object_set_themeable_style_property(ui_SolderingNum6, LV_PART_MAIN| LV_STATE_PRESSED, LV_STYLE_BG_IMG_RECOLOR_OPA, _ui_theme_alpha_ButtonPressed);

ui_SolderingNum7 = lv_btn_create(ui_SolderingNumPanel);
lv_obj_set_width( ui_SolderingNum7, 48);
lv_obj_set_height( ui_SolderingNum7, 32);
lv_obj_set_x( ui_SolderingNum7, -90 );
lv_obj_set_y( ui_SolderingNum7, 54 );
lv_obj_set_align( ui_SolderingNum7, LV_ALIGN_TOP_RIGHT );
lv_obj_add_flag( ui_SolderingNum7, LV_OBJ_FLAG_SCROLL_ON_FOCUS );   /// Flags
lv_obj_clear_flag( ui_SolderingNum7, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_radius(ui_SolderingNum7, 5, LV_PART_MAIN| LV_STATE_DEFAULT);
ui_object_set_themeable_style_property(ui_SolderingNum7, LV_PART_MAIN| LV_STATE_DEFAULT, LV_STYLE_BG_COLOR, _ui_theme_color_ButtonAndBack);
ui_object_set_themeable_style_property(ui_SolderingNum7, LV_PART_MAIN| LV_STATE_DEFAULT, LV_STYLE_BG_OPA, _ui_theme_alpha_ButtonAndBack);
lv_obj_set_style_bg_img_src( ui_SolderingNum7, &ui_img_num7_png, LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_shadow_color(ui_SolderingNum7, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_shadow_opa(ui_SolderingNum7, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_img_src( ui_SolderingNum7, &ui_img_num7_png, LV_PART_MAIN | LV_STATE_PRESSED );
ui_object_set_themeable_style_property(ui_SolderingNum7, LV_PART_MAIN| LV_STATE_PRESSED, LV_STYLE_BG_IMG_RECOLOR, _ui_theme_color_ButtonPressed);
ui_object_set_themeable_style_property(ui_SolderingNum7, LV_PART_MAIN| LV_STATE_PRESSED, LV_STYLE_BG_IMG_RECOLOR_OPA, _ui_theme_alpha_ButtonPressed);

ui_SolderingNum8 = lv_btn_create(ui_SolderingNumPanel);
lv_obj_set_width( ui_SolderingNum8, 48);
lv_obj_set_height( ui_SolderingNum8, 32);
lv_obj_set_x( ui_SolderingNum8, -212 );
lv_obj_set_y( ui_SolderingNum8, 97 );
lv_obj_set_align( ui_SolderingNum8, LV_ALIGN_TOP_RIGHT );
lv_obj_add_flag( ui_SolderingNum8, LV_OBJ_FLAG_SCROLL_ON_FOCUS );   /// Flags
lv_obj_clear_flag( ui_SolderingNum8, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_radius(ui_SolderingNum8, 5, LV_PART_MAIN| LV_STATE_DEFAULT);
ui_object_set_themeable_style_property(ui_SolderingNum8, LV_PART_MAIN| LV_STATE_DEFAULT, LV_STYLE_BG_COLOR, _ui_theme_color_ButtonAndBack);
ui_object_set_themeable_style_property(ui_SolderingNum8, LV_PART_MAIN| LV_STATE_DEFAULT, LV_STYLE_BG_OPA, _ui_theme_alpha_ButtonAndBack);
lv_obj_set_style_bg_img_src( ui_SolderingNum8, &ui_img_num8_png, LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_shadow_color(ui_SolderingNum8, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_shadow_opa(ui_SolderingNum8, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_img_src( ui_SolderingNum8, &ui_img_num8_png, LV_PART_MAIN | LV_STATE_PRESSED );
ui_object_set_themeable_style_property(ui_SolderingNum8, LV_PART_MAIN| LV_STATE_PRESSED, LV_STYLE_BG_IMG_RECOLOR, _ui_theme_color_ButtonPressed);
ui_object_set_themeable_style_property(ui_SolderingNum8, LV_PART_MAIN| LV_STATE_PRESSED, LV_STYLE_BG_IMG_RECOLOR_OPA, _ui_theme_alpha_ButtonPressed);

ui_SolderingNum9 = lv_btn_create(ui_SolderingNumPanel);
lv_obj_set_width( ui_SolderingNum9, 48);
lv_obj_set_height( ui_SolderingNum9, 32);
lv_obj_set_x( ui_SolderingNum9, -154 );
lv_obj_set_y( ui_SolderingNum9, 99 );
lv_obj_set_align( ui_SolderingNum9, LV_ALIGN_TOP_RIGHT );
lv_obj_add_flag( ui_SolderingNum9, LV_OBJ_FLAG_SCROLL_ON_FOCUS );   /// Flags
lv_obj_clear_flag( ui_SolderingNum9, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_radius(ui_SolderingNum9, 5, LV_PART_MAIN| LV_STATE_DEFAULT);
ui_object_set_themeable_style_property(ui_SolderingNum9, LV_PART_MAIN| LV_STATE_DEFAULT, LV_STYLE_BG_COLOR, _ui_theme_color_ButtonAndBack);
ui_object_set_themeable_style_property(ui_SolderingNum9, LV_PART_MAIN| LV_STATE_DEFAULT, LV_STYLE_BG_OPA, _ui_theme_alpha_ButtonAndBack);
lv_obj_set_style_bg_img_src( ui_SolderingNum9, &ui_img_num9_png, LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_shadow_color(ui_SolderingNum9, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_shadow_opa(ui_SolderingNum9, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_img_src( ui_SolderingNum9, &ui_img_num9_png, LV_PART_MAIN | LV_STATE_PRESSED );
ui_object_set_themeable_style_property(ui_SolderingNum9, LV_PART_MAIN| LV_STATE_PRESSED, LV_STYLE_BG_IMG_RECOLOR, _ui_theme_color_ButtonPressed);
ui_object_set_themeable_style_property(ui_SolderingNum9, LV_PART_MAIN| LV_STATE_PRESSED, LV_STYLE_BG_IMG_RECOLOR_OPA, _ui_theme_alpha_ButtonPressed);

ui_SolderingTempSetDock = lv_obj_create(ui_SolderingTempSetScreen);
lv_obj_remove_style_all(ui_SolderingTempSetDock);
lv_obj_set_width( ui_SolderingTempSetDock, 316);
lv_obj_set_height( ui_SolderingTempSetDock, 20);
lv_obj_set_x( ui_SolderingTempSetDock, 0 );
lv_obj_set_y( ui_SolderingTempSetDock, 2 );
lv_obj_set_align( ui_SolderingTempSetDock, LV_ALIGN_TOP_MID );
lv_obj_clear_flag( ui_SolderingTempSetDock, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_radius(ui_SolderingTempSetDock, 5, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(ui_SolderingTempSetDock, lv_color_hex(0x393939), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_SolderingTempSetDock, 255, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_TextSolderingTempSetting = lv_label_create(ui_SolderingTempSetDock);
lv_obj_set_width( ui_TextSolderingTempSetting, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_TextSolderingTempSetting, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_TextSolderingTempSetting, 0 );
lv_obj_set_y( ui_TextSolderingTempSetting, -1 );
lv_obj_set_align( ui_TextSolderingTempSetting, LV_ALIGN_CENTER );
lv_label_set_text(ui_TextSolderingTempSetting,"电烙铁温度设置");
ui_object_set_themeable_style_property(ui_TextSolderingTempSetting, LV_PART_MAIN| LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR, _ui_theme_color_TextAndImage);
ui_object_set_themeable_style_property(ui_TextSolderingTempSetting, LV_PART_MAIN| LV_STATE_DEFAULT, LV_STYLE_TEXT_OPA, _ui_theme_alpha_TextAndImage);
lv_obj_set_style_text_font(ui_TextSolderingTempSetting, &ui_font_ASCII, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_SolderingTempSetting = lv_obj_create(ui_SolderingTempSetScreen);
lv_obj_remove_style_all(ui_SolderingTempSetting);
lv_obj_set_width( ui_SolderingTempSetting, 282);
lv_obj_set_height( ui_SolderingTempSetting, 72);
lv_obj_set_x( ui_SolderingTempSetting, 36 );
lv_obj_set_y( ui_SolderingTempSetting, 25 );
lv_obj_clear_flag( ui_SolderingTempSetting, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_radius(ui_SolderingTempSetting, 5, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(ui_SolderingTempSetting, lv_color_hex(0x393939), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_SolderingTempSetting, 255, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_SolderingSetTemp = lv_label_create(ui_SolderingTempSetting);
lv_obj_set_width( ui_SolderingSetTemp, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_SolderingSetTemp, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_SolderingSetTemp, 6 );
lv_obj_set_y( ui_SolderingSetTemp, -8 );
lv_obj_set_align( ui_SolderingSetTemp, LV_ALIGN_LEFT_MID );
lv_label_set_text(ui_SolderingSetTemp,"---℃");
ui_object_set_themeable_style_property(ui_SolderingSetTemp, LV_PART_MAIN| LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR, _ui_theme_color_TextAndImage);
ui_object_set_themeable_style_property(ui_SolderingSetTemp, LV_PART_MAIN| LV_STATE_DEFAULT, LV_STYLE_TEXT_OPA, _ui_theme_alpha_TextAndImage);
lv_obj_set_style_text_font(ui_SolderingSetTemp, &ui_font_ASCII48, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_TextSolderingCurrentTemp = lv_label_create(ui_SolderingTempSetting);
lv_obj_set_width( ui_TextSolderingCurrentTemp, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_TextSolderingCurrentTemp, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_TextSolderingCurrentTemp, 8 );
lv_obj_set_y( ui_TextSolderingCurrentTemp, 22 );
lv_obj_set_align( ui_TextSolderingCurrentTemp, LV_ALIGN_LEFT_MID );
lv_label_set_text(ui_TextSolderingCurrentTemp,"设定温度:");
ui_object_set_themeable_style_property(ui_TextSolderingCurrentTemp, LV_PART_MAIN| LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR, _ui_theme_color_TextAndImage);
ui_object_set_themeable_style_property(ui_TextSolderingCurrentTemp, LV_PART_MAIN| LV_STATE_DEFAULT, LV_STYLE_TEXT_OPA, _ui_theme_alpha_TextAndImage);
lv_obj_set_style_text_font(ui_TextSolderingCurrentTemp, &ui_font_ASCII, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_SolderingCurrentTemp = lv_label_create(ui_SolderingTempSetting);
lv_obj_set_width( ui_SolderingCurrentTemp, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_SolderingCurrentTemp, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_SolderingCurrentTemp, 80 );
lv_obj_set_y( ui_SolderingCurrentTemp, 22 );
lv_obj_set_align( ui_SolderingCurrentTemp, LV_ALIGN_LEFT_MID );
lv_label_set_text(ui_SolderingCurrentTemp,"000℃");
ui_object_set_themeable_style_property(ui_SolderingCurrentTemp, LV_PART_MAIN| LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR, _ui_theme_color_TextAndImage);
ui_object_set_themeable_style_property(ui_SolderingCurrentTemp, LV_PART_MAIN| LV_STATE_DEFAULT, LV_STYLE_TEXT_OPA, _ui_theme_alpha_TextAndImage);
lv_obj_set_style_text_font(ui_SolderingCurrentTemp, &ui_font_ASCII, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_SolderingTempSetBack = lv_btn_create(ui_SolderingTempSetScreen);
lv_obj_set_width( ui_SolderingTempSetBack, 32);
lv_obj_set_height( ui_SolderingTempSetBack, 212);
lv_obj_set_x( ui_SolderingTempSetBack, -286 );
lv_obj_set_y( ui_SolderingTempSetBack, 25 );
lv_obj_set_align( ui_SolderingTempSetBack, LV_ALIGN_TOP_RIGHT );
lv_obj_add_flag( ui_SolderingTempSetBack, LV_OBJ_FLAG_SCROLL_ON_FOCUS );   /// Flags
lv_obj_clear_flag( ui_SolderingTempSetBack, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_radius(ui_SolderingTempSetBack, 5, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(ui_SolderingTempSetBack, lv_color_hex(0x393939), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_SolderingTempSetBack, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_img_src( ui_SolderingTempSetBack, &ui_img_852573520, LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_shadow_color(ui_SolderingTempSetBack, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_shadow_opa(ui_SolderingTempSetBack, 0, LV_PART_MAIN| LV_STATE_DEFAULT);

lv_obj_add_event_cb(ui_SolderingConfirm, ui_event_SolderingConfirm, LV_EVENT_ALL, NULL);
lv_obj_add_event_cb(ui_SolderingDelete, ui_event_SolderingDelete, LV_EVENT_ALL, NULL);
lv_obj_add_event_cb(ui_SolderingNum0, ui_event_SolderingNum0, LV_EVENT_ALL, NULL);
lv_obj_add_event_cb(ui_SolderingNum1, ui_event_SolderingNum1, LV_EVENT_ALL, NULL);
lv_obj_add_event_cb(ui_SolderingNum2, ui_event_SolderingNum2, LV_EVENT_ALL, NULL);
lv_obj_add_event_cb(ui_SolderingNum3, ui_event_SolderingNum3, LV_EVENT_ALL, NULL);
lv_obj_add_event_cb(ui_SolderingNum4, ui_event_SolderingNum4, LV_EVENT_ALL, NULL);
lv_obj_add_event_cb(ui_SolderingNum5, ui_event_SolderingNum5, LV_EVENT_ALL, NULL);
lv_obj_add_event_cb(ui_SolderingNum6, ui_event_SolderingNum6, LV_EVENT_ALL, NULL);
lv_obj_add_event_cb(ui_SolderingNum7, ui_event_SolderingNum7, LV_EVENT_ALL, NULL);
lv_obj_add_event_cb(ui_SolderingNum8, ui_event_SolderingNum8, LV_EVENT_ALL, NULL);
lv_obj_add_event_cb(ui_SolderingNum9, ui_event_SolderingNum9, LV_EVENT_ALL, NULL);
lv_obj_add_event_cb(ui_SolderingTempSetBack, ui_event_SolderingTempSetBack, LV_EVENT_ALL, NULL);

}
