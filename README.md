# cefgo

These are simple bindings for [CEF](https://bitbucket.org/chromiumembedded/cef/src/master/) `C API` in Go.
It's not documented yet, but you can follow the [example](examples/main.go) to figure out how to use. It's pretty simple.  
In order to run the examples, the CEF supporting files must be placed beside the executable.

## Features
- JS to Go callbacks
- Go to JS eval
- frameless window

## TODO
- add window functions (close, resize, etc. for JS callbacks especially)
- console output

## CEF Version and Compatibility
83.3.9+ged43e55+chromium-83.0.4103.61
This lib is compiled against CEF 83.3.9+ged43e55+chromium-83.0.4103.61 / Chromium 83.0.4103.61 from http://opensource.spotify.com/cefbuilds/index.html

## Installation
1. Download the CEF SDK for your system from [Spotify](http://opensource.spotify.com/cefbuilds/index.html).
2. Locate your installation of cefgo (typically in $GOPATH/src/github.com/Gurkengewuerz/cefgo or $GOPATH/pkg/mod/github.com/Gurkengewuerz/cefgo), and extract the CEF SDK.  
    Copy include/* to cefgo/include  
    Copy Release/* to cefgo/Release  
    Copy Resources/* to cefgo/Release  
3. Run your binaries inside the Release folder

## About JS callbacks
Callbacks need to be registered before browsers are created. Currently only basic type conversions are supported (undefined/null, bool, int/uint, double, string).
Objects and Arrays are possible, but not done. If you are running in multi-process mode (the default - as opposed to single-process) keep in mind the callback will be executed in the browser process and not in the main process.

## Updating the CEF Version
The process for updating CEF to a newer version is relatively simple.
1. Download a new version from Spotify and extract.
2. Update the headers inside the include folder
3. Extract the new cef resources and libs to the Release folder
4. Try to compile the test application: ``go clean -cache -testcache && CGO_ENABLED=1 go build -o Release/test.exe examples/main.go``
5. Fix any incompatibilities with new APIs until the application compiles.
6. Push the new version, and update the version listed in this README.