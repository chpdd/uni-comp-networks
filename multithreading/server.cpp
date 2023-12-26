#define _WINSOCK_DEPRECATED_NO_WARNINGS  
#include <iostream>
#include <winsock2.h>
#include <string>
#pragma comment(lib, "Ws2_32.lib")
using namespace std;
u_short MY_PORT = 666;
//порт который слушает сервер
/*макрос для печати кол-ва активных пользователей*/
#define PRINTNUSERS if (nclients) cout << "user on-line" << nclients << endl; else cout << "NO User on-line\n";


struct Person {
	int weight;
	int height;
	char secondName[25];
}P;

double getBMI(int height, int weight) {
	return double(weight * 10000) / (height * height);
}
/*прототип функции, обслуживающий подключивщихся пользователей*/
DWORD WINAPI ConToClient(LPVOID client_socket);
/*глобальнвя переменная - колво активных пользователей*/
int nclients = 0;

int main() {
	char buff[1024];//буфер для различных нужд
	cout << "TCP ECHO-SERVER \n";
	//инициализация библиотеки сокетов
	if (WSAStartup(0x0202, (WSADATA*)&buff[0])) {
		cout << "Error WSAStartup \n" << WSAGetLastError();
		return 1;
	}
	//создание сокета
	SOCKET mysocket;
	if ((mysocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		cout << "Error socket \n" << WSAGetLastError();
		WSACleanup();//деинициализация библиотеки winsock
		return -1;
	}
	//связывание сокета с локальным адресом
	sockaddr_in local_addr;
	local_addr.sin_addr.s_addr = 0;
	local_addr.sin_family = AF_INET;
	local_addr.sin_port = htons(MY_PORT);
	if (bind(mysocket, (sockaddr*)&local_addr, sizeof(local_addr))) {
		cout << "Error bind \n" << WSAGetLastError();//ошибка
		closesocket(mysocket);//закрываем сокет
		WSACleanup();
		return -1;
	}
	//ожидание подключений размер очереди - 0x100
	if (listen(mysocket, 0x100)) {
		cout << "Error listen \n" << WSAGetLastError();//ошибка
		closesocket(mysocket);
		WSACleanup();
		return -1;
	}

	cout << "Waiting connections\n";
	SOCKET client_socket;//сокет для клиента 
	sockaddr_in client_addr;//адрес клиента 
	int client_addr_size = sizeof(client_addr);
	/*цикл извлечения запросов на подключение из очереди*/
	while ((client_socket = accept(mysocket, (sockaddr*)&client_addr, &client_addr_size))) {//создание многопот-и
		nclients++;//увеличиваем счетчик подключивщихся клиентов
		HOSTENT* hst;//пытаемся получить имя хоста
		hst = gethostbyaddr((char*)&client_addr.sin_addr.s_addr, 4, AF_INET);
		cout << "+new connect!\n";
		//вывод сведений о клиенте 
		if (hst) cout << hst->h_name;
		//символьное имя клиента
		else cout << "";
		cout << inet_ntoa(client_addr.sin_addr);
		PRINTNUSERS
			cout << endl;
		//создание нового потока для обслуживания клиента
		DWORD thID;
		CreateThread(NULL, NULL, ConToClient, &client_socket, NULL, &thID);//поток
	}
	return 0;
}
/*эта функция создается для отдельного потока и обслуживает очередного подключившегося клиента независимо от остальных*/
DWORD WINAPI ConToClient(LPVOID client_socket) {
	SOCKET my_sock;
	int len;
	my_sock = ((SOCKET*)client_socket)[0];
	char buff[1024];
	//цикл эхо-сервера
	while (SOCKET_ERROR != (len = recv(my_sock, (char*)&P, sizeof(P), 0))) {//обработка
		cout << "New Client was connected!" << endl;
		double BMI = getBMI(P.height, P.weight);
		cout << "Received Data" << endl;
		cout << "Second Name:" << P.secondName << "; Height:" << P.height << "; Weight:" << P.weight << endl;
		cout << BMI << endl;
		send(my_sock, (char*)&BMI, sizeof(BMI), 0);
	}
	/*выход из цикла соединение разорвано */
	nclients--;//уменьшаем счетчик активных клиентов
	cout << "-disconnect\n";
	PRINTNUSERS
		closesocket(my_sock);//закрыли сокет
	return 0;
}