#include "wkbreader.hpp"
#include <sstream>

WKBReader::WKBReader() {
	_reader = new geos::io::WKBReader();
}

WKBReader::WKBReader(const geos::geom::GeometryFactory *gf) {
	_reader = new geos::io::WKBReader(*gf);
}

WKBReader::~WKBReader() {}

Nan::Persistent<Function> WKBReader::constructor;

NAN_MODULE_INIT(WKBReader::Initialize) {

	Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(WKBReader::New);
	tpl->InstanceTemplate()->SetInternalFieldCount(1);
	tpl->SetClassName(Nan::New("WKBReader").ToLocalChecked());

	Nan::SetPrototypeMethod(tpl, "readHEX", WKBReader::ReadHEX);

	constructor.Reset(tpl->GetFunction());

	target->Set(Nan::New("WKBReader").ToLocalChecked(), tpl->GetFunction());

}

NAN_METHOD(WKBReader::New) {

	WKBReader *wkbReader;

	if(info.Length() == 1) {
		GeometryFactory *factory = Nan::ObjectWrap::Unwrap<GeometryFactory>(info[0]->ToObject());
		wkbReader = new WKBReader(factory->_factory.get());
	} else {
		wkbReader = new WKBReader();
	}

	wkbReader->Wrap(info.This());
	info.GetReturnValue().Set(info.This());

}

NAN_METHOD(WKBReader::ReadHEX) {

	WKBReader* reader = Nan::ObjectWrap::Unwrap<WKBReader>(info.Holder());
	String::Utf8Value hex(info[0]->ToString());
	std::string str = std::string(*hex);
	std::istringstream is( str );

	try {
		geos::geom::Geometry* geom = reader->_reader->readHEX(is);
		info.GetReturnValue().Set(Geometry::New(geom));
	} catch (geos::io::ParseException e) {
		Nan::ThrowError(Nan::New(e.what()).ToLocalChecked());
	} catch (geos::util::GEOSException e) {
		Nan::ThrowError(Nan::New(e.what()).ToLocalChecked());
	} catch (...) {
		Nan::ThrowError(Nan::New("Exception while reading WKB.").ToLocalChecked());
	}
}
