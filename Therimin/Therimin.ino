enum RangoTonal { RANGO_GRAVE, RANGO_MEDIO, RANGO_AGUDO };

const int PIN_SENSOR = 9;   
const int PIN_BUZZER = 8;   

const int PIN_BOTON_GRAVE = 2; 
const int PIN_BOTON_MEDIO = 3; 
const int PIN_BOTON_AGUDO = 4; 

const unsigned long INTERVALO_MEDICION = 50; 
const unsigned long DEBOUNCE_DELAY = 50;      
const float DISTANCIA_MIN = 5.0;              
const float DISTANCIA_MAX = 200.0;           

int rangoActual = RANGO_MEDIO;// Tono activo actual  
unsigned long ultimoTiempoMedicion = 0;
unsigned long ultimoTiempoAntirrebote[3] = {0, 0, 0};// Controla el tiempo antirrebote para cada botón
int ultimoEstadoBoton[3] = {HIGH, HIGH, HIGH};
float distanciaFiltrada = 0.0;

void setup() {
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_BOTON_GRAVE, INPUT_PULLUP);
  pinMode(PIN_BOTON_MEDIO, INPUT_PULLUP); // Activar resistencia interna
  pinMode(PIN_BOTON_AGUDO, INPUT_PULLUP);

  Serial.begin(9600);
}

void loop() {
  leerPulsadores(); // Revisa si se presionó algún botón para cambiar el tono
  
  // Mide la distancia periódicamente
  if (millis() - ultimoTiempoMedicion >= INTERVALO_MEDICION) {
    ultimoTiempoMedicion = millis();
    
    float distancia = medirDistancia();
    if (distancia > 0) {
      distanciaFiltrada = filtrarDistancia(distancia); // // Filtra los saltos bruscos de la lectura
      gestionarSonido(distanciaFiltrada, rangoActual); // Reproduce el sonido según la distancia y el tono
    } else {
      noTone(PIN_BUZZER); 
    }
  }
}

// Lee los botones con sistema antirrebote
void leerPulsadores() {
  int pinesBoton[3] = {PIN_BOTON_GRAVE, PIN_BOTON_MEDIO, PIN_BOTON_AGUDO};
  int rangos[3] = {RANGO_GRAVE, RANGO_MEDIO, RANGO_AGUDO};

  for (int i = 0; i < 3; i++) {
    int estadoLectura = digitalRead(pinesBoton[i]);

    if (estadoLectura != ultimoEstadoBoton[i]) {
      if (millis() - ultimoTiempoAntirrebote[i] > DEBOUNCE_DELAY) {
        ultimoTiempoAntirrebote[i] = millis();
        ultimoEstadoBoton[i] = estadoLectura;

        if (estadoLectura == LOW) {
          rangoActual = rangos[i]; // Cambia el rango del tono actual
        }
      }
    }
  }
}

// Calcula la distancia usando el sensor 
float medirDistancia() {
  pinMode(PIN_SENSOR, OUTPUT);
  digitalWrite(PIN_SENSOR, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_SENSOR, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_SENSOR, LOW);

  pinMode(PIN_SENSOR, INPUT);
  long duracion = pulseIn(PIN_SENSOR, HIGH, 30000); 
  if (duracion == 0) return -1;

  float distancia = (duracion * 0.0343) / 2.0;
  
  if (distancia < 2.0 || distancia > 400.0) return -1;
  return distancia;
}

// Aplica un filtro para evitar saltos bruscos en la distancia
float filtrarDistancia(float nuevaLectura) {
  const float alpha = 0.4; 
  if (distanciaFiltrada == 0.0) {
    distanciaFiltrada = nuevaLectura;
  } else {
    distanciaFiltrada = (alpha * nuevaLectura) + ((1.0 - alpha) * distanciaFiltrada);
  }
  return distanciaFiltrada;
}

// Genera el sonido en el buzzer según la distancia y el tono seleccionado
void gestionarSonido(float distancia, int rango) {
  if (distancia < DISTANCIA_MIN || distancia > DISTANCIA_MAX) {
    noTone(PIN_BUZZER);
    return;
  }

  int freqMin = 262;
  int freqMax = 523;

  // Define los límites de frecuencia según el rango activo
  switch (rango) {
    case RANGO_GRAVE:
      freqMin = 131; freqMax = 262; 
      break;
    case RANGO_MEDIO:
      freqMin = 262; freqMax = 523; 
      break;
    case RANGO_AGUDO:
      freqMin = 523; freqMax = 1047; 
      break;
  }

  // Convierte la distancia en una frecuencia musical inversa (más cerca = más agudo)
  int frecuencia = map(distancia, DISTANCIA_MIN, DISTANCIA_MAX, freqMax, freqMin);
  frecuencia = constrain(frecuencia, freqMin, freqMax);

  tone(PIN_BUZZER, frecuencia);
}