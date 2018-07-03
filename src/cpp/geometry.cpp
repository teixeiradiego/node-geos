#include "geometry.hpp"

Geometry::Geometry() {}

Geometry::Geometry(geos::geom::Geometry *geom) : ObjectWrap() {
    _instance = geom;
}

Geometry::~Geometry() {}

Persistent<Function> Geometry::constructor;

void Geometry::Initialize(Handle<Object> target) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, Geometry::New);

    tpl->InstanceTemplate()->SetInternalFieldCount(1);
    tpl->SetClassName(String::NewFromUtf8(isolate, "Geometry"));

    NODE_SET_PROTOTYPE_METHOD(tpl, "toString", Geometry::ToString);
    //GEOS unary predicates
    NODE_SET_PROTOTYPE_METHOD(tpl, "isSimple", Geometry::IsSimple);
    NODE_SET_PROTOTYPE_METHOD(tpl, "isValid", Geometry::IsValid);
    NODE_SET_PROTOTYPE_METHOD(tpl, "isEmpty", Geometry::IsEmpty);
    NODE_SET_PROTOTYPE_METHOD(tpl, "isRectangle", Geometry::IsRectangle);
    //GEOS binary predicates
    //TODO maybe define a macro for this too
    NODE_SET_PROTOTYPE_METHOD(tpl, "disjoint", Geometry::Disjoint);
    NODE_SET_PROTOTYPE_METHOD(tpl, "touches", Geometry::Touches);
    NODE_SET_PROTOTYPE_METHOD(tpl, "intersects", Geometry::Intersects);
    NODE_SET_PROTOTYPE_METHOD(tpl, "crosses", Geometry::Crosses);
    NODE_SET_PROTOTYPE_METHOD(tpl, "within", Geometry::Within);
    NODE_SET_PROTOTYPE_METHOD(tpl, "contains", Geometry::Contains);
    NODE_SET_PROTOTYPE_METHOD(tpl, "overlaps", Geometry::Overlaps);
    NODE_SET_PROTOTYPE_METHOD(tpl, "equals", Geometry::Equals);
    NODE_SET_PROTOTYPE_METHOD(tpl, "covers", Geometry::Covers);
    NODE_SET_PROTOTYPE_METHOD(tpl, "coveredBy", Geometry::CoveredBy);

    NODE_SET_PROTOTYPE_METHOD(tpl, "isWithinDistance", Geometry::IsWithinDistance);

    //GEOS binary topologic functions
    NODE_SET_PROTOTYPE_METHOD(tpl, "intersection", Geometry::Intersection);
    NODE_SET_PROTOTYPE_METHOD(tpl, "union", Geometry::Union);
    NODE_SET_PROTOTYPE_METHOD(tpl, "difference", Geometry::Difference);
    NODE_SET_PROTOTYPE_METHOD(tpl, "symDifference", Geometry::SymDifference);

    //GEOS unary topologic functions
    NODE_SET_PROTOTYPE_METHOD(tpl, "getEnvelope", Geometry::GetEnvelope);
    NODE_SET_PROTOTYPE_METHOD(tpl, "getCentroid", Geometry::GetCentroid);
    NODE_SET_PROTOTYPE_METHOD(tpl, "getBoundary", Geometry::GetBoundary);
    NODE_SET_PROTOTYPE_METHOD(tpl, "convexHull", Geometry::ConvexHull);

    NODE_SET_PROTOTYPE_METHOD(tpl, "buffer", Geometry::Buffer);
    NODE_SET_PROTOTYPE_METHOD(tpl, "getEnvelopeInternal", Geometry::GetEnvelopeInternal);

    NODE_SET_PROTOTYPE_METHOD(tpl, "distance", Geometry::Distance);

    NODE_SET_PROTOTYPE_METHOD(tpl, "getArea", Geometry::GetArea);
    NODE_SET_PROTOTYPE_METHOD(tpl, "getLength", Geometry::GetLength);
    NODE_SET_PROTOTYPE_METHOD(tpl, "getSRID", Geometry::GetSRID);
    NODE_SET_PROTOTYPE_METHOD(tpl, "setSRID", Geometry::SetSRID);

    NODE_SET_PROTOTYPE_METHOD(tpl, "getGeometryType", Geometry::GetGeometryType);

    NODE_SET_PROTOTYPE_METHOD(tpl, "toJSON", Geometry::ToJSON);

    constructor.Reset(isolate, tpl->GetFunction());

    target->Set(String::NewFromUtf8(isolate, "Geometry"), tpl->GetFunction());
}

Handle<Value> Geometry::New(geos::geom::Geometry *geometry) {

    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    Geometry *geom = new Geometry(geometry);
    Handle<Value> ext = External::New(isolate, geom);

    Local<Function> cons = Local<Function>::New(isolate, constructor);
    MaybeLocal<v8::Object> maybeInstance = Nan::NewInstance(cons, 1, &ext);
    Local<v8::Object> instance;

    if (maybeInstance.IsEmpty()) {

        Nan::ThrowError("Could not create new Geometry instance");

        return Undefined(isolate);

    } else {

        instance = maybeInstance.ToLocalChecked();

        geom->Wrap(instance);

        return instance;

    }

}

void Geometry::New(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    Geometry *geom;
    geom = new Geometry();
    geom->Wrap(args.This());
    args.GetReturnValue().Set(args.This());
}

void Geometry::ToString(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    Geometry* geom = ObjectWrap::Unwrap<Geometry>(args.This());

    try {

        args.GetReturnValue().Set(String::NewFromUtf8(isolate, geom->_instance->toString().data()));

    } catch(geos::util::GEOSException exception) {
        isolate->ThrowException(
            Exception::Error(String::NewFromUtf8(isolate, exception.what()))
        );
    }

}

void Geometry::GetGeometryType(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    Geometry* geom = ObjectWrap::Unwrap<Geometry>(args.This());

    try {

        args.GetReturnValue().Set(String::NewFromUtf8(isolate, geom->_instance->getGeometryType().data()));

    } catch(geos::util::GEOSException exception) {
        isolate->ThrowException(
            Exception::Error(String::NewFromUtf8(isolate, exception.what()))
        );
    }

}

void Geometry::Distance(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    Geometry* geom = ObjectWrap::Unwrap<Geometry>(args.This());
    Geometry* geom2 = ObjectWrap::Unwrap<Geometry>(args[0]->ToObject());

    try {

        args.GetReturnValue().Set(Number::New(isolate, geom->_instance->distance(geom2->_instance)));

    } catch(geos::util::GEOSException exception) {
        isolate->ThrowException(
            Exception::Error(String::NewFromUtf8(isolate, exception.what()))
        );
    }

}

void Geometry::IsWithinDistance(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    Geometry* geom = ObjectWrap::Unwrap<Geometry>(args.This());
    Geometry* geom2 = ObjectWrap::Unwrap<Geometry>(args[0]->ToObject());
    double distance = args[0]->NumberValue();

    try {

        args.GetReturnValue().Set(
            geom->_instance->isWithinDistance(geom2->_instance, distance) ? True(isolate) : False(isolate)
        );

    } catch(geos::util::GEOSException exception) {
        isolate->ThrowException(
            Exception::Error(String::NewFromUtf8(isolate, exception.what()))
        );
    }

}

void Geometry::SetSRID(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    Geometry* geom = ObjectWrap::Unwrap<Geometry>(args.This());

    try {

        geom->_instance->setSRID(args[0]->IntegerValue());

        args.GetReturnValue().Set(Undefined(isolate));

    } catch(geos::util::GEOSException exception) {
        isolate->ThrowException(
            Exception::Error(String::NewFromUtf8(isolate, exception.what()))
        );
    }

}

void Geometry::ToJSON(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    Geometry* geom = ObjectWrap::Unwrap<Geometry>(args.This());
    GeoJSONWriter writer;
    if (args.Length() >= 1 && args[0]->IsInt32()) {
        writer.setRoundingPrecision(args[0]->Int32Value());
    }
    if (args.Length() >= 2 && args[1]->IsBoolean()) {
        writer.setBbox(args[1]->BooleanValue());
    }
    Handle<Value> json = writer.write(geom->_instance);
    args.GetReturnValue().Set(json);
}

void Geometry::Buffer(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    double distance;
    int quadrantSegments;
    Handle<Value> result;

    Geometry* geom = ObjectWrap::Unwrap<Geometry>(args.This());
    distance = args[0]->NumberValue();

    try {

        if (args.Length() == 1) {
            result = Geometry::New(geom->_instance->buffer(distance));
        } else if (args.Length() == 2) {
            quadrantSegments = args[1]->IntegerValue();
            result = Geometry::New(geom->_instance->buffer(distance, quadrantSegments));
        } else {
            quadrantSegments = args[1]->IntegerValue();
            int endCapStyle = args[2]->IntegerValue();
            result = Geometry::New(geom->_instance->buffer(distance, quadrantSegments, endCapStyle));
        }

        args.GetReturnValue().Set(result);

    } catch(geos::util::GEOSException exception) {
        isolate->ThrowException(
            Exception::Error(String::NewFromUtf8(isolate, exception.what()))
        );
    }

}

void Geometry::GetEnvelopeInternal(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    Geometry* geom = ObjectWrap::Unwrap<Geometry>(args.This());

    try {

        Handle<Value> result = Envelope::New(geom->_instance->getEnvelopeInternal());

        args.GetReturnValue().Set(result);

    } catch(geos::util::GEOSException exception) {
        isolate->ThrowException(
            Exception::Error(String::NewFromUtf8(isolate, exception.what()))
        );
    }

}

//GEOS unary predicates
NODE_GEOS_UNARY_PREDICATE(Geometry, IsSimple, isSimple);
NODE_GEOS_UNARY_PREDICATE(Geometry, IsValid, isValid);
NODE_GEOS_UNARY_PREDICATE(Geometry, IsEmpty, isEmpty);
NODE_GEOS_UNARY_PREDICATE(Geometry, IsRectangle, isRectangle);

// GEOS binary predicates
NODE_GEOS_BINARY_PREDICATE(Geometry, Disjoint, disjoint);
NODE_GEOS_BINARY_PREDICATE(Geometry, Touches, touches);
NODE_GEOS_BINARY_PREDICATE(Geometry, Intersects, intersects);
NODE_GEOS_BINARY_PREDICATE(Geometry, Crosses, crosses);
NODE_GEOS_BINARY_PREDICATE(Geometry, Within, within);
NODE_GEOS_BINARY_PREDICATE(Geometry, Contains, contains);
NODE_GEOS_BINARY_PREDICATE(Geometry, Overlaps, overlaps);
NODE_GEOS_BINARY_PREDICATE(Geometry, Equals, equals);
NODE_GEOS_BINARY_PREDICATE(Geometry, Covers, covers);
NODE_GEOS_BINARY_PREDICATE(Geometry, CoveredBy, coveredBy);

// GEOS unary topologic functions
NODE_GEOS_UNARY_TOPOLOGIC_FUNCTION(Geometry, GetEnvelope, getEnvelope);
NODE_GEOS_UNARY_TOPOLOGIC_FUNCTION(Geometry, GetCentroid, getCentroid);
NODE_GEOS_UNARY_TOPOLOGIC_FUNCTION(Geometry, GetBoundary, getBoundary);
NODE_GEOS_UNARY_TOPOLOGIC_FUNCTION(Geometry, ConvexHull, convexHull);

// GEOS binary topologic functions
NODE_GEOS_BINARY_TOPOLOGIC_FUNCTION(Geometry, Intersection, intersection);
NODE_GEOS_BINARY_TOPOLOGIC_FUNCTION(Geometry, Union, Union);
NODE_GEOS_BINARY_TOPOLOGIC_FUNCTION(Geometry, Difference, difference);
NODE_GEOS_BINARY_TOPOLOGIC_FUNCTION(Geometry, SymDifference, symDifference);

NODE_GEOS_DOUBLE_GETTER(Geometry, GetArea, getArea);
NODE_GEOS_DOUBLE_GETTER(Geometry, GetLength, getLength);
NODE_GEOS_DOUBLE_GETTER(Geometry, GetSRID, getSRID);
