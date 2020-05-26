package cef

import "C"
import "os"

func (cefClient *CEF) fillMainArgs()  {
	args := os.Args

	argv := make([]*C.char, len(args))
	idx := 0
	for arg, _ := range _BindFunc {
		cs := C.CString(arg)
		argv[idx] = cs
		idx++
	}

	cefClient.mainArgs.argc = C.int(len(args))
	cefClient.mainArgs.argv = &argv[0]
}