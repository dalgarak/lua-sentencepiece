-- spp_test.lua
--
local SentencePieceProcessor = require('SentencePieceProcessor')
local spp = SentencePieceProcessor.new("./sample.model/wordpiece_ko.model")

print ('Test 1: Simple Encode-Decode Test')

local pieces = spp:encode("'I don't have no money'에는 이중 부정이 들어 있다.")
print ('Encoded: ' .. pieces)

local reverted = spp:decode(pieces)
print ('Decoded: ' .. reverted)

print ('Test 2: With Extra Options')

spp:encodeExtraOptions("reverse:bos:eos");
pieces = spp:encode("'I don't have no money'에는 이중 부정이 들어 있다.")

print ('Encoded: ' .. pieces)

spp:decodeExtraOptions("reverse");

reverted = spp:decode(pieces)
print ('Decoded: ' .. reverted)


