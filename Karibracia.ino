#include <Wire.h>
#include <LSM303.h>

// Na stole polozene napojene do breadboardu
// min: {+16389,   -504,  +1902}    max: {+16490,   -425,  +1977}
// min: {+16309,   -517,  +1800}    max: {+16533,   -430,  +1921}
// ------- 
// min: {+16374,   -450,  +1870}    max: {+16477,   -408,  +1963}


// Dole hlavou
// min: {-16718,   -221,   +922}    max: {-16606,   -141,  +1097}
// ------- 
//  min: {-16646,   -125,  +2565}    max: {-16570,    -88,  +2680}


// Na boku (hore je diera, kolajnicky na stranach
// min: {   -46, -16735,  +1201}    max: {   +54, -16657,  +1498}
// ------- 
//  min: {  -948, -16696,  +1247}    max: {  -750, -16646,  +1315}


// Na opacnom boku (hore je cuplik)
// min: {  +441, +15874,   +919}    max: {  +547, +15953,  +1005}
// ------- 
//  min: {  +160, +15889,   +889}    max: {  +212, +15960,   +957}


// Na kolajnicke (senzor je pod kablami)
// min: {  -796,   -172, +17036}    max: {  -693,    -77, +17144}
// ------- 
//  min: {  -514,   -138, +17037}    max: {  -471,   -105, +17123}


// Na opacnej kolajnicke, senzor je nad kablami
// min: { +1090,   -547, -14895}    max: { +1171,   -483, -14810}
// ------- 
//  min: { -1626,   -539, -14860}    max: { -1551,   -484, -14778}


int modulo = 0;
int xHistory[10];
int yHistory[10];
int zHistory[10];


LSM303 compass;
LSM303::vector<int16_t> running_min = {32767, 32767, 32767}, running_max = {-32768, -32768, -32768};

char report[140];
int minX = -1626, minY = -539, minZ = 889;
int maxX = 212, maxY = -88, maxZ = 2680;

int* calibration(){
  
  long sumX = 0, sumY = 0, sumZ = 0;
  for(int i = 0; i < 10; i++){
    delay(10);
    compass.read();
    sumX += compass.a.x;
    sumY += compass.a.y;
    sumZ += compass.a.z;
  }

  int* retValues = malloc(sizeof(int)*3);
  retValues[0] = sumX/10;
  retValues[1] = sumY/10;
  retValues[2] = sumZ/10;

  return retValues;
}


int* averageValues(){
  xHistory[modulo] = compass.a.x;
  yHistory[modulo] = compass.a.y;
  zHistory[modulo] = compass.a.z;
  if(++modulo > 9)
    modulo = 0;

  //minX = minY = minZ = 0x7FFF;
  //maxX = maxY = maxZ = 0x8000;
  
  long sumX = 0, sumY = 0, sumZ = 0;
  for(int i = 0; i < 10; i++){
    sumX += xHistory[i];
    sumY += yHistory[i];
    sumZ += zHistory[i];
    /*
    if(xHistory[i] < minX)
      minX = xHistory[i];
    if(yHistory[i] < minY)
      minY = yHistory[i];
    if(zHistory[i] < minZ)
      minZ = zHistory[i];
    if(xHistory[i] > maxX)
      maxX = xHistory[i];
    if(yHistory[i] > maxY)
      maxY = yHistory[i];
    if(zHistory[i] > maxZ)
      maxZ = zHistory[i];
      */
  }

  int* retValues = malloc(sizeof(int)*3);
  retValues[0] = sumX/10;
  retValues[1] = sumY/10;
  retValues[2] = sumZ/10;

  return retValues;
}

void setup() {
  Serial.begin(9600);
  Wire.begin();
  compass.init();
  compass.enableDefault();
  compass.read();
  int *values = calibration();
  running_min.x = values[0];
  running_max.x = values[0];
  running_min.y = values[1];
  running_min.y = values[1];
  running_min.z = values[2];
  running_min.z = values[2];
}

void loop() {
  //int *values = calibration();
  
/*  running_min.x = min(running_min.x, values[0]);
  running_min.y = min(running_min.y, values[1]);
  running_min.z = min(running_min.z, values[2]);

  running_max.x = max(running_max.x, values[0]);
  running_max.y = max(running_max.y, values[1]);
  running_max.z = max(running_max.z, values[2]);


  long x = map(compass.a.x, minX, maxX, -1000, 1000);
  long y = map(compass.a.y, minY, maxY, -1000, 1000);
  long z = map(compass.a.z, minZ, maxZ, -1000, 1000);
*/
  int* avgValues;
  avgValues = calibration();
  /*
  snprintf(report, sizeof(report), "{%+6d, %+6d, %+6d}\tmin: {%+6d, %+6d, %+6d}    max: {%+6d, %+6d, %+6d} --> scaled: {%+6d, %+6d, %+6d}",
    compass.a.x, compass.a.y, compass.a.z,
    running_min.x, running_min.y, running_min.z,
    running_max.x, running_max.y, running_max.z,
    x, y, z);
  */

  if(avgValues[0] < maxX && avgValues[0] > minX)
    avgValues[0] = 0;
  if(avgValues[1] < maxY && avgValues[1] > minY)
    avgValues[1] = 0;
  if(avgValues[2] < maxZ && avgValues[2] > minZ)
    avgValues[2] = 0;

  /*snprintf(report, sizeof(report), "\tmin: {%+6d, %+6d, %+6d}    max: {%+6d, %+6d, %+6d}",
  running_min.x ,running_min.y ,running_min.z ,running_max.x ,running_max.y ,running_max.z);
*//*
  snprintf(report, sizeof(report), "{%+6d, %+6d, %+6d}\tmin: {%+6d, %+6d, %+6d}    max: {%+6d, %+6d, %+6d}",
    avgValues[0], avgValues[1], avgValues[2],
    minX, minY, minZ, maxX, maxY, maxZ);*/
  snprintf(report, sizeof(report), "{%+6d, %+6d}",
    avgValues[0], avgValues[1]);
  
  free(avgValues);
  Serial.println(report);
  
  delay(100);
}
