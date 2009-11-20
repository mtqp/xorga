BITS 32
%include "macrosmodoprotegido.mac"
%include "macroInterrupciones.mac"
extern pic1_intr_end
msgisr0: db 'EXCEPCION: Division por cero'
msgisr0_len equ $-msgisr0

; ----------------------------------------------------------------
; Interrupt Service Routines
; ----------------------------------------------------------------

%assign i 0
%rep 33
  exportarHandler i
  %assign i i + 1
%endrep

rutinaAtencionInterrupcionBasica 0, msgisr0, msgisr0_len
rutinaAtencionInterrupcionBasica 1, msgisr0, msgisr0_len
rutinaAtencionInterrupcionBasica 2, msgisr0, msgisr0_len
rutinaAtencionInterrupcionBasica 3, msgisr0, msgisr0_len
rutinaAtencionInterrupcionBasica 4, msgisr0, msgisr0_len
rutinaAtencionInterrupcionBasica 5, msgisr0, msgisr0_len
rutinaAtencionInterrupcionBasica 6, msgisr0, msgisr0_len
rutinaAtencionInterrupcionBasica 7, msgisr0, msgisr0_len
rutinaAtencionInterrupcionBasica 8, msgisr0, msgisr0_len
rutinaAtencionInterrupcionBasica 9, msgisr0, msgisr0_len
rutinaAtencionInterrupcionBasica 10, msgisr0, msgisr0_len
rutinaAtencionInterrupcionBasica 11, msgisr0, msgisr0_len
rutinaAtencionInterrupcionBasica 12, msgisr0, msgisr0_len
rutinaAtencionInterrupcionBasica 13, msgisr0, msgisr0_len
rutinaAtencionInterrupcionBasica 14, msgisr0, msgisr0_len
rutinaAtencionInterrupcionBasica 15, msgisr0, msgisr0_len
rutinaAtencionInterrupcionBasica 16, msgisr0, msgisr0_len
rutinaAtencionInterrupcionBasica 17, msgisr0, msgisr0_len
rutinaAtencionInterrupcionBasica 18, msgisr0, msgisr0_len
rutinaAtencionInterrupcionBasica 19, msgisr0, msgisr0_len
rutinaAtencionInterrupcionBasica 20, msgisr0, msgisr0_len
rutinaAtencionInterrupcionBasica 21, msgisr0, msgisr0_len
rutinaAtencionInterrupcionBasica 22, msgisr0, msgisr0_len
rutinaAtencionInterrupcionBasica 23, msgisr0, msgisr0_len
rutinaAtencionInterrupcionBasica 24, msgisr0, msgisr0_len
rutinaAtencionInterrupcionBasica 25, msgisr0, msgisr0_len
rutinaAtencionInterrupcionBasica 26, msgisr0, msgisr0_len
rutinaAtencionInterrupcionBasica 27, msgisr0, msgisr0_len
rutinaAtencionInterrupcionBasica 28, msgisr0, msgisr0_len
rutinaAtencionInterrupcionBasica 29, msgisr0, msgisr0_len
rutinaAtencionInterrupcionBasica 30, msgisr0, msgisr0_len
rutinaAtencionInterrupcionBasica 31, msgisr0, msgisr0_len

; Rutina de atencion de interrupcion para el clock
_isr32:
	pushad
	inc DWORD [isrnumero]
	mov ebx, [isrnumero]
	cmp ebx, 0x4
	jl .ok
		mov DWORD [isrnumero], 0x0
		mov ebx, 0
	.ok:
		add ebx, isrmessage1
		mov edx, isrmessage
		IMPRIMIR_TEXTO edx, 6, 0x0A, 23, 1, 0x13000
		IMPRIMIR_TEXTO ebx, 1, 0x0A, 23, 8, 0x13000
	popad
	
	mov al, 0x20
	out 0x20, al
	mov al, 0x20
	out 0x20, al

	iret

isrmessage: db 'Clock:'
isrnumero: dd 0x00000000
isrmessage1: db '|'
isrmessage2: db '/'
isrmessage3: db '-'
isrmessage4: db '\'
