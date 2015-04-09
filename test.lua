local dump = require('pretty-print').dump
local vterm = require('./build/vterm').new(40, 24)
vterm:set_utf8(false)
p(vterm, vterm:get_size())
local string = dump(require('uv'))
-- p(string)
vterm:input_write(string)

