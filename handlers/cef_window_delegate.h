#pragma once

#include <stdatomic.h>
#include "handlers/cef_base.h"
#include "handlers/cef_vars.h"
#include "handlers/cef_settings.h"
#include "handlers/cef_client.h"
#include "handlers/cef_browser_delegate.h"
#include "include/capi/cef_browser_capi.h"
#include "include/capi/cef_frame_capi.h"
#include "include/capi/views/cef_window_capi.h"
#include "include/capi/views/cef_view_capi.h"
#include "include/capi/views/cef_window_delegate_capi.h"
#include "include/capi/views/cef_panel_delegate_capi.h"
#include "include/capi/views/cef_view_delegate_capi.h"

typedef struct _window_delegate_t {
	cef_window_delegate_t handler;
	gui_settings settings;
	cef_browser_view_t* browser_view;
    atomic_int ref_count;
} window_delegate_t;

IMPLEMENT_REFCOUNTING(window_delegate_t)

///
// Return true (1) if |window| should be created without a frame or title bar.
// The window will be resizable if can_resize() returns true (1). Use
// cef_window_t::set_draggable_regions() to specify draggable regions.
///
int CEF_CALLBACK is_frameless (struct _cef_window_delegate_t* self, struct _cef_window_t* window) {
    gui_settings* settings = &(((window_delegate_t*)self)->settings);
    return settings->frameless;
}

///
// Return true (1) if |window| can be resized.
///
int CEF_CALLBACK can_resize (struct _cef_window_delegate_t* self, struct _cef_window_t* window) {
    gui_settings* settings = &(((window_delegate_t*)self)->settings);
    return settings->can_resize;
}

///
// Return true (1) if |window| can be maximized.
///
int CEF_CALLBACK can_maximize (struct _cef_window_delegate_t* self, struct _cef_window_t* window) {
    gui_settings* settings = &(((window_delegate_t*)self)->settings);
    return settings->can_maximize;
}

///
// Return true (1) if |window| can be minimized.
///
int CEF_CALLBACK can_minimize (struct _cef_window_delegate_t* self, struct _cef_window_t* window) {
    gui_settings* settings = &(((window_delegate_t*)self)->settings);
    return settings->can_minimize;
}

///
// Return true (1) if |window| can be closed. This will be called for user-
// initiated window close actions and when cef_window_t::close() is called.
///
int CEF_CALLBACK can_close (struct _cef_window_delegate_t* self, struct _cef_window_t* window) {
    DEBUG_CALLBACK("can_close\n");
    return 1;
}

void CEF_CALLBACK on_window_created (struct _cef_window_delegate_t* self, struct _cef_window_t* window) {
    DEBUG_CALLBACK("----------- window delegate created -----------\n");
    struct _cef_browser_settings_t browser_settings = {};
    browser_settings.size = sizeof(cef_browser_settings_t);
    browser_settings.databases = STATE_DISABLED;
    browser_settings.local_storage = STATE_ENABLED;
    browser_settings.application_cache = STATE_DISABLED;

    gui_settings* settings = &(((window_delegate_t*)self)->settings);

    client_t *c = calloc(1, sizeof(client_t));
    initialize_client_handler(c);
    cef_client_t *client = (cef_client_t *)c;
    client->base.add_ref((cef_base_ref_counted_t *)client);

    client->base.add_ref((cef_base_ref_counted_t *)client);

    cef_string_t cef_title = {};
    cef_string_utf8_to_utf16(settings->title, strlen(settings->title), &cef_title);

    cef_string_t cef_url = {};
    cef_string_utf8_to_utf16(settings->url, strlen(settings->url), &cef_url);

    browser_view_delegate_t* bvd = initialize_browser_view_delegate();
    inc_ref_browser(bvd);

    (((window_delegate_t*)self)->browser_view) = cef_browser_view_create(client, &cef_url, &browser_settings,
                            cef_dictionary_value_create(), cef_request_context_get_global_context(), (cef_browser_view_delegate_t *)bvd);
    cef_browser_view_t* bv = (((window_delegate_t*)self)->browser_view);
    bv->base.base.add_ref((cef_base_ref_counted_t*)bv);

    // Add the browser view and show the window.
    window->base.add_child_view((cef_panel_t*)window, (cef_view_t*)bv);
    window->show(window);

    window->set_title(window, &cef_title);

    cef_size_t size = {
        .width = settings->width,
        .height = settings->height,
    };
    window->center_window(window, &size);

    if(settings->maximized == 1) window->maximize(window);
    window->set_fullscreen(window, settings->fullscreen);
}

void inc_ref (struct _window_delegate_t* self) {
    atomic_fetch_add(&self->ref_count, 1);
}

struct _window_delegate_t* initialize_window_delegate(gui_settings settings)
{
    DEBUG_CALLBACK("initialize_window_delegate\n");

    window_delegate_t *a = calloc(1, sizeof(window_delegate_t));

    a->settings = settings;
    a->browser_view = calloc(1, sizeof(cef_browser_view_t));


    cef_window_delegate_t* handler = (cef_window_delegate_t*)a;

    handler->on_window_created = on_window_created;
    handler->is_frameless = is_frameless;
    handler->can_resize = can_resize;
    handler->can_maximize = can_maximize;
    handler->can_minimize = can_minimize;
    handler->can_close = can_close;

    cef_panel_delegate_t* panel = (cef_panel_delegate_t*)handler;
    cef_view_delegate_t* view = (cef_view_delegate_t*)panel;
    view->get_preferred_size = NULL;
    view->get_minimum_size = NULL;
    view->get_maximum_size = NULL;
    view->get_height_for_width = NULL;
    view->on_parent_view_changed = NULL;
    view->on_child_view_changed = NULL;
    view->on_focus = NULL;
    view->on_blur = NULL;

    cef_base_ref_counted_t* ref = (cef_base_ref_counted_t*)view;
    ref->size = sizeof(window_delegate_t);
    ref->add_ref = window_delegate_t_add_ref;
    ref->release = window_delegate_t_release;
    ref->has_one_ref = window_delegate_t_has_one_ref;
    ref->has_at_least_one_ref = NULL;

    a->ref_count = 1;
    return a;
}
