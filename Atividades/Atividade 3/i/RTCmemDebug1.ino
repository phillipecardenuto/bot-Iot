#define Show(name,value) Serial.print(name); Serial.println(value);
#define RTCMEMORYSTART 65
#define RTCMEMORYLEN 4// was127


extern "C" {
#include "user_interface.h" // this is for the RTC memory read/write functions
}
#include <NTPtimeESP.h>
#define DEBUG_ON
NTPtime NTPbr("br.pool.ntp.org");   // Choose server pool as required
char *ssid      = "Betel";               // Set you WiFi SSID
char *password  = "itapoan8";     // Set you WiFi password
 byte hour;
  byte minute;
  byte sec;
  int year;
  byte month;
  byte day;
  byte dayofWeek;
/*
 * The structure strDateTime contains the following fields:
 * struct strDateTime
{
  byte hour;
  byte minute;
  byte second;
  int year;
  byte month;
  byte day;
  byte dayofWeek;
  boolean valid;
};
 */
strDateTime dateTime;

/*
 * The structure contains following fields:
 * struct strDateTime
{
  byte hour;
  byte minute;
  byte sec;
  int year;
  byte month;
  byte day;
  byte dayofWeek;
  boolean valid;
};
 */

typedef struct {
  byte hour;
  byte minute;
  byte sec;
  byte year;
  byte month;
  byte day;
  byte dayofWeek;
  boolean valid;
} rtcStore;

rtcStore rtcMem;    // record to be read/written = 8 bytes
typedef struct {
  byte toggleFlag;
  byte countrec;
  byte byte3;
  byte byte4;
} controlbucket;
controlbucket bucket64;

int i;
int nbuckets;         // 1 "bucket" = 4 bytes
byte toggleFlag;
//*****************************************
void Showrecord(int i, int rtcPos){
      Serial.print("i: ");
      Serial.print(i);
      Serial.print(" Pos ");
     Serial.print(rtcPos);
      Serial.print(" hour: ");
      Serial.print(rtcMem.hour);
      Serial.print(" min: ");
      Serial.print(rtcMem.minute);
      Serial.print(" sec: ");
      Serial.print( rtcMem.sec);
      Serial.print(" year: ");
      Serial.print(rtcMem.year);
      Serial.print(" month: ");
      Serial.print(rtcMem.month);
      Serial.print(" day: ");
      Serial.print(rtcMem.day);
      Serial.print(" day of Week: ");
      Serial.println(rtcMem.dayofWeek);
      yield();
    }
//************************************************
void setup() {
  Serial.begin(115200);
  delay(10);
  Serial.println();
  Serial.println("Booted");
  Show("Connecting to Wi-Fi ", ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin (ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("WiFi connected");
  Serial.println(true); Serial.println(false);
  //----------------------------------------
  // Connectto NTP Server pool:
  // first parameter: Time zone in floating point (for India);
  //second parameter: 1 for European summer time; 2 for US daylight saving time (contributed by viewwer, not tested by me)
  for (i=1; i<=10; i++){   //num of connection trials
    dateTime = NTPbr.getNTPtime(-3.0, 0);  //Brasilia UTC
    // check dateTime.valid before using the returned time
   // Use "setSendInterval" or "setRecvTimeout" if required
    if(dateTime.valid){
      NTPbr.printDateTime(dateTime);  // Display ate and time returned byNTP Server
      hour = dateTime.hour;
      minute = dateTime.minute;
      sec = dateTime.second;
      year = dateTime.year;
      Show("Current year= ", year);  //DEBUG
      month = dateTime.month;
      day =dateTime.day;
      dayofWeek = dateTime.dayofWeek;
      break;
    } else Serial.print("*"); //max 10 trials
    delay(1000);
  } 
  //************ Read/Write records to RTC Memory********** 
  nbuckets = (sizeof(rtcMem) / 4); //number of buckets/record =2
  if (nbuckets == 0) nbuckets = 1;
  Serial.print("Number of buckets/rtcMemrecord= ");
  Serial.println(nbuckets);
  system_rtc_mem_read(64, &bucket64, 4); //read 1st bucket value
  toggleFlag= bucket64.toggleFlag;
  Show("toggleFlag(0= read RTC, 1= write RTC): ",toggleFlag);
  Show("bucket countrec= ",bucket64.countrec);

  if (toggleFlag==1) {    //Write RTC     // 1st time it should contain garbage, so write RTC memory
    Serial.println("Start Writing");     // so will fill RTC memory
     rtcMem.hour = hour;         // i = 0..62
      rtcMem.minute = minute; //(i+1) * 11; // 0 .. 11*62= 782
      rtcMem.sec = sec;
      rtcMem.year = year%2000;
      rtcMem.month=month;
      rtcMem.day=day;
      rtcMem.dayofWeek=dayofWeek;
      i=bucket64.countrec;
      int rtcPos = RTCMEMORYSTART + i * nbuckets;  //bucket address fo i-th iteration
      system_rtc_mem_write(rtcPos, &rtcMem, nbuckets * 4);
      bucket64.toggleFlag = 0;   //
      bucket64.countrec++;   
      system_rtc_mem_write(64, &bucket64, 4);  
      Showrecord(i,rtcPos);
      Serial.println("Writing done");   
      yield();        // ditto..
  }
  else {                   //Read RTC              
    Serial.println("Start reading");
    for (i = 0; i <= bucket64.countrec; i++) {
      int rtcPos = RTCMEMORYSTART + i * nbuckets;
      system_rtc_mem_read(rtcPos, &rtcMem, sizeof(rtcMem));
      bucket64.toggleFlag = 1;        // toggle flag so will write again after Deep Sleep
      system_rtc_mem_write(64, &bucket64, 4);
      Showrecord(i,rtcPos);
      yield();
      }
    Serial.println("reading done");   // Now clear all buckets except the 1st bucket,
   /*  for (i = 0; i < RTCMEMORYLEN / nbuckets; i++)  //so we can be sure to write all of them,
      rtcMem.hour = 0;                          // after deep Sleep wake-up
      rtcMem.minute = 0;
      rtcMem.sec=0;
      rtcMem.year=0;
      rtcMem.month=0;
      rtcMem.day=0;
      rtcMem.dayofWeek=0;
      rtcMem.valid=0;
      int rtcPos = RTCMEMORYSTART + i * nbuckets;
      system_rtc_mem_write(rtcPos, &rtcMem, nbuckets * 4);
   */
    }
  
  Serial.println("delay 20 secs before sleep");
  delay(20000);
  //ESP.deepSleep(5000000, WAKE_RFCAL);
}
void loop() {}
