/*
  file: number.ino
  description: led masks for number-clock mode
  date: 08/11/2015
  author: Ismael Salvador
*/

#if (NUMBER_CLOCK)

#include "DHT.h"
extern DHT dht;
extern int weather;

#include "number.h" 

//m_day_ten
//m_day_unit
//m_month
//m_hour
//m_min
//m_min_unit
//m_weather
//m_arrow
//m_pulse
//m_moon



void set_led_number_weather(){
  int weather;
  if (USE_DHT11) {      
     //refresh measures every half minute, it takes sometime in DHT11
     if ((csecond % 30) == 0) {
       humidity    = (int) dht.readHumidity();
       temperature = (int) dht.readTemperature();
       dew_point   = dewPoint(temperature, humidity);
     }
               
     //show it for 2 seconds every half minute or pressed button
     if (((csecond % 30) < 5) || (LOW == digitalRead(BUTTON_0))) {
      fill_matrix(0);
      set_led_mask(m_hour, (int)(humidity/10));   //use hour to display humidity
      set_led_mask(m_min, (int)temperature/10); //use mins to display temperature
      set_led_mask(m_min_unit, (int)temperature%10); 
     }
     
     //show dew point
     if (dew_point > 18)
       weather = 2; //humid
     else if (dew_point > 10 )
       weather = 1; //confortable
     else 
       weather = 0; //dry

     set_led_mask(m_weather, weather);  
  }
  
}

//main function
void set_led_number_clock(){ 
  set_led_mask(m_hour, chour%12);
  set_led_mask(m_min, cminute/10);
  set_led_mask(m_min_unit, cminute%10);
  
  if(csecond%2)
    set_led_mask(m_pulse, 0);
   
  set_led_number_weather();
  
  moon = moon_phase(cyear, cmonth, cday);
  set_led_mask(m_moon, moon/2); //0-7 phases

  set_led_mask(m_arrow, csecond/15);

  if ((cday/10) > 0)
    set_led_mask(m_day_ten, cday/10);
  set_led_mask(m_day_unit, cday%10);
  
  set_led_mask(m_month, cmonth-1);
}


#endif