#define Show(name,value) Serial.print(name); Serial.println(value);
#define RTCMEMORYSTART 65
#define RTCMEMORYLEN 223// was127
//#define RESET   // ONLY FIRST TIME RUNNING SKETCH

#include "DHT.h"
#define DHTPIN D2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);


typedef struct {
  byte hour;
  byte minute;
  byte sec;
  byte year;
  byte month;
  byte day;
}_time;
_time time_temp_gt, time_hum_gt, time_light_gt,time_temp_lw, time_hum_lw, time_light_lw;
short unsigned int stats[6] = {0,-1,0,-1,0,-1};

typedef struct {  // write this on RTC memory
  byte hour;
  byte minute;
  byte sec;
  byte year;
  byte month;
  byte day;
  byte dayofWeek;
  byte temperature;
  byte humity;
  short int light;
  boolean valid;

} rtcStore;       // size= 12 bytes =  3 RTC "buckets"
rtcStore rtcMem;    // record to be read/written = 8 bytes


void update_time(String s){
    if (s == "temp_gt"){
      time_temp_gt.hour = rtcMem.hour;
      time_temp_gt.minute = rtcMem.minute;
      time_temp_gt.sec = rtcMem.sec;

      time_temp_gt.year = rtcMem.year;
      time_temp_gt.month= rtcMem.month;
      time_temp_gt.day = rtcMem.day;

    }
    else if (s == "hum_gt"){
      time_hum_gt.hour = rtcMem.hour;
      time_hum_gt.minute = rtcMem.minute;
      time_hum_gt.sec = rtcMem.sec;

      time_hum_gt.year = rtcMem.year;
      time_hum_gt.month= rtcMem.month;
      time_hum_gt.day = rtcMem.day;
    }
    else if (s == "light_gt"){
      time_light_gt.hour = rtcMem.hour;
      time_light_gt.minute = rtcMem.minute;
      time_light_gt.sec = rtcMem.sec;

      time_light_gt.year = rtcMem.year;
      time_light_gt.month= rtcMem.month;
      time_light_gt.day = rtcMem.day;
    }
    else if (s == "temp_lw"){
      time_temp_lw.hour = rtcMem.hour;
      time_temp_lw.minute = rtcMem.minute;
      time_temp_lw.sec = rtcMem.sec;

      time_temp_lw.year = rtcMem.year;
      time_temp_lw.month= rtcMem.month;
      time_temp_lw.day = rtcMem.day;
    }
    else if (s == "hum_lw"){
      time_hum_lw.hour = rtcMem.hour;
      time_hum_lw.minute = rtcMem.minute;
      time_hum_lw.sec = rtcMem.sec;

      time_hum_lw.year = rtcMem.year;
      time_hum_lw.month= rtcMem.month;
      time_hum_lw.day = rtcMem.day;
    }
    else if (s == "light_lw"){
      time_light_lw.hour = rtcMem.hour;
      time_light_lw.minute = rtcMem.minute;
      time_light_lw.sec = rtcMem.sec;

      time_light_lw.year = rtcMem.year;
      time_light_lw.month= rtcMem.month;
      time_light_lw.day = rtcMem.day;
    }
}




short int getLight() {
  // read the input on analog pin 0:
  return (short int)(analogRead(A0));
}

byte getTemp(){
  return (byte)dht.readTemperature();
}

byte getHumity(){
  return (byte)dht.readHumidity();
}


extern "C" {
#include "user_interface.h" // this is for the RTC memory read/write functions
}
#include <NTPtimeESP.h>
#define DEBUG_ON


NTPtime NTPbr("br.pool.ntp.org");   // Choose server pool as required
char *ssid      = "aula-ic3";               // Set you WiFi SSID
char *password  = "iotic@2019";     // Set you WiFi password
byte hour;
byte minute;
byte sec;
int year;
byte month;
byte day;
byte dayofWeek;
byte temperature;
byte humity;
short int light;

void print_date(_time t){
  Serial.print(t.hour);Serial.print("/");Serial.print(t.minute);Serial.print("/");Serial.print(t.sec);Serial.println();
  Serial.print(t.day);Serial.print("/");Serial.print(t.month);Serial.print("/");Serial.print(t.year);Serial.println();
 }

strDateTime dateTime;

typedef struct {
  byte toggleFlag;
  byte countrec;
  byte byte3;  //unused
  byte byte4;   //unused
} controlbucket;
controlbucket bucket64; // read/write state control bucket

int i, rtcPos;
const int nbuckets= sizeof(rtcMem)/4;    // buckets/record, 1 "bucket" = 4 bytes
byte toggleFlag;  // 0= read , 1= write
//**********************************************
void ClearRTCmemory(){
  for (i=0; i<RTCMEMORYLEN/nbuckets; i++){
     rtcMem.hour = 0;
      rtcMem.minute = 0;
      rtcMem.sec = 0;
      rtcMem.year = 0;
      rtcMem.month=0;
      rtcMem.day=0;
      rtcMem.dayofWeek= 0;
      rtcMem.temperature= 0;
      rtcMem.humity= 0;
      rtcMem.light= 0;
      rtcMem.valid=0;
  }
}

//*****************************************
void Showrecord(int i, int rtcPos){  // Displays record i
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
      Serial.print(" temperature: ");
      Serial.print(rtcMem.temperature);
      Serial.print(" humity: ");
      Serial.print(rtcMem.humity);
      Serial.print(" light: ");
      Serial.println(rtcMem.light);
      yield();
    }
//************************************************
void setup() {
  Serial.begin(115200);
  delay(10);
  Serial.println();
  Serial.println("Booted");
  //Show("Connecting to Wi-Fi ", ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin (ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    //Serial.print(".");
    delay(500);
  }
  Serial.println("WiFi connected");
  //----------------------------------------
  // Connect to NTP Server pool:
  // first parameter: Time zone in floating point (for India);
  //second parameter: 1 for European summer time; 2 for US daylight saving time (contributed by viewwer, not tested by me)
  for (i=1; i<=10; i++){   //num of connection trials
    dateTime = NTPbr.getNTPtime(-3.0, 0);  //Brasilia UTC
    // check dateTime.valid before using the returned time
   // Use "setSendInterval" or "setRecvTimeout" if required
    delay(1000);
    if(dateTime.valid){
      NTPbr.printDateTime(dateTime);  // Display ate and time returned byNTP Server
      hour = dateTime.hour;
      minute = dateTime.minute;
      sec = dateTime.second;
      year = dateTime.year;
      //Show("Current year= ", year);  //DEBUG
      month = dateTime.month;
      day =dateTime.day;
      dayofWeek = dateTime.dayofWeek;

      temperature = getTemp();
      humity = getHumity();
      light = getLight();

      break;
    } else Serial.print("*"); //max 10 trials
  }
  //*********************************************
  #ifdef RESET
    Serial.println ("RESET: clear RTC memory and initialize control bucket");
    bucket64.toggleFlag=1; //start writing
    bucket64.countrec=0;
    system_rtc_mem_write(64, &bucket64, 4);
    ClearRTCmemory();
  #endif
  //************ Read/Write records to RTC Memory**********
  Show("Number of buckets/rtcMemrecord= ",nbuckets);
  system_rtc_mem_read(64, &bucket64, 4); //read 1st bucket value
  toggleFlag= bucket64.toggleFlag;
  Show("toggleFlag(0= read, 1= write): ",toggleFlag);
  Show("current bucket record count= ",bucket64.countrec);
  if (toggleFlag==1) {    //Write RTC     // 1st time it should contain garbage, so write RTC memory
    Serial.println("Start Writing");     // so will fill RTC memory

      rtcMem.hour = hour;         // i = 0..62
      rtcMem.minute = minute; //(i+1) * 11; // 0 .. 11*62= 782
      rtcMem.sec = sec;
      rtcMem.year = year%2000;
      rtcMem.month=month;
      rtcMem.day=day;
      rtcMem.dayofWeek=dayofWeek;

      rtcMem.temperature=temperature;
      rtcMem.humity=humity;
      rtcMem.light=light;


      i=bucket64.countrec;
      int rtcPos = RTCMEMORYSTART + i * nbuckets;  //bucket address fo i-th iteration
      system_rtc_mem_write(rtcPos, &rtcMem, nbuckets * 4);
      bucket64.toggleFlag = 0;   // read on next iteration
      bucket64.countrec++;
      system_rtc_mem_write(64, &bucket64, 4);
      Showrecord(i,rtcPos);
      Serial.println("Writing done");

      yield();        // ditto..
  }
  else {                   //Read RTC
    if (bucket64.countrec >= 12){
        Serial.println ("RESET: clear RTC memory and initialize control bucket");
        bucket64.toggleFlag=1; //start writing
        bucket64.countrec=0;
        system_rtc_mem_write(64, &bucket64, 4);
        ClearRTCmemory();
    }
    Serial.println("Start reading");
    for (i = 0; i < bucket64.countrec; i++) {
      int rtcPos = RTCMEMORYSTART + i * nbuckets;
      system_rtc_mem_read(rtcPos, &rtcMem, sizeof(rtcMem));

       if(rtcMem.temperature >= stats[0]){
        update_time("temp_gt");
        stats[0] = rtcMem.temperature;
      }
      if( rtcMem.temperature <= stats[1]){
        update_time("temp_lw");
        stats[1] = rtcMem.temperature;
      }
      if( rtcMem.humity >= stats[2]){
        update_time("hum_gt");
        stats[2] = rtcMem.humity;
      }
       if( rtcMem.humity <= stats[3]){
        update_time("hum_lw");
        stats[3] = rtcMem.humity;
      }
      if( rtcMem.light >= stats[4]){
        update_time("light_gt");
        stats[4] = rtcMem.light;
      }
       if( rtcMem.light <= stats[5]){
        update_time("light_lw");
        stats[5] = rtcMem.light;
      }

      bucket64.toggleFlag = 1;        // toggle flag so will write again after Deep Sleep
      system_rtc_mem_write(64, &bucket64, 4);
      Showrecord(i,rtcPos);
      yield();
      }
      Serial.println("Print STATS\n");
      time_temp_gt, time_hum_gt, time_light_gt,time_temp_lw, time_hum_lw, time_light_lw;
      Serial.print("GT Temp: ");Serial.println(stats[0]);print_date(time_temp_gt);
      Serial.print("LW Temp: ");Serial.println(stats[1]);print_date(time_temp_lw);
      Serial.print("GT Humity: ");Serial.println(stats[2]);print_date(time_hum_gt);
      Serial.print("LW Humity: ");Serial.println(stats[3]);print_date(time_hum_lw);
      Serial.print("GT Light: ");Serial.println(stats[4]);print_date(time_light_gt);
      Serial.print("LW Light: ");Serial.println(stats[5]);print_date(time_light_lw);

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
