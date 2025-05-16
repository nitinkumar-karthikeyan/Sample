#include <math.h>

const int B = 4275000;
const int R0 = 100000;
const int pinTempSensor = A0;

float sampling = 1.0; // Initial sampling rate 
const float min_sampling = 0.5; // Minimum sampling rate 
const float max_sampling = 4.0; // Maximum sampling rate 
const int samples = 60;  // Number of samples per cycle 
float temperature_value[samples]; // Temperature readings for current cycle

float temp_variations[10];  // Array to store temperature readings for current cycle
int cycle_count = 0; // Counts cycles for moving average

#define ACTIVE 0
#define IDLE 1
#define POWER_DOWN 2

int current_mode = ACTIVE; //Variable to store current power mode          

const float active = 3.0;
const float idle = 1.0;              
const int power_down = 5;       

void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.println("Collecting temperature data for 1 minute...");
  collect_temp_data();  // Collect temperature samples for one cycle

  float variation = calc_variation(); // To calculate total variation in this cycle
  float dominant_freq = apply_DFT(); // To find dominant frequency using DFT
  float trend_variation = moving_avg(variation); // To predict future variation using moving average

  adjust_sampling_rate(dominant_freq);  // Adjust sampling rate depending on dominant frequency
  current_mode = decide_power_mode(trend_variation);  //To decide power mode based on predicted variation

  print_output(trend_variation, dominant_freq);

  // If in power down mode, stop the system 
  if (current_mode == POWER_DOWN) {
    Serial.println("System entering Power Down Mode. Monitoring stopped.");
    while (true); // Infinite loop to simulate sleep
  }

  delay(1000); // Wait before next monitoring cycle
}


void collect_temp_data() {
  for (int i = 0; i < samples; i++) {
    int reading = analogRead(pinTempSensor);
    
    float R = (1023.0 / reading - 1.0);
    R = R0 * R;
    float temperature = 1.0 / (log(R / R0) / B + 1 / 298.15) - 273.15; 
    temperature_value[i] = temperature; 
    
    delay(1000 / sampling);
  }
}


float calc_variation() {
  float variation = 0;
  for (int i = 1; i < samples; i++) {
    variation = variation + abs(temperature_value[i] - temperature_value[i - 1]);
  }
  return variation;
}


float apply_DFT() {
  float max_magnitude = 0;
  int max_index = 1; // Skip DC component 

  for (int k = 1; k < samples / 2; k++) {
    float real = 0;
    float imag = 0;
    for (int j = 0; j < samples; j++) {
      float angle = 2 * PI * j * k / samples;
      real = real + temperature_value[j] * cos(angle);
      imag = imag - temperature_value[j] * sin(angle);
    }
    
    float magnitude = sqrt(real * real + imag * imag);
    
    if (magnitude > max_magnitude) {
      max_magnitude = magnitude;
      max_index = k;
    }
  }
  // Convert bin index to frequency (Hz)
  return (float)max_index / samples * sampling;
}


float moving_avg(float new_variation) {
  temp_variations[cycle_count % 10] = new_variation; // To store latest variation in buffer
  float sum = 0;
  for (int i = 0; i < 10; i++) {
    sum = sum + temp_variations[i];  // Sum for moving average
  }
  cycle_count++;
  return sum / 10;
}


int decide_power_mode(float predicted_variation) {
  if (predicted_variation >= 3.0) {
    return ACTIVE;
  } else if (predicted_variation >= 1.0) {
    return IDLE;
  } else {
   return POWER_DOWN;
  }
}

void adjust_sampling_rate(float freq) {
  float new_rate = 2.0 * freq; // Nyquist: at least twice the dominant frequency
  if (new_rate < min_sampling) new_rate = min_sampling;
  if (new_rate > max_sampling) new_rate = max_sampling;
  sampling = new_rate;
}


void print_output(float trend_var, float dom_freq) {
  Serial.print("Predicted Variation: ");
  Serial.print(trend_var, 2);
  Serial.print(", Dominant Frequency: ");
  Serial.print(dom_freq, 2);
  Serial.print(" Hz, New Sampling Rate: ");
  Serial.print(sampling, 2);
  Serial.print(" Hz, Power Mode: ");
  if (current_mode == ACTIVE){
    Serial.println("ACTIVE");
  }
  else if (current_mode == IDLE) 
  {
    Serial.println("IDLE");
  }
  else{ 
    Serial.println("POWER_DOWN");
}
}
