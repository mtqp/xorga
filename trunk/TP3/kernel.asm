BITS 16

%include "macrosmodoreal.mac"

global start
extern GDT_DESC
extern gdt;
extern idt
extern IDT_DESC
extern idtFill
extern tsss;

;Aca arranca todo, en el primer byte.
start:
		cli					;no me interrumpan por ahora, estoy ocupado
		jmp 	bienvenida

;aca ponemos todos los mensajes		
iniciando: db 'Iniciando el kernel mas inutil del mundo'
iniciando_len equ $ - iniciando		
xorga db "Grupo XOR", 0
xorga_len equ $-xorga

bienvenida:
	IMPRIMIR_MODO_REAL iniciando, iniciando_len, 0x07, 0, 0
	; Ejercicios AQUI

		;Habilitar A20
		call 	enable_A20
		call	check_A20
		cli
	;////////////////////////////////////////////////////////////////		
	;/////////////////////// Ejercicio 1/////////////////////////////
	;////////////////////////////////////////////////////////////////		
		; Cargar el registro GDTR
		lgdt 	[GDT_DESC]
		; Pasar a modo protegido
		mov eax, cr0		;cargo el registro de control cr0
		or eax, 1		;seteo el bit para habilitar modo protegido
		mov cr0, eax		;guardo el registro cr0
		jmp 0x08:modo_protegido	;salto al segmento de codigo

BITS 32
modo_protegido:
	
	mov 	ax, 0x10	;acomodo los registros al segmento de datos
	mov 	ds, ax		
	mov 	es, ax		
	mov 	fs, ax		
	mov 	gs, ax		
	mov 	ss, ax	

	mov 	ax, 0x18	;acomodo el es al segmento de video
	mov 	es, ax
	xor edi,edi		;necesito al edi como offset para el es
		

	;limpio la pantalla 

	mov ecx, (25 * 80) 	;para el loop..
	mov ax, 0x0000		;ax es el par de bytes a escribir en video
	limpiarPantalla:
		stosw		;Escribe ax en es:edi e incrementa solo el edi
		loop limpiarPantalla
	
	xor edi,edi	;restarteo el edi para empezar a hacer el marco
	mov ax,0xee00	;preparo el marco a dibujar

	mov ecx,80      ;80 es el tamanio de cada fila
	marcoSuperior:
		stosw
		loop marcoSuperior
	
	mov ecx, 23	;para las siguientes 23 filas, solo marco en las puntas
	marcoDeCostado:
		stosw	;dibujo la punta izquierda
		add edi,78*2 ;salteo todo el medio
		stosw 	
		loop marcoDeCostado
	
	mov ecx,80      ;80 es el tamanio de cada fila
	marcoInferior:
		stosw
		loop marcoInferior	
	;////////////////////////////////////////////////////////////////
	;///////////////////// Ejercicio 2///////////////////////////////
	;////////////////////////////////////////////////////////////////		
		; TODO: Habilitar paginacion
	
 		mov esp, 0x7FFC			;El stack pointer para poder hacer el call debe estar situado en una direccion valida
		mov ebp, esp			;son los ultimos 32 bytes del kernel, PREGUNTAR SI SE PUEDE PONER CUALQUIER COSA Q ENTRE EN EL KERNEL
		
		call page_init			;esto me inicializa los directorios
		mov eax, page_dir_kernel		;cargo la direccion del directorio del kernel en cr3
		mov cr3, eax	
	
		mov eax, cr0				
		or  eax, 0x80000000		;habilito paginacion
		mov cr0, eax
				
		mov ax, 0x18 			; Entramos por el segmento de video, con base
		mov es, ax			; en 0xB8000
		mov ecx, xorga_len
		mov ah, 0x0A 
		mov esi, xorga
		xor edi, edi
		add edi, 81 * 2			; ubicamos el texto en la pantalla
		.ciclo:
			lodsb 			; usa ds:esi
			stosw 			; usa es:edi
			loop .ciclo
	
	
		;jmp $ ;///////LO CUELGO XQ QUIERO VER SI ME IMPRIME EL NOMBRE DEL GRUPO!!!
		
		
		
	;////////////////////////////////////////////////////////////////
	;///////////////////// Ejercicio 3///////////////////////////////
	;////////////////////////////////////////////////////////////////	
		; TODO: Inicializar la IDT
		
		; TODO: Resetear la pic
		
		; TODO: Cargar el registro IDTR

		;Inicializacion PIC1
 		mov al, 0x11 		;ICW1: IRQs activas por flanco, Modo cascada, ICW4 Si.
 		out 20h, al
		mov al, 20h		;ICW2: INT base para el PIC1 Tipo 8.
		out 21h, al
		mov al, 04h 		;ICW3: PIC1 Master, tiene un Slave conectado a IRQ2
		out 21h ,al
		mov al, 01h 		;ICW4: Modo No Buffered, Fin de Interrupci´n Normal		
		out 21h, al 
		
		;Deshabilitamos las Interrupciones del PIC1
		mov al, 0xFF 		;OCW1: Set o Clearel IMR
		out 21h, al
		
		;Inicializacion PIC2
		mov al, 0x11 		;ICW1: IRQs activas por flanco, Modo cascada, ICW4 Si.
		out 0xA0, al
		mov al, 0x28		;ICW2: INT base para el PIC1 Tipo 070h.
		out 0xA1, al
		mov al, 0x02 		;ICW3: PIC2 Slave, IRQ2 es la linea que envia al Master
		out 0xA1, al
		mov al, 0x01 		;ICW4: Modo No Buffered, Fin de Interrupcion Normal
		out 0xA1, al

		mov al, 0xFF
		out 0xA1,al
		mov al, 0x00
		out 0x21, al
		mov al, 0x00
		out 0xA1, al

		mov eax, idt

		call idtFill	
 		lidt [IDT_DESC] 	;cargo la IDT
 		xchg bx, bx
		sti			;inicializo las interrupciones
				
		jmp $


	;////////////////////////////////////////////////////////////////				
	;///////////////////// Ejercicio 4///////////////////////////////
	;////////////////////////////////////////////////////////////////	
		; TODO: Inicializar las TSS
		
		; TODO: Inicializar correctamente los descriptores de TSS en la GDT
		
		; TODO: Cargar el registro TR con el descriptor de la GDT de la TSS actual
		
		; TODO: Habilitar la PIC
		
		; TODO: Habilitar Interrupciones
		
		; TODO: Saltar a la primer tarea

page_init:	;inicializo la primer entrada del directorio con la direccion de la tabla
	mov eax, page_table_0
	or 	eax, 0x3		;supervisor, read/write, present
	mov [page_dir_kernel], eax
;page_init_pintor:	;inicializo la primer entrada del directorio con la direccion de la tabla	
	mov eax, page_table_0_pintor
	or 	eax, 0x3		;supervisor, read/write, present
	mov [page_dir_pintor], eax
	ret
		
%include "a20.asm"

%define TASK1INIT 0x8000
%define TASK2INIT 0x9000
%define KORG 0x1200

TIMES TASK1INIT - KORG - ($ - $$) db 0x00
incbin "pintor.tsk"
incbin "traductor.tsk"

TIMES 0xA000 - KORG - ($ - $$) db 0x00
page_dir_pintor:
	dd 	0x00000000
	
TIMES (0xB000 - KORG - ($ - $$))/4 dd 0x00000002 ;rellena el directorio con supervisor, read/write, not present

page_dir_kernel:
	dd 	0x00000000

TIMES (0xC000 - KORG - ($ - $$))/4 dd 0x00000002 ;rellena el directorio con supervisor, read/write, not present

%include "pintor_paging.asm"
TIMES 0xD000 - KORG - ($ - $$) db 0x00	; rellena la tabla de páginas del pintor
%include "kernel-traductor_paging.asm"

