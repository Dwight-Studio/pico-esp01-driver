#ifndef _PICO_ESP01_H
#define _PICO_ESP01_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "pico/malloc.h"

#define ESP01_DEFAULT_BAUD_RATE 115200
#define ESP01_CMD_LENGTH 256
#define ESP01_RSP_LENGTH 4096
#define ESP01_DEFAULT_TIMEOUT 1000
#define ESP01_EXTENDED_TIMEOUT 10000
#define ESP01_EXTRA_EXTENDED_TIMEOUT 10000

struct esp01_inst {
    uart_inst_t *uart_inst;
    uint baud_rate;
    uint tx_pin;
    uint rx_pin;
} typedef esp01_inst_t;

/*!
 * Initiate a communication with ESP01 chip.
 *
 * @param uart_inst UART instance
 * @param baud_rate communication baud rate. @see ESP01_DEFAULT_BAUD_RATE
 * @param tx_pin TX pin number
 * @param rx_pin RX pin number
 * @return pointer to the communication instance
 */
esp01_inst_t *esp01_init(uart_inst_t *uart_inst, uint baud_rate, uint tx_pin, uint rx_pin);

/*!
 * Deinitiate a communication with ESP01 chip (but doesn't shutdown the chip).
 *
 * @param inst pointer to the communication instance
 */
void esp01_deinit(esp01_inst_t *inst);

/*!
 * Test communication
 *
 * @param inst
 * @return true if the communication is OK, false otherwise
 */
bool esp01_test(esp01_inst_t *inst);

#endif