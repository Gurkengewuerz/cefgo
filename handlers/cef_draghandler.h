#pragma once

#include "handlers/cef_base.h"
#include "include/capi/cef_drag_handler_capi.h"

typedef struct _drag_handler {
	cef_drag_handler_t handler;
	atomic_int ref_count;
} drag_handler;

IMPLEMENT_REFCOUNTING(drag_handler)
GENERATE_CEF_BASE_INITIALIZER(drag_handler)

///
// Called when an external drag event enters the browser window. |dragData|
// contains the drag event data and |mask| represents the type of drag
// operation. Return false (0) for default drag handling behavior or true (1)
// to cancel the drag event.
///
int CEF_CALLBACK on_drag_enter (struct _cef_drag_handler_t* self,
                               struct _cef_browser_t* browser,
                               struct _cef_drag_data_t* dragData,
                               cef_drag_operations_mask_t mask) {
    DEBUG_CALLBACK("on_drag_enter\n");
    return 0;
}

///
// Called whenever draggable regions for the browser window change. These can
// be specified using the '-webkit-app-region: drag/no-drag' CSS-property. If
// draggable regions are never defined in a document this function will also
// never be called. If the last draggable region is removed from a document
// this function will be called with an NULL vector.
///
void CEF_CALLBACK on_draggable_regions_changed (struct _cef_drag_handler_t* self,
                                                  struct _cef_browser_t* browser,
                                                  struct _cef_frame_t* frame,
                                                  size_t regionsCount,
                                                  cef_draggable_region_t const* regions) {
    DEBUG_CALLBACK("on_draggable_regions_changed\n");
    if(browser->is_popup(browser) == 1) return;
    MAIN_WINDOW->set_draggable_regions(MAIN_WINDOW, regionsCount, regions);
}

void initialize_draghandler(drag_handler *h)
{
    DEBUG_CALLBACK("initialize_draghandler\n");
    initialize_cef_base(h);
    cef_drag_handler_t *handler = (cef_drag_handler_t *)h;
    // callbacks
    handler->on_drag_enter = on_drag_enter;
    handler->on_draggable_regions_changed = on_draggable_regions_changed;
}