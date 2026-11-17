//
// Created by ZBright on 2026/11/13.
//

#ifndef OLED_UI_MENUDATA_H
#define OLED_UI_MENUDATA_H

// 检测是否是C++编译器
#ifdef __cplusplus
extern "C" {
#endif

#include "oled_ui.h"
#ifdef OLED_UI

//进行前置声明
extern MenuItem MainMenuItems[],SettingsMenuItems[],AboutThisDeviceMenuItems[],
AboutOLED_UIMenuItems[],MoreMenuItems[],Font8MenuItems[] ,Font12MenuItems[] ,
Font16MenuItems[] ,Font20MenuItems[],LongMenuItems[],SpringMenuItems[],LongListMenuItems[],SmallAreaMenuItems[];
extern MenuPage MainMenuPage,SettingsMenuPage,AboutThisDeviceMenuPage,
AboutOLED_UIMenuPage,MoreMenuPage,Font8MenuPage,Font12MenuPage,Font16MenuPage
,Font20MenuPage,LongMenuPage,SpringMenuPage,LongListMenuPage,SmallAreaMenuPage;

#endif

#ifdef __cplusplus
}  // extern "C"
#endif

#endif //OLED_UI_MENUDATA_H
