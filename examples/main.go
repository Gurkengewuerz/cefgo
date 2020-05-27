package main

import (
	"github.com/gurkengewuerz/cefgo/cef"
	"image/color"
	"log"
	"os"
)

var client *cef.CEF

func main() {
	wd, _ := os.Getwd()

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
		WindowAppIcon: wd + "/icon.png",
		BackgroundColor: color.RGBA{
			R: 51,
			G: 51,
			B: 51,
			A: 255,
		},
		Settings: cef.Settings{},
		BindFunc: make(map[string]interface{}),
	}

	cefSettings := cef.Settings{}
	cefSettings.RemoteDebuggingPort = 6696
	cefSettings.LogSeverity = cef.LOGSEVERITY_WARNING
	cefSettings.CommandLineArgsDisabled = false

	guiSettings.Settings = cefSettings

	guiSettings.BindFunc["test"] = func() string {
		log.Println("test()")
		client.Eval("console.log('Test aus Funktion');")
		return "Test aus Funktion!"
	}

	guiSettings.BindFunc["maximize"] = func() {
		client.WindowMaximize()
	}

	guiSettings.BindFunc["minimize"] = func() {
		client.WindowMinimize()
	}

	guiSettings.BindFunc["alwaysOnTop"] = func(onTop bool) {
		client.WindowSetAlwaysOnTop(onTop)
	}

	guiSettings.BindFunc["setFullscreen"] = func(fs bool) {
		client.WindowFullscreen(fs)
	}

	guiSettings.BindFunc["setTitle"] = func(title string) {
		client.WindowSetTitle(title)
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
