#include <LiquidCrystal.h>,

//LiquidCrystal lcd(8,9,4,5,6,7);
LiquidCrystal lcd(9, 8, 7, 6, 5, 4);
unsigned long tiempo_actual, tiempo_anterior = 0, delta_tiempo;
//Contadores
int contadorSegs = 0;
int contadorMins = 0;
int contadorHs = 0;
int contadorIntervalo = 1;

//Para calcular intervalo
int multiplicador = 1;
int calculador = 0;

//Para la comunicacion serial
int option;
int hrI = 0;
int minI = 0;

//Para mostrar los menus
int booleanReply = HIGH;

//Para la funcion analogica
String botones_LCD();
int buttonReply;
String buttonText;


void setup() {
  pinMode(3, OUTPUT);
  Serial.begin(9600);
  lcd.begin(16, 2);
}

void loop() {
  tiempo_actual = millis();
  //calculador = multiplicador * 10;
  buttonReply = analogRead(0);
//Menu reloj
  if (booleanReply == HIGH) {
    lcd.setCursor(5, 0);
    if (contadorHs < 10) lcd.print("0");
    lcd.print(contadorHs);
    lcd.print(":");
    lcd.setCursor(8, 0);
    if (contadorMins < 10) lcd.print("0");
    lcd.print(contadorMins);
  }
//Menu intervalo
  if (multiplicador <= 60 && multiplicador >= 1 && booleanReply == LOW) {
    lcd.setCursor(0, 0);
    lcd.print("Intervalo:");
    lcd.setCursor(0, 11);  // Vuelve a la posición para imprimir el nuevo valor
    if (multiplicador < 10) {
      lcd.print("0");
    }
    lcd.print(multiplicador);
  }

  /*
  if (Serial.available() == 0) {
    int option = Serial.parseInt();
    if (option == 1 || option == 2) {
      switch (option) {
        case 1:
          Serial.println("Ingrese la hora: ");
          while(Serial.available() == 1){
             //Tiempo de espera 
          }
          hrI = Serial.parseInt();
            if(hrI < 0 || hrI > 23 ) {
            Serial.println("Se admiten valores positivos hasta 23");
            } else {
              contadorHs = hrI;
            }
          break;
        case 2:
          Serial.println("Ingrese los minutos: ");
          while(Serial.available() == 1){
             //Tiempo de espera 
          }
            minI = Serial.parseInt();
            if(minI < 0 || minI > 59 ) {
            Serial.println("Se admiten valores positivos hasta 59");
            } else {
              contadorMins = minI;
            }
          break;
      }
    }
  }
  */
//Comunicacion con el puerto Serial
  if (Serial.available() != 0) {
    int option = Serial.parseInt();
    if (option == 1 || option == 2) {
      switch (option) {
        case 1:
          Serial.println("Ingrese la hora: ");
          while (Serial.available() == 0) {
            //Tiempo de espera
          }
          hrI = Serial.parseInt();
          if (hrI < 0 || hrI > 23) {
            Serial.println("Se admiten valores positivos hasta 23");
          } else {
            contadorHs = hrI;
          }
          break;
        case 2:
          Serial.println("Ingrese los minutos: ");
          while (Serial.available() == 0) {
            //Tiempo de espera
          }
          minI = Serial.parseInt();
          if (minI < 0 || minI > 59) {
            Serial.println("Se admiten valores positivos hasta 59");
          } else {
            contadorMins = minI;
          }
          break;
      }
    }
  }
//Logica reloj
  delta_tiempo = tiempo_actual - tiempo_anterior;

  if (delta_tiempo >= 1000) {
    contadorSegs++;
    contadorIntervalo++;
    Serial.println(contadorIntervalo);
    tiempo_anterior = tiempo_actual;
  }

  if (contadorSegs > 59) {
    contadorMins++;
    contadorSegs = 0;
  }

  if (contadorMins > 59) {
    contadorHs++;
    contadorMins = 0;
  }
  if (contadorHs > 23) {
    contadorHs = 0;
  }
//Llamada la funcion
  buttonText = botones_LCD(buttonReply);
  Serial.println(buttonText);
  if (buttonText == "btnDerecha") {
    if (multiplicador <= 60) {
      multiplicador++;
      booleanReply = LOW;
    }
  }
  if (buttonText == "btnIzquierda") {
    if (multiplicador >= 1) {
      multiplicador--;
      booleanReply = LOW;
    }
  }

  if (buttonText == "btnArriba") {
    if (multiplicador >= 0) {
      calculador = multiplicador * 60;
      contadorIntervalo = 55;
      booleanReply = HIGH;
      lcd.clear();
      contadorSegs = 1;
    }
  }

  if (buttonText == "btnAbajo") {
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("Sin funcion");
    delay(800);
    lcd.clear();
  }

  if (buttonText == "btnSelect") {
    digitalWrite(3, HIGH);
    lcd.clear();
    //digitalWrite(3,LOW );
    lcd.print("-Rociando Spray-");
    delay(800);
    lcd.clear();
    //digitalWrite(3, HIGH);
    digitalWrite(3, LOW);
  }


  if (contadorIntervalo == calculador) {
    lcd.clear();
    lcd.print("-Rociando Spray-");
    //digitalWrite(3,LOW );
    digitalWrite(3, HIGH);
    delay(800);
    //digitalWrite(3, HIGH);
    digitalWrite(3, LOW);
    contadorIntervalo = 0;
    lcd.clear();
  }
}

String botones_LCD(int buttonReply) {
  String text;
  Serial.println(buttonReply);
  //if (buttonReply < 60) text = "btnDerecha";
  if (buttonReply < 50) text = "btnDerecha";
  //if (buttonReply > 99 && buttonReply < 127)text = "btnArriba";
  if (buttonReply > 95 && buttonReply < 150) text = "btnArriba";
  //if (buttonReply > 255 && buttonReply < 280)  text = "btnAbajo";
  if (buttonReply > 240 && buttonReply < 350) text = "btnAbajo";
  // if (buttonReply > 399 && buttonReply < 435)  text = "btnIzquierda";
  if (buttonReply > 400 && buttonReply < 500) text = "btnIzquierda";
  // if (buttonReply > 640 && buttonReply < 668)  text = "btnSelect";
  if (buttonReply > 600 && buttonReply < 750) text = "btnSelect";
  //if (buttonReply > 668 && buttonReply < 1024) text = "btnNone";
  if (buttonReply > 700 && buttonReply < 1024) text = "btnNone";
  return text;
}

/*// SELECT (640 A 668)
// UP (99 A 127)
// DOWN (255 A 280)
// RIGHT (0 A 60)
// LEFT(399 A 435)*/
