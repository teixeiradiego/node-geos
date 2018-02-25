#ifndef ENVELOPE_HPP
#define ENVELOPE_HPP

#include <uv.h>
#include <nan.h>
#include <geos/geom/Envelope.h>
#include <geos/util/GEOSException.h>
#include "binding.hpp"
#include "geojsonwriter.hpp"

class Envelope : public ObjectWrap {
 public:
    const geos::geom::Envelope* _instance;
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
