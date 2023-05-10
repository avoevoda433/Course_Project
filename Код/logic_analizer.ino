#define MAX_V 3.3                     
#define MAX_DISCRET 4095              
#define D1_PORT 4                     
#define D2_PORT 2                     
#define SPEED_UPDATE_INTERVALE 1000   
#define BIT_INTERVALE 0.000667        

float D1_voltage = 0;
float D2_voltage = 0;                     
int count_SEO = 0;                       
int count_EOP = 0;                       
bool check_double_SEO = false;           
unsigned long previous_speed_time = 0;   
unsigned long previous_packet_time = 0;  

float get_voltage(int port_state){
  return (MAX_V * port_state) / MAX_DISCRET;
}

void find_EOP() {
  count_EOP++;
  count_SEO = 0;
  check_double_SEO = false;
}

void find_SEO() {
  if (get_voltage(analogRead(D1_PORT)) < 0.8 && 
      get_voltage(analogRead(D2_PORT)) < 0.8) {
    if (count_SEO == 0) {
      count_SEO++;
      check_double_SEO = true;
    }
    else if (count_SEO == 1) {
      find_EOP();
    }
  }
  if (count_SEO == 1 && !check_double_SEO) {
    count_SEO = 0;
    check_double_SEO = false;
  }
}

void speed(){
  if (count_EOP >= 499) {
    Serial.print(0); 
    Serial.println(" packets per second");
  }
  else {
    Serial.print(count_EOP); 
    Serial.println(" packets per second");
  }
  count_EOP = 0;
}

void setup() {
  Serial.begin(115200);
}

void loop() {
  unsigned long current_speed_time = millis();
  unsigned long current_packet_time = millis();
  if (current_speed_time - previous_speed_time >= SPEED_UPDATE_INTERVALE) {
    previous_speed_time = current_speed_time;
    speed();
  }
  if (current_packet_time - previous_packet_time >= BIT_INTERVALE) {
    previous_packet_time = current_packet_time;
    find_SEO();
  }
}
