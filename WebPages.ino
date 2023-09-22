String MainPage() {
  LED_Black();
  String sHTML;
  sHTML = "<!doctype html>";
  sHTML += "<html>";
  sHTML += "<html lang=\"de\">";
  /***************** head ****************/
  sHTML += "<head>";
  //sHTML += "<meta http-equiv=\"refresh\" content=\"5\">";
  sHTML += "<meta charset=\"utf-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">";
  sHTML += "<title>WasserSystem</title>";
  //  sHTML += "<style>h1{display: flex; flex-flow: row wrap; justify-content: center;} </style>";
  //  sHTML += "<style>h1{ color: green;}</style>";
  //  sHTML += "<style>h2{display: flex; flex-flow: row wrap; justify-content: center;} </style>";
  //  sHTML += "<style>h2{ color: blue;}</style>";
  //  sHTML += "<style>h5{display: flex; flex-flow: row wrap; justify-content: center;} </style>";
  //  sHTML += "<style>p{display: flex; flex-flow: row wrap; justify-content: center; margin-bottom: 3%;} </style>";
  //  sHTML += "<style>p1{display: flex; flex-flow: row wrap; justify-content: center; margin-bottom: 0%;} </style>";
  //  sHTML += "<style>p2{display: flex; flex-flow: row wrap; justify-content: center; margin-bottom: 0%;} </style>";
  sHTML += "</head>";
  sHTML += "<html><body><h1>Automatisches Wassersystem by MobilDigital </h1></body></html>";

  sHTML += "<p>";
  sHTML += "Feuchtigkeitssensor liest: ";
  sHTML += "<div id =\"moistureElement\">" + String(moisture) + "</div>";
  sHTML += "</p>";

  sHTML += "<p>";
  sHTML += "Grenzwert ist: ";
  sHTML += "<div id =\"thresholdElement\">" + String(threshold) + "</div>";
  sHTML += "</p>";

  sHTML += "<p>";
  sHTML += "Füllstand ";
  sHTML +=  "<div id =\"percentageElement\">" + String(percentage) + "</div>";
  sHTML += "%";
  sHTML += "</p>";

  sHTML += "<form action=\"/pumpTest\" method=\"get\">";
  sHTML += "<input type=\"submit\" value=\"Pumpzyklus\">";
  sHTML += "</form>";




  sHTML += "<div id=\'manualpumpdiv\'></div>";
  sHTML += "<hr>";
  
  sHTML += "<p>";
  sHTML += "Vor automatischer Kalibrierung Sensor in feuchte Erde stecken." ;
  sHTML += "Trennen Sie den Pumpmotor vor der Kalibrierung."; 
  sHTML += "</p>";
  sHTML += "<form action=\"/autoCalibration\" method=\"get\">";
  sHTML += "<input type=\"submit\" value=\"System kalibrieren\">";
  sHTML += "</form>";
  sHTML += "<div id=\'autocalDiv\'></div>";
   sHTML += "<hr>"; 
  sHTML += "<form action=\"/valueChange\" method=\"get\">";
  sHTML +=  "<input type=\"range\" min=\"50\" max=\"10000\" id=\"seqdelayslide\" value=";
  sHTML += Sequence_Delay;
  sHTML += " class=\"slider\" name=\"seqDelay\">";
  sHTML += "<input type=\"submit\" value=\"Pumpdauer einstellen\" >";
  sHTML += "</form>";
  sHTML += "<div id=\'seqdelaydiv\'>5</div>";
 sHTML += "<hr>"; 
  sHTML += "<form action=\"/valueChange\" method=\"get\">";
  sHTML +=  "<input type=\"range\" min=\"100\" max=\"10000\" id=\"threshslide\" value=";
  sHTML += threshold;
  sHTML += " class=\"slider\" name=\"threshold\">";
  sHTML += "<input type=\"submit\" value=\"Grenzwert einstellen\" >";
  sHTML += "</form>";
  sHTML += "<div id=\'threshslidediv\'>5</div>";
  sHTML += "<hr>";
  sHTML += "<form action=\"/delayedPump\" method=\"get\">";
  sHTML +=  "<input type=\"range\" min=\"1\" max=\"100\" id=\"delayslider\" value=";
  sHTML += "5";
  sHTML += " class=\"slider\" name=\"delay\">";
  sHTML += "<div id=\'delayslidediv\'>5</div>";
  sHTML +=  "<input type=\"range\" min=\"1\" max=\"10\" id=\"durationslider\" value=";
  sHTML += "3";
  sHTML += " class=\"slider\" name=\"duration\">";
  sHTML += "<div id=\'durationslidediv\'>5</div>";
  sHTML += "<input type=\"submit\" value=\"Verzögerung (Sek.) + Pumpdauer (Sek.) einstellen\">";
  sHTML += "</form>";

  sHTML += "<div id=\'manualpumpdiv\'></div>";

  sHTML += "<div id=\'delayedpumpdiv\'></div>";

  sHTML += "<script>";
  sHTML += "console.log(\"JAVASCRIPT LOADED\");";
  sHTML += " const slider = document.getElementById(\'threshslide\');";
  sHTML += " const sliderdelay = document.getElementById(\'delayslider\');";
  sHTML += " const sliderduration = document.getElementById(\'durationslider\');";
    sHTML += " const seqdelayslide = document.getElementById(\'seqdelayslide\');";
  sHTML += "console.log(slider);";
  sHTML += "const sliderValueElement = document.getElementById(\'threshslidediv\');";
  sHTML += "const sliderDelayElement = document.getElementById(\'delayslidediv\');";
  sHTML += "const sliderDurationElement = document.getElementById(\'durationslidediv\');";
  sHTML += "console.log(sliderValueElement);";
  sHTML += "sliderValueElement.textContent = slider.value;";
  sHTML += "sliderDelayElement.textContent = sliderdelay.value+ \" Sek. verzögert\";";
  sHTML += "sliderDurationElement.textContent = sliderduration.value + \" Sek. Pumpdauer\";";
  sHTML += "slider.addEventListener(\'mouseup\', function() {sliderValueElement.textContent = slider.value;  console.log(\"SLIDER MOVED\"); });";
  sHTML += "sliderdelay.addEventListener(\'mouseup\', function() {sliderDelayElement.textContent = sliderdelay.value+ \" Sek. verzögert\";  console.log(\"SLIDER MOVED\"); });";
  sHTML += "sliderduration.addEventListener(\'mouseup\', function() {sliderDurationElement.textContent = sliderduration.value + \" Sek. Pumpdauer\";  console.log(\"SLIDER MOVED\"); });";
 
  sHTML += "seqdelayslide.addEventListener(\'mouseup\', function() {seqdelaydiv.textContent = seqdelayslide.value + \" Sek. Pumpdauer\";  console.log(\"SLIDER MOVED\"); });";
  sHTML += "console.log(\"INPUT LISTENER ADDED\");";
  sHTML += "</script>";

  //lädt alle 1000 sekunden (interval) die addresse moisture, diese gibt den messwert
  //des feuchtigkeitssensors zurück
  //request element erstellen, dannn einen callback anlegen, dann request durchführen.
  //der wert wird im oben erstellen div
  sHTML +=  "<script>";
  sHTML += "setInterval(function() {getData();}," + String(interv) + ");";
  sHTML += "function getData() {";
  sHTML += "var xhttp = new XMLHttpRequest();";
  sHTML += "xhttp.onreadystatechange = function() {";
  sHTML += "if (this.readyState == 4 && this.status == 200) {";
  sHTML += "document.getElementById(\"moistureElement\").innerHTML = ";
  sHTML += "this.responseText; ";
  sHTML += "}  }; xhttp.open(\"GET\", \"moisture\", true); ";
  sHTML += "xhttp.send();}";
  sHTML += "</script> ";

  //das gleiche für manuellen pumpstatus
  sHTML +=  "<script>";
  sHTML += "setInterval(function() {getData2();}," + String(interv) + ");";
  sHTML += "function getData2() {";
  sHTML += "var xhttp = new XMLHttpRequest();";
  sHTML += "xhttp.onreadystatechange = function() {";
  sHTML += "if (this.readyState == 4 && this.status == 200) {";
  sHTML += "document.getElementById(\"manualpumpdiv\").innerHTML = ";
  sHTML += "this.responseText; ";
  sHTML += "}  }; xhttp.open(\"GET\", \"manualPumpActive\", true); ";
  sHTML += "xhttp.send();}";
  sHTML += "</script> ";

  //das gleiche für verzögerten pumpstatus
  sHTML +=  "<script>";
  sHTML += "setInterval(function() {getData3();}," + String(interv) + ");";
  sHTML += "function getData3() {";
  sHTML += "var xhttp = new XMLHttpRequest();";
  sHTML += "xhttp.onreadystatechange = function() {";
  sHTML += "if (this.readyState == 4 && this.status == 200) {";
  sHTML += "document.getElementById(\"delayedpumpdiv\").innerHTML = ";
  sHTML += "this.responseText; ";
  sHTML += "}  }; xhttp.open(\"GET\", \"delayedPumpActive\", true); ";
  sHTML += "xhttp.send();}";
  sHTML += "</script> ";


//script für anzeige des auto kalibrierungs zustandes 
  sHTML +=  "<script>";
  sHTML += "setInterval(function() {getData4();}," + String(interv) + ");";
  sHTML += "function getData4() {";
  sHTML += "var xhttp = new XMLHttpRequest();";
  sHTML += "xhttp.onreadystatechange = function() {";
  sHTML += "if (this.readyState == 4 && this.status == 200) {";
  sHTML += "document.getElementById(\"autocaldiv\").innerHTML = ";
  sHTML += "this.responseText; ";
  sHTML += "}  }; xhttp.open(\"GET\", \"autoCalActive\", true); ";
  sHTML += "xhttp.send();}";
  sHTML += "</script> ";

//script für anzeige des auto kalibrierungs zustandes 
  sHTML +=  "<script>";
  sHTML += "setInterval(function() {getData5();}," + String(interv) + ");";
  sHTML += "function getData5() {";
  sHTML += "var xhttp = new XMLHttpRequest();";
  sHTML += "xhttp.onreadystatechange = function() {";
  sHTML += "if (this.readyState == 4 && this.status == 200) {";
  sHTML += "document.getElementById(\"thresholdElement\").innerHTML = ";
  sHTML += "this.responseText; ";
  sHTML += "}  }; xhttp.open(\"GET\", \"threshold\", true); ";
  sHTML += "xhttp.send();}";
  sHTML += "</script> ";


//script für anzeige des Füllstandes 
  sHTML +=  "<script>";
  sHTML += "setInterval(function() {getData6();}," + String(interv) + ");";
  sHTML += "function getData6() {";
  sHTML += "var xhttp = new XMLHttpRequest();";
  sHTML += "xhttp.onreadystatechange = function() {";
  sHTML += "if (this.readyState == 4 && this.status == 200) {";
  sHTML += "document.getElementById(\"percentageElement\").innerHTML = ";
  sHTML += "this.responseText; ";
  sHTML += "}  }; xhttp.open(\"GET\", \"percentage\", true); ";
  sHTML += "xhttp.send();}";
  sHTML += "</script> ";

//script für anzeige der Pumpdauer
  sHTML +=  "<script>";
  sHTML += "setInterval(function() {getData7();}," + String(interv) + ");";
  sHTML += "function getData7() {";
  sHTML += "var xhttp = new XMLHttpRequest();";
  sHTML += "xhttp.onreadystatechange = function() {";
  sHTML += "if (this.readyState == 4 && this.status == 200) {";
  sHTML += "document.getElementById(\"seqdelaydiv\").innerHTML = ";
  sHTML += "this.responseText; ";
  sHTML += "}  }; xhttp.open(\"GET\", \"seqdelay\", true); ";
  sHTML += "xhttp.send();}";
  sHTML += "</script> ";
  sHTML += "<hr>";
  sHTML += "MM 2023 für MobilDigital";
  LED_Yellow();
  return sHTML;
}



String adminPage() {
  String sHTML;
  sHTML = " < !doctype html > ";
  sHTML += "<html>";
  sHTML += "<html lang = \"de\">";
  /***************** head ****************/
  sHTML += "<head>";
  sHTML += "<meta charset=\"utf-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">";
  sHTML += "<title>WasserSystem AdminPage</title>";
  sHTML += "<style>h1{display: flex; flex-flow: row wrap; justify-content: center;} </style>";
  sHTML += "<style>h1{ color: green;}</style>";
  sHTML += "<style>h2{display: flex; flex-flow: row wrap; justify-content: center;} </style>";
  sHTML += "<style>h2{ color: blue;}</style>";
  sHTML += "<style>h3{display: flex; flex-flow: row wrap; justify-content: center;} </style>";
  sHTML += "<style>h3{ color: red;}</style>";
  sHTML += "<style>h5{display: flex; flex-flow: row wrap; justify-content: center;} </style>";
  sHTML += "<style>p{display: flex; flex-flow: row wrap; justify-content: center; margin-bottom: 3%;} </style>";
  sHTML += "<style>p1{display: flex; flex-flow: row wrap; justify-content: center; margin-bottom: 0%;} </style>";
  sHTML += "<style>p2{display: flex; flex-flow: row wrap; justify-content: center; margin-bottom: 0%;} </style>";
  sHTML += "</head>";
  sHTML += "<html><body><h1>Wassersystem Adminseite</h1></body></html>";
  sHTML += "<html><body><h3>VORSICHT: SEITE REFRESHT NICHT AUTOMATISCH</h1></body></html>";
  sHTML += "<p>";
  sHTML += "Uptime in millisekunden: ";
  sHTML += String(millis());
  sHTML += "</p>";

  sHTML += "<p>";
  sHTML += "Ip-Adresse des Systems: ";
  sHTML += ip.toString();
  sHTML += "</p>";

  sHTML += "<p>";
  sHTML += "Feuchtigkeitssensor liest: ";
  sHTML += String(moisture);
  sHTML += "</p>";

  sHTML += "<p>";
  sHTML += "Grenzwert ist: ";
  sHTML += String(threshold);
  sHTML += "</p>";

  sHTML += "<p>";
  sHTML += "Füllstand: ";
  sHTML += String(100);
  sHTML += "%";
  sHTML += "</p>";


  sHTML += "<p>";
  sHTML += "Distanzsensor: ";
  sHTML += String(currentDistance);
  sHTML += "%";
  sHTML += "</p>";

  return sHTML;
}

String showMoisture() {
  return String(moisture);
}
String showSequenceDelay() {
  return String(Sequence_Delay);
}
String showThreshold() {
  return String(threshold);
}
String showPercentage() {
  return String(percentage);
}
String showManualPumpState() {
  if (manualPumpTriggered) {
    return "Manuelles Pumpen aktiviert";
  }
  else {
    return "";
  }
}

String showDelayedPumpState() {
  if (delayedPumpTriggered) {
    return "Verzögertes Pumpen aktiviert";
  }
  else {
    return "";
  }
}



String showAutoCalState() {
  if (autoCalibrationTriggered) {
    return "Automatische Kalibrierung wird vorgenommen";
  }
  else {
    return "";
  }
}
