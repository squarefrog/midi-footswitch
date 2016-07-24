/*
 Bluetooth MIDI Footswitch v0.1
 Copyright (c) Paul Williamson 2016

 This project uses the LightBlue-Bean board to send MIDI CC messages.
 The pedal may be used with an Core MIDI compliant iOS or macOS app including
 Positive Grid JamUp Pro.

 Latest source available at:
 http://github.com/squarefrog/midi-footswitch

 Changelog:
 http://github.com/squarefrog/midi-footswitch/CHANGELOG.md

 MIT License:
 http://github.com/squarefrog/midi-footswitch/LICENSE
 */

// Set number of buttons in use
const int numBtns = 6;

// Set up an array to track each buttons state
bool buttonState[numBtns];

void setup() {
  // Configure Bean to wake up when a client connects
  Bean.enableWakeOnConnect(true);

  // initialize serial communication at 57600 bits per second:
  Serial.begin(57600);

  for (int i = 0; i < numBtns; i++) {
    // Setup initial button state for input
    buttonState[i] = true;

    // Prepare digital pins for input
    pinMode(i, INPUT_PULLUP);
  }
}

void loop() {
  // Check if client has disconnected
  bool connected = Bean.getConnectionState();

  if (connected) {
    Bean.setLed(255, 0, 0);
    // Loop through each button, reading the state.
    for (int i = 0; i < numBtns; i++) {
      readButton(i);
    }
  } else {
    Bean.setLed(0, 0, 0);
    // Sleep for a long time to conserve power.
    // 0xFFFFFFFF = 4,294,967,295 milliseconds = 49 days
    Bean.sleep(0xFFFFFFFF);
  }
}

void readButton(int i) {
  bool oldState = buttonState[i];
  bool newState = digitalRead(i);

  // If button state hasn't changed, exit.
  if (oldState == newState) {
    return;
  }

  // Buttons are set as INPUT_PULLUP so the state is flipped.
  if (newState == true) {
    // Button not pressed
    buttonReleased(i);
  } else {
    // Button pressed
    if (oldState != newState) {
      buttonPressed(i);
    }
  }
}

void buttonPressed(int btn) {
  buttonState[btn] = false;
  logButtonAction("pressed", btn);
}

void buttonReleased(int btn) {
  buttonState[btn] = true;
  logButtonAction("released", btn);
}

void logButtonAction(String action, int btn) {
  String buttonString = String(btn, DEC);
  Serial.print("Button " + action + ": " + buttonString);
}

