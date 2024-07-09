#include "esp01.h"

#define QUERY '?'
#define SET '='
#define EXECUTE '\0'

// Generic
#define AT_TEST "AT"                    // Test AT startup.
#define AT_RESET "AT+RST"               // Restart a module.
#define AT_VERSION "AT+GMR"             // Check version information.
#define AT_LIST_COMMANDS "AT+CMD"       // List all AT commands and types supported in current firmware.
#define AT_SLEEP "AT+SLEEP"             // Set the sleep mode.
#define AT_DEEP_SLEEP "AT+GSLP"         // Enter Deep-sleep mode.
#define AT_ECHO "ATE"                   // Configure AT commands echoing.
#define AT_FACTORY_RESET "AT+RESTORE"   // Restore factory default settings of the module.
#define AT_UART_CURRENT "AT+UART_CUR"   // Current UART configuration, not saved in flash.
#define AT_UART_DEFAULT "AT+UART_DEF"   // Default UART configuration, saved in flash.
#define AT_STORE_MODE "AT+SYSSTORE"     // Query/Set parameter store mode.
#define AT_PROMPT_CFG "AT+SYSMSG"       // Query/Set System Prompt Information.

// Wifi
#define AT_WIFI_MODE "AT+CWMODE"                    // Set the Wi-Fi mode (Station/SoftAP/Station+SoftAP).
#define AT_WIFI_STATE "AT+CWSTATE"                  // Query the Wi-Fi state and Wi-Fi information.
#define AT_WIFI_CONNECT_AP "AT+CWJAP"               // Connect to an AP.
#define AT_WIFI_DISCONNECT_AP "AT+CWQAP"            // Disconnect from an AP.
#define AT_WIFI_RECONNECT_CFG "AT+CWRECONNCGF"      // Query/Set the Wi-Fi reconnecting configuration.
#define AT_WIFI_LIST_APS_CFG "AT+CWLAPOPT"          // Set the configuration for the command AT+CWLAP.
#define AT_WIFI_LIST_APS "AT+CWLAP"                 // List available APs.
#define AT_WIFI_SOFT_AP_CFG "AT+CWSAP"              // Query/Set the configuration of an ESP SoftAP.
#define AT_WIFI_LIST_STATIONS "AT+CWLIF"            // Obtain IP address of the station that connects to an ESP SoftAP.
#define AT_WIFI_DISCONNECT_STATION "AT+CWQIF"       // Disconnect stations from an ESP SoftAP.
#define AT_WIFI_DHCP "AT+CWDHCP"                    // Enable/disable DHCP.
#define AT_WIFI_DHCP_LEASES "AT+CWDHCPS"            // Query/Set the IP addresses allocated by an ESP SoftAP DHCP server.
#define AT_WIFI_AUTOCONNECT "AT+CWAUTOCONN"         // Connect to an AP automatically when powered on.
#define AT_WIFI_STATION_MAC "AT+CIPSTAMAC"          // Query/Set the MAC address of an ESP station.
#define AT_WIFI_AP_MAC "AT+CIPAPMAC"                // Query/Set the MAC address of an ESP SoftAP.
#define AT_WIFI_STATION_IP "AT+CIPSTA"              // Query/Set the IP address of an ESP station.
#define AT_WIFI_AP_IP "AT+CIPAP"                    // Query/Set the IP address of an ESP SoftAP.
#define AT_WIFI_WPS "AT+WPS"                        // Enable the WPS function.
#define AT_WIFI_MDNS "AT+MDNS"                      // Configure the mDNS function.
#define AT_WIFI_STATION_HOSTNAME "AT+CWHOSTNAME"    // Query/Set the host name of an ESP station.
#define AT_WIFI_COUNTRY "AT+CWCOUNTRY"              // Query/Set the Wi-Fi Country Code.

// IP
#define AT_IP_V6 "AT+CIPV6"                                 // Enable/disable the network of Internet Protocol Version 6 (IPv6).
#define AT_IP_STATUS "AT+CIPSTATUS"                         // Obtain the TCP/UDP/SSL connection status and information.
#define AT_IP_DOMAIN "AT+CIPDOMAIN"                         // Resolve a Domain Name.
#define AT_IP_START "AT+CIPSTART"                           // Establish TCP connection, UDP transmission, or SSL connection.
#define AT_IP_SEND "AT+CIPSEND"                             // Send data in the normal transmission mode or Wi-Fi passthrough mode.
#define AT_IP_CLOSE "AT+CIPCLOSE"                           // Close TCP/UDP/SSL connection.
#define AT_IP_LOCAL_ADDRESS "AT+CIFSR"                      // Obtain the local IP address and MAC address.
#define AT_IP_MUX_MODE "AT+CIPMUX"                          // Enable/disable the multiple connections mode.
#define AT_IP_SERVER "AT+CIPSERVER"                         // Delete/create a TCP/SSL server.
#define AT_IP_SERVER_MAX_CONNECTIONS "AT+CIPSERVERMAXCONN"  // Query/Set the maximum connections allowed by a server.
#define AT_IP_TX_MODE "AT+CIPMODE"                          // Query/Set the transmission mode.
#define AT_IP_AUTO_PASSTHROUGH "AT+SAVETRANSLINK"           // Set whether to enter Wi-Fi passthrough mode on power-up.
#define AT_IP_SERVER_TIMEOUT "AT+CIPSTO"                    // Query/Set the local TCP Server Timeout.
#define AT_IP_SNTP_CFG "AT+CIPSNTPCFG"                      // Query/Set the time zone and SNTP server.
#define AT_IP_SNTP "AT+CIPSNTPTIME"                         // Query the SNTP time.
#define AT_IP_UPGRADE "AT+CIUPDATE"                         // Upgrade the firmware through Wi-Fi.
#define AT_IP_SSL_LIST_CLIENTS "AT+CIPSSLCCONF"             // Query/Set SSL clients.
#define AT_IP_SSL_CCN "AT+CIPSSLCCN"                        // Query/Set the Common Name of the SSL client.
#define AT_IP_SSL_SNI "AT+CIPSSLCSNI"                       // Query/Set SSL client Server Name Indication (SNI).
#define AT_IP_SSL_ALPN "AT+CIPSSLCALPN"                     // Query/Set SSL client Application Layer Protocol Negotiation (ALPN).
#define AT_IP_SSL_PSK "AT+CIPSSLCPSK"                       // Query/Set SSL client Pre-shared Key (PSK).
#define AT_IP_PASSTHROUGH_RECONNECT "AT+CIPRECONNINTV"      // Query/Set the TCP/UDP/SSL reconnection interval for the Wi-Fi passthrough mode.
#define AT_IP_SOCKET_MODE "AT+CIPRECVMODE"                  // Query/Set socket receiving mode.
#define AT_IP_SOCKET_DATA "AT+CIPRECVDATA"                  // Obtain socket data in passive receiving mode.
#define AT_IP_SOCKET_DATA_LENGTH "AT+CIPRECVLEN"            // Obtain socket data length in passive receiving mode.
#define AT_IP_SOCKET_CFG "AT+CIPTCPOPT"                     // Query/Set the socket options.
#define AT_IP_PING "AT+PING"                                // Ping the remote host.
#define AT_IP_DNS "AT+CIPDNS"                               // Query/Set DNS server information.

// HTTP
#define AT_HTTP_CLIENT "AT+HTTPCLIENT"  // Send HTTP Client Request.
#define AT_HTTP_SIZE "AT+HTTPGETSIZE"   // Get HTTP Resource Size.
#define AT_HTTP_POST "AT+HTTPCPOST"     // Post HTTP data of specified length.

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