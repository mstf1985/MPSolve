if BUILD_EXAMPLES

#
# FORTRAN library
#
lib_LTLIBRARIES = libmps-fortran.la
libmps_fortran_la_FCFLAGS  = -I${top_srcdir}/include -I${top_builddir}/include -fPIC
libmps_fortran_la_CFLAGS  = -I${top_srcdir}/include -I${top_builddir}/include -fPIC
libmps_fortran_la_SOURCES = mps_impl.c 
libmps_fortran_la_LIBADD = ${top_builddir}/src/libmps/libmps.la
libmps_fortran_la_LDFLAGS = \
	-version-info 0:1:0 \
	-no-undefined

# include_HEADERS = mps.mod

# CLEANFILES = *.mod

#
# Simple example to check if the library works OK
#
if WINDOWS_BUILD
	noinst_PROGRAMS = roots_of_unity
	roots_of_unity_SOURCES = roots_of_unity.f90 dummy.cpp
	roots_of_unity_LDADD = libmps-fortran.la -lgfortran
else
	noinst_PROGRAMS = roots_of_unity
	roots_of_unity_SOURCES = roots_of_unity.f90
	roots_of_unity_LDADD = libmps-fortran.la 
endif

endif
