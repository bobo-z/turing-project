/*
 * @Description: turing_machine.cpp
 * @version: 0.1
 * @Author: ybzhang
 * @Date: 2020-12-21 19:52:33
 * @LastEditors: ybzhang
 * @LastEditTime: 2020-12-22 18:53:22
 */
#include"common.h"
#include"turing_machine.h"
using namespace std;

string FILE_CANNOT_OPEN = "file open error";
string SYNTAX_ERROR = "syntax error";

/**
 * @Description:TuringMachine构造函数 通过传入.tm文件构造图林机
 * @param 
 *  fname:命令行传入的图灵机文件名
 *  v:是否是verbose模式
 * @return None
 */
TuringMachine::TuringMachine(string fname, bool v)
{

    filename = fname;
    verbose = v;

    ifstream f(filename);

    if(!f.is_open())
    {
        cerr <<"\""<<filename<<"\""<< FILE_CANNOT_OPEN << endl;
        exit(1);
    }
    else
    {
        string tmp;
        int cnt = 0;//记录行数
        while(getline(f,tmp))
        {
            //可能会有行内注释
            ClearComment(tmp);
            //cout<<tmp<<endl;
            if(tmp == "")
            //do nothing
                ;
            else if(tmp[0] == '#')
            {
                string value = DefinitionAssert(tmp, cnt);
                switch(tmp[1])
                {
                    case 'Q':
                        break;
                    case 'S':
                        break;
                    case 'G':
                        break;
                    case 'B':
                        break;
                    case 'F':
                        break;
                    case 'N':
                        break;
                    default:
                        assert(tmp[1] == 'q' && tmp[2] == '0');
                        break;
                }
            }
            else
            {

            }
            cnt += 1;
        }
    }
    f.close();
}

/**
 * @Description: 清除行内注释及首尾空格
 * @param 
 *  line: 一行文本的引用
 * @return void
 */    
void TuringMachine::ClearComment(string& line)
{
    int pos = line.find(';');
    if(pos != line.npos)
    {
        line = line.erase(pos);
        const char ch = ' ';
        line.erase(line.find_last_not_of(ch) + 1);
        line.erase(0,line.find_first_not_of(ch));
    }
}

/**
 * @Description: 输出报错信息，并返回exit code
 * @param
 *  line: 错误语句
 *  str: 错误字符
 *  cnt: 文件行数
 *  error_code: 语法错误类型
 *  {
 *      1: 定义语句不完整
 *      2：含有非法字符
 * }
 * @return
 */
void TuringMachine::SyntaxError(string line, string str, int cnt, int error_code)
{
    cerr << SYNTAX_ERROR << endl;
    if(verbose)
    {
        switch(error_code)
        {
            case 1:
                cerr << "ERROR: Incomplete line in file '" << filename << "' at line " << cnt << endl
                     << "\t" << line << endl
                     << "Please check the syntax format" << endl;
                break;
            case 2:
                cerr << "ERROR: Invalid charactor '" << str << "' in file '" << filename << "' at line " << cnt << endl
                     << "\tUnknown Turing Machine Definition Syntax" << endl
                     << "\t" << line << endl;
                break;
            default:
                break;
        }
    }
    exit(1);
}

/**
 * @Description: Check the syntax of definition line
 * @param
 *  line:图灵机定义语句
 *  cnt: 语句所在文件行数
 * @return 分割出来的value
 */
string TuringMachine::DefinitionAssert(string line, int cnt)
{
    //图林继语法由7部分构成
    vector<string> Def = {"Q", "S", "G", "q0", "B", "F", "N"};
    //一条定义语句有变量、等号、值三部分构成
    bool variable_exist = false;
    bool equal_exist = false;
    bool value_exist = false;

    int first_space = line.find_first_of(' ');
    int last_space = line.find_last_of(' ');
    string variable = line.substr(1,first_space);
    string equal = line.substr(first_space + 1, last_space - first_space - 1);
    string value = line.substr(last_space + 1);
    
    //空格数>=2时才能正确分割
    //空格数<2时
    if(first_space == last_space)
    {
        SyntaxError(line, "", cnt, 1);
    }

    while(!variable_exist||!equal_exist||!value_exist)
    {
        if(!variable_exist)
        {
            auto it = find(Def.begin(), Def.end(), variable);
            if(it == Def.end())//图灵机语法有且仅有7部分
                SyntaxError(line, variable, cnt, 2);
            variable_exist = true;
        }
        else if(variable_exist && !equal_exist)
        {
            if(equal != "=")
                SyntaxError(line, equal, cnt, 2);
            equal_exist = true;
        }
        else if(variable_exist && equal_exist && !value_exist)
        {
            value_exist = true;
        }
    }
    return value;
}