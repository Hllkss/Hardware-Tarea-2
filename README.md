# Hardware-Tarea-2

## 1. Descripción del Proyecto
Este proyecto consiste en la construcción de un Theremin digital en Arduino UNO. El instrumento (Theremin) permite generar distintos tonos donde la frecuencia varía según la distancia a la que se encuentre la mano del usuario respecto a un sensor ultrasónico. 
El sistema cuenta con tres tonos distintos que se seleccionan mediante pulsadores, los tonos son grave, ,medio y agudo.

# 2. Componentes utilizados
Placa Microcontroladora: Arduino UNO.
Sensor de Distancia: Un Sensor ultrasónico para medir la distancia de la mano.
Zumbador: Un Buzzer para la salida del sonido mediante una frecuencia que puede variar .
Pulsadores : 3 pulsadores configurados con resistencias de internas del arduino para la selección de los tonos de sonido.

# 3. Diagrama de Conexiones
El esquema del circuito se encuentra en la carpeta EsquemaCircuito

# 4 Detalle de los pines:
Sensor Ultrasónico: Conectado al pin digital 9 , 5v y tierra.
Zumbador (buzzer): Conectado al Pin Digital 8 (+) y a tierra (GND) (-).
Pulsador rango grave: Conectado al pin digital 2  y a tierra (GND) .
Pulsador rango medio: Conectado al pin digital 3  y a tierra (GND) .
Pulsador rango agudo: Conectado al pin digital 4  y a tierra (GND) .

# 5. Instrucciones para cargar el código
 1. Asegúrate de tener instalado el Arduino IDE.
 2. copia o descarga el codigo de este repositorio GitHub de la carpeta llamada "Theremin".
 4. Conecta tu placa Arduino UNO al computador mediante un cable USB.
 5. En el Arduino IDE, selecciona tu placa Arduino Uno.
 6. Pon el codigo en el entorno de Arudino Ide y haz clic en el botón cargar para compilar e instalar el firmware en el microcontrolador.

# 6. Explicación de la Lógica de Funcionamiento
Antirrebote y entradas: Los pulsadores utilizan la resistencia interna (INPUT_PULLUP) del Arduino. Se implementó un algoritmo usando millis() para evitar que los pulsadores den pulsaciones erroneas y que el programa siga funcionando sin detenerse.

Medición y filtrado: El sensor ultrasónico detecta a intervalos regulares (50 ms) de forma no bloqueante. Se aplica un filtro por promedio móvil exponencial sobre la distancia obtenida para evitar saltos bruscos e inestabilidad en el tono.

Generación de sonido: Dependiendo del pulsador elegido (Grave, Medio o Agudo), la distancia útil medida de 5 cm a 30 cm (es un ejemplo, se puede cambiar) se convierte mediante map() y constrain() en una frecuencia proporcional de octava, logrando que a menor distancia se emita un tono más agudo (comportamiento de theremin normal). Si la mano se retira de la zona útil, el zumbador entra en silencio  mediante noTone().

# 7. Declaracion de IA
 Para esta tarea se utilizo la IA como apoyo para mejorarlo, para refinar el código y resolver dudas sobre este mismo