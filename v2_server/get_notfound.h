



void render_notfound() {
  server.sendHeader("Location", "/", true);
  server.send(302, "text/plain", "");
}


