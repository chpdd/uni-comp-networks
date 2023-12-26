#include <string>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <iostream>
#pragma comment (lib, "Ws2_32.lib")
#pragma warning(disable: 4996)
using namespace std;
#define request "GET/ HTTP/1.1\r\nHost: localhost \r\n\r\n"//www

#define max_packet_size 65535
//127.0.0.1:8000
int main() {
	WSADATA ws;
	SOCKET s;
	sockaddr_in adr;
	HOSTENT* hn;

	char buff[max_packet_size];

	if (WSAStartup(0x0202, &ws) != 0) { return -1; }//(WORD <запрашиваемая версия сокетов>, WSADATA * <указатель на структуру, 
	//хранящую текущую версию реализации сокетов>)

	if (INVALID_SOCKET == (s = socket(AF_INET, SOCK_STREAM, 0))) {//создаем сокет
		return -1;
	}
	//получаем айпи адрес сервера по символьному имени
	if (NULL == (hn = gethostbyname("localhost")))//www
	{
		return -1;
	}
	//задаем параметры сервера в адресной структуре 
	adr.sin_family = AF_INET;
	((unsigned long*)&adr.sin_addr)[0] =
		((unsigned long**)hn->h_addr_list)[0][0];
	adr.sin_port = htons(8000);//порт
	//устанавливаем соединение с сервером 
	if (SOCKET_ERROR == connect(s, (sockaddr*)&adr, sizeof(adr)))
	{
		int res = WSAGetLastError();
		return -1;
	}
	//посылаем запрос серверу
	if (SOCKET_ERROR == send(s, (char*)&request, sizeof(request), 0)) {
		int res = WSAGetLastError();
		return -1;
	}

	int len = 0;

	do {//получаем данные по частям пока лен не 0
		if (SOCKET_ERROR == (len = recv(s, (char*)&buff, max_packet_size, 0))) {
			int res = WSAGetLastError();
			return -1;
		}
		buff[len] = 0;
		cout << buff;//вывод полученной порции данных
	} while (len != 0);

	Sleep(5000);
	//закрываем соединение
	if (SOCKET_ERROR == closesocket(s)) {
		return -1;
	}

	return 0;
}