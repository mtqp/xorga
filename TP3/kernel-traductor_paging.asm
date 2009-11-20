page_table_0:
;/////////////////////////
%assign i 0x0000			; cada pag es supervisor, read/write, present
%rep    0x008
	
    dd 	i | 3
				
%assign i i+4096 
%endrep
;/////////////////////////
%assign i 0x8000
%rep    0x001
	
    dd 	0 | 0
				
%assign i i+4096 
%endrep
;/////////////////////////
%assign i 0x9000
%rep    0x008
	
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

	dd 0xb8000 | 3

%assign i i+4096
%endrep
;/////////////////////////
%assign i 0x19000
%rep 0x087		
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
%assign i 0xC0000
%rep 0x340

	dd 0 | 0

%assign i i+4096
%endrep
;////////////////////////
