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
		IsFrameless:   true,
		IsMaximized:   false,
		IsFullscreen:  false,
		CanMaximize:   true,
		CanMinimize:   true,
		CanResize:     true,
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

	client = &cef.CEF{
		Logger:      log.New(os.Stdout, "[example app]", 0),
		GuiSettings: guiSettings,
	}

	client.Init()
	client.OpenWindow()
	//client.Eval("console.log(\"Example\");")
	client.Run()
}
