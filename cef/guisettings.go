package cef

import "image/color"

type GuiSettings struct {
	WindowName      string
	StartURL        string
	IsFrameless     bool
	IsMaximized     bool
	IsFullscreen    bool
	CanMaximize     bool
	CanMinimize     bool
	CanResize       bool
	Height          int
	Width           int
	SubExecutable   bool
	WindowIcon      string
	WindowAppIcon   string
	BackgroundColor color.Color
	Settings        Settings
	BindFunc        map[string]interface{}
}
