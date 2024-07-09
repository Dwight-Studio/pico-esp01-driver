#ifndef ESP01_H
#define ESP01_H

#include <stdio.h>

#include "pico/stdlib.h"
#include "hardware/uart.h"

#define ESP01_DEFAULT_BAUD_RATE 115200
#define ESP01_MAX_CMD_LENGTH 256

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

struct esp01_inst {
    bool active;
    uart_inst_t* uart_inst;
    uint baud_rate;

} typedef esp01_inst_t;

/*!
 * Initiate a communication with ESP01 chip.
 *
 * @param uart_inst UART instance
 * @param baud_rate communication baud rate. @see ESP01_DEFAULT_BAUD_RATE
 * @param tx_pin TX pin number
 * @param rx_pin RX pin number
 * @return communication instance
 */
esp01_inst_t esp01_init(uart_inst_t *uart_inst, uint baud_rate, uint tx_pin, uint rx_pin);

#endif