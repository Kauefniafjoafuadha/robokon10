#include "module.hpp"

const int undercarriage_startstop_R_pin = ;
const int undercarriage_runbreak_R_pin = ;
const int undercarriage_fwdrev_R_pin = ;
const int undercarriage_VM_R_pin = ;

const int undercarriage_startstop_L_pin = ;
const int undercarriage_runbreak_L_pin = ;
const int undercarriage_fwdrev_L_pin = ;
const int undercarriage_VM_L_pin = ;

const int arm_sv_R_pin = ;
const int arm_sv_L_pin = ;

const int hand_R1_pin = ;
const int hand_R2_pin = ;
const int hand_R3_pin = ;
const int hand_R4_pin = ;
const int hand_L1_pin = ;
const int hand_L2_pin = ;
const int hand_L3_pin = ;
const int hand_L4_pin = ;

const int conveyor_pin1 = ;
const int conveyor_pin2 = ;
const int conveyor_pin3 = ;
const int conveyor_pin4 = ;

const int photo_front_Apin = ;
const int photo_back_Apin = ;
const int photo_left_Apin = ;
const int photo_right_Apin = ;

const int ultra_front_trig_pin = ;
const int ultra_front_echo_pin = ;
const int ultra_back_trig_pin = ;
const int ultra_back_echo_pin = ;

void setup() {
    Serial.begin(9600);
    Undercarriage undercarriage(
        undercarriage_startstop_R_pin, undercarriage_runbreak_R_pin,
        undercarriage_fwdrev_R_pin, undercarriage_VM_R_pin,
        undercarriage_startstop_L_pin, undercarriage_runbreak_L_pin,
        undercarriage_fwdrev_L_pin, undercarriage_VM_L_pin);
    Arm arm(arm_sv_R_pin, arm_sv_L_pin, 180, 0);
    Hand hand(hand_R1_pin, hand_R2_pin, hand_R3_pin, hand_R4_pin, hand_L1_pin,
              hand_L2_pin, hand_L3_pin, hand_L4_pin, 200, 100, 1000);
    Conveyor conveyor(conveyor_pin1, conveyor_pin2, conveyor_pin3,
                      conveyor_pin4, 200, 1000, 1000);
    Photo photo(photo_front_pin, photo_back_pin, photo_left_pin,
                photo_right_pin, 200);
    Ultrasonic ultrasonic_front(ultra_front_trig_pin, ultra_front_echo_pin);
    Ultrasonic ultrasonic_back(ultra_back_trig_pin, ultra_back_echo_pin);
}

void loop() {
    if (Serial.available() > 0) {
        char c = Serial.read();
        double val = Serial.readStringUntil('\n').toDouble();
        if (c == 'u') {
            undercarriage.move(100, true);
            delay(3000);
            undercarriage.move(100, false);
            delay(3000);
            undercarriage.turn_right();
            undercarriage.turn_left();
        } else if (c == 'a') {
            arm.lift();
            delay(1000);
            arm.unload();
            delay(1000);
        } else if (c == 'h') {
            hand.catch();
            delay(1000);
            hand.release();
            delay(1000);
        } else if (c == 'c') {
            conveyor.convey();
        } else if (c == 'p') {
            Serial.print(photo.detect(photo_front_pin));
            Serial.print(" ");
            Serial.print(photo.detect(photo_back_pin));
            Serial.print(" ");
            Serial.print(photo.detect(photo_left_pin));
            Serial.print(" ");
            Serial.println(photo.detect(photo_right_pin));
        } else if (c == 'ultra') {
            double val_f = ultrasonic_front.distance();
            double val_b = ultrasonic_back.distance();
            Serial.print(val_f);
            Serial.print(' ');
            Serial.println(val_b);
        }
    }
}
