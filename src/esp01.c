#include "esp01.h"

#define ESP01_DRIVER_DEBUG


esp01_inst_t *esp01_init(uart_inst_t *uart_inst, uint baud_rate, uint tx_pin, uint rx_pin) {
    esp01_inst_t *inst = malloc(sizeof(esp01_inst_t));

    gpio_set_function(tx_pin, GPIO_FUNC_UART);
    gpio_set_function(rx_pin, GPIO_FUNC_UART);

    inst->tx_pin = tx_pin;
    inst->rx_pin = rx_pin;

    // Setup UART communication with default configuration
    inst->uart_inst = uart_inst;
    inst->uart_settings.baud_rate = baud_rate;
    esp01_reinit(inst);

    return inst;
}

void esp01_deinit(esp01_inst_t *inst) {
    uart_deinit(inst->uart_inst);

    gpio_deinit(inst->tx_pin);
    gpio_deinit(inst->rx_pin);

    free(inst);
}

void esp01_reinit(esp01_inst_t *inst) {
    inst->uart_settings.baud_rate = uart_init(inst->uart_inst, inst->uart_settings.baud_rate);
    uart_set_format(inst->uart_inst, ESP01_DEFAULT_DATA_BITS, ESP01_DEFAULT_STOP_BITS, ESP01_DEFAULT_PARITY);
    uart_set_hw_flow(inst->uart_inst, ESP01_DEFAULT_CTS, ESP01_DEFAULT_RTS);
    uart_set_translate_crlf(inst->uart_inst, false);
}

esp01_uart_settings_t esp01_get_host_uart(esp01_inst_t *inst) {
    return inst->uart_settings;
}

void esp01_set_host_uart(esp01_inst_t *inst, esp01_uart_settings_t uart_set) {
    if (inst->uart_settings.baud_rate != uart_set.baud_rate) uart_set_baudrate(inst->uart_inst, uart_set.baud_rate);
    uart_set_hw_flow(inst->uart_inst, uart_set.cts, uart_set.rts);
    uart_set_format(inst->uart_inst, uart_set.data_bits, uart_set.stop_bits, uart_set.parity);
}

char *esp01_at_cmd(esp01_inst_t *inst, uint timeout_ms, char cmd_mode, char *label, ...) {
    // Allocate memory for the command
    char *o_cmd = malloc(ESP01_CMD_LENGTH + 1);
    char *cmd = o_cmd;
    *cmd = '\0';

    // Concatenate the label of the command
    strcpy(cmd, label);
    cmd += strlen(label);

    // Concatenate the command mode
    if (cmd_mode != '\0') {
        *cmd = cmd_mode;
        cmd++;
        *cmd = '\0';
    }

    va_list args;
    va_start(args, label);

    bool flag = true;
    while (flag) {
        // Return if the command overflows
        if (cmd - o_cmd >= ESP01_CMD_LENGTH) {
            return NULL;
        }

        // Get param from varargs and concatenate until it reaches \r or \n
        char *param = va_arg(args, char*);
        char c;
        for (int i = 0; i < strnlen(param, ESP01_CMD_LENGTH); i++) {
            c = param[i];
            if (c == '\r' || c == '\n') {
                *cmd = '\n';
                cmd++;
                *cmd = '\0';
                flag = false;
                break;
            } else {
                *cmd = c;
                cmd++;
                *cmd = '\0';
            }
        }
    }

    va_end(args);

#ifdef ESP01_DRIVER_DEBUG
    printf(">%s", o_cmd);
#endif

    // Send the command if possible
    if (uart_is_writable(inst->uart_inst)) {
        // Allocate memory for the device response
        char *o_rsp = malloc(ESP01_RSP_LENGTH + 1);
        char *rsp = o_rsp;
        *rsp = '\0';

        // Send command
        for (char *c = o_cmd; c < cmd; c++) {
            if (*c == '\n') {
                uart_putc(inst->uart_inst, '\r');
            }
            uart_putc(inst->uart_inst, *c);
        }

        // Concatenate the response until it reaches a termination word (OK/ERROR)
        while (uart_is_readable_within_us(inst->uart_inst, timeout_ms * 1000)) {
            *rsp = uart_getc(inst->uart_inst);
            if (*rsp == '\0' || *rsp == '\r') {
                continue;
            }
#ifdef ESP01_DRIVER_DEBUG
            putchar(*rsp);
#endif
            rsp++;

            // Return if the response overflows
            if (rsp - o_rsp >= ESP01_RSP_LENGTH) {
#ifdef ESP01_DRIVER_DEBUG
                printf("Response overflow!\n");
#endif
                return NULL;
            }

            *rsp = '\0';

            // Check for termination word
            if (*(rsp - 1) == '\n') {
                if (strstr(o_rsp, o_cmd) != NULL) {
                    rsp = o_rsp;
                    *rsp = '\0';
                } else if ((strstr(o_rsp, "\nOK\n") != NULL) || (strstr(o_rsp, "\nERROR\n") != NULL)) {
                    // Reallocate to free memory
                    return realloc(o_rsp, rsp - o_rsp + 1);
                }
            }
        }
#ifdef ESP01_DRIVER_DEBUG
        printf("Timeout!\n");
#endif
        return NULL;
    } else {
#ifdef ESP01_DRIVER_DEBUG
        printf("UART not writeable!\n");
#endif
        return NULL;
    }
}

bool esp01_rsp_ok(char *rsp) {
    // Return if it's the null pointer
    if (rsp == NULL) {
        return false;
    }

    // Check for OK
    if (strstr(rsp, "\nOK\n") != NULL) {
        return true;
    }

    // Fallback: return false (ERROR/malformed response)
    return false;
}

bool esp01_rsp_ok_free(char *rsp) {
    // Return if it's the null pointer
    if (rsp == NULL) {
        free(rsp);
        return false;
    }

    // Check for OK
    if (strstr(rsp, "\nOK\n") != NULL) {
        free(rsp);
        return true;
    }

    // Fallback: return false (ERROR/malformed response)
    free(rsp);
    return false;
}

// Basic

bool esp01_test(esp01_inst_t *inst) {
    char *rsp = esp01_at_cmd(inst, ESP01_DEFAULT_TIMEOUT, AT_EXECUTE, AT_TEST, "\n");
    return esp01_rsp_ok_free(rsp);
}

bool esp01_reset(esp01_inst_t *inst) {
    char *rsp = esp01_at_cmd(inst, ESP01_DEFAULT_TIMEOUT, AT_EXECUTE, AT_RESET, "\n");
    return esp01_rsp_ok_free(rsp);
}

bool esp01_get_version(esp01_inst_t *inst, esp01_version_t *ver) {
    char *o_rsp = esp01_at_cmd(inst, ESP01_DEFAULT_TIMEOUT, AT_EXECUTE, AT_VERSION, "\n");
    char *rsp = o_rsp;

    if (esp01_rsp_ok(rsp)) {
        ver->at = malloc(ESP01_CMD_LENGTH + 1);
        ver->sdk = malloc(ESP01_CMD_LENGTH + 1);
        ver->bin = malloc(ESP01_CMD_LENGTH + 1);

        *(ver->at) = *(ver->sdk) = *(ver->bin) = '\0';

        sscanf(rsp, "AT version:%[^\n]", ver->at);
        rsp = strstr(rsp, "\n") + 1;

        sscanf(rsp, "SDK version:%[^\n]", ver->sdk);
        rsp = strstr(rsp, "\n") + 1;

        if (strstr(rsp, "\n") != NULL) {
            rsp = strstr(rsp, "\n") + 1;
        }

        sscanf(rsp, "Bin version:%[^\n]", ver->bin);

        free(o_rsp);
        return true;
    } else {
        free(o_rsp);
        return false;
    }
}

bool esp01_get_sleep_mode(esp01_inst_t *inst, esp01_sleep_mode_t *mode) {
    char *rsp = esp01_at_cmd(inst, ESP01_DEFAULT_TIMEOUT, AT_QUERY, AT_SLEEP_CFG, "\n");

    if (esp01_rsp_ok(rsp)) {
        int m;

        sscanf(rsp, "+SLEEP:%d", &m);

        *mode = m;

        free(rsp);
        return true;
    } else {
        free(rsp);
        return false;
    }
}

bool esp01_set_sleep_mode(esp01_inst_t *inst, esp01_sleep_mode_t mode) {
    char n[2];

    sprintf(n, "%d", mode);

    char *rsp = esp01_at_cmd(inst, ESP01_DEFAULT_TIMEOUT, AT_SET, AT_SLEEP_CFG, n, "\n");
    return esp01_rsp_ok_free(rsp);
}

bool esp01_deep_sleep(esp01_inst_t *inst, uint duration) {
    char cmd[ESP01_CMD_LENGTH + 1];
    sprintf(cmd, "%u", duration);

    char *rsp = esp01_at_cmd(inst, ESP01_DEFAULT_TIMEOUT, AT_SET, AT_DEEP_SLEEP, cmd, "\n");
    return esp01_rsp_ok_free(rsp);
}

bool esp01_factory_reset(esp01_inst_t *inst) {
    char *rsp = esp01_at_cmd(inst, ESP01_DEFAULT_TIMEOUT, AT_EXECUTE, AT_FACTORY_RESET, "\n");
    return esp01_rsp_ok_free(rsp);
}

bool esp01_get_uart_settings(esp01_inst_t *inst, esp01_uart_settings_t *uart_set, bool current) {
    char *rsp = esp01_at_cmd(inst, ESP01_DEFAULT_TIMEOUT, AT_QUERY, current ? AT_UART_CURRENT : AT_UART_DEFAULT, "\n");

    if (esp01_rsp_ok(rsp)) {
        uint parity;
        uint flow_control;

        sscanf(rsp, current ? "+UART_CUR:%u,%u,%u,%u,%u" : "+UART_DEF:%u,%u,%u,%u,%u", &(uart_set->baud_rate),
               &(uart_set->data_bits), &(uart_set->stop_bits),
               &parity, &flow_control);

        free(rsp);

        switch (parity) {
            case 0:
                uart_set->parity = UART_PARITY_NONE;
                break;
            case 1:
                uart_set->parity = UART_PARITY_ODD;
                break;
            case 2:
                uart_set->parity = UART_PARITY_EVEN;
                break;
            default:
                printf("%u", parity);
                return false;
        }

        switch (flow_control) {
            case 0:
                uart_set->rts = false;
                uart_set->cts = false;
                break;
            case 1:
                uart_set->rts = true;
                uart_set->cts = false;
                break;
            case 2:
                uart_set->rts = false;
                uart_set->cts = true;
                break;
            case 3:
                uart_set->rts = true;
                uart_set->cts = true;
                break;
            default:
                return false;
        }

        return true;
    } else {
        free(rsp);
        return false;
    }
}

bool esp01_set_uart_settings(esp01_inst_t *inst, esp01_uart_settings_t uart_set, bool current) {
    uint parity;
    switch (uart_set.parity) {
        case UART_PARITY_NONE:
            parity = 0;
            break;
        case UART_PARITY_ODD:
            parity = 1;
            break;
        case UART_PARITY_EVEN:
            parity = 2;
            break;
        default:
            return false;
    }

    uint flow_control = 0;
    if (uart_set.rts) {
        flow_control += 1;
    }
    if (uart_set.cts) {
        flow_control += 2;
    }

    char cmd[ESP01_CMD_LENGTH + 1];
    sprintf(cmd, "%u,%u,%u,%u,%u", uart_set.baud_rate, uart_set.data_bits, uart_set.stop_bits, parity, flow_control);

    char *rsp = esp01_at_cmd(inst, ESP01_DEFAULT_TIMEOUT, AT_SET, current ? AT_UART_CURRENT : AT_UART_DEFAULT, cmd,
                             "\n");
    return esp01_rsp_ok_free(rsp);
}

bool esp01_get_store_mode(esp01_inst_t *inst, bool *mode) {
    char *rsp = esp01_at_cmd(inst, ESP01_DEFAULT_TIMEOUT, AT_QUERY, AT_STORE_MODE, "\n");

    if (esp01_rsp_ok(rsp)) {
        uint m;
        sscanf(rsp, "+SYSSTORE:%u", &m);
        *mode = m;
        free(rsp);
        return true;
    } else {
        free(rsp);
        return false;
    }
}

bool esp01_set_store_mode(esp01_inst_t *inst, bool mode) {
    char cmd[2];
    sprintf(cmd, "%u", mode);

    char *rsp = esp01_at_cmd(inst, ESP01_DEFAULT_TIMEOUT, AT_SET, AT_STORE_MODE, cmd, "\n");
    return esp01_rsp_ok_free(rsp);
}

// Wifi

bool esp01_get_wifi_mode(esp01_inst_t *inst, esp01_wifi_mode_t *mode) {
    char *rsp = esp01_at_cmd(inst, ESP01_DEFAULT_TIMEOUT, AT_QUERY, AT_WIFI_MODE, "\n");

    if (esp01_rsp_ok(rsp)) {
        int m;

        sscanf(rsp, "+CWMODE:%d", &m);

        *mode = m;

        free(rsp);
        return true;
    } else {
        free(rsp);
        return false;
    }
}

bool esp01_set_wifi_mode(esp01_inst_t *inst, esp01_wifi_mode_t mode) {
    char n[2];

    sprintf(n, "%d", mode);

    char *rsp = esp01_at_cmd(inst, ESP01_DEFAULT_TIMEOUT, AT_SET, AT_WIFI_MODE, n, "\n");
    return esp01_rsp_ok_free(rsp);
}

bool esp01_get_wifi_state(esp01_inst_t *inst, esp01_wifi_properties_t *state) {
    char *rsp = esp01_at_cmd(inst, ESP01_DEFAULT_TIMEOUT, AT_QUERY, AT_WIFI_STATE, "\n");

    if (esp01_rsp_ok(rsp)) {
        int s;
        *(state->ssid) = '\0';

        sscanf(rsp, "+CWSTATE:%d,\"%[^\"]\"", &s, state->ssid);

        state->state = s;

        free(rsp);
        return true;
    } else {
        free(rsp);
        return false;
    }
}

bool esp01_get_wifi_connection(esp01_inst_t *inst, esp01_connection_properties_t *properties) {
    char *rsp = esp01_at_cmd(inst, ESP01_DEFAULT_TIMEOUT, AT_QUERY, AT_WIFI_STATION_CONNECT, "\n");

    if (esp01_rsp_ok(rsp)) {

        int rtn = sscanf(rsp, "+CWJAP:\"%[^\"]\",\"%[^\"]\",%d,%d,%d,%d,%d,%d,%d", properties->ssid, properties->mac,
               &(properties->channel), &(properties->rssi), &(properties->pci_auth),
               &(properties->reconnection_interval), &(properties->listen_interval), &(properties->scan_mode),
               &(properties->pmf));

        free(rsp);
        if (rtn == 0) {
            return false;
        } else {
            return true;
        }
    } else {
        free(rsp);
        return false;
    }
}

bool esp01_wifi_connect(esp01_inst_t *inst, esp01_connection_properties_t properties, esp01_wifi_error_t *error_code) {
    char cmd[ESP01_CMD_LENGTH + 1];

    sprintf(cmd, "\"%s\",\"%s\"", properties.ssid, properties.password);

    if (strlen(properties.mac) != 0) {
        sprintf(cmd, "%s,%s", cmd, properties.mac);
    } else {
        sprintf(cmd, "%s,", cmd);
    }

    if (properties.pci_auth != ESP01_UNDEFINED) {
        sprintf(cmd, "%s,%d", cmd, properties.pci_auth);
    } else {
        sprintf(cmd, "%s,", cmd);
    }

    if (properties.reconnection_interval != ESP01_UNDEFINED) {
        sprintf(cmd, "%s,%d", cmd, properties.reconnection_interval);
    } else {
        sprintf(cmd, "%s,", cmd);
    }

    if (properties.listen_interval != ESP01_UNDEFINED) {
        sprintf(cmd, "%s,%d", cmd, properties.listen_interval);
    } else {
        sprintf(cmd, "%s,", cmd);
    }

    if (properties.scan_mode != ESP01_UNDEFINED) {
        sprintf(cmd, "%s,%d", cmd, properties.scan_mode);
    } else {
        sprintf(cmd, "%s,", cmd);
    }

    if (properties.jap_timeout != ESP01_UNDEFINED) {
        sprintf(cmd, "%s,%d", cmd, properties.jap_timeout);
    } else {
        sprintf(cmd, "%s,", cmd);
    }

    if (properties.pmf != ESP01_UNDEFINED) {
        sprintf(cmd, "%s,%d", cmd, properties.pmf);
    } else {
        sprintf(cmd, "%s,", cmd);
    }

    char *rsp = esp01_at_cmd(inst, ESP01_EXTRA_EXTENDED_TIMEOUT, AT_SET, AT_WIFI_STATION_CONNECT, cmd, "\n");

    if (esp01_rsp_ok(rsp)) {
        free(rsp);
        return true;
    } else {
        int code;
        sscanf(rsp, "+CWJAP:%d", &code);

        *error_code = code;

        free(rsp);
        return false;
    }
}

