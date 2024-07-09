#include "esp01.h"

#define QUERY '?'
#define SET '='
#define EXECUTE '\0'

// -- Private functions --

/*!
 * Send a command to the ESP01 chip.
 *
 * @param inst pointer to the communication instance
 * @param timeout_ms command timeout in ms
 * @param label command label (AT+...)
 * @param cmd_mode command mode ('?'/'='/'\0')
 * @param ... command params (last param must contain \n)
 * @return the respond
 */
char *at_cmd(esp01_inst_t *inst, uint timeout_ms, char *label, char cmd_mode, ...) {
    // Allocate memory for the command
    char *o_cmd = malloc(ESP01_CMD_LENGTH);
    char *cmd = o_cmd;
    *cmd = '\0';

    // Concatenate the label of the command
    strcat(cmd, label);
    cmd += strlen(label) + 1;

    // Concatenate the command mode
    if (cmd_mode != '\0') {
        *cmd = cmd_mode;
        cmd++;
    }

    va_list args;
    va_start(args, label);

    bool flag = true;
    while (flag) {
        // Return if the command overflows
        if (cmd - o_cmd >= ESP01_CMD_LENGTH) {
            return "\0CMD_OVERFLOW_0";
        }

        // Get param from varargs and concatenate until it reaches a \n
        char *param = va_arg(args, char*);
        for (int i = 0; i < strnlen(param, ESP01_CMD_LENGTH); i++) {
            *cmd = param[i];
            if (*cmd == '\n') {
                cmd++;
                flag = false;
                break;
            }
            cmd++;
        }

        // Return if the command overflows
        if (cmd - o_cmd >= ESP01_RSP_LENGTH) return "\0CMD_OVERFLOW_1";
        *cmd = '\0';
    }

    va_end(args);

    // Send the command if possible
    if (uart_is_writable(inst->uart_inst)) {
        for (int i = 0; i < strnlen(cmd, ESP01_CMD_LENGTH); i++) {
            uart_putc(inst->uart_inst, cmd[i]);
        }

        // Allocate memory for the chip response
        char *o_rsp = malloc(ESP01_RSP_LENGTH);
        char *rsp = o_rsp;
        *rsp = '\0';

        // Concatenate the response until it reaches a termination word (OK/ERROR)
        while (uart_is_readable_within_us(inst->uart_inst, timeout_ms * 1000)) {
            *rsp = uart_getc(inst->uart_inst);
            rsp++;

            // Return if the response overflows
            if (rsp - o_rsp >= ESP01_RSP_LENGTH) return "\0RSP_OVERFLOW";
            *rsp = '\0';

            // Check for termination word
            if (*(rsp - 1) == '\n') {
                if ((rsp - o_rsp >= 4 && strcmp(rsp - 4, "\nOK\n") == 0) || (rsp - o_rsp >= 7 && strcmp(rsp - 4, "\nERROR\n") == 0)) {
                    // Reallocate to free memory
                    return realloc(o_rsp, strlen(o_rsp) + 1);
                }
            }
        }
    }

    return NULL;
}

/*!
 * Check if the response is OK
 *
 * @param o_rsp
 * @return
 */
bool is_rsp_ok(char *o_rsp) {
    // Return if it's the null pointer
    if (o_rsp == NULL) return false;

    uint len = strlen(o_rsp);
    char *rsp = o_rsp + len;

    // Return if the len is zero
    if (len == 0) return false;

    // Check for OK
    if ((len >= 4 && strcmp(rsp - 4, "\nOK\n") == 0)) {
        return true;
    }

    // Fallback: return false (ERROR/malformed response)
    return false;
}

// -- Public functions --

esp01_inst_t *esp01_init(uart_inst_t *uart_inst, uint baud_rate, uint tx_pin, uint rx_pin) {
    esp01_inst_t *inst = malloc(sizeof(esp01_inst_t));

    gpio_set_function(tx_pin, GPIO_FUNC_UART);
    gpio_set_function(rx_pin, GPIO_FUNC_UART);

    inst->tx_pin = tx_pin;
    inst->rx_pin = rx_pin;

    inst->uart_inst = uart_inst;
    inst->baud_rate = uart_init(uart_inst, baud_rate);

    return inst;
}

void esp01_deinit(esp01_inst_t *inst) {
    uart_deinit(inst->uart_inst);

    gpio_deinit(inst->tx_pin);
    gpio_deinit(inst->rx_pin);

    free(inst);
}

bool esp01_test(esp01_inst_t *inst) {
    char* rsp = at_cmd(inst, ESP01_DEFAULT_TIMEOUT, AT_TEST, EXECUTE);

    return is_rsp_ok(rsp);
}