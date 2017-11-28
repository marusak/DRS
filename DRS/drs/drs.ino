#include <Wire.h>
#include <LSM303.h>
#include <LSM6.h>

LSM303 back;
LSM303::vector<int16_t> back_running_min = {32767, 32767, 32767}, back_running_max = {-32768, -32768, -32768};
LSM6 front;
LSM6::vector<int16_t> front_running_min = {32767, 32767, 32767}, front_running_max = {-32768, -32768, -32768};

char report[150];
int b_minX = -1046, b_minY = -311, b_minZ = - 400;
int b_maxX = 833, b_maxY = -10, b_maxZ = -300;
int f_minX = -1626, f_minY = -539, f_minZ = 889;
int f_maxX = 212, f_maxY = -88, f_maxZ = 2680;

void read_senzor(int *fx, int *fy, int *fz,
                 int *bx, int *by, int *bz){
    long sumFX = 0, sumFY = 0, sumFZ = 0;
    long sumBX = 0, sumBY = 0, sumBZ = 0;
    for(int i = 0; i < 10; i++){
        delay(10);
        front.read();
        back.read();
        sumFX += front.a.x;
        sumFY += front.a.y;
        sumFZ += front.g.z;
        sumBX += back.a.x;
        sumBY += back.a.y;
        //WHY 303 does not have gyro?:O
        //sumBZ += back.g.z;
    }

    *fx = sumFX / 10;
    *fy = sumFY / 10;
    *fz = sumFZ / 10;
    *bx = sumBX / 10;
    *by = sumBY / 10;
    *bz = sumBZ / 10;
}

void setup() {
    Serial.begin(9600);
    Wire.begin();
    front.init(LSM6::device_DS33, LSM6::sa0_high);
    front.enableDefault();
    front.read();
    back.init(LSM303::device_auto, LSM303::sa0_high);
    back.enableDefault();
    back.read();
}

void loop() {
    int avgValues[6];
    read_senzor(&avgValues[0], &avgValues[1], &avgValues[2],
                &avgValues[3], &avgValues[4], &avgValues[5]);
    /*
    if(avgValues[0] < f_maxX && avgValues[0] > f_minX)
        avgValues[0] = 0;
    if(avgValues[1] < f_maxY && avgValues[1] > f_minY)
        avgValues[1] = 0;
    if(avgValues[2] < f_maxZ && avgValues[2] > f_minZ)
        avgValues[2] = 0;
    if(avgValues[3] < b_maxX && avgValues[3] > b_minX)
        avgValues[3] = 0;
    if(avgValues[4] < b_maxY && avgValues[4] > b_minY)
        avgValues[4] = 0;
    if(avgValues[5] < b_maxZ && avgValues[5] > b_minZ)
        avgValues[5] = 0;
        */
    snprintf(report, sizeof(report), "{%+6d, %+6d, %+6d, %+6d, %+6d, %+6d}",
            avgValues[0], avgValues[1], avgValues[2], avgValues[3], avgValues[4], avgValues[5]);
    Serial.println(report);
    delay(100);
}
