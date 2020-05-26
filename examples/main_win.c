#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "handlers/cef_test.h"
#include "handlers/cef_app.h"
#include "handlers/cef_client.h"
#include "handlers/cef_base.h"
#include "handlers/cef_window_delegate.h"
#include "handlers/cef_vars.h"
#include "handlers/cef_utils.h"


int main(int argc, char** argv) {
    // This executable is called many times, because it
    // is also used for subprocesses. Let's print args
    // so we can differentiate between main process and
    // subprocesses. If one of the first args is for
    // example "--type=renderer" then it means that
    // this is a Renderer process. There may be more
    // subprocesses like GPU (--type=gpu-process) and
    // others. On Linux there are also special Zygote
    // processes.
    printf("\nProcess args: ");
    if (argc == 1) {
        printf("none (Main process)");
    } else {
        for (int i = 1; i < argc; i++) {
            if (strlen(argv[i]) > 128)
                printf("... ");
            else
                printf("%s ", argv[i]);
        }
    }
    printf("\n\n");

    cef_main_args_t main_args = {};
    // Main args
    #ifdef __unix__
    main_args.argc = argc;
    main_args.argv = argv;
    #else
    main_args.instance = GetModuleHandle(NULL);
    #endif

    // Cef app
    app *a = calloc(1, sizeof(app));

    int code = execute_process(a, &main_args, NULL, 0);
    if (code >= 0) {
        _exit(code);
    } 
    
    // Application settings. It is mandatory to set the
    // "size" member.
    cef_settings_t settings = {};
    settings.size = sizeof(cef_settings_t);
    settings.log_severity = LOGSEVERITY_WARNING; // Show only warnings/errors
    settings.no_sandbox = 1;
    settings.remote_debugging_port = 6696;

    gui_settings guiSettings = {};
    guiSettings.frameless = FALSE;
    guiSettings.maximized = FALSE;
    guiSettings.fullscreen = FALSE;
    guiSettings.height = 700;
    guiSettings.width = 700;
    guiSettings.url = "https://app01.mc8051.de";
    guiSettings.title = "Example";
    guiSettings.can_maximize = FALSE;
    guiSettings.can_minimize = FALSE;
    guiSettings.can_resize = TRUE;

    init_gui(a, &main_args, &settings, guiSettings);

    run();

    return 0;
}
