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

int y, x, y2, x2, y3, x3, y4, x4; // �����ġ
int speed, mode;
bool Fog = 0;
int po, py, px, py2, px2, py3, px3, py4, px4;//��������,������ǥ

void  print() {
    if (Fog) {
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 10; j++) {
                if (map[i][j] != 1) { cout << "��"; }      //1�� ���� �ǹ�
                else if (map[i][j] == 1) { cout << "��"; }      //2�� ���� �ǹ�
            }
            cout << endl;
        }
    }
    else {
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 10; j++) {
                if (map[i][j] == 0) cout << "��";
                else if (map[i][j] == 1) { cout << "��"; }   //2�� ���� �ǹ�
                else if (map[i][j] == 2) { cout << "��"; }      //������ ���⸶�� �ٸ��� ����
                else if (map[i][j] == 3) { cout << "��"; }
                else if (map[i][j] == 4) { cout << "��"; }
                else if (map[i][j] == 5) { cout << "��"; }
                else if (map[i][j] == -1)cout << "��";
            }
            cout << endl;
        }
    }
}
/*Ŭ���̾�Ʈ�� �޽����� �ְ� ���� ��Ʈ*/
#define  PORT 5500
/*Ŭ���̾�Ʈ���� ������ �޽����� �ִ� ũ��*/
#define BUFFERSIZE 100
/*Ŭ���̾�Ʈ ��ȭ���� �ִ� ũ��*/
#define NAMESIZE 30
/*���ؽ���  ������ ����*/
HANDLE  mutex;
/*���� �������� Ŭ���̾�Ʈ�� ���� ������ ����
   ����� ���� ���� Ŭ���̾�Ʈ�� �����Ƿ� �ʱⰪ�� 0*/
int clientNumber = 0;
/*�������� ID �� ������ ����*/
unsigned  threadID;
/*���� �������� ��� Ŭ���̾�Ʈ�� Ŭ���̾�Ʈ ������ ������ ����*/
int allClientSocket[100];
int Number[3] = { 0, };
int Fog_client[3] = { 0, };      //Ŭ���̾�Ʈ�鿡�� ���������� �������ִ� ����
class THD {

public:
    int Soc_Ser();
    static unsigned __stdcall broadcastAllClient(void* arg);
};
int THD::Soc_Ser()
{
    /*�����带 ����ȭ �ϱ� ���ؼ� ���ؽ� ��ü�� �����մϴ�.
    ���ؽ� ��ü�� �����ϴµ� �����ϸ� �����޽����� ����ϰ� ���α׷��� �����մϴ�.*/
    mutex = CreateMutex(NULL, FALSE, NULL);
    WSADATA wsadata;
    WSAStartup(MAKEWORD(2, 2), &wsadata);
    /*Ŭ���̾�Ʈ�� ������ ��ٸ��� ���� ���� ����*/
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    cout << "���� ������ ���� �߽��ϴ�." << endl;
    /*������ �ּҸ� �����ϴ� ����ü serverAddress ����*/
    sockaddr_in serverAddress;
    /*serverAddress ����ü�� 0���� �ʱ�ȭ*/
    memset(&serverAddress, 0, sizeof(serverAddress));
    /*serverAddress �� ���� IP ���� INADDR_ANY->�ڵ����� IP �� �����ϴ� ���*/
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    /*�ּ� Ÿ�� ����*/
    serverAddress.sin_family = AF_INET;
    /*Ŭ���̾�Ʈ�� �޽����� �ְ� ���� ��Ʈ ��ȣ ����*/
    serverAddress.sin_port = htons(PORT);
    /*���� ���Ͽ� �ּ����� �������մϴ�.������ �߻��ϸ� ���� �޽����� ����ϰ� ���α׷��� �����մϴ�. */
    if (bind(serverSocket, (sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        cout << "���� ������ ���ε� �ϴµ� �����߽��ϴ�." << endl;
        return 0;
    }
    /*Ŭ���̾�Ʈ�� ������ ��ٸ��� ������ ��ȯ*/
    if (listen(serverSocket, 5) == -1) {
        cout << "���� ������ listen ���� �����ϴµ� �����߽��ϴ�" << endl;
        return 0;
    }
    cout << "������ ���� �Է��Ͻʽÿ� (3������)    �Է� : ";
    cin >> Number[0];
    system("cls");
    cout << "���� ��� ����" << endl;
    cout << "�Ϲݸ�� : 1�� , �Ȱ���� : 2��     �Է� : ";
    cin >> Fog_client[0];
    system("cls");
    cout << "�Է��Ͻ� �����ڸ�ŭ ��������ּ���!";
    while (1) {
        /*Ŭ���̾�Ʈ�� �ּ� ������ ������ ����ü ����*/
        struct sockaddr_in clientAddress;
        int clientAddressSize = sizeof(clientAddress);
        /*Ŭ���̾�Ʈ�� ������ ����ϰ� Ŭ���̾�Ʈ�� �����͸�    �ְ�޴� Ŭ���̾�Ʈ ���ϻ���
         �մϴ�. Ŭ���̾�Ʈ�� �ּ� ������ clientAddress �� �����մϴ�.*/
        int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress,
            &clientAddressSize);
        /*���ؽ��� �̿��ؼ� ���� �ڿ��� ��޴ϴ�. ���� �ڿ��� ���� ���� �ٸ� ������� ����
           �ڿ��� ����� �� ���� ��� ���¿��� ��ٸ��ϴ�.*/
        WaitForSingleObject(mutex, INFINITE);
        /*���� �������� Ŭ���̾�Ʈ�� ���� 1���� ��ŵ�ϴ�.*/
        clientNumber++;
        /*���� �������� ��� Ŭ���̾�Ʈ�� Ŭ���̾�Ʈ ������ �����ϴ� �迭 allClientSocket ��
      Ŭ���̾�Ʈ������ �����մϴ�.*/
        allClientSocket[clientNumber - 1] = clientSocket;
        /*���� ������ ����� �����մϴ�. ���� �ڿ��� ����� �� ��� ��� ���̴� �ٸ�������
          �� ��� ���°� Ǯ���ϴ�.*/
        ReleaseMutex(mutex);
        /*Ŭ���̾�Ʈ���� ������ �޽����� �о� �鿩�� ��� Ŭ���̾�Ʈ�� �����ϴ� broadcastAllClient �Լ��� ȣ���մϴ�.*/
        unsigned long thread;
        thread = _beginthreadex(NULL, 0, broadcastAllClient, (void*)clientSocket, 0, &threadID);
    }
    cout << "������ ���� �մϴ�" << endl;
    return 0;
}
int from_client[3]; // ��ũ x�� y�� ��簪
int fromclient[15] = { 13,13,13,13,13,13,13,13,13,13,13,13,13,13,13 };
int fromclient_key[3] = { 13,13,13 };//��ũ ��ǥ
int toclient_key[6]; //��ũ�� ��ǥ�� ����
int fromclient_keyp[2]{ 13,13 }; //��������ǥ
int die[2]; //���� �Ű�


int tomap[11][10] = { 100, };
unsigned __stdcall THD::broadcastAllClient(void* arg)
{
    /*Ŭ���̾�Ʈ�� �����ϸ� main �Լ����� Ŭ���̾�Ʈ���� ������ ����ϰ� Ŭ���̾�Ʈ�� �����͸� �ְ� ������ �ִ� Ŭ���̾�Ʈ ������ �����մϴ�. ��Ƽ ������� broadcastAllClient �Լ��� �����ϴµ� �̶� ���ڷ� Ŭ���̾�Ʈ ������ �����մϴ�. main �Լ����� ������ Ŭ���̾�Ʈ ������ myClientsocket ������ �����߽��ϴ�.*/
    int myClientSocket = (int)arg;
    int i = 0;
    /*myClientSocket���� ����� ���� Ŭ���̾�Ʈ�� ������ �޽����� �о� ���̴� �κ��Դϴ�. ���� Ŭ���̾�Ʈ�� ������ ���� �Ҷ� ���� ��� �ݺ��մϴ�.*/
    //����� �������� �ο����� �޾Ƽ� Ŭ���̾�Ʈ�鿡�� �����Ѵ�.
    send(myClientSocket, (char*)Number, sizeof(Number), 0);
    send(myClientSocket, (char*)Fog_client, sizeof(Fog_client), 0);
    recv(myClientSocket, (char*)from_client, sizeof(from_client), 0);//�� Ŭ���̾�Ʈ�κ��� ���� ������ ó�� ��ũ ��ǥ�� �����Ѵ�.
    y = from_client[0];
    x = from_client[1];
    map[y][x] = from_client[2];
    if (clientNumber == 1) {
        for (int i = 0; i < 3; i++) {
            fromclient[i] = from_client[i];      //ù��° Ŭ���̾�Ʈ

        }
        fromclient[3] = 13; fromclient[4] = 13;
        y2 = y;
        x2 = x;
        map[y2][x2] = map[y][x];
    }
    else if (clientNumber == 2) {
        for (int i = 0; i < 3; i++) {
            fromclient[i + 5] = from_client[i];         //�ι��� Ŭ���̾�Ʈ
        }
        fromclient[8] = 13; fromclient[9] = 13;
        y3 = y;
        x3 = x;
        map[y3][x3] = map[y][x];

    }
    else if (clientNumber == 3) {
        for (int i = 0; i < 3; i++) {
            fromclient[i + 10] = from_client[i];    //������ Ŭ���̾�Ʈ
        }
        fromclient[13] = 13; fromclient[14] = 13;
        y4 = y;
        x4 = x;
        map[y4][x4] = map[y][x];
    }
    system("cls");
    print();
    for (i = 0; i < clientNumber; i++) {      //�� ������ ��ǥ�� ��� Ŭ���̾�Ʈ�鿡�� �����Ѵ�.
       /*allClientSocket �� I��° Ŭ���̾�Ʈ ������ �޽����� ������ Ŭ���̾�Ʈ�� �ƴ϶�� */
       //if (allClientSocket[i] != myClientSocket) {
         /*fromClient �� ����� �޽����� allClientSocket�� I��° Ŭ���̾�Ʈ ���ϰ� ����� Ŭ���̾�Ʈ���� ����*/
        send(allClientSocket[i], (char*)fromclient, sizeof(fromclient), 0);
        //}  //if
    }  // for
    px = 13; py = 13;
    map[0][0] = 0;
    while (1) {
        // px = 13; py=13; px2 = 13; py2 = 13; px3 = 13; py3 = 13; px4 = 13; py4 = 13;
        recv(myClientSocket, (char*)fromclient_key, sizeof(fromclient_key), 0);//��ũ��ǥ
        recv(myClientSocket, (char*)fromclient_keyp, sizeof(fromclient_keyp), 0);//���� ��ǥ



        if (_kbhit()) {
            int key;
            key = _getch();
            if (key == 114) {
                Fog = !(Fog & 1);
                system("cls");

                print();
            }
        }
        if (fromclient_key[0] != 13) { // �ָ� �ϻ�
           //������ �������� Ŭ���̾�Ʈ�κ��� �����ͼ� �Է��Ѵ�.
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
        //else {      /////���������� �������� ��������
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
          /*Ŭ���̾�Ʈ���� ������ �޽����� ȭ�鿡 ����մϴ�.*/
          /*���� Ŭ���̾�Ʈ�� ������ �޽����� �ٸ� ��� Ŭ���̾�Ʈ���� �����ϴ� �κ��Դϴ�. �ٸ� ��� Ŭ���̾�Ʈ���� �޽����� �����ϱ� ���ؼ� ��� Ŭ���̾�Ʈ�� ����� Ŭ���̾�Ʈ ������ ����� allClientSocket���� Ŭ���̾�Ʈ ������ ������ ��� Ŭ���̾�Ʈ���� fromClient �迭�� ����� �޽����� �����մϴ�. �̶� ������ ���� �� ���� �ٸ� �����忡�� allClientSocket �� ����� Ŭ���̾�Ʈ ������ �߰� �ϰų� ���� ���� ���ϵ��� ���ؽ��� ����ȭ ���׽��ϴ�.*/
        //WaitForSingleObject(mutex, INFINITE);//���ؽ� ���� ȭ ����


        for (i = 0; i < clientNumber; i++) {
            /*allClientSocket �� I��° Ŭ���̾�Ʈ ������ �޽����� ������ Ŭ���̾�Ʈ�� �ƴ϶�� */
            if (allClientSocket[i] != myClientSocket) {
                /*fromClient �� ����� �޽����� allClientSocket�� I��° Ŭ���̾�Ʈ ���ϰ� ����� Ŭ���̾�Ʈ���� ����*/
                send(allClientSocket[i], (char*)toclient_key, sizeof(toclient_key), 0);
            }
        }  
      //   ReleaseMutex(mutex);//���ؽ� ����ȭ ����     
    } //while 
    // ������ Ŭ���̾�Ʈ ���� ���� �κ� 

    WaitForSingleObject(mutex, INFINITE);//���ؽ� ����ȭ ����
    for (i = 0; i < clientNumber; i++) {
        /*allClientSocket �� i��° ������ ���� Ŭ���̾�Ʈ ������ myClientSocket �� ������*/
        if (allClientSocket[i] == myClientSocket) {
            /*���� Ŭ���̾�Ʈ ������ �Ʒ��ʿ� ����� Ŭ���̾�Ʈ ������ �ϳ��� ���� ������� ���� Ŭ���̾�Ʈ ������ �ڸ��� ���*/
            for (; i < clientNumber - 1; i++) {
                allClientSocket[i] = allClientSocket[i + 1];
            } // for
            break;
        } // if
        clientNumber--;//��ü Ŭ���̾�Ʈ�� ���� 1���� ��Ŵ
        ReleaseMutex(mutex);//���ؽ� ����ȭ ����
        closesocket(myClientSocket);//���� Ŭ���̾�Ʈ�� ����� ������ ������
    }  // for
    return 0;
}
int main()
{
    THD soc;

    soc.Soc_Ser();
}