package cef

type GuiSettings struct {
	WindowName    string
	StartURL      string
	IsFrameless   bool
	IsMaximized   bool
	IsFullscreen  bool
	CanMaximize   bool
	CanMinimize   bool
	CanResize     bool
	Height        int
	Width         int
	WindowIcon    string
	WindowAppIcon string
	Settings      Settings
	BindFunc      map[string]interface{}
}
