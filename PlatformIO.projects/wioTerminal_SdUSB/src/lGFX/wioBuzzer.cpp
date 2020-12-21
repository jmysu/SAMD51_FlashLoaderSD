#include <Arduino.h>

#define BUZZER_PIN WIO_BUZZER /* sig pin of the buzzer */

 
void setupBuzzer() {
    //set buzzer pin as output
    pinMode(BUZZER_PIN, OUTPUT);
}
 

//Play tone
void playTone(int tone, int duration) {
    for (long i = 0; i < duration * 1000L; i += tone * 2) {
        digitalWrite(BUZZER_PIN, HIGH);
        delayMicroseconds(tone);

        digitalWrite(BUZZER_PIN, LOW);
        delayMicroseconds(tone);
    }
}