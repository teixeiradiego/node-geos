#include "envelope.hpp"

Envelope::Envelope() {
	_instance = new geos::geom::Envelope();
}

Envelope::Envelope(const geos::geom::Envelope *envelope) {
	_instance = envelope;
}

Envelope::Envelope(double x1, double x2, double y1, double y2) {
	_instance = new geos::geom::Envelope(x1, x2, y1, y2);
}

Envelope::~Envelope() {}

Nan::Persistent<Function> Envelope::constructor;

NAN_MODULE_INIT(Envelope::Initialize) {

	Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(Envelope::New);
	tpl->InstanceTemplate()->SetInternalFieldCount(1);
	tpl->SetClassName(Nan::New("Envelope").ToLocalChecked());

	Nan::SetPrototypeMethod(tpl, "toString", Envelope::ToString);

	Nan::SetPrototypeMethod(tpl, "getMaxY", Envelope::GetMaxY);
	Nan::SetPrototypeMethod(tpl, "getMaxX", Envelope::GetMaxX);
	Nan::SetPrototypeMethod(tpl, "getMinY", Envelope::GetMinY);
	Nan::SetPrototypeMethod(tpl, "getMinX", Envelope::GetMinX);

	Nan::SetPrototypeMethod(tpl, "intersects", Envelope::Intersects);
	Nan::SetPrototypeMethod(tpl, "intersectsAsync", Envelope::IntersectsAsync);

	constructor.Reset(tpl->GetFunction());

	target->Set(Nan::New("Envelope").ToLocalChecked(), tpl->GetFunction());
}

Handle<Value> Envelope::New(const geos::geom::Envelope *envelope) {

	Nan::HandleScope scope();

	Envelope *env = new Envelope(envelope);
	Handle<Value> ext = Nan::New<External>(env);

	Local<Function> cons = Nan::New<Function>(constructor);
	MaybeLocal<v8::Object> maybeInstance = Nan::NewInstance(cons, 1, &ext);
	Local<v8::Object> instance;

	if (maybeInstance.IsEmpty()) {

		Nan::ThrowError("Could not create new Envelope instance");

		return Nan::Undefined();

	} else {

		instance = maybeInstance.ToLocalChecked();

		env->Wrap(instance);

		return instance;

	}

}

NAN_METHOD(Envelope::New) {

	Envelope* envelope;

	if (info.Length() == 0) {
		envelope = new Envelope();
	} else if (info.Length() == 1) {
		Envelope* env = Nan::ObjectWrap::Unwrap<Envelope>(info[0]->ToObject());
		envelope = new Envelope(env->_instance);
	} else {
		double minX = info[0]->NumberValue();
		double maxX = info[1]->NumberValue();
		double minY = info[2]->NumberValue();
		double maxY = info[3]->NumberValue();
		envelope = new Envelope(minX, maxX, minY, maxY);
	}

	envelope->Wrap(info.This());
	info.GetReturnValue().Set(info.This());

}

NAN_METHOD(Envelope::ToString) {

	Envelope* envelope = Nan::ObjectWrap::Unwrap<Envelope>(info.Holder());

	info.GetReturnValue().Set(Nan::New(envelope->_instance->toString().c_str()).ToLocalChecked());

}

NODE_GEOS_DOUBLE_GETTER(Envelope, GetMaxY, getMaxY);
NODE_GEOS_DOUBLE_GETTER(Envelope, GetMaxX, getMaxX);
NODE_GEOS_DOUBLE_GETTER(Envelope, GetMinY, getMinY);
NODE_GEOS_DOUBLE_GETTER(Envelope, GetMinX, getMinX);

// GEOS binary predicates
NODE_GEOS_BINARY_PREDICATE(geos::geom, Envelope, geos::geom, Envelope, Intersects, intersects);
