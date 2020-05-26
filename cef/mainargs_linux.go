package cef

import "C"
import "os"

func (cefClient *CEF) fillMainArgs()  {
	args := os.Args

	cefClient.Logger.Println("fillMainArgs", args, len(args))

	argv := make([]*C.char, len(args))
	for idx, arg := range args {
		cs := C.CString(arg)
		argv[idx] = cs
		idx++
	}

	cefClient.mainArgs.argc = C.int(len(args))
	cefClient.mainArgs.argv = &argv[0]
}