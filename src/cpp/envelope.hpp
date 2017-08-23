#ifndef ENVELOPE_HPP
#define ENVELOPE_HPP

#include <uv.h>
#include <geos/geom/Envelope.h>
#include <geos/util/GEOSException.h>
#include "binding.hpp"
#include "geojsonwriter.hpp"

#define NODE_GEOS_DOUBLE_GETTER(cppmethod, geosmethod)                          \
void Envelope::cppmethod(const FunctionCallbackInfo<Value>& args) {\
    Envelope *envelope = ObjectWrap::Unwrap<Envelope>(args.This());             \
    args.GetReturnValue().Set(envelope->_envelope->geosmethod());                   \
}                                                                           \

#define NODE_GEOS_BINARY_PREDICATE(cppmethod, geosmethod)                               \
typedef struct {                                                                    \
    Envelope *envelope;                                                                 \
    Envelope *envelope2;                                                                \
    Persistent<Function> cb;                                                        \
    bool result;                                                                    \
} geosmethod##_baton_t;                                                             \
                                                                                    \
void Envelope::cppmethod##Async(uv_work_t *req) {                                    \
    geosmethod##_baton_t *closure = static_cast<geosmethod##_baton_t *>(req->data); \
    closure->result = closure->envelope->_envelope->geosmethod(closure->envelope2->_envelope);      \
}                                                                                   \
                                                                                    \
void Envelope::cppmethod##AsyncComplete(uv_work_t *req, int status) {                   \
    Isolate* isolate = Isolate::GetCurrent();                                       \
    HandleScope scope(isolate);                                                     \
                                                                                    \
    assert(status == 0);                                                            \
    geosmethod##_baton_t *closure = static_cast<geosmethod##_baton_t *>(req->data); \
    Local<Value> argv[2] = {                                                        \
        Null(isolate), closure->result ? True(isolate) : False(isolate)             \
    };                                                                              \
    TryCatch tryCatch;                                                              \
    Local<Function> local_callback = Local<Function>::New(isolate, closure->cb);    \
    local_callback->Call(isolate->GetCurrentContext()->Global(), 2, argv);          \
                                                                                    \
    if(tryCatch.HasCaught()) {                                                      \
        FatalException(isolate, tryCatch);                                          \
    }                                                                               \
                                                                                    \
    closure->cb.Reset();                                                            \
    closure->envelope->Unref();                                                         \
    closure->envelope2->_unref();                                                       \
                                                                                    \
    delete closure;                                                                 \
    delete req;                                                                     \
}                                                                                   \
                                                                                    \
void Envelope::cppmethod(const FunctionCallbackInfo<Value>& args)                   \
{                                                                                   \
    Isolate* isolate = Isolate::GetCurrent();                                       \
    HandleScope scope(isolate);                                                     \
    Envelope *envelope = ObjectWrap::Unwrap<Envelope>(args.This());                     \
    Envelope *envelope2 = ObjectWrap::Unwrap<Envelope>(args[0]->ToObject());            \
    Local<Function> f = Local<Function>::Cast(args[1]);                             \
    if (args.Length() == 2) {                                                       \
        geosmethod##_baton_t *closure = new geosmethod##_baton_t();                 \
        closure->envelope = envelope;                                                       \
        closure->envelope2 = envelope2;                                                     \
        closure->cb.Reset(isolate, Persistent<Function>(isolate, f));               \
        uv_work_t *req = new uv_work_t;                                             \
        req->data = closure;                                                        \
        uv_queue_work(uv_default_loop(), req, cppmethod##Async, cppmethod##AsyncComplete);\
        envelope->Ref();                                                                \
        envelope2->_ref();                                                              \
        args.GetReturnValue().Set(Undefined(isolate));                              \
    } else {                                                                        \
        try {                                                                       \
            args.GetReturnValue().Set(                                              \
              envelope->_envelope->geosmethod(envelope2->_envelope) ? True(isolate) : False(isolate)\
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

#define NODE_GEOS_V8_FUNCTION(cppmethod) \
    static void cppmethod(const FunctionCallbackInfo<Value>& args); \
    static void cppmethod##Async(uv_work_t *req); \
    static void cppmethod##AsyncComplete(uv_work_t *req, int status); \

class Envelope : public ObjectWrap {
 public:
    const geos::geom::Envelope* _envelope;
    Envelope();
    Envelope(double x1, double x2, double y1, double y2);
    Envelope(const geos::geom::Envelope *envelope);
    ~Envelope();
    static void Initialize(Handle<Object> target);
    static Handle<Value> New(const geos::geom::Envelope* envelope);

    void _ref() { Ref(); };
    void _unref() { Unref(); };

 protected:
    static void New(const FunctionCallbackInfo<Value>& args);

    static void GetMaxY(const FunctionCallbackInfo<Value>& args);
    static void GetMaxX(const FunctionCallbackInfo<Value>& args);
    static void GetMinY(const FunctionCallbackInfo<Value>& args);
    static void GetMinX(const FunctionCallbackInfo<Value>& args);

    NODE_GEOS_V8_FUNCTION(Intersects);

 private:
    static Persistent<Function> constructor;
    static void ToString(const FunctionCallbackInfo<Value>& args);

};
#endif
