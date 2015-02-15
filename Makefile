CC=gcc
WGDB= -ggdb
CFLAGS= -std=c99 -Wall $(WGDB)
LIBREADLINE= -lreadline
#WREADLINE=`cat main.h|sed -e 's/\/\/.*//'|grep READLINE |cut -d ' ' -f2`
ifeq ($(WREADLINE),no)
LIBREADLINE=
$(shell echo "#undef HAVE_LIBREADLINE">config.h)
else
$(shell echo "">config.h)
endif
LIBS= -lm $(LIBREADLINE) 
OBJECTS= main.o env.o repl.o eval.o object.o print.o read.o stream.o
DIST_SOURCES= *.c *.h Makefile README.md *.man
TARGET=yscm
PNAME=Yoda-Scheme
VERSION=`cat main.h|sed -e 's/\/\/.*//'|grep VERSION |cut -d '"' -f2`
BINDIR=/usr/local/bin
MANDIR=/usr/local/share/man/man1
DIST_DIR=$(PNAME)
MANSRC=${PNAME}.man
MANTARGET1=${PNAME}.1
MANTARGET2=${TARGET}.1
SHELL=/bin/bash


all: msg $(TARGET)

.PHONY: clean install uninstall dist msg

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) $(LIBS) -o $(TARGET)

%.o : %.c
	$(CC) $(CFLAGS) -c $<

msg:
	@echo "making ${PNAME} >>> ${TARGET}"
	@echo "type 'make help' for help"

clean:
	rm -f *.o
	rm -f $(TARGET)
	rm -rf ./${DIST_DIR}/*
	if [ -d ${DIST_DIR} ]; then rmdir ${DIST_DIR}; fi

install:
	mkdir -p ${DESTDIR}${BINDIR}
	cp -p ${TARGET} ${DESTDIR}${BINDIR}/${TARGET}
	chmod 555 ${DESTDIR}${BINDIR}/${TARGET}
	mkdir -p ${DESTDIR}${MANDIR}
	cp -p ${MANSRC} ${DESTDIR}${MANDIR}/${MANTARGET1}
	cp -p ${MANSRC} ${DESTDIR}${MANDIR}/${MANTARGET2}
	chmod 644 ${DESTDIR}${MANDIR}/${MANTARGET1}
	chmod 644 ${DESTDIR}${MANDIR}/${MANTARGET2}

uninstall:
	rm -f ${DESTDIR}${BINDIR}/${TARGET}
	rm -f ${DESTDIR}${MANDIR}/${MANTARGET1}
	rm -f ${DESTDIR}${MANDIR}/${MANTARGET2}

dist:
	mkdir ${DIST_DIR}
	for i in $(DIST_SOURCES); do cp $$i ${DIST_DIR}/; done;
	COPYFILE_DISABLE=1 tar -cvzf ${PNAME}-${VERSION}.tar.gz ${DIST_DIR}/
	rm -rf ./${DIST_DIR}/*
	rmdir ${DIST_DIR}

help:
	@ echo "Yoda-Scheme Makefile"
	@ echo "The following targets are available"
	@ echo "make            ~ build scheme executable"
	@ echo "make help       ~ print this message"
	@ echo "make install    ~ install everything"
	@ echo "make uninstall  ~ uninstall everything"
	@ echo "make clean      ~ remove any temporary files"
	@ echo "make dist       ~ make a dist .tar.gz tarball package"
	@ echo " "
	@ echo "build options:"
	@ echo "make WREADLINE=no  ~ will build scheme without libreadline"
