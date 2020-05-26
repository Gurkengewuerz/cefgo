package cef

/*
#cgo CFLAGS: -I./../
#include "include/capi/cef_v8_capi.h"
*/
import "C"
import (
	"errors"
	"fmt"
	"log"
	"reflect"
	"unsafe"
)

//export BindFunctionCallback
func BindFunctionCallback(name *C.char, argumentCount C.size_t, arguments *[]C.cef_v8value_t) *C.cef_v8value_t {
	jsonStr := GetJSONString(int(argumentCount), arguments)

	funcName := C.GoString(name)
	f := _BindFunc[funcName]
	funcEr := errors.New("none")
	if res, err := f(jsonStr); err != nil {
		funcEr = err
	} else {
		v := reflect.ValueOf(res)

		switch v.Kind() {
		case reflect.Float32:
			return C.cef_v8value_create_double(C.double(res.(float32)))
		case reflect.Float64:
			return C.cef_v8value_create_double(C.double(res.(float64)))
		case reflect.String:
			var charStr *C.char = C.CString(res.(string))
			var cefStr C.cef_string_t
			defer C.free(unsafe.Pointer(charStr))
			C.cef_string_from_utf8(charStr, C.strlen(charStr), &cefStr)
			return C.cef_v8value_create_string(&cefStr)
		case reflect.Int:
			return C.cef_v8value_create_int(C.int32(res.(int)))
		case reflect.Bool:
			if res.(bool) {
				return C.cef_v8value_create_bool(C.int(1))
			}
			return C.cef_v8value_create_bool(C.int(0))
		default:
			return C.cef_v8value_create_null()
		}
	}
	log.Println(fmt.Sprintf("Error in function %s (): %s", string(funcName), funcEr.Error()))
	return C.cef_v8value_create_undefined()
}
