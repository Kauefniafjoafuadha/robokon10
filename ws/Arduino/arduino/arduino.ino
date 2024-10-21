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

const int hand_R_pin = 50;
const int hand_L_pin = 52;

const int conveyor_pin1 = 5;
const int conveyor_pin2 = 4;
const int conveyor_pin3 = 3;
const int conveyor_pin4 = 2;

const int ultra_front_trig_pin = 40;
const int ultra_front_echo_pin = 42;
const int ultra_back_trig_pin = 32;
const int ultra_back_echo_pin = 34;

// 現在の機体のパターンを表す変数
// 0: 前方の箱の前まで近づく
// 1: 掴んでベルトコンベアに乗せる
// 2: 後方に進行しゴールの直前まで近づく
// 3: 荷物を下ろす
// 4: 前方に進行し箱の前まで近づく
int mode = 0;

// 前方の箱までの距離を記憶する変数
double front_distance = 0;
// 後方のゴールまでの距離を記憶する変数
double back_distance = 0;

// 荷物を掴みやすい前方の箱までの距離
double front_target_distance = 5;
// 荷物を下ろしやすい障害物までの距離
double back_target_distance = 10;

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

    pinMode(hand_R_pin, OUTPUT);
    pinMode(hand_L_pin, OUTPUT);
}

void loop() {
    static Undercarriage undercarriage(
        undercarriage_startstop_R_pin, undercarriage_runbreak_R_pin,
        undercarriage_fwdrev_R_pin, undercarriage_VM_R_pin,
        undercarriage_startstop_L_pin, undercarriage_runbreak_L_pin,
        undercarriage_fwdrev_L_pin, undercarriage_VM_L_pin);
    static Arm arm(arm_sv_R_pin, arm_sv_L_pin, 120, 10);
    static Hand hand(hand_R_pin, hand_L_pin, 97, 50);
    static Conveyor conveyor(conveyor_pin1, conveyor_pin2, conveyor_pin3,
                             conveyor_pin4, 200, 1000, 10000);
    static Ultrasonic ultrasonic(ultra_front_trig_pin, ultra_front_echo_pin,
                                 ultra_back_trig_pin, ultra_back_echo_pin);

    front_distance = ultrasonic.front_distance();
    back_distance = ultrasonic.back_distance();

    hand.release();
    arm.unload();

    switch (mode) {
    // 前方の箱の前まで近づく
    case 0:
        if (front_distance > front_target_distance) {
            undercarriage.move(100, true);
        } else if (front_distance <= front_target_distance) {
            undercarriage.stop_slowly();
            mode = 1;
            delay(1000);
        }
        break;

    // 掴んでベルトコンベアに乗せる
    case 1:
        hand.catching();
        delay(1000);
        arm.lift();
        delay(1000);
        hand.release();
        delay(1000);
        mode = 2;
        break;

    // 後方に進行しゴールの直前まで近づく
    case 2:
        if (back_distance > back_target_distance) {
            undercarriage.move(100, false);
        } else if (back_distance <= back_target_distance) {
            undercarriage.stop_slowly();
            mode = 3;
        }
        delay(1000);
        break;

    // 荷物を下ろす
    case 3:
        conveyor.convey();
        mode = 0;
        delay(1000);
        hand.release();
        arm.unload();
        break;
    }
    Serial.println(mode);
}
