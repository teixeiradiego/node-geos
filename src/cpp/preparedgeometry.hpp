#ifndef PREPARED_GEOMETRY_HPP
#define PREPARED_GEOMETRY_HPP

#include <uv.h>
#include <geos/geom/prep/PreparedGeometry.h>
#include <geos/util/GEOSException.h>

#include "utils.hpp"
#include "binding.hpp"
#include "geometry.hpp"

class PreparedGeometry : public Nan::ObjectWrap {

	public:

		const geos::geom::prep::PreparedGeometry *_instance;

		PreparedGeometry();
		PreparedGeometry(const geos::geom::prep::PreparedGeometry *preparedGeometry);
		~PreparedGeometry();

		static NAN_MODULE_INIT(Initialize);
		static Handle<Value> New(const geos::geom::prep::PreparedGeometry* preparedGeometry);

		void _ref() { Ref(); };
		void _unref() { Unref(); };

		NODE_GEOS_V8_FUNCTION(GetGeometry);

		// GEOS binary predicates
		NODE_GEOS_V8_FUNCTION(Contains);
		NODE_GEOS_V8_FUNCTION(ContainsProperly);
		NODE_GEOS_V8_FUNCTION(CoveredBy);
		NODE_GEOS_V8_FUNCTION(Covers);
		NODE_GEOS_V8_FUNCTION(Crosses);
		NODE_GEOS_V8_FUNCTION(Disjoint);
		NODE_GEOS_V8_FUNCTION(Intersects);
		NODE_GEOS_V8_FUNCTION(Overlaps);
		NODE_GEOS_V8_FUNCTION(Touches);
		NODE_GEOS_V8_FUNCTION(Within);

	protected:

		static NAN_METHOD(New);

	private:

		static Nan::Persistent<Function> constructor;

};
#endif
