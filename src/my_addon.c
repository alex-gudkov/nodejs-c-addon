#include <assert.h>
#include <node_api.h>

static napi_value method(napi_env env, napi_callback_info info)
{
    napi_status status;
    napi_value world;

    status = napi_create_string_utf8(env, "world", 5, &world);

    assert(status == napi_ok);

    return world;
}

static napi_value Add(napi_env env, napi_callback_info info)
{
    napi_status status;

    size_t argc = 2;
    napi_value args[2];
    status = napi_get_cb_info(env, info, &argc, args, NULL, NULL);
    assert(status == napi_ok);

    if (argc < 2)
    {
        napi_throw_type_error(env, NULL, "Wrong number of arguments");
        return NULL;
    }

    napi_valuetype valuetype0;
    status = napi_typeof(env, args[0], &valuetype0);
    assert(status == napi_ok);

    napi_valuetype valuetype1;
    status = napi_typeof(env, args[1], &valuetype1);
    assert(status == napi_ok);

    if (valuetype0 != napi_number || valuetype1 != napi_number)
    {
        napi_throw_type_error(env, NULL, "Wrong arguments");
        return NULL;
    }

    double value0;
    status = napi_get_value_double(env, args[0], &value0);
    assert(status == napi_ok);

    double value1;
    status = napi_get_value_double(env, args[1], &value1);
    assert(status == napi_ok);

    napi_value sum;
    status = napi_create_double(env, value0 + value1, &sum);
    assert(status == napi_ok);

    return sum;
}

#define DECLARE_NAPI_METHOD(name, func)         \
    {                                           \
        name, 0, func, 0, 0, 0, napi_default, 0 \
    }

static napi_value init(napi_env env, napi_value exports)
{
    napi_status status;
    
    napi_property_descriptor desc = DECLARE_NAPI_METHOD("hello", method);
    status = napi_define_properties(env, exports, 1, &desc);
    assert(status == napi_ok);

    napi_property_descriptor addDescriptor = DECLARE_NAPI_METHOD("add", Add);
    status = napi_define_properties(env, exports, 1, &addDescriptor);
    assert(status == napi_ok);

    return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, init)