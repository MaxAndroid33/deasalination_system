#ifndef CONNECTION_H
#define CONNECTION_H

#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebSrv.h>
#include <WiFiUdp.h>
#include <DNSServer.h>


#define UDP_ADRS "239.1.2.3"
#define UDP_PORT 54321

class Connection{
public:
    IPAddress apIP;
    DNSServer dnsServer;
    WiFiUDP udp;
    IPAddress ip;

    AsyncWebServer server;
    AsyncWebSocket websocket;

    
    const char* ssid = "YourAPSSID";
    const char* password = "YourAPPassword";

    // const char* ssid = "Ak";
    // const char* password = "@AK@-12#";

    // const char* ssid = "tripleAAA";
    // const char* password = "12345678900";

    // const char* ssid = "root";
    // const char* password = "maxmax123";


    String message = "Initial message";
    int inputPostion=170;
    unsigned long interval;
    long num;
    String ipaddress;
    
    Connection();
    void setup();
    void update();

    void setupWIFI();
    void setupServer();
    void setupWebsocket();
    void setupDNSserver();//this can be used to access the server with anyname.local address 

    void broadcastIP();
    void broadcastMsg(String msg);
};

#endif