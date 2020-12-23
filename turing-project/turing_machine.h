/*
 * @Description: turing_machine.h
 * @version: 0.1
 * @Author: ybzhang
 * @Date: 2020-12-21 20:05:37
 * @LastEditors: ybzhang
 * @LastEditTime: 2020-12-24 00:26:13
 */
#include "common.h"
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

struct Cell
{
    int index;
    char symbol;
    Cell(int i, char s)
    {
        index = i;
        symbol = s;
    }
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
    set<char> input_char;//S
    set<char> tape_char;//G
    char blank;
    string start_state;
    int nTape;

    map<string, map<string, Action> > transitions;//key-state  val-transition
    vector<list<Cell> > tapes;
    vector<list<Cell>::iterator> heads;

    //argc
    string filename;
    bool verbose;

public:
    TuringMachine (string filename, bool verbose);
    void ClearComment(string& line);
    string DefinitionAssert(string line);
    void Error(vector<string> err_str, int error_code);
    void Spilt(string val, set<string> &words, char type);
    bool IsValid(char ch,char type);
    void IsValid(string str);
    bool IsNum(string str);
    void TokenSpilt(string str, vector<string> &tokens);
    void run(string input);
    void MoveWrite(string dir, string input);
    void Print(string current_state, int step);
};

#endif