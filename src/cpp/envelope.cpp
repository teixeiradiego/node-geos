#include "envelope.hpp"

Envelope::Envelope() {
    _envelope = new geos::geom::Envelope();
}

Envelope::Envelope(const geos::geom::Envelope *envelope) {
    _envelope = envelope;
}

Envelope::Envelope(double x1, double x2, double y1, double y2) {
    _envelope = new geos::geom::Envelope(x1, x2, y1, y2);
}

Envelope::~Envelope() {}

Persistent<Function> Envelope::constructor;

void Envelope::Initialize(Handle<Object> target) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, Envelope::New);

    tpl->InstanceTemplate()->SetInternalFieldCount(1);
    tpl->SetClassName(String::NewFromUtf8(isolate, "Envelope"));

    NODE_SET_PROTOTYPE_METHOD(tpl, "toString", Envelope::ToString);

    NODE_SET_PROTOTYPE_METHOD(tpl, "getMaxY", Envelope::GetMaxY);
    NODE_SET_PROTOTYPE_METHOD(tpl, "getMaxX", Envelope::GetMaxX);
    NODE_SET_PROTOTYPE_METHOD(tpl, "getMinY", Envelope::GetMinY);
    NODE_SET_PROTOTYPE_METHOD(tpl, "getMinX", Envelope::GetMinX);

    NODE_SET_PROTOTYPE_METHOD(tpl, "intersects", Envelope::Intersects);

    constructor.Reset(isolate, tpl->GetFunction());

    target->Set(String::NewFromUtf8(isolate, "Envelope"), tpl->GetFunction());
}

Handle<Value> Envelope::New(const geos::geom::Envelope *envelope) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    Envelope *env = new Envelope(envelope);
    Handle<Value> ext = External::New(isolate, env);

    Local<Function> cons = Local<Function>::New(isolate, constructor);
    Handle<Object> obj = cons->NewInstance(1, &ext);

    env->Wrap(obj);
    return obj;
}

void Envelope::New(const FunctionCallbackInfo<Value>& args) {

    Envelope* envelope;

    if (args.Length() == 0) {
        envelope = new Envelope();
    } else if (args.Length() == 1) {
        Envelope* env = ObjectWrap::Unwrap<Envelope>(args[0]->ToObject());
        envelope = new Envelope(env->_envelope);
    } else {
        double maxY = args[0]->NumberValue();
        double maxX = args[1]->NumberValue();
        double minY = args[2]->NumberValue();
        double minX = args[3]->NumberValue();
        envelope = new Envelope(maxY, maxX, minY, minX);
    }

    envelope->Wrap(args.This());
    args.GetReturnValue().Set(args.This());

}

void Envelope::ToString(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    Envelope* envelope = ObjectWrap::Unwrap<Envelope>(args.This());

    args.GetReturnValue().Set(String::NewFromUtf8(isolate, envelope->_envelope->toString().c_str()));
}

NODE_GEOS_DOUBLE_GETTER(GetMaxY, getMaxY);
NODE_GEOS_DOUBLE_GETTER(GetMaxX, getMaxX);
NODE_GEOS_DOUBLE_GETTER(GetMinY, getMinY);
NODE_GEOS_DOUBLE_GETTER(GetMinX, getMinX);

// GEOS binary predicates
NODE_GEOS_BINARY_PREDICATE(Intersects, intersects);

