#pragma once

#include "handlers/cef_vars.h"
#include "include/capi/views/cef_window_delegate_capi.h"
#include "include/capi/cef_process_message_capi.h"
#include "include/capi/cef_frame_capi.h"

///
// Show the Window.
///
void CEF_CALLBACK window_helper_show () {
    if (cef_currently_on(TID_UI)) MAIN_WINDOW->show(MAIN_WINDOW);
    else if (cef_currently_on(TID_RENDERER)) {
        char* messagename = "show";
        cef_string_t cef_messagename = {};
        cef_string_utf8_to_utf16(messagename, strlen(messagename), &cef_messagename);
        cef_process_message_t* pm = cef_process_message_create(&cef_messagename);
        if(MAIN_FRAME != NULL) MAIN_FRAME->send_process_message(MAIN_FRAME, PID_BROWSER, pm);
    }
}

///
// Hide the Window.
///
void CEF_CALLBACK window_helper_hide() {
    if (cef_currently_on(TID_UI)) MAIN_WINDOW->hide(MAIN_WINDOW);
    else if (cef_currently_on(TID_RENDERER)) {
        char* messagename = "hide";
        cef_string_t cef_messagename = {};
        cef_string_utf8_to_utf16(messagename, strlen(messagename), &cef_messagename);
        cef_process_message_t* pm = cef_process_message_create(&cef_messagename);
        if(MAIN_FRAME != NULL) MAIN_FRAME->send_process_message(MAIN_FRAME, PID_BROWSER, pm);
    }
}

///
// Sizes the Window to |size| and centers it in the current display.
///
void CEF_CALLBACK window_helper_center_window(int width, int height) {
    cef_size_t size = {};
    size.width = width;
    size.height = height;
    if (cef_currently_on(TID_UI)) MAIN_WINDOW->center_window(MAIN_WINDOW, &size);
    else if (cef_currently_on(TID_RENDERER)) {
        char* messagename = "center_window";
        cef_string_t cef_messagename = {};
        cef_string_utf8_to_utf16(messagename, strlen(messagename), &cef_messagename);
        cef_process_message_t* pm = cef_process_message_create(&cef_messagename);

        cef_list_value_t *args = pm->get_argument_list(pm);
        args->set_int(args, 0, width);
        args->set_int(args, 1, height);

        if(MAIN_FRAME != NULL) MAIN_FRAME->send_process_message(MAIN_FRAME, PID_BROWSER, pm);
    }
}

///
// Close the Window.
///
void CEF_CALLBACK window_helper_close() {
    if (cef_currently_on(TID_UI)) MAIN_WINDOW->close(MAIN_WINDOW);
    else if (cef_currently_on(TID_RENDERER)) {
        char* messagename = "close";
        cef_string_t cef_messagename = {};
        cef_string_utf8_to_utf16(messagename, strlen(messagename), &cef_messagename);
        cef_process_message_t* pm = cef_process_message_create(&cef_messagename);
        if(MAIN_FRAME != NULL) MAIN_FRAME->send_process_message(MAIN_FRAME, PID_BROWSER, pm);
    }
}

///
// Returns whether the Window is the currently active Window.
///
int CEF_CALLBACK window_helper_is_active() {
    if (cef_currently_on(TID_UI) == 0) return 0;
    return MAIN_WINDOW->is_active(MAIN_WINDOW);
}

///
// Bring this Window to the top of other Windows in the Windowing system.
///
void CEF_CALLBACK window_helper_bring_to_top () {
    if (cef_currently_on(TID_UI)) MAIN_WINDOW->bring_to_top(MAIN_WINDOW);
    else if (cef_currently_on(TID_RENDERER)) {
        char* messagename = "bring_to_top";
        cef_string_t cef_messagename = {};
        cef_string_utf8_to_utf16(messagename, strlen(messagename), &cef_messagename);
        cef_process_message_t* pm = cef_process_message_create(&cef_messagename);
        if(MAIN_FRAME != NULL) MAIN_FRAME->send_process_message(MAIN_FRAME, PID_BROWSER, pm);
    }
}

///
// Set the Window to be on top of other Windows in the Windowing system.
///
void CEF_CALLBACK window_helper_set_always_on_top (int on_top) {
    if (cef_currently_on(TID_UI)) MAIN_WINDOW->set_always_on_top(MAIN_WINDOW, on_top);
    else if (cef_currently_on(TID_RENDERER)) {
        char* messagename = "set_always_on_top";
        cef_string_t cef_messagename = {};
        cef_string_utf8_to_utf16(messagename, strlen(messagename), &cef_messagename);
        cef_process_message_t* pm = cef_process_message_create(&cef_messagename);

        cef_list_value_t *args = pm->get_argument_list(pm);
        args->set_int(args, 0, on_top);

        if(MAIN_FRAME != NULL) MAIN_FRAME->send_process_message(MAIN_FRAME, PID_BROWSER, pm);
    }
}

///
// Returns whether the Window has been set to be on top of other Windows in
// the Windowing system.
///
int CEF_CALLBACK window_helper_is_always_on_top () {
    if (cef_currently_on(TID_UI) == 0) return 0;
    return MAIN_WINDOW->is_always_on_top(MAIN_WINDOW);
}

///
// Maximize the Window.
///
void CEF_CALLBACK window_helper_maximize () {
    if (cef_currently_on(TID_UI)) MAIN_WINDOW->maximize(MAIN_WINDOW);
    else if (cef_currently_on(TID_RENDERER)) {
        char* messagename = "maximize";
        cef_string_t cef_messagename = {};
        cef_string_utf8_to_utf16(messagename, strlen(messagename), &cef_messagename);
        cef_process_message_t* pm = cef_process_message_create(&cef_messagename);
        if(MAIN_FRAME != NULL) MAIN_FRAME->send_process_message(MAIN_FRAME, PID_BROWSER, pm);
    }
}

///
// Minimize the Window.
///
void CEF_CALLBACK window_helper_minimize () {
    if (cef_currently_on(TID_UI)) MAIN_WINDOW->minimize(MAIN_WINDOW);
    else if (cef_currently_on(TID_RENDERER)) {
        char* messagename = "minimize";
        cef_string_t cef_messagename = {};
        cef_string_utf8_to_utf16(messagename, strlen(messagename), &cef_messagename);
        cef_process_message_t* pm = cef_process_message_create(&cef_messagename);
        if(MAIN_FRAME != NULL) MAIN_FRAME->send_process_message(MAIN_FRAME, PID_BROWSER, pm);
    }
}

///
// Restore the Window.
///
void CEF_CALLBACK window_helper_restore () {
    if (cef_currently_on(TID_UI)) MAIN_WINDOW->restore(MAIN_WINDOW);
    else if (cef_currently_on(TID_RENDERER)) {
        char* messagename = "restore";
        cef_string_t cef_messagename = {};
        cef_string_utf8_to_utf16(messagename, strlen(messagename), &cef_messagename);
        cef_process_message_t* pm = cef_process_message_create(&cef_messagename);
        if(MAIN_FRAME != NULL) MAIN_FRAME->send_process_message(MAIN_FRAME, PID_BROWSER, pm);
    }
}

///
// Set fullscreen Window state.
///
void CEF_CALLBACK window_helper_set_fullscreen (int fullscreen) {
    if (cef_currently_on(TID_UI)) MAIN_WINDOW->set_fullscreen(MAIN_WINDOW, fullscreen);
    else if (cef_currently_on(TID_RENDERER)) {
        char* messagename = "set_fullscreen";
        cef_string_t cef_messagename = {};
        cef_string_utf8_to_utf16(messagename, strlen(messagename), &cef_messagename);
        cef_process_message_t* pm = cef_process_message_create(&cef_messagename);

        cef_list_value_t *args = pm->get_argument_list(pm);
        args->set_int(args, 0, fullscreen);

        if(MAIN_FRAME != NULL) MAIN_FRAME->send_process_message(MAIN_FRAME, PID_BROWSER, pm);
    }
}

///
// Returns true (1) if the Window is maximized.
///
int CEF_CALLBACK window_helper_is_maximized () {
    if (cef_currently_on(TID_UI) == 0) return 0;
    return MAIN_WINDOW->is_maximized(MAIN_WINDOW);;
}

///
// Returns true (1) if the Window is minimized.
///
int CEF_CALLBACK window_helper_is_minimized () {
    if (cef_currently_on(TID_UI) == 0) return 0;
    return MAIN_WINDOW->is_minimized(MAIN_WINDOW);;
}

///
// Returns true (1) if the Window is fullscreen.
///
int CEF_CALLBACK window_helper_is_fullscreen () {
    if (cef_currently_on(TID_UI) == 0) return 0;
    return MAIN_WINDOW->is_fullscreen(MAIN_WINDOW);;
}

///
// Set the Window title.
///
void CEF_CALLBACK window_helper_set_title (char* title) {
    cef_string_t cef_title = {};
    cef_string_utf8_to_utf16(title, strlen(title), &cef_title);
    if (cef_currently_on(TID_UI)) MAIN_WINDOW->set_title(MAIN_WINDOW, &cef_title);
    else if (cef_currently_on(TID_RENDERER)) {
        char* messagename = "set_title";
        cef_string_t cef_messagename = {};
        cef_string_utf8_to_utf16(messagename, strlen(messagename), &cef_messagename);
        cef_process_message_t* pm = cef_process_message_create(&cef_messagename);

        cef_list_value_t *args = pm->get_argument_list(pm);
        args->set_string(args, 0, &cef_title);

        if(MAIN_FRAME != NULL) MAIN_FRAME->send_process_message(MAIN_FRAME, PID_BROWSER, pm);
    }
}