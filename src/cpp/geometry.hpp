#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <uv.h>
#include <geos/geom/Geometry.h>
#include <geos/util/GEOSException.h>
#include "utils.hpp"
#include "binding.hpp"
#include "geojsonwriter.hpp"
#include "envelope.hpp"

class Geometry : public Nan::ObjectWrap {

	public:

		geos::geom::Geometry *_instance;

		Geometry();
		Geometry(geos::geom::Geometry *geom);
		~Geometry();

		static NAN_MODULE_INIT(Initialize);

		static Handle<Value> New(geos::geom::Geometry * geometry);

		void _ref() { Ref(); };
		void _unref() { Unref(); };

	protected:

		static NAN_METHOD(New);

		// GEOS unary predicates
		NODE_GEOS_V8_FUNCTION(IsSimple);
		NODE_GEOS_V8_FUNCTION(IsValid);
		NODE_GEOS_V8_FUNCTION(IsEmpty);
		NODE_GEOS_V8_FUNCTION(IsRectangle);

		// GEOS binary predicates
		NODE_GEOS_V8_FUNCTION(Disjoint);
		NODE_GEOS_V8_FUNCTION(Touches);
		NODE_GEOS_V8_FUNCTION(Intersects);
		NODE_GEOS_V8_FUNCTION(Crosses);
		NODE_GEOS_V8_FUNCTION(Within);
		NODE_GEOS_V8_FUNCTION(Contains);
		NODE_GEOS_V8_FUNCTION(Overlaps);
		NODE_GEOS_V8_FUNCTION(Equals);
		NODE_GEOS_V8_FUNCTION(Covers);
		NODE_GEOS_V8_FUNCTION(CoveredBy);

		//static NAN_METHOD(EqualsExact);
		static NAN_METHOD(IsWithinDistance);

		// GEOS topologic function
		static NAN_METHOD(Intersection);
		static NAN_METHOD(Union);
		static NAN_METHOD(Difference);
		static NAN_METHOD(SymDifference);

		static NAN_METHOD(GetBoundary);
		static NAN_METHOD(GetEnvelope);
		static NAN_METHOD(GetEnvelopeInternal);
		static NAN_METHOD(GetCentroid);
		static NAN_METHOD(ConvexHull);

		static NAN_METHOD(Buffer);

		static NAN_METHOD(Distance);

		static NAN_METHOD(GetArea);
		static NAN_METHOD(GetLength);

		static NAN_METHOD(GetSRID);
		static NAN_METHOD(SetSRID);

		static NAN_METHOD(GetGeometryType);

		static NAN_METHOD(ToJSON);

	private:

		static Nan::Persistent<Function> constructor;
		static NAN_METHOD(ToString);

};
#endif
