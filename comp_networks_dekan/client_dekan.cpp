#include <iostream>
#include <fstream>
#include <windows.h>

using namespace std;

struct Student
{
    char surname[50];
    int grades[4];
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

string name_Request = "C:/VisualStudioProjects/comp_networks_dekan/REQUEST.bin";
string name_Answer = "C:/VisualStudioProjects/comp_networks_dekan/ANSWER.bin";
ofstream file_Request_in(name_Request, ios::app | ios::binary);
ifstream file_Request_out;
ofstream file_Answer_in(name_Answer, ios::app | ios::binary);
ifstream file_Answer_out;

void write_Request()
{
    cout << "\nВведите фамилию, и четыре оценки студента:\n";
    cin >> student.surname;
    for (int i = 0; i < 4; i++)
    {
        cin >> student.grades[i];
    }
    request.student = student;
    file_Request_in.open(name_Request, ios::app | ios::binary);
    file_Request_in.seekp(0, ios::end);
    request.id = file_Request_in.tellp() / sizeof(request);
    file_Request_in.write((char*)&request, sizeof(request));
    file_Request_in.close();
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
                case 2:
                {
                    cout << "У студента есть задолженности, стипендии нет" << endl;
                    break;
                }
                case 3:
                {
                    cout << "У студента задолженностей нет, стипендии тоже" << endl;
                    break;
                }
                case 4:
                {
                    cout << "У студента задолженностей нет, стипендия 1500 рублей" << endl;
                    break;
                }
                case 5:
                {
                    cout << "У студента задолженностей нет, стипендия 3000 рублей" << endl;
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