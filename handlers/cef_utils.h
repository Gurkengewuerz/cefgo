#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "handlers/cef_vars.h"
#include "handlers/cef_app.h"
#include "handlers/cef_client.h"
#include "handlers/cef_base.h"
#include "handlers/cef_settings.h"

#include "include/capi/cef_app_capi.h"
#include "include/capi/cef_base_capi.h"
#include "include/capi/cef_task_capi.h"
#include "include/capi/cef_browser_capi.h"
#include "include/capi/cef_frame_capi.h"
#include "include/capi/cef_v8_capi.h"

int execute_process(app *a, cef_main_args_t *main_args, char *goFuncMap[], int funcMapLength) {
    funcLength = funcMapLength;
    if (funcMapLength > 0) {
        funcMap = &goFuncMap[0];
    }

    // Cef app
    initialize_app_handler(a);
    cef_app_t *app = (cef_app_t *)a;
    app->base.add_ref((cef_base_ref_counted_t *)a);

    // Execute subprocesses. It is also possible to have
    // a separate executable for subprocesses by setting
    // cef_settings_t.browser_subprocess_path. In such
    // case cef_execute_process should not be called here.
    app->base.add_ref((cef_base_ref_counted_t *)a);
    int code = cef_execute_process(main_args, app, NULL);
    return code;
}

void init_gui(
    app *a,
    cef_main_args_t *main_args,
    cef_settings_t *settings,
    gui_settings guiSettings) {
    DEBUG_CALLBACK("init_gui\n");
    cef_app_t *app = (cef_app_t *)a;

    // Initialize CEF
    DEBUG_CALLBACK("cef_initialize\n");
    app->base.add_ref((cef_base_ref_counted_t *)a);
    cef_initialize(main_args, settings, app, NULL);
    app->base.release((cef_base_ref_counted_t *)a);

    // Create the Window. It will show itself after creation.
    window_delegate_t* h = initialize_window_delegate(guiSettings);
    inc_ref(h);
    MAIN_WINDOW = cef_window_create_top_level((cef_window_delegate_t *)h);
}

void run() {
    DEBUG_CALLBACK("cef_run_message_loop\n");
    cef_run_message_loop();

    // Shutdown CEF
    DEBUG_CALLBACK("cef_shutdown\n");
    cef_shutdown();
}

void eval_js(char js[]) {
    char scriptUrl[] = "UNKNOWN.go";
    cef_string_t cefScriptUrl = {};
    cef_string_utf8_to_utf16(scriptUrl, strlen(scriptUrl), &cefScriptUrl);

    cef_string_t cef_js = {};
    cef_string_utf8_to_utf16(js, strlen(js), &cef_js);

    if(cef_currently_on(TID_UI) == 1) MAIN_FRAME->execute_java_script(MAIN_FRAME, &cef_js, &cefScriptUrl, 0);
    if(cef_currently_on(TID_RENDERER) == 1 && cef_v8context_in_context() == 1) {
        cef_v8context_t* cntx = cef_v8context_get_current_context();
        int isValid = cntx->is_valid(cntx);
        if (isValid == 1) {
            cef_v8value_t *retval = NULL;
            cef_v8exception_t *exception = NULL;
            cntx->eval(cntx, &cef_js, &cefScriptUrl, 0, &retval, &exception);
        }
    }
}