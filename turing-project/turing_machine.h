/*
 * @Descripttion: turing_machine.h
 * @version: 0.1
 * @Author: ybzhang
 * @Date: 2020-12-21 20:05:37
 * @LastEditors: ybzhang
 * @LastEditTime: 2020-12-22 14:06:27
 */
#include "common.h"
using namespace std;
#ifndef _TURING_MACHINE_H_
#define _TURING_MACHINE_H_

class TuringMachine
{
private:
    vector<string> states;

public:
    int read_tm(string filename);//task 1:read .tm file, translate it into a turing machine
};

#endif