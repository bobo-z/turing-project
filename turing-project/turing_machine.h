/*
 * @Description: turing_machine.h
 * @version: 0.1
 * @Author: ybzhang
 * @Date: 2020-12-21 20:05:37
 * @LastEditors: ybzhang
 * @LastEditTime: 2020-12-22 21:41:02
 */
#include "common.h"
using namespace std;
#ifndef _TURING_MACHINE_H_
#define _TURING_MACHINE_H_



class TuringMachine
{
private:
    //方便报错
    string reading_line;
    int line_cnt;

    //define
    set<string> states;//Q
    set<string> final_states;//F
    set<string> input_char;//S
    set<string> tape_char;//G
    string blank;

    //argc
    string filename;
    string input;
    bool verbose;

public:
    TuringMachine (string filename, bool verbose);
    void ClearComment(string& line);
    string DefinitionAssert(string line,int cnt);
    void SyntaxError(string str, int error_code)
    void Spilt(string val, vector<string> &words, int type);
    bool IsValid(char ch,int type);
    bool IsValid(string str, int type);
};

#endif