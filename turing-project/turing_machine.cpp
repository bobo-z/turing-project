/*
 * @Descripttion: turing_machine.cpp
 * @version: 0.1
 * @Author: ybzhang
 * @Date: 2020-12-21 19:52:33
 * @LastEditors: ybzhang
 * @LastEditTime: 2020-12-22 14:14:38
 */
#include"common.h"
#include"turing_machine.h"
using namespace std;

string FILE_CANNOT_OPEN = "file open error";

int TuringMachine::read_tm(string filename)
{
    ifstream f(filename);

    //f.open();

    if(!f.is_open())
    {
        cerr <<"\""<<filename<<"\""<< FILE_CANNOT_OPEN << endl;
        exit(1);
    }
    else
    {
        string tmp;
        while(getline(f,tmp))
        {
            cout<<tmp<<endl;
        }
    }
    f.close();
    return 0;
}