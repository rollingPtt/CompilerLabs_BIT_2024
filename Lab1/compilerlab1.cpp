#include<bits/stdc++.h>
using namespace std;
typedef struct variable{
    char name;
    int id;
    int value;
} vr;
// 存储器
vr v[52];
int count_of_vr = 0; // 统计当前的变量数

// ------------------------------ 使用命令行参数 ------------------------------
// int main(int argc, char *argv[]) {
int main() {
    // 函数声明
    void func_declare(string);
    void func_mov(int, vr);
    void func_mov(int, int);
    void func_expression(vector<string>);
    int string_to_int(string);
    int estimate_type(string);
    int find_vr(char);
    void func_return(int);

    // ------------------------------ 下一行的注释去掉 ------------------------------
    // ifstream input_file(argv[1]);
    
    string s0;
    vector<string> reset;
    // 读入
    // ------------------------------ 下一行的 cin 改为 input_file ------------------------------
    while ( getline(cin, s0) ) {
        if (s0 == "") continue;
        // ------------------------------ 删掉下面一行 ------------------------------
        if (s0 == "+++") break;
        // 分词，结果存储在reset中
        stringstream sh;
        sh << s0;
        while (sh >> s0)
            reset.push_back(s0);
    }
    
    int j = 0;
    while ( j < reset.size() ) {
        vector<string> res;
        while (reset[j]!=";") {
            res.push_back(reset[j]);
            j++;
        } j++;
        res.push_back(";");
        if (res[0]==";") continue;

        // 执行相应的函数
        // 如果遇到 int，表明变量声明
        if ( res[0] == "int" ) {
            func_declare(res[1]);
            continue;
        }
        // 如果遇到 return，表明返回值
        if ( res[0] == "return" ) {
            int id = find_vr(res[1][0]);
            func_return(id);
            return 0;
        }
        // 最简单的赋值语句：分为直接赋数字和直接赋变量的值
        if ( res[3] == ";" ) {
            if ( estimate_type(res[2])==3 ) {
                int num = string_to_int(res[2]);
                func_mov( find_vr(res[0][0]), num );
            }
            else {
                func_mov( find_vr(res[0][0]), v[find_vr(res[2][0])] );
            }
            continue;
        }
        // 涉及到数学运算
        {
            func_expression(res);
        }
    }
    // ------------------------------ 下一行的注释去掉 ------------------------------
    // input_file.close();
    return 0;
}

// declare
void func_declare(string s) {
    v[count_of_vr].name = s[0];
    v[count_of_vr].value = 0;
    v[count_of_vr].id = count_of_vr;
    printf("mov DWORD PTR [ebp-%d], 0\n", (count_of_vr+1)*4);
    count_of_vr ++;
    return;
}

// mov
void func_mov(int id1, vr vr2){
    v[id1].value = vr2.value;
    printf("mov eax, DWORD PTR [ebp-%d]\n", (vr2.id+1)*4);
    printf("mov DWORD PTR [ebp-%d], eax\n", (id1+1)*4);
    return;
}
void func_mov(int id1, int num2){
    v[id1].value = num2;
    printf("mov DWORD PTR [ebp-%d], %d\n", (id1+1)*4, num2);
    return;
}
// calculate
void func_expression(vector<string> res) {
    int find_vr(char);
    int estimate_type(string);
    int func_cal(int, int, char);
    int string_to_int(string);
    int i = 0;
    stack<int> da;
    stack<char> op;
    for (string ss: res) {
        if (i==0 || i==1) {
            i++;
            continue;
        }
        if (ss==";") {
            // 计算栈中的元素
            while (op.size()>0) {
                int a, b;
                char c;
                b = da.top(); da.pop();
                printf("pop ebx\n");
                a = da.top(); da.pop();
                printf("pop eax\n");
                c = op.top(); op.pop();
                int d = func_cal(a, b, c);
                // 把计算结果放入栈
                da.push(d);
                printf("push eax\n");
            }
        }
        // 是字母，将其所表示的值取出来，然后放进da栈
        else if (estimate_type(ss) == 4) {
            printf("mov eax, DWORD PTR [ebp-%d]\n", (find_vr(ss[0])+1)*4);
            printf("push eax\n");
            da.push(v[find_vr(ss[0])].value);
        }
        // 是数字，将其直接放进da栈
        else if (estimate_type(ss) == 3) {
            int num = string_to_int(ss);
            printf("mov eax, %d\n", num);
            da.push(num);
            printf("push eax\n");
        }
        // 是左括号或右括号
        else if (estimate_type(ss) == 2) {
            if (ss=="(") {
                op.push(ss[0]);
            }
            else {
                while (op.size()>0 && op.top()!='(') {
                    int a, b;
                    char c;
                    b = da.top(); da.pop();
                    printf("pop ebx\n");
                    a = da.top(); da.pop();
                    printf("pop eax\n");
                    c = op.top(); op.pop();
                    int d = func_cal(a, b, c);
                    // 把计算结果放入栈
                    da.push(d);
                    printf("push eax\n");
                }
                op.pop();
            }
        }
        // 乘和除，当op栈顶为乘或除时，先计算；否则直接放入
        else if (estimate_type(ss) == 1) {
            if (op.size()>0 && (op.top()=='*' || op.top()=='/')) {
                int a, b;
                char c;
                b = da.top(); da.pop();
                printf("pop ebx\n");
                a = da.top(); da.pop();
                printf("pop eax\n");
                c = op.top(); op.pop();
                int d = func_cal(a, b, c);
                // 把计算结果放入栈
                da.push(d);
                printf("push eax\n");
            }
            op.push(ss[0]);
        }
        else {
            // 加和减，当op栈顶为空或左括号时直接放入，否则要计算
            if (op.size()==0 || op.top()=='(') { }
            else {
                int a, b;
                char c;
                b = da.top(); da.pop();
                printf("pop ebx\n");
                a = da.top(); da.pop();
                printf("pop eax\n");
                c = op.top(); op.pop();
                int d = func_cal(a, b, c);
                // 把计算结果放入栈
                da.push(d);
                printf("push eax\n");
            } 
            op.push(ss[0]);
        }
        // if (op.size()>0) cout << "op is not empty, the size of op is: " << op.size() << ", the top of op is: " << op.top() << endl;
        // if (da.size()>0) cout << "da is not empty, the size of da is: " << da.size() << ", the top of da is: " << da.top() << endl;
    }
    printf("pop eax\n");
    // 把计算结果存入变量
    v[find_vr(res[0][0])].value = da.top();
    printf("mov DWORD PTR [ebp-%d], eax\n", (find_vr(res[0][0])+1)*4);
    return;
}

// find the variable
int find_vr(char c) {
    for (int i=0; i<count_of_vr; i++) {
        if (v[i].name == c) {
            return i;
        }
    }
    return 0;
}

// 加减乘除
int func_cal(int a, int b, char c) {
    if (c=='+') {
        printf("add eax, ebx\n");
        return a+b;
    }
    else if (c=='-') {
        printf("sub eax, ebx\n");
        return a-b;
    }
    else if (c=='*') {
        printf("imul eax, ebx\n");
        return a*b;
    }
    else {
        printf("cdq\n");
        printf("idiv ebx\n");
        return a/b;
    }
    return 0;
}

// transform string to numbers
int string_to_int(string str) {
    stringstream number(str);
    int num = 0;
    number >> num;
    return num;
}

// 判断类型：返回0，表示+或-；返回1，表示*或/；返回2，表示(或)；返回3，表示数字；返回4，表示字母；
int estimate_type(string str) {
    smatch result;
    regex r1("\\+|-");
    if ( regex_match(str, result, r1) ) {
        return 0;
    }
    regex r2("\\*|/");
    if ( regex_match(str, result, r2) ) {
        return 1;
    }
    regex r3("\\(|\\)");
    if ( regex_match(str, result, r3) ) {
        return 2;
    }
    regex r4("-?[0-9]+");
    if ( regex_match(str, result, r4) ) {
        return 3;
    }
    return 4;
}

// return
void func_return(int id) {
    printf("mov eax, DWORD PTR [ebp-%d]\n", (id+1)*4);
    return;
}
