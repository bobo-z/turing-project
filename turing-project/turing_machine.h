/*
 * @Descripttion: turing_machine.h
 * @version: 0.1
 * @Author: ybzhang
 * @Date: 2020-12-21 20:05:37
 * @LastEditors: ybzhang
 * @LastEditTime: 2020-12-21 20:24:21
 */
#include "common.h"
using namespace std;
#ifndef _TURING_MACHINE_H_
#define _TURING_MACHINE_H_

string FILE_CANNOT_OPEN = "file open error";

class TuringMachine
{
private:
    vector<string> states;

public:
    int read_tm(string filename);//task 1:read .tm file, translate it into a turing machine
};

#endif