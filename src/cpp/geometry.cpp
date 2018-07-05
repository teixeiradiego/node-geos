#include "geometry.hpp"

Geometry::Geometry() {}

Geometry::Geometry(geos::geom::Geometry *geom) : Nan::ObjectWrap() {
	_instance = geom;
}

Geometry::~Geometry() {}

Nan::Persistent<Function> Geometry::constructor;

NAN_MODULE_INIT(Geometry::Initialize) {

	Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(Geometry::New);
	tpl->InstanceTemplate()->SetInternalFieldCount(1);
	tpl->SetClassName(Nan::New("Geometry").ToLocalChecked());

	Nan::SetPrototypeMethod(tpl, "toString", Geometry::ToString);
	//GEOS unary predicates
	Nan::SetPrototypeMethod(tpl, "isSimple", Geometry::IsSimple);
	Nan::SetPrototypeMethod(tpl, "isValid", Geometry::IsValid);
	Nan::SetPrototypeMethod(tpl, "isEmpty", Geometry::IsEmpty);
	Nan::SetPrototypeMethod(tpl, "isRectangle", Geometry::IsRectangle);
	//GEOS binary predicates
	//TODO maybe define a macro for this too
	Nan::SetPrototypeMethod(tpl, "disjoint", Geometry::Disjoint);
	Nan::SetPrototypeMethod(tpl, "touches", Geometry::Touches);
	Nan::SetPrototypeMethod(tpl, "intersects", Geometry::Intersects);
	Nan::SetPrototypeMethod(tpl, "crosses", Geometry::Crosses);
	Nan::SetPrototypeMethod(tpl, "within", Geometry::Within);
	Nan::SetPrototypeMethod(tpl, "contains", Geometry::Contains);
	Nan::SetPrototypeMethod(tpl, "overlaps", Geometry::Overlaps);
	Nan::SetPrototypeMethod(tpl, "equals", Geometry::Equals);
	Nan::SetPrototypeMethod(tpl, "covers", Geometry::Covers);
	Nan::SetPrototypeMethod(tpl, "coveredBy", Geometry::CoveredBy);

	Nan::SetPrototypeMethod(tpl, "isWithinDistance", Geometry::IsWithinDistance);

	//GEOS binary topologic functions
	Nan::SetPrototypeMethod(tpl, "intersection", Geometry::Intersection);
	Nan::SetPrototypeMethod(tpl, "union", Geometry::Union);
	Nan::SetPrototypeMethod(tpl, "difference", Geometry::Difference);
	Nan::SetPrototypeMethod(tpl, "symDifference", Geometry::SymDifference);

	//GEOS unary topologic functions
	Nan::SetPrototypeMethod(tpl, "getEnvelope", Geometry::GetEnvelope);
	Nan::SetPrototypeMethod(tpl, "getCentroid", Geometry::GetCentroid);
	Nan::SetPrototypeMethod(tpl, "getBoundary", Geometry::GetBoundary);
	Nan::SetPrototypeMethod(tpl, "convexHull", Geometry::ConvexHull);

	Nan::SetPrototypeMethod(tpl, "buffer", Geometry::Buffer);
	Nan::SetPrototypeMethod(tpl, "getEnvelopeInternal", Geometry::GetEnvelopeInternal);

	Nan::SetPrototypeMethod(tpl, "distance", Geometry::Distance);

	Nan::SetPrototypeMethod(tpl, "getArea", Geometry::GetArea);
	Nan::SetPrototypeMethod(tpl, "getLength", Geometry::GetLength);
	Nan::SetPrototypeMethod(tpl, "getSRID", Geometry::GetSRID);
	Nan::SetPrototypeMethod(tpl, "setSRID", Geometry::SetSRID);

	Nan::SetPrototypeMethod(tpl, "getGeometryType", Geometry::GetGeometryType);

	Nan::SetPrototypeMethod(tpl, "toJSON", Geometry::ToJSON);

	constructor.Reset(tpl->GetFunction());

	target->Set(Nan::New("Geometry").ToLocalChecked(), tpl->GetFunction());
}

Handle<Value> Geometry::New(geos::geom::Geometry *geometry) {

	Isolate* isolate = Isolate::GetCurrent();
	HandleScope scope(isolate);

	Geometry *geom = new Geometry(geometry);
	Handle<Value> ext = External::New(isolate, geom);

	Local<Function> cons = Local<Function>::New(isolate, constructor);
	MaybeLocal<v8::Object> maybeInstance = Nan::NewInstance(cons, 1, &ext);
	Local<v8::Object> instance;

	if (maybeInstance.IsEmpty()) {

		Nan::ThrowError("Could not create new Geometry instance");

		return Undefined(isolate);

	} else {

		instance = maybeInstance.ToLocalChecked();

		geom->Wrap(instance);

		return instance;

	}

}

NAN_METHOD(Geometry::New) {

	Geometry *geom;
	geom = new Geometry();
	geom->Wrap(info.This());
	info.GetReturnValue().Set(info.This());

}

NAN_METHOD(Geometry::ToString) {

	Geometry* geom = Nan::ObjectWrap::Unwrap<Geometry>(info.Holder());

	try {

		info.GetReturnValue().Set(Nan::New(geom->_instance->toString().data()).ToLocalChecked());

	} catch(geos::util::GEOSException exception) {
		Nan::ThrowError(Nan::New(exception.what()).ToLocalChecked());
	}

}

NAN_METHOD(Geometry::GetGeometryType) {

	Geometry* geom = Nan::ObjectWrap::Unwrap<Geometry>(info.Holder());

	try {

		info.GetReturnValue().Set(Nan::New(geom->_instance->getGeometryType().data()).ToLocalChecked());

	} catch(geos::util::GEOSException exception) {
		Nan::ThrowError(Nan::New(exception.what()).ToLocalChecked());
	}

}

NAN_METHOD(Geometry::Distance) {

	Geometry* geom = Nan::ObjectWrap::Unwrap<Geometry>(info.Holder());
	Geometry* geom2 = Nan::ObjectWrap::Unwrap<Geometry>(info[0]->ToObject());

	try {

		info.GetReturnValue().Set(Nan::New<Number>(geom->_instance->distance(geom2->_instance)));

	} catch(geos::util::GEOSException exception) {
		Nan::ThrowError(Nan::New(exception.what()).ToLocalChecked());
	}

}

NAN_METHOD(Geometry::IsWithinDistance) {

	Geometry* geom = Nan::ObjectWrap::Unwrap<Geometry>(info.Holder());
	Geometry* geom2 = Nan::ObjectWrap::Unwrap<Geometry>(info[0]->ToObject());
	double distance = info[0]->NumberValue();

	try {

		info.GetReturnValue().Set(
			geom->_instance->isWithinDistance(geom2->_instance, distance) ? Nan::True() : Nan::False()
		);

	} catch(geos::util::GEOSException exception) {
		Nan::ThrowError(Nan::New(exception.what()).ToLocalChecked());
	}

}

NAN_METHOD(Geometry::SetSRID) {

	Geometry* geom = ObjectWrap::Unwrap<Geometry>(info.Holder());

	try {

		geom->_instance->setSRID(info[0]->IntegerValue());

		info.GetReturnValue().Set(Nan::Undefined());

	} catch(geos::util::GEOSException exception) {
		Nan::ThrowError(Nan::New(exception.what()).ToLocalChecked());
	}

}

NAN_METHOD(Geometry::ToJSON) {

	Geometry* geom = ObjectWrap::Unwrap<Geometry>(info.Holder());
	GeoJSONWriter writer;

	if (info.Length() >= 1 && info[0]->IsInt32()) {
		writer.setRoundingPrecision(info[0]->Int32Value());
	}

	if (info.Length() >= 2 && info[1]->IsBoolean()) {
		writer.setBbox(info[1]->BooleanValue());
	}

	Handle<Value> json = writer.write(geom->_instance);
	info.GetReturnValue().Set(json);

}

NAN_METHOD(Geometry::Buffer) {

	double distance;
	int quadrantSegments;

	Geometry* geom = ObjectWrap::Unwrap<Geometry>(info.Holder());
	distance = info[0]->NumberValue();

	try {

		Handle<Value> result;

		if (info.Length() == 1) {
			result = Geometry::New(geom->_instance->buffer(distance));
		} else if (info.Length() == 2) {
			quadrantSegments = info[1]->IntegerValue();
			result = Geometry::New(geom->_instance->buffer(distance, quadrantSegments));
		} else {
			quadrantSegments = info[1]->IntegerValue();
			int endCapStyle = info[2]->IntegerValue();
			result = Geometry::New(geom->_instance->buffer(distance, quadrantSegments, endCapStyle));
		}

		info.GetReturnValue().Set(result);

	} catch(geos::util::GEOSException exception) {
		Nan::ThrowError(Nan::New(exception.what()).ToLocalChecked());
	}

}

NAN_METHOD(Geometry::GetEnvelopeInternal) {

	Geometry* geom = ObjectWrap::Unwrap<Geometry>(info.Holder());

	try {

		Handle<Value> result = Envelope::New(geom->_instance->getEnvelopeInternal());

		info.GetReturnValue().Set(result);

	} catch(geos::util::GEOSException exception) {
		Nan::ThrowError(Nan::New(exception.what()).ToLocalChecked());
	}

}

//GEOS unary predicates
NODE_GEOS_UNARY_PREDICATE(Geometry, IsSimple, isSimple);
NODE_GEOS_UNARY_PREDICATE(Geometry, IsValid, isValid);
NODE_GEOS_UNARY_PREDICATE(Geometry, IsEmpty, isEmpty);
NODE_GEOS_UNARY_PREDICATE(Geometry, IsRectangle, isRectangle);

// GEOS binary predicates
NODE_GEOS_BINARY_PREDICATE(Geometry, Geometry, Disjoint, disjoint);
NODE_GEOS_BINARY_PREDICATE(Geometry, Geometry, Touches, touches);
NODE_GEOS_BINARY_PREDICATE(Geometry, Geometry, Intersects, intersects);
NODE_GEOS_BINARY_PREDICATE(Geometry, Geometry, Crosses, crosses);
NODE_GEOS_BINARY_PREDICATE(Geometry, Geometry, Within, within);
NODE_GEOS_BINARY_PREDICATE(Geometry, Geometry, Contains, contains);
NODE_GEOS_BINARY_PREDICATE(Geometry, Geometry, Overlaps, overlaps);
NODE_GEOS_BINARY_PREDICATE(Geometry, Geometry, Equals, equals);
NODE_GEOS_BINARY_PREDICATE(Geometry, Geometry, Covers, covers);
NODE_GEOS_BINARY_PREDICATE(Geometry, Geometry, CoveredBy, coveredBy);

// GEOS unary topologic functions
NODE_GEOS_UNARY_TOPOLOGIC_FUNCTION(geos::geom, Geometry, GetEnvelope, getEnvelope);
NODE_GEOS_UNARY_TOPOLOGIC_FUNCTION(geos::geom, Geometry, GetCentroid, getCentroid);
NODE_GEOS_UNARY_TOPOLOGIC_FUNCTION(geos::geom, Geometry, GetBoundary, getBoundary);
NODE_GEOS_UNARY_TOPOLOGIC_FUNCTION(geos::geom, Geometry, ConvexHull, convexHull);

// GEOS binary topologic functions
NODE_GEOS_BINARY_TOPOLOGIC_FUNCTION(geos::geom, Geometry, Intersection, intersection);
NODE_GEOS_BINARY_TOPOLOGIC_FUNCTION(geos::geom, Geometry, Union, Union);
NODE_GEOS_BINARY_TOPOLOGIC_FUNCTION(geos::geom, Geometry, Difference, difference);
NODE_GEOS_BINARY_TOPOLOGIC_FUNCTION(geos::geom, Geometry, SymDifference, symDifference);

NODE_GEOS_DOUBLE_GETTER(Geometry, GetArea, getArea);
NODE_GEOS_DOUBLE_GETTER(Geometry, GetLength, getLength);
NODE_GEOS_DOUBLE_GETTER(Geometry, GetSRID, getSRID);
