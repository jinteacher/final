#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAX_SENSOR_VALUE 750
#define THRESHOLD 500

int main(int argc, char **argv)
{
    int gas_sensor_value;

    /* 난수 생성기 초기화 */
    srand(time(NULL));

    /* 가스 센서 값 시뮬레이션 */
    do {
        gas_sensor_value = rand() % (MAX_SENSOR_VALUE + 1);
    } while (gas_sensor_value >= THRESHOLD);

    char msg_text[BUFSIZ];
    sprintf(msg_text, "Gas sensor value: %d ppm", gas_sensor_value);

    printf("%s\n", msg_text);

    return 0;
}
