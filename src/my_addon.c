#include <assert.h>
#include <node_api.h>
#include <stdio.h>

#define DECLARE_NAPI_METHOD(name, func)         \
    {                                           \
        name, 0, func, 0, 0, 0, napi_default, 0 \
    }

static napi_value get_string(napi_env env, napi_callback_info info)
{
    napi_status status;
    napi_value value;

    status = napi_create_string_utf8(env, "Hello world!", 12, &value);
    assert(status == napi_ok);

    return value;
}

static napi_value sum_numbers(napi_env env, napi_callback_info info)
{
    napi_status status;

    size_t argc = 2;
    napi_value args[2];

    status = napi_get_cb_info(env, info, &argc, args, NULL, NULL);
    assert(status == napi_ok);

    if (argc != 2)
    {
        napi_throw_type_error(env, NULL, "Wrong number of arguments");

        return NULL;
    }

    napi_valuetype arg0_type;

    status = napi_typeof(env, args[0], &arg0_type);
    assert(status == napi_ok);

    napi_valuetype arg1_type;

    status = napi_typeof(env, args[1], &arg1_type);
    assert(status == napi_ok);

    if (arg0_type != napi_number || arg1_type != napi_number)
    {
        napi_throw_type_error(env, NULL, "Wrong arguments type");

        return NULL;
    }

    double arg0;

    status = napi_get_value_double(env, args[0], &arg0);
    assert(status == napi_ok);

    double arg1;

    status = napi_get_value_double(env, args[1], &arg1);
    assert(status == napi_ok);

    napi_value value;

    status = napi_create_double(env, arg0 + arg1, &value);
    assert(status == napi_ok);

    return value;
}

static napi_value run_callback(napi_env env, const napi_callback_info info)
{
    napi_status status;

    size_t argc = 1;
    napi_value args[1];
 
    status = napi_get_cb_info(env, info, &argc, args, NULL, NULL);
    assert(status == napi_ok);

    napi_value callback = args[0];

    napi_value argv[1];

    status = napi_create_string_utf8(env, "hello world", NAPI_AUTO_LENGTH, argv);
    assert(status == napi_ok);

    napi_value global;

    status = napi_get_global(env, &global);
    assert(status == napi_ok);

    napi_value result;

    status = napi_call_function(env, global, callback, 1, argv, &result);
    assert(status == napi_ok);

    return NULL;
}

static napi_value init(napi_env env, napi_value exports)
{
    napi_status status;

    napi_property_descriptor get_string_descriptor = DECLARE_NAPI_METHOD("getString", get_string);

    status = napi_define_properties(env, exports, 1, &get_string_descriptor);
    assert(status == napi_ok);

    napi_property_descriptor sum_numbers_descriptor = DECLARE_NAPI_METHOD("sumNumbers", sum_numbers);

    status = napi_define_properties(env, exports, 1, &sum_numbers_descriptor);
    assert(status == napi_ok);

    napi_property_descriptor run_callback_descriptor = DECLARE_NAPI_METHOD("runCallback", run_callback);

    status = napi_define_properties(env, exports, 1, &run_callback_descriptor);
    assert(status == napi_ok);

    return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, init)