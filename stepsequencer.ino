int slideSwitch1 = 32;
int leds[4] = { 3, 4, 5, 6 };
int tempoPotVal = 0;
int currentStep = 0;
int buttonPins[4] = { 29 , 28 , 27 , 26 } ;
boolean lastButtonState[4] = { LOW , LOW , LOW , LOW } ;
boolean buttonState[4] = { LOW , LOW , LOW , LOW };
boolean on[4] = { false , false , false , false };    
int slideSwitch2 = 6;


void setup() {
  pinMode(slideSwitch1, INPUT);
  for (int i = 0; i < 4; i++) {
    pinMode(leds[i], OUTPUT);
    pinMode(buttonPins[i], INPUT); 
  }
}

void loop() {
  sequence() ;
  checkButtons() ;
  setLeds() ;  // performs these 3 functions at same time
}

void forward() {  //sequencer moves forward
  tempoPotVal = analogRead(A14);   //tempo pot controlling spead of sequencer
  tempo = map(tempoPotVal, 0, 1023, 0, 1000); 
  if (millis() > lastStepTime + tempo) {

    usbMIDI.sendNoteOff(60, 0, 1);     
    digitalWrite(leds[currentStep], LOW);   //turn note off + LED off

    currentStep = currentStep + 1;
    if (currentStep > 3) {
      currentStep = 0;
    }
    lastStepTime = millis();
    if (on[currentStep] == HIGH) { 
      usbMIDI.sendNoteOn(60, 127, 1);  // turn LED + note on (note on only if the step its on is turned on with button)
    }
    digitalWrite(leds[currentStep], HIGH);
  }
}
void reverse() {  //sequencer moves in reverse  
  tempoPotVal = analogRead(A14);  //potentiometer to control speed of sequencer
  tempo = map(tempoPotVal, 0, 1023, 0, 1000);
  if (millis() > lastStepTime + tempo) {
    digitalWrite(leds[currentStep], LOW); //turn note off + LED off
    usbMIDI.sendNoteOff(70, 0, 1);
    currentStep = currentStep -= 1;
    if (currentStep < 0) {
      currentStep = 3;
    }
    lastStepTime = millis();
    if (on[currentStep] == HIGH) {
      usbMIDI.sendNoteOn(70, 127, 1);    // turn LED + note on (note on only if the step its on is turned on with button)
    }
    digitalWrite(leds[currentStep], HIGH);
  }
}

void sequence() {
  if (digitalRead(slideSwitch1) == HIGH) {    // if slide switch is towards power, sequencer goes forward
    forward() ;
  }
  if (digitalRead(slideSwitch1) == LOW) {   // if slide switch is away from power, sequencer goes in reverse
    reverse() ;
  }
}
void checkButtons() {   
  for (int i = 0; i < 4; i++) {
    lastButtonState[i] = buttonState[i];    
    buttonState[i] = digitalRead(buttonPins[i]);
    if (buttonState[i] == HIGH && lastButtonState[i] == LOW) {
      if (on[i] == false) {
        on[i] = true;
      } else if (on[i] == true) {
        on[i] = false;
      }
    }
  }
}

void setLeds() {
  for (int i = 0; i < 4; i++) {
    if (on[i] == true) {
      digitalWrite(leds[i], HIGH);
    }  else if (currentStep == i) {
      digitalWrite(leds[i], HIGH) ;
    }
    else {
      digitalWrite(leds[i], LOW);
    }
  }
}
