#include <iostream> 
#define _WINSOCK_DEPRECATED_NO_WARNINGS 
#include <winsock2.h>
#include <string>
#include <windows.h>
#pragma comment (lib, "Ws2_32.lib")
#pragma warning(disable: 4996) 
using namespace std;
#define SRV_HOST "localhost" 
#define SRV_PORT 666
#define CLNT_PORT 1235 
#define BUF_SIZE 64 

struct Person {
    int weight;
    int height;
    char secondName[25];
}P;


int main() {
    char buff[1024];
    if (WSAStartup(0x0202, (WSADATA*)&buff[0])) {
        cout << "Error WSAStartup \n" << WSAGetLastError();  // Ошибка!
        return 1;
    }
    SOCKET s;
    int from_len;   char buf[BUF_SIZE] = { 0 };
    hostent* hp;
    sockaddr_in clnt_sin, srv_sin;
    s = socket(AF_INET, SOCK_STREAM, 0);
    clnt_sin.sin_family = AF_INET;
    clnt_sin.sin_addr.s_addr = 0;
    clnt_sin.sin_port = htons(CLNT_PORT);
    bind(s, (sockaddr*)&clnt_sin, sizeof(clnt_sin));
    hp = gethostbyname(SRV_HOST);
    srv_sin.sin_port = htons(SRV_PORT);
    srv_sin.sin_family = AF_INET;
    ((unsigned long*)&srv_sin.sin_addr)[0] =
        ((unsigned long**)hp->h_addr_list)[0][0];
    connect(s, (sockaddr*)&srv_sin, sizeof(srv_sin));
    double BMI;
    while (1) {
        cout << "Input Second Name: \n";
        cin >> P.secondName;
        cout << "Height; Weight:";
        cin >> P.height >> P.weight;
        if (P.height == 1 || P.weight == 1) break;
        int* BMIData = new int[2] { P.height, P.weight };
        send(s, (char*)&P, sizeof(P), 0);
        from_len = recv(s, (char*)&BMI, sizeof(BMI), 0);

        if (from_len != sizeof(BMI)) {
            cout << "Data recive error";
            return 1;
        }
        else {
            cout << P.secondName << " has ";
            if (BMI >= 18.5 && BMI < 24.9) cout << "Normal BMI\n";
            if (BMI < 18.5) cout << "Underweight\n";
            if (BMI >= 24.9) cout << "Overweight\n";
        }
    }
    Sleep(5000);
    closesocket(s);
    return 0;
}
