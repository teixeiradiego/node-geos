#include "wkbwriter.hpp"

WKBWriter::WKBWriter() {
	_writer = new geos::io::WKBWriter();
}

WKBWriter::~WKBWriter() {}

Nan::Persistent<Function> WKBWriter::constructor;

NAN_MODULE_INIT(WKBWriter::Initialize) {

	Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(WKBWriter::New);
	tpl->InstanceTemplate()->SetInternalFieldCount(1);
	tpl->SetClassName(Nan::New("WKBWriter").ToLocalChecked());

	Nan::SetPrototypeMethod(tpl, "writeHEX", WKBWriter::WriteHEX);

	constructor.Reset(tpl->GetFunction());

	target->Set(Nan::New("WKBWriter").ToLocalChecked(), tpl->GetFunction());

}

NAN_METHOD(WKBWriter::New) {

	WKBWriter* writer = new WKBWriter();
	writer->Wrap(info.This());
	info.GetReturnValue().Set(info.This());

}

NAN_METHOD(WKBWriter::WriteHEX) {

	WKBWriter *writer = Nan::ObjectWrap::Unwrap<WKBWriter>(info.Holder());
	Geometry *geom = Nan::ObjectWrap::Unwrap<Geometry>(info[0]->ToObject());

	//TODO catch exception?
	std::stringstream ss;
	writer->_writer->writeHEX(*geom->_instance, ss);
	info.GetReturnValue().Set(Nan::New(ss.str().c_str()).ToLocalChecked());

}
