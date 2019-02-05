package="lua-sentencepiece"
version="dev-1"
source = {
  url = ""
}

description = {
  summary = "Provides sentencepiece binding for encode or decode strings with sentencepiece model.",
  homepage = "",
  license = "MIT/X11"
}

dependencies = {
  "lua >= 5.1"
}

external_dependencies = {
  SENTPIECE = {
    header = "sentencepiece_processor.h"
  }
}

build = {
  type = "make",
  build_variables = {
    CFLAGS="$(CFLAGS) -std=c++11 -I$(LUA_INCDIR) -I$(SENTPIECE_INCDIR)",
    LDFLAGS="$(LIBFLAG) -L$(SENTPIECE_LIBDIR) -lsentencepiece"
  },
  install_variables = {
    INST_PREFIX="$(PREFIX)",
    INST_BINDIR="$(BINDIR)",
    INST_LIBDIR="$(LIBDIR)",
    INST_LUADIR="$(LUADIR)",
    INST_CONFDIR="$(CONFDIR)",
  },
}
