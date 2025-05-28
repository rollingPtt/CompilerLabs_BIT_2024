#include<bits/stdc++.h>
using namespace std;

// 分词
string tokenization(string text){
    string text_produced = "";
    for (char i: text){
        // 只要遇到了非字母、数字和下划线的符号，就在其前后添加space
        if (!isalnum(i) && i!='_' && i!=' ') {
            text_produced += " ";
            text_produced += i;
            text_produced += " ";
        }
        else text_produced += i;
    }
    return text_produced;
}

regex identifier("[a-zA-Z_][0-9a-zA-Z_]*");             // 06
regex number("[-+]?[0-9]+");                            // 07
regex sig("[*/%+-><&|^~!]|[><=!]=|\\{|}|&&|\\|\\|");
int classification(string str) {
    smatch match;
    if (str=="int")                                     return 1;
    if (str=="void")                                    return 2;
    if (str=="return")                                  return 3;
    if (str=="main")                                    return 4;
    if (str=="println_int")                             return 5;
    if ( regex_match(str, match, identifier) )          return 6;
    if ( regex_match(str, match, number) )              return 7;
    if (str=="=")                                       return 8;
    if (str==";")                                       return 9;
    if (str=="("||str==")")                             return 50;

    if ( regex_match(str, match, sig) ) {
        if (str=="~"||str=="!")                         return 11;
        if (str=="*"||str=="/"||str=="%")               return 12;
        if (str=="+"||str=="-")                         return 13;
        if (str==">"||str=="<"||str==">="||str=="<=")   return 14;
        if (str=="=="||str=="!=")                       return 15;
        if (str=="&")                                   return 16;
        if (str=="^")                                   return 17;
        if (str=="|")                                   return 18;
        if (str=="&&")                                  return 19;
        if (str=="||")                                  return 20;
        if (str=="{"||str=="}")                         return 31;
    }
    return -1;
}

// 传进来的参数已经分别在 eax 和 ebx 上
// 该函数结束后，结果保存在 eax 上
// int 用来区别 false 和 true，这里其实是堆屎山的做法了，不建议学我
// 你可以自定义一个函数，专门用来处理&&和||，当遇到的时候，就call这个函数
void func_cal(string c, int p) {
    if (c=="~") {
        printf("not eax\n");
        return;
    }
    if (c=="!") {
        printf("test eax, eax\nsetz al\nmovzx eax, al\n");
        return;
    }
    if (c=="+") {
        printf("add eax, ebx\n");
        return;
    }
    if (c=="-") {
        // 对于计算 -4，应该传入：(0, 4, -)，即默认被减数是 0
        printf("sub eax, ebx\n");
        return;
    }
    if (c=="*") {
        printf("imul eax, ebx\n");
        return;
    }
    if (c=="/") {
        printf("cdq\nidiv ebx\n");
        return;
    }
    if (c=="%") {
        printf("cdq\nidiv ebx\nmov eax, edx\n");
        return;
    }
    if (c=="<") {
        printf("cmp eax, ebx\nsetl al\nmovzx eax, al\n");
        return;
    }
    if (c=="<=") {
        printf("cmp eax, ebx\nsetle al\nmovzx eax, al\n");
        return;
    }
    if (c==">") {
        printf("cmp eax, ebx\nsetg al\nmovzx eax, al\n");
        return;
    }
    if (c==">=") {
        printf("cmp eax, ebx\nsetge al\nmovzx eax, al\n");
        return;
    }
    if (c=="==") {
        printf("cmp eax, ebx\nsete al\nmovzx eax, al\n");
        return;
    }
    if (c=="!=") {
        printf("cmp eax, ebx\nsetne al\nmovzx eax, al\n");
        return;
    }
    if (c=="&") {
        printf("and eax, ebx\n");
        return;
    }
    if (c=="|") {
        printf("or eax, ebx\n");
        return;
    }
    if (c=="^") {
        printf("xor eax, ebx\n");
        return;
    }
    if (c=="&&") {
        printf("test eax, eax\njz false%d\ntest ebx, ebx\njz false%d\nmov eax, 1\njmp done%d\nfalse%d:\nfalse%d:\nmov eax, 0\ndone%d:\n", 2*p-1, 2*p, 200+p, 2*p-1, 2*p, 200+p);
        return;
    }
    if (c=="||") {
        printf("test eax, eax\njnz true%d\ntest ebx, ebx\njnz true%d\nmov eax, 0\njmp done%d\ntrue%d:\ntrue%d:\nmov eax, 1\ndone%d:\n", 2*p-1, 2*p, 200+p, 2*p-1, 2*p, 200+p);
        return;
    }
    return;
}

int string_to_int(string str) {
    stringstream number(str);
    int num = 0;
    number >> num;
    return num;
}
