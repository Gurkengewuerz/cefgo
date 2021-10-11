// CEF C API example
// Project website: https://github.com/cztomczak/cefcapi

#pragma once

#include "handlers/cef_base.h"
#include "handlers/cef_life_span_handler.h"
#include "handlers/cef_draghandler.h"
#include "handlers/cef_displayhandler.h"
#include "handlers/cef_window_helper.h"
#include "include/capi/cef_client_capi.h"
#include "include/capi/views/cef_window_capi.h"

typedef struct _client_t {
	cef_client_t client;
	cef_window_t* window;
	atomic_int ref_count;
} client_t;

IMPLEMENT_REFCOUNTING(client_t)
GENERATE_CEF_BASE_INITIALIZER(client_t)

// ----------------------------------------------------------------------------
// struct cef_client_t
// ----------------------------------------------------------------------------

///
// Implement this structure to provide handler implementations.
///

///
// Return the handler for context menus. If no handler is
// provided the default implementation will be used.
///

struct _cef_context_menu_handler_t* CEF_CALLBACK get_context_menu_handler(
        struct _cef_client_t* self) {
    DEBUG_CALLBACK("get_context_menu_handler\n");
    return NULL;
}

///
// Return the handler for dialogs. If no handler is provided the default
// implementation will be used.
///
struct _cef_dialog_handler_t* CEF_CALLBACK get_dialog_handler(
        struct _cef_client_t* self) {
    DEBUG_CALLBACK("get_dialog_handler\n");
    return NULL;
}

///
// Return the handler for browser display state events.
///
struct _cef_display_handler_t* CEF_CALLBACK get_display_handler(
        struct _cef_client_t* self) {
    DEBUG_CALLBACK("get_display_handler\n");
    display_handler *h;
    h = calloc(1, sizeof(display_handler));
    initialize_displayhandler(h);
    cef_display_handler_t *handler = &h->handler;
    handler->base.add_ref((cef_base_ref_counted_t *)h);

    return handler;
}

///
// Return the handler for download events. If no handler is returned downloads
// will not be allowed.
///
struct _cef_download_handler_t* CEF_CALLBACK get_download_handler(
        struct _cef_client_t* self) {
    DEBUG_CALLBACK("get_download_handler\n");
    return NULL;
}

///
// Return the handler for drag events.
///
struct _cef_drag_handler_t* CEF_CALLBACK get_drag_handler(
        struct _cef_client_t* self) {
    DEBUG_CALLBACK("get_drag_handler\n");
    drag_handler *h;
    h = calloc(1, sizeof(drag_handler));
    initialize_draghandler(h);
    cef_drag_handler_t *handler = &h->handler;
    handler->base.add_ref((cef_base_ref_counted_t *)h);

    return handler;
}

///
// Return the handler for focus events.
///
struct _cef_focus_handler_t* CEF_CALLBACK get_focus_handler(
        struct _cef_client_t* self) {
    DEBUG_CALLBACK("get_focus_handler\n");
    return NULL;
}

///
// Return the handler for geolocation permissions requests. If no handler is
// provided geolocation access will be denied by default.
///
struct _cef_geolocation_handler_t* CEF_CALLBACK get_geolocation_handler(
        struct _cef_client_t* self) {
    DEBUG_CALLBACK("get_geolocation_handler\n");
    return NULL;
}

///
// Return the handler for JavaScript dialogs. If no handler is provided the
// default implementation will be used.
///
struct _cef_jsdialog_handler_t* CEF_CALLBACK get_jsdialog_handler(
        struct _cef_client_t* self) {
    DEBUG_CALLBACK("get_jsdialog_handler\n");
    return NULL;
}

///
// Return the handler for keyboard events.
///
struct _cef_keyboard_handler_t* CEF_CALLBACK get_keyboard_handler(
        struct _cef_client_t* self) {
    DEBUG_CALLBACK("get_keyboard_handler\n");
    return NULL;
}

///
// Return the handler for browser life span events.
///
struct _cef_life_span_handler_t* CEF_CALLBACK get_life_span_handler(
        struct _cef_client_t* self) {
    DEBUG_CALLBACK("get_life_span_handler\n");

    
    life_span_handler_t *h;
    h = calloc(1, sizeof(life_span_handler_t));

    cef_life_span_handler_t *handler = &h->handler;

    initialize_cef_base(h);
    handler->on_before_popup = on_before_popup;
    handler->on_after_created = on_after_created;
    handler->on_before_close = on_before_close;

    handler->base.add_ref((cef_base_ref_counted_t *)h);

    return handler;
}

///
// Return the handler for browser load status events.
///
struct _cef_load_handler_t* CEF_CALLBACK get_load_handler(
        struct _cef_client_t* self) {
    DEBUG_CALLBACK("get_load_handler\n");
    return NULL;
}

///
// Return the handler for off-screen rendering events.
///
struct _cef_render_handler_t* CEF_CALLBACK get_render_handler(
        struct _cef_client_t* self) {
    DEBUG_CALLBACK("get_render_handler\n");
    return NULL;
}

///
// Return the handler for browser request events.
///
struct _cef_request_handler_t* CEF_CALLBACK get_request_handler(
        struct _cef_client_t* self) {
    DEBUG_CALLBACK("get_request_handler\n");
    return NULL;
}

///
// Called when a new message is received from a different process. Return true
// (1) if the message was handled or false (0) otherwise. Do not keep a
// reference to or attempt to access the message outside of this callback.
///
int CEF_CALLBACK on_process_message_received(
      struct _cef_client_t* self,
      struct _cef_browser_t* browser,
      struct _cef_frame_t* frame,
      cef_process_id_t source_process,
      struct _cef_process_message_t* message) {
    DEBUG_CALLBACK("on_process_message_received\n");
    int success = 0;
    cef_string_userfree_t name = message->get_name(message);
    cef_string_utf8_t out = {};
    cef_string_utf16_to_utf8(name->str, name->length, &out);
    cef_string_userfree_free(name);
    cef_list_value_t *arguments = message->get_argument_list(message);


    if (strcmp(out.str, "show") == 0 && cef_currently_on(TID_UI) == 1) window_helper_show();
    else if (strcmp(out.str, "hide") == 0 && cef_currently_on(TID_UI) == 1) window_helper_hide();
    else if (strcmp(out.str, "close") == 0 && cef_currently_on(TID_UI) == 1) window_helper_close();
    else if (strcmp(out.str, "bring_to_top") == 0 && cef_currently_on(TID_UI) == 1) window_helper_bring_to_top();
    else if (strcmp(out.str, "maximize") == 0 && cef_currently_on(TID_UI) == 1) window_helper_maximize();
    else if (strcmp(out.str, "minimize") == 0 && cef_currently_on(TID_UI) == 1) window_helper_minimize();
    else if (strcmp(out.str, "restore") == 0 && cef_currently_on(TID_UI) == 1) window_helper_restore();

    else if (strcmp(out.str, "set_always_on_top") == 0 && cef_currently_on(TID_UI) == 1) {
        window_helper_set_always_on_top(arguments->get_int(arguments, 0));
    } else if (strcmp(out.str, "center_window") == 0 && cef_currently_on(TID_UI) == 1) {
        window_helper_center_window(arguments->get_int(arguments, 0), arguments->get_int(arguments, 1));
    } else if (strcmp(out.str, "set_fullscreen") == 0 && cef_currently_on(TID_UI) == 1) {
        window_helper_set_fullscreen(arguments->get_int(arguments, 0));
    } else if (strcmp(out.str, "set_title") == 0 && cef_currently_on(TID_UI) == 1) {
        cef_string_userfree_t value = arguments->get_string(arguments, 0);
        cef_string_utf8_t title = {};
        cef_string_utf16_to_utf8(value->str, value->length, &title);
        cef_string_userfree_free(value);

        window_helper_set_title(title.str);

        cef_string_utf8_clear(&title);
    }
    cef_string_utf8_clear(&out);


    return success;
}

void initialize_client_handler(client_t* c) {
    DEBUG_CALLBACK("initialize_client_handler\n");
    cef_client_t *client = (cef_client_t *)c;
    initialize_cef_base(c);
    // callbacks
    client->get_context_menu_handler = get_context_menu_handler;
    client->get_dialog_handler = get_dialog_handler;
    client->get_display_handler = get_display_handler;
    client->get_download_handler = get_download_handler;
    client->get_drag_handler = get_drag_handler;
    client->get_focus_handler = get_focus_handler;
    //client->get_geolocation_handler = get_geolocation_handler;
    client->get_jsdialog_handler = get_jsdialog_handler;
    client->get_keyboard_handler = get_keyboard_handler;
    client->get_life_span_handler = get_life_span_handler;  // Implemented!
    client->get_load_handler = get_load_handler;
    client->get_render_handler = get_render_handler;
    client->get_request_handler = get_request_handler;
    client->on_process_message_received = on_process_message_received;
}
