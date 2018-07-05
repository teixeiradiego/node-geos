#ifndef PREPARED_GEOMETRY_FACTORY_HPP
#define PREPARED_GEOMETRY_FACTORY_HPP

#include "binding.hpp"
#include "geometry.hpp"
#include "preparedgeometry.hpp"

#include "geos/geom/prep/PreparedGeometryFactory.h"

class PreparedGeometryFactory : public Nan::ObjectWrap {

	public:

		static NAN_MODULE_INIT(Initialize);

		static NAN_METHOD(Prepare);
		static NAN_METHOD(Destroy);

};
#endif
