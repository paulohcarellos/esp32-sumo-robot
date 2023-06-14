
#include <string>

// CONTROL STATE
#define TELEOPERATED 0
#define AUTONOMOUS 1
#define SHUTDOWN 2

// AUTONOMOUS MODE
#define SEARCH 0
#define ATTACK 1
#define REVERSE 2

int state = 0;
int mode = 0;
bool timeout = false;

int presenceSensor = 0;
bool lineSensor1 = false;
bool lineSensor2 = false;

float inputX = 0;
float inputY = 0;

float rightSpeed = 0;
float leftSpeed = 0;

void setRight(int velocity);
void setLeft(int velocity);

void sleep(int seconds);

void rotateLeft() {
    setRight(1);
    setLeft(-1);
}

void charge() {
    setRight(1);
    setLeft(1);
}

void stepBack() {
    setRight(-1);
    setLeft(-1);

    sleep(1);

    setRight(0);
    setLeft(0);
}

void receiveBluetooth() {
    std::string command = "-100  100 0";

    inputX = std::stoi(command.substr(0, 5));
    inputX = std::stoi(command.substr(6, 5));

    int newState = std::stoi(command.substr(12, 1));

    if (state == TELEOPERATED && newState == AUTONOMOUS)
        timeout = true;

    state = newState;
}

void sendBluetooth() {
    std::string sensors = "XXX";

    sensors[0] = toascii(presenceSensor);
    sensors[1] = toascii(lineSensor1);
    sensors[2] = toascii(lineSensor2);
}

void updateSensor() {
    presenceSensor = 0; // getDistance();

    lineSensor1 = false; // PINO_X
    lineSensor2 = false; // PINO_X
}

void calculateSpeed(float &rightSpeed, float &leftSpeed);

void loop() {
    receiveBluetooth();
    // updateSensors();

    if (state == 0) {
        sendBluetooth();
        calculateSpeed(rightSpeed, leftSpeed);
        setRight(rightSpeed);
        setLeft(leftSpeed);
    }

    if (state == 1) {
        if (timeout) {
            sleep(5);
            timeout = false;
        }

        mode = SEARCH;

        if (presenceSensor < 50)
            mode = ATTACK;

        if (lineSensor1 || lineSensor2)
            mode = REVERSE;

        if (mode == SEARCH) {
            rotateLeft();
        }

        if (mode == ATTACK) {
            charge();
        }

        if (mode == REVERSE) {
            stepBack();
        }
    }

    if (state == 2) {
        sleep(99999); // esp_deep_sleep_start();
    }
}