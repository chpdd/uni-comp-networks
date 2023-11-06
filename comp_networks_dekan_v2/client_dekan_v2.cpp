#include <iostream>
#include <fstream>
#include <windows.h>
#include <stdio.h>
#include <filesystem>

using namespace std;

struct Student
{
    char surname[50];
    int grades[4];
};

string base_name = "C:/VisualStudioProjects/comp_networks_dekan_v2/";
string name_Request = base_name + "REQUEST0.bin";
string name_Answer = base_name + "ANSWER0.bin";

ofstream file_Request_in;
ifstream file_Answer_out;

void write_Request()
{
    Student student;
    cout << "\nВведите фамилию, и четыре оценки студента:\n";
    cin >> student.surname;
    for (int i = 0; i < 4; i++)
    {
        cin >> student.grades[i];
    }
    file_Request_in.open(name_Request, ios::binary | ios::app);
    file_Request_in.write((char*)&student, sizeof(student));
    file_Request_in.close();
}

int main() {
    setlocale(LC_ALL, "Russian");
    int index = 0;

    while (true)
    {
        name_Request = base_name + "REQUEST" + to_string(index) + ".bin";
        name_Answer = base_name + "ANSWER" + to_string(index) + ".bin";
        if (!filesystem::exists(name_Request))
        {
            file_Request_in.open(name_Request, ios::binary);
            file_Request_in.close();
            write_Request();
            int prev_size = 0;
            while (true)
            {
                Sleep(1000);
                file_Answer_out.open(name_Answer, ios::binary);
                file_Answer_out.seekg(0, ios::end);
                if (prev_size != file_Answer_out.tellg())
                {
                    int result;
                    file_Answer_out.seekg(prev_size, ios::beg);
                    file_Answer_out.read((char*)&result, sizeof(result));
                    prev_size = file_Answer_out.tellg();
                    file_Answer_out.close();
                    switch (result)
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
                else
                {
                    file_Answer_out.close();
                }
            }
        }
        index++;
    }
}