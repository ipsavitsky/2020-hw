#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

// end by Ctrl+C
// no more processes
// max count of patients is undefined
// don't change the protocol of getting to the queue

// doctor can talk with only one patient
// doctor mustn't handle the requests "who is the latest"
// doctor mustn't handle the patients queue

// almost all time patients sleep in queue and eventually:
//      - check if the previous patient is existed
//      - answer to requests "who is the latest"

// please don't use arrays of pipes
// don't use pipes for long-time storing the information
// (but you can use pipe for transferring
//  the complex information between processes)

// don't use active waiting

// don't increase zombies
// freezing by Ctrl+Z and the following "fg" command mustn't cause the deadlock

int doctor(void);
int patient(int patient_no);

pid_t next_process;

int is_first;
int is_latest;
int num_called = 0;

int patient_no;
pid_t pid;

int doctor2patient[2];
int doctor2next_patient[2];
int patient2doctor[2];
int latest_pid[2];

void dummy_hndlr(int s) {}

void check_next(int s) {
    // int status;
    if (kill(next_process, 0) == -1) {
        is_first = 1;
    }
    alarm(1);
}

#define MESSAGE(str) fprintf(stderr, "Patient %02d [%06d]: " str "\n", patient_no, pid)
void check_if_latest(int s) {
    if (is_latest) {
        MESSAGE("2b. I'm the latest");
        pid_t my_pid = getpid();
        write(latest_pid[1], &my_pid, sizeof(pid_t));
        raise(SIGSTOP);  // freeze ourselves so that we won't read ourselves
        is_latest = 0;
    }
}
#undef MESSAGE

void check_for_answer(int s) {
    num_called++;
    is_latest = 1;

    if ((read(latest_pid[0], &next_process, sizeof(pid_t)) == -1) && (errno == EAGAIN)) {
    } else {
        kill(next_process, SIGCONT);  // unfreeze the process that gave us their pid
    }
    alarm(1);
}

void remove_zombie(int signo) { wait(0); }

int main(void) {
    srand(time(0));
    signal(SIGUSR1, SIG_IGN);
    signal(SIGCHLD, remove_zombie);
    if (fork() == 0) {
        return doctor();
    }
    pipe(doctor2patient);
    pipe(doctor2next_patient);
    pipe(patient2doctor);
    pipe(latest_pid);
    int flags = fcntl(latest_pid[0], F_GETFL, 0);
    fcntl(latest_pid[0], F_SETFL, flags | O_NONBLOCK);
    patient_no = 0;
    signal(SIGUSR2, dummy_hndlr);
    while (1) {
        sleep(rand() * (10.0 / RAND_MAX) + 3);
        if (patient_no == INT_MAX) {
            break;
        }
        ++patient_no;
        if (fork() == 0) {
            return patient(patient_no);
        }
    }
    return 0;
}

#define MESSAGE(str) fprintf(stderr, "Doctor: " str "\n")
int doctor(void) {
    signal(SIGUSR1, SIG_IGN);
    while (1) {
        char buf;
        MESSAGE("1. Please come in");
        // 1. sleep until a patient will come
        write(doctor2next_patient[1], &buf, sizeof(buf));
        read(patient2doctor[0], &buf, sizeof(buf));
        // 2. talk with the patient
        MESSAGE("2. Hello a new patient, my good patient");
        sleep(rand() * (7.0 / RAND_MAX) + 2);

        MESSAGE("3. Good bye");
        write(doctor2patient[1], &buf, sizeof(buf));
    }
    return 0;
}
#undef MESSAGE

#define MESSAGE(str) fprintf(stderr, "Patient %02d [%06d]: " str "\n", patient_no, pid)
int patient(int patient_n) {
    pid = getpid();
    patient_no = patient_n;

    MESSAGE("1. Hi!");
    signal(SIGUSR1, check_if_latest);
    // Only one patient may do steps 1 and 2 simultaneously

    // 1. Broadcast the request, who is the latest patient?
    is_latest = 1;
    MESSAGE("2. Who is the latest?");
    is_latest = 0;
    kill(0, SIGUSR1);
    signal(SIGALRM, check_for_answer);
    alarm(1);
    next_process = -1;
    while ((num_called < 5) && (next_process == -1)) {
        pause();
    }
    signal(SIGALRM, SIG_DFL);
    // if (next_process == -1) {
    //     MESSAGE("I AM THE FIRST IN LINE");
    // }
    // else{
    //     fprintf(stderr, "Patient %02d [%06d]: Next process = %d\n", patient_no, pid,
    //     next_process);
    // }

    // 2. wait for the answer for a 5 second (check the answer each second)

    //!!! THE LATEST PROCESS MUST INVOKE MESSAGE("2b. I'm the latest");

    // Now you have PID of the latest patient process ("previous patient")
    // or info that you are the first patient

    MESSAGE("3. I'm waiting for my turn");
    char buf;
    if (next_process != -1) {
        signal(SIGALRM, check_next);
        alarm(1);
        is_first = 0;
        while (!is_first) {
            pause();
        }
        signal(SIGALRM, SIG_DFL);
    }

    // 3. wait until we become the first patient to the doctor
    //  (i.e. check each second if the previous person process is existed)

    MESSAGE("4. I'm the next to the doctor");
    read(doctor2next_patient[0], &buf, sizeof(buf));
    write(patient2doctor[1], &buf, sizeof(buf));
    // 4. wait until doctor will become free
    MESSAGE("5. Hi doctor!");
    // 5. wake up the doctor and exit my process
    read(doctor2patient[0], &buf, sizeof(buf));

    return 0;
}
#undef MESSAGE