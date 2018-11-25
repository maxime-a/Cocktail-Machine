#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>  //https://github.com/bbx10/WebServer_tng



WebServer server ( 80 );

const char* ssid     = "ssid";
const char* password = "password";



void setup() 
{
  Serial.begin(9600);

  connectToWifi();

  beginServer();
}

void loop() {
 
 server.handleClient();
 

 delay(1000);
 
}

void connectToWifi()
{
  WiFi.enableSTA(true);
  
  delay(2000);

  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
        delay(500);        
    }
  
}

void beginServer()
{
  server.on ( "/", handleRoot );
  server.begin();
}

void handleRoot(){ 
  if ( server.hasArg("cocktail") ) {
    handleSubmit();
  } else {
    server.send ( 200, "text/html", getPage() );
  }  
}


void handleSubmit() {

  String cocktail;
  cocktail = server.arg("cocktail"); 
  Serial.println(cocktail);    
  server.send ( 200, "text/html", getPage() );
}

String getPage(){
  String page = "<html lang=en-EN><head><meta http-equiv='refresh' content='60'/>";
  page += "<title>ESP32 WebServer - educ8s.tv</title>";
  page += "<style> body { background-color: #fffff; font-family: Arial, Helvetica, Sans-Serif; Color: #000000; }</style>";
  page += "</head><body><h1>ESP32 Cocktail WebServer</h1>";
  page += "<form action='/' method='POST'>";
  page += "<ul><li>Cocktail";
  page += "";
  page += "<INPUT type='text' name='cocktail'>";
  page += "<INPUT type='submit' value='Submit'>";

  page += "</body></html>";
  return page;
}
