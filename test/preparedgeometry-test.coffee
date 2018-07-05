#!/usr/bin/env coffee

vows = require "vows"
assert = require "assert"

PreparedGeometry = (require "../src").PreparedGeometry
PreparedGeometryFactory = (require "../src").PreparedGeometryFactory
WKTReader = (require "../src").WKTReader

tests = (vows.describe "PreparedGeometry").addBatch

  "A PreparedGeometry":
    topic: ->
      geometry = (new WKTReader()).read "POINT(1 1)"
      preparedGeometry = (new PreparedGeometryFactory()).prepare geometry
      @callback geometry, preparedGeometry

    "a new instance should be an instance of PreparedGeometry": (geom, prepgeom) ->
      assert.instanceOf prepgeom, PreparedGeometry

    # geos binary predicates
    "should have a disjoint function": (geom, prepgeom) ->
      assert.isFunction prepgeom.disjoint
      assert.isFalse prepgeom.disjoint geom

    "shoudl have an async disjoint function": (geom, prepgeom) ->
      prepgeom.disjoint geom, (error, disjoint) ->
        assert.isFalse disjoint

    "should have a touches function": (geom, prepgeom) ->
      assert.isFunction prepgeom.touches
      assert.ok not prepgeom.touches geom

    "should have an async touches function": (geom, prepgeom) ->
      prepgeom.touches geom, (error, touches) ->
        assert.isFalse touches

    "should have a intersects function": (geom, prepgeom) ->
      assert.isFunction prepgeom.intersects
      assert.isTrue prepgeom.intersects geom

    "should have an async intersects function": (geom, prepgeom) ->
      prepgeom.intersects geom, (error, intersects) ->
        assert.isTrue intersects

    "should have a crosses function": (geom, prepgeom) ->
      assert.isFunction prepgeom.crosses
      assert.isFalse prepgeom.crosses geom

    "should have an async crosses function": (geom, prepgeom) ->
      prepgeom.crosses geom, (error, crosses) ->
        assert.isFalse crosses

    "should have a within function": (geom, prepgeom) ->
      assert.isFunction prepgeom.within
      assert.isTrue prepgeom.within geom

    "should have an async within function": (geom, prepgeom) ->
      prepgeom.within geom, (error, within) ->
        assert.isTrue within

    "should have a contains function": (geom, prepgeom) ->
      assert.isFunction prepgeom.contains
      assert.isTrue prepgeom.contains geom

    "should have an async contains function": (geom, prepgeom) ->
      prepgeom.contains geom, (error, contains) ->
        assert.isTrue contains

    "should have a overlaps function": (geom, prepgeom) ->
      assert.isFunction prepgeom.overlaps
      assert.isFalse prepgeom.overlaps geom

    "should have a async overlaps function": (geom, prepgeom) ->
      prepgeom.overlaps geom, (error, overlaps) ->
        assert.isFalse overlaps

    "should have a containsProperly function": (geom, prepgeom) ->
      assert.isFunction prepgeom.containsProperly
      assert.isTrue prepgeom.containsProperly geom

    "should have an async containsProperly function": (geom, prepgeom) ->
      prepgeom.containsProperly geom, (error, equals) ->
        assert.isTrue equals

    "should have a covers function": (geom, prepgeom) ->
      assert.isFunction prepgeom.covers
      assert.isTrue prepgeom.covers geom

    "should have an async covers function": (geom, prepgeom) ->
      prepgeom.covers geom, (error, covers) ->
        assert.isTrue covers

    "should have a coveredBy function": (geom, prepgeom) ->
      assert.isFunction prepgeom.coveredBy
      assert.isTrue prepgeom.coveredBy geom

    "should have an async coveredBy function": (geom, prepgeom) ->
      prepgeom.coveredBy geom, (error, coveredBy) ->
        assert.isTrue coveredBy

  # "A direct call to PreparedGeometry constructor":

  #   topic: ->
  #     ->
  #       PreparedGeometry

  #   "should throw an Error": (call) ->
  #     assert.throws call(), Error

tests.export module
