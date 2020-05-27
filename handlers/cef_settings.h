#pragma once

#include "include/capi/cef_base_capi.h"

typedef struct _gui_settings {
    char* url;
    char* title;
    int frameless;
    int maximized;
    int fullscreen;
    int can_maximize;
    int can_minimize;
    int can_resize;
    int height;
    int width;
    uint8_t BGA;
    uint8_t BGR;
    uint8_t BGG;
    uint8_t BGB;
    char* window_icon;
    char* window_app_icon;
} gui_settings;

