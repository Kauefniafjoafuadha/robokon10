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
        pinMode(_startstop_R_pin11, OUTPUT);
        pinMode(_runbreak_R_pin10, OUTPUT);
        pinMode(_fwdrev_R_pin9, OUTPUT);
        pinMode(_VM_R_pin5, OUTPUT);
        pinMode(_startstop_L_pin11, OUTPUT);
        pinMode(_runbreak_L_pin10, OUTPUT);
        pinMode(_fwdrev_L_pin9, OUTPUT);
        pinMode(_VM_L_pin5, OUTPUT);
        digitalWrite(_startstop_R_pin11, HIGH);
        digitalWrite(_runbreak_R_pin10, HIGH);
        digitalWrite(_startstop_L_pin11, HIGH);
        digitalWrite(_runbreak_L_pin10, HIGH);
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
        digitalWrite(_startstop_R_pin11, HIGH);
        digitalWrite(_runbreak_R_pin10, HIGH);
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
        digitalWrite(_startstop_L_pin11, HIGH);
        digitalWrite(_runbreak_L_pin10, HIGH);
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
    privete : Servo Rservo;
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
        Rservo.attach(_Rpin, 500, 2400);
        Lservo.attach(_Lpin, 500, 2400);
    }
    // アームを上げる関数
    void lift() {
        Rservo.write(_up_angle);
        Lservo.write(_up_angle);
    }

    // アームを下げる関数
    void unload() {
        Rservo.write(_down_angle);
        Lservo.write(_down_angle);
    }
};

// ロボットハンドの制御を行うクラス
class Hand {
  private:
    const int _R1_pin;
    const int _R2_pin;
    const int _R3_pin;
    const int _R4_pin;
    const int _L1_pin;
    const int _L2_pin;
    const int _L3_pin;
    const int _L4_pin;
    int _MOTOR_STEPS; // １回転あたりのステップ数
    int _rpm;         // １分あたりの回転数
    int _step;        // ハンドを開閉するのに必要なstep数

  public:
    // コンストラクタ
    Hand(int R1_pin, int R2_pin, int R3_pin, int R4_pin, int L1_pin, int L2_pin,
         int L3_pin, int L4_pin, int MOTOR_STEP, int rpm, int step)
        : _R1_pin(R1_pin), _R2_pin(R2_pin), _R3_pin(R3_pin), _R4_pin(R4_pin),
          _L1_pin(_L1_pin), _L2_pin(L2_pin), _L3_pin(L3_pin), _L4_pin(L4_pin),
          _MOTOR_STEPS(MOTOR_STEP), _rpm(rpm), _step(step) {
        Stepper Rstepper(_MOTOR_STEPS, _R1_pin, _R2_pin, _R3_pin, _R4_pin);
        Stepper Lstepper(_MOTOR_STEPS, _L1_pin, _L2_pin, _L3_pin, _L4_pin);
        Rstepper.setSpeed(_rpm);
        Lstepper.setSpeed(_rpm);
    }

    // つかむ関数
    void catch () {
        Rstepper.step(_step);
        Lstepper.step(_step);
        digitalWrite(_R1_pin, LOW);
        digitalWrite(_R2_pin, LOW);
        digitalWrite(_R3_pin, LOW);
        digitalWrite(_R4_pin, LOW);
        digitalWrite(_L1_pin, LOW);
        digitalWrite(_L2_pin, LOW);
        digitalWrite(_L3_pin, LOW);
        digitalWrite(_L4_pin, LOW);
    }

    // 放す関数
    void release {
        Rstepper.step(step * (-1));
        Lstepper.step(step * (-1));
        digitalWrite(_R1_pin, LOW);
        digitalWrite(_R2_pin, LOW);
        digitalWrite(_R3_pin, LOW);
        digitalWrite(_R4_pin, LOW);
        digitalWrite(_L1_pin, LOW);
        digitalWrite(_L2_pin, LOW);
        digitalWrite(_L3_pin, LOW);
        digitalWrite(_L4_pin, LOW);
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
  public:
    // コンストラクタ
    Conveyor(int pin1, int pin2, int pin3, int pin4, int MOTOR_STEPS, int rpm,
             int step)
        : _pin1(pin1), _pin2(pin2), _pin3(pin3), _pin4(pin4),
          _MOTOR_STEPS(MOTOR_STEPS), _rpm(rpm), _step(step) {
        Stepper stepper(_MOTOR_STEPS, _pin1, _pin2, _pin3, _pin4);
        stepper.setpeed(_rpm)
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

// フォトリフレクタの制御を行うクラス
class Photo {
  private:
    int _front_pin;
    int _back_pin;
    int _left_pin;
    int _right_pin;
    int _boundary; // 白と黒の境界を表す数値。これより値が小さいと黒と認識する。
  public:
    // コンストラクタ
    Photo(int front_pin, int back_pin, int left_pin, int right_pin,
          int boundary)
        : _front_pin(front_pin), _back_pin(back_pin), _left_pin(left_pin),
          _right_pin(right_pin), _boundary(boundary) {}
    // フォトリフレクタが検知している情報を受け取る関数
    // 黒を検知すると1を返し、それ以外を検知すると0を返す。
    int detect(int pin) {
        int val = analogRead(pin);
        if (val <= _boundary) {
            return 1;
        } else {
            return 0;
        }
    }

    // 地面のラインの模様を読み取る関数
    // 以下戻り値
    // 0:条件に当てはまらない模様の検出
    // 1:すべてがラインを認識しなかった
    // 2:十字を認識
    int detet_pattern() {
        int front = detect(_front_pin);
        int back = detect(_back_pin);
        int left = detect(_left_pin);
        int right = detect(_right_pin);
        // ここから下に読み取りたい模様を受け取ると特定の数字を返す文を追加すること
        if (front + back + left + right == 0) {
            return 1;
        } else if (front + back + left + right == 4) {
            return 2;
        }

        return 0;
    };
};

// 超音波センサーの制御を行うクラス
class Ultrasonic {
  private:
    int _trig_pin;
    int _echo_pin;

  public:
    Ultrasonic(int trig_pin, int echo_pin)
        : _trig_pin(trig_pin), _echo_pin(echo_pin) {
        pinMode(_echo_pin, INPUT);
        pinMode(_trig_pin, OUTPUT);
    }

    // 壁との距離を返す関数
    // 戻り値はの単位はcm
    int distance() {
        digitalWrite(trig_pin, HIGH);
        delayMicroseconds(10);
        digitalWrite(trig_pin, LOW);
        duration = pulseIn(echo_pin, HIGH);
        return duration * 340 * 100 / 1000000 / 2;
    }
};