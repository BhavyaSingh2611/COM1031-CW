#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "includes/seven_segment.h"
#include <math.h>

#define BUTTON_PIN 16

void decoder();

void check_button(char add);

void wipe_array();

bool repeating_timer_callback(struct repeating_timer *t) {
    decoder();
    return true;
}

uint64_t t;
uint64_t tpush;

char morse_input[5];

int pressed = 0;

bool debug_mode = false;
bool is_timer_set = false;

struct repeating_timer timer;

int main() {
    stdio_init_all();

    seven_segment_init();

    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_down(BUTTON_PIN);
    alarm_pool_init_default();

    printf("Hello!\n");
    seven_segment_show(8);
    sleep_ms(2500);
    seven_segment_off();

    int s = 0, i;
    while (true) {
        i = gpio_get(BUTTON_PIN);
        t = time_us_64();

        switch (s) {
            case 0:
                if (i) {
                    seven_segment_show(36);
                    s = 1;
                    tpush = t;
                    if (is_timer_set) {
                        is_timer_set = false;
                        cancel_repeating_timer(&timer);
                    }
                }
                break;
            case 1:
                if (!i) {
                    s = 0;
                    seven_segment_off();
                    if ((t - tpush) > 700000) {
                        seven_segment_show(8);
                        sleep_ms(500);
                        seven_segment_off();
                        wipe_array();
                    } else if ((t - tpush) > 250000) {
                        check_button('-');
                        pressed++;
                        if (debug_mode) {
                            printf("Button Held \n");
                        }
                    } else if ((t - tpush) < 250000) {
                        check_button('.');
                        pressed++;
                        if (debug_mode) {
                            printf("Button Pushed \n");
                        }
                    }
                    if (!is_timer_set) {
                        is_timer_set = true;
                        add_repeating_timer_ms(400, repeating_timer_callback, NULL, &timer);
                    }
                }
                break;
        }
        sleep_until((absolute_time_t) {t + 10000});
    }
}

void wipe_array() {
    for (int i = 0; i < sizeof(morse_input); i++) {
        morse_input[i] = '\0';
    }
    pressed = 0;
}

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
        //seven_segment_show(36);
        printf("No match found\n");
    }
    wipe_array();
}

void check_button(char add) {
    for (int i = 0; i < sizeof(morse_input); i++) {
        if (morse_input[i] == '\0') {
            morse_input[i] = add;
            break;
        }
    }
}