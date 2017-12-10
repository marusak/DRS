#include <Wire.h>
#include <LSM303.h>
#include <L3G.h>

LSM303 front;
LSM303::vector<int16_t> front_running_min = {32767, 32767, 32767}, front_running_max = {-32768, -32768, -32768};
L3G front_gyro;

char report[40];
int f_minX = -2935, f_minY = -2927, f_minZ = -450;
int f_maxX = 1611, f_maxY = 1651, f_maxZ = 882;

void read_senzor(int *fx, int *fy, int *fz){
    long sumFX = 0, sumFY = 0, sumFZ = 0;
    for(int i = 0; i < 10; i++){
        delay(10);
        front.read();
        front_gyro.read();
        sumFX += front.a.x;
        sumFY += front.a.y;
        sumFZ += front_gyro.g.z;
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
    front.init();
    //front.init(LSM6::device_DS33, LSM6::sa0_high);
    front.enableDefault();
    front.read();
    front_gyro.init();
    front_gyro.enableDefault();
    front_gyro.read();

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
