#include<bits/stdc++.h>
using namespace std;
void print_begin(){
    printf(".intel_syntax noprefix\n");
    printf(".global main\n");
    printf(".extern printf\n");
    printf(".data\n");
    printf("format_str:\n");
    cout << "  .asciz \"%d\\n\"" << endl;
    printf(".text\n");
    printf("main:\n");
    printf("  push ebp\n");
    printf("  mov ebp, esp\n");
    printf("  sub esp, 0x100\n");
    return;
}

void print_end(){
    printf("leave\nret\n");
    return;
}

// 一个问题，输入int a;，会分词为int  a ;，虽然不影响结果，但不美观;
string tokenization(string text){
    int classification(string);
    string text_produced = "";
    string str = "";
    for (char i: text){
        if (str=="" && i==' ') continue;
        // cout << "str: " << str << endl;
        // cout << "i: " << i << endl;
        // cout << "classification(str + i): " << classification(str + i) << endl;
        if (classification(str + i)==-1) {
            // 判断是符号数还是运算符
            string temp_s = "";
            if (text_produced!="") temp_s += text_produced[text_produced.length()-2];
            if (classification(str)==3 && (str[0]=='+'||str[0]=='-') 
            && ((classification(temp_s))==3)||(classification(temp_s)==2)) {
                int k = 0;
                for (char j: str) {
                    text_produced += j;
                    if (k==0) { k=1; text_produced+=" "; }
                }
                text_produced += " ";
                str = i;
            }
            else {
                text_produced += (str+" ");
                str = i;
            }
        }
        else str += i;
    }
    text_produced += str;
    return text_produced;
}

regex identifier("[a-zA-Z_][0-9a-zA-Z_]*");             // 02
regex number("[-+]?[0-9]+");                            // 03
regex sig("[*/%+-><&|^]|[><=!]=|\\{|}");
int classification(string str) {
    smatch match;
    if (str=="int")                                     return 0;
    if (str=="return")                                  return 1;
    if (str=="main")                                    return 7;
    if (str=="println_int")                             return 5;
    if ( regex_match(str, match, identifier) )          return 2;
    if ( regex_match(str, match, number) )              return 3;
    if (str=="=")                                       return 4;
    if (str==";")                                       return 6;
    if (str=="("||str==")")                             return 50;

    if ( regex_match(str, match, sig) ) {
        if (str=="*"||str=="/"||str=="%")               return 11;
        if (str=="+"||str=="-")                         return 12;
        if (str==">"||str=="<"||str==">="||str=="<=")   return 21;
        if (str=="=="||str=="!=")                       return 22;
        if (str=="&")                                   return 23;
        if (str=="^")                                   return 24;
        if (str=="|")                                   return 25;
        if (str=="{"||str=="}")                         return 31;
    }
    return -1;
}

int func_cal(int a, int b, string c) {
    if (c=="+") {
        return a+b;
    }
    else if (c=="-") {
        return a-b;
    }
    else if (c=="*") {
        return a*b;
    }
    else if (c=="/") {
        return a/b;
    }
    else if (c=="%") {
        return a%b;
    }
    else if (c=="<") {
        return a<b;
    }
    else if (c=="<=") {
        return a<=b;
    }
    else if (c==">") {
        return a>b;
    }
    else if (c==">=") {
        return a>=b;
    }
    else if (c=="==") {
        return a==b;
    }
    else if (c=="!=") {
        return a!=b;
    }
    else if (c=="&") {
        return a&b;
    }
    else if (c=="|") {
        return a|b;
    }
    else if (c=="^") {
        return a^b;
    }
    return 0;
}

int string_to_int(string str) {
    stringstream number(str);
    int num = 0;
    number >> num;
    return num;
}

void func_return(int num) {
    printf("mov eax, %d\n", num);
    return;
}

void print_int(int num) {
    printf("mov eax, %d\n", num);
    printf("push eax\n");
    printf("push offset format_str\n");
    printf("call printf\n");
    printf("add esp, 8\n");
    // cout << "print_int: " << num << endl;
    return;
}
