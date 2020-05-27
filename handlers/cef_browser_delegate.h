#pragma once

#include <stdatomic.h>
#include "handlers/cef_vars.h"
#include "handlers/cef_base.h"
#include "include/capi/cef_frame_capi.h"
#include "include/capi/cef_browser_capi.h"


typedef struct _browser_view_delegate_t {
	cef_browser_view_delegate_t handler;
    atomic_int ref_count;
} browser_view_delegate_t;

IMPLEMENT_REFCOUNTING(browser_view_delegate_t)

void inc_ref_browser (struct _browser_view_delegate_t* self) {
    atomic_fetch_add(&self->ref_count, 1);
}

///
// Called when |browser| associated with |browser_view| is created. This
// function will be called after cef_life_span_handler_t::on_after_created()
// is called for |browser| and before on_popup_browser_view_created() is
// called for |browser|'s parent delegate if |browser| is a popup.
///
void CEF_CALLBACK on_browser_created (struct _cef_browser_view_delegate_t* self, struct _cef_browser_view_t* browser_view, struct _cef_browser_t* browser) {
    DEBUG_CALLBACK("on_browser_created\n");
    MAIN_FRAME = browser->get_main_frame(browser);
}

struct _browser_view_delegate_t* initialize_browser_view_delegate()
{
    DEBUG_CALLBACK("initialize_browser_view_delegate\n");

    browser_view_delegate_t *a = calloc(1, sizeof(browser_view_delegate_t));

    cef_browser_view_delegate_t* handler = (cef_browser_view_delegate_t*)a;

    handler->on_browser_created = on_browser_created;

    cef_view_delegate_t* view = (cef_view_delegate_t*)handler;
    view->get_preferred_size = NULL;
    view->get_minimum_size = NULL;
    view->get_maximum_size = NULL;
    view->get_height_for_width = NULL;
    view->on_parent_view_changed = NULL;
    view->on_child_view_changed = NULL;
    view->on_focus = NULL;
    view->on_blur = NULL;

    cef_base_ref_counted_t* ref = (cef_base_ref_counted_t*)view;
    ref->size = sizeof(browser_view_delegate_t);
    ref->add_ref = browser_view_delegate_t_add_ref;
    ref->release = browser_view_delegate_t_release;
    ref->has_one_ref = browser_view_delegate_t_has_one_ref;
    ref->has_at_least_one_ref = NULL;

    a->ref_count = 1;
    return a;
}
