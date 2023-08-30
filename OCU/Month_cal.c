#include<stdio.h>
#include <stdlib.h> // 프로그램 종료시키는함수를 쓰기위한 라이브러리

int year = 2022, month = 1, day = 1, enter;

void input() {
    int stack = 0; // 5번틀리는지 확인해주는 스택값
    printf("\n\n날짜인 1~365 정수값을 입력하여 주십시오.오늘은 2022년 1월 1일을 기준으로 계산합니다.\n\n");
    for (int i = 0; i < 5; i++) {
        scanf_s("%d", &enter); // 계산할 정수값 입력
        if (enter > 0 && enter < 366)break; //맞는 범위값 정수가 들어오면 for문 정지
        else {
            printf("입력값이 범위안의 값이 아닙니다. 1~365 값을 입력하여 주십시오.\n");
            stack++;
        }// 스택 쌓기
    }
    if (stack == 5) {//스택이 5번 쌓여져 있으면 프로그램 종료
        printf("\n\n입력값이 5번 틀리셔서 프로그램을 강제로 종료합니다.\n\n");
        exit(0);
    }
}
void cal() {
    int month_cal[12] = { 31,28,31,30,31,30,31,30,31,31,30,31 }; // 1년의 각 월마다 일수 저장해놓는 배열
    for (int i = 0; i < 12; i++) { // 12월동안 돌기
        if (enter > month_cal[i]) {
            enter -= month_cal[i];//해당 월의 일수보다 많으면 입력값에서 그 일수만큼 빼주고 month값 증가
            month++;
        }
        else {
            day = enter; // 만약 해당 일수보다 남은 값이 적으면 남은 일수를 day값으로 하고 for문 나가기
            break;
        }
    }
}
int main() {
    printf("오늘 날짜는 %d년 %d월 %d일 입니다.", year, month, day);
    input(); // 입력받는 부함수
    cal();   // 날짜 계산해주는 부함수
    printf("오늘 날짜는 %d년 %d월 %d일 입니다.", year, month, day);
}
