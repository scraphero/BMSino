//Este sketch se cargara en un arduino dedicado a la medicion de la velocidad del vehiculo
//esta velocidad sera enviada al arduino principal mediante comunicacion en paralelo usando 7 pines digitales.

int short TACHOMETER_PIN     = 3 ;    // Pin for tachometer
int short TACHOMETER_GND_PIN = 4 ;    // Pin for tachometer gnd
int short PULSE_LIMIT        = 4 ;    // Define how many pulses are needed until calcs
int short PULSES_X_SPIN      = 1 ;    // Define how much pulses we get per wheel spin
int       WHEEL_DISTANCE     = 1500 ; // Define the wheel spin straight travel

int short tachometer_count = 0 ; 
int       new_time = 0 ;
int       last_time = 0 ;
int       measuring_time = 0 ;
float     pulse_x_second = 0 ;
int       rpm = 0 ;
int       kmh = 0 ;


void tachometer()  // tachometer_count | measuring_time | pulse_x_second | rpm | kmh
{
	tachometer_count = tachometer_count + 1 ;
	if( tachometer_count >= PULSE_LIMIT )
	{
		new_time       = millis();
		measuring_time = new_time - last_time ;
		pulse_x_second = tachometer_count / (measuring_time / 1000) ;
		rpm            = ( pulse_x_second / PULSES_X_SPIN ) * 60 ;
		kmh            = rpm * WHEEL_DISTANCE * 60 ;
	
		tachometer_count = 0 ;
		last_time = new_time;
	}
}



/*void parallel_send_speed(){
	//{    0kmh | 000000
		if (kmh == 0)
		{
			digitalWrite( parallel_pin_1, LOW);
			digitalWrite( parallel_pin_2, LOW);
			digitalWrite( parallel_pin_4, LOW);
			digitalWrite( parallel_pin_8, LOW);
			digitalWrite(parallel_pin_16, LOW);
			digitalWrite(parallel_pin_32, LOW);
		}
	//{    1kmh | 000001
		if (kmh == 1)
		{
			digitalWrite( parallel_pin_1,  HIGH);
			digitalWrite( parallel_pin_2,  LOW);
			digitalWrite( parallel_pin_4,  LOW);
			digitalWrite( parallel_pin_8,  LOW);
			digitalWrite( parallel_pin_16, LOW);
			digitalWrite( parallel_pin_32, LOW);
		}
	//{    2kmh | 000010
		if (kmh == 2)
		{
			digitalWrite( parallel_pin_1,  LOW);
			digitalWrite( parallel_pin_2,  HIGH);
			digitalWrite( parallel_pin_4,  LOW);
			digitalWrite( parallel_pin_8,  LOW);
			digitalWrite( parallel_pin_16, LOW);
			digitalWrite( parallel_pin_32, LOW);
		}
	//{    3kmh | 000011
		if (kmh == 3)
		{
			digitalWrite( parallel_pin_1,  HIGH);
			digitalWrite( parallel_pin_2,  HIGH);
			digitalWrite( parallel_pin_4,  LOW);
			digitalWrite( parallel_pin_8,  LOW);
			digitalWrite( parallel_pin_16, LOW);
			digitalWrite( parallel_pin_32, LOW);
		}
	//{    4kmh | 000100
		if (kmh == 4)
		{
			digitalWrite( parallel_pin_1,  LOW);
			digitalWrite( parallel_pin_2,  LOW);
			digitalWrite( parallel_pin_4,  HIGH);
			digitalWrite( parallel_pin_8,  LOW);
			digitalWrite( parallel_pin_16, LOW);
			digitalWrite( parallel_pin_32, LOW);
		}
	//{    5kmh | 000101
		if (kmh == 5)
		{
			digitalWrite( parallel_pin_1,  HIGH);
			digitalWrite( parallel_pin_2,  LOW);
			digitalWrite( parallel_pin_4,  HIGH);
			digitalWrite( parallel_pin_8,  LOW);
			digitalWrite( parallel_pin_16, LOW);
			digitalWrite( parallel_pin_32, LOW);
		}
	//{    6kmh | 000110
		if (kmh == 6)
		{
			digitalWrite( parallel_pin_1,  LOW);
			digitalWrite( parallel_pin_2,  HIGH);
			digitalWrite( parallel_pin_4,  HIGH);
			digitalWrite( parallel_pin_8,  LOW);
			digitalWrite( parallel_pin_16, LOW);
			digitalWrite( parallel_pin_32, LOW);
		}
	//{    7kmh | 000111
		if (kmh == 7)
		{
			digitalWrite( parallel_pin_1,  HIGH);
			digitalWrite( parallel_pin_2,  HIGH);
			digitalWrite( parallel_pin_4,  HIGH);
			digitalWrite( parallel_pin_8,  LOW);
			digitalWrite( parallel_pin_16, LOW);
			digitalWrite( parallel_pin_32, LOW);
		}
	//{    8kmh | 001000
		if (kmh == 8)
		{
			digitalWrite( parallel_pin_1,  LOW);
			digitalWrite( parallel_pin_2,  LOW);
			digitalWrite( parallel_pin_4,  LOW);
			digitalWrite( parallel_pin_8,  HIGH);
			digitalWrite( parallel_pin_16, LOW);
			digitalWrite( parallel_pin_32, LOW);
		}
	//{    9kmh | 001001
		if (kmh == 9)
		{
			digitalWrite( parallel_pin_1,  HIGH);
			digitalWrite( parallel_pin_2,  LOW);
			digitalWrite( parallel_pin_4,  LOW);
			digitalWrite( parallel_pin_8,  HIGH);
			digitalWrite( parallel_pin_16, LOW);
			digitalWrite( parallel_pin_32, LOW);
		}
	//{   10kmh | 001010
		if (kmh == 10)
		{
			digitalWrite( parallel_pin_1,  LOW);
			digitalWrite( parallel_pin_2,  HIGH);
			digitalWrite( parallel_pin_4,  LOW);
			digitalWrite( parallel_pin_8,  HIGH);
			digitalWrite( parallel_pin_16, LOW);
			digitalWrite( parallel_pin_32, LOW);
		}
}*/

//PARALLEL SPEED DATA PINS {
 int parallel_pin_1  = 13 ;
 int parallel_pin_2  = 12 ;
 int parallel_pin_4  = 11 ;
 int parallel_pin_8  = 10 ;
 int parallel_pin_16 =  9 ;
 int parallel_pin_32 =  8 ; //}
//PARALLEL INPROVED {

	int bit_p_i = 0 ;

	void parallel_inproved() 
	{
		//  BIT 1  {
			bit_p_i = kmh & 1 ;
			if (bit_p_i == 1) { digitalWrite(parallel_pin_1, HIGH); }
			else { digitalWrite(parallel_pin_1, LOW); }  //}
		//  BIT 2  {
			bit_p_i = kmh & 2 ;
			if (bit_p_i == 2) { digitalWrite(parallel_pin_2, HIGH); }
			else { digitalWrite(parallel_pin_2, LOW); }  //}
		//  BIT 4  {
			bit_p_i = kmh & 4 ;
			if (bit_p_i == 4) { digitalWrite(parallel_pin_4, HIGH); }
			else { digitalWrite(parallel_pin_4, LOW); }  //}
		//  BIT 8  {
			bit_p_i = kmh & 8 ;
			if (bit_p_i == 8) { digitalWrite(parallel_pin_8, HIGH); }
			else { digitalWrite(parallel_pin_8, LOW); }  //}
		//  BIT 16 {
			bit_p_i = kmh & 16 ;
			if (bit_p_i == 16) { digitalWrite(parallel_pin_16, HIGH); }
			else { digitalWrite(parallel_pin_16, LOW); }  //}
		//  BIT 32 {
			bit_p_i = kmh & 32 ;
			if (bit_p_i == 32) { digitalWrite(parallel_pin_32, HIGH); }
			else { digitalWrite(parallel_pin_32, LOW); }  //}
	}

void setup()
{
	Serial.begin(9600);
	// PARALLEL COMMUNICATION {
		pinMode(parallel_pin_1,  OUTPUT);
		pinMode(parallel_pin_2,  OUTPUT);
		pinMode(parallel_pin_4,  OUTPUT);
		pinMode(parallel_pin_8,  OUTPUT);
		pinMode(parallel_pin_16, OUTPUT);
		pinMode(parallel_pin_32, OUTPUT); //}
	// TACHOMETER {
		pinMode        ( TACHOMETER_GND_PIN, OUTPUT );
		digitalWrite   ( TACHOMETER_GND_PIN, LOW );
		digitalWrite   ( TACHOMETER_PIN    , HIGH );
		pinMode        ( TACHOMETER_PIN    , INPUT );
		//attachInterrupt( digitalPinToInterrupt(TACHOMETER_PIN), tachometer, RISING);  //}
}

void loop()
{
	//parallel_send_speed();
	
kmh = kmh + 1 ;
  if (kmh > 50){
    kmh = 0 ;
  }
  Serial.println(kmh);
	parallel_inproved();

	delay(300);
}
