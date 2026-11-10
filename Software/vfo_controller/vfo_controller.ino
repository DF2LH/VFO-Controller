// Fork from VFO-Controller by JI1UDD
// For replace the T-Mate 1 Controller internal conroller with an Ardoino Leonardo
// with VFO switching ID 16/17 for MIDI control on Thetis 
// 11/2026 by Thomas Seibt DF2LH + Claude

#include <Control_Surface.h> // Control Surface library by Pieter P (tttapa)

// PIN for Aruduino Leonardo with IDs in Thetis MIDI
#define PIN_ENC_A   (3) // GPIO3 - ID 16/17 Wheel
#define PIN_ENC_B   (2) // GPIO2 - ID 16/17 Wheel
#define PIN_PSW1   (A3) // A3    - ID 80
#define PIN_PSW2    (4) // GPIO4 - ID 19
#define PIN_PSW3    (5) // GPIO5 - ID 18
#define PIN_PSW4   (A2) // A2    - Switch VFO A/B between ID 16/17 (no MIDI)
#define PIN_LED_A   (6) // LED VFO A green (Common Anode, LOW = on)
#define PIN_LED_B   (7) // LED VFO B red   (Common Anode, LOW = on)

bool vfoB = false;

// Instantiate a MIDI over USB interface.
USBMIDI_Interface midi;

// Instantiate a CCRotaryEncoder CC16 - VFO A
CCRotaryEncoder enc {
  {PIN_ENC_A, PIN_ENC_B},           // pins
  {MCU::V_POT_1, CHANNEL_1},        // CC 16
  1,                                // optional multiplier if the control isn't fast enough
};

// Instantiate a CCButton object
CCButton pushswitches[] {
  // Left Button used internal by A2
  { PIN_PSW2, {MIDI_CC::General_Purpose_Controller_3, CHANNEL_1} },  // upper left middle
  { PIN_PSW3, {MIDI_CC::General_Purpose_Controller_4, CHANNEL_1} },  // right middel button
  { PIN_PSW4, {MIDI_CC::General_Purpose_Controller_5, CHANNEL_1} },  // right button
};

Button vfoSwitch {PIN_PSW1};

void updateLeds() {
  digitalWrite(PIN_LED_A, vfoB ? HIGH : LOW);
  digitalWrite(PIN_LED_B, vfoB ? LOW : HIGH);
}

void updateEncoderAddress() {
  // VFO A -> CC 16, VFO B -> CC 17
  enc.setAddress({vfoB ? 17 : 16, CHANNEL_1});
}

void setup() {
  RelativeCCSender::setMode(relativeCCmode::TWOS_COMPLEMENT);
  Control_Surface.begin();

  pinMode(PIN_LED_A, OUTPUT);
  pinMode(PIN_LED_B, OUTPUT);
  updateLeds();
  updateEncoderAddress();
}

void loop() {
  Control_Surface.loop();

  if (vfoSwitch.update() == Button::Falling) {
    vfoB = !vfoB;
    updateLeds();
    updateEncoderAddress();
  }
}
