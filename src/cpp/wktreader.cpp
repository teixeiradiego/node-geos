#include "wktreader.hpp"

WKTReader::WKTReader() {
	_reader = new geos::io::WKTReader();
}

WKTReader::WKTReader(const geos::geom::GeometryFactory *gf) {
	_reader = new geos::io::WKTReader(gf);
}

WKTReader::~WKTReader() {}

Nan::Persistent<Function> WKTReader::constructor;

NAN_MODULE_INIT(WKTReader::Initialize) {

	Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(WKTReader::New);
	tpl->InstanceTemplate()->SetInternalFieldCount(1);
	tpl->SetClassName(Nan::New("WKTReader").ToLocalChecked());

	Nan::SetPrototypeMethod(tpl, "read", WKTReader::Read);

	constructor.Reset(tpl->GetFunction());

	target->Set(Nan::New("WKTReader").ToLocalChecked(), tpl->GetFunction());

}

NAN_METHOD(WKTReader::New) {

	WKTReader *wktReader;

	if(info.Length() == 1) {
		GeometryFactory *factory = Nan::ObjectWrap::Unwrap<GeometryFactory>(info[0]->ToObject());
		wktReader = new WKTReader(factory->_factory.get());
	} else {
		wktReader = new WKTReader();
	}

	wktReader->Wrap(info.This());
	info.GetReturnValue().Set(info.This());

}

NAN_METHOD(WKTReader::Read) {

	WKTReader* reader = Nan::ObjectWrap::Unwrap<WKTReader>(info.Holder());

	try {
		geos::geom::Geometry* geom = reader->_reader->read(*String::Utf8Value(info[0]->ToString()));
		info.GetReturnValue().Set(Geometry::New(geom));
	} catch (geos::io::ParseException e) {
		Nan::ThrowError(Nan::New(e.what()).ToLocalChecked());
	} catch (geos::util::GEOSException e) {
		Nan::ThrowError(Nan::New(e.what()).ToLocalChecked());
	} catch (...) {
		Nan::ThrowError(Nan::New("Exception while reading WKT.").ToLocalChecked());
	}

}
