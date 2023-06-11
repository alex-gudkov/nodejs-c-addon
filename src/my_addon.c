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

    status = napi_create_string_utf8(env, "Hello world!", NAPI_AUTO_LENGTH, &value);
    assert(status == napi_ok);

    return value;
}

static napi_value sum_numbers(napi_env env, napi_callback_info info)
{
    napi_status status;

    size_t argc = 2;
    napi_value argv[2];

    status = napi_get_cb_info(env, info, &argc, argv, NULL, NULL);
    assert(status == napi_ok);

    if (argc != 2)
    {
        napi_throw_type_error(env, NULL, "Wrong number of arguments");

        return NULL;
    }

    napi_valuetype arg0_type;

    status = napi_typeof(env, argv[0], &arg0_type);
    assert(status == napi_ok);

    napi_valuetype arg1_type;

    status = napi_typeof(env, argv[1], &arg1_type);
    assert(status == napi_ok);

    if (arg0_type != napi_number || arg1_type != napi_number)
    {
        napi_throw_type_error(env, NULL, "Wrong arguments type");

        return NULL;
    }

    double arg0;

    status = napi_get_value_double(env, argv[0], &arg0);
    assert(status == napi_ok);

    double arg1;

    status = napi_get_value_double(env, argv[1], &arg1);
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
    napi_value argv[1];

    status = napi_get_cb_info(env, info, &argc, argv, NULL, NULL);
    assert(status == napi_ok);

    if (argc != 1)
    {
        napi_throw_type_error(env, NULL, "Wrong number of arguments");

        return NULL;
    }

    napi_valuetype arg0_type;

    status = napi_typeof(env, argv[0], &arg0_type);
    assert(status == napi_ok);

    if (arg0_type != napi_function)
    {
        napi_throw_type_error(env, NULL, "Wrong arguments type");

        return NULL;
    }

    napi_value callback = argv[0];

    const unsigned int callback_argc = 1U;
    napi_value callback_argv[callback_argc];

    status = napi_create_double(env, 12.34, callback_argv + 0);
    assert(status == napi_ok);

    napi_value global;

    status = napi_get_global(env, &global);
    assert(status == napi_ok);

    napi_value result;

    status = napi_call_function(env, global, callback, 1, callback_argv, &result);
    assert(status == napi_ok);

    return NULL;
}

static napi_value get_object(napi_env env, napi_callback_info info)
{
    napi_status status;
    napi_value object;

    status = napi_create_object(env, &object);
    assert(status == napi_ok);

    napi_value named_property_x;

    status = napi_create_int32(env, 1, &named_property_x);
    assert(status == napi_ok);

    status = napi_set_named_property(env, object, "x", named_property_x);
    assert(status == napi_ok);

    napi_value named_property_y;

    status = napi_create_int32(env, 2, &named_property_y);
    assert(status == napi_ok);

    status = napi_set_named_property(env, object, "y", named_property_y);
    assert(status == napi_ok);

    return object;
}

static napi_value get_array(napi_env env, napi_callback_info info)
{
    napi_status status;
    napi_value array;

    unsigned long length = 5;

    status = napi_create_array_with_length(env, length, &array);
    assert(status == napi_ok);

    for (unsigned long i = 0; i < length; ++i)
    {
        napi_value element;

        status = napi_create_int32(env, i + 1, &element);
        assert(status == napi_ok);

        status = napi_set_element(env, array, i, element);
        assert(status == napi_ok);
    }

    return array;
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

    napi_property_descriptor get_object_descriptor = DECLARE_NAPI_METHOD("getObject", get_object);

    status = napi_define_properties(env, exports, 1, &get_object_descriptor);
    assert(status == napi_ok);

    napi_property_descriptor get_array_descriptor = DECLARE_NAPI_METHOD("getArray", get_array);

    status = napi_define_properties(env, exports, 1, &get_array_descriptor);
    assert(status == napi_ok);

    return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, init)