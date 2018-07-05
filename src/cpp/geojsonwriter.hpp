/*
GeoJSONWriter class

Writes a Geometry object (wrapped or unwrapped) to a GeoJSON (v1.0) object.
For the The GeoJSON Format Specification see http://geojson.org/geojson-spec.html

- The "coordinates" property of the exported GeoJSON object may be null, if the geometry is empty.
- There is no "crs" property exported.
- You can set the rounding precision and whether you want to add the "bbox" property.
- 2/3-dimensional coordinates are supported.
*/

#ifndef GEOJSONWRITER_HPP
#define GEOJSONWRITER_HPP

#include <math.h>
#include <geos/geom/Coordinate.h>
#include <geos/geom/Point.h>
#include <geos/geom/LineString.h>
#include <geos/geom/Polygon.h>
#include <geos/geom/MultiPoint.h>
#include <geos/geom/MultiLineString.h>
#include <geos/geom/MultiPolygon.h>
#include "binding.hpp"
#include "geometry.hpp"

class GeoJSONWriter : public Nan::ObjectWrap {

	protected:
		int decimalPlaces;
		double factor;
		bool bbox;

	private:
		static Nan::Persistent<Function> constructor;

	public:
		GeoJSONWriter();
		~GeoJSONWriter();
		void setRoundingPrecision(int places);
		void setBbox(bool bbox);
		Handle<Value> write(const geos::geom::Geometry* geom);
		Handle<Value> writeBbox(const geos::geom::Geometry* geom);

		static NAN_MODULE_INIT(Initialize);
		static NAN_METHOD(New);

	protected:
		double roundNumber(double coord);
		Handle<Array> coordinateToArray(const geos::geom::Coordinate* coord);
		Handle<Array> coordinateSequenceToArray(const geos::geom::CoordinateSequence* seq);
		Handle<Array> geometryCollectionToArrayOfArrays(const geos::geom::GeometryCollection* geom);
		Handle<Array> geometryCollectionToArrayOfObjects(const geos::geom::GeometryCollection* geom);
		Handle<Value> getCoordsOrGeom(const geos::geom::Geometry* geom);

		static NAN_METHOD(SetRoundingPrecision);
		static NAN_METHOD(SetBbox);
		static NAN_METHOD(Write);
		static NAN_METHOD(WriteBbox);

};

#endif
