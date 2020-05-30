package main

import (
	"github.com/gurkengewuerz/cefgo/cef"
	"os"
)

func main() {
	client := cef.New(cef.GuiSettings{}, nil)
	os.Exit(client.InitSubprocess())
}
