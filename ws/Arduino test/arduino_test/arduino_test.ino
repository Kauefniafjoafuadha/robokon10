#include "module.hpp"

const int undercarriage_startstop_R_pin = 9;
const int undercarriage_runbreak_R_pin = 8;
const int undercarriage_fwdrev_R_pin = 7;
const int undercarriage_VM_R_pin = 6;

const int undercarriage_startstop_L_pin = 13;
const int undercarriage_runbreak_L_pin = 12;
const int undercarriage_fwdrev_L_pin = 11;
const int undercarriage_VM_L_pin = 10;

const int arm_sv_R_pin = 22;
const int arm_sv_L_pin = 24;

//const int hand_R_pin = ;
//const int hand_L_pin = ;

const int conveyor_pin1 = 5;
const int conveyor_pin2 = 4;
const int conveyor_pin3 = 3;
const int conveyor_pin4 = 2;

// const int ultra_front_trig_pin = ;
// const int ultra_front_echo_pin = ;
// const int ultra_back_trig_pin = ;
// const int ultra_back_echo_pin = ;

void setup() {
    Serial.begin(9600);
    pinMode(undercarriage_startstop_R_pin, OUTPUT);
    pinMode(undercarriage_runbreak_R_pin, OUTPUT);
    pinMode(undercarriage_fwdrev_R_pin, OUTPUT);
    pinMode(undercarriage_VM_R_pin, OUTPUT);

    pinMode(undercarriage_startstop_L_pin, OUTPUT);
    pinMode(undercarriage_runbreak_L_pin, OUTPUT);
    pinMode(undercarriage_fwdrev_L_pin, OUTPUT);
    pinMode(undercarriage_VM_L_pin, OUTPUT);

    pinMode(arm_sv_R_pin, OUTPUT);
    pinMode(arm_sv_L_pin, OUTPUT);

    //pinMode(hand_R_pin, OUTPUT);
    //pinMode(hand_L_pin, OUTPUT);
}

void loop() {
    static Undercarriage undercarriage(
        undercarriage_startstop_R_pin, undercarriage_runbreak_R_pin,
        undercarriage_fwdrev_R_pin, undercarriage_VM_R_pin,
        undercarriage_startstop_L_pin, undercarriage_runbreak_L_pin,
        undercarriage_fwdrev_L_pin, undercarriage_VM_L_pin);
    static Arm arm(arm_sv_R_pin, arm_sv_L_pin, 120, 10);
    //static Hand hand(hand_R_pin, hand_L_pin, 30, 100);
    static Conveyor conveyor(conveyor_pin1, conveyor_pin2, conveyor_pin3,
                             conveyor_pin4, 200, 1000, 1000);
    // static Ultrasonic ultrasonic_front(ultra_front_trig_pin,
    // ultra_front_echo_pin, ultra_back_trig_pin, ultra_back_echo_pin);

    arm.lift();
    delay(1000);
    arm.unload();
    delay(1000);

    // if (Serial.available() > 0) {
    //     char c = Serial.read();
    //     double val = Serial.readStringUntil('\n').toDouble();
    //     if (c == 'u') {
    //         undercarriage.move(100, true);
    //         delay(3000);
    //         undercarriage.move(100, false);
    //         delay(3000);
    //     } else if (c == 'a') {
    //         arm.lift();
    //         delay(1000);
    //         arm.unload();
    //         delay(1000);
    //     } else if (c == 'h') {
    //         hand.catching();
    //         delay(1000);
    //         hand.release();
    //         delay(1000);
    //     } else if (c == 'c') {
    //         conveyor.convey();
    // } else if (c == 'ultra') {
    //     double val_f = ultrasonic_front.distance();
    //     Serial.println(val_b);
    // }
    //}
    //}
}
