#!/usr/bin/env coffee

vows = require "vows"
assert = require "assert"

WKTReader = (require "../src").WKTReader
Geometry = (require "../src").Geometry
PreparedGeometry = (require "../src").PreparedGeometry
PreparedGeometryFactory = (require "../src").PreparedGeometryFactory

tests = (vows.describe "PreparedGeometryFactory").addBatch

  "A PreparedGeometryFactory":
    topic: ->
      new PreparedGeometryFactory()

    "a new instance should be an instance of PreparedGeometryFactory": (factory) ->
      assert.instanceOf factory, PreparedGeometryFactory

    "should return a PreparedGeometry": (factory) ->
      geometry = (new WKTReader()).read "MULTIPOLYGON (((40 40, 20 45, 45 30, 40 40)), ((20 35, 10 30, 10 10, 30 5, 45 20, 20 35), (30 20, 20 15, 20 25, 30 20)))"
      preparedGeometry = factory.prepare geometry
      assert.instanceOf preparedGeometry, PreparedGeometry

tests.export module
