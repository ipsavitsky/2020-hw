# Решение таска p01

## Задание 10

> Проверьте, выполняется ли правило "ленивых вычислений" выражений в Си, т.е. прекращается ли вычисление выражений с логическими операциями, если возможно "досрочно" установить значение результата. 

*Ленивые вычисления*: стратегия вычисления которая гласит, что вычисления надо откладывать до тех пор, пока не понадобится их результат. Грубо говоря, самое большое преимущество таких вычислений состоит в том, что вычсисления результат которых не понадобится никогда не произойдут.

Для проверки того, выполняются ли в языке Си такие вычисления была написана следующая программа:

```c
#include <stdio.h>

int check(void){
    printf("lazy evaluation DOES NOT WORK!! ABANDON SHIP!!!\n");
    return 1;
}

int main(void){
    printf("%d\n", 0 && check());
    return 0;
}
```

В `printf()` в функции `main()` стоит логическое выражение `0 && check()`. Если ленивые вычисления работают, то функция `check()` никогда и не вызовится потому что результат операции `&&` уже детерменирован нулем. Нетрудно проверить, что так и есть. Программа выводит:
```
0
```
<!---
 TODO: Можно вставить сюда пример с отключенными ленивыми вычислениями
-->

## Задание 18

> Определите, каким образом при выполнении операции присваивания и явном приведении происходит преобразование беззнаковых целых (M-битовое представление) к беззнаковым целым (N-битовое представление) при M > N, M = N, M < N. 

Рассмотрим три случая. Для каждого из них запустим диссасемблер:
```
gcc -g -c test.c
objdump -d -M intel -S test.o
```
(можно сразу сделать вывод при помощи `gcc -S -fverbose-asm -masm=intel test.c`, однако дисассемблер дает более "чистый" вывод с которым проще работать, хотя на всякий случай я перепроверил)

Некоторые части этих преобразований могут зависеть от компилятора, поэтому, на всякий случай, укажу версию компилятора
```
gcc version 9.3.0 (Ubuntu 9.3.0-10ubuntu2)
```
Для каждого пункта в условии рассмотрим вывод дисассемблера:

---
- `M > N`

```c
int main(void){
	unsigned long int i = 15;
	unsigned int j = (int)i;
	return 0;
}
```

```x86asm
    ;int main(void){
    endbr64 
    push   rbp
    mov    rbp,rsp
    ;long int i = 15;
    mov    QWORD PTR [rbp-0x8],0xf
    ;int j = i;
    mov    rax,QWORD PTR [rbp-0x8]
    mov    DWORD PTR [rbp-0xc],eax
    ;return 0;
    mov    eax,0x0
    ;}
    pop    rbp
    ret  
```
(На всякий случай, добавим случай в котором значение в изначальной переменной не поместится в конечную переменную)
```c
#include <stdio.h>

int main(void){
	unsigned long int i = 4294967295;
	unsigned int j = (int)i;
	return 0;
}
```
```x86asm
    ;int main(void){
    endbr64 
    push   rbp
    mov    rbp,rsp
    ;unsigned long int i = 4294967295;
    mov    eax,0xffffffff
    mov    QWORD PTR [rbp-0x8],rax
    ;unsigned int j = (int)i;
    mov    rax,QWORD PTR [rbp-0x8]
    mov    DWORD PTR [rbp-0xc],eax
    ;return 0;
    mov    eax,0x0
    pop    rbp
    ret 
```
---

 - `M = N`

 ```c
#include <stdio.h>

int main(void){
	unsigned int i = 15;
	unsigned int j = (int)i;
	return 0;
}
 ```

 ```x86asm
    ;int main(void){
    endbr64 
    push   rbp
    mov    rbp,rsp
    ;unsigned int i = 15;
    mov    DWORD PTR [rbp-0x8],0xf
    ;unsigned int j = (int)i;
    mov    eax,DWORD PTR [rbp-0x8]
    mov    DWORD PTR [rbp-0x4],eax
    ;return 0;
    mov    eax,0x0
    ;}
    pop    rbp
    ret 
 ```

 ---
 - `M < N`

 ```c
#include <stdio.h>

int main(void){
	unsigned int i = 15;
	unsigned long int j = (long int)i;
	return 0;
}
 ```

 ```x86asm  
    ;int main(void){
    endbr64 
    push   rbp
    mov    rbp,rsp
    ;unsigned int i = 15;
    mov    DWORD PTR [rbp-0xc],0xf
    ;unsigned long int j = (long int)i;
    mov    eax,DWORD PTR [rbp-0xc]
    mov    QWORD PTR [rbp-0x8],rax
    ;return 0;
    mov    eax,0x0  
    ;}
    pop    rbp
    ret 
```
---
Очевидно, что преобразование целых типов данный компилятор производит тем, что при помощи `DWORD PTR` И `QWORD PTR`(потому что для тестирования были выбраны `int` и `long int` размером 4 и 8 байт соответственно) берет участки памяти разного размера. При приведении большего типа к меньшему компилятор берет младшие биты, а старшие "обрубает"