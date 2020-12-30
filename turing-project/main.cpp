/*
 * @Description: main.cpp
 * @version: 0.1
 * @Author: ybzhang
 * @Date: 2020-12-21 17:25:59
 * @LastEditors: ybzhang
 * @LastEditTime: 2020-12-30 11:04:49
 */
#include"common.h"
#include"turing_machine.h"

const char* HELP_OUTPUT = "usage: turing [-v|--verbose] [-h|--help] <tm> <input>";
const char* UNKNOWN_OPTION = "unknown option:";
const char* UNKNOWN_COMMAND = "unknown command:";
const char *END = "==================== END ====================";

int main(int argc, char **argv)
{
    string tm_file = "";
    string input_str = "";
    bool verbose = false;

    if(argc == 1)
        cout<<HELP_OUTPUT<<endl;
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
                    exit(10);
                }
            }
            else//command
            {
                if(tm_file=="")
                {
                    int len = arg.length();
                    if(len>3&&arg.substr(len-3,len)==".tm")
                        tm_file = arg;
                    else
                    {
                        cerr << UNKNOWN_COMMAND << arg << endl;
                        exit(10);
                    }
                }
                else if(input_str=="")
                {
                    input_str = arg;
                }
                else
                {
                    cerr << UNKNOWN_COMMAND << arg << endl;
                    exit(10);
                }
            }
        }
    }

    TuringMachine tm(tm_file, verbose);
    string res = tm.run(input_str);
    if(verbose)
        cout << "Result: ";
    cout << res << endl;
    if(verbose)
        cout <<END << endl;
    return 0;
}