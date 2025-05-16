#include <math.h>

const int B = 4275000;            // B value of the thermistor
const int R0 = 100000;            // R0 = 100k
const int pinTempSensor = A0;     // Grove - Temperature Sensor connect to A0

const int samples = 180; // 180 samples

float temperature_value[samples]; // Array for storing temperature data

#define ACTIVE 0
#define IDLE 1
#define POWER_DOWN 2

void setup()
{
    Serial.begin(9600);
    while (!Serial);
    collect_temperature_data();
    apply_dft();
}

void collect_temperature_data() {
    Serial.println("Collecting temperature data...");
    for (int i = 0; i < samples; i++) {
        int reading = analogRead(pinTempSensor); // Read analog value
        float R = 1023.0 / reading - 1.0;
        R = R0 * R;
        float temperature = 1.0/(log(R / R0)/B +1/298.15) - 273.15; // Convert resistance to Celsius
        temperature_value[i] = temperature;
         Serial.print("Sample ");
         Serial.print(i + 1);
         Serial.print(": ");
         Serial.println(temperature);

        delay(1000); // Maintain sampling interval
    }
     Serial.println("Temperature data collected.");
}

void apply_dft() {
     Serial.println("DFT Start.");
    for (int i = 0; i < samples; i++) { // had to add this section of code in this function for introducing frequency and magnitude as there was a memory error while computing 
        float real = 0;
        float imag = 0;
        for (int j = 0; j < samples; j++) {
            float angle = 2 * PI * j * i / samples;
            real = real + temperature_value[j] * cos(angle);
            imag = imag - temperature_value[j] * sin(angle);
        }

        float magnitude = sqrt(real * real + imag * imag);
        float frequency = float(i) / samples;
         Serial.print("Frequency: ");
         Serial.print(frequency);  
         Serial.print(" Hz, Magnitude: ");
         Serial.println(magnitude);

    }
     Serial.println("DFT End.");
}

int decide_power_mode() {
    float total_magnitude = 0;
    float freq_sum = 0;

    for (int i = 0; i < samples; i++) {
        float real = 0;
        float imag = 0;

        for (int j = 0; j < samples; j++) {
            float angle = 2.0 * PI * j * i / samples;
            real = real + temperature_value[j] * cos(angle);
            imag = imag - temperature_value[j] * sin(angle);
        }

        float magnitude = sqrt(real * real + imag * imag);
        float frequency = float(i) / samples;

        freq_sum = freq_sum + magnitude * frequency;
        total_magnitude = total_magnitude + magnitude;
    }

    float avg_freq = 0;
    if (total_magnitude > 0) {
        avg_freq = freq_sum / total_magnitude;
    }
    Serial.print("Average frequency: ");
    Serial.println(avg_freq);

    Serial.print("Power Mode: ");
    if (avg_freq >= 0.5) {
       Serial.println("ACTIVE");
       return ACTIVE;
    } else if (avg_freq >= 0.1) {
       Serial.println("IDLE");
       return IDLE;
    } else {
       Serial.println("POWER_DOWN");
       return POWER_DOWN;
}

}

void loop() {
    collect_temperature_data();
    apply_dft();
    int mode = decide_power_mode();
    while (true); 
}
