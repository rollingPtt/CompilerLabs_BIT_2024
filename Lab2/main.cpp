#include "F.h"
// 变量的结构体
typedef struct variable{
    string name;
    int id;
    int value;
} vr;
// 存储器
vr v[100];
int count_of_vr = 0; // 统计当前的变量数

int main(int argc, char *argv[]) {
    void func_declare(string);
    int find_vr(string);
    void func_expression(vector<string>);
    
    ifstream input_file(argv[1]);

    print_begin();
    
    string s0;
    vector<string> pre_reset, reset;
    // 读入
    while ( getline(input_file, s0) ) {
        if (s0 == "") continue;
        // 按空格分词
        s0 = tokenization(s0);
        // 分词，结果存储在reset中
        stringstream sh;
        sh << s0;
        while (sh >> s0)
            pre_reset.push_back(s0);
    }
    // 去除int main(...), {, }
    int j = 0;
    for (string st: pre_reset){
        if (j<3) { j++; continue; }
        else {
            if (j>3&&classification(st)!=31) {
                reset.push_back(st);
            }
            else if (st==")") {
                j++; continue;
            }
        }
    }

    j = 0;
    while ( j < reset.size() ) {
        vector<string> res;
        while (reset[j]!=";") {
            res.push_back(reset[j]);
            j++;
        } j++;
        res.push_back(";");
        if (classification(res[0])==6) continue;

        // 如果遇到 int，表明变量声明
        if ( classification(res[0])==0 ) {
            func_declare(res[1]);
            continue;
        }
        // 如果遇到 println_int，表明输出
        if ( classification(res[0])==5 ) {
            if (classification(res[2])==2) print_int(v[find_vr(res[2])].value);
            else print_int(string_to_int(res[2]));
            continue;
        }
        // 如果遇到 return，表明返回值
        if ( classification(res[0])==1 ) {
            int id = find_vr(res[1]);
            if (classification(res[1])==2)
                func_return(v[id].value);
            else func_return(string_to_int(res[1]));
            print_end();
            return 0;
        }
        // 最简单的赋值语句：分为直接赋数字和直接赋变量的值
        if ( classification(res[3])==6) {
            if ( classification(res[2])==3 ) {
                int num = string_to_int(res[2]);
                v[find_vr(res[0])].value = num;
            }
            else {
                v[find_vr(res[0])].value = v[find_vr(res[2])].value;
            }
            continue;
        }
        // 涉及到数学运算
        {
            func_expression(res);
        }
    }
    input_file.close();

    return 0;
}

// declare
void func_declare(string s) {
    v[count_of_vr].name = s;
    v[count_of_vr].value = 0;
    v[count_of_vr].id = count_of_vr;
    count_of_vr ++;
    return;
}

// calculate
void func_expression(vector<string> res) {
    int find_vr(string);
    int i = 0;
    stack<int> da;
    stack<string> op;
    for (string ss: res) {
        if (i==0 || i==1) {
            i++;
            continue;
        }
        if (ss==";") {
            // 计算栈中的元素
            while (op.size()>0) {
                int a, b;
                string c;
                b = da.top(); da.pop();
                a = da.top(); da.pop();
                c = op.top(); op.pop();
                int d = func_cal(a, b, c);
                // 把计算结果放入栈
                da.push(d);
            }
        }
        // 是变量，将其所表示的值取出来，然后放进da栈
        else if (classification(ss) == 2) {
            da.push(v[find_vr(ss)].value);
        }
        // 是数字，将其直接放进da栈
        else if (classification(ss) == 3) {
            int num = string_to_int(ss);
            da.push(num);
        }
        // 是左括号或右括号
        else if (classification(ss) == 50) {
            if (ss=="(") {
                op.push(ss);
            }
            else {
                while (op.size()>0 && op.top()!="(") {
                    int a, b;
                    string c;
                    b = da.top(); da.pop();
                    a = da.top(); da.pop();
                    c = op.top(); op.pop();
                    int d = func_cal(a, b, c);
                    // 把计算结果放入栈
                    da.push(d);
                }
                op.pop();
            }
        }
        // 当栈内符号优先级小于等于自己的时候，要先计算，再入栈
        else {
            while (op.size()>0 && ( classification(op.top()) <= classification(ss) )){
                int a, b;
                string c;
                b = da.top(); da.pop();
                a = da.top(); da.pop();
                c = op.top(); op.pop();
                int d = func_cal(a, b, c);
                // 把计算结果放入栈
                da.push(d);
            }
            op.push(ss);
        }
    }
    // 把计算结果存入变量
    v[find_vr(res[0])].value = da.top();
    return;
}

// find the variable
int find_vr(string s) {
    for (int i=0; i<count_of_vr; i++) {
        if (v[i].name.compare(s)==0) {
            return i;
        }
    }
    return 0;
}
