package cef
/*
#cgo CFLAGS: -I./../
#cgo LDFLAGS: -L./../Release -llibcef
#include <stdlib.h>
#include "include/capi/cef_app_capi.h"
*/
import "C"
import "unsafe"

func (cefClient *CEF) fillMainArgs()  {
	cefClient.mainArgs.instance = (C.HINSTANCE)(unsafe.Pointer(GetModuleHandle()))
}