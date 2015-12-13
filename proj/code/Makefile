# Makefile for the Battle Tetris Project

COMPILER_TYPE= gnu

CC=gcc

PROG=	proj
SRCS=	proj.c vbe.c video_gr.c timer.c read_xpm.c kbd.c logic.c menu.c mouse.c piece.c rtc.c serial.c game.c bitmap.c handler.c

CCFLAGS= -Wall

DPADD+=	${LIBDRIVER} ${LIBSYS}
LDADD+= -llm -ldriver -lsys

LDFLAGS+= -L .

MAN=

.include <bsd.prog.mk>
.include <bsd.gcc.mk>
