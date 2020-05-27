// CEF C API example
// Project website: https://github.com/cztomczak/cefcapi

#pragma once

#include "handlers/cef_base.h"
#include "include/capi/cef_app_capi.h"
#include "include/capi/cef_life_span_handler_capi.h"

typedef struct _life_span_handler_t {
	cef_life_span_handler_t handler;
	atomic_int ref_count;
} life_span_handler_t;

IMPLEMENT_REFCOUNTING(life_span_handler_t)
GENERATE_CEF_BASE_INITIALIZER(life_span_handler_t)

// ----------------------------------------------------------------------------
// struct cef_life_span_handler_t
// ----------------------------------------------------------------------------

///
// Implement this structure to handle events related to browser life span. The
// functions of this structure will be called on the UI thread unless otherwise
// indicated.
///

// NOTE: There are many more callbacks in cef_life_span_handler,
//       but only on_before_close is implemented here.

///
// Called just before a browser is destroyed. Release all references to the
// browser object and do not attempt to execute any functions on the browser
// object after this callback returns. This callback will be the last
// notification that references |browser|. See do_close() documentation for
// additional usage information.
///
void CEF_CALLBACK on_before_close(struct _cef_life_span_handler_t* self,
                                  struct _cef_browser_t* browser) {
    DEBUG_CALLBACK("on_before_close\n");
    // TODO: Check how many browsers do exist and quit message
    //       loop only when last browser is closed. Otherwise
    //       closing a popup window will exit app while main
    //       window shouldn't be closed.
    browser->base.release((cef_base_ref_counted_t *)browser);
    cef_quit_message_loop();
}

void CEF_CALLBACK on_after_created (struct _cef_life_span_handler_t* self, struct _cef_browser_t* browser) {
        DEBUG_CALLBACK("on_after_created\n");
}

int CEF_CALLBACK on_before_popup (struct _cef_life_span_handler_t* self, struct _cef_browser_t* browser, struct _cef_frame_t* frame,
      const cef_string_t* target_url, const cef_string_t* target_frame_name, cef_window_open_disposition_t target_disposition,
      int user_gesture, const struct _cef_popup_features_t* popupFeatures, struct _cef_window_info_t* windowInfo, struct _cef_client_t** client,
      struct _cef_browser_settings_t* settings, struct _cef_dictionary_value_t** extra_info, int* no_javascript_access) {
    return 1;
}
