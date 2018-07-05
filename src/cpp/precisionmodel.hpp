#ifndef PRECISION_MODEL_HPP
#define PRECISION_MODEL_HPP

#include <geos/geom/PrecisionModel.h>
#include "binding.hpp"

class PrecisionModel : public Nan::ObjectWrap {
	public:
		PrecisionModel();
		PrecisionModel(double newScale);
		PrecisionModel(geos::geom::PrecisionModel::Type nModelType);
		PrecisionModel(const geos::geom::PrecisionModel *model);
		~PrecisionModel();
		geos::geom::PrecisionModel *_model;

		static NAN_MODULE_INIT(Initialize);
		static Nan::Persistent<Function> constructor;
		static NAN_METHOD(New);
		static Handle<Value> New(const geos::geom::PrecisionModel *model);
		static NAN_METHOD(GetType);
		static NAN_METHOD(GetScale);
		static NAN_METHOD(GetOffsetX);
		static NAN_METHOD(GetOffsetY);
		static NAN_METHOD(ToString);
		static NAN_METHOD(IsFloating);
		static NAN_METHOD(CompareTo);

};
#endif
