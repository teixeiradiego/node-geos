#ifndef WKTWRITER_HPP
#define WKTWRITER_HPP

#include <geos/io/WKTWriter.h>
#include "binding.hpp"
#include "geometry.hpp"

class WKTWriter : public Nan::ObjectWrap {

	public:
		WKTWriter();
		~WKTWriter();
		static NAN_MODULE_INIT(Initialize);
		static Nan::Persistent<Function> constructor;
		static NAN_METHOD(New);
		static NAN_METHOD(Write);

	protected:
		static NAN_METHOD(SetRoundingPrecision);
		static NAN_METHOD(SetTrim);

	private:
		geos::io::WKTWriter* _writer;

};
#endif
