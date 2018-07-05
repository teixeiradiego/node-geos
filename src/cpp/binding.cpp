#include "binding.hpp"
#include "wktreader.hpp"
#include "wkbreader.hpp"
#include "wktwriter.hpp"
#include "wkbwriter.hpp"
#include "envelope.hpp"
#include "geometry.hpp"
#include "geometryfactory.hpp"
#include "precisionmodel.hpp"
#include "geojsonwriter.hpp"
#include "geojsonreader.hpp"

#include "preparedgeometry.hpp"
#include "preparedgeometryfactory.hpp"

extern "C" {
	void init (Handle<Object> target) {
		Isolate* isolate = Isolate::GetCurrent();
		HandleScope scope(isolate);

		target->Set(String::NewFromUtf8(isolate, "geosversion"), String::NewFromUtf8(isolate, geos::geom::geosversion().data()));
		target->Set(String::NewFromUtf8(isolate, "jtsport"), String::NewFromUtf8(isolate, geos::geom::jtsport().data()));

		Envelope::Initialize(target);
		Geometry::Initialize(target);
		WKTReader::Initialize(target);
		WKBReader::Initialize(target);
		WKTWriter::Initialize(target);
		WKBWriter::Initialize(target);
		GeometryFactory::Initialize(target);
		PrecisionModel::Initialize(target);
		GeoJSONWriter::Initialize(target);
		GeoJSONReader::Initialize(target);
		PreparedGeometry::Initialize(target);
		PreparedGeometryFactory::Initialize(target);
	}

	NODE_MODULE(geos, init)
}
