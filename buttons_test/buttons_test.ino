//Buttons
// push button pins
const int streetMode = 4;
const int readSignButton = 5;
const int navigationMode = 6;
const int fallCancelButton = 3;

// push button states
int streetModeState = 0;
int readSignState = 0;
int navigationModeState = 0;
int fallCancelState = 0;

// mode state
bool streetModeOn = false;
bool readSignOn = false;
bool navigationModeOn = false;

char dict[30];

void setup() {
  // put your setup code here, to run once:
  pinMode(streetMode, INPUT);
  pinMode(navigationMode, INPUT);
  pinMode(readSignButton, INPUT);
  pinMode(fallCancelButton, INPUT);
  Serial.begin(9600); // Starts the serial communication
}

void loop() {
  streetModeState = digitalRead(streetMode);
  readSignState = digitalRead(readSignButton);
  navigationModeState = digitalRead(navigationMode);
  fallCancelState = digitalRead(fallCancelButton);

  if (streetModeState == HIGH){
    streetModeOn = !streetModeOn;
    delay(500);
  }

  else if (navigationModeState == HIGH) {
    navigationModeOn = !navigationModeOn;
    delay(500);
  }

  if (readSignState == HIGH) {
    readSignOn = true;
    streetModeOn = false;
  } else {
    readSignOn = false;
  }

  if (fallCancelState == HIGH) {
    Serial.println("fall button pushed");
  }
  sprintf(dict, "{\"nm\": %d, \"sm\": %d, \"rs\": %d}", navigationModeOn, streetModeOn, readSignOn);
  Serial.println(dict);

  delay(100);
}
