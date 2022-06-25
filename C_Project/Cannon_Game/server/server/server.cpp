#include <winsock2.h>
#include <windows.h>
#include <iostream>
#include <process.h>
#include <stdlib.h>
#include <time.h>
#include<conio.h>



#pragma comment (lib, "ws2_32.lib")
#pragma warning(disable:4996)

using namespace std;

int map[10][10] = {
{0,0,0,0,0,0,0,0,0,0 },
{0,0,0,1,1,0,0,0,0,0},
{0,0,0,1,1,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,1,1,0},
{0,0,0,0,0,0,0,1,1,0},
{0,1,1,0,0,0,0,0,0,0},
{0,1,1,0,0,1,1,0,0,0},
{0,0,0,0,0,1,1,0,0,0},
{0,0,0,0,0,0,0,0,0,0} };

int y, x, y2, x2, y3, x3, y4, x4; // 상대위치
int speed, mode;
bool Fog = 0;
int po, py, px, py2, px2, py3, px3, py4, px4;//대포방향,대포좌표

void  print() {
    if (Fog) {
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 10; j++) {
                if (map[i][j] != 1) { cout << "□"; }      //1은 맵을 의미
                else if (map[i][j] == 1) { cout << "■"; }      //2은 벽을 의미
            }
            cout << endl;
        }
    }
    else {
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 10; j++) {
                if (map[i][j] == 0) cout << "□";
                else if (map[i][j] == 1) { cout << "■"; }   //2은 벽을 의미
                else if (map[i][j] == 2) { cout << "▲"; }      //대포의 방향마다 다른값 설정
                else if (map[i][j] == 3) { cout << "◀"; }
                else if (map[i][j] == 4) { cout << "▶"; }
                else if (map[i][j] == 5) { cout << "▼"; }
                else if (map[i][j] == -1)cout << "º";
            }
            cout << endl;
        }
    }
}
/*클라이언트와 메시지를 주고 받을 포트*/
#define  PORT 5500
/*클라이언트에서 전달할 메시지의 최대 크기*/
#define BUFFERSIZE 100
/*클라이언트 대화명의 최대 크기*/
#define NAMESIZE 30
/*뮤텍스를  저장할 변수*/
HANDLE  mutex;
/*현재 접속중인 클라이언트의 수를 저장할 변수
   현재는 접속 중인 클라이언트가 없으므로 초기값은 0*/
int clientNumber = 0;
/*쓰레드의 ID 를 저장할 변수*/
unsigned  threadID;
/*현재 접속중인 모든 클라이언트의 클라이언트 소켓을 저장할 변수*/
int allClientSocket[100];
int Number[3] = { 0, };
int Fog_client[3] = { 0, };      //클라이언트들에게 어떤모드인지를 전달해주는 변수
class THD {

public:
    int Soc_Ser();
    static unsigned __stdcall broadcastAllClient(void* arg);
};
int THD::Soc_Ser()
{
    /*쓰레드를 동기화 하기 위해서 뮤텍스 객체를 생성합니다.
    뮤텍스 객체를 생성하는데 실패하면 에러메시지를 출력하고 프로그램을 종료합니다.*/
    mutex = CreateMutex(NULL, FALSE, NULL);
    WSADATA wsadata;
    WSAStartup(MAKEWORD(2, 2), &wsadata);
    /*클라이언트의 접속을 기다리는 서버 소켓 생성*/
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    cout << "서버 소켓을 생성 했습니다." << endl;
    /*서버의 주소를 저장하는 구조체 serverAddress 생성*/
    sockaddr_in serverAddress;
    /*serverAddress 구조체를 0으로 초기화*/
    memset(&serverAddress, 0, sizeof(serverAddress));
    /*serverAddress 에 현재 IP 대입 INADDR_ANY->자동으로 IP 를 리턴하는 상수*/
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    /*주소 타입 설정*/
    serverAddress.sin_family = AF_INET;
    /*클라이언트와 메시지를 주고 받을 포트 번호 설정*/
    serverAddress.sin_port = htons(PORT);
    /*서버 소켓에 주소정보 설정하합니다.에러가 발생하면 에러 메시지를 출력하고 프로그램을 종료합니다. */
    if (bind(serverSocket, (sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        cout << "서버 소켓을 바인드 하는데 실패했습니다." << endl;
        return 0;
    }
    /*클라이언트의 접속을 기다리는 대기모드로 전환*/
    if (listen(serverSocket, 5) == -1) {
        cout << "서버 소켓을 listen 모드로 설정하는데 실패했습니다" << endl;
        return 0;
    }
    cout << "참가자 수를 입력하십시오 (3명이하)    입력 : ";
    cin >> Number[0];
    system("cls");
    cout << "게임 모드 선택" << endl;
    cout << "일반모드 : 1번 , 안개모드 : 2번     입력 : ";
    cin >> Fog_client[0];
    system("cls");
    cout << "입력하신 참가자만큼 실행시켜주세요!";
    while (1) {
        /*클라이언트의 주소 정보를 저장할 구조체 선언*/
        struct sockaddr_in clientAddress;
        int clientAddressSize = sizeof(clientAddress);
        /*클라이언트의 접속을 허락하고 클라이언트와 데이터를    주고받는 클라이언트 소켓생성
         합니다. 클라이언트의 주소 정보를 clientAddress 에 대입합니다.*/
        int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress,
            &clientAddressSize);
        /*뮤텍스를 이용해서 공유 자원을 잠급니다. 공유 자원을 열때 까지 다른 쓰레드는 공유
           자원을 사용할 수 없고 대기 상태에서 기다립니다.*/
        WaitForSingleObject(mutex, INFINITE);
        /*현재 접속중인 클라이언트의 수를 1증가 시킵니다.*/
        clientNumber++;
        /*현재 접속중인 모든 클라이언트의 클라이언트 소켓을 저장하는 배열 allClientSocket 에
      클라이언트소켓을 대입합니다.*/
        allClientSocket[clientNumber - 1] = clientSocket;
        /*공유 변수의 잠금을 해제합니다. 공유 자원을 사용할 수 없어서 대기 중이던 다른쓰레드
          의 대기 상태가 풀립니다.*/
        ReleaseMutex(mutex);
        /*클라이언트에서 전송한 메시지를 읽어 들여서 모든 클라이언트로 전송하는 broadcastAllClient 함수를 호출합니다.*/
        unsigned long thread;
        thread = _beginthreadex(NULL, 0, broadcastAllClient, (void*)clientSocket, 0, &threadID);
    }
    cout << "게임을 종료 합니다" << endl;
    return 0;
}
int from_client[3]; // 탱크 x값 y값 모양값
int fromclient[15] = { 13,13,13,13,13,13,13,13,13,13,13,13,13,13,13 };
int fromclient_key[3] = { 13,13,13 };//탱크 좌표
int toclient_key[6]; //탱크값 대표값 저장
int fromclient_keyp[2]{ 13,13 }; //대포알좌표
int die[2]; //생존 신고


int tomap[11][10] = { 100, };
unsigned __stdcall THD::broadcastAllClient(void* arg)
{
    /*클라이언트가 접속하면 main 함수에서 클라이언트와의 접속을 허락하고 클라이언트와 데이터를 주고 받을수 있는 클라이언트 소켓을 생성합니다. 멀티 쓰레드로 broadcastAllClient 함수를 시작하는데 이때 인자로 클라이언트 소켓을 전달합니다. main 함수에서 전달한 클라이언트 소켓을 myClientsocket 변수에 대입했습니다.*/
    int myClientSocket = (int)arg;
    int i = 0;
    /*myClientSocket으로 연결된 현재 클라이언트가 전송한 메시지를 읽어 들이는 부분입니다. 현재 클라이언트가 접속을 종료 할때 까지 계속 반복합니다.*/
    //몇명이 참가할지 인원수를 받아서 클라이언트들에게 전달한다.
    send(myClientSocket, (char*)Number, sizeof(Number), 0);
    send(myClientSocket, (char*)Fog_client, sizeof(Fog_client), 0);
    recv(myClientSocket, (char*)from_client, sizeof(from_client), 0);//각 클라이언트로부터 받은 값으로 처음 탱크 좌표를 설정한다.
    y = from_client[0];
    x = from_client[1];
    map[y][x] = from_client[2];
    if (clientNumber == 1) {
        for (int i = 0; i < 3; i++) {
            fromclient[i] = from_client[i];      //첫번째 클라이언트

        }
        fromclient[3] = 13; fromclient[4] = 13;
        y2 = y;
        x2 = x;
        map[y2][x2] = map[y][x];
    }
    else if (clientNumber == 2) {
        for (int i = 0; i < 3; i++) {
            fromclient[i + 5] = from_client[i];         //두번쨰 클라이언트
        }
        fromclient[8] = 13; fromclient[9] = 13;
        y3 = y;
        x3 = x;
        map[y3][x3] = map[y][x];

    }
    else if (clientNumber == 3) {
        for (int i = 0; i < 3; i++) {
            fromclient[i + 10] = from_client[i];    //세번쨰 클라이언트
        }
        fromclient[13] = 13; fromclient[14] = 13;
        y4 = y;
        x4 = x;
        map[y4][x4] = map[y][x];
    }
    system("cls");
    print();
    for (i = 0; i < clientNumber; i++) {      //각 대포의 좌표를 모든 클라이언트들에게 전달한다.
       /*allClientSocket 의 I번째 클라이언트 소켓이 메시지를 전달한 클라이언트가 아니라면 */
       //if (allClientSocket[i] != myClientSocket) {
         /*fromClient 에 저장된 메시지를 allClientSocket의 I번째 클라이언트 소켓과 연결된 클라이언트에게 전송*/
        send(allClientSocket[i], (char*)fromclient, sizeof(fromclient), 0);
        //}  //if
    }  // for
    px = 13; py = 13;
    map[0][0] = 0;
    while (1) {
        // px = 13; py=13; px2 = 13; py2 = 13; px3 = 13; py3 = 13; px4 = 13; py4 = 13;
        recv(myClientSocket, (char*)fromclient_key, sizeof(fromclient_key), 0);//탱크좌표
        recv(myClientSocket, (char*)fromclient_keyp, sizeof(fromclient_keyp), 0);//대포 좌표



        if (_kbhit()) {
            int key;
            key = _getch();
            if (key == 114) {
                Fog = !(Fog & 1);
                system("cls");

                print();
            }
        }
        if (fromclient_key[0] != 13) { // 주목 하삼
           //대포의 움직임을 클라이언트로부터 가져와서 입력한다.
            if (allClientSocket[0] == myClientSocket) {
                map[py2][px2] = 0;
                py2 = fromclient_keyp[0];
                px2 = fromclient_keyp[1];
                map[py2][px2] = -1;
                map[y2][x2] = 0;
                y2 = fromclient_key[0];
                x2 = fromclient_key[1];
                map[y2][x2] = fromclient_key[2];
                toclient_key[0] = 1;
                toclient_key[1] = fromclient_key[0];
                toclient_key[2] = fromclient_key[1];
                toclient_key[3] = fromclient_key[2];
                toclient_key[4] = fromclient_keyp[0];
                toclient_key[5] = fromclient_keyp[1];

            }
            else if (allClientSocket[1] == myClientSocket) {

                map[py3][px3] = 0;
                py3 = fromclient_keyp[0];
                px3 = fromclient_keyp[1];
                map[py3][px3] = -1;
                map[y3][x3] = 0;
                y3 = fromclient_key[0];
                x3 = fromclient_key[1];
                map[y3][x3] = fromclient_key[2];
                toclient_key[0] = 2;
                toclient_key[1] = fromclient_key[0];
                toclient_key[2] = fromclient_key[1];
                toclient_key[3] = fromclient_key[2];
                toclient_key[4] = fromclient_keyp[0];
                toclient_key[5] = fromclient_keyp[1];
            }
            else if (allClientSocket[2] == myClientSocket) {
                map[py4][px4] = 0;
                py4 = fromclient_keyp[0];
                px4 = fromclient_keyp[1];
                map[py4][px4] = -1;
                map[y4][x4] = 0;
                y4 = fromclient_key[0];
                x4 = fromclient_key[1];
                map[y4][x4] = fromclient_key[2];
                toclient_key[0] = 3;
                toclient_key[1] = fromclient_key[0];
                toclient_key[2] = fromclient_key[1];
                toclient_key[3] = fromclient_key[2];
                toclient_key[4] = fromclient_keyp[0];
                toclient_key[5] = fromclient_keyp[1];
            }

            system("cls");
            print();

        }
        //else {      /////대포맞으면 서버에서 없어지기
        //    map[fromclient_key[1]][fromclient_key[2]] = 0;
        //    system("cls");
        //    print();
        for (int i = 0; i < clientNumber; i++) {
            for (int i2 = 0; i2 < 10; i2++) {
                for (int j2 = 0; j2 < 10; j2++) {
                    tomap[i2 + 1][j2] = map[i2][j2];
                }
            }
            send(allClientSocket[i], (char*)tomap, sizeof(tomap), 0);
        }

        //recv(myClientSocket, fromClient, sizeof(fromClient), 0);
          /*클라이언트에서 전달한 메시지를 화면에 출력합니다.*/
          /*현재 클라이언트가 전달한 메시지를 다른 모든 클라이언트에게 전달하는 부분입니다. 다른 모든 클라이언트에게 메시지를 전달하기 위해서 모든 클라이언트와 연결된 클라이언트 소켓이 저장된 allClientSocket에서 클라이언트 소켓을 꺼내서 모든 클라이언트에게 fromClient 배열에 저장된 메시지를 전달합니다. 이때 전송이 끝날 때 까지 다른 쓰레드에서 allClientSocket 에 저장된 클라이언트 소켓을 추가 하거나 삭제 하지 못하도록 뮤텍스로 동기화 시켰습니다.*/
        //WaitForSingleObject(mutex, INFINITE);//뮤텍스 동기 화 시작


        for (i = 0; i < clientNumber; i++) {
            /*allClientSocket 의 I번째 클라이언트 소켓이 메시지를 전달한 클라이언트가 아니라면 */
            if (allClientSocket[i] != myClientSocket) {
                /*fromClient 에 저장된 메시지를 allClientSocket의 I번째 클라이언트 소켓과 연결된 클라이언트에게 전송*/
                send(allClientSocket[i], (char*)toclient_key, sizeof(toclient_key), 0);
            }
        }  
      //   ReleaseMutex(mutex);//뮤텍스 동기화 해제     
    } //while 
    // 종료한 클라이언트 소켓 제거 부분 

    WaitForSingleObject(mutex, INFINITE);//뮤텍스 동기화 시작
    for (i = 0; i < clientNumber; i++) {
        /*allClientSocket 의 i번째 소켓이 현재 클라이언트 소켓인 myClientSocket 과 같으면*/
        if (allClientSocket[i] == myClientSocket) {
            /*현재 클라이언트 소켓의 아래쪽에 저장된 클라이언트 소켓이 하나씩 위로 당겨져서 현재 클라이언트 소켓의 자리를 덮어씀*/
            for (; i < clientNumber - 1; i++) {
                allClientSocket[i] = allClientSocket[i + 1];
            } // for
            break;
        } // if
        clientNumber--;//전체 클라이언트의 수를 1감소 시킴
        ReleaseMutex(mutex);//뮤텍스 동기화 종료
        closesocket(myClientSocket);//현재 클라이언트와 연결된 소켓을 종료함
    }  // for
    return 0;
}
int main()
{
    THD soc;

    soc.Soc_Ser();
}