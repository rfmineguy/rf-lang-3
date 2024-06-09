; module Main

exit:
	mov rax, 60
	mov rbx, 0
	syscall

main_i_c_i:
	push rbp
	mov rbp, rsp
	mov dword ptr [rbp - 4], 4
	mov byte ptr [rbp - 8], 38

	mov rsp, rbp
	pop rbp
	call exit
