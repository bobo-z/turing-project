/*
 * @Description: turing_machine.cpp
 * @version: 0.1
 * @Author: ybzhang
 * @Date: 2020-12-21 19:52:33
 * @LastEditors: ybzhang
 * @LastEditTime: 2020-12-23 16:40:57
 */
#include"common.h"
#include"turing_machine.h"
using namespace std;

string FILE_CANNOT_OPEN = "file open error";
string SYNTAX_ERROR = "syntax error";

vector<string> Def = {"Q", "S", "G", "q0", "B", "F", "N"};//图灵机定义

/**
 * @Description: 输出报错信息，并返回exit code
 * @param
 *  line: 错误语句
 *  str: 错误字符
 *  line_cnt: 文件行数
 *  error_code: 语法错误类型
 *  {
 *      1: 定义语句不完整
 *      2：含有非法字符
 * }
 * @return
 */
void TuringMachine::SyntaxError(string str, int error_code)
{
    cerr << SYNTAX_ERROR << endl;
    if(verbose)
    {
        switch(error_code)
        {
            case 1:
                cerr << "ERROR: Incomplete line in file '" << filename << "' at line " << line_cnt << endl
                     << "\t" << reading_line << endl
                     << "Please check the syntax format" << endl;
                break;
            case 2:
                cerr << "ERROR: Invalid charactor '" << str << "' in file '" << filename << "' at line " << line_cnt << endl
                     << "\tUnknown Turing Machine Definition Syntax" << endl
                     << "\t" << reading_line << endl;
                break;
            case 3:
                cerr << "ERROR: Incomplete definition in file '" << filename << "' at line " << line_cnt << endl
                     << "Expected '" << str << "'" << endl
                     << "\t" << reading_line << endl;
                break;
            default:
                break;
        }
    }
    exit(1);
}

/**
 * @Description:TuringMachine构造函数 通过传入.tm文件构造图灵机
 * @param 
 *  fname:命令行传入的图灵机文件名
 *  v:是否是verbose模式
 * @return None
 */
TuringMachine::TuringMachine(string fname, bool v)
{

    filename = fname;
    verbose = v;
    line_cnt = 0;
    reading_line = "";

    ifstream f(filename);

    if(!f.is_open())
    {
        cerr <<"\""<<filename<<"\""<< FILE_CANNOT_OPEN << endl;
        exit(1);
    }
    else
    {
        string tmp;
        stringstream ss;
        vector<string> tokens;
        map<string, Action> actions;
        string current_state = "";
        while(getline(f,tmp))
        {
            //可能会有行内注释
            reading_line = tmp;
            ClearComment(tmp);
            //cout<<tmp<<endl;
            if(tmp == "")
            //do nothing
                ;
            else if(tmp[0] == '#')
            {
                string value = DefinitionAssert(tmp);
                switch(tmp[1])
                {
                    case 'Q':
                        if(value[0] != '{')
                            SyntaxError("{", 3);
                        if(value.back()!='}')
                            SyntaxError("}", 3);
                        Spilt(value.substr(1, value.length() - 2), states, 'Q');
                        for(auto i=states.begin(); i!=states.end();i++)
                            transitions.insert(pair<string, map<string, Action>>(*i, actions));
                        break;
                    case 'S':
                        if(value[0] != '{')
                            SyntaxError("{", 3);
                        if(value.back()!='}')
                            SyntaxError("}", 3);
                        Spilt(value.substr(1, value.length() - 2), input_char, 'S');
                        break;
                    case 'G':
                        if(value[0] != '{')
                            SyntaxError("{", 3);
                        if(value.back()!='}')
                            SyntaxError("}", 3);
                        Spilt(value.substr(1, value.length() - 2), tape_char, 'G');
                        break;
                    case 'B':
                        if(value.length()==1&&IsValid(value[0],'B'))
                            blank = value[0];
                        else
                            SyntaxError(value, 2);
                        break;
                    case 'F':
                        if(value[0] != '{')
                            SyntaxError("{", 3);
                        if(value.back()!='}')
                            SyntaxError("}", 3);
                        Spilt(value.substr(1, value.length() - 2), final_states, 'F');
                        break;
                    case 'N':
                        ss << value;
                        ss >> nTape;
                        break;
                    default:
                        assert(tmp[1] == 'q' && tmp[2] == '0');
                        start_state = value;
                        break;
                }
            }
            else//transition function
            {
                tokens.clear();
                TokenSpilt(tmp, tokens);
                Action a(tokens[2], tokens[3], tokens[4]);
                auto it = transitions.find(tokens[0]);
                it->second.insert(pair<string, Action>(tokens[1], a));
            }
            line_cnt += 1;
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
    }
    const char ch = ' ';
    line.erase(line.find_last_not_of(ch) + 1);
    line.erase(0,line.find_first_not_of(ch));
}



/**
 * @Description: Check the syntax of definition line
 * @param
 *  line:图灵机定义语句
 *  line_cnt: 语句所在文件行数
 * @return 分割出来的value
 */
string TuringMachine::DefinitionAssert(string line)
{
    //图灵继语法由7部分构成
    //一条定义语句有变量、等号、值三部分构成
    bool variable_exist = false;
    bool equal_exist = false;
    bool value_exist = false;

    int first_space = line.find_first_of(' ');
    int last_space = line.find_last_of(' ');
    string variable = line.substr(1,first_space-1);
    string equal = line.substr(first_space + 1, last_space - first_space - 1);
    string value = line.substr(last_space + 1);
    
    //空格数>=2时才能正确分割
    //空格数<2时
    if(first_space == last_space)
    {
        SyntaxError("", 1);
    }

    while(!variable_exist||!equal_exist||!value_exist)
    {
        if(!variable_exist)
        {
            auto it = find(Def.begin(), Def.end(), variable);
            if(it == Def.end())//图灵机语法有且仅有7部分
                SyntaxError(variable,2);
            variable_exist = true;
        }
        else if(variable_exist && !equal_exist)
        {
            if(equal != "=")
                SyntaxError(equal,2);
            equal_exist = true;
        }
        else if(variable_exist && equal_exist && !value_exist)
        {
            value_exist = true;
        }
    }
    return value;
}



/**
 * @Description: spilt the string into a set
 * @param 
 *  val: 形如{...,...,...,...}的string
 *  words: 存放每一个词作为返回值
 *  type: 字符类型（IsValid)
 * @return void
 */
void TuringMachine::Spilt(string val, set<string> &words, char type)
{
    /*state:
    0   stop
    1   read
    2   continue
    */
    //int state = 0;
    string temp = "";
    string i_str = "";
    for(auto i = val.begin(); i != val.end();++i,i_str.clear(),i_str.push_back(*i))
    {
        //i_str.clear();
        //i_str.push_back(*i);
        if(*i==',')
        {
            if(temp == "")
                SyntaxError(i_str, 2);
            if(type == 'S' || type == 'G')
                if(temp.length()>1)
                    SyntaxError(i_str, 2);
            words.insert(temp);
            temp = "";
        }
        else
        {
            if(!IsValid(*i,type))
                SyntaxError(i_str, 2);
            else
                temp = temp + *i;
        }
    }
    if(temp == "")
        SyntaxError(i_str, 2);
    if(type == 'S' || type == 'G')
        if(temp.length()>1)
            SyntaxError(i_str, 2);
    words.insert(temp);
}

/**
 * @Description: whether the given char is valid
 * @param 
 *  ch: 
 * @return true/false
 */
bool TuringMachine::IsValid(char ch, char type)
{
    switch(type)
    {
        case 'S':case 'G':case 'B'://symbol
            if(ch>=32 && ch<=126 &&ch != ' ' && ch != ',' && ch != ';'&&ch != '{'&&ch!='}'&&ch != '*')
                return true;
            else
                return false;
            break;
        case 'Q':case 'q':case 'F'://state
            if(ch>='a'&&ch<='z')
                return true;
            else if(ch>='A'&&ch<='Z')
                return true;
            else if(ch>='0'&&ch<='9')
                return true;
            else if(ch=='_')
                return true;
            else
                return false;
            break;
        default:
            return false;
    }
    
}

/**
 * @Description: whether the given string is valid
 * @param 
 *  ch: 
 * @return true/false
 */
bool TuringMachine::IsValid(string str, char type)
{
    if(str=="")
        return false;
    for(auto i=str.begin();i!=str.end();i++)
    {
        char ch = *i;
        if(!IsValid(ch, type))
            return false;
    }
    return true;
}

void TuringMachine::TokenSpilt(string str, vector<string>& tokens)
{
    string temp;
    for(auto i=str.begin();i!=str.end();i++)
    {
        if(*i==' ')
        {
            tokens.push_back(temp);
            temp = "";
        }
        else
            temp = temp + *i;
    }
    tokens.push_back(temp);
}

void TuringMachine::run(string input)
{
    for (int i = 0; i < nTape;i++)
    {
        list<char> tape(1, ' ');
        tapes.push_back(tape);
        heads.push_back(tapes[i].begin());
    }
    tapes[0].clear();
    for (auto i = input.begin(); i != input.end();i++)
        tapes[0].push_back(*i);
    heads[0]=tapes[0].begin();
    string current_state = start_state;
    while(true)
    {
        auto it = final_states.find(current_state);
        if(it != final_states.end())//halt
        {
            tapes[0].remove(blank);
            for(auto i=tapes[0].begin(); i!=tapes[0].end();i++)
                cout << *i;
            return;
        }
        auto trans_it = transitions.find(current_state);//TODO: check
        string current_symbol = "";
        for (int i = 0; i < nTape;i++)
        {
            char ch = *heads[i];
            if(ch == ' ')
                ch = blank;
            current_symbol = current_symbol + ch;
        }
        auto action_it = trans_it->second.find(current_symbol);//TODO check
        Action a = action_it->second;
        MoveWrite(a.direction, a.write);
        current_state = a.next;
    }
}

void TuringMachine::MoveWrite(string dir, string input)//move all tape headers and write
{
    assert(dir.length() == nTape);
    for(int i = 0; i < nTape;i++)
    {
        *heads[i] = input[i];
        switch(dir[i])
        {
            case 'l':
                if(heads[i]==tapes[i].begin())
                {
                    tapes[i].push_front(' ');
                    heads[i]--;
                }
                else
                    heads[i]--;
                break;
            case 'r':
                if(heads[i]==--tapes[i].end())
                {
                    tapes[i].push_back(' ');
                    heads[i]++;
                }
                else
                    heads[i]++;
                break;
            case '*':
                break;
            default:
                assert(0);
                break;
        }
    }
}