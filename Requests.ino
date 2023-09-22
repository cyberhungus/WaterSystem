void changeValues(AsyncWebServerRequest *request) {
  LED_Yellow();

  Serial.println("changeValuesCalled");
  if (request->hasParam("threshold")) {
    String threshval = request->getParam("threshold")->value();
    Serial.print("CHANGED PARAMETER - threshold: ");
    Serial.println(threshval);
    int newThreshold = threshval.toInt();
    Serial.print("AS INTEGER ");
    Serial.println(newThreshold);
    threshold = newThreshold;
  }
  if (request->hasParam("seqDelay")) {
    String seqdelay = request->getParam("seqDelay")->value();
    Serial.print("CHANGED PARAMETER - seqdelay: ");
    Serial.println(seqdelay);
    int newseqdelay = seqdelay.toInt();
    Serial.print("AS INTEGER ");
    Serial.println(newseqdelay);
    Sequence_Delay = newseqdelay;
  }



  LED_Black();
  request->redirect("/");
}

void delayedPump(AsyncWebServerRequest *request) {
  LED_Yellow();
  int delayint = 3;
  int durationint = 1;

  if (request->hasParam("delay")) {
    String delayval = request->getParam("delay")->value();
    Serial.print("Started Delayed Pump - delay: ");
    Serial.println(delayval);
    delayint = delayval.toInt();
    Serial.print("AS INTEGER ");
    Serial.println(delayint);
  }

  if (request->hasParam("duration")) {
    String durationval = request->getParam("duration")->value();
    Serial.print("Started Delayed Pump - delay: ");
    Serial.println(durationval);
    durationint = durationval.toInt();
    Serial.print("AS INTEGER ");
    Serial.println(durationint);
  }
  DelayedPumpSequence(delayint, durationint);
  LED_Black();
  request->redirect("/");
}
