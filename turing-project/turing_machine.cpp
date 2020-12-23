/*
 * @Description: turing_machine.cpp
 * @version: 0.1
 * @Author: ybzhang
 * @Date: 2020-12-21 19:52:33
 * @LastEditors: ybzhang
 * @LastEditTime: 2020-12-23 21:14:39
 */
#include"common.h"
#include"turing_machine.h"

//ERROR
#define FILE_OPEN_ERROR 1
#define INCOMPLETE_LINE 2
#define INVALID_CHAR 3
#define STATE_MISS 4
#define NOT_CHAR 5
#define NOT_NUM 6
#define INCOMPLETE_FUNC 7
#define CHAR_MISS 8
#define STATUS_MISS 9
#define COMMAND_LINE_ERROR 10
#define INVALID_INPUT 11
#define DEF_MISS 12
#define CHAR_NUM_ERR 13
#define ERR_DIR 14

#define TRANS_FUNC_PART 5//转移方程5部分


const char *error_line = "==================== ERR ====================";
const char *end_line = "==================== END ====================";
const char *run_line = "==================== RUN ====================";
const char *cur_off_line = "---------------------------------------------";
const char* FILE_CANNOT_OPEN = "file open error";
const char* SYNTAX_ERROR = "syntax error";
const char* INPUT_ERROR = "illegal input";

set<string> Def = {"Q", "S", "G", "q0", "B", "F", "N"};//图灵机定义

/**
 * @Description: 输出报错信息，并返回exit code
 * @param
 *  line: 错误语句
 *  str: 错误字符
 *  line_cnt: 文件行数
 *  error_code: 语法错误类型
 *  {
 *      1: 文件打开错误（文件缺失、路径错误）
 *      2: 定义语句不完整
 *          定义语句需由三部分构成：变量、等号、值
 *          变量：由'#'开始，其余部分需在Def定义之中
 *          等号：左右两侧各有一个空格
 *          值：有些需用大括号{}括起来
 *      3：含有非法字符
 *      4：赋值失败——初始或终止状态不在Q内
 *      5：赋值失败——B的值仅能有一个字符
 *      6：赋值失败——N的值需为数字
 *      7：转移方程定义不完整（需由五部分构成）
 *      8：缺少字符
 *      9：状态匹配错误（不存在对于纸带old_symbol的转移函数
 *      10：命令行格式错误
 *      11:输入含有非法字符
 *      12:定义不完整(图灵机七个部分都得有定义)
 *      13:纸带数与操作数不吻合
 *      14:纸带移动方向错误
 *  必须先定义，再写转移函数
 * }
 * @return
 */
void TuringMachine::Error(vector<string> err_str, int error_code)
{
    switch(error_code)
    {
        case 1:
            cerr << FILE_CANNOT_OPEN << endl;
            if(verbose)
            {
                cerr << error_line << endl
                     << "error: \""
                     << filename << "\"cannot open. Please check the file name and try again."  << endl;
            }
            break;
        case 2:
            cerr <<SYNTAX_ERROR << endl;
            if(verbose)
            {
                cerr << error_line << endl
                     << "error: Incomplete definition format in file '" << filename << "' at line " << line_cnt << endl
                     << "\t" << reading_line << endl
                     << "Please check the format:" << endl
                     << "<variable><=><value>\t(One Space separated)" << endl;
            }
            break;
        case 3:
            cerr <<SYNTAX_ERROR << endl;
            if(verbose)
            {
                cerr << error_line << endl
                     << "error: Invalid charactor '" << err_str[0] << "' in file '" << filename << "' at line " << line_cnt << endl
                     << "\t" << reading_line << endl
                     << "Please check the Turing Machine symbol set" << endl;
            }
            break;
        case 4:
            cerr <<SYNTAX_ERROR << endl;
            if(verbose)
            {
                cerr << error_line << endl
                     << "error: Assignment failed because of invalid state: " << err_str[0] << "' in file '" << filename << "' at line " << line_cnt << endl
                     << "\t" << reading_line << endl
                     << "Please make sure the " << err_str[1] << " is in the total states" << endl;
            }
            break;
        case 5:
            cerr <<SYNTAX_ERROR << endl;
            if(verbose)
            {
                cerr << error_line << endl
                     << "error: Invalid "<<err_str[1]<<" character '" << err_str[0] << "' in file '" << filename << "''"
                     << "' at line " << line_cnt << endl
                     << "\t" << reading_line << endl
                     <<err_str[1]<< " character contain ONLY ONE character" << endl;
            }
            break;
        case 6:
            cerr <<SYNTAX_ERROR << endl;
            if(verbose)
            {
                cerr << error_line << endl
                     << "error: Invalid tape number '" << err_str[0] << "' in file '" << filename << "''"
                     << "' at line " << line_cnt << endl
                     << "\t" << reading_line << endl
                     << "Tape number should be a NUMBER" << endl;
            }
            break;
        case 7:
            cerr <<SYNTAX_ERROR << endl;
            if(verbose)
            {
                cerr << error_line << endl
                     << "error: Transition function definition error"
                     << " in file '" << filename << "''"
                     << "' at line " << line_cnt << endl
                     << "\t" << reading_line << endl
                     << "Please check the format: " << endl
                     << "<Current_state><Scanned_symbol><Print_symbol><Move_tape><Next_state>\t(Space separated)" << endl;
            }
            break;
        case 8:
            cerr <<SYNTAX_ERROR << endl;
            if(verbose)
            {
                cerr << error_line << endl
                     << "error: Incomplete line in file '" << filename << "' at line " << line_cnt << endl
                     << "\t" << reading_line << endl
                     << "Expected " << err_str[0] << endl;
            }
            break;
        case 9:
            cerr <<SYNTAX_ERROR << endl;
            if(verbose)
            {
                cerr << error_line << endl
                     << "error: Status match error."
                     << " in file '" << filename << "''"
                     << "' at line " << line_cnt << endl
                     << "\t" << reading_line << endl
                     << "Status '" << err_str[0] << "' does not match any trans function" << endl;
            }
            break;
        case 11:      
            if(verbose)
            {
                cerr << error_line << endl
                     << "error: '" << err_str[0] << "' was not declared in the set of input symbols" << endl
                     << "Input: " << err_str[1] << endl
                     << err_str[2] << endl;
            }
            else
                cerr <<INPUT_ERROR << endl;
            break;
        case 12:
            cerr <<SYNTAX_ERROR << endl;
            if(verbose)
            {
                cerr << error_line << endl
                     << "error: " << err_str[0] << err_str[1] << "Undefined." << endl;
            }
        case 13:
            cerr << SYNTAX_ERROR << endl;
            if(verbose)
            {
                cerr << error_line << endl
                     << "error: " << err_str[0] <<err_str[1]<< "' should be " << nTape << " characters" << endl;
            }
        case 14:
        cerr << SYNTAX_ERROR << endl;
            if(verbose)
            {
                cerr << error_line << endl
                     << "error: tape can be only moved to l,r,*"<< endl;
            }
        default:
            break;
    }
    cerr << end_line << endl;
    exit(error_code);
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
        Error({}, FILE_OPEN_ERROR);
    }
    else
    {
        string tmp;
        stringstream ss;
        vector<string> tokens;
        map<string, Action> actions;
        set<string> temp_set;
        string current_state = "";
        while(getline(f,tmp))
        {
            //可能会有行内注释
            reading_line = tmp;
            ClearComment(tmp);
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
                            Error({"'{'"}, CHAR_MISS);
                        if(value.back()!='}')
                            Error({"'}'"}, CHAR_MISS);
                        Spilt(value.substr(1, value.length() - 2), states, 'Q');
                        for(auto i=states.begin(); i!=states.end();i++)
                            transitions.insert(pair<string, map<string, Action>>(*i, actions));
                        break;
                    case 'S':
                        if(value[0] != '{')
                            Error({"'{'"}, CHAR_MISS);
                        if(value.back()!='}')
                            Error({"'}'"}, CHAR_MISS);
                        Spilt(value.substr(1, value.length() - 2), temp_set, 'S');
                        for (auto i = temp_set.begin(); i != temp_set.end();i++)
                            input_char.insert((*i)[0]);
                        temp_set.clear();
                        break;
                    case 'G':
                        if(value[0] != '{')
                            Error({"'{'"}, CHAR_MISS);
                        if(value.back()!='}')
                            Error({"'}'"}, CHAR_MISS);
                        Spilt(value.substr(1, value.length() - 2), temp_set, 'G');
                        for (auto i = temp_set.begin(); i != temp_set.end();i++)
                            tape_char.insert((*i)[0]);
                        temp_set.clear();
                        break;
                    case 'B':
                        if(value!="_")
                            Error({value}, INVALID_CHAR);
                        blank = '_';
                        break;
                    case 'F':
                        if(value[0] != '{')
                            Error({"'{'"}, CHAR_MISS);
                        if(value.back()!='}')
                            Error({"'}'"}, CHAR_MISS);
                        Spilt(value.substr(1, value.length() - 2), final_states, 'F');
                        for (auto it = final_states.begin();it!=final_states.end();it++)
                        {
                            auto find_it = states.find(*it);
                            if(find_it == states.end())
                                Error({*find_it, "final_state"}, STATE_MISS);
                        }
                        break;
                    case 'N':
                        if(!IsNum(value))
                            Error({value}, NOT_NUM);
                        ss << value;
                        ss >> nTape;
                        break;
                    default:
                        assert(tmp[1] == 'q' && tmp[2] == '0');
                        auto find_it = states.find(value);
                        if(find_it == states.end())
                            Error({value, "start_state"}, STATE_MISS);
                        start_state = value;
                        break;
                }
            }
            else//transition function
            {
                tokens.clear();
                TokenSpilt(tmp, tokens);
                if(tokens.size()!=TRANS_FUNC_PART)
                    Error({}, INCOMPLETE_FUNC);
                if(tokens[1].length()!=nTape)
                    Error({"<Scanned_symbol> '", tokens[1]}, CHAR_NUM_ERR);
                IsValid(tokens[1]);
                if(tokens[2].length()!=nTape)
                    Error({"<Print_symbol> '", tokens[2]}, CHAR_NUM_ERR);
                IsValid(tokens[2]);
                if(tokens[3].length()!=nTape)
                    Error({"<Move_tape> '", tokens[3]}, CHAR_NUM_ERR);
                Action a(tokens[2], tokens[3], tokens[4]);
                auto find_it = transitions.find("")
                find_it->second.insert(pair<string, Action>(tokens[1], a));
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
    //图灵机语法由7部分构成
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
        Error({}, INCOMPLETE_LINE);
    }

    while(!variable_exist||!equal_exist||!value_exist)
    {
        if(!variable_exist)
        {
            auto it = Def.find(variable);
            if(it == Def.end())//图灵机语法有且仅有7部分
                Error({variable},INVALID_CHAR);
            variable_exist = true;
        }
        else if(variable_exist && !equal_exist)
        {
            if(equal != "=")
                Error({equal},INVALID_CHAR);
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
    string temp = "";
    string i_str = "";
    for(auto i = val.begin(); i != val.end();++i,i_str.clear(),i_str.push_back(*i))
    {
        if(*i==',')
        {
            if(temp == "")
                Error({i_str}, INVALID_CHAR);
            if(type == 'S' )
                if(temp.length()>1)
                    Error({i_str,"Input symbol"}, NOT_CHAR);
            else if(type == 'G')
                if(temp.length()>1)
                    Error({i_str,"Alphabet"}, NOT_CHAR);
            words.insert(temp);
            temp = "";
        }
        else
        {
            if(!IsValid(*i,type))
                Error({i_str}, INVALID_CHAR);
            else
                temp = temp + *i;
        }
    }
    if(temp == "")
        Error({i_str}, INVALID_CHAR);
    if(type == 'S' )
        if(temp.length()>1)
            Error({i_str,"Input symbol"}, NOT_CHAR);
    else if(type == 'G')
        if(temp.length()>1)
            Error({i_str,"Alphabet"}, NOT_CHAR);
    words.insert(temp);//重复插入不会崩溃
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
        case 'S':
            if(ch>=32 && ch<=126 &&ch != ' ' && ch != ',' && ch != ';'&&ch != '{'&&ch!='}'&&ch != '*'&&ch != '_')
                return true;
            else
                return false;
            break;
        case 'G':case 'B'://symbol
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

void TuringMachine::IsValid(string str)
{
    string tmp = "";
    for(auto i = str.begin(); i != str.end();i++)
    {
        auto find_it = tape_char.find(*i);
        if(find_it==tape_char.end())
        {
            tmp = tmp + *i;
            Error({tmp}, INVALID_CHAR);
        }
    }
}
/**
 * @Description: whether the given string is valid
 * @param 
 *  ch: 
 * @return true/false
 */
bool TuringMachine::IsNum(string str)
{
    for(auto i=str.begin();i!=str.end();i++)
        if(*i>'9'||*i<'0')
            return false;
    return true;
}

/**
 * @Description: Spilt a line seperated by space
 * @param
 *  str: the given string
 *  tokens: store the tokens into the vector
 * @return void
 */
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

/**
 * @Description: run the turing machine
 * @param
 *  input: the input string
 * @return void
 */
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
    {
        auto it = input_char.find(*i);
        if(it == input_char.end)
        {
            string tmp = "";
            tmp = tmp + *i;
            int pos = input.find_first_of(*i);
            string head_pos = "       ";
            for (int i = 0; i < pos;i++)
                head_pos = head_pos + ' ';
            head_pos = head_pos + '^';
            Error({tmp,
                   input,
                   head_pos},
                  INVALID_INPUT);
        }
        tapes[0].push_back(*i);
    }
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
        if(trans_it == transitions.end())
            Error({current_state, "current_state"}, STATE_MISS);
        string current_symbol = "";
        for (int i = 0; i < nTape;i++)
        {
            char ch = *heads[i];
            if(ch == ' ')
                ch = blank;
            current_symbol = current_symbol + ch;
        }
        auto action_it = trans_it->second.find(current_symbol);//TODO check
        if(action_it == trans_it->second.end())
            Error({current_symbol, "current_symbol"}, STATUS_MISS);
        Action a = action_it->second;
        MoveWrite(a.direction, a.write);
        auto trans_it = states.find(a.next);//TODO: check
        if(trans_it == states.end())
            Error({a.next_state, "next_state"}, STATE_MISS);
        current_state = a.next;
    }
}

/**
 * @Description: write on the tape and move the header
 * @param 
 *  dir:move direction
 *  input: write input to the tapes
 * @return void
 */
void TuringMachine::MoveWrite(string dir, string input)//move all tape headers and write
{
    assert(dir.length() == nTape && ainput.length() == nTape);
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
                Error({}, ERR_DIR);
                break;
        }
    }
}