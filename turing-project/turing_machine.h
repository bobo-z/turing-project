/*
 * @Description: turing_machine.h
 * @version: 0.1
 * @Author: ybzhang
 * @Date: 2020-12-21 20:05:37
 * @LastEditors: ybzhang
 * @LastEditTime: 2020-12-23 15:34:32
 */
#include "common.h"
using namespace std;
#ifndef _TURING_MACHINE_H_
#define _TURING_MACHINE_H_

struct Action
{
    string next;//next state
    string write;//write char to tape
    string direction;//direction to move
    Action(string w, string d, string n)
    {
        next = n;
        write = w;
        direction = d;
    };
};

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
    char blank;
    string start_state;
    int nTape;

    //transition function
    //map<string, Action> actions;//key-old symbol val-aciton
    map<string, map<string, Action> > transitions;//key-state  val-transition
    vector<list<char> > tapes;
    vector<list<char>::iterator> heads;

    //argc
    string filename;
    //string input;
    bool verbose;

public:
    TuringMachine (string filename, bool verbose);
    void ClearComment(string& line);
    string DefinitionAssert(string line);
    void SyntaxError(string str, int error_code);
    void Spilt(string val, set<string> &words, char type);
    bool IsValid(char ch,char type);
    bool IsValid(string str,char type);
    void TokenSpilt(string str, vector<string> &tokens);
    void run(string input);
    void MoveWrite(string dir, string input);
};

#endif