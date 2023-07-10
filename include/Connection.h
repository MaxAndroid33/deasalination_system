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
private:
    DNSServer dnsServer;
    WiFiUDP udp;
    IPAddress ip;

    AsyncWebServer server;
    AsyncWebSocket websocket;

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
    int inputPosition = 170; //TODO implement a better a way
    int pumpState = 1;
    unsigned long interval;
    String ipaddress;
    
    Connection(const char*, const char*);
    void setup(bool isAccessPoint = true);
    void update();

    void broadcastIP();
    void broadcastMsg(String msg);
};

#endif