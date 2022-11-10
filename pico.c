    #include "pico/stdlib.h"
    #include "pico/cyw43_arch.h"
    #define LED_PIN 0
    #define DATA_PIN 2

    int main() { 
    stdio_init_all();
    if (cyw43_arch_init()) {
        printf("WiFi init failed");
        return -1;
    }
    int count;
    int last = 0;
    int j = 0;
    
    float lasttemp = 0;
    gpio_init(3);
    gpio_set_dir(3,1);
    gpio_init(2);
    gpio_set_dir(2,1);
    cyw43_hal_pin_high(2);
    sleep_ms(250);
    while (true) {  
        cyw43_arch_gpio_put(0,1);
        gpio_set_dir(2,1);
        cyw43_hal_pin_low(2);
        sleep_ms(18);
        cyw43_hal_pin_high(2);
        sleep_us(40);
        gpio_set_dir(2,0);

        int count0 = 0;
        for (uint i = 0; i < 85; i++) {
            if (cyw43_hal_pin_read(2) != 0) {
                break;
            }
        count0++;
        sleep_us(1);
        }   
        int count1 = 0;
        for (uint i = 0; i < 85; i++) {
            if (cyw43_hal_pin_read(2) == 0) {
                break;
            }
        count1++;
        sleep_us(1);
    }
        int bajt = 0;
        int bit = 0;
        uint8_t data[5] = {0, 0, 0, 0, 0};
        while (bit < 40){
            int countLOW = 0;
            for (uint i = 0; i < 85; i++) {
                if (cyw43_hal_pin_read(2) != 0) {
                    break;
                }
                countLOW++;
                //sleep_us(1);
                busy_wait_us_32(1);
            } 

            int countHIGH = 0;
            for (uint i = 0; i < 85; i++) {
            if (cyw43_hal_pin_read(2) == 0) {
                break;
            }
            countHIGH++;
            //sleep_us(1);
            busy_wait_us_32(1);
        }   
            if (countHIGH > 30) {
                //cyw43_hal_pin_high(3);
                busy_wait_us(1);
                //cyw43_hal_pin_low(3);
                //busy_wait_us(1);
                data[bajt] = data[bajt] | (1 << (7 - (bit%8))) ;

                
                   
                
            }
            bit++;

            if ((bit%8) == 0){

                for (int i = 0; i < 8; i++) {
            }
    
                bajt++;
            }

            
            
       }
        
        uint checksum = data[0] + data[2] + data[3] & 0xFF;
        uint check = data[4];
        uint temp = data[3];
        float humidity = (float)data[0];
        float temperature = (float)data[2] + (float)temp/10;
        float deltatemp = temperature - lasttemp;
        if (checksum == check && deltatemp < 5 ){
            printf("Humidity: %0.0f %%", humidity);
            printf(" Temperature: %0.1f°C", temperature);
            printf(" Check Sum: %d = %d\n", check, checksum);
        }else{
            printf("Bad data\n");
        }
        lasttemp = temperature;


        
        cyw43_arch_gpio_put(0,0);
        sleep_ms(200);   
    }  
    }
        


       
    



