
#define RTCMEMORYSTART 65
#define RTCMEMORYLEN 12


extern "C" {
#include "user_interface.h" // this is for the RTC memory read/write functions
}

typedef struct {
  int battery;
  int other;
} rtcStore;

rtcStore rtcMem;    // record to be read/written = 8 bytes

int i;
int buckets;         // 1 "bucket" = 4 bytes
bool toggleFlag;

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("Start");
  buckets = (sizeof(rtcMem) / 4); //number of buckets/record =2
  if (buckets == 0) buckets = 1;
  Serial.print("Number of buckets/rtcMem record= ");
  Serial.println(buckets);
  system_rtc_mem_read(64, &toggleFlag, 4); //read 1st bucket value
  Serial.print("toggle Flag= ");
  Serial.println(toggleFlag);
  if (toggleFlag) {         // 1st time it should contain garbage, so write RTC memory
    Serial.println("Start Writing");     // so will fill RTC memory
    for (i = 0; i < RTCMEMORYLEN / buckets; i++) { // we have space for 63 rtcMem records
      rtcMem.battery = i;         // i = 0..62
      rtcMem.other = i * 11;      // 0 .. 11*62= 782
      int rtcPos = RTCMEMORYSTART + i * buckets;  //bucket address fo i-th iteration
      system_rtc_mem_write(rtcPos, &rtcMem, buckets * 4);
      toggleFlag = false;         // wil write it at each iteration
      system_rtc_mem_write(64, &toggleFlag, 4);  // just in case???
      Serial.print("i: ");                 // show all we have written:
      Serial.print(i);                     // iteration count,
      Serial.print(" Position: ");         
      Serial.print(rtcPos);                 // bucket number,
      Serial.print(", battery: ");
      Serial.print(rtcMem.battery);        // 1st record field,
      Serial.print(", other: ");
      Serial.println(rtcMem.other);        // and 2nd record field
      yield();
    }
    Serial.println("Writing done");       // ditto..
  }
  else {                                 
    Serial.println("Start reading");
    for (i = 0; i < RTCMEMORYLEN / buckets; i++) {
      int rtcPos = RTCMEMORYSTART + i * buckets;
      system_rtc_mem_read(rtcPos, &rtcMem, sizeof(rtcMem));
      toggleFlag = true;        // toggle flag so will write again after Deep Sleep
      system_rtc_mem_write(64, &toggleFlag, 4);

      Serial.print("i: ");
      Serial.print(i);
      Serial.print(" Position ");
      Serial.print(rtcPos);
      Serial.print(", battery: ");
      Serial.print(rtcMem.battery);
      Serial.print(", other: ");
      Serial.println(rtcMem.other);
      yield();
    }
    Serial.println("reading done");   // Now clear all buckets except the 1st bucket,
    for (i = 0; i < RTCMEMORYLEN / buckets; i++) { //so we can be sure to write all of them,
      rtcMem.battery = 0;                          // after deep Sleep wake-up
      rtcMem.other = 0;
      int rtcPos = RTCMEMORYSTART + i * buckets;
      system_rtc_mem_write(rtcPos, &rtcMem, buckets * 4);
    }
  }
  Serial.println("delay 20 secs before sleep");
  delay(20000);
  ESP.deepSleep(5000000, WAKE_RFCAL);
}
void loop() {}
