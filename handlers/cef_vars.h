#pragma once

#include "include/capi/cef_frame_capi.h"
#include "include/capi/views/cef_browser_view_capi.h"
#include "include/capi/views/cef_window_capi.h"

static char **funcMap;
static int funcLength;
static cef_window_t* MAIN_WINDOW = NULL;
static cef_frame_t* MAIN_FRAME = NULL;