#include <WiFi.h>
#include <WebServer.h>

#define RX1 2
#define TX1 4
#define RX2 16
#define TX2 17

//const char* ssid = "Lulu";
//const char* password = "luluru14";
const char* ssid = "rodrigo_hotspot";
const char* password = "Fideo123";
int parkingStatus[8] = {true,true,true,true,true,true,true,true};
int uart1Read = 0;
int uart2Read = 0;
int parkingCount = 0;
WebServer server(80);

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200, SERIAL_8N1, RX1, TX1);
  Serial2.begin(115200, SERIAL_8N1, RX2, TX2);
  Serial.println("Connecting to ");
  Serial.println(ssid);
  //Se inicia la conexión a WiFi
  WiFi.begin(ssid, password);
  //Esperar a que se conecte el WiFi
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  //Se imprime mensaje de exito y la dirección IP del servidor
  Serial.println("");
  Serial.println("WiFi connected successfully");
  Serial.print("Got IP: ");
  Serial.println(WiFi.localIP());
  //Handler principal de la página
  server.on("/", handle_OnConnect);
  //Cuando no se encuentra la pagina
  server.onNotFound(handle_NotFound);
  //Iniciar servidor
  server.begin();
  Serial.println("HTTP server started");
  delay(10000);
}

void loop() {
  server.handleClient();
  server.send(200, "text/html", HTMLPage());
  uart1Read = Serial1.read();
  if (uart1Read > 48 && uart1Read <60) {Serial.println(uart1Read);}
  if (uart1Read == 49) {
    parkingStatus[0] = true;
  } else if (uart1Read == 50) {
    parkingStatus[1] = true;
  } else if (uart1Read == 51) {
    parkingStatus[2] = true;
  } else if (uart1Read == 52) {
    parkingStatus[3] = true;
  } else if (uart1Read == 53) {
    parkingStatus[0] = false;
  } else if (uart1Read == 54) {
    parkingStatus[1] = false;
  } else if (uart1Read == 55) {
    parkingStatus[2] = false;
  } else if (uart1Read == 56) {
    parkingStatus[3] = false;
  }
  uart2Read = Serial2.read();
  if (uart2Read > 48 && uart2Read < 60) {Serial.println(uart2Read);}
  if (uart2Read == 49) {
    parkingStatus[4] = true;
  } else if (uart2Read == 50) {
    parkingStatus[5] = true;
  } else if (uart2Read == 51) {
    parkingStatus[6] = true;
  } else if (uart2Read == 52) {
    parkingStatus[7] = true;
  } else if (uart2Read == 53) {
    parkingStatus[4] = false;
  } else if (uart2Read == 54) {
    parkingStatus[5] = false;
  } else if (uart2Read == 55) {
    parkingStatus[6] = false;
  } else if (uart2Read == 56) {
    parkingStatus[7] = false;
  }
  parkingNumber();
}

void handle_OnConnect() {
  server.send(200, "text/html", HTMLPage());
}

String HTMLPage (void) {
  String html = "<!DOCTYPE html>\n";
  html += "<html lang=en>\n";
  html += "<head>\n";
  html += "<meta charset=UTF-8>\n";
  html += "<meta http-equiv=X-UA-Compatible content=\"IE=edge\">\n";
  html += "<meta name=viewport content=\"width=device-width, initial-scale=1.0\">\n";
  html += "<title>Parqueo</title>\n";
  html += "<meta http-equiv=\"refresh\" content=\"1\">";
  html += "</head>\n";
  html += "<body style=background-color:#d9d9d9;position:absolute;width:100%;height:100%;display:flex;align-items:center;justify-content:center>\n";
  html += "<section class=card style=\"background-color:white;width:50%;height:75%;border-radius:10px;box-shadow:-webkit-box-shadow:0 0 14px 0 rgba(64,64,64,0.78);box-shadow:0 0 14px 0 rgba(64,64,64,0.78)\">\n";
  html += "<header class=card-header style=\"display:flex;justify-content:center;align-items:center;height:10%;width:100;border-radius:10px 10px 0 0;background-color:#949494;font-size:32px;font-family:Arial,Helvetica,sans-serif;font-weight:600;color:white\">Parqueo de Parqueos</header>\n";
  html += "<section class=Disponibilidad style=width:100%;height:80%>\n";
  
  html += "<section class=Parqueo_A: style=display:flex;flex-direction:column;width:100%;height:50%>\n";
  html += "<header class=Parque_A_Header style=margin:20px;font-size:24px;font-weight:600;color:#404040;font-family:Arial,Helvetica,sans-serif>Parqueo A</header>\n";
  html += "<section class=box-container style=display:flex;justify-content:space-around;height:70%;width:100%>\n";
  if (parkingStatus[0] == true) {
    html += "<section class=parking-space style=border-radius:10px;display:flex;align-items:center;justify-content:center;height:100%;width:20%;background-color:#29f505><span style=font-size:28px;font-family:Arial,Helvetica,sans-serif;font-weight:500>A1</span></section>\n";
  } else if (parkingStatus[0] == false) {
    html += "<section class=parking-space style=border-radius:10px;display:flex;align-items:center;justify-content:center;height:100%;width:20%;background-color:#db1212><span style=font-size:28px;font-family:Arial,Helvetica,sans-serif;font-weight:500>A1</span></section>\n";
  }
  if (parkingStatus[1] == true) {
    html += "<section class=parking-space style=border-radius:10px;display:flex;align-items:center;justify-content:center;height:100%;width:20%;background-color:#29f505><span style=\"font-size:28px;font-family:Arial,Helvetica,sans-serif;font-weight:500\"\"><span>A2</span></section>\n";
  } else if (parkingStatus[1] == false) {
    html += "<section class=parking-space style=border-radius:10px;display:flex;align-items:center;justify-content:center;height:100%;width:20%;background-color:#db1212><span style=\"font-size:28px;font-family:Arial,Helvetica,sans-serif;font-weight:500\"\"><span>A2</span></section>\n";
  }
  if (parkingStatus[2] == true) {
    html += "<section class=parking-space style=border-radius:10px;display:flex;align-items:center;justify-content:center;height:100%;width:20%;background-color:#29f505><span style=\"font-size:28px;font-family:Arial,Helvetica,sans-serif;font-weight:500\"\"><span>A3</span></section>\n";
  } else if (parkingStatus[2] == false) {
    html += "<section class=parking-space style=border-radius:10px;display:flex;align-items:center;justify-content:center;height:100%;width:20%;background-color:#db1212><span style=\"font-size:28px;font-family:Arial,Helvetica,sans-serif;font-weight:500\"\"><span>A3</span></section>\n";
  }
  if (parkingStatus[3] == true) {
    html += "<section class=parking-space style=border-radius:10px;display:flex;align-items:center;justify-content:center;height:100%;width:20%;background-color:#29f505><span style=\"font-size:28px;font-family:Arial,Helvetica,sans-serif;font-weight:500\"\"><span>A4</span></section>\n";
  } else if (parkingStatus[3] == false) {
    html += "<section class=parking-space style=border-radius:10px;display:flex;align-items:center;justify-content:center;height:100%;width:20%;background-color:#db1212><span style=\"font-size:28px;font-family:Arial,Helvetica,sans-serif;font-weight:500\"\"><span>A4</span></section>\n";
  }
  html += "</section>\n";
  html += "</section>\n";
  html += "<section class=Parqueo_B: style=display:flex;flex-direction:column;width:100%;height:50%>\n";
  html += "<header class=Parque_B_Header style=margin:20px;font-size:24px;font-weight:600;color:#404040;font-family:Arial,Helvetica,sans-serif>Parqueo B</header>\n";
  html += "<section class=box-container style=display:flex;justify-content:space-around;height:70%;width:100%>\n";
  if (parkingStatus[4] == true) {
    html += "<section class=parking-space style=border-radius:10px;display:flex;align-items:center;justify-content:center;height:100%;width:20%;background-color:#29f505><span style=font-size:28px;font-family:Arial,Helvetica,sans-serif;font-weight:500>B1</span></section>\n";
  } else if (parkingStatus[4] == false) {
    html += "<section class=parking-space style=border-radius:10px;display:flex;align-items:center;justify-content:center;height:100%;width:20%;background-color:#db1212><span style=font-size:28px;font-family:Arial,Helvetica,sans-serif;font-weight:500>B1</span></section>\n";
  }
  if (parkingStatus[5] == true) {
    html += "<section class=parking-space style=border-radius:10px;display:flex;align-items:center;justify-content:center;height:100%;width:20%;background-color:#29f505><span style=\"font-size:28px;font-family:Arial,Helvetica,sans-serif;font-weight:500\"\"><span>B2</span></section>\n";
  } else if (parkingStatus[5] == false) {
    html += "<section class=parking-space style=border-radius:10px;display:flex;align-items:center;justify-content:center;height:100%;width:20%;background-color:#db1212><span style=\"font-size:28px;font-family:Arial,Helvetica,sans-serif;font-weight:500\"\"><span>B2</span></section>\n";
  }
  if (parkingStatus[6] == true) {
    html += "<section class=parking-space style=border-radius:10px;display:flex;align-items:center;justify-content:center;height:100%;width:20%;background-color:#29f505><span style=\"font-size:28px;font-family:Arial,Helvetica,sans-serif;font-weight:500\"\"><span>B3</span></section>\n";
  } else if (parkingStatus[6] == false) {
    html += "<section class=parking-space style=border-radius:10px;display:flex;align-items:center;justify-content:center;height:100%;width:20%;background-color:#db1212><span style=\"font-size:28px;font-family:Arial,Helvetica,sans-serif;font-weight:500\"\"><span>B3</span></section>\n";
  }
  if (parkingStatus[7] == true) {
    html += "<section class=parking-space style=border-radius:10px;display:flex;align-items:center;justify-content:center;height:100%;width:20%;background-color:#29f505><span style=\"font-size:28px;font-family:Arial,Helvetica,sans-serif;font-weight:500\"\"><span>B4</span></section>\n";
  } else if (parkingStatus[7] == false) {
    html += "<section class=parking-space style=border-radius:10px;display:flex;align-items:center;justify-content:center;height:100%;width:20%;background-color:#db1212><span style=\"font-size:28px;font-family:Arial,Helvetica,sans-serif;font-weight:500\"\"><span>B4</span></section>\n";
  }
  html += "</section>\n";
  html += "</section>\n";
  html += "<section class=Total: style=display:flex;justify-content:center;align-items:center;height:10%;width:100%>\n";
  html += "<span style=font-size:24px;font-weight:600;color:#404040;font-family:Arial,Helvetica,sans-serif;margin-right:15px>Parqueos Disponibles:</span>\n";
  html += "<span style=font-size:24px;font-weight:600;color:black;font-family:Arial,Helvetica,sans-serif>";
  html += String(parkingCount);
  html += "</span>\n";
  html += "</section>\n";
  html += "</section>\n";
  html += "</section>\n";
  html += "</body>\n";
  html += "</html>";
  return html;
}

void parkingNumber (void) {
  parkingCount = 0;
  for (int i = 0; i < 8; i++) {
    if (parkingStatus[i] == true) {
      parkingCount++;
    }
  }
}

void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}
