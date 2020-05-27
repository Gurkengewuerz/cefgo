package main

import (
	"github.com/gurkengewuerz/cefgo/cef"
	"log"
	"os"
)

var client *cef.CEF

func main() {
	guiSettings := cef.GuiSettings{
		WindowName:    "CEF Test",
		StartURL:      "https://app01.mc8051.de",
		IsFrameless:   false,
		IsMaximized:   false,
		IsFullscreen:  false,
		CanMaximize:   true,
		CanMinimize:   true,
		CanResize:     true,
		SubExecutable: false,
		Height:        700,
		Width:         450,
		WindowIcon:    "",
		WindowAppIcon: "",
		Settings:      cef.Settings{},
		BindFunc:      make(map[string]interface{}),
	}

	cefSettings := cef.Settings{}
	cefSettings.RemoteDebuggingPort = 6696
	cefSettings.LogSeverity = cef.LOGSEVERITY_VERBOSE
	cefSettings.CommandLineArgsDisabled = false

	guiSettings.Settings = cefSettings

	guiSettings.BindFunc["test"] = func() string {
		log.Println("test()")
		client.Eval("console.log('Test aus Funktion');")
		return "Test aus Funktion!"
	}

	client = cef.New(guiSettings, log.New(os.Stdout, "[example app]", 0))

	exitCode := client.InitSubprocess()
	if exitCode >= 0 {
		os.Exit(exitCode)
	}

	client.OpenWindow()
	//client.Eval("console.log(\"Example\");")
	client.Run()
}
