#pragma once

#ifdef __unix__
#define HINSTANCE int
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "handlers/cef_vars.h"
#include "handlers/cef_app.h"
#include "handlers/cef_settings.h"
#include "handlers/cef_window_delegate.h"

#include "include/capi/cef_app_capi.h"
#include "include/capi/cef_base_capi.h"
#include "include/capi/cef_task_capi.h"
#include "include/capi/cef_browser_capi.h"
#include "include/capi/cef_frame_capi.h"
#include "include/capi/cef_v8_capi.h"

void init(app *a) {
    // Cef app
    initialize_app_handler(a);
    cef_app_t *app = (cef_app_t *)a;
    app->base.add_ref((cef_base_ref_counted_t *)a);
}

int execute_process(app *a, int argc, char** argv, HINSTANCE instance) {

    cef_main_args_t main_args = {};
    // Main args
    #ifdef __unix__
    main_args.argc = argc;
    main_args.argv = argv;
    #else
    #ifndef IS_GO
    main_args.instance = GetModuleHandle(NULL);
    #else
    main_args.instance = instance;
    #endif
    #endif

    printf("->>>>>>>>%s\n", main_args.argv[0]);

    printf("\nProcess args (2 => %d): ", argc);
    if (argc == 1) {
        printf("none (Main process)");
    } else {
        for(int index = 0; index < argc; index++) {
            printf("%s ", argv[index]);
        }
    }
    printf("\n\n");

    cef_app_t *app = (cef_app_t *)a;
    app->base.add_ref((cef_base_ref_counted_t *)a);
    int code = cef_execute_process(&main_args, app, NULL);
    return code;
}

void init_gui(
    app *a,
    cef_settings_t *settings,
    gui_settings guiSettings,
    char *goFuncMap[],
    int funcMapLength,
    int argc, char** argv, HINSTANCE instance) {
    DEBUG_CALLBACK("init_gui\n");

    cef_main_args_t main_args = {};
    // Main args
    #ifdef __unix__
    main_args.argc = argc;
    main_args.argv = argv;
    #else
    #ifndef IS_GO
    main_args.instance = GetModuleHandle(NULL);
    #else
    main_args.instance = instance;
    #endif
    #endif

    funcLength = funcMapLength;
    if (funcMapLength > 0) {
        funcMap = &goFuncMap[0];
    }

    cef_app_t *app = (cef_app_t *)a;

    // Initialize CEF
    DEBUG_CALLBACK("cef_initialize\n");
    app->base.add_ref((cef_base_ref_counted_t *)a);
    cef_initialize(&main_args, settings, app, NULL);
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