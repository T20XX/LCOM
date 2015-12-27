#ifndef __SERIAL_H
#define __SERIAL_H

#define COM1_ADDRESS				0x3F8
#define COM1_IRQ						4

//RBR
#define RECEIVER_BUFFER_RBR				0

//THR
#define TRANSMITTER_HOLDING_THR			0

//IER
#define INTERRUPT_ENABLE_IER			1
#define RECEIVED_DATA_INT				0
#define TRANSMITTER_EMPTY_INT			1
#define LINE_STATUS_INT					2

//IIR
#define INTERRUPT_IDENTIFICATION_IIR	2
#define INTERRUPT_STATUS				0
#define INTERRUPT_ORIGIN				1
#define X64_BYTE_FIFO					5
#define FIFO_STATUS						6

int serial_subscribe_int();

int serial_unsubscribe_int();

int serial_interrupt_identification();

char serial_read_char();

int serial_write_char(char temp);

#endif /* __SERIAL_H */
