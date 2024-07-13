#ifndef _PICO_ESP01_H
#define _PICO_ESP01_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "pico/malloc.h"

#define ESP01_DEFAULT_BAUD_RATE 115273
#define ESP01_DEFAULT_DATA_BITS 8
#define ESP01_DEFAULT_STOP_BITS 1
#define ESP01_DEFAULT_PARITY 0
#define ESP01_DEFAULT_CTS false
#define ESP01_DEFAULT_RTS true
#define ESP01_CMD_LENGTH 256
#define ESP01_RSP_LENGTH 4096
#define ESP01_DEFAULT_TIMEOUT 1000
#define ESP01_EXTENDED_TIMEOUT 10000
#define ESP01_EXTRA_EXTENDED_TIMEOUT 10000

#define AT_QUERY '?'
#define AT_SET '='
#define AT_EXECUTE '\0'

// Generic
#define AT_TEST "AT"                    // [X] Test AT startup.
#define AT_RESET "AT+RST"               // [X] Restart a module.
#define AT_VERSION "AT+GMR"             // [X] Check version information.
#define AT_LIST_COMMANDS "AT+CMD"       // [ ] List all AT commands and types supported in current firmware.
#define AT_SLEEP_CFG "AT+SLEEP"         // [X] Set the sleep mode.
#define AT_DEEP_SLEEP "AT+GSLP"         // [X] Enter Deep-sleep mode.
#define AT_FACTORY_RESET "AT+RESTORE"   // [X] Restore factory default settings of the module.
#define AT_UART_CURRENT "AT+UART_CUR"   // [X] Current UART configuration, not saved in flash.
#define AT_UART_DEFAULT "AT+UART_DEF"   // [ ] Default UART configuration, saved in flash.
#define AT_STORE_MODE "AT+SYSSTORE"     // [~] Query/Set parameter store mode.
#define AT_PROMPT_CFG "AT+SYSMSG"       // [ ] Query/Set System Prompt Information.

// Wifi
#define AT_WIFI_MODE "AT+CWMODE"                    // [ ] Set the Wi-Fi mode (Station/SoftAP/Station+SoftAP).
#define AT_WIFI_STATE "AT+CWSTATE"                  // [ ] Query the Wi-Fi state and Wi-Fi information.
#define AT_WIFI_CONNECT_AP "AT+CWJAP"               // [ ] Connect to an AP.
#define AT_WIFI_DISCONNECT_AP "AT+CWQAP"            // [ ] Disconnect from an AP.
#define AT_WIFI_RECONNECT_CFG "AT+CWRECONNCGF"      // [ ] Query/Set the Wi-Fi reconnecting configuration.
#define AT_WIFI_LIST_APS_CFG "AT+CWLAPOPT"          // [ ] Set the configuration for the command AT+CWLAP.
#define AT_WIFI_LIST_APS "AT+CWLAP"                 // [ ] List available APs.
#define AT_WIFI_SOFT_AP_CFG "AT+CWSAP"              // [ ] Query/Set the configuration of an ESP SoftAP.
#define AT_WIFI_LIST_STATIONS "AT+CWLIF"            // [ ] Obtain IP address of the station that connects to an ESP SoftAP.
#define AT_WIFI_DISCONNECT_STATION "AT+CWQIF"       // [ ] Disconnect stations from an ESP SoftAP.
#define AT_WIFI_DHCP "AT+CWDHCP"                    // [ ] Enable/disable DHCP.
#define AT_WIFI_DHCP_LEASES "AT+CWDHCPS"            // [ ] Query/Set the IP addresses allocated by an ESP SoftAP DHCP server.
#define AT_WIFI_AUTOCONNECT "AT+CWAUTOCONN"         // [ ] Connect to an AP automatically when powered on.
#define AT_WIFI_STATION_MAC "AT+CIPSTAMAC"          // [ ] Query/Set the MAC address of an ESP station.
#define AT_WIFI_AP_MAC "AT+CIPAPMAC"                // [ ] Query/Set the MAC address of an ESP SoftAP.
#define AT_WIFI_STATION_IP "AT+CIPSTA"              // [ ] Query/Set the IP address of an ESP station.
#define AT_WIFI_AP_IP "AT+CIPAP"                    // [ ] Query/Set the IP address of an ESP SoftAP.
#define AT_WIFI_WPS "AT+WPS"                        // [ ] Enable the WPS function.
#define AT_WIFI_MDNS "AT+MDNS"                      // [ ] Configure the mDNS function.
#define AT_WIFI_STATION_HOSTNAME "AT+CWHOSTNAME"    // [ ] Query/Set the host name of an ESP station.
#define AT_WIFI_COUNTRY "AT+CWCOUNTRY"              // [ ] Query/Set the Wi-Fi Country Code.

// IP
#define AT_IP_V6 "AT+CIPV6"                                 // [ ] Enable/disable the network of Internet Protocol Version 6 (IPv6).
#define AT_IP_STATUS "AT+CIPSTATUS"                         // [ ] Obtain the TCP/UDP/SSL connection status and information.
#define AT_IP_DOMAIN "AT+CIPDOMAIN"                         // [ ] Resolve a Domain Name.
#define AT_IP_START "AT+CIPSTART"                           // [ ] Establish TCP connection, UDP transmission, or SSL connection.
#define AT_IP_SEND "AT+CIPSEND"                             // [ ] Send data in the normal transmission mode or Wi-Fi passthrough mode.
#define AT_IP_CLOSE "AT+CIPCLOSE"                           // [ ] Close TCP/UDP/SSL connection.
#define AT_IP_LOCAL_ADDRESS "AT+CIFSR"                      // [ ] Obtain the local IP address and MAC address.
#define AT_IP_MUX_MODE "AT+CIPMUX"                          // [ ] Enable/disable the multiple connections mode.
#define AT_IP_SERVER "AT+CIPSERVER"                         // [ ] Delete/create a TCP/SSL server.
#define AT_IP_SERVER_MAX_CONNECTIONS "AT+CIPSERVERMAXCONN"  // [ ] Query/Set the maximum connections allowed by a server.
#define AT_IP_TX_MODE "AT+CIPMODE"                          // [ ] Query/Set the transmission mode.
#define AT_IP_AUTO_PASSTHROUGH "AT+SAVETRANSLINK"           // [ ] Set whether to enter Wi-Fi passthrough mode on power-up.
#define AT_IP_SERVER_TIMEOUT "AT+CIPSTO"                    // [ ] Query/Set the local TCP Server Timeout.
#define AT_IP_SNTP_CFG "AT+CIPSNTPCFG"                      // [ ] Query/Set the time zone and SNTP server.
#define AT_IP_SNTP "AT+CIPSNTPTIME"                         // [ ] Query the SNTP time.
#define AT_IP_UPGRADE "AT+CIUPDATE"                         // [ ] Upgrade the firmware through Wi-Fi.
#define AT_IP_SSL_LIST_CLIENTS "AT+CIPSSLCCONF"             // [ ] Query/Set SSL clients.
#define AT_IP_SSL_CCN "AT+CIPSSLCCN"                        // [ ] Query/Set the Common Name of the SSL client.
#define AT_IP_SSL_SNI "AT+CIPSSLCSNI"                       // [ ] Query/Set SSL client Server Name Indication (SNI).
#define AT_IP_SSL_ALPN "AT+CIPSSLCALPN"                     // [ ] Query/Set SSL client Application Layer Protocol Negotiation (ALPN).
#define AT_IP_SSL_PSK "AT+CIPSSLCPSK"                       // [ ] Query/Set SSL client Pre-shared Key (PSK).
#define AT_IP_PASSTHROUGH_RECONNECT "AT+CIPRECONNINTV"      // [ ] Query/Set the TCP/UDP/SSL reconnection interval for the Wi-Fi passthrough mode.
#define AT_IP_SOCKET_MODE "AT+CIPRECVMODE"                  // [ ] Query/Set socket receiving mode.
#define AT_IP_SOCKET_DATA "AT+CIPRECVDATA"                  // [ ] Obtain socket data in passive receiving mode.
#define AT_IP_SOCKET_DATA_LENGTH "AT+CIPRECVLEN"            // [ ] Obtain socket data length in passive receiving mode.
#define AT_IP_SOCKET_CFG "AT+CIPTCPOPT"                     // [ ] Query/Set the socket options.
#define AT_IP_PING "AT+PING"                                // [ ] Ping the remote host.
#define AT_IP_DNS "AT+CIPDNS"                               // [ ] Query/Set DNS server information.

// HTTP
#define AT_HTTP_CLIENT "AT+HTTPCLIENT"  // [ ] Send HTTP Client Request.
#define AT_HTTP_SIZE "AT+HTTPGETSIZE"   // [ ] Get HTTP Resource Size.
#define AT_HTTP_POST "AT+HTTPCPOST"     // [ ] Post HTTP data of specified length.

struct esp01_inst {
    uart_inst_t *uart_inst;
    uint baud_rate;
    uint tx_pin;
    uint rx_pin;
} typedef esp01_inst_t;

/*!
 * Initialize a communication with ESP01 device.
 *
 * @param uart_inst UART instance
 * @param baud_rate Communication baud rate. @see ESP01_DEFAULT_BAUD_RATE
 * @param tx_pin TX pin number
 * @param rx_pin RX pin number
 * @return Pointer to the communication instance
 */
esp01_inst_t *esp01_init(uart_inst_t *uart_inst, uint baud_rate, uint tx_pin, uint rx_pin);

/*!
 * Deinitialize a communication with ESP01 device (but doesn't shutdown the device).
 *
 * @param inst Pointer to the communication instance
 */
void esp01_deinit(esp01_inst_t *inst);

/*!
 * Reinitialize a communication with ESP01 device.
 * @note It is useful when the ESP01 is reset (i.e. wakes up from deep sleep).
 *
 * @param inst
 */
void esp01_reinit(esp01_inst_t *inst);

/*!
 * Set the host UART configuration.
 *
 * @param inst Pointer to the communication instance
 * @param data_bits Number of bits of data. 5..8
 * @param stop_bits Number of stop bits 1..2
 * @param parity Parity option
 * @param cts If true enable flow control of TX by clear-to-send input
 * @param rts If true enable assertion of request-to-send output by RX flow control
 */
void esp01_set_host_uart(esp01_inst_t *inst, uint data_bits, uint stop_bits, uart_parity_t parity, bool cts, bool rts);

struct esp01_uart_settings {
    uint baud_rate;
    uint data_bits;
    uint stop_bits;
    uart_parity_t parity;
    bool cts;
    bool rts;
} typedef esp01_uart_settings_t;

/*!
 * Set the host UART configuration from device settings.
 *
 * @param inst Pointer to the communication instance
 * @param uart_set UART device settings
 */
void esp01_set_host_uart_from_device(esp01_inst_t *inst, esp01_uart_settings_t uart_set);

/*!
 * Send a command to the ESP01 device.
 *
 * @param inst Pointer to the communication instance
 * @param timeout_ms Command timeout in ms
 * @param cmd_mode Command mode ('?'/'='/'\0')
 * @param label Command label (AT+...)
 * @param ... Command params (last param must end with \r or \n)
 * @return The device respond
 */
char *esp01_at_cmd(esp01_inst_t *inst, uint timeout_ms, char cmd_mode, char *label, ...);

/*!
 * Check if the response is OK.
 *
 * @param rsp The device respond
 * @return True if the communication is OK, false otherwise
 */
bool esp01_rsp_ok(char *rsp);

/*!
 * Check if the response is OK and free memory.
 *
 * @param rsp The device respond
 * @return True if the communication is OK, false otherwise
 */
bool esp01_rsp_ok_free(char *rsp);

/*!
 * Test communication.
 *
 * @param inst Pointer to the communication instance
 * @return True if the communication is OK, false otherwise
 */
bool esp01_test(esp01_inst_t *inst);

/*!
 * Reset device.
 * 
 * @param inst Pointer to the communication instance
 * @return True if the command was successfully executed, false otherwise
 */
bool esp01_reset(esp01_inst_t *inst);

struct esp01_version {
    char *at;
    char *sdk;
    char *bin;
} typedef esp01_version_t;

/*!
 * Get device firmware version.
 *
 * @param inst Pointer to the communication instance
 * @return True if the command was successfully executed, false otherwise
 */
bool esp01_get_version(esp01_inst_t *inst, esp01_version_t *ver);

enum esp01_sleep_mode {
    DISABLED = 0,
    MODEM_DTIM = 1,
    MODEM_INTERVAL = 3,
    LIGHT = 2,
} typedef esp01_sleep_mode_t;

/*!
 * Get device sleep mode.
 *
 * @param inst Pointer to the communication instance
 * @param mode Pointer to the variable used to store the result
 * @return True if the command was successfully executed, false otherwise
 */
bool esp01_get_sleep_mode(esp01_inst_t *inst, esp01_sleep_mode_t *mode);

/*!
 * Set device sleep mode.
 *
 * @param inst Pointer to the communication instance
 * @param mode New sleep mode
 * @return True if the command was successfully executed, false otherwise
 */
bool esp01_set_sleep_mode(esp01_inst_t *inst, esp01_sleep_mode_t mode);

/*!
 * Trigger device deep sleep.
 *
 * @param inst Pointer to the communication instance
 * @param duration Deep sleep duration in milliseconds
 * @return True if the command was successfully executed, false otherwise
 */
bool esp01_deep_sleep(esp01_inst_t *inst, uint duration);

/*!
 * Restore the device to factory settings.
 *
 * @param inst Pointer to the communication instance
 * @return True if the command was successfully executed, false otherwise
 */
bool esp01_factory_reset(esp01_inst_t *inst);

/*!
 * Get current UART settings.
 *
 * @param inst Pointer to the communication instance
 * @param uart_set Pointer to the variable used to store the result
 * @return True if the command was successfully executed, false otherwise
 */
bool esp01_get_uart_settings(esp01_inst_t *inst, esp01_uart_settings_t *uart_set);

/*!
 * Set current UART settings.
 *
 * @param inst Pointer to the communication instance
 * @param uart_set New UART settings
 * @return True if the command was successfully executed, false otherwise
 */
bool esp01_set_uart_settings(esp01_inst_t *inst, esp01_uart_settings_t uart_set);

/*!
 * Get store mode.
 *
 * @param inst Pointer to the communication instance
 * @param mode Pointer to the variable used to store the result
 * @return True if the command was successfully executed, false otherwise
 */
bool esp01_get_store_mode(esp01_inst_t *inst, bool *mode);

/*!
 * Set store mode.
 *
 * @param inst Pointer to the communication instance
 * @param mode New store mode
 * @return True if the command was successfully executed, false otherwise
 */
bool esp01_set_store_mode(esp01_inst_t *inst, bool mode);

#endif