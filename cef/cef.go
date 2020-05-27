package cef

/*
#cgo CFLAGS: -I./../ -DIS_GO=TRUE
#cgo windows LDFLAGS: -L./../Release -lcef
#cgo linux LDFLAGS: -L./../Release -lcef -Wl,-rpath=./
#cgo darwin LDFLAGS: -L./../Release -lcef -Wl,-rpath=./

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "handlers/cef_utils.h"
*/
import "C"
import (
	"encoding/json"
	"errors"
	"io/ioutil"
	"reflect"
	"unsafe"
)
import (
	"log"
	"os"
	"runtime"
)

type CEF struct {
	logger      *log.Logger
	guiSettings GuiSettings
	appHandler  *C.struct__app
	cefSettings *C.struct__cef_settings_t
}

var _BindFunc map[string]func(req string) (interface{}, error)

func (cefClient *CEF) SetLogger(logger *log.Logger) {
	cefClient.logger = logger
}

func (cefClient *CEF) initializeGlobalCStructures() {
	cefClient.appHandler = (*C.struct__app)(C.calloc(1, C.sizeof_struct__app))
}

func New(settings GuiSettings, logger *log.Logger) *CEF {
	if logger == nil {
		logger = log.New(ioutil.Discard, "cef", log.LstdFlags)
	}

	c := &CEF{
		logger:      logger,
		guiSettings: settings,
		appHandler:  nil,
		cefSettings: nil,
	}

	c.initializeGlobalCStructures()
	C.init(c.appHandler)
	return c
}

func (cefClient *CEF) InitSubprocess() int {
	argc, argv, handle := cefClient.fillMainArgs()
	var args **C.char
	if argv != nil {
		args = &argv[0]
	}

	return int(C.execute_process(cefClient.appHandler, C.int(argc), args, handle))
}

func (cefClient *CEF) bind(name string, f interface{}) error {
	v := reflect.ValueOf(f)
	// f must be a function
	if v.Kind() != reflect.Func {
		return errors.New("only functions can be bound")
	}
	// f must return either value and error or just error
	if n := v.Type().NumOut(); n > 2 {
		return errors.New("function may only return a value or a value+error")
	}

	_BindFunc[name] = func(req string) (interface{}, error) {
		raw := []json.RawMessage{}
		if err := json.Unmarshal([]byte(req), &raw); err != nil {
			return nil, err
		}

		isVariadic := v.Type().IsVariadic()
		numIn := v.Type().NumIn()
		if (isVariadic && len(raw) < numIn-1) || (!isVariadic && len(raw) != numIn) {
			return nil, errors.New("function arguments mismatch")
		}
		args := []reflect.Value{}
		for i := range raw {
			var arg reflect.Value
			if isVariadic && i >= numIn-1 {
				arg = reflect.New(v.Type().In(numIn - 1).Elem())
			} else {
				arg = reflect.New(v.Type().In(i))
			}
			if err := json.Unmarshal(raw[i], arg.Interface()); err != nil {
				return nil, err
			}
			args = append(args, arg.Elem())
		}
		errorType := reflect.TypeOf((*error)(nil)).Elem()
		res := v.Call(args)
		switch len(res) {
		case 0:
			// No results from the function, just return nil
			return nil, nil
		case 1:
			// One result may be a value, or an error
			if res[0].Type().Implements(errorType) {
				if res[0].Interface() != nil {
					return nil, res[0].Interface().(error)
				}
				return nil, nil
			}
			return res[0].Interface(), nil
		case 2:
			// Two results: first one is value, second is error
			if !res[1].Type().Implements(errorType) {
				return nil, errors.New("second return value must be an error")
			}
			if res[1].Interface() == nil {
				return res[0].Interface(), nil
			}
			return res[0].Interface(), res[1].Interface().(error)
		default:
			return nil, errors.New("unexpected number of return values")
		}
	}
	return nil
}

func (cefClient *CEF) boolToCInt(x bool) C.int {
	if x {
		return C.int(1)
	}
	return C.int(0)
}

func (cefClient *CEF) OpenWindow() {
	cefClient.logger.Println("ExecuteProcess")

	_BindFunc = make(map[string]func(req string) (interface{}, error))
	for name, i := range cefClient.guiSettings.BindFunc {
		_ = cefClient.bind(name, i)
	}

	var cFuncArray **C.char
	funcArraySize := 0
	if len(_BindFunc) > 0 {
		argv := make([]*C.char, len(_BindFunc))
		idx := 0
		for name := range _BindFunc {
			cs := C.CString(name)
			argv[idx] = cs
			idx++
		}
		cFuncArray = &argv[0]
		funcArraySize = idx
	}

	cefClient.initializeSettings(cefClient.guiSettings.Settings)

	cWindowName := C.CString(cefClient.guiSettings.WindowName)
	defer C.free(unsafe.Pointer(cWindowName))
	cStartURL := C.CString(cefClient.guiSettings.StartURL)
	defer C.free(unsafe.Pointer(cStartURL))

	var gs C.struct__gui_settings
	gs.url = C.CString(cefClient.guiSettings.StartURL)
	gs.title = C.CString(cefClient.guiSettings.WindowName)
	gs.frameless = cefClient.boolToCInt(cefClient.guiSettings.IsFrameless)
	gs.maximized = cefClient.boolToCInt(cefClient.guiSettings.IsMaximized)
	gs.fullscreen = cefClient.boolToCInt(cefClient.guiSettings.IsFullscreen)
	gs.can_maximize = cefClient.boolToCInt(cefClient.guiSettings.CanMaximize)
	gs.can_minimize = cefClient.boolToCInt(cefClient.guiSettings.CanMinimize)
	gs.can_resize = cefClient.boolToCInt(cefClient.guiSettings.CanResize)
	gs.height = C.int(cefClient.guiSettings.Height)
	gs.width = C.int(cefClient.guiSettings.Width)
	gs.window_icon = C.CString(cefClient.guiSettings.WindowIcon)
	gs.window_app_icon = C.CString(cefClient.guiSettings.WindowAppIcon)

	if cefClient.guiSettings.BackgroundColor != nil {
		r, g, b, a := cefClient.guiSettings.BackgroundColor.RGBA()
		gs.BGA = (C.uint8_t)(a)
		gs.BGR = (C.uint8_t)(r)
		gs.BGG = (C.uint8_t)(g)
		gs.BGB = (C.uint8_t)(b)
	} else {
		gs.BGA = C.uint8_t(255)
		gs.BGR = C.uint8_t(255)
		gs.BGG = C.uint8_t(255)
		gs.BGB = C.uint8_t(255)
	}

	argc, argv, handle := cefClient.fillMainArgs()
	var args **C.char
	if argv != nil {
		args = &argv[0]
	}

	C.init_gui(cefClient.appHandler, cefClient.cefSettings, gs, cFuncArray, C.int(funcArraySize), C.int(argc), args, handle)
}

func (cefClient *CEF) initializeSettings(settings Settings) {
	cefClient.logger.Println("Initialize Settings")

	// Initialize cef_settings_t structure.
	cefClient.cefSettings = (*C.struct__cef_settings_t)(C.calloc(1, C.sizeof_struct__cef_settings_t))
	cefClient.cefSettings.size = C.sizeof_struct__cef_settings_t

	// cache_path
	// ----------
	if settings.CachePath != "" {
		cefClient.logger.Println("CachePath=", settings.CachePath)
		var cachePath *C.char = C.CString(settings.CachePath)
		defer C.free(unsafe.Pointer(cachePath))
		C.cef_string_from_utf8(cachePath, C.strlen(cachePath), &cefClient.cefSettings.cache_path)
	}

	// log_severity
	// ------------
	cefClient.cefSettings.log_severity = settings.LogSeverity

	// log_file
	// --------
	if settings.LogFile != "" {
		cefClient.logger.Println("LogFile=", settings.LogFile)
		var logFile *C.char = C.CString(settings.LogFile)
		defer C.free(unsafe.Pointer(logFile))
		C.cef_string_from_utf8(logFile, C.strlen(logFile), &cefClient.cefSettings.log_file)
	}

	// resources_dir_path
	// ------------------
	if settings.ResourcesDirPath == "" && runtime.GOOS != "darwin" {
		// Setting this path is required for the tests to run fine.
		cwd, _ := os.Getwd()
		settings.ResourcesDirPath = cwd
	}
	if settings.ResourcesDirPath != "" {
		cefClient.logger.Println("ResourcesDirPath=", settings.ResourcesDirPath)
	}
	var resourcesDirPath *C.char = C.CString(settings.ResourcesDirPath)
	defer C.free(unsafe.Pointer(resourcesDirPath))
	C.cef_string_from_utf8(resourcesDirPath, C.strlen(resourcesDirPath), &cefClient.cefSettings.resources_dir_path)

	// locales_dir_path
	// ----------------
	if settings.LocalesDirPath == "" && runtime.GOOS != "darwin" {
		// Setting this path is required for the tests to run fine.
		cwd, _ := os.Getwd()
		settings.LocalesDirPath = cwd + "/locales"
	}
	if settings.LocalesDirPath != "" {
		cefClient.logger.Println("LocalesDirPath=", settings.LocalesDirPath)
	}
	var localesDirPath *C.char = C.CString(settings.LocalesDirPath)
	defer C.free(unsafe.Pointer(localesDirPath))
	C.cef_string_from_utf8(localesDirPath, C.strlen(localesDirPath), &cefClient.cefSettings.locales_dir_path)

	// user_agent
	// --------
	if settings.UserAgent != "" {
		var userAgent *C.char = C.CString(settings.UserAgent)
		defer C.free(unsafe.Pointer(userAgent))
		C.cef_string_from_utf8(userAgent, C.strlen(userAgent), &cefClient.cefSettings.user_agent)
		cefClient.logger.Println("UserAgent=", settings.UserAgent)
	}

	// remote_debugging_port
	// --------
	cefClient.cefSettings.remote_debugging_port = C.int(settings.RemoteDebuggingPort)
	cefClient.logger.Println("RemoteDebuggingPort=", settings.RemoteDebuggingPort)

	// IgnoreCertificateErrors
	// --------
	cefClient.cefSettings.ignore_certificate_errors = cefClient.boolToCInt(settings.IgnoreCertificateErrors)
	cefClient.logger.Println("IgnoreCertificateErrors=", settings.IgnoreCertificateErrors)

	// CommandLineArgsDisabled
	// --------
	cefClient.cefSettings.command_line_args_disabled = cefClient.boolToCInt(settings.CommandLineArgsDisabled)
	cefClient.logger.Println("CommandLineArgsDisabled=", settings.CommandLineArgsDisabled)

	if cefClient.guiSettings.SubExecutable {
		// browser_subprocess_path
		//--------
		cwd, _ := os.Getwd()
		cwd = cwd + "/helper"
		if runtime.GOOS == "windows" {
			cwd = cwd + ".exe"
		}
		var cwdC *C.char = C.CString(cwd)
		defer C.free(unsafe.Pointer(cwdC))
		C.cef_string_from_utf8(cwdC, C.strlen(cwdC), &cefClient.cefSettings.browser_subprocess_path)
		cefClient.logger.Println("SubprocessPath=", cwd)
	}

	// no_sandbox
	// ----------
	cefClient.cefSettings.no_sandbox = C.int(1)
}

func (cefClient *CEF) Eval(js string) {
	cJS := C.CString(js)
	defer C.free(unsafe.Pointer(cJS))
	C.eval_js(cJS)
}

func (cefClient *CEF) Run() {
	C.run()
}
