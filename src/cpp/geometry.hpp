#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <uv.h>
#include <nan.h>
#include <geos/geom/Geometry.h>
#include <geos/util/GEOSException.h>
#include "utils.hpp"
#include "binding.hpp"
#include "geojsonwriter.hpp"
#include "envelope.hpp"

class Geometry : public ObjectWrap {
 public:
    geos::geom::Geometry *_instance;
    Geometry();
    Geometry(geos::geom::Geometry *geom);
    ~Geometry();
    static void Initialize(Handle<Object> target);
    static Handle<Value> New(geos::geom::Geometry* geometry);

    void _ref() { Ref(); };
    void _unref() { Unref(); };

 protected:
    static void New(const FunctionCallbackInfo<Value>& args);
    // GEOS unary predicates
    NODE_GEOS_V8_FUNCTION(IsSimple);
    NODE_GEOS_V8_FUNCTION(IsValid);
    NODE_GEOS_V8_FUNCTION(IsEmpty);
    NODE_GEOS_V8_FUNCTION(IsRectangle);

    // GEOS binary predicates
    NODE_GEOS_V8_FUNCTION(Disjoint);
    NODE_GEOS_V8_FUNCTION(Touches);
    NODE_GEOS_V8_FUNCTION(Intersects);
    NODE_GEOS_V8_FUNCTION(Crosses);
    NODE_GEOS_V8_FUNCTION(Within);
    NODE_GEOS_V8_FUNCTION(Contains);
    NODE_GEOS_V8_FUNCTION(Overlaps);
    NODE_GEOS_V8_FUNCTION(Equals);
    NODE_GEOS_V8_FUNCTION(Covers);
    NODE_GEOS_V8_FUNCTION(CoveredBy);

    //static void EqualsExact(const FunctionCallbackInfo<Value>& args);
    static void IsWithinDistance(const FunctionCallbackInfo<Value>& args);

    // GEOS topologic function
    static void Intersection(const FunctionCallbackInfo<Value>& args);
    static void Union(const FunctionCallbackInfo<Value>& args);
    static void Difference(const FunctionCallbackInfo<Value>& args);
    static void SymDifference(const FunctionCallbackInfo<Value>& args);

    static void GetBoundary(const FunctionCallbackInfo<Value>& args);
    static void GetEnvelope(const FunctionCallbackInfo<Value>& args);
    static void GetEnvelopeInternal(const FunctionCallbackInfo<Value>& args);
    static void GetCentroid(const FunctionCallbackInfo<Value>& args);
    static void ConvexHull(const FunctionCallbackInfo<Value>& args);

    static void Buffer(const FunctionCallbackInfo<Value>& args);

    static void Distance(const FunctionCallbackInfo<Value>& args);

    static void GetArea(const FunctionCallbackInfo<Value>& args);
    static void GetLength(const FunctionCallbackInfo<Value>& args);

    static void GetSRID(const FunctionCallbackInfo<Value>& args);
    static void SetSRID(const FunctionCallbackInfo<Value>& args);

    static void GetGeometryType(const FunctionCallbackInfo<Value>& args);

    static void ToJSON(const FunctionCallbackInfo<Value>& args);

 private:
    static Persistent<Function> constructor;
    static void ToString(const FunctionCallbackInfo<Value>& args);

};
#endif
