# Makefile for the lab3 driver (based on that of hello).

COMPILER_TYPE= gnu

CC=gcc

PROG=	lab3
SRCS=	lab3.c test3.c kbd.c timer.c kbd_asm.S

CCFLAGS= -Wall

DPADD+=	${LIBDRIVER} ${LIBSYS}
LDADD+= -ldriver -lsys

LDFLAGS+= -L .

MAN=

.include <bsd.prog.mk>
.include <bsd.gcc.mk>
