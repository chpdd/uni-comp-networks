#include <string>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <iostream>
#include <sstream>
#include <string>
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment (lib, "Ws2_32.lib")
using namespace std;

int main() {
	WSADATA wsaData;
	struct addrinfo* addr = NULL;
	struct addrinfo hints;
	ZeroMemory(&hints, sizeof(hints));//предназначена для обнуления памяти,первый определяет, 
	//начиная с какого места в памяти мы делаем ее обнуление и второй определяет, сколько байт мы обнуляем.
	hints.ai_family = AF_INET;
	hints.ai_flags = AI_PASSIVE;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_socktype = SOCK_STREAM;

	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);//нициирует использование библиотеки DLL Winsock процессом
	if (result != 0) {
		cerr << "WSAStartup failed: " << result << "\n";
		return result;
	}

	if ((result = getaddrinfo("localhost", "8000", &hints, &addr)) != 0) {
		cerr << "GetAddrInfo Error: " << result << "\n";
		WSACleanup();
		return 1;
	}
	//создание сокета
	int listen_socket = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
	if (listen_socket == INVALID_SOCKET) {
		cerr << "Error at socket: " << WSAGetLastError() << "\n";
		freeaddrinfo(addr);
		WSACleanup();
		return 1;
	}
	//привязываем сокет к айпи адресу
	result = bind(listen_socket, addr->ai_addr, (int)addr->ai_addrlen);
	if (result == SOCKET_ERROR) {
		cerr << "bind failed with error: " << WSAGetLastError() << "\n";
		freeaddrinfo(addr);
		closesocket(listen_socket);
		WSACleanup();
		return -1;
	}
	//инициализируем слушающий сокет
	if (listen(listen_socket, SOMAXCONN) == SOCKET_ERROR)
	{
		cerr << "listen failed with error: " << WSAGetLastError() << "\n";
		closesocket(listen_socket);
		WSACleanup();
		return -1;
	}

	const int max_client_buffer_size = 1024;
	char buf[max_client_buffer_size];
	int client_socket = INVALID_SOCKET;
	//принимаем входящие данные
	for (;;) {
		client_socket = accept(listen_socket, NULL, NULL);
		if (client_socket == INVALID_SOCKET) {
			cerr << "accept failed: " << WSAGetLastError() << "\n";
			closesocket(listen_socket);
			WSACleanup();
			return 1;
		}

		stringstream response;
		stringstream response_body;

		result = recv(client_socket, buf, max_client_buffer_size, 0);
		if (result == SOCKET_ERROR) {
			//ошибка получения данных
			cerr << "recv failed: " << result << "\n";
			closesocket(client_socket);
		}//соединение закрыто клиентом
		else if (result == 0) {
			cerr << "connection closed \n";

		}
		//данные запроса успешно получены
		else if (result > 0)
		{
			buf[result] = '\0';
			//формируем тело ответа 
			response_body << "<title> Test C++ HTTP Server</title>\n"
				<< "<h1>Test page</h1>\n"
				<< "<p>This is body of the test page...</p>\n"
				<< "<h2>Request headers</h2>\n"
				<< "<pre>" << buf << "</pre\n>"
				//включаем в ответ полученный запрос 
				<< "<em><small>Test C++ Http Server </small></em>\n";
			//ответ вместе с заголовками
			response << "HTTP/1.1 200 OK\r\n" << "Version: HTTP/1.1\r\n" << "Content - Type: text/html; charset=utf-8\r\n" << "Content-Length: " << response_body.str().length() << "\r\n\r\n" << response_body.str();
			//отправка клиенту
			result = send(client_socket, response.str().c_str(), response.str().length(), 0);

			if (result == SOCKET_ERROR) {
				cerr << "send failed: " << WSAGetLastError() << "\n";
			}
			//закрыли соединеие
			closesocket(client_socket);
		}
	}
	closesocket(listen_socket);
	freeaddrinfo(addr);
	WSACleanup();
	return 0;

}