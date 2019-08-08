# $Id: Makefile,v 1.16 2019-08-01 15:33:38-07 - - $

MKFILE     = Makefile
DEPFILE    = ${MKFILE}.dep
NOINCL     = ci clean spotless
NEEDINCL   = ${filter ${NOINCL}, ${MAKECMDGOALS}}
GMAKE      = ${MAKE} --no-print-directory

GPPWARN    = -Wall -Wextra -Wpedantic -Wshadow -Wold-style-cast
GPPOPTS    = ${GPPWARN} -fdiagnostics-color=never
COMPILECPP = g++ -std=gnu++17 -g -O0 ${GPPOPTS}
MAKEDEPCPP = g++ -std=gnu++17 -MM ${GPPOPTS}
MESALIB    = /usr/local/android-sdk/emulator/lib64/gles_mesa
#LINKOPTS   = -Wl,-rpath,${MESALIB}
LINKOPTS   =
UTILBIN    = /afs/cats.ucsc.edu/courses/cmps109-wm/bin

MODULES    = graphics interp rgbcolor shape debug util main
CPPSOURCE  = $(wildcard ${MODULES:=.cpp})
GENFILES   = colors.cppgen
MODFILES   = ${foreach MOD, ${MODULES}, ${MOD}.h ${MOD}.tcc ${MOD}.cpp}
SOURCES    = ${wildcard ${MODFILES}}
OTHERS     = mk-colors.perl ${MKFILE} ${DEPFILE}
ALLSOURCES = ${SOURCES} ${OTHERS}
EXECBIN    = gdraw
OBJECTS    = ${CPPSOURCE:.cpp=.o}
LINKLIBS   = -lGL -lGLU -lglut -ldrm -lm
LISTING     = Listing.ps

all : ${EXECBIN}

${EXECBIN} : ${OBJECTS}
	${COMPILECPP} ${LINKOPTS} -o $@ ${OBJECTS} ${LINKLIBS}

%.o : %.cpp
	${COMPILECPP} -c $<
	- ${UTILBIN}/checksource $<
	- ${UTILBIN}/cpplint.py.perl $<

colors.cppgen: mk-colors.perl
	mk-colors.perl >colors.cppgen

ci : ${ALLSOURCES}
	${UTILBIN}/cid + ${ALLSOURCES}
	- ${UTILBIN}/checksource ${ALLSOURCES}

lis : ${ALLSOURCES}
	- pkill -g 0 gv || true
	${UTILBIN}/mkpspdf ${LISTING} ${ALLSOURCES}

clean :
	- rm ${OBJECTS} ${DEPFILE} core ${GENFILES}

spotless : clean
	- rm ${EXECBIN} ${LISTING} ${LISTING:.ps=.pdf}


dep : ${CPPSOURCE} ${GENFILES}
	@ echo "# ${DEPFILE} created `LC_TIME=C date`" >${DEPFILE}
	${MAKEDEPCPP} ${CPPSOURCE} >>${DEPFILE}

${DEPFILE} :
	@ touch ${DEPFILE}
	${GMAKE} dep

again :
	${GMAKE} spotless dep ci all lis

ifeq (${NEEDINCL}, )
include ${DEPFILE}
endif

