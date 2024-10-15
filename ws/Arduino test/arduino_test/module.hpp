#include <Servo.h>
#include <Stepper.h>

// 足回りの制御を行うクラス
class Undercarriage {
  private:
    // 変数名の詳細はBLH2D15H-Kの取扱説明書を参照
    // ※LOW出力でon、HIGH出力でoffになることに注意

    // モーターを運転する信号です。
    // START/STOP入力とRUN/BRAKE入力の両方をONにすると加速時間に
    // 従ってモーターが回転します。START/STOP入力をOFFにするとモーター
    // が減速時間に従って停止します。RUN/BRAKE入力をOFFにするとモーター
    // は瞬時停止します。
    const int _startstop_R_pin11;
    const int _runbreak_R_pin10;
    const int _startstop_L_pin11;
    const int _runbreak_L_pin10;
    // モーターの回転方向を変更します。
    // ONにするとCW方向に回転し、OFFにするとCCW方向に回転します。
    // 右側のモーターはccw方向で前進、左側のモーターはcw方向で前進します。
    const int _fwdrev_R_pin9;
    const int _fwdrev_L_pin9;
    // モーターの回転速度を変更します。
    // 0~5vの電圧に対応して0~3000rpmの回転速度が出ます。
    const int _VM_R_pin5;
    const int _VM_L_pin5;

  public:
    Undercarriage(int startstop_R_pin11, int runbreak_R_pin11,
                  int fwdrev_R_pin9, int VM_R_pin5, int startstop_L_pin11,
                  int runbreak_L_pin11, int fwdrev_L_pin9, int VM_L_pin5)
        : _startstop_R_pin11(startstop_R_pin11),
          _runbreak_R_pin10(runbreak_R_pin11), _fwdrev_R_pin9(fwdrev_R_pin9),
          _VM_R_pin5(VM_R_pin5), _startstop_L_pin11(startstop_L_pin11),
          _runbreak_L_pin10(runbreak_L_pin11), _fwdrev_L_pin9(fwdrev_L_pin9),
          _VM_L_pin5(VM_L_pin5) {
        // pinMode(_startstop_R_pin11, OUTPUT);
        // pinMode(_runbreak_R_pin10, OUTPUT);
        // pinMode(_fwdrev_R_pin9, OUTPUT);
        // pinMode(_VM_R_pin5, OUTPUT);
        // pinMode(_startstop_L_pin11, OUTPUT);
        // pinMode(_runbreak_L_pin10, OUTPUT);
        // pinMode(_fwdrev_L_pin9, OUTPUT);
        // pinMode(_VM_L_pin5, OUTPUT);
        // digitalWrite(_startstop_R_pin11, HIGH);
        // digitalWrite(_runbreak_R_pin10, HIGH);
        // digitalWrite(_startstop_L_pin11, HIGH);
        // digitalWrite(_runbreak_L_pin10, HIGH);
    }
    // 右モーターを回す関数
    // dutyの0~255が0~3000rpmの回転速度に対応する。
    // directionがtrueならモーターに接続されているタイヤが前進し、falseなら後退する。
    void rotateR(int duty, bool direction) {
        if (direction) {
            digitalWrite(_fwdrev_R_pin9, HIGH);
        } else {
            digitalWrite(_fwdrev_R_pin9, LOW);
        }
        analogWrite(_VM_R_pin5, duty);
        digitalWrite(_startstop_R_pin11, LOW);
        digitalWrite(_runbreak_R_pin10, LOW);
    }
    // 左モーターを回す関数
    // dutyの0~255が0~3000rpmの回転速度に対応する。
    // directionがtrueならモーターに接続されているタイヤが前進し、falseなら後退する。
    void rotateL(int duty, bool direction) {
        if (direction) {
            digitalWrite(_fwdrev_L_pin9, LOW);
        } else {
            digitalWrite(_fwdrev_L_pin9, HIGH);
        }
        analogWrite(_VM_L_pin5, duty);
        digitalWrite(_startstop_L_pin11, LOW);
        digitalWrite(_runbreak_L_pin10, LOW);
    }
    // 機体を前進または後退させる関数
    // dutyで速度を設定
    // directionで進行方向を選択　trueで前進、falseで後退
    void move(int duty, bool direction) {
        rotateR(duty, direction);
        rotateL(duty, direction);
    }
    // 機体を急停止する関数
    void stop_suddenly() {
        digitalWrite(_runbreak_R_pin10, HIGH);
        digitalWrite(_runbreak_L_pin10, HIGH);
    }
    // 機体をゆっくり停止する関数
    void stop_slowly() {
        digitalWrite(_startstop_R_pin11, HIGH);
        digitalWrite(_startstop_L_pin11, HIGH);
    }
    // 機体を右に向ける関数
    void turn_right() {
        rotateR(100, false);
        rotateL(100, true);
        delay(1000);
        stop_suddenly();
    }
    // 機体を左に向ける関数
    void turn_left() {
        rotateR(100, true);
        rotateL(100, false);
        delay(1000);
        stop_suddenly();
    }
};

// ロボットアームの制御を行うクラス
class Arm {
  private:
    Servo Rservo;
    Servo Lservo;
    const int _Rpin;
    const int _Lpin;
    int _up_angle;
    int _down_angle;

  public:
    // コンストラクタ
    Arm(int Rpin, int Lpin, int up_angle, int down_angle)
        : _Rpin(Rpin), _Lpin(Lpin), _up_angle(up_angle),
          _down_angle(down_angle) {
        Rservo.attach(_Rpin, 500, 2500);
        Lservo.attach(_Lpin, 500, 2500);
    }
    // アームを上げる関数
    void lift() {
        Rservo.write(_up_angle);
        Lservo.write(180 - _up_angle);
    }

    // アームを下げる関数
    void unload() {
        Rservo.write(_down_angle);
        Lservo.write(180 - _down_angle);
    }
};

// ロボットハンドの制御を行うクラス
class Hand {
  private:
    Servo Rservo;
    Servo Lservo;
    const int _Rpin;
    const int _Lpin;
    int _opened_angle;
    int _closed_angle;

  public:
    // コンストラクタ
    Hand(int Rpin, int Lpin, int opened_angle, int closed_angle)
        : _Rpin(Rpin), _Lpin(Lpin), _opened_angle(opened_angle),
          _closed_angle(closed_angle) {
        Rservo.attach(_Rpin, 500, 2500);
        Lservo.attach(_Lpin, 500, 2500);
    }

    // 掴む関数
    void catching() {
        Rservo.write(_opened_angle);
        Lservo.write(_opened_angle);
    }

    // 放す関数
    void release() {
        Rservo.write(_closed_angle);
        Lservo.write(_closed_angle);
    }
};

// ベルトコンベアの制御を行うクラス
class Conveyor {
  private:
    const int _pin1;
    const int _pin2;
    const int _pin3;
    const int _pin4;
    int _MOTOR_STEPS; // １回転あたりのステップ数
    int _rpm;         // １分あたりの回転数
    int _step; // ベルトコンベアでものを落としきるのに必要なステップ数
    Stepper stepper;

  public:
    // コンストラクタ
    Conveyor(int pin1, int pin2, int pin3, int pin4, int MOTOR_STEPS, int rpm,
             int step)
        : _pin1(pin1), _pin2(pin2), _pin3(pin3), _pin4(pin4),
          _MOTOR_STEPS(MOTOR_STEPS), _rpm(rpm), _step(step),
          stepper(_MOTOR_STEPS, _pin1, _pin2, _pin3, _pin4) {
        stepper.setSpeed(_rpm);
    }

    // ベルトコンベアからものを落とす関数
    void convey() {
        stepper.step(_step);
        digitalWrite(_pin1, LOW);
        digitalWrite(_pin2, LOW);
        digitalWrite(_pin3, LOW);
        digitalWrite(_pin4, LOW);
    }
};

// 超音波センサーの制御を行うクラス
class Ultrasonic {
  private:
    int _front_trig_pin;
    int _front_echo_pin;
    int _back_trig_pin;
    int _back_echo_pin;

  public:
    Ultrasonic(int front_trig_pin, int front_echo_pin, int back_trig_pin,
               int back_echo_pin)
        : _front_trig_pin(front_trig_pin), _front_echo_pin(front_echo_pin),
          _back_trig_pin(back_trig_pin), _back_echo_pin(back_echo_pin) {
        pinMode(_front_echo_pin, INPUT);
        pinMode(_front_trig_pin, OUTPUT);
        pinMode(_back_echo_pin, INPUT);
        pinMode(_back_trig_pin, OUTPUT);
    }

    // 前方の障害物との距離を返す関数
    // 戻り値はの単位はcm
    double front_distance() {
        digitalWrite(_front_trig_pin, HIGH);
        delayMicroseconds(10);
        digitalWrite(_front_trig_pin, LOW);
        double duration = pulseIn(_front_echo_pin, HIGH);
        return duration * 340 * 100 / 1000000 / 2;
    }

    // 後方の障害物との距離を返す関数
    // 戻り値はの単位はcm
    double back_distance() {
        digitalWrite(_back_trig_pin, HIGH);
        delayMicroseconds(10);
        digitalWrite(_back_trig_pin, LOW);
        double duration = pulseIn(_back_echo_pin, HIGH);
        return duration * 340 * 100 / 1000000 / 2;
    }
};