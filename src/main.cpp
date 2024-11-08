#include <Arduino.h>
#include <System.h>
#include <Signal.h>
#include <Detectors.h>

static Signal signal;
static LevelDetector levelDetector(&signal);
static SlopeDetector slopeDetector(&signal);
static PitchDetector pitchDetector(&signal, &slopeDetector);

int main(void)
{
  System::setupWithSerial(9600);
  pitchDetector.setRefFreq(440.0f);

  System::loop([] { // loop
    // If clipping was detected, ensure the clipping indicator LED is off
    if (signal.isClipping())
      System::turnOff(System::Pin::Indicator);

    String note = pitchDetector.pitch().note();
    Serial.println(note);
  }); // loop
}

DAC_LOOP(
    // passthrough input
    System::writeDAC(signal.input());

    ) // DAC_LOOP

ADC_LOOP(
    System::turnOff(System::Pin::Output); // Turn off output pin initially

    signal.update(System::readADC()); // Update signal with the latest ADC value
    Patterns::Trigger trigger = signal.isTriggered(ADC_EQUILIBRIUM);

    if (trigger == Patterns::Trigger::PositiveEdge) {
      pitchDetector.detect([]() {            // Defer execution with closure
        System::turnOn(System::Pin::Output); // Detect pitch and turn on output pin
      });
    }

    if (signal.isClipping()) {                // If clipping is detected
      System::turnOn(System::Pin::Indicator); // Turn on the clipping indicator LED
    }

    pitchDetector.update();
    levelDetector.update();

    ) // ADC_LOOP
