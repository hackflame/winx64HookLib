.code

extern HookHandlerDispath:proc 

e9Hook proc param:QWORD
 pushfq;
 push r15;
 push r14;
 push r13;
 push r12;
 push r11;
 push r10;
 push r9;
 push r8;
 push rdi;
 push rsi;

 push [rbp];
 ;push rbp;
 
 push rbx;
 push rdx;
 push [rbp+10h];  //;push rcx;
 push [rbp+18h];  //;push rax;

 lea rdx,[rsp];
 sub rsp,0100h
 call HookHandlerDispath
 add rsp,0100h
 pop rax;
 mov [rbp+18h],rax; //修正
 pop rcx;
 mov [rbp+10h],rcx; //修正
 pop rdx;
 pop rbx;
 pop rbp;

 pop rsi;
 pop rdi;
 pop r8;
 pop r9;
 pop r10;
 pop r11;
 pop r12;
 pop r13;
 pop r14;
 pop r15;
 popfq;

 mov [rsp],rbp; //主要是绕过编译器生成的代码 为了修改RBP不容易啊
 mov rbp,rsp;
 
 ret;
e9Hook endp

_setNumber proc index:QWORD
	mov r10,rcx;
	ret;
_setNumber endp

__syscall proc
	mov rax,r10;
	mov r10,rcx;
	syscall


	ret;
__syscall endp;

end