#ifndef WKBREADER_HPP
#define WKBREADER_HPP

#include <geos/io/WKBReader.h>
#include <geos/io/ParseException.h>

#include "binding.hpp"
#include "geometry.hpp"
#include "geometryfactory.hpp"

class WKBReader : public Nan::ObjectWrap {

	public:
		WKBReader();
		WKBReader(const geos::geom::GeometryFactory *gf);
		~WKBReader();
		static NAN_MODULE_INIT(Initialize);
		static Nan::Persistent<Function> constructor;
		static NAN_METHOD(New);
		static NAN_METHOD(ReadHEX);

	private:
		geos::io::WKBReader* _reader;

};
#endif
