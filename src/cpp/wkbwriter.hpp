#ifndef WKBWRITER_HPP
#define WKBWRITER_HPP

#include <sstream>
#include <geos/io/WKBWriter.h>
#include "binding.hpp"
#include "geometry.hpp"

class WKBWriter : public Nan::ObjectWrap {

	public:
		WKBWriter();
		~WKBWriter();
		static NAN_MODULE_INIT(Initialize);
		static Nan::Persistent<Function> constructor;
		static NAN_METHOD(New);
		static NAN_METHOD(WriteHEX);

	private:
		geos::io::WKBWriter* _writer;

};
#endif
