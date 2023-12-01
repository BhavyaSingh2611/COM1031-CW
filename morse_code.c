//Imports libraries
#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "includes/seven_segment.h"
#include <math.h>
//Defines the button component
#define BUTTON_PIN 16

//Defines functions so they can be referenced throughout the code
void decoder();

void check_button(char add);

void wipe_array();

bool repeating_timer_callback(struct repeating_timer *t) {
    decoder();
    return true;
}

//Defines variables
uint64_t t;
uint64_t tpush;

char morse_input[5];

int pressed = 0;

bool is_timer_set = false;

struct repeating_timer timer;

int main() {
    //Initialises each component
    stdio_init_all();

    seven_segment_init();

    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_down(BUTTON_PIN);
    alarm_pool_init_default();
    //Welcome message
    printf("Hello!\n");
    seven_segment_show(36);
    sleep_ms(2500);
    seven_segment_off();

    int s = 0, i;
    while (true) {
        i = gpio_get(BUTTON_PIN);
        t = time_us_64();
        //Implemented as a Finite State Machine
        switch (s) {
            //Checks if the button has been pressed
            case 0:
                if (i) {
                    seven_segment_show(36);
                    s = 1;
                    tpush = t;
                    //If the timer is set, avoid decoding during user input
                    if (is_timer_set) {
                        is_timer_set = false;
                        cancel_repeating_timer(&timer);
                    }
                }
                break;
                //Checks if the button has been released
            case 1:
                if (!i) {
                    //Switches case back to 0
                    s = 0;
                    seven_segment_off();
                    //Detects if longer than 700ms
                    if ((t - tpush) > 700000) {
                        seven_segment_show(8);
                        sleep_ms(500);
                        seven_segment_off();
                        printf("Error: Button held for too long");
                        wipe_array();
                        //Detects if longer than 250ms
                    } else if ((t - tpush) > 250000) {
                        check_button('-');
                        printf("Button held");
                        pressed++;
                        //Detects if less than 250ms
                    } else if ((t - tpush) < 250000) {
                        check_button('.');
                        printf("Button pressed");
                        pressed++;
                    }
                    //Validation to avoid duplicate timers
                    if (!is_timer_set) {
                        is_timer_set = true;
                        add_repeating_timer_ms(400, repeating_timer_callback, NULL, &timer);
                    }
                }
                break;
        }
        // Taken from:
        // Fairhead, H. (2021a) in Programming the raspberry pi pico in C. Leyburn, North Yorkshire: I/O Press, p. 60.
        sleep_until((absolute_time_t) {t + 10000});
    }
}

//Clears the current input, so it becomes \0\0\0\0\0
void wipe_array() {
    for (int i = 0; i < sizeof(morse_input); i++) {
        morse_input[i] = '\0';
    }
    pressed = 0;
}

//Checks button input in order to assign the correct segment output and output it to the console
void decoder() {
    if (pressed > 5) {
        printf("Error: Too many inputs");
        seven_segment_show(36);
    } else if (strcmp(morse_input, "-----") == 0) {
        seven_segment_show(0);
        printf("----- : 0\n");
    } else if (strcmp(morse_input, ".----") == 0) {
        seven_segment_show(1);
        printf(".---- : 1\n");
    } else if (strcmp(morse_input, "..---") == 0) {
        seven_segment_show(2);
        printf("..--- : 2\n");
    } else if (strcmp(morse_input, "...--") == 0) {
        seven_segment_show(3);
        printf("...-- : 3\n");
    } else if (strcmp(morse_input, "....-") == 0) {
        seven_segment_show(4);
        printf("....- : 4\n");
    } else if (strcmp(morse_input, ".....") == 0) {
        seven_segment_show(5);
        printf("..... : 5\n");
    } else if (strcmp(morse_input, "-....") == 0) {
        seven_segment_show(6);
        printf("-.... : 6\n");
    } else if (strcmp(morse_input, "--...") == 0) {
        seven_segment_show(7);
        printf("--... : 7\n");
    } else if (strcmp(morse_input, "---..") == 0) {
        seven_segment_show(8);
        printf("---.. : 8\n");
    } else if (strcmp(morse_input, "----.") == 0) {
        seven_segment_show(9);
        printf("----. : 9\n");
    } else if (strcmp(morse_input, ".-\0\0\0") == 0) {
        seven_segment_show(10); //a
        printf(".- : A\n");
    } else if (strcmp(morse_input, "-...\0") == 0) {
        seven_segment_show(11); //b
        printf("-... : B\n");
    } else if (strcmp(morse_input, "-.-.\0") == 0) {
        seven_segment_show(12); //c
        printf("-.-. : C\n");
    } else if (strcmp(morse_input, "-..\0\0") == 0) {
        seven_segment_show(13); //d
        printf("-.. : D\n");
    } else if (strcmp(morse_input, ".\0\0\0\0") == 0) {
        seven_segment_show(14); //e
        printf(". : E\n");
    } else if (strcmp(morse_input, "..-.\0") == 0) {
        seven_segment_show(15); //f
        printf("..-. : F\n");
    } else if (strcmp(morse_input, "--.\0\0") == 0) {
        seven_segment_show(16); //g
        printf("--. : G\n");
    } else if (strcmp(morse_input, "....\0") == 0) {
        seven_segment_show(17); //h
        printf(".... : H\n");
    } else if (strcmp(morse_input, "..\0\0\0") == 0) {
        seven_segment_show(18); //i
        printf(".. : I\n");
    } else if (strcmp(morse_input, ".---\0") == 0) {
        seven_segment_show(19); //j
        printf(".--- : J\n");
    } else if (strcmp(morse_input, "-.-\0\0") == 0) {
        seven_segment_show(20); //k
        printf("-.- : K\n");
    } else if (strcmp(morse_input, ".-..\0") == 0) {
        seven_segment_show(21); //l
        printf(".-.. : L\n");
    } else if (strcmp(morse_input, "--\0\0\0") == 0) {
        seven_segment_show(22); //m
        printf("-- : M\n");
    } else if (strcmp(morse_input, "-.\0\0\0") == 0) {
        seven_segment_show(23); //n
        printf("-. : N\n");
    } else if (strcmp(morse_input, "---\0\0") == 0) {
        seven_segment_show(24); //o
        printf("--- : O\n");
    } else if (strcmp(morse_input, ".--.\0") == 0) {
        seven_segment_show(25); //p
        printf(".--. : P\n");
    } else if (strcmp(morse_input, "--.-\0") == 0) {
        seven_segment_show(26); //q
        printf("--.- : Q\n");
    } else if (strcmp(morse_input, ".-.\0\0") == 0) {
        seven_segment_show(27); //r
        printf(".-. : R\n");
    } else if (strcmp(morse_input, "...\0\0") == 0) {
        seven_segment_show(28); //s
        printf("... : S\n");
    } else if (strcmp(morse_input, "-\0\0\0\0") == 0) {
        seven_segment_show(29); //t
        printf("- : T\n");
    } else if (strcmp(morse_input, "..-\0\0") == 0) {
        seven_segment_show(30); //u
        printf("..- : U\n");
    } else if (strcmp(morse_input, "...-\0") == 0) {
        seven_segment_show(31); //v
        printf("...- : V\n");
    } else if (strcmp(morse_input, ".--\0\0") == 0) {
        seven_segment_show(32); //w
        printf(".-- : W\n");
    } else if (strcmp(morse_input, "-..-\0") == 0) {
        seven_segment_show(33); //x
        printf("-..- : X\n");
    } else if (strcmp(morse_input, "-.--\0") == 0) {
        seven_segment_show(34); //y
        printf("-.-- : Y\n");
    } else if (strcmp(morse_input, "--..\0") == 0) {
        seven_segment_show(35); //z
        printf("--.. : Z\n");
    } else if (strcmp(morse_input, "\0\0\0\0\0") != 0) {
        seven_segment_show(8);
        sleep_ms(500);
        seven_segment_off();
        printf("Error: No match found\n");
    }
    wipe_array();
}

//Can't overwrite current morse characters unless wipe_array() is called
void check_button(char add) {
    for (int i = 0; i < sizeof(morse_input); i++) {
        //Checks if the char position is already \0
        //Assign the input to the array
        if (morse_input[i] == '\0') {
            morse_input[i] = add;
            break;
        }
    }
}
