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

// TODO. rewrite for subprocess

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
        for (int i = i; i < argc; i++) {
            if (strlen(argv[i]) > 128)
                printf("... ");
            else
                printf("%s ", argv[i]);
        }
    }
    printf("\n\n");


    // Cef app
    app *a = calloc(1, sizeof(app));

    init(a);

    int code = execute_process(a, argc, argv, 0);
    if (code >= 0) {
        _exit(code);
    } 
    
    // Application settings. It is mandatory to set the
    // "size" member.
    cef_settings_t settings = {};
    settings.size = sizeof(cef_settings_t);
    settings.log_severity = LOGSEVERITY_VERBOSE;
    settings.no_sandbox = 1;
    settings.remote_debugging_port = 6696;

    gui_settings guiSettings = {};
    guiSettings.frameless = 0;
    guiSettings.maximized = 0;
    guiSettings.fullscreen = 0;
    guiSettings.height = 700;
    guiSettings.width = 700;
    guiSettings.url = "https://app01.mc8051.de";
    guiSettings.title = "Example";
    guiSettings.can_maximize = 0;
    guiSettings.can_minimize = 0;
    guiSettings.can_resize = 1;

    init_gui(a, &settings, guiSettings, NULL, 0, argc, argv, 0);

    run();

    return 0;
}
