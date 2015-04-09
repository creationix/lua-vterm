#include <lua.h>
#include <lauxlib.h>
#include "vterm.h"
#include <string.h>

typedef struct {
  VTerm* vterm;
} LVTerm;

static int state_putglyph(VTermGlyphInfo *info, VTermPos pos, void *user) {
  printf("putglyph\n");
  return 1;
}

static int state_movecursor(VTermPos pos, VTermPos oldpos, int visible, void *user) {
  printf("movecursor\n");
  return 1;
}

static int state_scrollrect(VTermRect rect, int downward, int rightward, void *user) {
  printf("scrollrect\n");
  return 1;
}

static int state_moverect(VTermRect dest, VTermRect src, void *user) {
  printf("moverect\n");
  return 1;
}

static int state_erase(VTermRect rect, int selective, void *user) {
  printf("erase\n");
  return 1;
}

static int state_initpen(void *user) {
  printf("initpen\n");
  return 1;
}

// int boolean;
// int number;
// char *string;
// VTermColor color;
static int state_setpenattr(VTermAttr attr, VTermValue *val, void *user) {
  switch (attr) {
    case VTERM_ATTR_BOLD:        // bool:   1, 22
      printf("setpenattr BOLD\n");
      break;
    case VTERM_ATTR_UNDERLINE:   // number: 4, 21, 24
      printf("setpenattr UNDERLINE\n");
      break;
    case VTERM_ATTR_ITALIC:      // bool:   3, 23
      printf("setpenattr ITALIC\n");
      break;
    case VTERM_ATTR_BLINK:       // bool:   5, 25
      printf("setpenattr BLINK\n");
      break;
    case VTERM_ATTR_REVERSE:     // bool:   7, 27
      printf("setpenattr REVERSE\n");
      break;
    case VTERM_ATTR_STRIKE:      // bool:   9, 29
      printf("setpenattr STRIKE\n");
      break;
    case VTERM_ATTR_FONT:        // number: 10-19
      printf("setpenattr FONT\n");
      break;
    case VTERM_ATTR_FOREGROUND:  // color:  30-39 90-97
      printf("setpenattr FOREGROUND #%02x%02x%02x\n",
        val->color.red,
        val->color.green,
        val->color.blue
      );
      break;
    case VTERM_ATTR_BACKGROUND:  // color:  40-49 100-107
      printf("setpenattr BACKGROUND #%02x%02x%02x\n",
        val->color.red,
        val->color.green,
        val->color.blue
      );
      break;
  }
  return 1;
}

static int state_settermprop(VTermProp prop, VTermValue *val, void *user) {
  printf("settermprop\n");
  return 1;
}

static int state_bell(void *user) {
  printf("bell\n");
  return 1;
}

static int state_resize(int rows, int cols, VTermPos *delta, void *user) {
  printf("resize\n");
  return 1;
}

static int state_setlineinfo(int row, const VTermLineInfo *newinfo, const VTermLineInfo *oldinfo, void *user) {
  printf("setlineinfo\n");
  return 1;
}


static const VTermStateCallbacks callbacks = {
  state_putglyph,
  state_movecursor,
  state_scrollrect,
  state_moverect,
  state_erase,
  state_initpen,
  state_setpenattr,
  state_settermprop,
  state_bell,
  state_resize,
  state_setlineinfo,
};

static int lvterm_new(lua_State* L) {
  int rows = luaL_checkinteger(L, 1);
  int cols = luaL_checkinteger(L, 2);
  LVTerm* lvterm = lua_newuserdata(L, sizeof(*lvterm));
  luaL_getmetatable(L, "vterm");
  lua_setmetatable(L, -2);

  lvterm->vterm = vterm_new(rows, cols);
  vterm_state_set_bold_highbright(vterm_obtain_state(lvterm->vterm), 1);

  vterm_state_set_callbacks(vterm_obtain_state(lvterm->vterm), &callbacks, L);

  return 1;
}

static const luaL_Reg vterm_functions[] = {
  {"new", lvterm_new},
  {NULL, NULL}
};

static VTerm* lvterm_check(lua_State* L, int pos) {
  LVTerm* lvterm = luaL_checkudata(L, 1, "vterm");
  return lvterm->vterm;
}

static int lvterm_get_size(lua_State* L) {
  VTerm* vterm = lvterm_check(L, 1);
  int rows, cols;
  vterm_get_size(vterm, &rows, &cols);
  lua_pushinteger(L, rows);
  lua_pushinteger(L, cols);
  return 2;
}

static int lvterm_set_size(lua_State* L) {
  VTerm* vterm = lvterm_check(L, 1);
  int rows = luaL_checkinteger(L, 2);
  int cols = luaL_checkinteger(L, 3);
  vterm_set_size(vterm, rows, cols);
  return 0;
}

static int lvterm_get_utf8(lua_State* L) {
  VTerm* vterm = lvterm_check(L, 1);
  lua_pushboolean(L, vterm_get_utf8(vterm));
  return 1;
}

static int lvterm_set_utf8(lua_State* L) {
  VTerm* vterm = lvterm_check(L, 1);
  luaL_checktype(L, 2, LUA_TBOOLEAN);
  int is_utf8 = lua_toboolean(L, 2);
  vterm_set_utf8(vterm, is_utf8);
  return 0;
}

static int lvterm_input_write(lua_State* L) {
  VTerm* vterm = lvterm_check(L, 1);
  size_t len;
  const char* bytes  = luaL_checklstring(L, 2, &len);
  lua_pushinteger(L, vterm_input_write(vterm, bytes, len));
  // vterm_screen_flush_damage(vterm);
  return 1;
}

void *vterm_parser_get_cbdata(VTerm *vt);


static const luaL_Reg vterm_methods[] = {
  {"get_size", lvterm_get_size},
  {"set_size", lvterm_set_size},
  {"get_utf8", lvterm_get_utf8},
  {"set_utf8", lvterm_set_utf8},
  {"input_write", lvterm_input_write},
  {NULL, NULL}
};

static int lvterm_tostring(lua_State* L) {
  VTerm* vterm = lvterm_check(L, 1);
  lua_pushfstring(L, "vterm: %p", vterm);
  return 1;
}

static int lvterm_gc(lua_State* L) {
  VTerm* vterm = lvterm_check(L, 1);
  printf("before free\n");
  vterm_free(vterm);
  printf("after free\n");
  return 0;
}

LUALIB_API int luaopen_vterm (lua_State *L) {

  luaL_newmetatable (L, "vterm");
  lua_pushcfunction(L, lvterm_tostring);
  lua_setfield(L, -2, "__tostring");
  lua_pushcfunction(L, lvterm_gc);
  lua_setfield(L, -2, "__gc");
  lua_newtable(L);
  luaL_register(L, NULL, vterm_methods);
  lua_setfield(L, -2, "__index");

  lua_newtable(L);
  luaL_register(L, NULL, vterm_functions);
  return 1;
}
