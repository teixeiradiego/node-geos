#include "preparedgeometryfactory.hpp"

NAN_MODULE_INIT(PreparedGeometryFactory::Initialize) {

	Isolate* isolate = Isolate::GetCurrent();
	HandleScope scope(isolate);

	Local<FunctionTemplate> ctor = Nan::New<FunctionTemplate>();
	ctor->SetClassName(Nan::New("PreparedGeometryFactory").ToLocalChecked());
	ctor->InstanceTemplate()->SetInternalFieldCount(1);

	Nan::SetPrototypeMethod(ctor, "prepare", Prepare);
	Nan::SetPrototypeMethod(ctor, "destroy", Destroy);

	target->Set(Nan::New("PreparedGeometryFactory").ToLocalChecked(), ctor->GetFunction());
}

NAN_METHOD(PreparedGeometryFactory::Prepare) {

	Isolate* isolate = Isolate::GetCurrent();
	HandleScope scope(isolate);

	Geometry *geom = Nan::ObjectWrap::Unwrap<Geometry>(info[0]->ToObject());

	Handle<Value> result = PreparedGeometry::New(geos::geom::prep::PreparedGeometryFactory::prepare(geom->_instance));
	info.GetReturnValue().Set(result);

}

NAN_METHOD(PreparedGeometryFactory::Destroy) {

	Isolate* isolate = Isolate::GetCurrent();
	HandleScope scope(isolate);

	PreparedGeometry *geom = Nan::ObjectWrap::Unwrap<PreparedGeometry>(info[0]->ToObject());
	geos::geom::prep::PreparedGeometryFactory::destroy(geom->_instance);
	info.GetReturnValue().Set(Nan::Undefined());

}
