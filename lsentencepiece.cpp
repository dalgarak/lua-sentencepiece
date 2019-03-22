extern "C"
{
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#include <cstring>
#include <cstdlib>
#include <vector>
#include <string>
#include <iostream>

#include <sentencepiece_processor.h>

#define LSPP_INSTANCE	"luaL_SentencePieceProcessor"

#if LUA_VERSION_NUM < 502
  #define luaL_newlibtable(L,l) lua_createtable(L,0,sizeof(l)/sizeof((l)[0]))
  #define luaL_newlib(L,l) (luaL_newlibtable(L,l),luaL_register(L,NULL,l))
  #define luaL_setfuncs(L,l,nups) luaL_register(L,NULL,l)
  #define lua_pushunsigned(L,val) lua_pushinteger(L,val)
#endif

using namespace std;

static sentencepiece::SentencePieceProcessor* check_spprocessor(lua_State *L, int n)
{
	return *(sentencepiece::SentencePieceProcessor **) luaL_checkudata(L, n, LSPP_INSTANCE);
}

extern "C" {

int l_SentencePieceProcessor_new(lua_State* L) {
	const char* filename = luaL_checkstring(L, 1);

	sentencepiece::SentencePieceProcessor** spptr = (sentencepiece::SentencePieceProcessor **) lua_newuserdata(L, sizeof(sentencepiece::SentencePieceProcessor*));
	luaL_getmetatable(L, LSPP_INSTANCE);
	lua_setmetatable(L, -2);  

	*spptr = new sentencepiece::SentencePieceProcessor();
	const sentencepiece::util::Status status = (*spptr)->Load(filename);	
	if (!status.ok()) {
		lua_pushnil(L);
		lua_pushstring(L, "ERROR: CANNOT OPEN SENTENCEPIECE MODEL");
		return 2;
	}

	return 1;
}

int l_SentencePieceProcessor_destruct(lua_State* L)
{
	sentencepiece::SentencePieceProcessor* spptr = check_spprocessor(L, 1);
	delete spptr;

	return 0;
}

int l_SentencePieceProcessor_encode(lua_State* L)
{
	sentencepiece::SentencePieceProcessor* spptr = check_spprocessor(L, 1);
	const char* input_string = luaL_checkstring(L, 2); 
	std::string whitespace_tokenized_output;

	std::vector<std::string> pieces;
	spptr->Encode(input_string, &pieces);
	for (const std::string &token : pieces) {
		if (whitespace_tokenized_output.length() > 0) whitespace_tokenized_output += " ";
		whitespace_tokenized_output += token;
	}

	lua_pushstring(L, whitespace_tokenized_output.c_str());

	return 1;
}

int l_SentencePieceProcessor_decode(lua_State* L)
{
	sentencepiece::SentencePieceProcessor* spptr = check_spprocessor(L, 1);
	const char *input_string = luaL_checkstring(L, 2);
	char *tmp_input_str = strdup(input_string);
	std::vector<std::string> pieces;
	std::string final_outputs = "";
	
	char *token = NULL;
       	char *next = NULL;
	
	token = strtok_r(tmp_input_str, " ", &next);
	while (token != NULL) {
		pieces.push_back(token);
		token = strtok_r(NULL, " ", &next);
	}

	free(tmp_input_str);
	tmp_input_str = NULL;

	spptr->Decode(pieces, &final_outputs);
	lua_pushstring(L, final_outputs.c_str());

	return 1;
}

int l_SentencePieceProcessor_encodeExtraOptions(lua_State* L)
{
	sentencepiece::SentencePieceProcessor* spptr = check_spprocessor(L, 1);
	const char *option_str = luaL_checkstring(L, 2);
	if (strlen(option_str) == 0) {
		return 0;
	}

	spptr->SetEncodeExtraOptions(option_str);

	return 0;
}

int l_SentencePieceProcessor_decodeExtraOptions(lua_State* L)
{
	sentencepiece::SentencePieceProcessor* spptr = check_spprocessor(L, 1);
	const char *option_str = luaL_checkstring(L, 2);
	if (strlen(option_str) == 0) {
		return 0;
	}

	spptr->SetDecodeExtraOptions(option_str);

	return 0;
}

static const struct luaL_Reg sSentencePieceProcRegs[] = {
	{ "new", l_SentencePieceProcessor_new },
	{ NULL, NULL }
};

static const struct luaL_Reg sSentencePieceProc_m[] = {
	{ "__gc", l_SentencePieceProcessor_destruct },
	{ "encode", l_SentencePieceProcessor_encode },
	{ "decode", l_SentencePieceProcessor_decode },
	{ "encodeExtraOptions", l_SentencePieceProcessor_encodeExtraOptions },
	{ "decodeExtraOptions", l_SentencePieceProcessor_decodeExtraOptions },
	{ NULL, NULL }
};

int luaopen_SentencePieceProcessor(lua_State* L)
{
	luaL_newmetatable(L, LSPP_INSTANCE);
	luaL_setfuncs(L, sSentencePieceProc_m, 0);
	lua_pushvalue(L, -1);
	lua_setfield(L, -2, "__index");

	lua_newtable(L);
	luaL_newlib(L, sSentencePieceProcRegs);

	return 1;
}

}

#if defined(__TEST_LUA_SENTPIECE)

int main(int argc, char *argv[])
{
	lua_State *l = luaL_newstate();
	luaL_openlibs(l);
	luaopen_SentencePieceProcessor(l);
	//lua_setglobal(l, "SentencePieceProcessor");

	int erred = luaL_dofile(l, "spp_test.lua");
	if (erred) {
		std::cout << "Lua error: " << luaL_checkstring(l, -1) << std::endl;
	}

	lua_close(l);

	return EXIT_SUCCESS;
}

#endif
