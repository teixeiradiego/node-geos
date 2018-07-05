#include "wktwriter.hpp"

WKTWriter::WKTWriter() {
	_writer = new geos::io::WKTWriter();
}

WKTWriter::~WKTWriter() {}

Nan::Persistent<Function> WKTWriter::constructor;

NAN_MODULE_INIT(WKTWriter::Initialize) {

	Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(WKTWriter::New);

	tpl->InstanceTemplate()->SetInternalFieldCount(1);
	tpl->SetClassName(Nan::New("WKTWriter").ToLocalChecked());

	Nan::SetPrototypeMethod(tpl, "write", WKTWriter::Write);
	Nan::SetPrototypeMethod(tpl, "setRoundingPrecision", WKTWriter::SetRoundingPrecision);
	Nan::SetPrototypeMethod(tpl, "setTrim", WKTWriter::SetTrim);

	constructor.Reset(tpl->GetFunction());

	target->Set(Nan::New("WKTWriter").ToLocalChecked(), tpl->GetFunction());
}

NAN_METHOD(WKTWriter::New) {
	WKTWriter* writer = new WKTWriter();
	writer->Wrap(info.This());
	info.GetReturnValue().Set(info.This());
}

NAN_METHOD(WKTWriter::Write) {

	WKTWriter *writer = Nan::ObjectWrap::Unwrap<WKTWriter>(info.Holder());
	Geometry *geom = Nan::ObjectWrap::Unwrap<Geometry>(info[0]->ToObject());

	//TODO catch exception?
	std::string str = writer->_writer->write(geom->_instance);
	info.GetReturnValue().Set(Nan::New(str.data()).ToLocalChecked());
}

NAN_METHOD(WKTWriter::SetRoundingPrecision) {

	WKTWriter *writer = Nan::ObjectWrap::Unwrap<WKTWriter>(info.Holder());
	writer->_writer->setRoundingPrecision(info[0]->Int32Value());
	info.GetReturnValue().Set(Nan::Undefined());

}

NAN_METHOD(WKTWriter::SetTrim) {

	WKTWriter *writer = Nan::ObjectWrap::Unwrap<WKTWriter>(info.Holder());
	writer->_writer->setTrim(info[0]->BooleanValue());
	info.GetReturnValue().Set(Nan::Undefined());

}
