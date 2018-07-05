#ifndef WKTREADER_HPP
#define WKTREADER_HPP

#include <geos/io/WKTReader.h>
#include <geos/io/ParseException.h>

#include "binding.hpp"
#include "geometry.hpp"
#include "geometryfactory.hpp"

class WKTReader : public Nan::ObjectWrap {

	public:
		WKTReader();
		WKTReader(const geos::geom::GeometryFactory *gf);
		~WKTReader();
		static NAN_MODULE_INIT(Initialize);
		static Nan::Persistent<Function> constructor;
		static NAN_METHOD(New);
		static NAN_METHOD(Read);

	protected:

	private:
		geos::io::WKTReader* _reader;

};
#endif
