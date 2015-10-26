#include "i8254.h"
#include "i8042.h"

.data


.text
.global _kbd_int_handler_asm
_kbd_int_handler_asm:
	xor		%eax,%eax
	inb		$OUT_BUF,%al
	ret

