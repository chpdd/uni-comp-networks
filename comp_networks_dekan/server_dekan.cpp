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

bool find_grade(int *a, int x)
{
    for (int i = 0; i < 4; i++)
    {
        if (a[i] == x)
        {
            return true;
        }
    }
    return false;
}

int sum_grade(int* a)
{
    int sum = 0;
    for (int i = 0; i < 4; i++)
    {
        sum += a[i];
    }
    return sum;
}


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
            cout << endl;
            if (!find_grade(student.grades, 2))
            {
                if (!find_grade(student.grades, 3))
                {
                    if (sum_grade(student.grades) == 20)
                    {
                        answer.result = 5;
                    }
                    else
                    {
                        answer.result = 4;
                    }
                }
                else
                {
                    answer.result = 3;
                }
            }
            else
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