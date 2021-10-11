#pragma once

#include "handlers/cef_base.h"
#include "include/capi/cef_display_handler_capi.h"

extern void ConsoleMessage(int, char*, int, char*);

typedef struct _display_handler {
    cef_display_handler_t handler;
    atomic_int ref_count;
} display_handler;

IMPLEMENT_REFCOUNTING(display_handler)
GENERATE_CEF_BASE_INITIALIZER(display_handler)

///
// Called to display a console message. Return true (1) to stop the message
// from being output to the console.
///
int CEF_CALLBACK on_console_message(struct _cef_display_handler_t* self,
                                    struct _cef_browser_t* browser,
                                    cef_log_severity_t level, const cef_string_t* message,
                                    const cef_string_t* source,
                                    int line) {
    cef_string_utf8_t outMessage = {};
    cef_string_utf8_t outSource = {};

    cef_string_utf16_to_utf8(message->str, message->length, &outMessage);
    cef_string_utf16_to_utf8(source->str, source->length, &outSource);

    DEBUG_CALLBACK("on_console_message\n");

    ConsoleMessage(level, outSource.str, line, outMessage.str);

    cef_string_utf8_clear(&outMessage);
    cef_string_utf8_clear(&outSource);

    return 0;
}

void initialize_displayhandler(display_handler* h) {
    DEBUG_CALLBACK("initialize_displayhandler\n");
    initialize_cef_base(h);
    cef_display_handler_t* handler = (cef_display_handler_t*)h;
    // callbacks
    handler->on_console_message = on_console_message;
}