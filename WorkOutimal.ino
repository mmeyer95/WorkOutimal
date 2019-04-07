
int lightPin = 0;  //define a pin for Photo resistor
int ledPin=13;     //define a pin for LED
long t=0; //create a time variable
long now=0;    //variable for current photoresistor reading
int i=0;      //indexing varaible for darkTimes
long darkTimes[3];  //array of times that dark strips pass
long rotationTime;  //time for 2 rotations
float velocity=0;   //flywheel velocity
float pedal_velocity=0;  //pedal velocity
int flagDark=0;  //flag for a dark strip
int flagLight=0; //flag for a light strip
int p=0;

void setup()
{
    Serial.begin(9600);  //Begin serial communcation
    pinMode( ledPin, OUTPUT );
    analogWrite(ledPin, 400); //turn LED on

    pinMode (2, OUTPUT);
    pinMode (3, OUTPUT);
    pinMode (4, OUTPUT);
    pinMode (5, OUTPUT);
    pinMode (10, OUTPUT);
    pinMode (11, OUTPUT);
    
    Serial.println("Starting actuator control");
    digitalWrite(10, HIGH); //turns on necessary pins
    digitalWrite(11, HIGH);
    
    Serial.println("Retracting all the way.");
    digitalWrite(4, HIGH);
    digitalWrite(5, LOW);
    digitalWrite(2, HIGH);
    digitalWrite(3, LOW); //retract
    delay(3000);

    digitalWrite(4, LOW);
    digitalWrite(5, HIGH);
    digitalWrite(2, LOW);
    digitalWrite(3, HIGH); //extend
    delay(1750);

    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
    digitalWrite(2, LOW);
    digitalWrite(3, LOW); //pause
    
}

void loop()
{  
  t=millis(); //define time at current reading
  now=analogRead(lightPin); //define current light value
//  Serial.print("Current time is: ");  //can uncomment this if needing to know time
//  Serial.println(millis());

if (now>800){flagLight=1;}    //LIGHT STRIP: set a flag for when air passes in front of sensor

if (now<600 && flagLight==1){flagDark=1;}      //DARK STRIP: set a flag for when a spoke passes in front of sensor & there has been light in between

if (flagDark==1 && flagLight==1){     //both flags triggering means a flywheel spoke and empty spot have both passed
    darkTimes[i]=t;            //record the time that a dark strip passed
    i++;
    flagDark=0;                     //reset the flags to find the next instance
    flagLight=0;
    if (i==2){                   //after 2 spokes have passed, calculate approximate velocity
      rotationTime=darkTimes[1]-darkTimes[0];  //time for 1/3 rotations, in milliseconds
      velocity=60000/(3*rotationTime);           //rotations per minute
      pedal_velocity=velocity/6.75;               //pedal velocity, based on ~6.75 rotations of the flywheel=1 pedal rotation
      //Serial.print("Approximate flywheel velocity is: ");  //can uncomment this to print flywheel velocity to Serial monitor
      //Serial.println(velocity);
      Serial.print("Approximate pedal velocity is: ");
      Serial.println(pedal_velocity);
      Serial.print("Current time is: ");
      Serial.println(millis());
      i=0;  //restart, empty darkTimes array
      }
    }
 
}


