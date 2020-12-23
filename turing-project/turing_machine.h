/*
 * @Description: turing_machine.h
 * @version: 0.1
 * @Author: ybzhang
 * @Date: 2020-12-21 20:05:37
 * @LastEditors: ybzhang
 * @LastEditTime: 2020-12-23 10:03:15
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
    int nTape;

    //argc
    string filename;
    string input;
    bool verbose;

public:
    TuringMachine (string filename, bool verbose);
    void ClearComment(string& line);
    string DefinitionAssert(string line);
    void SyntaxError(string str, int error_code);
    void Spilt(string val, set<string> &words, char type);
    bool IsValid(char ch,char type);
    bool IsValid(string str,char type);
};

#endif