package cef

/*
#cgo CFLAGS: -I./../
#include <stdio.h>
#include <stdlib.h>
#include "include/capi/cef_v8_capi.h"

int get_js_type(cef_v8value_t* arg) {
	if(arg->is_valid(arg)) {
		if (arg->is_undefined(arg)) return 0;
		if (arg->is_null(arg)) return 1;
		if (arg->is_bool(arg)) return 2;
		if (arg->is_int(arg)) return 3;
		if (arg->is_uint(arg)) return 4;
		if (arg->is_double(arg)) return 5;
		if (arg->is_string(arg)) return 6;
	}
    return -1;
}

int js_to_bool(cef_v8value_t* arg) {
	return arg->get_bool_value(arg);
}

int js_to_int(cef_v8value_t* arg) {
	return arg->get_int_value(arg);
}

uint32 js_to_uint(cef_v8value_t* arg) {
	return arg->get_uint_value(arg);
}

double js_to_double(cef_v8value_t* arg) {
	return arg->get_double_value(arg);
}

char* js_to_string(cef_v8value_t* arg) {
	cef_string_utf8_t out = {};
	cef_string_userfree_t strVal = arg->get_string_value(arg);
	cef_string_utf16_to_utf8(strVal->str, strVal->length, &out);
	cef_string_userfree_free(strVal);
	return out.str;
}
*/
import "C"
import (
	"encoding/json"
	"log"
	"unsafe"
)

func GetJSONString(argLength int, arguments *[]C.cef_v8value_t) string {
	if argLength == 0 {
		return "[]"
	}

	start := unsafe.Pointer(arguments)
	var x *C.cef_v8value_t
	size := unsafe.Sizeof(x)

	argList := make([]interface{}, argLength)

	for i := 0; i < argLength; i++ {
		addr := uintptr(start) + size * uintptr(i)
		arg := (**C.cef_v8value_t)(unsafe.Pointer(addr))
		switch int(C.get_js_type(*arg)) {
		case 0:
		case 1:
			// nil
			argList[i] = nil
		case 2:
			// boolean
			if int(C.js_to_bool(*arg)) == 1 {
				argList[i] = true
			} else {
				argList[i] = false
			}

		case 3:
			// int
			argList[i] = int(C.js_to_int(*arg))
		case 4:
			// uint
			argList[i] = int(C.js_to_uint(*arg))
		case 5:
			// double
			argList[i] = float64(C.js_to_double(*arg))
		case 6:
			// string
			argList[i] = C.GoString(C.js_to_string(*arg))
		default:
			log.Println("Unknown argument type")
		}
	}

	val, _  := json.Marshal(argList)
	return string(val)
}
