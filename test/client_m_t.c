#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/msg.h>

#define MIN_SENSOR_VALUE 100  // 가스 센서의 최소값
#define MAX_SENSOR_VALUE 120  // 가스 센서의 최대값
#define MIN_TEMP_VALUE 20     // 온도의 최소값
#define MAX_TEMP_VALUE 24     // 온도의 최대값
#define OVER_RANGE_SENSOR_VALUE 500  // 가스 센서의 초과 범위 값
#define OVER_RANGE_TEMP_VALUE 200    // 온도의 초과 범위 값
#define OVER_RANGE_RATE 3  // 초과 범위의 확률
#define MSQKEY 51234  // 메시지 큐 키
#define TIME_LIMIT 1800  // 제한시간(초)

struct msgbuf {
    long mtype;  // 메시지 타입
    char mtext[BUFSIZ];  // 메시지 내용
};

int main(int argc, char **argv)
{
    key_t key;
    int rc, msqid;
    time_t start_time;
    srand(time(NULL));  // 난수 생성기 초기화
    start_time = time(NULL);  // 시작 시간 설정

    struct msgbuf *mb;
    mb = (struct msgbuf*)malloc(sizeof(struct msgbuf));  // 메시지 버퍼 할당

    key = MSQKEY;  // 키 설정
    if((msqid = msgget(key, 0666)) < 0) {  // 메시지 큐 생성
        perror("msgget()");
        return -1;
    }

    while(1) {  // 무한 루프 시작
        time_t current_time = time(NULL);
        struct tm *local_time_info = localtime(&current_time);  // 현재 시간 정보 가져오기
        char time_str[26];
        strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", local_time_info);  // 시간 정보를 문자열로 변환

        // 가스 센서 값 시뮬레이션
        int gas_sensor_value = MIN_SENSOR_VALUE + rand() % (MAX_SENSOR_VALUE - MIN_SENSOR_VALUE + 1);
        if((rand() % 100) < OVER_RANGE_RATE) {  // 3% 확률로 가스 센서 값이 500 이상이 되도록 함
            gas_sensor_value = OVER_RANGE_SENSOR_VALUE + rand() % (MAX_SENSOR_VALUE - MIN_SENSOR_VALUE + 1);
        }

        // 온도 값 시뮬레이션
        int temp_value = MIN_TEMP_VALUE + rand() % (MAX_TEMP_VALUE - MIN_TEMP_VALUE + 1);
        if((rand() % 100) < OVER_RANGE_RATE) {  // 3% 확률로 온도 값이 200 이상이 되도록 함
            temp_value = OVER_RANGE_TEMP_VALUE + rand() % (MAX_TEMP_VALUE - MIN_TEMP_VALUE + 1);
        }

        // 가스 센서 값, 온도 값, 현재 시간을 메시지 큐로 전송
        mb->mtype = 1;
        sprintf(mb->mtext, "[%s] Gas sensor value: %d ppm, Temperature: %d", time_str, gas_sensor_value, temp_value);
        rc = msgsnd(msqid, mb, strlen(mb->mtext)+1, 0);
        if(rc == -1) {
            perror("msgsnd()");
            return -1;
        }

        // 가스 센서 값이나 온도 값이 500이나 200을 넘기거나, 측정 시작으로부터 30분이 지나면 메시지 큐를 종료
        if(gas_sensor_value > OVER_RANGE_SENSOR_VALUE || temp_value > OVER_RANGE_TEMP_VALUE || difftime(time(NULL), start_time) > TIME_LIMIT) {
            mb->mtype = 2;
            sprintf(mb->mtext, "Sensor value exceeded limit or time limit reached.");
            msgsnd(msqid, mb, strlen(mb->mtext)+1, 0);
            break;
        }
    }

    return 0;
}
