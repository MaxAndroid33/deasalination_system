#include <Connection.h>

Connection::Connection():apIP(8,8,4,4), server(80), websocket("/ws"){

}
void Connection::setupWIFI(){
    // WiFi.begin(ssid, password);
    // Serial.println("\nConnecting");

    // while(WiFi.status() != WL_CONNECTED){
    //     Serial.print(".");
    //     delay(100);
    // }
    // IPAddress ip = WiFi.localIP();
    // ipaddress = "HUB|"+ ip.toString();
    // Serial.print("IP address: ");
    // Serial.println(ipaddress);    
    
    WiFi.softAP(ssid, password);
    ip = WiFi.softAPIP();
    ipaddress = "HUB|"+ ip.toString();
    Serial.print("AP IP address: ");
    Serial.println(ip);    
}

void Connection::setupServer(){
    server.addHandler(&websocket);
    String test = "hello";
    server.on("/", HTTP_GET, [this](AsyncWebServerRequest *request){
        request->send(200, "text/plain", this->message);
  
    });
     server.on("/pos", HTTP_GET, [this](AsyncWebServerRequest *request){
        
        inputPostion = request->getParam("pos")->value().toInt();
        request->send(200, "text/plain", "OK");
  
    });
    server.onNotFound([](AsyncWebServerRequest *request) {
        String nfmsg = "Hello World!\n\n";
        nfmsg += "URI: ";
        nfmsg += request->url();

        request->send(200, "text/plain", nfmsg);
    });

    server.begin();
}
void Connection::setupWebsocket(){
    websocket.onEvent([](AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len){
        if(type == WS_EVT_CONNECT){
            Serial.println("WebSocket client connected");
        } else if(type == WS_EVT_DISCONNECT){
            Serial.println("WebSocket client disconnected");
        }
    });
}
void Connection::setupDNSserver(){
    dnsServer.start(53, "*", ip);
    dnsServer.setErrorReplyCode(DNSReplyCode::ServerFailure);
    interval = millis();
}
void Connection::setup(){
    setupWIFI();
    setupServer();
    setupWebsocket();
    setupDNSserver();

    interval = millis();
}

void Connection::update(){
    dnsServer.processNextRequest();
    websocket.cleanupClients();
}

void Connection::broadcastIP(){
    udp.beginPacket(UDP_ADRS, UDP_PORT);
    udp.printf(ipaddress.c_str());
    udp.endPacket();
}
void Connection::broadcastMsg(String msg){
    message = msg;
    Serial.println(message);
    websocket.textAll(message);
}