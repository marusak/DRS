#include <Wire.h>
#include <LSM303.h>

#define CALIBRATE 0

LSM303 compass;
LSM303::vector<int16_t> running_min = {32767, 32767, 32767}, running_max = {-32768, -32768, -32768};

char report[140];
int minX = -1626, minY = -539, minZ = 889;
int maxX = 212, maxY = -88, maxZ = 2680;

void read_avg(int *rx, int *ry, int *rz){
    long sumX = 0, sumY = 0, sumZ = 0;
    for(int i = 0; i < 10; i++){
        delay(10);
        compass.read();
        sumX += compass.a.x;
        sumY += compass.a.y;
        sumZ += compass.a.z;
    }

    *rx = sumX / 10;
    *ry = sumY / 10;
    *rz = sumZ / 10;
}

void setup() {
    Serial.begin(9600);
    Wire.begin();
    compass.init();
    compass.enableDefault();
    compass.read();
}

void calibrate() {
    int values[3];
    read_avg(&values[0], &values[1], &values[2]);
    running_min.x = min(running_min.x, values[0]);
    running_min.y = min(running_min.y, values[1]);
    running_min.z = min(running_min.z, values[2]);

    running_max.x = max(running_max.x, values[0]);
    running_max.y = max(running_max.y, values[1]);
    running_max.z = max(running_max.z, values[2]);

    snprintf(report, sizeof(report), "min: {%+6d, %+6d, %+6d}  max: {%+6d, %+6d, %+6d}",
        running_min.x, running_min.y, running_min.z, running_max.x, running_max.y, running_min.z);
    Serial.println(report);
}

void loop() {
    if (CALIBRATE){
        calibrate();
        delay(100);
    } else {
        int avgValues[3];
        read_avg(&avgValues[0], &avgValues[1], &avgValues[2]);
        if(avgValues[0] < maxX && avgValues[0] > minX)
            avgValues[0] = 0;
        if(avgValues[1] < maxY && avgValues[1] > minY)
            avgValues[1] = 0;
        if(avgValues[2] < maxZ && avgValues[2] > minZ)
            avgValues[2] = 0;
        snprintf(report, sizeof(report), "{%+6d, %+6d}", avgValues[0], avgValues[1]);
        Serial.println(report);
        delay(100);
    }
}
