void changeValues(AsyncWebServerRequest *request) {
  String message = "No values set! ";
  
  if (request->hasParam("red")) {
    String redV = request->getParam("name")->value();
    message += "Red: " + redV + "\n";
  }
  
  if (request->hasParam("age")) {
    String age = request->getParam("age")->value();
    message += "Age: " + age + "\n";
  }
  

  request->send(200, "text/plain", message);
}
