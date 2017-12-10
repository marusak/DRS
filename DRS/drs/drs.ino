#include <Wire.h>
#include <LSM6.h>

LSM6 front;
LSM6::vector<int16_t> front_running_min = {32767, 32767, 32767}, front_running_max = {-32768, -32768, -32768};

char report[40];
int f_minX = -318, f_minY = -389, f_minZ = -410;
int f_maxX = -151, f_maxY = -124, f_maxZ = -273;

void read_senzor(int *fx, int *fy, int *fz){
    long sumFX = 0, sumFY = 0, sumFZ = 0;
    for(int i = 0; i < 10; i++){
        delay(10);
        front.read();
        sumFX += front.a.x;
        sumFY += front.a.y;
        sumFZ += front.g.z;
    }

    *fx = sumFX / 10;
    *fy = sumFY / 10;
    *fz = sumFZ / 10;
}

void setup() {
    Serial.begin(9600);
    Wire.begin();
    digitalWrite(SCL, LOW);
    digitalWrite(SDA, LOW);
    front.init(LSM6::device_DS33, LSM6::sa0_high);
    front.enableDefault();
    front.read();

}
int normReading(int value, int minV, int maxV){
    if(value < maxV){
      if(value > minV)
        value = 0;
      else
        value += minV;
    }
    else
        value -= maxV;

    return value;
}

void loop() {
    int avgValues[3];
    read_senzor(&avgValues[0], &avgValues[1], &avgValues[2]);
    avgValues[0] = normReading(avgValues[0], f_minX, f_maxX);
    avgValues[1] = normReading(avgValues[1], f_minY, f_maxY);
    avgValues[2] = normReading(avgValues[2], f_minZ, f_maxZ);
    snprintf(report, sizeof(report), "{%+6d, %+6d, %+6d}",
            avgValues[0], avgValues[1], avgValues[2]);
    Serial.println(report);
}
