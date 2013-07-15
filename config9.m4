dnl
dnl $Id: config9.m4,v 1.9 2007/11/01 14:30:16 mikl Exp $
dnl

PHP_ARG_ENABLE(memcache, whether to enable memcache support,
[  --enable-memcache       Enable memcache support])

PHP_ARG_ENABLE(memcache-session, whether to enable memcache session handler support,
[  --disable-memcache-session       Disable memcache session handler support], yes, no)

PHP_ARG_ENABLE(memcache-igbinary, whether to enable memcache igbinary serializer support,
[ --enable-memcache-igbinary Enable memcache igbinary serializer support], yes, yes)

if test -z "$PHP_ZLIB_DIR"; then
PHP_ARG_WITH(zlib-dir, for the location of ZLIB,
[  --with-zlib-dir[=DIR]   memcache: Set the path to ZLIB install prefix.], no, no)
fi

if test -z "$PHP_DEBUG"; then
  AC_ARG_ENABLE(debug,
  [  --enable-debug          compile with debugging symbols],[
    PHP_DEBUG=$enableval
  ],[
    PHP_DEBUG=no
  ]) 
fi

if test "$PHP_MEMCACHE" != "no"; then

  if test "$PHP_ZLIB_DIR" != "no" && test "$PHP_ZLIB_DIR" != "yes"; then
    if test -f "$PHP_ZLIB_DIR/include/zlib/zlib.h"; then
      PHP_ZLIB_DIR="$PHP_ZLIB_DIR"
      PHP_ZLIB_INCDIR="$PHP_ZLIB_DIR/include/zlib"
    elif test -f "$PHP_ZLIB_DIR/include/zlib.h"; then
      PHP_ZLIB_DIR="$PHP_ZLIB_DIR"
      PHP_ZLIB_INCDIR="$PHP_ZLIB_DIR/include"
    else
      AC_MSG_ERROR([Can't find ZLIB headers under "$PHP_ZLIB_DIR"])
    fi
  else
    for i in /usr/local /usr; do
      if test -f "$i/include/zlib/zlib.h"; then
        PHP_ZLIB_DIR="$i"
        PHP_ZLIB_INCDIR="$i/include/zlib"
      elif test -f "$i/include/zlib.h"; then
        PHP_ZLIB_DIR="$i"
        PHP_ZLIB_INCDIR="$i/include"
      fi
    done
        
  fi

  dnl # zlib
  AC_MSG_CHECKING([for the location of zlib])
  if test "$PHP_ZLIB_DIR" = "no"; then
    AC_MSG_ERROR([memcache support requires ZLIB. Use --with-zlib-dir=<DIR> to specify prefix where ZLIB include and library are located])
  else
    AC_MSG_RESULT([$PHP_ZLIB_DIR])
    if test "z$PHP_LIBDIR" != "z"; then
    dnl PHP5+
      PHP_ADD_LIBRARY_WITH_PATH(z, $PHP_ZLIB_DIR/$PHP_LIBDIR, MEMCACHE_SHARED_LIBADD)
    else 
    dnl PHP4
      PHP_ADD_LIBRARY_WITH_PATH(z, $PHP_ZLIB_DIR/lib, MEMCACHE_SHARED_LIBADD)
    fi
    PHP_ADD_INCLUDE($PHP_ZLIB_INCDIR)
  fi

  dnl check bzip2  
	AC_MSG_CHECKING(for BZip2 in default path)
	for i in /usr/local /usr; do
	  if test -r $i/include/bzlib.h; then
	    BZIP_DIR=$i
	    AC_MSG_RESULT(found in $i)
	  fi
	done

  if test -z "$BZIP_DIR"; then
    AC_MSG_RESULT(not found)
    AC_MSG_ERROR(Please reinstall the BZip2 distribution)
  fi

  PHP_ADD_INCLUDE($BZIP_DIR/include)

  PHP_ADD_LIBRARY_WITH_PATH(bz2, $BZIP_DIR/lib, MEMCACHE_SHARED_LIBADD)
  AC_CHECK_LIB(bz2, BZ2_bzerror, [AC_DEFINE(HAVE_BZ2,1,[ ])], [AC_MSG_ERROR(bz2 module requires libbz2 >= 1.0.0)],)

 
  if test "$PHP_MEMCACHE_SESSION" != "no"; then 
	AC_MSG_CHECKING([for session includes])
    session_inc_path=""

    if test -f "$abs_srcdir/include/php/ext/session/php_session.h"; then
      session_inc_path="$abs_srcdir/include/php"
    elif test -f "$abs_srcdir/ext/session/php_session.h"; then
      session_inc_path="$abs_srcdir"
    elif test -f "$phpincludedir/ext/session/php_session.h"; then
      session_inc_path="$phpincludedir"
    else
      for i in php php4 php5 php6; do
        if test -f "$prefix/include/$i/ext/session/php_session.h"; then
          session_inc_path="$prefix/include/$i"
        fi
      done
    fi

    if test "$session_inc_path" = ""; then
      AC_MSG_ERROR([Cannot find php_session.h])
    else
      AC_MSG_RESULT([$session_inc_path])
    fi
  fi

  dnl check igbinary include 
    
  if test "$PHP_MEMCACHED_IGBINARY" != "no"; then
    AC_MSG_CHECKING([for igbinary includes])
    igbinary_inc_path=""

    if test -f "$abs_srcdir/include/php/ext/igbinary/igbinary.h"; then
      igbinary_inc_path="$abs_srcdir/include/php"
    elif test -f "$abs_srcdir/ext/igbinary/igbinary.h"; then
      igbinary_inc_path="$abs_srcdir"
    elif test -f "$phpincludedir/ext/session/igbinary.h"; then
      igbinary_inc_path="$phpincludedir"
    elif test -f "$phpincludedir/ext/igbinary/igbinary.h"; then
      igbinary_inc_path="$phpincludedir"
    else
      for i in php php4 php5 php6; do
        if test -f "$prefix/include/$i/ext/igbinary/igbinary.h"; then
          igbinary_inc_path="$prefix/include/$i"
        fi
      done
    fi

    if test "$igbinary_inc_path" = ""; then
      AC_MSG_ERROR([Cannot find igbinary.h])
    else
      AC_MSG_RESULT([$igbinary_inc_path])
    fi
  fi  
  
  AC_MSG_CHECKING([for memcache igbinary support])
  if test "$PHP_MEMCACHE_IGBINARY" != "no"; then
    AC_MSG_RESULT([enabled])
    AC_DEFINE(HAVE_MEMCACHE_IGBINARY,1,[Whether memcache igbinary serializer is enabled])
    IGBINARY_INCLUDES="-I$igbinary_inc_path"
    ifdef([PHP_ADD_EXTENSION_DEP],
    [
      PHP_ADD_EXTENSION_DEP(memcache, igbinary)
    ])
  else
    IGBINARY_INCLUDES=""
    AC_MSG_RESULT([disabled])
  fi

  AC_MSG_CHECKING([for minilzo library])
  PHP_PARSELIB_DIR="/usr/local/lib64"
  PHP_PARSELIB_INCPATH="/usr/local/include/minilzo"
  if test -f "$PHP_PARSELIB_INCPATH/minilzo.h"; then
    PHP_ADD_LIBRARY_WITH_PATH(minilzo, $PHP_PARSELIB_DIR, MEMCACHE_SHARED_LIBADD)
    PHP_ADD_INCLUDE($PHP_PARSELIB_INCPATH)
    PHP_SUBST(MEMCACHE_SHARED_LIBADD)
  else
    AC_MSG_ERROR([Cannot find parsing minilzo])
  fi

  AC_CHECK_TYPE(ptrdiff_t,long)
  AC_TYPE_SIZE_T
  AC_CHECK_SIZEOF(short)
  AC_CHECK_SIZEOF(int)
  AC_CHECK_SIZEOF(long)
  AC_CHECK_SIZEOF(long long)
  AC_CHECK_SIZEOF(__int64)
  AC_CHECK_SIZEOF(void *)
  AC_CHECK_SIZEOF(size_t)
  AC_CHECK_SIZEOF(ptrdiff_t)
  AC_C_CONST

  PHP_REQUIRE_CXX()
  PHP_ADD_LIBRARY(stdc++, 1, MEMCACHE_SHARED_LIBADD)
  PHP_SUBST(MEMCACHE_SHARED_LIBADD)

  AC_MSG_CHECKING([for memcache session support])
  if test "$PHP_MEMCACHE_SESSION" != "no"; then
    AC_MSG_RESULT([enabled])
    AC_DEFINE(HAVE_MEMCACHE_SESSION,1,[Whether memcache session handler is enabled])
    AC_DEFINE(HAVE_MEMCACHE,1,[Whether you want memcache support])
    PHP_NEW_EXTENSION(memcache, memcache.cpp memcache_queue.c memcache_standard_hash.c memcache_consistent_hash.c memcache_session.c loader.cpp log.cpp parse.cpp, $ext_shared,,-I$session_inc_path -I$abs_srcdir/include -DMINILZO_HAVE_CONFIG_H)
    ifdef([PHP_ADD_EXTENSION_DEP]$abs_srcdir/include ,
    [
      PHP_ADD_EXTENSION_DEP(memcache, session)
    ])					   
  else 
    AC_MSG_RESULT([disabled])
    AC_DEFINE(HAVE_MEMCACHE,1,[Whether you want memcache support])
    PHP_NEW_EXTENSION(memcache, memcache.cpp memcache_queue.c memcache_standard_hash.c memcache_consistent_hash.c, $ext_shared)
    PHP_NEW_EXTENSION(memcache, memcache.cpp memcache_queue.c memcache_standard_hash.c memcache_consistent_hash.cc loader.cpp log.cpp parse.cpp, $ext_shared, -I$abs_srcdir/include -DMINILZO_HAVE_CONFIG_H)
  fi

dnl this is needed to build the extension with phpize and -Wall

  if test "$PHP_DEBUG" = "yes"; then
    CFLAGS="$CFLAGS -Wall"
  fi

fi
