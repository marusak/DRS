/*
 * Drifting Recognition Software
 *
 * Created as project on FIT BUT Brno, December 2017
 * Authors:
 *    Matej Marusak xmarus06@stud.fit.vutbr.cz
 *    Andrej Barna  xbarna01@stud.fit.vutbr.cz
 */

#include <Wire.h>
#include <LSM303.h>
#include <L3G.h>

// LSM303 holds accelerometer and magnetometer
LSM303 front;
LSM303::vector<int16_t> front_running_min = {32767, 32767, 32767}, front_running_max = {-32768, -32768, -32768};
// L3G holds gyroscope
L3G front_gyro;

// String for sending results to the serial
char report[40];

/* Maximum and minimum values for each axis.
 *  These values were measured in process of calibration.
 *  See Docs for more information.
 */
int f_minX = -935, f_minY = -927, f_minZ = -450;
int f_maxX = 611, f_maxY = 651, f_maxZ = 882;

/* Read 10 values from the senzor and return their average value.
 * It helps with interpolating of the noise.
 */
void read_senzor(int *fx, int *fy, int *fz){
    long sumFX = 0, sumFY = 0, sumFZ = 0;
    for(int i = 0; i < 10; i++){
        // Each 10 miliseconds get new readings
        delay(10);
        front.read();
        front_gyro.read();
        sumFX += front.a.x;
        sumFY += front.a.y;
        sumFZ += front_gyro.g.z;
    }

    // Calculate the average
    *fx = sumFX / 10;
    *fy = sumFY / 10;
    *fz = sumFZ / 10;
}

void setup() {
    Serial.begin(9600);
    Wire.begin();

    // Turn off build in pull-up rezistors. We use our own.
    digitalWrite(SCL, LOW);
    digitalWrite(SDA, LOW);

    // Prepare accelerometer and take testing reading
    front.init();
    front.enableDefault();
    front.read();

    // Prepare gyroscope and take testing reading
    front_gyro.init();
    front_gyro.enableDefault();
    front_gyro.read();
}

/* Normalize values.
 *  Each axis is split into three parts by minimum and maximum that was
 *  received from calibration.
 *
 *                      min          max
 *   --------------------|------------|------------------------------
 *        --> shift            0               <-- shift
 *
 *    All values smaller that minimum, are shifted by the value of minimum to the
 *    right, opposite is done for values bigger than max. Readings between these
 *    two values are considered to be 0.
 */
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


// The main infinite program loop
void loop() {
    int avgValues[3];

    // Read new values from IMU
    read_senzor(&avgValues[0], &avgValues[1], &avgValues[2]);

    // Normalize the received values
    avgValues[0] = normReading(avgValues[0], f_minX, f_maxX);
    avgValues[1] = normReading(avgValues[1], f_minY, f_maxY);
    avgValues[2] = normReading(avgValues[2], f_minZ, f_maxZ);

    // Propagate the values to the serial
    snprintf(report, sizeof(report), "{%+6d, %+6d, %+6d}",
            avgValues[0], avgValues[1], avgValues[2]);
    Serial.println(report);

    //No delay, delay is crated in the values reading
}
