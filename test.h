#pragma once

// single headerfile testing suite
// prints output to STDOUT, relies on a VT105 compatable terminal

// include this /only once/, then pass test functions
// to the "execute tests" function

// git @ https://github.com/NuclearPhone/testc

#include <bits/types/struct_timespec.h>
#include <pthread.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

typedef struct {
    const char* name;
    const char* desc;
    const char* (*ptr)();
} test_t;

typedef struct {
    const char* (*test_fn)();
    bool finished;
} wrapper_args;

static void* test_wrapper(void* _args) {
    wrapper_args* args = (wrapper_args*)_args;
    const char* val = args->test_fn();
    args->finished = true;
    return (void*)val;
}

// returns true on pass
static bool execute_test(const test_t test) {
    wrapper_args args = {
        .test_fn = test.ptr,
        .finished = false,
    };

    pthread_t test_thread;
    pthread_create(&test_thread, NULL, test_wrapper, &args);

    uint8_t idx = 0;
    uint32_t time_ms = 0;  // millisecond precision
    uint32_t time_s = 0;

    do {
        if (time_ms > 1000) {
            printf("\033[1K\033[0G");
            printf("%s: ", test.name);

            for (uint8_t i = 0; i < time_s % 4; i++)
                printf(".");

            time_s += time_ms / 1000;
            time_ms %= 1000;
        }

        if (args.finished)
            break;

        static const struct timespec sleep_time = (struct timespec){
            .tv_sec = 0,
            .tv_nsec = 1000,
        };

        nanosleep(&sleep_time, NULL);
        time_ms += 1;
    } while (!args.finished);

    const char* test_return = NULL;
    pthread_join(test_thread, (void**)&test_return);

    if (test_return == NULL)
        printf("\033[1K\033[0G%s: SUCCESS!", test.name);
    else
        printf("\033[1K\033[0G%s: Error [\"%s\"]", test.name, test_return);

    float total_time = time_s + ((float)time_ms / 1000);
    printf("\n\tFinished in: %.2fs\n", total_time);

    return test_return ? false : true;
}
