#ifndef __USART_H__
#define __USART_H__


/**
 * @brief Configure USART3
 * @return none
 *
 * Configure USART3:
 * 	PD8 as AF7 (USART3_TX) mode push/pull high speed
 * 	USART3 transmitter enable
 * 	1 bit stop
 * 	no parity check
 * 	baud rate 9600
 */
void serial_init();

void serial_putc(char c);

void serial_puts(const char *string);

uint8_t serial_getc();

#endif
