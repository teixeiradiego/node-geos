#include "preparedgeometry.hpp"

PreparedGeometry::PreparedGeometry() {}

PreparedGeometry::PreparedGeometry(const geos::geom::prep::PreparedGeometry *preparedGeometry) : Nan::ObjectWrap() {
	_instance = preparedGeometry;
}

PreparedGeometry::~PreparedGeometry() {}

Nan::Persistent<Function> PreparedGeometry::constructor;

NAN_MODULE_INIT(PreparedGeometry::Initialize) {

	Isolate* isolate = Isolate::GetCurrent();
	HandleScope scope(isolate);

	Local<FunctionTemplate> ctor = Nan::New<FunctionTemplate>(PreparedGeometry::New);
	ctor->InstanceTemplate()->SetInternalFieldCount(1);
	ctor->SetClassName(Nan::New("PreparedGeometry").ToLocalChecked());

	Nan::SetPrototypeMethod(ctor, "getGeometry", PreparedGeometry::GetGeometry);

	Nan::SetPrototypeMethod(ctor, "contains", PreparedGeometry::Contains);
	Nan::SetPrototypeMethod(ctor, "containsProperly", PreparedGeometry::ContainsProperly);
	Nan::SetPrototypeMethod(ctor, "coveredBy", PreparedGeometry::CoveredBy);
	Nan::SetPrototypeMethod(ctor, "covers", PreparedGeometry::Covers);
	Nan::SetPrototypeMethod(ctor, "crosses", PreparedGeometry::Crosses);
	Nan::SetPrototypeMethod(ctor, "disjoint", PreparedGeometry::Disjoint);
	Nan::SetPrototypeMethod(ctor, "intersects", PreparedGeometry::Intersects);
	Nan::SetPrototypeMethod(ctor, "overlaps", PreparedGeometry::Overlaps);
	Nan::SetPrototypeMethod(ctor, "touches", PreparedGeometry::Touches);
	Nan::SetPrototypeMethod(ctor, "within", PreparedGeometry::Within);

	constructor.Reset(ctor->GetFunction());

	target->Set(Nan::New("PreparedGeometry").ToLocalChecked(), ctor->GetFunction());

}

NAN_METHOD(PreparedGeometry::New) {

	Isolate* isolate = Isolate::GetCurrent();
	HandleScope scope(isolate);

	PreparedGeometry *preparedGeometry;
	preparedGeometry = new PreparedGeometry();
	preparedGeometry->Wrap(info.This());
	info.GetReturnValue().Set(info.This());

}

Handle<Value> PreparedGeometry::New(const geos::geom::prep::PreparedGeometry* preparedGeometry) {

	Isolate* isolate = Isolate::GetCurrent();
	HandleScope scope(isolate);

	PreparedGeometry *geom = new PreparedGeometry(preparedGeometry);
	Handle<Value> ext = External::New(isolate, geom);

	Local<Function> cons = Local<Function>::New(isolate, constructor);
	MaybeLocal<v8::Object> maybeInstance = Nan::NewInstance(cons, 1, &ext);
	Local<v8::Object> instance;

	if (maybeInstance.IsEmpty()) {

		Nan::ThrowError("Could not create new PreparedGeometry instance");

		return Undefined(isolate);

	} else {

		instance = maybeInstance.ToLocalChecked();

		geom->Wrap(instance);

		return instance;

	}

}

// GEOS getters
NAN_METHOD(PreparedGeometry::GetGeometry) {

	PreparedGeometry *preparedgeometry = Nan::ObjectWrap::Unwrap<PreparedGeometry>(info.Holder());
	const geos::geom::Geometry & result = preparedgeometry->_instance->getGeometry();
	info.GetReturnValue().Set(Geometry::New(const_cast<geos::geom::Geometry*>(&result)));

}

// GEOS binary predicates
NODE_GEOS_BINARY_PREDICATE(geos::geom::prep, PreparedGeometry, geos::geom, Geometry, Contains, contains);
NODE_GEOS_BINARY_PREDICATE(geos::geom::prep, PreparedGeometry, geos::geom, Geometry, ContainsProperly, containsProperly);
NODE_GEOS_BINARY_PREDICATE(geos::geom::prep, PreparedGeometry, geos::geom, Geometry, CoveredBy, coveredBy);
NODE_GEOS_BINARY_PREDICATE(geos::geom::prep, PreparedGeometry, geos::geom, Geometry, Covers, covers);
NODE_GEOS_BINARY_PREDICATE(geos::geom::prep, PreparedGeometry, geos::geom, Geometry, Crosses, crosses);
NODE_GEOS_BINARY_PREDICATE(geos::geom::prep, PreparedGeometry, geos::geom, Geometry, Disjoint, disjoint);
NODE_GEOS_BINARY_PREDICATE(geos::geom::prep, PreparedGeometry, geos::geom, Geometry, Intersects, intersects);
NODE_GEOS_BINARY_PREDICATE(geos::geom::prep, PreparedGeometry, geos::geom, Geometry, Overlaps, overlaps);
NODE_GEOS_BINARY_PREDICATE(geos::geom::prep, PreparedGeometry, geos::geom, Geometry, Touches, touches);
NODE_GEOS_BINARY_PREDICATE(geos::geom::prep, PreparedGeometry, geos::geom, Geometry, Within, within);
