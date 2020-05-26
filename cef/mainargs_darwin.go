package cef

import "C"
import "os"

var _Argv = make([]*C.char, len(os.Args))

func (cefClient *CEF) fillMainArgs()  {
	args := os.Args

	argv := make([]*C.char, len(args))
	idx := 0
	for arg, _ := range args {
		cs := C.CString(arg)
		argv[idx] = cs
		idx++
	}

	cefClient.mainArgs.argc = C.int(len(args))
	cefClient.mainArgs.argv = &argv[0]
}