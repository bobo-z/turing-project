/*
 * @Descripttion: 
 * @version: 
 * @Author: ybzhang
 * @Date: 2020-12-21 17:25:59
 * @LastEditors: ybzhang
 * @LastEditTime: 2020-12-21 19:45:17
 */
#include <stdio.h>
#include <iostream>
#include <string>
using namespace std;

string HELP_OUTPUT = "usage: turing [-v|--verbose] [-h|--help] <tm> <input>";
string UNKNOWN_OPTION = "unknown option:";
string UNKNOWN_COMMAND = "unknown command:";

int main(int argc, char **argv)
{
    string tm_file = "";
    string input_str = "";
    bool verbose = false;

    if(argc == 1)
        cout<<HELP_OUTPUT<<endl;
        //fprintf(stdout, "%s\n", HELP_OUTPUT);
    else
    {
        for(int i = 1; i < argc;i++)
        {   
            string arg = argv[i];
            if(arg[0]=='-')//option
            {
                if(arg == "-h"||arg == "--help")
                {
                    cout <<HELP_OUTPUT << endl;
                    break;
                }
                else if(arg == "-v"||arg == "--verbose")
                    verbose = true;
                else
                {
                    cerr << UNKNOWN_OPTION << arg << endl;
                    exit(1);
                }
            }
            else//command
            {
                if(tm_file=="")
                {
                    int len = arg.length();
                    cout<<len<<endl;
                    if(len>3&&arg.substr(len-3,len)==".tm")
                        tm_file = arg;
                    else
                    {
                        cerr << UNKNOWN_COMMAND << arg << endl;
                        exit(1);
                    }
                }
                else if(input_str=="")
                {
                    input_str = arg;
                }
                else
                {
                    cerr << UNKNOWN_COMMAND << arg << endl;
                    exit(1);
                }
            }
        }
    }
    return 0;
}