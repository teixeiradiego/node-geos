#include "geometryfactory.hpp"

GeometryFactory::GeometryFactory() {
	_factory = geos::geom::GeometryFactory::create();
}

GeometryFactory::GeometryFactory(const geos::geom::PrecisionModel *pm) {
	_factory = geos::geom::GeometryFactory::create(pm);
}

GeometryFactory::GeometryFactory(const geos::geom::PrecisionModel *pm, int newSRID) {
	_factory = geos::geom::GeometryFactory::create(pm, newSRID);
}

GeometryFactory::~GeometryFactory() {}

Nan::Persistent<Function> GeometryFactory::constructor;

NAN_MODULE_INIT(GeometryFactory::Initialize) {

	Isolate* isolate = Isolate::GetCurrent();
	HandleScope scope(isolate);

	Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(GeometryFactory::New);
	tpl->InstanceTemplate()->SetInternalFieldCount(1);
	tpl->SetClassName(Nan::New("GeometryFactory").ToLocalChecked());

	Nan::SetPrototypeMethod(tpl, "getPrecisionModel", GetPrecisionModel);
	Nan::SetPrototypeMethod(tpl, "getSRID", GetSRID);
	Nan::SetPrototypeMethod(tpl, "destroy", Destroy);

	constructor.Reset(tpl->GetFunction());

	target->Set(Nan::New("GeometryFactory").ToLocalChecked(), tpl->GetFunction());
}

NAN_METHOD(GeometryFactory::New) {

	GeometryFactory* factory;

	if (info.Length() == 0) {
		factory = new GeometryFactory();
	} else if (info.Length() == 1) {
		PrecisionModel* model = Nan::ObjectWrap::Unwrap<PrecisionModel>(info[0]->ToObject());
		factory = new GeometryFactory(model->_model);
	} else {
		PrecisionModel* model = Nan::ObjectWrap::Unwrap<PrecisionModel>(info[0]->ToObject());
		int newSRID = Nan::To<int>(info[1]).FromJust();
		factory = new GeometryFactory(model->_model, newSRID);
	}

	factory->Wrap(info.This());
	info.GetReturnValue().Set(info.This());

}

NAN_METHOD(GeometryFactory::GetSRID) {

	GeometryFactory *factory = Nan::ObjectWrap::Unwrap<GeometryFactory>(info.Holder());
	info.GetReturnValue().Set(Nan::New<Integer>(factory->_factory.get()->getSRID()));

}

NAN_METHOD(GeometryFactory::GetPrecisionModel) {

	GeometryFactory * factory = Nan::ObjectWrap::Unwrap<GeometryFactory>(info.Holder());
	info.GetReturnValue().Set(PrecisionModel::New(factory->_factory.get()->getPrecisionModel()));

}

NAN_METHOD(GeometryFactory::Destroy) {

  GeometryFactory *factory = Nan::ObjectWrap::Unwrap<GeometryFactory>(info.Holder());
  Geometry *geom = Nan::ObjectWrap::Unwrap<Geometry>(info[0]->ToObject());
  factory->_factory->destroyGeometry(geom->_instance);
  info.GetReturnValue().Set(Nan::Undefined());

}
