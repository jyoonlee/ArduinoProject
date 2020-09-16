void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(10, OUTPUT); //Vibration motor
  pinMode(8, OUTPUT); //potentiometer
  pinMode(7, OUTPUT); // Buzzer
  pinMode(2, OUTPUT); //Push button output
  pinMode(4, INPUT); //Push button input
  pinMode(3,OUTPUT); //Light Sensor
}

int count=0,temp2=0;
int ch=0;
int select=-1;
int button=0; //Check button is push or not
int i=0;
int potpin=1;  // analog pin used to connect the potentiometer
int val;  //variable to read the value from the analog pin

int enter()
{ //Insert time in Arduino
  int temp=Serial.read();
  if (temp == -1) {  // if not inserted, implement recursive function
     enter();
   }
   else { // if entered, return the input value
     Serial.println(temp-'0');
     return temp-'0';  //due to limitation of Serial board
   }
}//end enter

int check()
{ // select program 1, 2.
  int temp=Serial.read();
  if (temp == -1) {  // if not inserted, implement recursive function
     check();
  }
  else {  // if entered, return the input value
    Serial.println(temp-'0');
    return temp-'0';
  }
} //end check

void alarm()
{ //Alarm (vibrator) function
  digitalWrite(2,HIGH);
  while(1) {

    if (temp2 == 0) {
        ch=enter(); //Input time in Arduino
        //ch*=3600;  //can also be entered in units of hours
        temp2++; //prevent from implementing again.
    }

   if(count!=ch) {
     count++;
     delay(1000);
   } //end if

   Serial.println(count);
   Serial.println(ch);   //print the results.

   while (count == ch) {
     digitalWrite(8,HIGH);
     button=digitalRead(4);
     val=analogRead(potpin); //potentiometer
     Serial.println(val);
     analogWrite(10,val/4); //Turn on the Vibrator
     digitalWrite(7, HIGH); //Turn on the Buzzer
     delay(1000);
     digitalWrite(10, LOW); //Turn off
     digitalWrite(7, LOW); //Turn off
     delay(1000);

     Serial.println(button);
     if ( button == HIGH ) //When you push the button
        break;
   }//end while
   if(button == HIGH) //When you push the button
     break;
  } //end while2
}//end alarm functionfhh

void checklight()
{ //Light Sensor Function
  int ledbright=0;
  button=0;
  digitalWrite(3, HIGH);
  digitalWrite(2, HIGH);
  while(1) {
    ledbright = analogRead(0);
    Serial.println(ledbright); // Use light sensor

    if(ledbright > 500) { // When your sleeping place is bright
       button=digitalRead(4);
       Serial.println(button);
       if(button == HIGH) {
            digitalWrite(10, LOW);
            break;
       }
       digitalWrite(8,HIGH);
       val=analogRead(potpin);
       analogWrite(10,val/4); //Turn on vibration motor
       digitalWrite(7, HIGH); // Turn on the Buzzer
       delay(1000);
       digitalWrite(10, LOW);//Turn off vibration motor
       digitalWrite(7, LOW); //Turn off the Buzzer
       delay(1000);
    }//end if
    else if (ledbright < 500) { //When your sleeping place is dark
       digitalWrite(10, LOW);//Turn off vibration motor
       delay(1000);
    }//end else if

    if(button == HIGH)
       break;
  }//end while
}//end checklight function

void loop() {

  if (i == 0) { //choose the options
    Serial.println("If you type number 1 : You can use orginal alarm");
    Serial.println("If you type number 2 : You can use alarm with light sensor");
    select = check();
    i++;  //prevent from implementing again.
  }

  if(select != -1) {
    switch(select) {
    case 1 : //Original Alarm & Buzzer
      alarm();
      break;

    case 2 : //Light Sensor & Buzzer
      checklight();
      break;
    } //end switch
  }//end if

  if(button == HIGH)
  {  //if button is pressed, initialize all the variables.
    button=0;
    i=0;
    if (count == ch) {
      temp2=0;
      count=0;
    }
  }//end if
}//end loop