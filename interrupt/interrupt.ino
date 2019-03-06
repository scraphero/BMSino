//Aqui hare pruebas para conseguir pulsos por segundo antes de integrar este codigo en ecuino.ino

int short TACHOMETER_PIN = 2 ;    // Pin for tachometer
int short PULSE_LIMIT = 4 ;       // Define how many pulses are needed until calcs
int short PULSES_X_SPIN = 1 ;     // Define how much pulses we get per wheel spin
int       WHEEL_DISTANCE = 1500 ; // Define the wheel spin straight travel

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
	Serial.print(tachometer_count);
	if( tachometer_count >= PULSE_LIMIT )
	{
		new_time       = millis();
		measuring_time = new_time - last_time ;
		pulse_x_second = tachometer_count / (measuring_time / 1000) ;
		rpm            = ( pulse_x_second / PULSES_X_SPIN ) * 60 ;
		kmh            = rpm * WHEEL_DISTANCE * 60 ;
		
		Serial.print(" ");
		Serial.print(tachometer_count);
		Serial.print("np ");
		Serial.print( measuring_time );
		Serial.print("t ");
		Serial.print( pulse_x_second );
		Serial.println("Hz");

		tachometer_count = 0 ;
		last_time = new_time;
	}
}

void setup()
{
	Serial.begin(9600);
	pinMode(12, OUTPUT);
	pinMode(TACHOMETER_PIN, INPUT);
	attachInterrupt( digitalPinToInterrupt(TACHOMETER_PIN), tachometer, RISING);
}

void loop()
{
	digitalWrite(12, HIGH);
	delay(100);
	digitalWrite(12, LOW);
	delay(100);
}