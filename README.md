# BrainFuckVM
a brainFuck virtual machine uesd cpp

## start: 

```sh
make
```

## use:

```sh
bl [/d] filename1 filename2
```

`/d`: debug mode, it can print the ram step by step .

## others

about brainFuck: [https://en.wikipedia.org/wiki/Brainfuck](https://en.wikipedia.org/wiki/Brainfuck)

扩展语法 expand grammar:

1. 支持以 `//`, `#`, `/*`和`*/` 的方式使用注释. 
Support for comments in the form " //", "#", " / *" and " * /".

2. (n) 像当前指针所指的地址填入数字n. eg. (10), (-23), (+33). (n) 
Fill in the number n. E.g. (10), (-23), (+ 33)

3. "string" 从当前地址开始, 依次填入字符 string 向各个地址中, 当前所指向的地址不变. 例如内存一开始为 `00000`, 执行 `>"abc">.`, 输出 `b`, 内存变为 `0abc0`. 
"string" starts at the current address, and then fills the string to each address in turn, and the current pointed address does not change. For example, the memory starts with `00000`, executes `>"abc">.`, the output is `b`, and the memory becomes `0abc0`.

4. {n} 从当前地址开始打印 n 个字符. 
{n} Print n characters from the current address.

5. ? 输出当前地址的内容的ascii码值. 例如 `"a".` 输出 `a`, `"a"?` 输出 `(97)`.
Output the ASCII code value of the current address content. For example ``a''. `output `a`, `"a"? `output` (97)`.

## test

run bf files in `./test`