# -*- Autoconf -*- Process this file with autoconf to produce a configure script.

AC_PREREQ([2.65])
AC_INIT([MPSolve], [3.2.1], [leonardo@robol.it])

MPS_MAJOR_VERSION=3
MPS_MINOR_VERSION=2
MPS_PATCH_VERSION=1

AC_DEFINE(MPS_MAJOR_VERSION, 3, [libmps' major version number])
AC_DEFINE(MPS_MINOR_VERSION, 2, [libmps' minor version number])
AC_DEFINE(MPS_PATCH_VERSION, 1, [libmps' patch version number])

AC_SUBST(MPS_MAJOR_VERSION)
AC_SUBST(MPS_MINOR_VERSION)
AC_SUBST(MPS_PATCH_VERSION)

AC_CONFIG_SRCDIR([Makefile.am])
AC_CONFIG_HEADERS([config.h])

AC_CONFIG_MACRO_DIR([m4])

LT_INIT([win32-dll shared])

AM_INIT_AUTOMAKE([dist-bzip2 -Wno-portability subdir-objects])

m4_ifdef([AM_SILENT_RULES],[AM_SILENT_RULES([yes])])

#
# ===== OPTION PARSING =====
#
# Add options to the configure script so that the user can select the features
# he's interested in. 
#

# Determine CFLAGS based on the options that the user has specified.
# The supported build type are
#
# --without-debug Do not code debug prints (that is not the default)
# 
AC_ARG_ENABLE([debug],
	AS_HELP_STRING([--disable-debug], [Disable debug prints in the code]),
	[enable_debug=$enableval have_debug=$enableval],
	[enable_debug=yes have_debug=yes])
# AC_MSG_RESULT([$enable_debug])

# Select CFLAGS based on debug enabled or not
AS_IF([test x$enable_debug != xno], [
	LIBMPS_CFLAGS="$LIBMPS_CFLAGS -DNICE_DEBUG"
], [ 
	LIBMPS_CFLAGS="$LIBMPS_CFLAGS -DDISABLE_DEBUG"
	AC_DEFINE([DISABLE_DEBUG], 1, [Defined if MPSolve is built without debugging support])
])

# Check if we are building in debug-mode. If that's the case disable the hard-to-debug
# optimization flag of the compiler. 
AC_ARG_ENABLE([debug-build],
         AS_HELP_STRING([--enable-debug-build], 
             [Build mpsolve with debug symbols and disable all the optimizations of the compiler]), 
             [enable_debug_build=$enableval], [enable_debug_build=no]
	 )         

AS_IF([test x$enable_debug_build == xno], [], [
	  # CFLAGS=""
	  # CXXFLAGS=""
          LIBMPS_CFLAGS="$LIBMPS_CFLAGS -O0 -g"
      ]
)

# Check if the user wants to build the QML UI
AC_ARG_ENABLE([qml-ui],
	AS_HELP_STRING([--enable-qml-ui], 
		[Build xmpsolve with the Qml UI]), 
	[enable_qml_ui=$enableval], [enable_qml_ui=no]
	)
AM_CONDITIONAL([BUILD_QML_UI], [test "x$enable_qml_ui" != "xno"])

# Determine if examples are desired. If that's the case check for the library
# that are installed on the system and see which examples can be added. 
AC_ARG_ENABLE([examples],
	AS_HELP_STRING([--disable-examples], [Disable additional examples and modules bundled with MPSolve]),
	[build_examples=$enableval have_examples=$enableval],
	[build_examples=yes have_examples=yes])

# Add another option, enable/disable UI. 
AC_ARG_ENABLE([ui],
	AS_HELP_STRING([--disable-ui], [Don't build the graphical user interface for MPSolve]),
	[build_ui=$enableval have_ui=$enableval],
	[build_ui=yes have_ui=yes])
	
# Determine if the Graphical debugger must be integrated in MPSolve
AC_ARG_ENABLE([graphical-debugger],
    AS_HELP_STRING([--disable-graphical-debugger], [Don't bundle the GTK graphical debugger in MPSolve]),
    [build_graphical_debugger=$enableval have_graphical_debugger=$enableval],
    [build_graphical_debugger=yes have_graphical_debugger=yes])

# Determine if the user wants to build the documentation
AC_ARG_ENABLE([documentation], 
    AS_HELP_STRING([--disable-documentation], [Don't build Doxygen documentation]), 
    [build_doc=$enableval hvae_doc=$enableval],
    [build_doc=yes have_doc=yes])

# Determine if the user wants to build on ARM using hard floating point
AC_ARG_ENABLE([vfpv3],
    AS_HELP_STRING([--enable-vfpv3], [Build with FPU support for ARM using VFPv3-instructions]), 
    [build_hard_float=$enableval], [build_hard_float=no])

AS_IF([test x$build_hard_float = xyes], [
  CFLAGS="$CFLAGS -mfloat-abi=softfp -mfpu=vfpv3-d16"
])

# Check if the user wants to use the standard system malloc even if tcmalloc
# is installed.
AC_ARG_ENABLE([tcmalloc],
        AS_HELP_STRING([--disable-tcmalloc], [Disable the use of the thread caching malloc]),
        [enable_tcmalloc=$enableval have_tcmalloc=$enableval],
        [enable_tcmalloc=yes have_tcmalloc=yes])


#
# ==== LIBRARY DETECTION ==== 
#
# All these sections handle library detection and understanding the system characteristics. 
#


##
## Section 0) This section contains checks for compilers, basic headers files, ...
#

  # Checks for needed compilers: 
  AC_PROG_FC
  AC_PROG_CC
  AC_PROG_CXX
  AC_PROG_CC_C99
  AC_PROG_YACC
  AM_PROG_LEX
  AM_PROG_CC_C_O
  AC_C_INLINE

  # Check for some compiler options that provide significant optimizations
  AX_CHECK_COMPILE_FLAG([-fcx-limited-range],   [ LIBMPS_CFLAGS="$LIBMPS_CFLAGS -fcx-limited-range" ])
  AX_CHECK_COMPILE_FLAG([-fno-math-errno],      [ LIBMPS_CFLAGS="$LIBMPS_CFLAGS -fno-math-errno" ])
  AX_CHECK_COMPILE_FLAG([-fomit-frame-pointer], [ LIBMPS_CFLAGS="$LIBMPS_CFLAGS -fomit-frame-pointer" ])

  # Checks for typedefs, structures, and compiler characteristics.
  AC_HEADER_STDBOOL
  AC_TYPE_SIZE_T

  # Checks for basic library functions. Without these there will be no hope 
  # of compiling MPSolve. 
  AC_FUNC_ERROR_AT_LINE
  AC_CHECK_FUNCS([memset modf pow sqrt strchr])

  ## Subsection 0.1) Check if we are building on Windows and tune the CFLAGS accordingly

    # Check if we are building on Windows, so we can add additional
    # hacks to make MPSolve work there. 
    AC_MSG_CHECKING([for Win32 platform])
     case "$host" in
       *-*-mingw*|*-*-cygwin*)
         platform_win32=yes
       ;;
       *)
         platform_win32=no
       ;;
     esac
    AC_MSG_RESULT([$platform_win32])  

    # This makes detecting Windows real easy...
    if test "$platform_win32" = yes; then
      CFLAGS="$CFLAGS -D__WINDOWS"
      LDFLAGS="$LDFLAGS -mwindows" 

      # In this case check if we have windres compiler
      AC_CHECK_TOOL([WINDRES], [windres])
    fi

    AM_CONDITIONAL([WINDOWS_BUILD], [test "$platform_win32" == "yes"])
    AM_CONDITIONAL([HAVE_WINDRES], [test "$WINDRES" != ""])

  ## Subsection 0.2) Check for the support of the visibily attribute. If this is supported
  ## we can make a lot of routines "private", protecting misuse of libmps and shortening
  ## load times. 

    AC_CACHE_CHECK([for __attribute__((visibility("hidden")))],
      ac_cv_hidden_visibility_attribute, [
      echo 'int __attribute__ ((visibility ("hidden"))) foo (void) { return 1; }' > conftest.c
      ac_cv_hidden_visibility_attribute=no
      if AC_TRY_COMMAND(${CC-cc} -Werror -S conftest.c -o conftest.s 1>&AS_MESSAGE_LOG_FD);
      then
          if grep '\.hidden.*foo' conftest.s >/dev/null;
          then
              ac_cv_hidden_visibility_attribute=yes
          fi
      fi
      rm -f conftest.*
      ])

    if test $ac_cv_hidden_visibility_attribute = yes;
    then
      AC_DEFINE(HAVE_HIDDEN_VISIBILITY_ATTRIBUTE, 1,
            [Define if __attribute__((visibility("hidden"))) is supported.])
    fi


  ## Subsection 0.3) Multithread support. 
    

    ACX_PTHREAD([], [
      AC_MSG_ERROR([Pthread library not found. Please set PTHREAD_CFLAGS and PTHREAD_LIBS correctly for your setup])
    ])
    CC="$PTHREAD_CC"

    # Set the correct PTHREAD flags and, if needed, change the compiler to one that is pthread-enabled.
    AC_SUBST(PTHREAD_CFLAGS)
    AC_SUBST(PTHREAD_LDFLAGS)
    AC_SUBST(PTHREAD_LIBS)
    AC_SUBST(CC)

  ## Subsection 0.3) Check for a way to get the number of cores installed on the system. 
    AC_MSG_CHECKING([if sysconf(_SC_NPROCESSORS_ONLN) is supported])
    AC_COMPILE_IFELSE([
        AC_LANG_SOURCE(
        [[
          #include <unistd.h>
           int main() {
             int cores = sysconf(_SC_NPROCESSORS_ONLN);
             return 0;
           }
        ]]
        )
      ], 
      [have_sysconf=yes], 
      [have_sysconf=no]
    )

    AC_MSG_RESULT([$have_sysconf])
    AS_IF([test "$have_sysconf" == "yes"], [
      AC_DEFINE([HAVE_SYSCONF], [1], [
        Defined if sysconf(_SC_NPROCESSORS_ONLN) is supported]
      )
    ])


  ## Subsection 0.4) Check for a custom malloc implementation with threading support and a good
  ## caching algorithm. 
    AS_IF([test "$have_tcmalloc" == "yes"], [
      AC_CHECK_LIB([tcmalloc_minimal], [malloc])
    ])




## 
## Section 1) Input/Output routines
##

  # The function vsnprintf() is used in input-output.c 
  # to handle the mps_error() outputs in a proper way. 
  AX_FUNC_SNPRINTF

  # Checks for basic header files that should be present on
  # the system. 
  AC_CHECK_HEADERS([stdlib.h string.h unistd.h])

  # getline is used to parse the input files. 
  AC_CHECK_FUNCS(getline)

  # strndup is used in the source, but we provide a fallback
  # implementation if it's not available in the system. 
  AC_CHECK_FUNCS(strndup)


##
## Section 2) Mathematical routines and libaries
##

  # If available, the <complex.h> header will be used since
  # it will probably provide a better (and faster) implementation
  # of floating point complex arithmetic. 
  AC_MSG_CHECKING([if C99 complex.h is supported from the C++ compiler])
  AC_COMPILE_IFELSE([
    AC_LANG_SOURCE([[
      extern "C" {
        #include <complex.h>
      }

      int main() {
        _Complex double a;
        return 0;
      }
    ]])],
    [have_complex_h=yes], 
    [have_complex_h=no]
  )
  AC_MSG_RESULT([$have_complex_h])

  AS_IF([test x$have_complex_h == xyes],  [
    AC_SUBST([MPS_USE_BUILTIN_COMPLEX], [0])
  ], [
    COMPLEX_CFLAGS="-DMPS_USE_BUILTIN_COMPLEX"
    AC_SUBST([MPS_USE_BUILTIN_COMPLEX], [1])
  ])

  # The general C math library. 
  AC_CHECK_LIB([m], [pow])

  ## Subsection 2.1) Check for GMP

    # Add GMP_CFLAGS and GMP_LIBS to CFLAGS and LDFLAGS in case they are set.
    # This is an easy way for the user to override system libgmp or to provide
    # its location if it's not installed system-wide. 
    AS_IF([test -z $GMP_CFLAGS], [], [
      CFLAGS="$GMP_CFLAGS $CFLAGS"
    ])
    AS_IF([test -z $GMP_LDFLAGS], [], [
      LDFLAGS="$GMP_LDFLAGS $LDFLAGS"
    ])
    AS_IF([test -z $GMP_LIBS], [], [
      LDFLAGS="$GMP_LIBS $LDFLAGS"
    ])

    AC_SUBST(GMP_CFLAGS)
    AC_SUBST(GMP_LIBS)

    LIBMPS_CFLAGS="-D_REENTRANT -Wall $COMPLEX_CFLAGS $LIBMPS_CFLAGS"

    AC_CHECK_LIB([gmp], [__gmpz_init2])
    AC_CHECK_LIB([gmpxx], [main])
    AC_CHECK_HEADERS([gmp.h], [mpsolve_found_gmp_headers=yes; break;])

    # Raise an error if GMP is not correctly installed on this system.
    AS_IF([test "x$mpsolve_found_gmp_headers" != "xyes"],
      [AC_MSG_ERROR([Unable to find gmp headers, please install libgmp-dev])])

##
## Section 3) Check support: try to find all the necessary headers and libraries
## that are needed to run the test cases. This way MPSolve will be checked by the
## make check command. 
##

  # Check for the check module, so MPSolve tests can be run.
  PKG_CHECK_MODULES([CHECK], [check >= 0.9.4], [have_check=yes], [have_check=no])
  AM_CONDITIONAL([CHECK], [test x$have_check = xyes])

  AC_SUBST(CHECK_PROGRAMS)
  AC_SUBST(CHECK_CFLAGS)
  AC_SUBST(CHECK_LIBS)



##
## Section 4) Graphical toolkits: MPSolve has some parts that can be optionally compiled if 
## the appropriate GUI toolkits are present. 
##

  ## Subsection 4.1) GTK support for mpsolve -x (live approximation tracking) 

    # Check for the GTK libraries on this system. We can use both GTK 2.0 and GTK 3.0
    have_gtk="1"
    PKG_CHECK_MODULES([GTK], [gtk+-3.0], [], [
        PKG_CHECK_MODULES([GTK], [gtk+-2.0], [], [
            have_gtk="0"
        ])
    ])

    AS_IF([test x$have_gtk = x1], [
        AC_DEFINE([HAVE_GTK], $have_gtk, [Defined if GTK is available])
    ])

    AC_SUBST(GTK_CFLAGS)
    AC_SUBST(GTK_LIBS)

    # Check that we have also the cairo graphic library. This will be already bundled
    # in GTK3 but is not to be taken for granted in GTK2. 
    PKG_CHECK_MODULES([CAIRO], [cairo], [have_cairo="1"], [have_cairo="0"])

    AS_IF([test x$have_cairo = x1], [
       AC_DEFINE([HAVE_CAIRO], 1, [Defined if CAIRO is available])
    ])

    AC_SUBST(CAIRO_CFLAGS)
    AC_SUBST(CAIRO_LIBS)

    AS_IF([test x$have_gtk == x1 && test x$have_graphical_debugger == xyes && test x$have_cairo == x1], [
        AC_DEFINE([HAVE_GRAPHICAL_DEBUGGER], 1, [Defined if the graphical debugger should be bundled in MPSolve])
    ])

##
## Section 5) XMPsolve: the MPSolve graphical user interface. 
##

  # Search qmake and Qt related things, so we can build
  # MPSolve UI. Don't build the UI if the user has manually
  # disabled it.
  QT_PROGRAMS=
  AS_IF([test "$build_ui" == "yes"], [

	# The case QML is simpler since we have to require Qt5. Otherwise try to 
	# test for both Qt4 and Qt5, and choose the best. 
	AS_IF([test x$enable_qml_ui = xyes], [ 
		have_qt4=no
		PKG_CHECK_MODULES(QT5, [ Qt5Core Qt5Gui Qt5Qml Qt5Quick >= 5.1 ], [have_qt5=yes], [have_qt5=no])
		QT5_CFLAGS="$QT5_CFLAGS -DMPS_USE_QML"

		AS_IF([test x$have_qt5 = xno], [AC_MSG_ERROR([Qt >= 5.1 is needed to enable the QML UI.])])
	], [
          PKG_CHECK_MODULES(QT4, [ QtCore QtGui ],   [have_qt4=yes], [have_qt4=no])
          PKG_CHECK_MODULES(QT5, [ Qt5Core Qt5Gui Qt5Widgets ], [have_qt5=yes], [have_qt5=no])
	]) 

          AS_IF([test "$have_qt5" == "no" && test "$have_qt4" == "no"], [
	        # No hope to have the UI, just disable it. 
                AC_MSG_NOTICE([Could not find Qt4 or Qt5, disabling the graphical UI])
                QT_PROGRAMS=""
          ], [
	       QT_PROGRAMS=xmpsolve${EXEEXT}
          ])

          AS_IF([test "$have_qt5" == "yes"], [
	        # Check for needed programs
                AC_CHECK_PROGS([UIC], [uic-qt5 uic])
                AC_CHECK_PROGS([MOC], [moc-qt5 moc])
                AC_CHECK_PROGS([RCC], [rcc-qt5 rcc])

                QT_CFLAGS=$QT5_CFLAGS
                QT_LDFLAGS=$QT5_LDFLAGS
                QT_LIBS=$QT5_LIBS
                QT_VERSION=qt5

                AS_IF([test "$UIC" == "" || test "$MOC" == ""], [
                        AC_MSG_WARN([uic or moc were not found in your path, so Qt5 is not usable])
                        have_qt5=no
                      ])
                ], [])
	
          # Enable Qt4 only if Qt5 was not found, or if the required tools
	  # to build the interface are not available. 
	  AS_IF([test "$have_qt4" == "yes" && test "$have_qt5" == "no"], [
                  AC_CHECK_PROGS([UIC], [uic-qt4 uic])
                  AC_CHECK_PROGS([MOC], [moc-qt4 moc])
                  AC_CHECK_PROGS([RCC], [rcc-qt4 rcc])

                  QT_CFLAGS=$QT4_CFLAGS
                  QT_LDFLAGS=$QT4_LDFLAGS
                  QT_LIBS=$QT4_LIBS
                  QT_VERSION=qt4

                  AS_IF([test "$UIC" == "" || test "$MOC" == ""], [
                    AC_MSG_WARN([uic and/or moc were not found in your path, so Qt4 is not usable])
                      QT_PROGRAMS=""
                  ])
          ]) # End of have_qt4 == yes && have_qt5 == no

          AS_IF([test x"$have_qt4" == x"no" && test x"$have_qt5" == x"no"], [
            QT_PROGRAMS=""
	  ])

	  AS_IF([test "$QT_PROGRAMS" == ""], [
            have_ui=no
            build_ui=no
	  ])
])

AM_CONDITIONAL([HAVE_UI], [test x"$QT_PROGRAMS" != x""])
AC_SUBST(QT_PROGRAMS)
AC_SUBST(QT_CFLAGS)
AC_SUBST(QT_LDFLAGS)
AC_SUBST(QT_LIBS)
AC_SUBST(QT_VERSION)


#
# ===== EXAMPLES ======
#
# This section contains the checks that are needed in case the user wants to build 
# the examples contained in the examples/ directory
#

# Check the existance of mkoctfile to compile octave modules.
AS_IF([test x$build_examples == xyes], [
	AC_CHECK_PROGS([MKOCTFILE], [mkoctfile])
	enable_octave=no
	
	# Detect Octave version. We are not able to build the Octave module
	# if Octave is older than 3.0
	AC_CHECK_PROGS([OCTAVE], [octave])

	# Perform the version check only if Octave is found
	AS_IF([test "$OCTAVE" != ""], [
		AC_MSG_CHECKING([if Octave is newer than 3.0])
		OCTAVE_VERSION=`octave -q -H --eval "disp(OCTAVE_VERSION)"`	
		AX_COMPARE_VERSION([$OCTAVE_VERSION], [ge], [3.0.0], [
				AC_MSG_RESULT([yes])
				
				# If we have Octave that is sufficiently recent, we can enable the Octave module
	        	AS_IF([test "$MKOCTFILE" != ""], [enable_octave=yes])			
			], [
				AC_MSG_RESULT([no])
			])
	], [])
])

AM_CONDITIONAL([BUILD_OCT_FILE], [test "$enable_octave" == "yes"])

# Check for the presence of "mex", the Matlab compiler
# and if it's present, define MATLAB_MODULES_PROGRAMS
# to be mps_roots.mexglx if we are on a 32 bit pc
# and mps_roots.mexglxa64 if we are on a 64 bit machine.
AS_IF([test x$build_examples == xyes], [

	# The following very involved check is needed since mex is also the name
	# of the polish TeX compiler. If both are in path we need to test all the
	# possibilities until we found the _real_ mex compiler. 
	AC_CACHE_CHECK([for a valid mex compiler], [ac_cv_path_MEX], 
	AC_PATH_PROGS_FEATURE_CHECK([MEX], [mex], 
	[[
          mex_is_matlab_output=`$ac_path_MEX -help 2>&1 | grep "MATLAB\|mkoctfile"`
	  mex_is_matlab_retcode=$?
	  if [[ "$mex_is_matlab_retcode" != "0" ]]; then
	    MEX=""
          else
            ac_cv_path_MEX=$ac_path_MEX
	    MEX=$ac_path_MEX
          fi
          ]]))

	# Set the MEX compiler if that has been found
	MEX=$ac_cv_path_MEX
	AC_SUBST(MEX)	

	# If we have find a valid Matlab compiler then add
	# MATLAB to the target of compilation
	enable_matlab=no
	AS_IF([test "$MEX" != ""], [enable_matlab=yes
	  MATLAB_MODULES_PROGRAMS="mps_roots_string mps_roots_double"])
	AC_SUBST(MATLAB_MODULES_PROGRAMS)

	# If we have found the MATLAB compiler, check how the 
	# -output option has to specified. 
	AS_IF([test "$MEX" != ""], [
	AC_CACHE_CHECK([for the correct way to give -output to the mex compiler], 
		[ac_cv_MEX_OUTPUT_FLAG], [
            mex_is_matlab_output=`$MEX -help 2>&1 | grep "MATLAB\|mkoctfile"`
            echo $mex_is_matlab_output | grep mkoctfile > /dev/null
            if [[ $? -eq 0 ]]; then
              ac_cv_MEX_OUTPUT_FLAG="-o"
            else
              ac_cv_MEX_OUTPUT_FLAG="-output"
            fi
	])])

	MEX_OUTPUT_FLAG=$ac_cv_MEX_OUTPUT_FLAG
	AC_SUBST(MEX_OUTPUT_FLAG)

	# If this is the MATLAB mex compiler (and not the GNU/Octave one)
	# we will need to enable also some other flags. 
	AS_IF([test "$MEX_OUTPUT_FLAG" == "-output"], [
            MEXOPTS="-cxx -D__MPS_MATLAB_MODE=1 "
            MEX_OUTPUT_SUFFIX=""
	    MEX_LDFLAGS_PREFIX="LDFLAGS="
        ], [
            MEX_OUTPUT_SUFFIX=".mex"
	    MEX_LDFLAGS_PREFIX=""
        ])

	AC_SUBST(MEXOPTS)
	AC_SUBST(MEX_OUTPUT_SUFFIX)		
	AC_SUBST(MEX_LDFLAGS_PREFIX)
])

AM_CONDITIONAL([BUILD_MATLAB_TOOLBOX], [test "$enable_matlab" == "yes"])


# == PYTHON MODULE ==
have_python=yes
AM_PATH_PYTHON([2.3],,[:])
AC_ARG_VAR([PYTHON_INCLUDE], [Include flags for python, bypassing python-config])
AC_ARG_VAR([PYTHON_CONFIG], [Path to python-config])
AS_IF([test -z "$PYTHON_INCLUDE"], [
  AS_IF([test -z "$PYTHON_CONFIG"], [
    AC_PATH_PROGS([PYTHON_CONFIG],
                  [python$PYTHON_VERSION-config python-config],
                  [no],
                  [`dirname $PYTHON`])
    AS_IF([test "$PYTHON_CONFIG" = no], [
        AC_MSG_WARN([cannot find python-config for $PYTHON.])
	have_python=no
    ], [
        AC_MSG_CHECKING([python include flags])
        PYTHON_INCLUDE=`$PYTHON_CONFIG --includes`
        AC_MSG_RESULT([$PYTHON_INCLUDE])
	
	AX_PYTHON_MODULE([ctypes])

	AS_IF([test "$HAVE_PYMOD_CTYPES" = "no"], [
		    have_python=no
	])
    ])
  ])

])

AM_CONDITIONAL([HAVE_PYTHON], [test "$have_python" == "yes"])
AM_CONDITIONAL([BUILD_PYTHON_MODULE], [test "$have_python" == "yes" && test "$disable_examples" != "xyes"])

# Trick to get the extension of the python module right. 
host_os=`echo "$host" | sed 's/^\([^-]*\)-\([^-]*\)-\(.*\)$/\3/'`
py_libext=so
case "$host_os" in
  cygwin* | mingw* | pw32* | cegcc*)
    py_libext=dll
    ;;
esac

# Check for GTK libraries to enable the example that is given
# in examples/gtk
AS_IF([test x$build_examples == xyes && test x$have_gtk == x1 && test x$have_cairo == x1], [
	ROOTS_OF_UNITY_PROGRAMS=roots_of_unity

	AC_SUBST([GTK_CFLAGS])
	AC_SUBST([GTK_LIBS])
	AC_SUBST([GTK_FOUND])
	AC_SUBST([ROOTS_OF_UNITY_PROGRAMS])
])

AM_CONDITIONAL([BUILD_EXAMPLES], [test x$build_examples = xyes])

#
# ===== MPSOLVE DOCUMENTATION ======
#
# Check that we have doxygen on this sytem. If that's the case, build the documentation for
# MPSolve. 

AS_IF([test x$build_doc == xyes], [
  AC_CHECK_PROGS([DOXYGEN], [doxygen])

  if [test -z "$DOXYGEN"]; then
    AC_MSG_WARN([Doxygen not found, documentation will not be generated])
  else
    AC_CHECK_PROGS([DOT], [dot])
    AC_SUBST(HAVE_DOT)
  fi
])

AM_CONDITIONAL([HAVE_DOXYGEN], [test -n "$DOXYGEN"])


LIBMPS_CFLAGS="$LIBMPS_CFLAGS -D_MPS_PRIVATE"
LIBMPS_LDFLAGS=" $LDFLAGS"

if test "$platform_win32" = yes; then
  LIBMPS_LDFLAGS="$LIBMPS_LDFLAGS -no-undefined -Wl,--export-all-symbols -Wl,--output-def,.libs/libmps-3.dll.def"
fi

AC_SUBST(LIBMPS_LDFLAGS)
AC_SUBST(LIBMPS_CFLAGS)

AC_CONFIG_FILES([Makefile
    include/Makefile
    include/mps/Makefile
    include/mps/private/Makefile
    examples/Makefile
    examples/C/Makefile
    examples/fortran/Makefile
    examples/octave/Makefile
    examples/matlab/Makefile
    examples/gtk/Makefile
    examples/python/Makefile
    examples/python/tests/Makefile
    examples/mandelbrot-solver/Makefile
    examples/mandelbrot-solver/mandelbrot-solver.1
    examples/quadratic-solver/Makefile
    examples/quadratic-solver/quadratic-solver.1
    include/mps/mt.h
    include/mps/version.h
    src/Makefile
    src/mpsolve/Makefile
    src/mpsolve/mpsolve.1
    src/libmps/Makefile
    src/tests/Makefile
    src/xmpsolve/Makefile
    src/xmpsolve/xmpsolve.desktop
    src/xmpsolve/xmpsolve.1
    doc/Makefile
    doc/Doxyfile
    mime/Makefile])

AC_OUTPUT

echo "

MPSolve configuration:

        Source location:	${srcdir}
	Pthread CFLAGS:		${PTHREAD_CFLAGS}
        C compiler:		${CC}
	CFLAGS:			${LIBMPS_CFLAGS}
        LDFLAGS:                ${LIBMPS_LDFLAGS}
	Additional CFLAGS:	${CFLAGS}
        Debug enabled:		$enable_debug
        Check enabled:		$have_check"

# Check Octave module
if [test x$enable_octave = xyes]; then
        echo "	Octave module:		$enable_octave (generates examples/octave/mpsolve.tar.gz)"
else
        echo "	Octave module:		no"
fi

# Check MATLAB (TM) module
if [test x$enable_matlab = xyes]; then
        echo "	MATLAB (tm) module:	$enable_matlab (generates examples/matlab/mpsolve-toolbox.tar.gz)"
else
        echo "	MATLAB (tm) module:	no"
fi

if [test x$build_ui = xyes]; then
	echo "	Graphical UI:		$build_ui"
else
	echo "	Graphical UI:		no"
fi

        echo "	Python module:          $have_python"


echo "
Type 'make' to compile MPSolve, and then make install
to install the binaries, the library and the headers
system-wide.

"
