#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

// end by Ctrl+C
// no more processes
// max count of persons is undefined
// exactly CAR_CAPACITY passengers in the car

int car(void);
int person(int person_no);

enum { CAR_CAPACITY = 5 };

void remove_zombie(int signo) { wait(0); }

int pers2car[2];
int exit_permit[2];
int available_places[2];
int in_trip[2];

int main(void) {
    srand(time(0));
    signal(SIGCHLD, remove_zombie);

    pipe(pers2car);
    pipe(exit_permit);
    pipe(available_places);
    pipe(in_trip);

    if (fork() == 0) {
        return car();
    }

    int person_no = 0;
    while (1) {
        sleep(rand() * (5.0 / RAND_MAX));
        if (person_no == INT_MAX) {
            break;
        }
        ++person_no;
        if (fork() == 0) {
            return person(person_no);
        }
    }
    return 0;
}

int car(void) {
#define MESSAGE(str) fprintf(stderr, "Car: " str "\n")

    MESSAGE("1. Hi!");

    while (1) {
        MESSAGE("2. Please load the car");
        // 1. allow passengers to seat and wait while the last passenger will wake up me
        char buf = 0;
        for(int i = 0; i < CAR_CAPACITY - 1; ++i){
            // write byte to stream for every vacant place
            write(available_places[1], &buf, sizeof(buf));
        }
        // write a special byte for the last place in the car
        buf = 1;
        write(available_places[1], &buf, sizeof(buf));
        read(pers2car[0], &buf, sizeof(buf));
        MESSAGE("3. Go!");
        // write for every 
        for (int i = 0; i < CAR_CAPACITY; ++i) {
            write(in_trip[1], &buf, sizeof(buf));
        }
        sleep(5);

        MESSAGE("4. Trip ends");
        // 2. allow passengers to go out and wait while the last passenger will wake up me
        char term = 1;
        buf = 0;
        for(int i = 0; i < CAR_CAPACITY - 1; ++i){
            write(exit_permit[1], &buf, sizeof(buf));
        }
        write(exit_permit[1], &term, sizeof(term));
        
        read(pers2car[0], &buf, sizeof(buf));
    }
    return 0;

#undef MESSAGE
}

int person(int person_no) {
#define MESSAGE(str) fprintf(stderr, "Person %02d: " str "\n", person_no)

    MESSAGE("1. Hi!");
    // 1. wait while there is possibility to seat into the car
    char buf;
    read(available_places[0], &buf, sizeof(buf));
    // 2. occupy the seat in the car (+ wake up the car if I occupy the last seat)
    MESSAGE("2. I'm sitting into the car");
    if (buf == 1) {
        write(pers2car[1], &buf, sizeof(buf));
    }

    sleep(1);
    read(in_trip[0], &buf, sizeof(buf));

    MESSAGE("3. I'm in trip!");
    // 3. wait while there is possibility to leave the car (+ wake up the car if I leave the last
    // seat)
    read(exit_permit[0], &buf, sizeof(buf));
    MESSAGE("4. Thanks for the trip. Bye!");
    if(buf == 1) write(pers2car[1], &buf, sizeof(buf));
    return 0;

#undef MESSAGE
}