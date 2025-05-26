## 实验一：初代编译器实验

### 实验要求

详细实验要求请参考文件《Lab1实验说明和要求-X86.pdf》。



### 文件

共包括两个 cpp 文件：

- compilerlab1_test_v2.cpp

  是可以在本地直接编译使用的源文件，是用于测试输入输出结果的文件。

  注意每次输入结束后，需要另起一行，并在该行输入+++（3个加号）才能终止输入并显示输出。

- compilerlab1.cpp

  是根据实验平台提交要求修改了输入输出方式的文件，也是最终提交的文件。



### 实验注意

1. 该部分主要阐述本人在做该实验时踩过的坑

2. 注意按照平台要求的命令行输入输出，否则提交无效

3. 关于测试用例8：需要处理可能出现的回车，可能出现一条语句中间被回车截断，如

   ```
   int a ;
   a 
   = 
   1
   ;			// 一条语句 a = 1 ; 被拆成了多行输入
   return a
   ;
   // 下面是个人的猜想，还可能会出现同一行多条语句，例如：
   int b ; int c ; b = 1 ; c = 2 ;
   ```

4. 关于测试用例3和7：注意处理直接赋变量的值的语句，如

   ```
   int a ;
   int b ;
   a = 1 ;			// 对于这一句，直接把1存入a即可，即 mov DWORD PTR [ebp-4], 1 
   b = a ;			// 对于这一句，容易写成把a的值直接mov到b中；然而这样做是错的，要先从a取出放到eax，再从eax取出放到b
   return b ;
   ```



### 测试用例（可用于复制粘贴本地测试）

```
输入用例 1:
int a ;
int b ;
int d ;
a = 1 ;
b = 2 ;
d = a + b ;
return d ;
```

```
用例 1 的期待输出: (你的输出中不必输出#行的内容)
# int a ;
mov DWORD PTR [ebp-4], 0
# int b ;
mov DWORD PTR [ebp-8], 0
# int d ;
mov DWORD PTR [ebp-12], 0
# a = 1 ;
mov DWORD PTR [ebp-4], 1
# b = 2 ;
mov DWORD PTR [ebp-8], 2
# d = a + b ;
mov eax, DWORD PTR [ebp-4]
push eax
mov eax, DWORD PTR [ebp-8]
push eax
pop ebx
pop eax
add eax, ebx
push eax
pop eax
mov DWORD PTR [ebp-12], eax
# return d ;
mov eax, DWORD PTR [ebp-12] 
```



```
输入用例 2:
int a ;
int b ;
int c ;
int d ;
a = 1 ;
b = 2 ;
c = 3 ;
d = ( a + b * 2 ) / c - 3 ;
return d ;
```

```
用例 2 的期待输出: (你的输出中不必输出#行的内容)
# int a ;
mov DWORD PTR [ebp-4], 0
# int b ;
mov DWORD PTR [ebp-8], 0
# int c ;
mov DWORD PTR [ebp-12], 0 
# int d ;
mov DWORD PTR [ebp-16], 0
# a = 1 ;
mov DWORD PTR [ebp-4], 1
# b = 2 ;
mov DWORD PTR [ebp-8], 2
# c = 3 ;
mov DWORD PTR [ebp-12], 3
# d = ( a + b * 2 ) / c - 3
mov eax, DWORD PTR [ebp-4]
push eax
mov eax, DWORD PTR [ebp-8]
push eax
mov eax, 2
push eax
pop ebx
pop eax
imul eax, ebx
push eax
pop ebx
pop eax
add eax, ebx
push eax
mov eax, DWORD PTR [ebp-12]
push eax
pop ebx
pop eax
cdq
idiv ebx
push eax
mov eax, 3
push eax
pop ebx
pop eax
sub eax, ebx
push eax
pop eax
mov DWORD PTR [ebp-16], eax
# return d
mov eax, DWORD PTR [ebp-16]
```

```
输入用例 3: (试试直接赋值,顺便试试大写字母)
int a ;
int A ;
int B ;
a = 1 ;
A = a ;
B = A / a ;
return B ;
```

```
用例 3 的期待输出: (你的输出不必像我这里特意搞空行)
mov DWORD PTR [ebp-4], 0

mov DWORD PTR [ebp-8], 0

mov DWORD PTR [ebp-12], 0

mov DWORD PTR [ebp-4], 1

mov eax, DWORD PTR [ebp-4]
mov DWORD PTR [ebp-8], eax

mov eax, DWORD PTR [ebp-8]
push eax
mov eax, DWORD PTR [ebp-4]
push eax
pop ebx
pop eax
cdq
idiv ebx
push eax
pop eax
mov DWORD PTR [ebp-12], eax

mov eax, DWORD PTR [ebp-12]
```

```
输入用例 4: (莫名其妙的换行)
int
p
; int q ; p =
1 
; 
q
= p 
+
p ; return 
q 
;
```

```
用例 4 的期待输出: (你的输出不必像我这里特意搞空行)
mov DWORD PTR [ebp-4], 0

mov DWORD PTR [ebp-8], 0

mov DWORD PTR [ebp-4], 1

mov eax, DWORD PTR [ebp-4]
push eax
mov eax, DWORD PTR [ebp-4]
push eax
pop ebx
pop eax
add eax, ebx
push eax
pop eax
mov DWORD PTR [ebp-8], eax

mov eax, DWORD PTR [ebp-8]
```



朱逸晨  2024.3.18  ver1.2