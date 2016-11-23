#include <stdbool.h>

const uint32_t SwitchCount = 2;
const uint32_t ButtonCount = 2;
const uint32_t Switches[SwitchCount] = { PA_7, PA_6 };
const uint32_t Buttons[ButtonCount] = { PD_2, PE_0 };
const uint32_t Potentiometer = PE_3;

struct ButtonState { 
  bool state;
  bool isRising;
};

static struct InputState {
  bool                switches[2];
  struct ButtonState  buttons[2];
  float               potentiometer;
} ControlInputState;

void ControlInit() {
  ControlInputState = {0};

  for (int i=0; i<SwitchCount; i++)
    pinMode(Switches[i], INPUT);
  for (int i=0; i<ButtonCount; i++)
    pinMode(Buttons[i], INPUT);
}

void ReadInput() {
  for (int i=0; i<SwitchCount; i++)
    ControlInputState.switches[i] = digitalRead(Switches[i]);
  for (int i=0; i<ButtonCount; i++) { 
    bool previousState = ControlInputState.buttons[i].state;
    ControlInputState.buttons[i].state = digitalRead(Buttons[i]);
    ControlInputState.buttons[i].isRising = (!previousState && ControlInputState.buttons[i].state);
  }
  ControlInputState.potentiometer = analogRead(Potentiometer);
}

bool GetButtonEnter() {
  return ControlInputState.buttons[0].isRising;
}

bool GetButtonCancel() {
  return ControlInputState.buttons[1].isRising;
}

bool GetSwitchTempLock() {
  return ControlInputState.switches[0];
}

bool GetSwitchControlLock() {
  return ControlInputState.switches[1];
}

float GetPotentiometer() {
  return ControlInputState.potentiometer;
}

