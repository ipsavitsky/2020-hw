// end by Ctrl+C
// no more processes
// max count of clients is undefined (but fits into unsigned short)
// queue to each seller
// no queue to choosing the seller
// clients may be executed in independent processes

// seller don't handle the clients queue

// program can't increase zombies continuosly

// more parallelism
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

int initializer(void);
int finalizer(void);
int sellers(void);
int seller(int seller_no);
int client(void);

enum { SELLERS_COUNT = 5 };
enum { ROWS_COUNT = 10 };
enum { ROW_LENGTH = 40 };

int default_runner(void);
void finalizer_candy(int s) {
    finalizer();
    exit(0);
}

int main(int argc, char *argv[]) {
    signal(SIGINT, finalizer_candy);
    if (strcmp(argv[0], "sellers") == 0) {
        return sellers();
    } else if (strcmp(argv[0], "client") == 0) {
        return client();
    } else if (strcmp(argv[0], "initializer") == 0) {
        return initializer();
    } else if (strcmp(argv[0], "finalizer") == 0) {
        return finalizer();
    } else {
        return default_runner();
    }
}

void remove_zombie(int signo) { wait(0); }

int default_runner(void) {
    srand(time(0));
    signal(SIGCHLD, remove_zombie);
    int r;
    if ((r = initializer()) != 0) {
        return r;
    }

    if (fork() == 0) {
        return sellers();
    }

    while (1) {
        sleep(rand() * (10.0 / RAND_MAX) + 3);
        if (fork() == 0) {
            return client();
        }
    }
    return 0;
}

int initializer(void) {
    // create IPC resources
    key_t key;
    int semid, shmid, msgid;
    if ((key = ftok("./main.c", 's')) == -1) {
        perror("ftok()");
        exit(1);
    }
    if ((semid = semget(key, SELLERS_COUNT * 2, 0666 | IPC_CREAT | IPC_EXCL)) == -1) {
        perror("semget()");
        exit(1);
    }
    unsigned short *init_vals = calloc(SELLERS_COUNT * 2, sizeof(unsigned short));
    semctl(semid, 0, SETALL, init_vals);
    if ((msgid = msgget(key, 066 | IPC_CREAT | IPC_EXCL)) == -1) {
        perror("msgget()");
        exit(1);
    }
    for(int i = 0; i < SELLERS_COUNT; ++i){
        struct Message {
            long mestype;
            pid_t pid;
        } msg;
        msg = (struct Message){.mestype = -i, .pid = -1};
        msgsnd(msgid, &msg, sizeof(msg.pid), 0);
    }
    if ((shmid = shmget(key, ROW_LENGTH * ROWS_COUNT, 0666 | IPC_CREAT)) == -1) {
        perror("shmget()");
        exit(1);
    }
    char *addr = shmat(shmid, 0, 0);
    memset(addr, 0, ROW_LENGTH * ROWS_COUNT);
    shmdt(addr);
    return 0;
}

int finalizer(void) {
    // remove IPC resources
    key_t key;
    int semid, shmid, msgid;
    if ((key = ftok("./main.c", 's')) == -1) {
        perror("ftok()");
        exit(1);
    }
    if ((semid = semget(key, 0, 0)) == -1) {
        perror("semget()");
        exit(1);
    }
    if ((msgid = msgget(key, 0)) == -1) {
        perror("msgget()");
        exit(1);
    }
    if ((shmid = shmget(key, 0, 0)) == -1) {
        perror("shmget()");
        exit(1);
    }
    shmctl(shmid, IPC_RMID, NULL);
    msgctl(msgid, IPC_RMID, NULL);
    semctl(semid, 0, IPC_RMID, NULL);
    return 0;
}

int sellers(void) {
    for (int i = 0; i < SELLERS_COUNT; ++i) {
        if (fork() == 0) {
            return seller(i);
        }
    }
    while (wait(0) != -1)
        ;
    return 0;
}

int seller(int seller_no) {
#define MESSAGE(str) fprintf(stderr, "Seller %d: " str "\n", seller_no)
    key_t key;
    int semid;
    if ((key = ftok("./main.c", 's')) == -1) {
        perror("ftok()");
        exit(1);
    }
    if ((semid = semget(key, 0, 0)) == -1) {
        perror("semget()");
        exit(1);
    }
    while (1) {
        MESSAGE("1. Please come in");
        // 1. wait for a new client
        struct sembuf op;
        op = (struct sembuf){.sem_num = seller_no + SELLERS_COUNT, .sem_op = -1, .sem_flg = 0};
        semop(semid, &op, 1);
        // 2. client chooses its room place himself
        MESSAGE("2. Hello a new client; choose your place");
        // sleep until client chooses its seat
        op = (struct sembuf){.sem_num = seller_no + SELLERS_COUNT, .sem_op = -2, .sem_flg = 0};
        semop(semid, &op, 1);
        // 3. get the ticket
        MESSAGE("3. Here is your ticket. Good bye");
        op = (struct sembuf){.sem_num = seller_no + SELLERS_COUNT, .sem_op = -3, .sem_flg = 0};
        semop(semid, &op, 1);
    }
    return 0;

#undef MESSAGE
}

int client(void) {
#define MESSAGE(str, ...) fprintf(stderr, "Client %d: " str "\n", pid, ##__VA_ARGS__)

    srand(time(0));
    key_t key;
    int semid, shmid, msgid;
    pid_t pid = getpid();
    int my_row, my_pos, num_in_queue;
    if ((key = ftok("./main.c", 's')) == -1) {
        perror("ftok()");
        exit(1);
    }
    if ((semid = semget(key, 0, 0)) == -1) {
        perror("semget()");
        exit(1);
    }
    if ((msgid = msgget(key, 0)) == -1) {
        perror("msgget()");
        exit(1);
    }
    if ((shmid = shmget(key, 0, 0)) == -1) {
        perror("shmget()");
        exit(1);
    }
    char *addr = shmat(shmid, 0, 0);
    MESSAGE("1. Hi!");

    // 1. choose the seller (may be random =) )
    int seller_no = rand() * ((double)SELLERS_COUNT / RAND_MAX);
    MESSAGE("2. I choose the seller number %d", seller_no);

    // 2. put self into the queue
    struct sembuf op = (struct sembuf){.sem_num = seller_no, .sem_op = 1, .sem_flg = 0};
    semop(semid, &op, 1);
    num_in_queue = semctl(semid, seller_no, GETVAL);
    MESSAGE("2b. My uniq number in queue = %d", num_in_queue);

    MESSAGE("3. I sleep until my turn");  // I will be waken up by previous client in my queue
    // check if i'm the first

    // msgctl(msgid)
    struct Message {
        long mestype;
        pid_t pid;
    } msg;
    msgrcv(msgid, &msg, sizeof(pid), -seller_no, 0);
    if(msg.pid != -1){
        pid_t prevpid = msg.pid;
        msg = (struct Message){.mestype = -seller_no, .pid = getpid()};
        msgsnd(msgid, &msg, sizeof(pid), 0);
        msgrcv(msgid, &msg, sizeof(pid), prevpid, 0);
    }
    else{
        msg = (struct Message){.mestype = -seller_no, .pid = getpid()};
        msgsnd(msgid, &msg, sizeof(pid), 0);
    }
    MESSAGE("4. Hello the seller!");
    // 3. wake the seller
    op = (struct sembuf){.sem_num = seller_no + SELLERS_COUNT, .sem_op = 1, .sem_flg = 0};
    semop(semid, &op, 1);
    MESSAGE("5a. I'm choosing the seat");
    // 4. choose the place
    do {
        my_row = rand() * ((double)ROWS_COUNT / RAND_MAX);
        my_pos = rand() * ((double)ROW_LENGTH / RAND_MAX);
    } while (addr[ROW_LENGTH * my_row + my_pos] != 0);
    addr[ROW_LENGTH * my_row + my_pos] = 1;
    MESSAGE("5b. I've chosen the seat %d %d", my_row, my_pos);
    op = (struct sembuf){.sem_num = seller_no + SELLERS_COUNT, .sem_op = 2, .sem_flg = 0};
    semop(semid, &op, 1);
    MESSAGE("6. Thank you! Bye!");
    // op = (struct sembuf){.sem_num = seller_no, .sem_op = -1, .sem_flg = 0};
    // semop(semid, &op, 1);
    op = (struct sembuf){.sem_num = seller_no + SELLERS_COUNT, .sem_op = 3, .sem_flg = 0};
    semop(semid, &op, 1);
    msg = (struct Message){.mestype = getpid(), .pid = getpid()};
    msgsnd(msgid, &msg, sizeof(pid_t), 0);

    // 5. wake up the following client from the queue

    return 0;

#undef MESSAGE
}