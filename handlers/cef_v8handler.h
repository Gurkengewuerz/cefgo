#pragma once

#include "include/internal/cef_string_types.h"

#include "handlers/cef_base.h"
#include "handlers/cef_life_span_handler.h"
#include "include/capi/cef_v8_capi.h"

extern cef_v8value_t* BindFunctionCallback(char*, size_t, cef_v8value_t*[]);

typedef struct _invocation_handler {
	cef_v8handler_t handler;
	atomic_int ref_count;
} invocation_handler;

IMPLEMENT_REFCOUNTING(invocation_handler)
GENERATE_CEF_BASE_INITIALIZER(invocation_handler)

static int CEF_CALLBACK executeHandler(struct _cef_v8handler_t* self,
    const cef_string_t* name, struct _cef_v8value_t* object,
    size_t argumentsCount, struct _cef_v8value_t* const* arguments,
    struct _cef_v8value_t** retval, cef_string_t* exception)
{
    cef_string_utf8_t out = {};
    cef_string_utf16_to_utf8(name->str, name->length, &out);

    cef_v8value_t *start[argumentsCount];
    for(int i = 0; i < argumentsCount; i++) {
         start[i] = arguments[i];
     }

    if(argumentsCount > 0) *retval = BindFunctionCallback(out.str, argumentsCount, &start[0]);
    else *retval = BindFunctionCallback(out.str, argumentsCount, start);

    cef_string_utf8_clear(&out);

    return 1;
}

void initialize_v8handler(invocation_handler *h)
{
    DEBUG_CALLBACK("initialize_v8handler\n");
    initialize_cef_base(h);
    cef_v8handler_t *handler = (cef_v8handler_t *)h;
    // callbacks
    handler->execute = executeHandler;
}
