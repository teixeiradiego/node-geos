#include "precisionmodel.hpp"

PrecisionModel::PrecisionModel() {
	_model = new geos::geom::PrecisionModel();
}

PrecisionModel::PrecisionModel(double newScale) {
	_model = new geos::geom::PrecisionModel(newScale);
}

PrecisionModel::PrecisionModel(geos::geom::PrecisionModel::Type nModelType) {
	_model = new geos::geom::PrecisionModel(nModelType);
}
PrecisionModel::PrecisionModel(const geos::geom::PrecisionModel *model) {
	_model = (geos::geom::PrecisionModel*) model; //hacky
}

PrecisionModel::~PrecisionModel() {}

Nan::Persistent<Function> PrecisionModel::constructor;

NAN_MODULE_INIT(PrecisionModel::Initialize) {

	Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(PrecisionModel::New);
	tpl->InstanceTemplate()->SetInternalFieldCount(1);
	tpl->SetClassName(Nan::New("PrecisionModel").ToLocalChecked());

	Nan::SetPrototypeMethod(tpl, "getType", GetType);
	Nan::SetPrototypeMethod(tpl, "getScale", GetScale);
	Nan::SetPrototypeMethod(tpl, "getOffsetX", GetOffsetX);
	Nan::SetPrototypeMethod(tpl, "getOffsetY", GetOffsetY);

	Nan::SetPrototypeMethod(tpl, "toString", ToString);

	Nan::SetPrototypeMethod(tpl, "isFloating", IsFloating);

	Nan::SetPrototypeMethod(tpl, "compareTo", CompareTo);

	constructor.Reset(tpl->GetFunction());

	target->Set(Nan::New("PrecisionModel").ToLocalChecked(), tpl->GetFunction());

}

NAN_METHOD(PrecisionModel::New) {

	PrecisionModel *model;

	if (info.Length() == 0) {
		model = new PrecisionModel();
	} else {

		if (info[0]->IsString()) {

			if(info[0]->ToString()->Equals(Nan::New("FIXED").ToLocalChecked())) {
				model = new PrecisionModel(geos::geom::PrecisionModel::FIXED);
			} else if (info[0]->ToString()->Equals(Nan::New("FLOATING").ToLocalChecked())) {
				model = new PrecisionModel(geos::geom::PrecisionModel::FLOATING);
			} else {
				model = new PrecisionModel(geos::geom::PrecisionModel::FLOATING_SINGLE);
			}

		} else {
			model = new PrecisionModel(info[0]->NumberValue());
		}

	}

	model->Wrap(info.This());
	info.GetReturnValue().Set(info.This());

}

Handle<Value> PrecisionModel::New(const geos::geom::PrecisionModel *m) {

	Isolate* isolate = Isolate::GetCurrent();
	HandleScope scope(isolate);

	PrecisionModel *model = new PrecisionModel(m);
	Handle<Value> ext = External::New(isolate, model);

	Local<Function> cons = Local<Function>::New(isolate, constructor);
	MaybeLocal<v8::Object> maybeInstance = Nan::NewInstance(cons, 1, &ext);
	Local<v8::Object> instance;

	if (maybeInstance.IsEmpty()) {

		Nan::ThrowError("Could not create new PrecisionModel instance");

		return Undefined(isolate);

	} else {

		instance = maybeInstance.ToLocalChecked();

		model->Wrap(instance);

		return instance;

	}

}

NAN_METHOD(PrecisionModel::GetType) {
	PrecisionModel *model = Nan::ObjectWrap::Unwrap<PrecisionModel>(info.Holder());
	info.GetReturnValue().Set(Nan::New<Integer>(model->_model->getType()));
}

//TODO add a macro for this?
NAN_METHOD(PrecisionModel::GetScale) {
	PrecisionModel *model = Nan::ObjectWrap::Unwrap<PrecisionModel>(info.Holder());
	info.GetReturnValue().Set(Nan::New<Number>(model->_model->getScale()));
}

NAN_METHOD(PrecisionModel::GetOffsetX) {
	PrecisionModel *model = Nan::ObjectWrap::Unwrap<PrecisionModel>(info.Holder());
	info.GetReturnValue().Set(Nan::New<Number>(model->_model->getOffsetX()));
}

NAN_METHOD(PrecisionModel::GetOffsetY) {
	PrecisionModel *model = Nan::ObjectWrap::Unwrap<PrecisionModel>(info.Holder());
	info.GetReturnValue().Set(Nan::New<Number>(model->_model->getOffsetY()));
}

NAN_METHOD(PrecisionModel::ToString) {
	PrecisionModel *model = Nan::ObjectWrap::Unwrap<PrecisionModel>(info.Holder());
	info.GetReturnValue().Set(Nan::New(model->_model->toString().data()).ToLocalChecked());
}

NAN_METHOD(PrecisionModel::IsFloating) {
	PrecisionModel *model = Nan::ObjectWrap::Unwrap<PrecisionModel>(info.Holder());
	info.GetReturnValue().Set(model->_model->isFloating() ? Nan::True() : Nan::False());
}

NAN_METHOD(PrecisionModel::CompareTo) {
	PrecisionModel *model = Nan::ObjectWrap::Unwrap<PrecisionModel>(info.Holder());
	PrecisionModel *model2 = Nan::ObjectWrap::Unwrap<PrecisionModel>(info[0]->ToObject());
	info.GetReturnValue().Set(Nan::New<Integer>(model->_model->compareTo(model2->_model)));
}
