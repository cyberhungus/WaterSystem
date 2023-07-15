String MainPage() {
  String sHTML;
  sHTML = "<!doctype html>";
  sHTML += "<html>";
  sHTML += "<html lang=\"de\">";
  /***************** head ****************/
  sHTML += "<head>";
  sHTML += "<meta http-equiv=\"refresh\" content=\"5\">";
  sHTML += "<meta charset=\"utf-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">";
  sHTML += "<title>LetsgoING IoT</title>";
  sHTML += "<style>h1{display: flex; flex-flow: row wrap; justify-content: center;} </style>";
  sHTML += "<style>h1{ color: green;}</style>";
  sHTML += "<style>h2{display: flex; flex-flow: row wrap; justify-content: center;} </style>";
  sHTML += "<style>h2{ color: blue;}</style>";
  sHTML += "<style>h5{display: flex; flex-flow: row wrap; justify-content: center;} </style>";
  sHTML += "<style>p{display: flex; flex-flow: row wrap; justify-content: center; margin-bottom: 3%;} </style>";
  sHTML += "<style>p1{display: flex; flex-flow: row wrap; justify-content: center; margin-bottom: 0%;} </style>";
  sHTML += "<style>p2{display: flex; flex-flow: row wrap; justify-content: center; margin-bottom: 0%;} </style>";
  sHTML += "</head>";
  sHTML += "<html><body><h1>Automatisches Wassersystem by MobilDigital </h1></body></html>";


  return sHTML;
}

String adminPage() {
  String sHTML;
  sHTML = "<!doctype html>";
  sHTML += "<html>";
  sHTML += "<html lang=\"de\">";
  /***************** head ****************/
  sHTML += "<head>";
  sHTML += "<meta charset=\"utf-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">";
  sHTML += "<title>LetsgoING IoT</title>";
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

  return sHTML;
}
