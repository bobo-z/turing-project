/*
 * @Description: turing_machine.h
 * @version: 0.1
 * @Author: ybzhang
 * @Date: 2020-12-21 20:05:37
 * @LastEditors: ybzhang
 * @LastEditTime: 2020-12-22 18:42:25
 */
#include "common.h"
using namespace std;
#ifndef _TURING_MACHINE_H_
#define _TURING_MACHINE_H_

class TuringMachine
{
private:
    vector<string> states;
    string filename;
    bool verbose;

public:
    TuringMachine (string filename, bool verbose);
    void ClearComment(string& line);
    string DefinitionAssert(string line,int cnt);
    void SyntaxError(string line, string str, int cnt, int error_code);
};

#endif