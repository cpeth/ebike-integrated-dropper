const int PWM1 = 5;    // Forward PWM pin
const int PWM2 = 3;    // Reverse PWM pin
const int buttonPin = 2;  // Button input pin

bool wasPressed = false;  // Tracks button state
bool reversing = false;   // Tracks if reversing is active
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;  
unsigned long reverseStartTime = 0;

const unsigned long reverseDuration = 180;
const unsigned long forwardPower = 255; // Forward at 100% speed
const unsigned long reversePower = 175; // Reverse at 68% since it is helped my dropper spring force

void setup() {
    pinMode(PWM1, OUTPUT);
    pinMode(PWM2, OUTPUT);
    pinMode(buttonPin, INPUT_PULLUP);  // Enable internal pull-up resistor
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
    static bool lastButtonState = HIGH;  // Last stable state of the button
    bool reading = digitalRead(buttonPin); // Read button state

    // Debounce check: Only update if the state is stable for debounceDelay ms
    if (reading != lastButtonState) {
        lastDebounceTime = millis(); // Reset the debounce timer
    }

    if ((millis() - lastDebounceTime) > debounceDelay) {
        // State is stable, now check the actual button state
        if (reading == LOW) {  // Button is pressed
            analogWrite(PWM1, forwardPower);  
            analogWrite(PWM2, 0);    // Reverse OFF
            wasPressed = true;
        } 
        else if (wasPressed && !reversing) {  
            // Button was released, start reverse
            analogWrite(PWM1, 0);
            analogWrite(PWM2, reversePower);  
            digitalWrite(LED_BUILTIN, HIGH);
            reverseStartTime = millis();
            reversing = true;
        }
    }

    // Handle reverse timing non-blocking
    if (reversing && (millis() - reverseStartTime >= reverseDuration)) {
        // Stop the motor after reverse duration
        analogWrite(PWM1, 0);
        analogWrite(PWM2, 0);
        digitalWrite(LED_BUILTIN, LOW);
        reversing = false;
        wasPressed = false;
    }

    lastButtonState = reading;  // Update last button state
}
