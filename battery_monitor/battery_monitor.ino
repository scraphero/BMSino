//v0.1 Esta version añade la funcion de calcular la autonomia_segundos_totales en tiempo restante de bateria

int tiempo_real;

/*PINES*/
const int PIN_AMP_METER = A0;          //PIN AMPERIMETRO
const int PIN_VOLT_METER = A7;         //PIN VOLTIMETRO

/*SENSOR HALL*/
const float CORRECT_VOLT_HALL = -2.50; //Para restar el voltage que registra el sensor hall cuando la corriente es = 0
const float SENSIBILIDAD = 65;         //sensibilidad en Voltios/Amperio para sensor de 5A

/*VOLTIMETRO*/
const float CORRECT_VOLT_BATT = 49.8;  //La bateria proporciona alrededor de 50V a la que fisicamente se le ha aplicado un divisor de tension de 10:1 asique para 50V de bateria le llegan 5

/*BATERIA*/
const int TIEMPO_DE_LECTURA = 100;     //sera el tiempo en milisegundos 
float bateriaRestante = 162000;        //valor asignado como capacidad total inicialmente, resultado de multiplicar la capacidad de la bateria "45Ah" x 3600s puesto que restaremos el valor medio de corriente medido cada segundo
int tiempo_medicion = 0;               //Esta variable se usara para tomar los valores de medida cada cierto intervalo de tiempo definido en la siguiente constante
const int DELAY_MEDICION = 1;          //En este caso el valor es 1 porque queremos que se realice una medicion cada segundo
int porcentaje_bateria = 0;            //Almacenara el porcentaje de bateria que se calculara a partir de bateriaRestante posteriormente
int long autonomia_segundos_totales = 0;       //Medira la autonomia instantanea expresada en segundos
int short autonomia_segundos = 0;
int long autonomia_minutos_totales = 0;
int short autonomia_minutos = 0;
int long autonomia_horas_totales = 0;

void medicion_periodica()
{
  float voltajeSensor= analogRead(PIN_AMP_METER)*(5.0 / 1023.0);                //lectura del sensor hall, medida transformada en valores de 0 a 5 para conocer el valor de tension real del output del sensor HALL
  float corriente_hall = ( voltajeSensor + CORRECT_VOLT_HALL ) * SENSIBILIDAD;  //Ecuación  para obtener la corriente a partir del valor anterior, ajuste de offset y sensibilidad, convertir la señal de voltaje output del sensor HALL en el valor de corriente real
  float voltajeBatt= analogRead(PIN_VOLT_METER)*(CORRECT_VOLT_BATT / 1023.0);   //lectura de volaje 
  float Watt = voltajeBatt * corriente_hall ;                                   //Potencia = V * I
  bateriaRestante = bateriaRestante - corriente_hall;                           //Cada vez que se ejecute esta linea se restara el valor medido de corriente a las unidades totales de bateria
  porcentaje_bateria = (bateriaRestante / 162000)*100;                          //Sencilla operacion para calcular en forma de porcentaje la bateria restante a partir de esas unidades sin magnitud que nos hemos inventado
  
  autonomia_segundos_totales = bateriaRestante / corriente_hall;                        //autonomia_segundos_totales sera tiempo restante de bateria en segundos, sera un valor instantaneo
  autonomia_segundos = autonomia_segundos_totales % 60;
  autonomia_minutos_totales = autonomia_segundos_totales / 60;
  autonomia_minutos = autonomia_minutos_totales % 60;
  autonomia_horas_totales = autonomia_minutos_totales / 60;

  //Serial.print("tiempo real:");
  //Serial.print(tiempo_real);
  //Serial.print("    tiempo medicion");
  //Serial.print(tiempo_medicion);
  //Serial.print("    bateria restante:");
  //Serial.print(bateriaRestante);
  //Serial.print("    ");
  Serial.print(porcentaje_bateria);         //Serial print porcentaje bateria
  Serial.print("%    ");

  Serial.print("Autonomia:");
  Serial.print(autonomia_segundos_totales);         //Serial print autonomia en segundos totales
  Serial.print("s left    ");

  Serial.print(autonomia_horas_totales);
  Serial.print("h");
  Serial.print(autonomia_minutos);
  Serial.print("m");
  Serial.print(autonomia_segundos);
  Serial.print("s");

  Serial.print("    Voltaje sensor: ");
  Serial.print(voltajeSensor);

  Serial.print("    W: "); //Watios
  Serial.print(Watt);
  
  Serial.print("    V: "); //Voltios
  Serial.print(voltajeBatt);
  
  Serial.print("    A: "); //Amperios
  Serial.println(corriente_hall,2); 

  tiempo_medicion = tiempo_medicion + DELAY_MEDICION ;    //Como hemos comentado antes esta linea le añade el tiempo que queremos que retarde entre lecturas
}


void setup() {                            //SETUP
  Serial.begin(115200);                   //Activar comunicacion serie
}

void loop()                               //LOOP
{
  tiempo_real = (millis()/1000);          //tiempo_real almacenara el tiempo en segundos para ello hemos dividido millis entre 1000

  if ( tiempo_medicion == tiempo_real )   //Al final de la funcion medicion_periodica le sumamos a tiempo_medicion el tiempo en segundos DELAY_MEDICION asi no se volvera a ejecutar la funcion hasta que tiempo_real vuelva a igualar tiempo_medicion
  {
    medicion_periodica();                 //Llamada a la funcion que engloba las mediciones sobre la bateria y sus calculos V, corriente_hall, W, bateria restante etc.
  }
}
