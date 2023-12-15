#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/msg.h>

#define MIN_SENSOR_VALUE 100
#define MAX_SENSOR_VALUE 120
#define MIN_TEMP_VALUE 20
#define MAX_TEMP_VALUE 24
#define OVER_RANGE_SENSOR_VALUE 500
#define OVER_RANGE_TEMP_VALUE 200
#define OVER_RANGE_RATE 3
#define MSQKEY 51234
#define TIME_LIMIT 1800

struct msgbuf {
    long mtype;
    char mtext[BUFSIZ];
};

int main(int argc, char **argv)
{
    key_t key;
    int rc, msqid;
    time_t start_time;
    srand(time(NULL));
    start_time = time(NULL);

    struct msgbuf *mb;
    mb = (struct msgbuf*)malloc(sizeof(struct msgbuf));

    key = MSQKEY;
    if((msqid = msgget(key, 0666)) < 0) { 
        perror("msgget()");
        return -1;
    }

    while(1) {
        time_t current_time = time(NULL);
        struct tm *local_time_info = localtime(&current_time);
        char time_str[26];
        strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", local_time_info);

        int gas_sensor_value = MIN_SENSOR_VALUE + rand() % (MAX_SENSOR_VALUE - MIN_SENSOR_VALUE + 1);
        if((rand() % 100) < OVER_RANGE_RATE) {
            gas_sensor_value = OVER_RANGE_SENSOR_VALUE + rand() % (MAX_SENSOR_VALUE - MIN_SENSOR_VALUE + 1);
        }

        int temp_value = MIN_TEMP_VALUE + rand() % (MAX_TEMP_VALUE - MIN_TEMP_VALUE + 1);
        if((rand() % 100) < OVER_RANGE_RATE) {
            temp_value = OVER_RANGE_TEMP_VALUE + rand() % (MAX_TEMP_VALUE - MIN_TEMP_VALUE + 1);
        }

        mb->mtype = 1;
        sprintf(mb->mtext, "[%s] Gas sensor value: %d ppm, Temperature: %d", time_str, gas_sensor_value, temp_value);
        rc = msgsnd(msqid, mb, strlen(mb->mtext)+1, 0);
        if(rc == -1) {
            perror("msgsnd()");
            return -1;
        }

        if(gas_sensor_value > OVER_RANGE_SENSOR_VALUE || temp_value > OVER_RANGE_TEMP_VALUE || difftime(time(NULL), start_time) > TIME_LIMIT) {
            mb->mtype = 2;
            sprintf(mb->mtext, "Sensor value exceeded limit or time limit reached.");
            msgsnd(msqid, mb, strlen(mb->mtext)+1, 0);
            break;
        }
    }

    return 0;
}
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/msg.h>

#define MIN_SENSOR_VALUE 100
#define MAX_SENSOR_VALUE 120
#define MIN_TEMP_VALUE 20
#define MAX_TEMP_VALUE 24
#define OVER_RANGE_SENSOR_VALUE 500
#define OVER_RANGE_TEMP_VALUE 200
#define OVER_RANGE_RATE 3
#define MSQKEY 51234
#define TIME_LIMIT 1800

struct msgbuf {
    long mtype;
    char mtext[BUFSIZ];
};

int main(int argc, char **argv)
{
    key_t key;
    int rc, msqid;
    time_t start_time;
    srand(time(NULL));
    start_time = time(NULL);

    struct msgbuf *mb;
    mb = (struct msgbuf*)malloc(sizeof(struct msgbuf));

    key = MSQKEY;
    if((msqid = msgget(key, 0666)) < 0) { 
        perror("msgget()");
        return -1;
    }

    while(1) {
        time_t current_time = time(NULL);
        struct tm *local_time_info = localtime(&current_time);
        char time_str[26];
        strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", local_time_info);

        int gas_sensor_value = MIN_SENSOR_VALUE + rand() % (MAX_SENSOR_VALUE - MIN_SENSOR_VALUE + 1);
        if((rand() % 100) < OVER_RANGE_RATE) {
            gas_sensor_value = OVER_RANGE_SENSOR_VALUE + rand() % (MAX_SENSOR_VALUE - MIN_SENSOR_VALUE + 1);
        }

        int temp_value = MIN_TEMP_VALUE + rand() % (MAX_TEMP_VALUE - MIN_TEMP_VALUE + 1);
        if((rand() % 100) < OVER_RANGE_RATE) {
            temp_value = OVER_RANGE_TEMP_VALUE + rand() % (MAX_TEMP_VALUE - MIN_TEMP_VALUE + 1);
        }

        mb->mtype = 1;
        sprintf(mb->mtext, "[%s] Gas sensor value: %d ppm, Temperature: %d", time_str, gas_sensor_value, temp_value);
        rc = msgsnd(msqid, mb, strlen(mb->mtext)+1, 0);
        if(rc == -1) {
            perror("msgsnd()");
            return -1;
        }

        if(gas_sensor_value > OVER_RANGE_SENSOR_VALUE || temp_value > OVER_RANGE_TEMP_VALUE || difftime(time(NULL), start_time) > TIME_LIMIT) {
            mb->mtype = 2;
            sprintf(mb->mtext, "Sensor value exceeded limit or time limit reached.");
            msgsnd(msqid, mb, strlen(mb->mtext)+1, 0);
            break;
        }
    }

    return 0;
}
