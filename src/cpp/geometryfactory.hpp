#ifndef GEOMETRY_FACTORY_HPP
#define GEOMETRY_FACTORY_HPP

#include "binding.hpp"
#include "geometry.hpp"
#include "precisionmodel.hpp"
#include "geos/geom/GeometryFactory.h"
#include "geos/geom/PrecisionModel.h"

class GeometryFactory : public Nan::ObjectWrap {

	public:

		geos::geom::GeometryFactory::unique_ptr _factory;

		GeometryFactory();
		GeometryFactory(const geos::geom::PrecisionModel *pm);
		GeometryFactory(const geos::geom::PrecisionModel *pm, int newSRID);
		~GeometryFactory();

		static NAN_MODULE_INIT(Initialize);
		static Nan::Persistent<Function> constructor;
		static NAN_METHOD(New);

		static NAN_METHOD(GetPrecisionModel);
		static NAN_METHOD(GetSRID);
		static NAN_METHOD(Destroy);

};
#endif
