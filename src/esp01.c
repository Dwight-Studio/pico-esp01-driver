#include "esp01.h"



esp01_inst_t esp01_init(uart_inst_t *uart_inst, uint baud_rate, uint tx_pin, uint rx_pin) {
    esp01_inst_t inst;

    gpio_set_function(tx_pin, GPIO_FUNC_UART);
    gpio_set_function(rx_pin, GPIO_FUNC_UART);

    inst.uart_inst = uart_inst;
    inst.baud_rate = uart_init(uart_inst, baud_rate);

    return inst;
}
