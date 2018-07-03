#define NODE_GEOS_UNARY_PREDICATE(cppclass, cppmethod, geosmethod)                                \
    typedef struct {                                                                    \
        cppclass *geom;                                                                 \
        Persistent<Function> cb;                                                        \
        bool result;                                                                    \
    } geosmethod##_baton_t;                                                             \
                                                                                        \
    void cppclass::cppmethod##Async(uv_work_t *req) {                                    \
        geosmethod##_baton_t *closure = static_cast<geosmethod##_baton_t *>(req->data); \
        closure->result = closure->geom->_instance->geosmethod();                           \
    }                                                                                   \
                                                                                        \
    void cppclass::cppmethod##AsyncComplete(uv_work_t *req, int status) {                   \
        Isolate* isolate = Isolate::GetCurrent();                                       \
        HandleScope scope(isolate);                                                     \
                                                                                        \
        assert(status == 0);                                                            \
        geosmethod##_baton_t *closure = static_cast<geosmethod##_baton_t *>(req->data); \
        Local<Value> argv[2] = { Null(isolate),                                         \
            closure->result ? True(isolate) : False(isolate)                            \
        };                                                                              \
        TryCatch tryCatch(isolate);                                                     \
        Local<Function> local_callback = Local<Function>::New(isolate, closure->cb);    \
        local_callback->Call(isolate->GetCurrentContext()->Global(), 2, argv);          \
                                                                                        \
        if(tryCatch.HasCaught()) {                                                      \
            FatalException(isolate, tryCatch);                                          \
        }                                                                               \
                                                                                        \
        closure->cb.Reset();                                                            \
        closure->geom->Unref();                                                         \
                                                                                        \
        delete closure;                                                                 \
        delete req;                                                                     \
    }                                                                                   \
                                                                                        \
    void cppclass::cppmethod(const FunctionCallbackInfo<Value>& args)                   \
    {                                                                                   \
        Isolate* isolate = Isolate::GetCurrent();                                       \
        HandleScope scope(isolate);                                                     \
                                                                                        \
        cppclass *geom = ObjectWrap::Unwrap<cppclass>(args.This());                     \
        if (args.Length() == 1) {                                                       \
            geosmethod##_baton_t *closure = new geosmethod##_baton_t();                 \
            closure->geom = geom;                                                       \
            closure->cb.Reset(isolate, Local<Function>::Cast(args[0]));                 \
            uv_work_t *req = new uv_work_t;                                             \
            req->data = closure;                                                        \
            uv_queue_work(uv_default_loop(), req, cppmethod##Async, cppmethod##AsyncComplete);\
            geom->Ref();                                                                \
            args.GetReturnValue().Set(Undefined(isolate));                              \
        } else {                                                                        \
            try {                                                                       \
                args.GetReturnValue().Set(                                              \
                  geom->_instance->geosmethod() ? True(isolate) : False(isolate)            \
                );                                                                      \
                return;                                                                 \
            } catch(geos::util::GEOSException exception) {                              \
                isolate->ThrowException(                                                \
                  Exception::Error(String::NewFromUtf8(isolate, exception.what()))      \
                );                                                                      \
            }                                                                           \
            args.GetReturnValue().Set(Undefined(isolate));                              \
        }                                                                               \
    }


#define NODE_GEOS_BINARY_PREDICATE(cppclass, cppmethod, geosmethod)                               \
    typedef struct {                                                                    \
        cppclass *geom;                                                                 \
        cppclass *geom2;                                                                \
        Persistent<Function> cb;                                                        \
        bool result;                                                                    \
    } geosmethod##_baton_t;                                                             \
                                                                                        \
    void cppclass::cppmethod##Async(uv_work_t *req) {                                    \
        geosmethod##_baton_t *closure = static_cast<geosmethod##_baton_t *>(req->data); \
        closure->result = closure->geom->_instance->geosmethod(closure->geom2->_instance);      \
    }                                                                                   \
                                                                                        \
    void cppclass::cppmethod##AsyncComplete(uv_work_t *req, int status) {                   \
        Isolate* isolate = Isolate::GetCurrent();                                       \
        HandleScope scope(isolate);                                                     \
                                                                                        \
        assert(status == 0);                                                            \
        geosmethod##_baton_t *closure = static_cast<geosmethod##_baton_t *>(req->data); \
        Local<Value> argv[2] = {                                                        \
            Null(isolate), closure->result ? True(isolate) : False(isolate)             \
        };                                                                              \
        TryCatch tryCatch(isolate);                                                              \
        Local<Function> local_callback = Local<Function>::New(isolate, closure->cb);    \
        local_callback->Call(isolate->GetCurrentContext()->Global(), 2, argv);          \
                                                                                        \
        if(tryCatch.HasCaught()) {                                                      \
            FatalException(isolate, tryCatch);                                          \
        }                                                                               \
                                                                                        \
        closure->cb.Reset();                                                            \
        closure->geom->Unref();                                                         \
        closure->geom2->_unref();                                                       \
                                                                                        \
        delete closure;                                                                 \
        delete req;                                                                     \
    }                                                                                   \
                                                                                        \
    void cppclass::cppmethod(const FunctionCallbackInfo<Value>& args)                   \
    {                                                                                   \
        Isolate* isolate = Isolate::GetCurrent();                                       \
        HandleScope scope(isolate);                                                     \
        cppclass *geom = ObjectWrap::Unwrap<cppclass>(args.This());                     \
        cppclass *geom2 = ObjectWrap::Unwrap<cppclass>(args[0]->ToObject());            \
        Local<Function> f = Local<Function>::Cast(args[1]);                             \
        if (args.Length() == 2) {                                                       \
            geosmethod##_baton_t *closure = new geosmethod##_baton_t();                 \
            closure->geom = geom;                                                       \
            closure->geom2 = geom2;                                                     \
            closure->cb.Reset(isolate, Persistent<Function>(isolate, f));               \
            uv_work_t *req = new uv_work_t;                                             \
            req->data = closure;                                                        \
            uv_queue_work(uv_default_loop(), req, cppmethod##Async, cppmethod##AsyncComplete);\
            geom->Ref();                                                                \
            geom2->_ref();                                                              \
            args.GetReturnValue().Set(Undefined(isolate));                              \
        } else {                                                                        \
            try {                                                                       \
                args.GetReturnValue().Set(                                              \
                  geom->_instance->geosmethod(geom2->_instance) ? True(isolate) : False(isolate)\
                );                                                                      \
                return;                                                                 \
            } catch(geos::util::GEOSException exception) {                              \
                isolate->ThrowException(                                                \
                  Exception::Error(String::NewFromUtf8(isolate, exception.what()))      \
                );                                                                      \
            }                                                                           \
            args.GetReturnValue().Set(Undefined(isolate));                              \
        }                                                                               \
    }

#define NODE_GEOS_UNARY_TOPOLOGIC_FUNCTION(cppclass, cppmethod, geosmethod)             \
    void cppclass::cppmethod(const FunctionCallbackInfo<Value>& args) {                 \
        Isolate* isolate = Isolate::GetCurrent();                                       \
        HandleScope scope(isolate);                                                     \
        cppclass *geom = ObjectWrap::Unwrap<cppclass>(args.This());                     \
        try {                                                                           \
            geos::geom::cppclass* result = geom->_instance->geosmethod();               \
            args.GetReturnValue().Set(cppclass::New(result));                           \
        } catch(geos::util::GEOSException exception) {                                  \
            isolate->ThrowException(                                                    \
                Exception::Error(String::NewFromUtf8(isolate, exception.what()))        \
            );                                                                          \
        }                                                                               \
    }

#define NODE_GEOS_BINARY_TOPOLOGIC_FUNCTION(cppclass, cppmethod, geosmethod)            \
    void cppclass::cppmethod(const FunctionCallbackInfo<Value>& args) {                 \
        Isolate* isolate = Isolate::GetCurrent();                                       \
        HandleScope scope(isolate);                                                     \
        cppclass *geom = ObjectWrap::Unwrap<cppclass>(args.This());                     \
        cppclass *geom2 = ObjectWrap::Unwrap<cppclass>(args[0]->ToObject());            \
        try {                                                                           \
            geos::geom::cppclass* result = geom->_instance->geosmethod(geom2->_instance);   \
            args.GetReturnValue().Set(cppclass::New(result));                           \
        } catch(geos::util::GEOSException exception) {                                  \
            isolate->ThrowException(                                                    \
                Exception::Error(String::NewFromUtf8(isolate, exception.what()))        \
            );                                                                          \
        }                                                                               \
    }

#define NODE_GEOS_DOUBLE_GETTER(cppclass, cppmethod, geosmethod)                        \
    void cppclass::cppmethod(const FunctionCallbackInfo<Value>& args) {                 \
        Isolate* isolate = Isolate::GetCurrent();                                       \
        HandleScope scope(isolate);                                                     \
        cppclass *geom = ObjectWrap::Unwrap<cppclass>(args.This());                     \
        try {                                                                           \
            args.GetReturnValue().Set(geom->_instance->geosmethod());                   \
        } catch(geos::util::GEOSException exception) {                                  \
            isolate->ThrowException(                                                    \
                Exception::Error(String::NewFromUtf8(isolate, exception.what()))        \
            );                                                                          \
        }                                                                               \
    }

#define NODE_GEOS_V8_FUNCTION(cppmethod) \
    static void cppmethod(const FunctionCallbackInfo<Value>& args); \
    static void cppmethod##Async(uv_work_t *req); \
    static void cppmethod##AsyncComplete(uv_work_t *req, int status);
