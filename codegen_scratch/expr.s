; codegen
; main = argc: int, argv: char** -> int {
;   a: int = 4 - 3 * 5
; }

; at&t syntax
main_i_cpp_i:
	push ebp
	mov  ebp, esp

	mov  eax, 3
	imul eax, 5
	mov  ebx, 4
	sub  ebx, eax

	mov  esp, ebp
	pop  ebp
