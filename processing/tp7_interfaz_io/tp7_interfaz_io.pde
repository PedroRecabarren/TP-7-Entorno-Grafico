import processing.serial.*;

Serial puerto;

int e1 = 0, e2 = 0;
int l1 = 0, l2 = 0;

float e1X = 220, e1Y = 120, boxW = 90, boxH = 60;
float e2X = 350, e2Y = 120;
float l1X = 265, l1Y = 290, ledR = 64;
float l2X = 395, l2Y = 290;

void setup() {
  size(560, 400);
  println("Puertos disponibles:");
  printArray(Serial.list());

  if (Serial.list().length > 0) {
    String nombrePuerto = Serial.list()[0];
    puerto = new Serial(this, nombrePuerto, 9600);
    puerto.bufferUntil('\n');
  }
}

void draw() {
  background(245);

  fill(120, 155, 195);
  rect(40, 120, 130, 60);
  fill(0);
  textAlign(CENTER, CENTER);
  textSize(18);
  text("Entradas", 105, 150);

  fill(245, 220, 200);
  rect(40, 260, 130, 60);
  fill(0);
  text("Salidas", 105, 290);

  dibujarEntrada("E1", e1X, e1Y, boxW, boxH, e1 == 1);
  dibujarEntrada("E2", e2X, e2Y, boxW, boxH, e2 == 1);

  dibujarSalida("L1", l1X, l1Y, ledR, l1 == 1);
  dibujarSalida("L2", l2X, l2Y, ledR, l2 == 1);
}

void dibujarEntrada(String etiqueta, float x, float y, float w, float h, boolean activa) {
  fill(activa ? color(80, 200, 120) : color(255));
  stroke(0);
  strokeWeight(2);
  rect(x, y, w, h);

  fill(0);
  textSize(18);
  text(etiqueta, x + w / 2, y + h / 2);
}

void dibujarSalida(String etiqueta, float x, float y, float diametro, boolean encendida) {
  fill(encendida ? color(255, 190, 40) : color(255));
  stroke(0);
  strokeWeight(3);
  ellipse(x, y, diametro, diametro);

  fill(0);
  textSize(18);
  text(etiqueta, x, y);
}

void mousePressed() {
  if (puerto == null) return;

  if (dist(mouseX, mouseY, l1X, l1Y) <= ledR / 2) {
    puerto.write('1');
  } else if (dist(mouseX, mouseY, l2X, l2Y) <= ledR / 2) {
    puerto.write('2');
  }
}

void serialEvent(Serial p) {
  String trama = p.readStringUntil('\n');
  if (trama == null) return;
  trama = trim(trama);

  String[] partes = split(trama, ';');
  if (partes.length == 2 && partes[0].startsWith("E:") && partes[1].startsWith("S:")) {
    String[] valsE = split(partes[0].substring(2), ',');
    String[] valsS = split(partes[1].substring(2), ',');
    if (valsE.length == 2 && valsS.length == 2) {
      e1 = int(trim(valsE[0]));
      e2 = int(trim(valsE[1]));
      l1 = int(trim(valsS[0]));
      l2 = int(trim(valsS[1]));
    }
  }
}
