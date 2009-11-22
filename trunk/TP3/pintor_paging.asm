page_table_0_pintor:
;/////////////////////////
%assign i 0x0000			; cada pag es supervisor, read/write, present
%rep    0x009
	
    dd 	i | 3
				
%assign i i+4096 
%endrep
;/////////////////////////
%assign i 0x9000			
%rep    0x005				;TEORICAMENTE TENDRIA Q LLENAR HASTA DFFF CON CEROS, NO ESTAMOS SEGUROSSSSSSSSSSSSSSSSSSSSS
	
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
