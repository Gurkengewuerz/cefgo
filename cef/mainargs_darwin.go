package cef

/*
#cgo CFLAGS: -I./../
#include <stdlib.h>

#if defined(__unix__) || defined(__APPLE__)
#define HINSTANCE int
#endif
*/
import "C"
import "os"

func (cefClient *CEF) fillMainArgs() (int, []*C.char, C.HINSTANCE) {
	args := os.Args

	cefClient.logger.Println("fillMainArgs", args, len(args))

	arglist := make([]*C.char, len(args))
	for idx, arg := range args {
		cs := C.CString(arg)
		arglist[idx] = cs
		idx++
	}

	return len(args), arglist, 0
}
