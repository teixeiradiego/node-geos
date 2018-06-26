{print} = require "util"
_spawn = (require "child_process").spawn

coffeeBin = "coffee"
vows = "vows"
nodeGyp = "node-gyp"
prebuildInstall = "prebuild-install"

if process.platform is "win32"
  coffeeBin = "coffee.cmd"
  vows = "vows.cmd"
  nodeGyp = "node-gyp.cmd"
  prebuildInstall = "prebuild-install.cmd"

spawn = (command, options, exit_cb) ->
  program = _spawn command, options
  program.stdout.on "data", (data) -> print data.toString()
  program.stderr.on "data", (data) -> print data.toString()
  program.on "exit", (code)->
    if code != 0
      exit_cb?()
  program #return program

buildBinary = ->
  spawn nodeGyp, ["rebuild"]

build = (src, dst, watch) ->
  options = ["-c", "-o", dst, src]
  options.unshift "-w" if watch

  spawn coffeeBin, options

task "binary", "Compile C++ Code", ->
  buildBinary()

task "build", "Compile CoffeeScript source files", ->
  build "src", "lib"
  build "examples", "examples"

task "install", "Compile CoffeeScript and C++ Code", ->
  spawn prebuildInstall, ["--debug", "--verbose"], buildBinary
  build "src", "lib"
  build "examples", "examples"

task "watch", "Recompile CoffeeScript source files when modified", ->
  build "src", "lib", true
  build "examples", "examples", true

task "test", "Run the geos tests", ->
  spawn vows, ["--spec"]

task "clean", "Remove all \"binary\" data", ->
  spawn nodeGyp, ["clean"]
  spawn "rm", ["./lib/index.js"]
