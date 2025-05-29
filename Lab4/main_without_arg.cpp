#include "F.h"
// 变量的结构体
typedef struct variable{
    string name;    // 函数名 或 变量名
    int id;         
    int nature;     // 其值可能是0或1. 对于函数, 0 表示返回 void, 1 表示返回 int; 对于变量, 0 表示局部变量, 1 表示参数.
} vr;

vr vf[100];             // 函数存储器
vr va[100];             // 变量存储器
string out_block[100];  // 输出
int count_of_vf = 0;    // 统计当前的函数数
int count_of_va = 0;    // 统计当前的变量数, 等于 count_of_va_positive + count_of_va_negative
int count_of_va_positive = 0;   // 传参的个数
int count_of_va_negative = 0;   // 局部变量的个数
int jmpid = 0;                  // 给不同的 jump 进行标号
stack<int> if_jmp_ids;          // 存储历史需要 jump 的 id
stack<int> continue_jmp_ids;    // 维护一个栈，存储 continue 相关的 jump 入口
stack<int> break_jmp_ids;       // 维护一个栈，存储 break 相关的 jump 入口
stack<int> return_jmp_ids;      // 维护一个栈，存储 return 相关的 jump 入口

// ----------------------------------------- 下面这行改掉 -----------------------------------------
int main() {
// int main(int argc, char *argv[]) {
    int find_vr(string);
    void return_void();
    void return_int();
    void print_int();
    void onesentence(const vector<string>&);
    void expression(const vector<string>&, int, int);
    void pass_value(int);
    void print_allbegin();

    // ----------------------------------------- 下面这行注释去掉 -----------------------------------------
    // ifstream input_file(argv[1]);

    string s0;
    vector<string> reset;
    // 读入
    // ----------------------------------------- 下面这行改掉 -----------------------------------------
    while ( getline(cin, s0) ) {
    // while ( getline(input_file, s0) ) {
        if (s0 == "") continue;
        // ----------------------------------------- 下面这行去掉 -----------------------------------------
        if (s0 == "+++") break;
        // 添加空格
        s0 = tokenization(s0);
        // 分词，结果存储在reset中
        stringstream sh;
        sh << s0;
        string lst = "";
        // 注意对双节符号的甄别
        while (sh >> s0) {
            if (lst=="") reset.push_back(s0);
            else if (((lst=="!"||lst=="="||lst==">"||lst=="<")&&s0=="=") || (lst=="&"&&s0=="&") || (lst=="|"&&s0=="|")) {
                reset[reset.size()-1]+=s0;
            }
            else {
                reset.push_back(s0);
            };
            lst = s0;
        }
    }

    // ----------------------------------------- 下面这行注释去掉 -----------------------------------------
    // input_file.close();
    
    // 记录有效的右大括号出现的位置
    int j = 0;
    vector<int> funcdot;
    funcdot.push_back(0);
    int brace3lmr = 0;      // brace3lmr = 左大括号数量 - 右大括号数量
    for (string s: reset){
        if (s=="{") brace3lmr++;
        if (s=="}") brace3lmr--;
        if (brace3lmr==0 && s=="}") funcdot.push_back(j+1);
        j++;
    }
    count_of_vf = funcdot.size()-1;
    for (int i=1; i<count_of_vf+1; i++) {
        // 第i个函数体
        // 保存函数信息
        vf[i-1].name = reset[funcdot[i-1]+1];
        if (reset[funcdot[i-1]]=="void") {
            vf[i-1].nature = 0;
        }
        else 
            vf[i-1].nature = 1;
    }
    print_allbegin();
    
    for (int i=1; i<funcdot.size(); i++) {
        // 翻译第 i 个函数体
        // 输出函数名
        cout << vf[i-1].name << ":" << endl;

        printf("push ebp\nmov ebp, esp\nsub esp, 0x100\n"); 
        
        // 处理传递的参数
        count_of_va = 0;    // 统计当前的变量数
        j = funcdot[i-1]+3;
        while (reset[j]!=")") {
            if (reset[j]=="int") {
                va[count_of_va].name = reset[j+1];
                va[count_of_va].nature = 1;
                va[count_of_va].id = count_of_va_positive;
                count_of_va_positive++;
                count_of_va++;
            }
            j++;
        } j+=2;
        
        vector<string> one_sentence;
        for (;j<funcdot[i]-2;) {
            one_sentence.clear();
            // 开始处理语句
            // 我们认为一句话是：或者它以无前置 { 的 ; 结尾，或者它以平衡的 } 结尾
            brace3lmr = 0;
            while (1) {
                if (reset[j]=="{") brace3lmr++;
                if (reset[j]=="}") brace3lmr--;
                if ( brace3lmr==0 && (reset[j]=="}" || reset[j]==";") ) {
                    one_sentence.push_back(reset[j]);
                    break;
                }
                else {
                    one_sentence.push_back(reset[j]);
                    j++;
                }
            } j++;

            // 如果 if 带 else，则把 else 一起送进去
            if (one_sentence[0]=="if" && j+2<funcdot[i] && reset[j]=="else") {
                brace3lmr = 0;
                while (1) {
                    if (reset[j]=="{") brace3lmr++;
                    if (reset[j]=="}") brace3lmr--;
                    if ( brace3lmr==0 && reset[j]=="}" ) {
                        one_sentence.push_back(reset[j]);
                        break;
                    }
                    else {
                        one_sentence.push_back(reset[j]);
                        j++;
                    }
                } j++;
            }
            onesentence(one_sentence);
        }
        // 退出函数
        if (vf[i-1].nature==0) {
            return_void();
        }
        else {
            return_int();
        }
        // 清理信息，为下一个函数做准备
        count_of_va = 0;
        count_of_va_negative = 0;
        count_of_va_positive = 0;
    }
    return 0;
}

// find the variation, return the allid of the variation
int find_vr(string s) {
    for (int i=0; i<count_of_va; i++) {
        if (s == va[i].name) {
            return i;
        }
    }
    return -1;
}

// 将 return 改为对应的汇编输出
void return_void() {
    while (return_jmp_ids.size()>0) {
        int tempid = return_jmp_ids.top();
        return_jmp_ids.pop();
        printf("jump%d:\n", tempid);
    }
    printf("leave\nret\n");
    return;
}
void return_int() {
    while (return_jmp_ids.size()>0) {
        int tempid = return_jmp_ids.top();
        return_jmp_ids.pop();
        printf("jump%d:\n", tempid);
    }
    printf("pop eax\nleave\nret\n");
    return;
}

// 将 println_int 改为对应的汇编输出
// 在调用该函数之前，要输出的内容已经存储在栈里
void print_int() {
    printf("push offset format_str\n");
    printf("call printf\n");
    printf("add esp, 8\n");
    return;
}

// 将 向变量赋值 改为对应的汇编输出
// 在调用该函数之前，要传递的内容已经存储在栈里
void pass_value(int ii) {
    printf("pop eax\n");
    int ij = va[ii].id;
    if (va[ii].nature==0) {
        printf("mov DWORD PTR [ebp-%d], eax\n", (ij+1)*4);
    }
    else {
        printf("mov DWORD PTR [ebp+%d], eax\n", (ij+2)*4);
    }
    return;
}

// one_sentence 是个句子
void onesentence(const vector<string>& one_sentence) {

    // // test begin
    // cout << "-----  Hey, now you are in the func onesentence!  -----" << endl;
    // cout << "The sentence you've passed in: " << endl;
    // for (string s: one_sentence){
    //     cout << s << " ";
    // }
    // cout << endl;
    // cout << "-----  Processing:  -----" << endl;
    // // test end

    void expression(const vector<string>&, int, int);
    // 如果遇到了 int，表明变量声明，可能会带初始化
    if (one_sentence[0]=="int") {
        for (int k=1; k<one_sentence.size()-1; k++) {
            if ( classification(one_sentence[k])==61 && (one_sentence[k-1]==","||one_sentence[k-1]=="int") ) {
                va[count_of_va].name = one_sentence[k];
                va[count_of_va].id = count_of_va_negative;
                va[count_of_va].nature = 0;
                // 如果字符的下一位是 =，说明带初始化，则找到下一个逗号，这个逗号前面的括号数量平衡，并把介于等号到逗号之间的内容作为表达式拿去处理
                if ( (k+1<one_sentence.size()-1) && (one_sentence[k+1]=="=") ) {
                    int p = k+2;
                    int brace1lmr=0;     // brace1lmr = 左小括号数量 - 右小括号数量
                    while (p<one_sentence.size()-1) {
                        if (one_sentence[p]=="," && brace1lmr==0) break;
                        if (one_sentence[p]=="(") brace1lmr++;
                        else if (one_sentence[p]==")") brace1lmr--;
                        p++;
                    }
                    expression(one_sentence, k+2, p-1);
                    pass_value(count_of_va);
                    k+=2;
                }
                else {
                    // 没有初始化
                    printf("push 0\n");
                    pass_value(count_of_va);
                }
                count_of_va_negative++;
                count_of_va++;
            }
        }
        return;
    }
            
    // 如果遇到了 println_int，表明输出
    if (one_sentence[0]=="println_int") {
        expression(one_sentence, 2, one_sentence.size()-3);
        print_int();
        return;
    }

    // 如果遇到了 if 或 if-else，表明条件判断
    if (one_sentence[0]=="if") {
        // 处理条件判断
        int j = 2;
        while ( one_sentence[j]!="{" ) {
            j++;
        }
        expression(one_sentence, 2, j-2);
        printf("pop eax\ncmp eax, 0\nje jump%d\n", jmpid);
        if_jmp_ids.push(jmpid);
        jmpid++;

        vector<string> sub_sentence;
        // 定位第一个左大括号后的第一个单词
        j+=1;
        // 定位 if 的内容
        int brace3lmr = 1, ifotlse = j;
        while (1) {
            if (one_sentence[ifotlse]=="{") brace3lmr++;
            if (one_sentence[ifotlse]=="}") brace3lmr--;
            if (one_sentence[ifotlse]=="}" && brace3lmr==0) {
                break;
            }
            ifotlse++;
        }   // ifotlse 将会定位在 if 右大括号的位置

        // 逐句翻译 if 内部的句子
        for (;j<ifotlse;) {
            sub_sentence.clear();
            // 逐句处理。我们认为一句话是：或者它以无前置 { 的 ; 结尾，或者它以平衡的 } 结尾
            int brace3lmr = 0;
            while (1) {
                if (one_sentence[j]=="{") brace3lmr++;
                if (one_sentence[j]=="}") brace3lmr--;
                if ( brace3lmr==0 && one_sentence[j]==";" ) {
                    sub_sentence.push_back(one_sentence[j]);
                    // cout << "press ;" << endl;
                    break;
                }
                else if ( brace3lmr==0 && one_sentence[j]=="}" ) {
                    sub_sentence.push_back(one_sentence[j]);
                    // cout << "press }" << endl;
                    break;
                }
                else {
                    sub_sentence.push_back(one_sentence[j]);
                    j++;
                }
            } j++;

            // 如果是 continue，当场处理
            if ( sub_sentence[0]=="continue" ) {
                int tempid = continue_jmp_ids.top();
                continue_jmp_ids.pop();
                printf("jmp jump%d\n", tempid);
                continue;
            }
            // 如果是 break，当场处理
            if ( sub_sentence[0]=="break" ) {
                break_jmp_ids.push(jmpid);
                printf("jmp jump%d\n", jmpid);
                jmpid++;
                continue;
            }
            // 如果是 return，把返回值入栈，然后设置 jump，没有返回值就只设置 jump
            if ( sub_sentence[0]=="return" ) {
                expression(sub_sentence, 1, sub_sentence.size()-2);
                return_jmp_ids.push(jmpid);
                printf("jmp jump%d\n", jmpid);
                jmpid++;
                continue;
            }
            // 如果是 if 带 else 的，要把 else 一起进入
            if (sub_sentence[0]=="if" && j+1<ifotlse && one_sentence[j]=="else") {
                brace3lmr = 0;
                while (1) {
                    if (one_sentence[j]=="{") brace3lmr++;
                    if (one_sentence[j]=="}") brace3lmr--;
                    if ( brace3lmr==0 && one_sentence[j]=="}" ) {
                        sub_sentence.push_back(one_sentence[j]);
                        break;
                    }
                    else {
                        sub_sentence.push_back(one_sentence[j]);
                        j++;
                    }
                } j++;
            }
            // 直接调用 onesentence 函数
            onesentence(sub_sentence);
        }
        
        // 先判断有没有 else，如果有，就逐句翻译 else 内部的句子
        // 有 else 的判断条件是：ifotlse 指向的位置后面还有单词
        if ( ifotlse<one_sentence.size()-1 ) {
            // 无条件结束 if 块
            int tempjumpid = jmpid;
            printf("jmp jump%d\n", tempjumpid);
            // 设置 else 的 jump 入口
            tempjumpid = if_jmp_ids.top();
            if_jmp_ids.pop();
            printf("jump%d:\n", tempjumpid);
            if_jmp_ids.push(jmpid);
            jmpid++;
            
            // 翻译 else 内部的句子
            for (j=ifotlse+3; j<one_sentence.size()-2; ) {
                sub_sentence.clear();
                // 逐句处理。我们认为一句话是：或者它以无前置 { 的 ; 结尾，或者它以平衡的 } 结尾
                int brace3lmr = 0;
                while (1) {
                    if (one_sentence[j]=="{") brace3lmr++;
                    if (one_sentence[j]=="}") brace3lmr--;
                    if ( brace3lmr==0 && one_sentence[j]==";" ) {
                        sub_sentence.push_back(one_sentence[j]);
                        break;
                    }
                    else if ( brace3lmr==0 && one_sentence[j]=="}" ) {
                        sub_sentence.push_back(one_sentence[j]);
                        break;
                    }
                    else {
                        sub_sentence.push_back(one_sentence[j]);
                        j++;
                    }   
                } j++;

                // 如果是 continue，当场处理
                if ( sub_sentence[0]=="continue" ) {
                    int tempid = continue_jmp_ids.top();
                    continue_jmp_ids.pop();
                    printf("jmp jump%d\n", tempid);
                    continue;
                }
                // 如果是 break，当场处理
                if ( sub_sentence[0]=="break" ) {
                    break_jmp_ids.push(jmpid);
                    printf("jmp jump%d\n", jmpid);
                    jmpid++;
                    continue;
                }
                // 如果是 return，把返回值入栈，然后设置 jump，没有返回值就只设置 jump
                if ( sub_sentence[0]=="return" ) {
                    expression(sub_sentence, 1, sub_sentence.size()-2);
                    return_jmp_ids.push(jmpid);
                    printf("jmp jump%d\n", jmpid);
                    jmpid++;
                    continue;
                }
                // 如果是 if 带 else 的，要把 else 一起进入
                if (sub_sentence[0]=="if" && j+3<one_sentence.size() && one_sentence[j]=="else") {
                    brace3lmr = 0;
                    while (1) {
                        if (one_sentence[j]=="{") brace3lmr++;
                        if (one_sentence[j]=="}") brace3lmr--;
                        if ( brace3lmr==0 && one_sentence[j]=="}" ) {
                            sub_sentence.push_back(one_sentence[j]);
                            break;
                        }
                        else {
                            sub_sentence.push_back(one_sentence[j]);
                            j++;
                        }
                    } j++;
                }
                // 直接调用 onesentence 函数
                onesentence(sub_sentence);
            }

            // 设置 if 块结束的入口
            tempjumpid = if_jmp_ids.top();
            if_jmp_ids.pop();
            printf("jump%d:\n", tempjumpid);
        }
        // 没有 else，则直接结束即可
        else {
            int tempjumpid = if_jmp_ids.top();
            if_jmp_ids.pop();
            printf("jump%d:\n", tempjumpid);
        }
        return;
    }

    // 如果遇到了 while，表明 while 循环
    if (one_sentence[0]=="while") {
        // ----------------------------------------- unfinished -----------------------------------------
        // 对 return 的处理

        // while 开头位置
        int whilest = jmpid;
        printf("jump%d:\n", whilest);
        jmpid++;
        
        // 提前扫描一遍内容，检查属于该 while 的 continue 的数量
        // 注意，子 while 的 continue 属于它自己，既不属于父 while 也不属于父 if
        // 子 if 的 continue 属于所有父亲中的第一个父 while
        // break 的父子从属关系和 continue 一样
        int j = 2;
        while ( one_sentence[j]!="{" ) {
            j++;
        }
        vector<string> sub_sentence;
        // 在 while 的内部寻找 continue，只要找到了 continue，就判断该 continue 是不是属于自己，如果是，则进入 continue_jmp_ids
        stack<string> titles;
        string title = "while";
        int count_of_whiles = 0;
        for (; j<one_sentence.size()-1; j++) {
            if ( one_sentence[j]=="while" ) {
                title = "while";
                count_of_whiles++;
                continue;
            }
            if ( one_sentence[j]=="if" || one_sentence[j]=="else") {
                title = "if";
                continue;
            }
            if ( one_sentence[j]=="{" ) {
                titles.push(title);
                continue;
            }
            if ( one_sentence[j]=="}" ) {
                if ( titles.top()=="while" ) count_of_whiles--;
                titles.pop();
                continue;
            }
            if ( one_sentence[j]=="continue" ) {
                if ( count_of_whiles==0 ) {
                    continue_jmp_ids.push(jmpid);
                    printf("jump%d:\n", jmpid);
                    jmpid++;
                }
                continue;
            }
        }

        // 处理条件判断
        j = 2;
        while ( one_sentence[j]!="{" ) {
            j++;
        }
        expression(one_sentence, 2, j-2);
        printf("pop eax\ncmp eax, 0\nje jump%d\n", jmpid);
        if_jmp_ids.push(jmpid);
        jmpid++;

        // 定位第一个左大括号后的第一个单词
        j+=1;
        // 先划分内部的句子，然后一句一句地调用 onesentence 函数处理
        for (;j<one_sentence.size()-1;) {
            sub_sentence.clear();
            // 逐句处理。我们认为一句话是：或者它以无前置 { 的 ; 结尾，或者它以平衡的 } 结尾
            int brace3lmr = 0;
            while (1) {
                if (one_sentence[j]=="{") brace3lmr++;
                if (one_sentence[j]=="}") brace3lmr--;
                if ( brace3lmr==0 && one_sentence[j]==";" ) {
                    sub_sentence.push_back(one_sentence[j]);
                    // cout << "press ;" << endl;
                    break;
                }
                else if ( brace3lmr==0 && one_sentence[j]=="}" ) {
                    sub_sentence.push_back(one_sentence[j]);
                    // cout << "press }" << endl;
                    break;
                }
                else {
                    sub_sentence.push_back(one_sentence[j]);
                    j++;
                }
            } j++;
            // 如果是 continue 和 break，当场处理
            if ( sub_sentence.size()==2 && (sub_sentence[0]=="continue" || sub_sentence[0]=="break") ) {
                if ( sub_sentence[0]=="continue" ) {
                    int tempid = continue_jmp_ids.top();
                    continue_jmp_ids.pop();
                    printf("jmp jump%d\n", tempid);
                }
                else {      // 是 break
                    break_jmp_ids.push(jmpid);
                    printf("jmp jump%d\n", jmpid);
                    jmpid++;
                }
            }
            // 既不是 continue 和 break，则调用 onesentence 函数
            else {
                onesentence(sub_sentence);
            }
        }

        printf("jmp jump%d\n", whilest);

        int tempjumpid;
        // break 相关的 jump
        while ( break_jmp_ids.size()>0 ) {
            tempjumpid = break_jmp_ids.top();
            break_jmp_ids.pop();
            printf("jump%d:\n", tempjumpid);
        }

        tempjumpid = if_jmp_ids.top();
        if_jmp_ids.pop();
        printf("jump%d:\n", tempjumpid);
        return;
    }

    // 如果遇到了 return，不处理
    if (one_sentence[0]=="return") { return; }
    // 不是以上的情况，此时可能是直接调用void函数，或者是赋值语句
    // 第一种是直接调用 void 函数
    if (one_sentence[1]=="(") {
        expression(one_sentence, 0, one_sentence.size()-2);
        return;
    }
    // 第二种是赋值语句，左边是一个变量，右边是表达式
    else {
        // 先计算右边的表达式
        expression(one_sentence, 2, one_sentence.size()-2);
        // 再向左边赋值
        pass_value(find_vr(one_sentence[0]));
        return;
    }

    // cout << "----- This part is over. -----" << endl;

    return;
}

// 处理表达式，可能是常数、变量、调用函数或计算式，它的本质是一个值
// 该函数需要的内容要么提前存储在栈中，要么存储在变量里
// 该函数结束后得到的结果存放在栈里
void expression(const vector<string>& exp, int st, int ed) {
    int find_vr(string);
    // // test begin
    // cout << "-----  Hey, now you are in the func expression!  -----" << endl;
    // cout << "The sentence you've passed in: " << endl;
    // for (int l=st; l<=ed; l++){
    //     cout << exp[l] << " ";
    // }
    // cout << endl;
    // cout << "-----  Processing:  -----" << endl;
    // // test end
    
    if (st==ed) {
        // 常数
        if (classification(exp[st])==71) {
            cout << "push " << string_to_int(exp[st]) << endl;
        }
        // 变量
        else {
            int ii = find_vr(exp[st]);
            if (va[ii].nature == 0) {
                printf("mov eax, DWORD PTR [ebp-%d]\npush eax\n", (va[ii].id+1)*4);
            }
            else {
                printf("mov eax, DWORD PTR [ebp+%d]\npush eax\n", (va[ii].id+2)*4);
            }
        }
    }
    // 表达式
    else {
        // 先验工作：检查是否为仅调用函数，这里有点堆屎山
        int forno=1, forn=0;
        if (classification(exp[st])==61 && exp[st+1]=="(") {
            for (forn=st+2; forn<=ed; forn++) {
                if (exp[forn]=="(") forno++;
                else if (exp[forn]==")") forno--;
                if (forno==0) break;
            }
        }
        // 如果是仅调用函数
        // 注意对仅调用函数的判断，判断过程是从最左扫描到最右，要求第一个单词是函数名，第二个单词是(，后续所有位置中有且仅有最后一个位置达到了左右括号数量平衡
        if (forno==0 && forn==ed) {
            // 记录各段逗号出现的位置
            // 注意 f(a,f(b,c)) 这里有两个逗号，不能简单地按照逗号分割，否则就会变成三段分别是 a 、 f(b 、 c)，显然有错
            // 一个可能的做法是，遇到逗号后，计算逗号左侧的括号数量，如果 左括号数量-右括号数量=1，说明这是一个可以用于分割的逗号
            vector<int> dots;
            dots.push_back(st+2);
            int brace1lmr=0;     // brace1lmr = 左括号数量 - 右括号数量
            for (int j=st; j<ed; j++){
                if (exp[j]=="(") brace1lmr++;
                if (exp[j]==")") brace1lmr--;
                if (exp[j]=="," && brace1lmr==1) dots.push_back(j+1);
            }
            dots.push_back(ed+1);
            for (int j=dots.size()-1; j>=1; j--) {
                // 如果逗号之间只有一个单词，说明是直接参数传递
                if (dots[j]-dots[j-1]==2) {
                    // 可能是数字或者变量
                    if (classification(exp[dots[j-1]])==71) printf("mov eax, %d\npush eax\n", string_to_int(exp[dots[j-1]]));
                    else {
                        int ii = find_vr(exp[dots[j-1]]);
                        if (va[ii].nature==0) printf("mov eax, DWORD PTR [ebp-%d]\npush eax\n", (va[ii].id+1)*4);
                        else                  printf("mov eax, DWORD PTR [ebp+%d]\npush eax\n", (va[ii].id+2)*4);
                    }
                }
                // 如果长度大于1，说明涉及到表达式，递归调用自己
                else {
                    expression(exp, dots[j-1], dots[j]-2);
                }
            }
            // 调用函数
            printf("call ");
            cout << exp[st] << endl; 
            printf("add esp, %d\n", 4*(dots.size()-1));
            for (int i=0; i<count_of_vf; i++) {
                if (exp[st] == vf[i].name) {
                    if (vf[i].nature==1) {
                        printf("push eax\n");
                    }
                }
            }
        }
        // 如果是涉及运算
        else {
            // 单目运算的处理类似负号，可以将其与后面紧跟的内容看为一个整体，优先级最高
            // 双目运算按照优先级入栈或计算
            // 对于函数，本质上是一个数值，其应当与数字或变量在同一个栈内
            int i = 0;
            stack<int> da;
            stack<string> op;
            for (int j=st; j<=ed; j++) {
                // 如果遇到了数字、变量或调用函数，直接入栈
                if (classification(exp[j])==71) {        // 是数字
                    da.push(0);
                    printf("mov eax, %d\npush eax\n", string_to_int(exp[j]));
                }
                else if (classification(exp[j])==61) {  // 是标识符
                    if (find_vr(exp[j])!=-1) {          // 是变量
                        da.push(0);
                        int ii = find_vr(exp[j]);
                        if (va[ii].nature==0) printf("mov eax, DWORD PTR [ebp-%d]\npush eax\n", (va[ii].id+1)*4);
                        else                  printf("mov eax, DWORD PTR [ebp+%d]\npush eax\n", (va[ii].id+2)*4);
                    }
                    else {                              // 是调用函数, 直接处理后把内容进入栈
                        // 找到它后面对应的括号位置 
                        int comp = 1, start=j;
                        j+=2;
                        while (comp!=0) {
                            if (exp[j]=="(") comp++;
                            if (exp[j]==")") comp--;
                            j++;
                        } j--;
                        expression(exp, start, j);
                        da.push(0);
                    }
                }
                // 如果遇到了符号
                else {
                    // 对于双目运算符，直到栈内运算符的优先级比它低才能入栈
                    // 对于单目运算符，存在两个问题
                    // 第一个问题是如何区别出单目运算符，我的方法是：对于~和!可以直接区分(我的代码不存在误识别!=的问题)，最麻烦的是-，需要检查它的前一个单词，如果是数字、变量或调用函数或右括号，那就是减号；如果前一个没有单词或前一个是符号，那就是负号
                    // 第二个问题是区别出单目运算符后怎么处理，我的方法是：将该符号与后面一个单词组合为一个新的单词，这个新的单词作为一个整体进入da栈
                    if (exp[j]=="(") {
                        op.push(exp[j]);
                    }
                    // 如果是右括号
                    else if (exp[j]==")") {
                        // 一直计算直到遇到左括号
                        while (op.top()!="(") {
                            printf("pop ebx\npop eax\n");
                            string c = op.top(); op.pop();
                            func_cal(c, jmpid);
                            jmpid+=3;
                            da.pop();
                            // 运算结果存入栈
                            printf("push eax\n");
                        }
                        op.pop();
                    }
                    // 如果是单目运算符
                    else if (exp[j]=="~" || exp[j]=="!" || (exp[j]=="-" && j==st) || (exp[j]=="-" && exp[j-1]!=")" && classification(exp[j-1])!=61 && classification(exp[j-1])!=71)) {
                        // 继续向下读，直到读到其他字符（即数字、变量或函数）为止
                        int start = j;
                        while (exp[j]=="~" || exp[j]=="!" || exp[j]=="-") {
                            j++;
                        }
                        int end = j;
                        if (classification(exp[j])==71) printf("mov eax, %d\n", string_to_int(exp[j]));
                        else if (find_vr(exp[j])!=-1) {
                            int ii = find_vr(exp[j]);
                            if (va[ii].nature==0)      printf("mov eax, DWORD PTR [ebp-%d]\n", (va[ii].id+1)*4);
                            else                       printf("mov eax, DWORD PTR [ebp+%d]\n", (va[ii].id+2)*4);
                        }
                        else {
                            // 对函数的处理
                            int comp = 1;
                            j+=2;
                            while (comp!=0) {
                                if (exp[j]=="(") comp++;
                                if (exp[j]==")") comp--;
                                j++;
                            } j--;
                            expression(exp, end, j);
                            printf("pop eax\n");
                        }
                        for (int k=end-1; k>=start; k--) {
                            if (exp[k]=="-") {
                                printf("push eax\npop ebx\nmov eax, 0\n");
                                func_cal("-", 0);
                            }
                            else func_cal(exp[k], 0);
                        }
                        // 运算结果存入栈
                        da.push(0);
                        printf("push eax\n");
                    }
                    // 如果是双目运算符
                    else {
                        while (op.size()>0 && ( classification(op.top()) <= classification(exp[j]) )) {
                            printf("pop ebx\npop eax\n");
                            string c = op.top(); op.pop();
                            func_cal(c, jmpid);
                            jmpid+=3;
                            da.pop();
                            // 运算结果存入栈
                            printf("push eax\n");
                        }
                        op.push(exp[j]);
                    }
                }
            }
            // 最后还需要清理栈中的内容
            while (op.size()>0) {
                printf("pop ebx\npop eax\n");
                string c = op.top(); op.pop();
                func_cal(c, jmpid);
                jmpid+=3;
                da.pop();
                // 运算结果存入栈
                printf("push eax\n");
            }
        }
    }
    // cout << "----- This part is over. -----" << endl;
    return;
}

// 汇编输出的开头内容
void print_allbegin(){
    printf(".intel_syntax noprefix\n");
    for (int i=0; i<count_of_vf; i++) {
        printf(".global ");
        cout << vf[i].name << endl;
    }
    printf(".extern printf\n");
    printf(".data\n");
    printf("format_str:\n");
    cout << ".asciz \"%d\\n\"" << endl;
    printf(".text\n");
    return;
}
