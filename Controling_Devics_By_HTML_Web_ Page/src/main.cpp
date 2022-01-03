#include <Arduino.h>
#include<ESP8266WiFi.h>

WiFiClient client;
WiFiServer NodeMCU(80);
#define led1 D5
#define led2 D6
String request;
  
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
WiFi.softAP("NodeMCU");
Serial.println("NodeMCU is started");
Serial.println(WiFi.softAPIP());
NodeMCU.begin();
pinMode(led1,OUTPUT);
pinMode(led2,OUTPUT);
digitalWrite(led2,HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
 client = NodeMCU.available();
 if( client == 1){
  request = client.readStringUntil('\n');
  Serial.println(request);
  request.trim();
   if(request == "GET /led1on HTTP/1.1") digitalWrite(led1,HIGH);
   if(request == "GET /led2on HTTP/1.1") digitalWrite(led2,LOW);
   if(request == "GET /led2off HTTP/1.1") digitalWrite(led2,HIGH); 
   if(request == "GET /led1off HTTP/1.1") digitalWrite(led1,LOW);

   //here is the HTML code (which is the response to the client by the Server
             // this is HTML code for the web page  ----->
             client.println("HTTP/1.1 200 OK");
             client.println("Content-Type: text/html");
             client.println("");
             client.println("<!DOCTYPE HYML>");
             client.println("<html>");
             client.println("<head>");
             client.println("c<style>");
             client.println(".B1{background-color:Green;");
              client.println("color:black;");
              client.println("text-align:center;");
              client.println("font-size: 100px;}");
             client.println(".B2{background-color:Red;");
              client.println("color:black;");
              client.println("text-align:center;");
              client.println("font-size: 100px;}");
              client.println("</style>");    
              client.println("</head>");
              client.println("<body>");
              client.println("<h1>\"<span style=\"color:Orange\">Welcome to the </span><span style=\"color:Blue\">My Webpage!</span>\"</h1>");
              client.println("<h2>Here is your  LED Control</h3>");
               client.println("<br> ");
              client.println("<a href=\"\\led1on\"><button class=\"B1\">LED 1 ON</button></a>");
              client.println(" <a href=\"\\led1off\"><button class=\"B2\">LED 1 OFF</button></a>");
               client.println("</br>");
               client.println("<br>");
               client.println("<a href=\"\\led2on\"><button class=\"B1\">LED 2 ON</button> </a>");
               client.println("<a href=\"\\led2off\"><button class=\"B2\">LED 2 OFF</button></a>");
               client.println("</br>");
               client.println("<body>");
               client.println("</html>");       
 } 
}