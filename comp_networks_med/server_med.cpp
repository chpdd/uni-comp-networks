#include <iostream>
#include <fstream>
#include <windows.h>

using namespace std;

struct Student
{
    char surname[50];
    float height;
    float weight;
} student;

struct Request
{
    Student student;
    int id;
} request;

struct Answer
{
    int result;
    int id;
} answer;

string name_Request = "C:/VisualStudioProjects/comp_networks_med/REQUEST.bin";
string name_Answer = "C:/VisualStudioProjects/comp_networks_med/ANSWER.bin";
ofstream file_Request_in(name_Request, ios::app | ios::binary);
ifstream file_Request_out;
ofstream file_Answer_in(name_Answer, ios::app | ios::binary);
ifstream file_Answer_out;


int main() {
    setlocale(LC_ALL, "Russian");
    file_Request_in.close();
    file_Answer_in.close();

    cout << "Сервер работает\n";

    file_Request_out.open(name_Request, ios::binary);
    file_Request_out.seekg(0, ios::end);
    long long size_last = file_Request_out.tellg();
    file_Request_out.close();

    while (true)
    {
        Sleep(1000);
        file_Request_out.open(name_Request, ios::binary);
        file_Request_out.seekg(0, ios::end);
        if (size_last != file_Request_out.tellg())
        {
            file_Request_out.seekg(size_last, ios::beg);
            file_Request_out.read((char*)&request, sizeof(request));
            student = request.student;
            answer.id = request.id;
            float index_weight = student.weight / (student.height * student.height);
            if (index_weight < 18.5)
            {
                answer.result = 0;
            }
            if (index_weight >= 18.5 && index_weight <= 25)
            {
                answer.result = 1;
            }
            if (index_weight > 25)
            {
                answer.result = 2;
            }
            file_Answer_in.open(name_Answer, ios::app | ios::binary);
            file_Answer_in.write((char*)&answer, sizeof(answer));
            file_Answer_in.close();
            size_last += sizeof(request);
        }
        file_Request_out.close();
    }
}