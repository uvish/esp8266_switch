
#include <ESP8266WiFi.h>
 
const char* ssid = "Atul ";
const char* password = "9793065906";
 
int Pump = 13; // GPIO13---D7 of NodeMCU
int value = LOW;
WiFiServer server(80);
 
void setup() {
  Serial.begin(115200);
  delay(10);
 
  pinMode(Pump, OUTPUT);    //D7 as output
  digitalWrite(Pump, LOW);  //Deactivate Pump
 
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
  // Start the server
  server.begin();
  Serial.println("Server started");
 
  // Print the IP address on serial monitor
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");    //URL IP to be typed in mobile/desktop browser
  Serial.print(WiFi.localIP());
  Serial.println("/");
 
}
 
void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
 
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
 
  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
 
  // Match the request
 
  
  if (request.indexOf("/Pump=ON") != -1)  {
    digitalWrite(Pump, HIGH);
    value = HIGH;
  }
  if (request.indexOf("/Pump=OFF") != -1)  {
    digitalWrite(Pump, LOW);
    value = LOW;
  }
 
// Set Pump according to the request
//digitalWrite(Pump, value);
 
  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html lang=\"en\"><head> <meta charset=\"UTF-8\"> <meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\"> <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"> <link rel=\"preconnect\" href=\"https://fonts.googleapis.com\"> <link rel=\"preconnect\" href=\"https://fonts.gstatic.com\" crossorigin> <link href=\"https://fonts.googleapis.com/css2?family=Roboto:wght@300;400&display=swap\" rel=\"stylesheet\"> <title>Pump</title> <style>*{margin:0;padding:0}*,:after,:before{box-sizing:border-box}.container{height:100vh;width:100vw}.half{height:50%;width:100%}.upper{display:flex;justify-content:center;align-items:center}.btn{height:5rem;width:5rem;border:none;border-radius:1rem}.btn_on{margin-bottom:5px;margin-right:5px;box-shadow:5px 5px #888}.btn_off{margin-bottom:10px;margin-right:10px;box-shadow:10px 10px #888}.status_on{display:flex;font-family:Roboto,sans-serif;font-size:12rem;color:#90ee90}.status_off{display:flex;font-family:Roboto,sans-serif;font-size:12rem;color:#f08080}.status_btn{color:#d3d3d3}.lower{background:#d3d3d3;display:flex;justify-content:center;align-items:center}</style></head><body><div class=\"container\">");
  client.println(" <div class=\"upper half\">");
  if(value == HIGH) {
    client.print("<h1 class=\"status_on\">ON</h1>");
    client.println("<div class=\"lower half\">");
     client.println("<a href=\"/Pump=OFF\"><button class=\"btn btn_off\"><h1 class=\"status_btn\">OFF</h1></button></a>");
  } else {
    client.print("<h1 class=\"status_off\">OFF</h1>");
     client.println("<div class=\"lower half\">");
    client.println("<a href=\"/Pump=ON\"><button class=\"btn btn_on\"><h1 class=\"status_btn\">ON</h1></button></a>");
  } 
  client.println("</div></div></body></html>");
  delay(1);
  Serial.println("Client disonnected");
  Serial.println(""); 
}
