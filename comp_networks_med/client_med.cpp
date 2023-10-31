#include <iostream>
#include <fstream>
#include <windows.h>
#include <string>
#include <ctime>

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

void write_Request()
{
    cout << "\nВведите фамилию, рост и вес студента:\n";
    cin >> student.surname >> student.height >> student.weight;
    student.height /= 100;
    request.student = student;
    file_Request_in.open(name_Request, ios::app | ios::binary);
    file_Request_in.seekp(0, ios::end);
    cout << file_Request_in.tellp() << endl;
    request.id = file_Request_in.tellp() / sizeof(request);
    file_Request_in.write((char*)&request, sizeof(request));
    file_Request_in.close();
    cout << request.id << endl;
}

int main() {
    setlocale(LC_ALL, "Russian");
    file_Request_in.close();
    file_Answer_in.close();
    
    file_Answer_out.open(name_Answer, ios::binary);
    file_Answer_out.seekg(0, ios::end);
    long long size_last = file_Answer_out.tellg();
    file_Answer_out.close();

    write_Request();

    while (true)
    {
        Sleep(1000);
        file_Answer_out.open(name_Answer, ios::binary);
        file_Answer_out.seekg(0, ios::end);
        if (size_last != file_Answer_out.tellg())
        {
            file_Answer_out.seekg(size_last, ios::beg);
            file_Answer_out.read((char*)&answer, sizeof(answer));
            if (request.id == answer.id)
            {
                switch (answer.result)
                {
                case 0:
                {
                    cout << "Индекс веса ниже среднего" << endl;
                    break;
                }
                case 1:
                {
                    cout << "Индекс веса в норме" << endl;
                    break;
                }
                case 2:
                {
                    cout << "Индекс веса выше среднего" << endl;
                    break;
                }
                }
                write_Request();
            }
            size_last += sizeof(answer);
        }
        file_Answer_out.close();
    }
}
