#ifndef CONNECTION_H
#define CONNECTION_H

#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <WiFiUdp.h>
#include <DNSServer.h>
#include <Esp.h>

#define UDP_ADRS "239.1.2.3"
#define UDP_PORT 54321

class Connection{
private:
    DNSServer dnsServer;
    WiFiUDP udp;
    IPAddress ip;

    AsyncWebServer server;
    AsyncWebSocket websocket;
    void (*handleWebSocketMessage) (void *, uint8_t *, size_t ) ;

    const char* ssid;
    const char* password;
    String index;

    void setupAP();
    void setupWIFI();
    void setupServer();
    void setupWebsocket();
    void setupDNSserver(); // this can be used to access the server with anyname.local address

public:
    String message = "Initial message";
    unsigned long interval;
    String ipaddress;
    
    Connection(const char*, const char*);
    void setup(void(*)(void *, uint8_t *, size_t ), bool = true);
    void update();
    void resetEsp();

    void broadcastIP();
    void broadcastMsg(String msg);
};

#endif