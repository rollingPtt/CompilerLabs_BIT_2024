## 实验二：二代编译器实验

### 一、实验要求

详细实验要求请参考文件《Lab2实验说明和要求v5.pdf》。



### 二、文件

共包括五个文件：

- main_without_arg.cpp

  是可以在本地直接编译使用的源文件，是用于测试输入输出结果的文件。

  注意每次输入结束后，需要另起一行，并在该行输入+++（3个加号）才能终止输入并显示输出。

- main.cpp

  是根据实验平台提交要求修改了输入输出方式的文件，**是最终提交的文件之一**。

- F.h

  用于声明函数，**是最终提交的文件之二**。

- F.cpp

  用于具体定义 F.h 中的函数，**是最终提交的文件之三**。
  
- CMakeLists.txt

  用于组装文件，后文有提到它的写法，**是最终提交的文件之四**。



### 三、实验思路

1、准备工作 1

```
定义函数: int classification(string)
作用: 对字符串进行词法分析
接受的输入: 字符串
返回值: 一个整数, 代表该字符串的内容
下面是一个示例:
int classification(string str_to_be_processed){
	if ( str_to_be_processed == "int" )
		return 01;
	if ( str_to_be_processed == "return" )
		return 02;
	if ( str_to_be_processed == "main" )
		return 03;
	if ( str_to_be_processed 是 数字 )
		return 10;
	if ( str_to_be_processed 是 标识符 )
		return 20;
	if ( str_to_be_processed == "=" )
		return 30;
	...
	// 不属于以上任何一种情形, 表示读到了未知字符串
	return 0;
}
```

2、准备工作 2

```
定义函数: string tokenization(string)
作用: 对文本添加空格
接受的输入: 待处理的字符串, 如 int main(){int a;a=1+1;println_int(a);return 0;};
返回值: 一个处理后的字符串, 上面的例子是 int main ( ) { int a ; a = 1 + 1 ; println_int ( a ) ; return 0 ; }
这样处理后就和lab1一模一样了.

该函数的实现原理是: 依次读字符, 并判断组成的字符串是否可识别.
下面是一个简单的示例, 假设我们读到的是 str_before_processed = "int main(){return 0;}"
string tokenization(string str_before_processed){
	string str_after_process = "";
	string str_temp = "";
	for (char i: str_before_processed) {
		// 如果 str_temp+i 不是可识别字符串, 说明 str_temp 是一个单词
		if ( classification(str_temp+i)==0 ) {
			str_after_process += (str_temp+" ");
			// 如果 i 是无意义的空格, 直接忽略; 否则是有意义的字符, 选择保留
			if (i==' ') {
				str_temp = "";
			}
			else {
				str_temp = "" + i;
			}
		}
		// 否则 str_temp+i 是可识别的字符串, 把 i 加到 str_temp 的后面
		else {
			str_temp += i;
		}
	}
	return str_after_process;
}
提醒: 这个示例非常简单, 实际上你会遇到一些其他的细节处理, 你自己写写就知道了~
```

<font size='5'>下面是一个更详细的例子</font>

<font size='10'>int main()</font>

```
1. 读 'i' , 发现 "i" 是合法的字符串, 继续往下读
2. 读 'n' , 发现 "in" 是合法的字符串, 继续往下读
3. 读 't' , 发现 "int" 是合法的字符串, 继续往下读
4. 读 ' '(空格), 发现 "int " 不是合法的字符串, 说明 "int" 是一个词, 把 "int" 存入 str_after_process,  str_after_process = “int”
5. 当前符号是 ' '(空格), 没有意义, 直接丢弃. 
6. 读 'm' , 发现 "m" 是合法的字符串, 继续往下读
7. 读 'a' ,  发现 "ma" 是合法的字符串, 继续往下读
8. 读 'i' , 发现 "mai" 是合法的字符串, 继续往下读
9. 读 'n' , 发现 "main" 是合法的字符串, 继续往下读
10. 读 '(' , 发现 "main(" 不是合法的字符串, 说明 "main" 是一个词, 把 "main" 存入 str_after_process,  str_after_process = “int main”
11. 当前符号是 '(', 保留
12. 读 ')' ,  发现 "()" 不是合法的字符串, 说明 "(" 是一个词, 把 "(" 存入 str_after_process, str_after_process = “int main (”
13. 当前符号是 ')', 保留
14. 后面没有字符了, 说明 ")" 是最后一个词, 把 ")" 存入 str_after_process,  str_after_process = “int main ( )”
15. 分词结束, return 的内容是: "int main ( )"
```

3、其他准备工作

```
包括但不限于:
1. 用于输出程序固定开头的
void print_begin();
2. 用于输出程序固定结尾的
void print_end();
3. 把字符串转化为数字的
输入: "-10"
输出: -10
int from_string_to_int(string str);
...
根据你自己的需要添加函数. 
```

4、代码大致过程

1. 第一步，当然是读入数据
2. **第二步**，预处理数据，或者叫分词。这一步的目的是把输入转化成好处理的形式。也就是我上面写的 tokenization 函数做的事情。
3. 第三步，输出程序的开头固定内容。该步骤可以放在第一步。
4. **第四步**，逐行识别表达式的含义，该定义新变量就定义新变量，该赋值就赋值，该计算表达式就计算，该 `println_int` 就 `println_int` ，该 `return` 就 `return`。
5. 第五步，输出程序的结尾固定内容。



### 四、实验注意

1. 该部分主要阐述本人在做该实验时踩过的坑

2. 这次提交方式和 lab1 略有不同，主要表现在提交到希冀自带的 github 上，按照 pdf 里介绍的方法提交即可。

   我第一次提交时不懂 **CMakeList 的写法**，其实很简单，格式如下：

   ```
   cmake_minimum_required(VERSION 3.16)
   project(lab02)
   set(CMAKE_CXX_STANDARD 14)
   add_compile_options(-pedantic)
   # add_compile_options(-fsanitize=address)
   # add_link_options(-fsanitize=address)
   add_executable(Compilerlab2
    main.cpp	# 请根据你自己的文件名修改
    F.cpp		# 请根据你自己的文件名修改
   )
   target_compile_features(Compilerlab2 PRIVATE cxx_std_14)
   ```

   补充：如果你用了 FLEX 之类的辅助生成工具的话，因为我没用到这个，这种我不会写，可以看看题目文件或者自己研究一下吧。

3. 当遇到大片的 internal error 时

   请检查你的拼写。本人在开头的

   ```
   .intel_syntax noprefix
   ```

   写错为

   ```
   .intex_syntax noprefix
   ```

   检查不少于三次才发现。所以务必认真仔细。

4. 还是**拼写错误**，我在

   ```
   mov eax, 2
   add esp, 8
   ```

   写错为

   ```
   mov eax 2
   add esp 8
   ```

   没学过汇编的人是这样的（笑）

5. 如果你在提交代码后，发生**编译错误**，具体如下

   ```
   collect2: error: ld returned 1 exit status	# 尤其是这一句
   make[2]: *** [CMakeFiles/Compilerlab2.dir/build.make:113: Compilerlab2] Error 1
   make[1]: *** [CMakeFiles/Makefile2:83: CMakeFiles/Compilerlab2.dir/all] Error 2
   make: *** [Makefile:91: all] Error 2
   ```

   错误的可能性非常多，你需要去搜索一下 `collect2: error: ld returned 1 exit status`，然后逐步纠错。

   如果你像我一样，改了很久还是实在不知道哪里编译错误，你可以在不报错的情况下删掉一些函数或者内容，也就是排除法，逐步定位到具体是哪个部分导致的出错。

6. 如果你出现了**超时**的问题

   请考虑正则表达式的效率问题。如果你使用大量的正则表达式匹配，例如：

   ```
   int classification(string str) {
       smatch match;
       regex _main("main");
       regex _int("int");
       regex _return("return");
       regex 标识符("[a-zA-Z_][0-9a-zA-Z_]*");
       regex 数字("[-+]?[0-9]+");
       regex 运算符1("[*/%]"), 运算符2("[+-]");
       regex 运算符3("[><]=?"), 运算符4("==|!=");
       regex 其他符号串(...);
   
       if ( regex_match(str, match, _main) )			return 1;
       if ( regex_match(str, match, _int) )			return 2;
       if ( regex_match(str, match, _return) )			return 3;
       if ( regex_match(str, match, 标识符) )				return 11;
       if ( regex_match(str, match, 数字) )				 return 21;
       if ( regex_match(str, match, 运算符1) )			return 31;
       if ( regex_match(str, match, 运算符2) )			return 32;
       if ( regex_match(str, match, 运算符3) )			return 33;
       if ( regex_match(str, match, 运算符4) )			return 34;
       if ( regex_match(str, match, 其他符号串) )			return ...;
       
       return -1;
   }
   ```

   那是不行的。要么就是 CMakeList 时会超时，要么就是运行时 TLE。所以你不应该太依赖于正则表达式匹配，如上面的程序，可以这么修改：

   ```
   // 把 regex 的定义放在外面
   regex 标识符("[a-zA-Z_][0-9a-zA-Z_]*");
   regex 数字("[-+]?[0-9]+");
   regex 运算符("[*/%+-><]|[><!=]=");
   regex 其他符号串(...);
   
   int classification(string str) {
       smatch match;
       // 不用正则表达式匹配
       if ( str=="main" )								return 1;
       if ( str=="int" )								return 2;
       if ( str=="return" )							return 3;
       // 实在不好处理的内容可以适当地用正则表达式
       if ( regex_match(str, match, 标识符) )				return 11;
       if ( regex_match(str, match, 数字) )				 return 21;
       if ( regex_match(str, match, 运算符) )				return 31;
       if ( regex_match(str, match, 其他符号串) )			return ...;
       
       return 0;
   }
   ```

   这在一定程度上可以缓解问题。

7. 如果你的个别用例（如用例 6 ）总是出现 **internal error**

   请考虑括号问题。这里指的是词语后面跟着的括号，而不是运算中的括号。你一定要把词语和括号分开处理，而不要试图投机取巧。例如下面的情况都是可能出现的：

   ```
   int main(){
   int main() {
   int main( ){
   int main (){
   println_int(a);
   println_int (a);
   println_int( a );
   ```

   甚至是下面这种

   ```
   int main
   (){
   ```

   或者

   ```
   int main(
   )
   {
   ```

   其实对于后面两种，我也不敢保证一定有，但我还是觉得你应当考虑这些。

8. 运算

   本人对于表达式的处理是直接算中缀表达式的，如果你想先转换成后缀再计算，亦或者有其他方法，只要能抓到耗子，都是好猫。

   处理表达式可能会遇到诸多细节，这时请移步数据结构与算法设计，我们不讨论这些内容。

   关于运算，我在下面为你提供了测试 2 和测试 5，你可以试试能否运行，以及结果是否正确。

9. 如果你的用例 6 总是 wrong

   请考虑变量赋值的情况。该错误来自我的舍友，他表示在处理诸如 `b=a;` 这类赋值时需要注意。



### 五、测试

我为你准备了一些测试用例，你可以自己测测看。

```
# 测试用例1
# 说明: 该用例与实验要求中提供的测试用例一致, 是最简单的测试
int main() {
 int a;
 int b;
 int c;
 a = 1;
 println_int(a);
 b = 2;
 println_int(b);
 c = 114514;
 println_int(c);
 return 0;
}
```

```
# 测试用例2
# 说明: 该用例测试了所有的简单双目运算
int main ( ) {
    int a;
    int b;
    a=10;
    b=3;
    int c;
    c=a+b;
    println_int(c);
    c=a-b;
    println_int(c);
    c=a*b;
    println_int(c);
    c=a/b;
    println_int(c);
    c=a%b;
    println_int(c);
    c=a|b;
    println_int(c);
    c=a&b;
    println_int(c);
    c=a^b;
    println_int(c);
    c=a==b;
    println_int(c);
    c=a!=b;
    println_int(c);
    c=a>b;
    println_int(c);
    c=a<b;
    println_int(c);
    c=a>=b;
    println_int(c);
    c=a<=b;
    println_int(c);
    return 0;
}
```

```
用例 2 的期待回答:
13, 7, 30, 3, 1, 11, 2, 9, 0, 1, 1, 0, 1, 0
```

```
# 测试用例3
# 说明: 该用例测试了一些特别的标识符, 以及等号前后空格的处理
int main (){
    int _app;
    _app=100;
    int main1;
    main1 =_app;
    println_int(main1);
    int int_return_;
    int_return_= main1+_app;
    return int_return_;
}
```

```
# 测试用例4
# 说明: 该用例测试了乱七八糟的输入, 空格和回车
int main
(){ int aa;
int   

BB; println_int ( aa ) ;
return
BB;


}
```

```
# 测试用例5
# 说明: 该用例测试了一些参数的返回值
int main(){
	println_int(10);
	int a;
	a=20;
	println_int(a);
	return 30;
}
```

```
# 测试用例6
# 说明: 该用例为难度二, 我们测试一些较为复杂的运算
int main(){
    int a1;
    a1=-5-2*4+16;
    println_int(a1);
    int b2;
    b2=(a1==3)&(a1^1)!=2;
    println_int(b2);
    int c3;
    c3=a1>=b2+3!=0&1;
    println_int(c3);
    int d4;
    d4=(a1^b2^c3)!=c3;
    println_int(d4);
    e5=a1%(d4+c3+b2)<d4;
    println_int(e5);
    return e5;
}
```

```
用例 6 的期待回答:
3, 0, 1, 1, 0
```



朱逸晨  2024.4.6  ver1.4