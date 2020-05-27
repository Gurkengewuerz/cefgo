package cef
/*
#cgo CFLAGS: -I./../
#cgo LDFLAGS: -L./../Release -llibcef
#include <stdlib.h>
#include "include/capi/cef_app_capi.h"
*/
import "C"
import "unsafe"

func (cefClient *CEF) fillMainArgs() (int, []*C.char, C.HINSTANCE) {
	return 0, nil, (C.HINSTANCE)(unsafe.Pointer(GetModuleHandle()))
}