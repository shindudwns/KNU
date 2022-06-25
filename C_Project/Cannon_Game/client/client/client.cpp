#include <winsock2.h>
#include <windows.h>
#include <iostream>
#include <process.h>
#include <stdlib.h>
#include <time.h>
#include<conio.h>
#include<mutex>

#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80
#define STOP 115
#define CANNON 32

#pragma comment (lib, "ws2_32.lib")
#pragma warning(disable:4996)

using namespace std;

mutex _mutex;


int map[10][10]{
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
int x, y, x4, y4, x2, y2, x3, y3;      //���� ��ǥ
int po, px, py, px2, py2, px3, py3, px4, py4;//��������,������ǥ
int key, ckey, mkey, akey, last; //Ű�Է� ��Ű ����Ű �Է°� ����
int stac, st, kill, o, p;//���ý׾Ƽ� Ȯ�ο�
bool Fog = 0;      //�Ȱ���带 �ǹ�

void skip(int i, int j) {
    if (map[i][j] == -1) { cout << "��"; }
    else  if (map[i][j] == 0) { cout << "��"; }      //1�� ���� �ǹ�
    else if (map[i][j] == 1) { cout << "��"; }   //2�� ���� �ǹ�
    else if (map[i][j] == 2) { cout << "��"; }      //������ ���⸶�� �ٸ��� ����
    else if (map[i][j] == 3) { cout << "��"; }
    else if (map[i][j] == 4) { cout << "��"; }
    else if (map[i][j] == 5) { cout << "��"; }
    else if (map[i][j] == 6) { cout << "��"; }      //������ ���⸶�� �ٸ��� ����
    else if (map[i][j] == 7) { cout << "��"; }
    else if (map[i][j] == 8) { cout << "��"; }
    else if (map[i][j] == 9) { cout << "��"; }
}
void print() { //��� ��� �Ͻ�
    cout << endl << endl << endl << "              Sky ��" << endl << endl << endl;
    if (Fog == 0) {
        for (int i = 0; i < 10; i++) {
            cout << "        ";
            for (int j = 0; j < 10; j++) {
                skip(i, j);
            }cout << endl;
        }
    }
    else {  //�Ȱ� ��� �Ͻ�
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 10; j++) {
                if (map[i][j] == 1) { cout << "��"; }
                else {
                    if (map[y][x] == 2) {
                        if (j == x && i <= y && i >= y - 5) skip(i, j);
                        else  cout << "��";
                    }
                    else if (map[y][x] == 3) {
                        if (i == y && j <= x && j >= x - 5) skip(i, j);
                        else  cout << "��";
                    }
                    else if (map[y][x] == 4) {
                        if (i == y && j >= x && j <= x + 5) skip(i, j);
                        else cout << "��";
                    }
                    else if (map[y][x] == 5) {
                        if (i == x && j >= y && j <= y + 5) skip(i, j);
                        else cout << "��";
                    }
                }
            } cout << endl;
        } cout << endl;
    }
}

void _print(int a) {
    cout << endl << endl << endl << "              ���� ��" << endl << endl << endl;
    cout << "        ";
    if (Fog == 0) {
        for (int k = 0; k < 10; k++) {
            if (map[a][k] != 0)skip(a, k);
            else cout << "  ";
        }cout << endl << "        ";
        for (int k = 0; k < 10; k++) cout << "��";
        cout << endl;
    }
    else {
        for (int i = 0; i < 10; i++) {
            if (map[y][i] == 1) { cout << "��"; }
            else {
                if (map[y][x] == 3 && i <= x && i >= x - 5) {
                    if (map[a][i] == -1) { cout << "��"; }
                    else  if (map[a][i] == 0) { cout << "  "; }
                    else if (map[a][i] == 2) { cout << "��"; }
                    else if (map[a][i] == 3) { cout << "��"; }
                    else if (map[a][i] == 4) { cout << "��"; }
                    else if (map[a][i] == 5) { cout << "��"; }
                    else if (map[a][i] == 6) { cout << "��"; }
                    else if (map[a][i] == 7) { cout << "��"; }
                    else if (map[a][i] == 8) { cout << "��"; }
                    else if (map[a][i] == 9) { cout << "��"; }
                }
                else if (map[y][x] == 4 && i >= x && i <= x + 5) {
                    if (map[a][i] == -1) { cout << "��"; }
                    else  if (map[a][i] == 0) { cout << "  "; }
                    else if (map[a][i] == 2) { cout << "��"; }
                    else if (map[a][i] == 3) { cout << "��"; }
                    else if (map[a][i] == 4) { cout << "��"; }
                    else if (map[a][i] == 5) { cout << "��"; }
                    else if (map[a][i] == 6) { cout << "��"; }
                    else if (map[a][i] == 7) { cout << "��"; }
                    else if (map[a][i] == 8) { cout << "��"; }
                    else if (map[a][i] == 9) { cout << "��"; }
                }
                else {
                    if (map[a][i] == 2) { cout << "��"; }
                    else if (map[a][i] == 3) { cout << "��"; }
                    else if (map[a][i] == 4) { cout << "��"; }
                    else if (map[a][i] == 5) { cout << "��"; }
                    else if (map[a][i] == -1) { cout << "��"; }
                    else { cout << "  "; }
                }
            }
        }cout << endl;
        for (int i = 0; i < 10; i++) {
            cout << "��";
        }
    }
}
bool stop;
void cannon() { //������ ó�� �� ���������ְ� ù�߽�� �Լ�
    p = map[y][x];
    //stac = 0;
    px = x; py = y;
    stac = 0; p = map[y][x];
    if (map[y][x] == 2) {//��
        if (y != 0 && map[y - 1][x] != 1) { px = x; py = y; py -= 1; map[py][px] = -1; }p = 2; stac = 1;
    }
    else if (map[y][x] == 3) {//��
        if (x != 0 && map[y][x - 1] != 1) { px = x; py = y; px -= 1; map[py][px] = -1; }p = 3; stac = 1;
    }
    else if (map[y][x] == 4) {//��
        if (x != 9 && map[y][x + 1] != 1) { px = x; py = y; px += 1; map[py][px] = -1; }p = 4; stac = 1;
    }
    else if (map[y][x] == 5) {//�Ʒ�
        if (y != 9 && map[y + 1][x] != 1) { px = x; py = y; py += 1; map[py][px] = -1; }p = 5; stac = 1;
    }

}
void cannon2() { //������ ��°�� ���� ��� �Լ�
    if (kill != 0) { kill = 0; stac = 0; }
    if (stac > 0 && stac < 4) {
        if (stac == 3) { stac = 0; px = 15; py = 15; }
        else {
            if (p == 2) {//����
                if (py != 0 && map[py - 1][px] != 1) {
                    py -= 1; map[py][px] = -1;
                    cout << py << " " << px << endl;
                }
            }
            else if (p == 3) {//����
                if (px != 0 && map[py][px - 1] != 1) {
                    px -= 1; map[py][px] = -1; cout << py << " " << px << endl;
                }
            }
            else if (p == 4) {//����
                if (px != 9 && map[py][px + 1] != 1) {
                    px += 1; map[py][px] = -1; cout << py << " " << px << endl;
                }
            }
            else if (p == 5) {//�Ʒ�
                if (py != 9 && map[py + 1][px] != 1) {
                    py += 1; map[py][px] = -1; cout << py << " " << px << endl;
                }
            }
            stac++;
        }
        map[y][x] = last;
    }
}
void Move() { // Ű���ް� �����̴� �Լ� (������ ���ԵǾ��ֽ��ϴ�)
    cannon2();
    if (key == CANNON) {
        if (stac == 0) {
            kill = 0; cannon();
        }key = mkey;
    }
    else if (key == LEFT) {
        if (x != 0 && map[y][x - 1] < 1) { map[y][x] = 0; x -= 1; map[y][x] = 3; }
    }
    else if (key == RIGHT) {
        if (x != 9 && map[y][x + 1] < 1) {
            map[y][x] = 0; x += 1; map[y][x] = 4;
        }
    }
    else if (key == UP) {
        if (y != 0 && map[y - 1][x] < 1) {
            map[y][x] = 0; y -= 1; map[y][x] = 2;
        }
    }
    else if (key == DOWN) {
        if (y != 9 && map[y + 1][x] < 1) {
            map[y][x] = 0; y += 1; map[y][x] = 5;
        }
    }
    else if (key == STOP) { map[y][x] = last; }
    else {
        if (map[y][x] == 2) {//��
            if (y != 0 && map[y - 1][x] < 1) { map[y][x] = 0; y -= 1; map[y][x] = 2; }
        }
        else if (map[y][x] == 3) {//��
            if (x != 0 && map[y][x - 1] < 1) { map[y][x] = 0; x -= 1; map[y][x] = 3; }
        }
        else if (map[y][x] == 4) {//��
            if (x != 9 && map[y][x + 1] < 1) { map[y][x] = 0; x += 1; map[y][x] = 4; }
        }
        else if (map[y][x] == 5) {//�Ʒ�
            if (y != 9 && map[y + 1][x] < 1) { map[y][x] = 0; y += 1; map[y][x] = 5; }
        }
        cout << endl << "�߸��Ȱ��� �Է��߽��ϴ�!!!!!!!" << endl;
    }
}
#define PORT   5500
#define BUFFERSIZE 100
#define NAMESIZE 30
class Cannon {
public:
    Cannon() { cout << "Cannon Ŭ������ �����Ͽ����ϴ�!" << endl; }
    virtual void end_message() = 0;
    ~Cannon() { cout << "Cannon Ŭ������ �Ҹ��Ͽ����ϴ�!" << endl; }
};
unsigned  sendThread, receiveThread;
class THD : public Cannon {
public:
    int Soc_Cli();
    static unsigned __stdcall sendMessage(void* arg);
    static unsigned __stdcall receiveMessage(void* arg);
    void end_message() { cout << "������ ����Ǿ����ϴ�!" << endl; }
};

int THD::Soc_Cli()
{
    WSADATA wsadata;
    WSAStartup(MAKEWORD(2, 2), &wsadata);

    int mySocket = socket(PF_INET, SOCK_STREAM, 0);
    if (mySocket == -1) {
        cout << "Ŭ���̾�Ʈ ������ �����ϴµ� ���� �߽��ϴ�" << endl;
        return 0;
    }
    sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);
    if (connect(mySocket, (sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        cout << "������ �����ϴµ� �����߽��ϴ�" << endl;
        return 0;
    }
    unsigned long hSendThread = _beginthreadex(NULL, 0, sendMessage, (void*)mySocket, 0, &sendThread);

    unsigned long hReceiveThread = _beginthreadex(NULL, 0, receiveMessage, (void*)mySocket, 0, &receiveThread);

    WaitForSingleObject((HANDLE)hSendThread, INFINITE);
    WaitForSingleObject((HANDLE)hReceiveThread, INFINITE);
}

int toserver[3] = { 0, };  // x y ��ũ ��ǥ��
int fromserver[15];          // 3��Ŭ���̾�Ʈ ��ũ ������ǥ �޴� ����
int arr[3] = { 15,15,15 };     //�ش� Ŭ���̾�Ʈ ��ũ ��ǥ
int fromserver_key[6];   // 0�迭�� ���° Ŭ���̾�Ʈ��    1~3�� ��ũ ��ǥ�� ���� 4,5�� ������ ��ǥ
int die[2] = { 13,13 }; //�״°� Ȯ�� ���ִ°�

unsigned __stdcall THD::sendMessage(void* arg) {
    for (int i = 0; i < 15; i++) { fromserver[i] = 13; }
    do {      //�������� ��ũ ��ġ ����
        srand((unsigned)(time)(NULL));
        x = rand() % 10;
        y = rand() % 10;
    } while (map[y][x] == 1);
    map[y][x] = 3;
    toserver[0] = y;
    toserver[1] = x;
    toserver[2] = map[y][x];
    int mySocket = (int)arg;
    send(mySocket, (char*)toserver, sizeof(toserver), 0);
    print();
    _print(y);
    py = 15; px = 15; py2 = 15; px2 = 15; py3 = 15; px3 = 15; py4 = 15; px4 = 15;
    key = 0;
    key = _getch();
    mkey = STOP;
    if (key != 0) {
        while (1) {
            map[py][px] = 0; map[py2][px2] = 0; map[py3][px3] = 0; map[py4][px4] = 0;
            if (mkey == CANNON) { akey = mkey; }
            if (key != CANNON) {
                if (key == STOP) {
                    ckey = mkey;// ���� �������� ��ž�̾����� mkey���� ����  mkey���� ĳ���̾ƴ� Ű ���尪
                }mkey = key;
            }
            //last = map[y][x]; //������ ȭ��ǥ ���Ⱚ ����
            //key = _getch();
            if (_kbhit()) { key = _getch(); if (key == 224)key = _getch(); }//���� ����Ű�� ���´ٸ� ������ �ҷ��ռ� ����
            Move();
            //Sleep(speed);
            die[0] = py; die[1] = px;
            arr[0] = y;
            arr[1] = x;
            arr[2] = map[y][x];
            last = map[y][x]; //������ ȭ��ǥ ���Ⱚ ����
            send(mySocket, (char*)arr, sizeof(arr), 0);// ��ũ ��ǥ��
            send(mySocket, (char*)die, sizeof(die), 0);// ������ ��ǥ��

            Sleep(450);
            system("cls");
            print();
            _print(y);
        }
    }
    return 0;
}

int Number[3] = { 0, };
int Fog_server[3] = { 0, };      //�����κ��� � ��������� �ҷ����� ����
int servermap[11][10];
unsigned __stdcall THD::receiveMessage(void* arg) {
    int mySocket = (int)arg; // main �Լ��κ��� ���޹��� Ŭ���̾�Ʈ ������ mySocket �� ����
    int strlen = 0; // �������� ������ �޽����� ����Ʈ ���� ������ ����*/
    recv(mySocket, (char*)Number, sizeof(Number), 0);
    recv(mySocket, (char*)Fog_server, sizeof(Fog_server), 0);
    if (Fog_server[0] == 1) {      //�Ϲ� ���
        Fog = 0;
    }
    else if (Fog_server[0] == 2) {      //�Ȱ����
        Fog = 1;
    }
    while (1) {
        recv(mySocket, (char*)fromserver, sizeof(fromserver), 0);
        if (fromserver[2] != 13) {
            y2 = fromserver[0];
            x2 = fromserver[1];
            map[y2][x2] = fromserver[2];// +4;
            py2 = fromserver[3];
            px2 = fromserver[4];
            map[py2][px2] = -1;
        }
        if (fromserver[7] != 13) {
            x3 = fromserver[5];
            y3 = fromserver[6];
            map[x3][y3] = fromserver[7];// +4;
            py3 = fromserver[8];
            px3 = fromserver[9];
            map[py3][px3] = -1;
        }
        if (fromserver[12] != 13) {
            x4 = fromserver[10];
            y4 = fromserver[11];
            map[x4][y4] = fromserver[12];// +4;
            py4 = fromserver[13];
            px4 = fromserver[14];
            map[py4][px4] = -1;
        }
        map[y][x] = toserver[2]; //���ΰ�
        system("cls");
        print();
        _print(y);
        if (Number[0] == 1) { //����̼� �ϴ����� ���� ���� ����
            if (fromserver[2] != 13)
                break;
        }
        if (Number[0] == 2) {
            if (fromserver[7] != 13)
                break;
        }
        if (Number[0] == 3) {
            if (fromserver[12] != 13)
                break;
        }
    }
    while (1) {
        recv(mySocket, (char*)servermap, sizeof(servermap), 0);
        if (servermap[0][0] == 100) {
            for (int i = 0; i < 10; i++) {
                for (int j = 0; j < 10; j++) {
                    map[i][j] = servermap[i + 1][j];
                }
            }
            system("cls");
            _mutex.lock();
            print();
            _print(y);
            _mutex.unlock();
        }
        else {
            recv(mySocket, (char*)fromserver_key, sizeof(fromserver_key), 0);
            if (fromserver_key[0] == 1) {
                map[y2][x2] = 0;
                y2 = fromserver_key[1];
                x2 = fromserver_key[2];
                map[y2][x2] = fromserver_key[3];

                py2 = fromserver_key[4];
                px2 = fromserver_key[5];
                map[py2][px2] = -1;
            }
            else if (fromserver_key[0] == 2) {
                map[y3][x3] = 0;
                y3 = fromserver_key[1];
                x3 = fromserver_key[2];
                map[y3][x3] = fromserver_key[3];

                py3 = fromserver_key[4];
                px3 = fromserver_key[5];
                map[py3][px3] = -1;
            }
            else if (fromserver_key[0] == 3) {
                map[y4][x4] = 0;
                y4 = fromserver_key[1];
                x4 = fromserver_key[2];
                map[y4][x4] = fromserver_key[3];
                py4 = fromserver_key[4];
                px4 = fromserver_key[5];
                map[py4][px4] = -1;
            }
            if (y == fromserver_key[4] && x == fromserver_key[5]) { 
                system("cls");
                cout << "����� ������ �������� �й��Ͽ����ϴ�." << endl;
                //closesocket(mySocket);
                return 0;
            }
            /*   else {
                  map[fromserver_key[1]][fromserver_key[2]] = 0;
               }*/

        }
    }
    return 0;
}

int main()
{
    THD cli;

    cli.Soc_Cli();
    cli.end_message();
}