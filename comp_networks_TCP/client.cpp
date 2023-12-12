#include <iostream>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <string>
#include <windows.h>
#pragma comment (lib, "Ws2_32.lib")
#pragma warning(disable: 4996) //visual studio
using namespace std;

#define SERVER_HOST "localhost"
#define SERVER_PORT 1234
#define CLIENT_PORT 1235
#define INFO_SIZE 64
#define ADRESS 0

struct Student {
    char surname[50];
    int grades[4];
};

struct CommandResult {
    int messageNumber;
    int result;
};

string resultHandler(CommandResult comRes) {
    switch (comRes.result) {
        case 2: {
            return "Student has debts, no scholarship";
        }
        case 3: {
            return "Student has no debts, no scholarship.";
        }
        case 4: {
            return "Student has no debts, scholarship 1500 rubles";
        }
        case 5: {
            return "Student has no debts, scholarship 3000 rubles";
        }
    }
}

int main() {
    /*char wsaData[1024];*/
    WSADATA wsaData;
    if (WSAStartup(0x0202, &wsaData)) {
        cerr << "Error WSAStartup: \n" << WSAGetLastError();
        return -1;
    }

    SOCKET sock;
    sockaddr_in sinClient, sinServer; // sin - socket adress in
    hostent* pHostent; // hostent - host entry
    int lenReceived;
    char infoReceived[INFO_SIZE] = { 0 };

    //в этом блоке подсоединяем клиент с сервером
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // AF_INET - Address Family - Internet(IPv4)
    sinClient.sin_family = AF_INET;
    sinClient.sin_addr.s_addr = ADRESS;
    sinClient.sin_port = htons(CLIENT_PORT); // htons - host to network short преобразует в сетевой порядок расположения байтов
    bind(sock, (sockaddr*)&sinClient, sizeof(sinClient));
    pHostent = gethostbyname(SERVER_HOST); // gethostbyname - возвращает структуру hostent по имени хоста
    sinServer.sin_family = AF_INET;
    sinServer.sin_port = htons(SERVER_PORT);
    ((unsigned long*)&sinServer.sin_addr)[0] = ((unsigned long**)pHostent->h_addr_list)[0][0];
    connect(sock, (sockaddr*)&sinServer, sizeof(sinServer));
    
    boolean commandFlag = false;
    string stringSend, stringReceived;
    cout << "Help: Write messages up to 64 characters or write /calcSchol' to calculate a student's scholarship" << endl;
    do {
        if (commandFlag == false) {
            fill(infoReceived, infoReceived + sizeof(infoReceived), 0);
            lenReceived = recv(sock, (char*)&infoReceived, INFO_SIZE, 0);
            stringReceived = infoReceived;

            cout << "Server: " << stringReceived << endl;
            if (stringReceived == "Enter the student's data.") {
                commandFlag = true;
                continue;
            }
            cout << "You: ";
            getline(cin, stringSend);

            send(sock, (char*)&stringSend[0], stringSend.size(), 0);
        } else {
            Student student;
            cout << "Name: ";
            cin >> student.surname;
            cout << "Grades: ";
            for (int i = 0; i < size(student.grades); i++) {
                cin >> student.grades[i];
            }
            send(sock, (char*)&student, sizeof(student), 0);

            CommandResult comRes;
            lenReceived = recv(sock, (char*)&comRes, sizeof(comRes), 0);
            cout << resultHandler(comRes) << endl;
            commandFlag = false;
        }
        
    } while (stringSend != "Bye");
    closesocket(sock);
    return 0;
}
