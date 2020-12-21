/*
 * @Descripttion: turing_machine.cpp
 * @version: 0.1
 * @Author: ybzhang
 * @Date: 2020-12-21 19:52:33
 * @LastEditors: ybzhang
 * @LastEditTime: 2020-12-21 20:30:09
 */
#include"common.h"


TuringMachine::read_tm(string filename)
{
    ifstream f(filename);

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
    f.close()
}