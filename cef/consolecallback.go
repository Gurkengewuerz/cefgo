package cef

/*
#cgo CFLAGS: -I./../
#include <string.h>
#include <stdlib.h>

*/
import "C"

type SecurityLogLevel int

const (
	///
	// Default logging (currently INFO logging).
	///
	LEVEL_DEFAULT SecurityLogLevel = 0

	///
	// DEBUG logging.
	///
	LEVEL_DEBUG SecurityLogLevel = 1

	///
	// INFO logging.
	///
	LEVEL_INFO SecurityLogLevel = 2

	///
	// WARNING logging.
	///
	LEVEL_WARNING SecurityLogLevel = 3

	///
	// ERROR logging.
	///
	LEVEL_ERROR SecurityLogLevel = 4

	///
	// FATAL logging.
	///
	LEVEL_FATAL SecurityLogLevel = 5

	///
	// Disable logging to file for all messages, and to stderr for messages with
	// severity less than FATAL.
	///
	LEVEL_DISABLE SecurityLogLevel = 99
)

type consoleMessageCallbackI func(SecurityLogLevel, string, int, string)

//export ConsoleMessage
func ConsoleMessage(level C.int, source *C.char, line C.int, message *C.char) {
	sourceFile := string(C.GoString(source))
	messageString := string(C.GoString(message))

	ConsoleMessageCallback(SecurityLogLevel(level), sourceFile, int(line), messageString)
}
