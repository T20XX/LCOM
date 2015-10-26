#include "i8042.h"

.global _kbd_int_handler_asm

.text
_kbd_int_handler_asm:
	xor		%eax,%eax
loop:
	inb		$STAT_REG, %al
	and		$OBF, %eax
	jz		loop
	inb		$OUT_BUF, %al
	ret

