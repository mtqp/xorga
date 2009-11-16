BITS 32
page_init:	;inicializo la primer entrada del directorio con la direccion de la tabla
	pushad
	mov eax, page_table_0
	or 	eax, 0x3		;supervisor, read/write, present
	mov [page_dir], eax
	popad
	ret

ALIGN 0xE00	;Y este numero? Bueno, esto tiene que estar alineado a 0x1000 (4096) bytes
			;El align alinea a la direccion de inicio de la seccion. (MANUAL NASM)
			;Como el ORG es 0x1200 y hay que alinear a 0x1000, nos sobra 0x200.
			;Tenemos que decirle al NASM que lo haga a 0x1000 - 0x200 = 0xE00
			;0x0E00 + 0x1200 = 0x2000
			;0x1E00 + 0x1200 = 0x3000 ... justo como lo queriamos
page_dir:
	dd 	0x00000000
	
%rep	0x400 - 1
	dd	0x00000002		;supervisor, read/write, not present
%endrep

page_table_0:
;/////////////////////////
%assign i 0x0000			; cada pag es supervisor, read/write, present
%rep    0x008
	
    dd 	i | 3
				
%assign i i+4096 
%endrep
;/////////////////////////
%assign i 0x8000			; cada pag es supervisor, read/write, present
%rep    0x001
	
    dd 	0 | 0
				
%assign i i+4096 
%endrep
;/////////////////////////
%assign i 0x9000			; cada pag es supervisor, read/write, present
%rep    0x002
	
    dd 	i | 3
				
%assign i i+4096 
%endrep
;/////////////////////////
%assign i 0x11000
%rep 0x002

	dd 0 | 0

%assign i i+4096
%endrep
;/////////////////////////
%assign i 0x13000
%rep 0x001

	dd 0xb8000 | 3

%assign i i+4096
%endrep
;/////////////////////////
%assign i 0x14000
%rep 0x002

	dd 0 | 0

%assign i i+4096
%endrep
;/////////////////////////
%assign i 0x16000
%rep 0x001

	dd i | 3

%assign i i+4096
%endrep
;/////////////////////////
%assign i 0x17000
%rep 0x001

	dd 0 | 0

%assign i i+4096
%endrep
;/////////////////////////
%assign i 0x18000
%rep 0x001

	dd i | 3

%assign i i+4096
%endrep
;/////////////////////////
%assign i 0x19000
%rep 0x087		;son 87 decimal de paginas se q corre

	dd 0 | 0

%assign i i+4096
%endrep
;/////////////////////////
%assign i 0xA0000
%rep 0x020

	dd i | 3

%assign i i+4096
%endrep
;////////////////////////
