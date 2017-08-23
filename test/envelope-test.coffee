#!/usr/bin/env coffee

vows = require "vows"
assert = require "assert"

Envelope = (require "../src").Envelope
WKTReader = (require "../src").WKTReader

tests = (vows.describe "Envelope").addBatch

  "A Envelope":
    topic: ->
      new Envelope(1.0, 1.0, 1.0, 1.0)

    "a new instance should be an instance of Envelope": (envelope) ->
      assert.instanceOf envelope, Envelope

    "should have a toString function" : (envelope) ->
      assert.isFunction envelope.toString
      assert.equal envelope.toString(), "Env[1:1,1:1]"

    "should have a getMaxY function" : (envelope) ->
      assert.isFunction envelope.getMaxY
      assert.equal envelope.getMaxY(), "1"

    "should have a getMaxX function" : (envelope) ->
      assert.isFunction envelope.getMaxX
      assert.equal envelope.getMaxX(), "1"

    "should have a getMinY function" : (envelope) ->
      assert.isFunction envelope.getMinY
      assert.equal envelope.getMinY(), "1"

    "should have a getMinX function" : (envelope) ->
      assert.isFunction envelope.getMinX
      assert.equal envelope.getMinX(), "1"

    "should have a intersects function": (envelope) ->
      assert.isFunction envelope.intersects
      assert.isTrue envelope.intersects envelope

    "should have an async intersects function": (envelope) ->
      envelope.intersects envelope, (error, intersects) ->
        assert.isTrue intersects

  "A direct call to Geometry default constructor":

    topic: ->
      new Envelope()

    "a new instance should be an instance of Envelope": (envelope) ->
      assert.instanceOf envelope, Envelope

    "should return an empty envelope": (envelope) ->
      assert.equal envelope.toString(), "Env[0:-1,0:-1]"

tests.export module
