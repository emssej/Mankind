AC_INIT([mankind], [alpha 0.0.2], [bug@k00l.net])
AM_INIT_AUTOMAKE([-Wall])
AC_PROG_CXX
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile src/Makefile])
DEPS_CPPFLAGS=`pkg-config --cflags sdl2 SDL2_image SDL2_ttf gl glew tinyxml2`
DEPS_LDFLAGS=`pkg-config --libs-only-other sdl2 SDL2_image SDL2_ttf gl glew tinyxml2`
DEPS_LDADD=`pkg-config --libs-only-l sdl2 SDL2_image SDL2_ttf gl glew tinyxml2`
AC_SUBST(DEPS_CPPFLAGS)
AC_SUBST(DEPS_LDFLAGS)
AC_SUBST(DEPS_LDADD)
AC_OUTPUT
