;BITS 32
;page_init_pintor:	;inicializo la primer entrada del directorio con la direccion de la tabla
;	pushad
;	mov eax, page_table_0_pintor
;	or 	eax, 0x3		;supervisor, read/write, present
;	mov [page_dir_pintor], eax
;	popad
;	ret

;ALIGN 0xE00	;Y este numero? Bueno, esto tiene que estar alineado a 0x1000 (4096) bytes
			;El align alinea a la direccion de inicio de la seccion. (MANUAL NASM)
			;Como el ORG es 0x1200 y hay que alinear a 0x1000, nos sobra 0x200.
			;Tenemos que decirle al NASM que lo haga a 0x1000 - 0x200 = 0xE00
			;0x0E00 + 0x1200 = 0x2000
			;0x1E00 + 0x1200 = 0x3000 ... justo como lo queriamos

page_table_0_pintor:
;/////////////////////////
%assign i 0x0000			; cada pag es supervisor, read/write, present
%rep    0x009
	
    dd 	i | 3
				
%assign i i+4096 
%endrep
;/////////////////////////
%assign i 0x9000			; cada pag es supervisor, read/write, present
%rep    0x004
	
    dd 	0 | 0
				
%assign i i+4096 
%endrep
;/////////////////////////
%assign i 0xE000			; cada pag es supervisor, read/write, present
%rep    0x002
	
    dd 	i | 3
				
%assign i i+4096 
%endrep
;/////////////////////////
%assign i 0x10000
%rep 0x003

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
%rep 0x001

	dd 0 | 0

%assign i i+4096
%endrep
;/////////////////////////
%assign i 0x15000
%rep 0x001

	dd i | 3

%assign i i+4096
%endrep
;/////////////////////////
%assign i 0x16000
%rep 0x0A2

	dd 0 | 0

%assign i i+4096
%endrep
;/////////////////////////
%assign i 0xB8000
%rep 0x001

	dd 0x10000 | 3

%assign i i+4096
%endrep
