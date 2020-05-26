package cef

/*
#cgo CFLAGS: -I./../

#include "include/internal/cef_types.h"
*/
import "C"

const (
	LOGSEVERITY_DEFAULT = C.LOGSEVERITY_DEFAULT
	LOGSEVERITY_VERBOSE = C.LOGSEVERITY_VERBOSE
	LOGSEVERITY_INFO    = C.LOGSEVERITY_INFO
	LOGSEVERITY_WARNING = C.LOGSEVERITY_WARNING
	LOGSEVERITY_ERROR   = C.LOGSEVERITY_ERROR
	LOGSEVERITY_FATAL   = C.LOGSEVERITY_FATAL
	LOGSEVERITY_DISABLE = C.LOGSEVERITY_DISABLE
)

type Settings struct {
	CachePath               string
	LogSeverity             int
	LogFile                 string
	ResourcesDirPath        string
	LocalesDirPath          string
	UserAgent               string
	RemoteDebuggingPort     int
	IgnoreCertificateErrors bool
	CommandLineArgsDisabled bool
}