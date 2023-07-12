#include <Connection.h>

Connection::Connection(const char* ss, const char* pass):
                                        ssid(ss),
                                        password(pass), 
                                        server(80), 
                                        websocket("/ws")
{
    //apIP(8,8,4,4) //TODO used to explain MDNS
}

void Connection::setupAP()
{
    WiFi.softAP(ssid, password);
    ip = WiFi.softAPIP();
    ipaddress = "HUB|" + ip.toString();
    Serial.print("AP IP address: ");
    Serial.println(ip);
}

void Connection::setupWIFI()
{
    WiFi.begin(ssid, password);
    Serial.println("\nConnecting");

    while(WiFi.status() != WL_CONNECTED){
        Serial.print(".");
        delay(100);
    }
    IPAddress ip = WiFi.localIP();
    ipaddress = "HUB|"+ ip.toString();
    Serial.print("IP address: ");
    Serial.println(ipaddress);
}

void Connection::setupServer()
{
    server.addHandler(&websocket);

    server.on("/", HTTP_GET, [this](AsyncWebServerRequest *request){
         request->send(200, "text/plain", this->message); 
    });
    server.onNotFound([](AsyncWebServerRequest *request){
        String nfmsg = "Hello World!\n\n";
        nfmsg += "URI: ";
        nfmsg += request->url();

        request->send(200, "text/plain", nfmsg); 
    });

    server.begin();
}
void Connection::setupWebsocket()
{
    websocket.onEvent([this](AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len){
        if(type == WS_EVT_CONNECT)
        {
            Serial.println("WebSocket client connected");
        } 
        else if(type == WS_EVT_DISCONNECT)
        {
            Serial.println("WebSocket client disconnected");
        } 
        else if(type == WS_EVT_DATA)
        {
            this->handleWebSocketMessage(arg, data, len);
        }
    });
}
void Connection::setupDNSserver()
{
    dnsServer.start(53, "*", ip);
    dnsServer.setErrorReplyCode(DNSReplyCode::ServerFailure);
}
void Connection::setup(void(*hwsm)(void *, uint8_t *, size_t ), bool isAccessPoint)
{
    if(isAccessPoint)
    {
        setupAP();
    }
    else
    {
        setupWIFI();
    }
    setupServer();
    setupWebsocket();
    setupDNSserver();

    handleWebSocketMessage = hwsm;
}

void Connection::update()
{
    dnsServer.processNextRequest();
    websocket.cleanupClients();
}

void Connection::broadcastIP()
{
    udp.beginPacket(UDP_ADRS, UDP_PORT);
    udp.printf(ipaddress.c_str());
    udp.endPacket();
}
void Connection::broadcastMsg(String msg)
{
    message = msg;
    Serial.println(message);
    websocket.textAll(message);
}