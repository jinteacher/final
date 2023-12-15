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
    long mtype; /* 메시지의 타입 : 0 이상의 정숫값 */
    char mtext[BUFSIZ]; /* 메시지의 내용 : 1바이트 이상의 문자열 */
};

int main(int argc, char **argv)
{
    int gas_sensor_value;
    int temp_value;
    time_t start_time;

    /* 난수 생성기 초기화 */
    srand(time(NULL));

    /* 메시지 큐의 채널을 생성한다. */
    key_t key = MSQKEY;
    int msqid;
    if((msqid = msgget(key, IPC_CREAT | IPC_EXCL | 0666)) < 0) {
        perror("msgget()");
        return -1;
    }

    /* 측정 시작 시간 설정 */
    start_time = time(NULL);

    /* 무한 루프 시작 */
    while(1) {
        /* 현재 시간을 가져옴 */
        time_t current_time = time(NULL);
        struct tm *local_time_info = localtime(&current_time);
        char time_str[26];
        strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", local_time_info);

        /* 가스 센서 값 시뮬레이션 */
        gas_sensor_value = MIN_SENSOR_VALUE + rand() % (MAX_SENSOR_VALUE - MIN_SENSOR_VALUE + 1);

        /* 3% 확률로 가스 센서 값이 500 이상이 되도록 함 */
        if((rand() % 100) < OVER_RANGE_RATE) {
            gas_sensor_value = OVER_RANGE_SENSOR_VALUE + rand() % (MAX_SENSOR_VALUE - MIN_SENSOR_VALUE + 1);
        }

        /* 온도 값 시뮬레이션 */
        temp_value = MIN_TEMP_VALUE + rand() % (MAX_TEMP_VALUE - MIN_TEMP_VALUE + 1);

        /* 3% 확률로 온도 값이 200 이상이 되도록 함 */
        if((rand() % 100) < OVER_RANGE_RATE) {
            temp_value = OVER_RANGE_TEMP_VALUE + rand() % (MAX_TEMP_VALUE - MIN_TEMP_VALUE + 1);
        }

        /* 가스 센서 값, 온도 값, 현재 시간을 메시지 큐로 전송 */
        struct msgbuf mb;
        mb.mtype = 1;
        sprintf(mb.mtext, "[%s] Gas sensor value: %d ppm, Temperature: %d", time_str, gas_sensor_value, temp_value);
        msgsnd(msqid, &mb, sizeof(mb), 0);

        /* 가스 센서 값이나 온도 값이 500이나 200을 넘기거나, 측정 시작으로부터 30분이 지나면 메시지 큐를 종료 */
        if(gas_sensor_value > OVER_RANGE_SENSOR_VALUE || temp_value > OVER_RANGE_TEMP_VALUE || difftime(time(NULL), start_time) > TIME_LIMIT) {
            mb.mtype = 2;
            sprintf(mb.mtext, "Sensor value exceeded limit or time limit reached.");
            msgsnd(msqid, &mb, sizeof(mb), 0);
            break;
        }
    }

    return 0;
}
