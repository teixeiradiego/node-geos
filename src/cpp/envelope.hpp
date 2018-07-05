#ifndef ENVELOPE_HPP
#define ENVELOPE_HPP

#include <uv.h>
#include <geos/geom/Envelope.h>
#include <geos/util/GEOSException.h>
#include "binding.hpp"
#include "geojsonwriter.hpp"

class Envelope : public Nan::ObjectWrap {

	public:

		const geos::geom::Envelope * _instance;

		Envelope();
		Envelope(double x1, double x2, double y1, double y2);
		Envelope(const geos::geom::Envelope * envelope);
		~Envelope();

		static NAN_MODULE_INIT(Initialize);
		static Handle<Value> New(const geos::geom::Envelope * envelope);

		void _ref() { Ref(); };
		void _unref() { Unref(); };

	protected:

		static NAN_METHOD(New);

		static NAN_METHOD(GetMaxY);
		static NAN_METHOD(GetMaxX);
		static NAN_METHOD(GetMinY);
		static NAN_METHOD(GetMinX);

		NODE_GEOS_V8_FUNCTION(Intersects);

	private:

		static Nan::Persistent<Function> constructor;
		static NAN_METHOD(ToString);

};
#endif
