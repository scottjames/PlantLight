//
// A simple server implementation showing how to:
//  * serve static messages
//  * read GET and POST parameters
//  * handle missing pages / 404s
//

AsyncWebServer server(80);
const char* PARAM_MESSAGE = "message";

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

// call AFTER wifi_setup
void web_setup() {

  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(200, "text/plain",
                  String(compiledDate) + " " + String(compiledTime)
                  + " plantlight up: "
                  + String(millis() / 1000)
                  + " secs"
                 );
    sendMessage("sent root");
  });

  server.on("/heap", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(200, "text/plain", String(ESP.getFreeHeap()));
    sendMessage("getFreeHeap: " + String(ESP.getFreeHeap()) );
  });


  server.on("/off", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(200, "text/plain", String("led off"));
    led_shutdown();
    sendMessage("led off");
  });


  server.on("/on", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(200, "text/plain", String("led on"));
    led_setup();
    sendMessage("led on");
  });

  // Send a GET request to <IP>/get?message=<message>
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest * request) {
    String message;
    if (request->hasParam(PARAM_MESSAGE)) {
      message = request->getParam(PARAM_MESSAGE)->value();
    } else {
      message = "No message sent";
    }
    request->send(200, "text/plain", "Hello, GET: " + message);
    sendMessage("set GET param: " + message);
  });

  // Send a POST request to <IP>/post with a form field message set to <message>
  server.on("/post", HTTP_POST, [](AsyncWebServerRequest * request) {
    String message;
    if (request->hasParam(PARAM_MESSAGE, true)) {
      message = request->getParam(PARAM_MESSAGE, true)->value();
    } else {
      message = "No message sent";
    }
    request->send(200, "text/plain", "Hello, POST: " + message);
    sendMessage("set POST param: " + message);
  });
  server.onNotFound(notFound);
  server.begin();
  MDNS.addService("http", "tcp", 80);

  Serial.println("web setup done");
}
