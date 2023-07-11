## phase_1(400ee0)
    %rsb = %rsb - 0x8
    %eax = 0x402400
    call <strings_not_equal>
    if(%eax != 0){
        call explode_bomb
    }
    %rsp += 0x8
    return

## strings_not_equal(401338)
    //args: char *s in %rdi, char *b in %rsi
    push %r12, %rbp, %rbx #后续要用的寄存器
    %rbx = s(%rdi)
    %rbp = b(%rsi)
    %r12d = string_lengths(s(%rdi))
    %rdi = b(%rbp)
    %eax = string_lengths(b(%rdi))
    %edx = 1
    if(%eax != %r12d){
        // if len(s) != len(b), return 1
        goto stop(+99)
    }
    %al = *s (%rbx)
    if(%al == 0){
        //*s == 0
        goto return_0(+80)
    }
    if(*%rbp == %al){
        //*b == *s
        goto next_char(+58)
    }
    return_1(+87)
    if(*%rbp != %al){   <---char_judge(+50)
        goto return_1(+94)
    }
    s++(%rbx)           <---next_char(+58)
    b++(%rbp)
    %al = *s(%rbx)
    if(%al != 0){
        goto char_judge(+50)
    }
    %edx = 0
    goto stop(+99)
    %edx = 0            <---return_0(+80)
    goto stop(+99)
    %edx = 1            <---return_1(+87)
    goto stop(+99)
    %edx = 1            <---return_1(+94)
    %eax = %edx         <---stop(+99)
    pop %rbx, %rbp, %r12
    return


## string_length(40131b)
    //args: char *s(s = &input[0]) in rdi
    if(*s == 0){
        %eax = 0
        return //if EOF return 0
    }
    %rdx = s(%rdi)
    %rdx += 1       <---begin
    %eax = %edx - %edi //calculate length
    if((%rdx) != 0){
        //if next not EOF
        goto begin
    }
    return (%eax)


## skip(4013f9)
    push %rbx#保存被调用者
    %rax = num_input_strings
    %rdi = 5 * %rax
    %rdi <<= 4 # %rdi = 80 * %rax
    %rdi += 0x603780 # rdi = 80 * %rax + 0x603780
    %rdx = infile
    %esi = 0x50


## phase_2(400efc)
    //M[0x6037d0] = (user's input)
    push %rbp, %rbx
    %rsp -= 0x28    //alloc 40 bytes in stack
    %rsi = %rsp
    call read_six_numbers   //read 6 numbers from stdin, store in stack, if no 6 numbers, explode.
    if(M[%rsp] == 1)
        //check first number: 1
        goto +52
    call explode_bomb
    goto +52
    %eax = M[%rbx-0x4]      <---+27
    %eax += %eax
    if(%eax == M[%rbx])
        //next number should be 2*x
        goto +41
    call explode_bomb
    %rbx += 4               <---+41
    if(%rbp != %rbx)
        goto +27
    goto +64
    %rbx = %rsp + 0x4   //next number      <---+52
    %rbp = %rsp + 0x18  //6 numbers in total
    goto +27
    %rsp += 0x28    //restore stack <---+64
    pop %rbx, %rbp
    return

## read_six_numbers(40145c)
    %rsp -= 0x18    //alloc  24 bytes in stack
    %rdx = %rsi     //%rdx = original stack pointer
    %rcx = %rsi + 4
    %rax = %rsi + 20
    M[%rsp+0x8] = %rax    //stack:NA, %rsi+0x14, NA stack:NA, &(0x40143100000000), NA
    %rax = %rsi + 16
    M[%rsp] = %rax  //stack:NA, %rsi+0x14, %rsi+0x10    stack:&(0x02), &(0x40143100000000)
    %r9 = %rsi + 12
    %r8 = %rsi + 8
    %esi = 0x4025c3
    %eax = 0
    call __sioc99_sscabf@plt
    if(%eax > 5)
        goto pass(+61)
    call explode_bomb
    %rsp += 0x18    //restore stack <---pass(+61)
    return

## phase_3
    %rsp -= 0x18    //alloc 24 bytes in stack
    %rcx = %rsp + 0xc
    %rdx = %rsp + 0x8
    %esi = 0x4025cf
    %eax = 0
    call __isoc99_sscanf@plt
    if(%eax > 1)
        goto +39
    call explode_bomb
    if(M[%rsp+8] > 7)               <---+39
        goto fail(+106)
    %eax = M[%rsp+8]
    goto M[0x402470 + 8*%rax]  //offset = num1
    %eax = 0xcf                    <---num1=0
    goto +123
    %eax = 0x2c3                   <---num1=2
    goto +123
    %eax = 0x100                   <---num1=3
    goto +123
    %eax = 0x185                   <---num1=4
    goto +123
    %eax = 0xce                    <---num1=5
    goto +123
    %eax = 0x2aa                   <---num1=6
    goto +123
    %eax = 0x147                   <---num1=7
    goto +123
    call explode_bomb               <---fail(+106)
    %eax = 0
    goto +123
    %eax = 0x137                    <---num1=1
    if(%eax == M[0xc+%rsp])         <---+123
        goto +134
    call explode_bomb
    %rso += 18                      <---+134
    return

    //good pairs:
    0, 0xcf
    1, 0x137
    2, 0x2c3
    3, 0x100
    4, 0x185
    5, 0xce
    6, 0x2aa
    7, 0x147

## phase_4
    %rsp -= 0x18    //alloc 28 bytes
    %rcx = %rsp + 0xc
    %rdx = %rsp + 0x8
    %esi = 0x4025cf
    %eax = 0
    call __isoc99_sscanf@plt
    if(%eax != 2)   //should input 2 figures
        goto fail(+41)
    if(M[%rsp+0x8] <= 0xe) 
        goto continue(+46)
    call explode_bomb           <---fail(+41)
    %edx = 0xe                  <---continue(+46)
    %edx = 0xe
    %esi = 0x0
    %edi = M[%rsp+0x8]  //num1
    call func4  //when arg1(%rdi) is 0, 1, 3, 7, return 0
    if(%eax != 0)
        goto fail(+76)  //num 1 should let func4 return 0
    if(M[%rsp+0xc] == 0)    //num2 should be 0
        goto continue(+81)
    call explode_bomb          <---fail(+76)
    %rsp += 8                  <---continue(+81)
    return

## func4
    //args: %rdi = num1; %rsi = 0x0; %rdx = 0xe
    %rsp -= 0x8   //alloc 8 bytes
    %eax = %edx
    %eax -= %esi
    %ecx = %eax
    %ecx >>L 0x1f   //save sign only
    %eax += %ecx
    %eax >>A ?        
    %ecx = %rax + %rsi
    if(%ecx <= %edi)
        goto +36
    %edx = %rcx - 1
    call func4
    %eax += %eax
    goto return(+57)
    %eax = 0                <---+36
    if(%ecx >= %edi)
        goto return(+57)
    %esi = %rcx + 1
    call func4
    %eax =2*%rax + 1
    %rsp += 8               <---return(+57)
    return

    //possible num1:
    0, 1, 3, 7

## phase_5
    push %rbx
    %rsp -= 0x20        //alloc 32 bytes in stack
    %rbx =  %rdi        //pointer to user_input_pwd
    %rax = %fs + 0x28
    M[%rsp + 0x18] = %rax
    %eax = 0
    call string_length
    if(%eax != 0x6)
        call explode_bomb   //target string should be 6 chars
    goto +112
    %ecx = M[%rax + %rbx]  //%rax(th) character in input         <---process_start(+41)
    M[%rsp] = %cl   //%cl is %ecsx
    %rdx = M[%rsp]
    %edx &= 0xf         //extract low 4 bits, %rdx = 0~f
    %edx = M[0x4024b0 + %rdx]   
    /*when %rdx = 0~f, %edx should be:
    6d, 61, 64, 75
    69, 65, 72, 73
    6e, 66, 6f, 74
    76, 62, 79, 6c*/ 
    %dl = M[%rax + %rsp + 0x10] //store the converted string in M[%rsp + 0x10]
    %rax += 1
    %if(%rax != 6)
        goto process_start(+41)
    M[%rsp + 0x16] = 0
    %esi = 0x40245e             //"flyers"
    %rdi = %rsp + 0x10          //*converted_string
    call strings_not_equal
    if(%eax == 0)
        //converted_string should be "flyers"
        //original_string should be:
        //x9 xf xe x5 x6 x7 where x is any from 0 to f
        goto start_return(+119)
    call explode_bomb
    %eax = 0                    <---+112
    goto process_start(+41)
    %rax = M[%rsp + 0x18]       <---start_return(+119)
    if(!(%rax ^ %fs:0x28))
        goto successful_return(+140)
    call __stack_chk_fail@plt
    %rsp += 0x20        <---successful_return(+140)
    pop %rbx
    return

## phase_6
    push %r14, %r13, %r12, %rbp, %rbx
    %rsp -= 0x50        //alloc 80 bytes in stack
    %r13 = %rsp     //points to first number
    %si = %rsp
    call read_six_numbers   //6 numbers stored in *%rsp
    %r14 = %rsp
    %r12d = 0
    %rbp = %r13         <---+32
    %eax = M[%r13]
    %eax -= 1
    if(%eax > 5)
    //no number should > 6
        call explode_bomb
    %r12d += 1
    if(%r12d == 6)      //%r12d: outer_looper
        goto +95
    %ebx = %r12d    //%ebx: small_looper
    %rax = %ebx             <---+65
    %eax = M[%rsp + 4*%rax] //%rax(th) number
    if(M[%rbp] == %eax)
        //no numbers should be the same as the first
        call explode_bomb
    %ebx += 1
    if(%ebx <= 5)
        goto +65
    %r13 += 4   //points to next number
    goto +32
    //no two numbers are same
    %rsi = %rsp + 0x18  //points right after 6 numbers
    %rax = %r14 //points to the start of the numbers
    %ecx = 7
    %edx = %ecx         <---+108
    %edx -= M[%rax] 
    M[%rax] = %edx  //n[%rax] = 7-n[%rax]
    %rax += 4
    if(%rax != %rsi)
        goto +108
    %esi = 0
    goto +163
    %rdx = M[%rdx + 8] //%rdx = %rdx->next     <---+130
    %eax += 1
    if(%eax != %ecx)
        goto +130
    //%rdx = *list[num[%rsi](modified)]
    //original:n --> %rdx:*n
    goto +148
    %edx = 0x6032d0                 <---+143
    M[%rsp + 2*%rsi + 0x20] = %rdx  <---+148
    %rsi += 4
    if(%rsi == 18)
        goto +183
    %ecx = M[%rsp + %rsi]   //%ecx = num[%rsi](modified)
    if(%ecx <= 1)
        goto +143
    %eax = 1
    %edx = 0x6032d0     //starting point of an linked list.
    //element: num, serial, *next
    goto +130
    %rbx = M[%rsp + 0x20]
    %rax = %rsp + 0x28
    %rsi = %rsp + 0x50
    %rcx = %rbx
    %rdx = M[%rax]          <---+201
    M[%rcx + 8] = %rdx  //%rcx->next = %rdx
    %rax += 8
    if(%rax == %rsi)
        goto +222
    %rcx = %rdx
    goto +201
    M[%rdx + 8] = 0
    %ebp = 5
    %rax = M[%rbx + 8]      <---+235
    %eax = M[%rax]
    if(M[%rbx] < %eax)
    //should sort the element in linked list
        call explode_bomb
    %rbx = M[%rbx + 8]
    %ebp -= 1
    if(%ebp != 0)
        goto +235
    %rsp += 50
    pop %rbx, %rbx, %r12, %r13, %r14
    return

