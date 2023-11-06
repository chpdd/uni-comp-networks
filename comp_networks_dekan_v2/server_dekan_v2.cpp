#include <iostream>
#include <fstream>
#include <windows.h>
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

ifstream file_Request_out;
ofstream file_Answer_in;

bool find_grade(int* a, int x)
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
    cout << "Сервер работает\n";
    vector<int> v_sizes = { 0, 0, 0, 0, 0 };
    int index = 0;
    while (true)
    {
        name_Request = base_name + "REQUEST" + to_string(index) + ".bin";
        name_Answer = base_name + "ANSWER" + to_string(index) + ".bin";
        if (filesystem::exists(name_Request))
        {
            if (index > v_sizes.size() - 1)
            {
                v_sizes.push_back(0);
            }
            file_Request_out.open(name_Request, ios::binary);
            file_Request_out.seekg(0, ios::end);
            if (v_sizes[index] != file_Request_out.tellg())
            {
                Student student;
                file_Request_out.seekg(v_sizes[index], ios::beg);
                file_Request_out.read((char*)&student, sizeof(student));
                v_sizes[index] = file_Request_out.tellg();
                file_Request_out.close();
                int result;
                if (!find_grade(student.grades, 2))
                {
                    if (!find_grade(student.grades, 3))
                    {
                        if (sum_grade(student.grades) == 20)
                        {
                            result = 5;
                        }
                        else
                        {
                            result = 4;
                        }
                    }
                    else
                    {
                        result = 3;
                    }
                }
                else
                {
                    result = 2;
                }
                file_Answer_in.open(name_Answer, ios::binary | ios::app);
                file_Answer_in.write((char*)&result, sizeof(result));
                file_Answer_in.close();
            }
            else
            {
                file_Request_out.close();
            }
            index++;
        }
        else
        {
            index = 0;
        }
    }
}