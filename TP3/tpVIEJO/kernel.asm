BITS 16

%include "macrosmodoreal.mac"
%include "macroInterrupciones.mac"
%define colorFondo 	0x3000
%define colorBorde 	0x7000
%define colorMensaje	0x0c

global start
extern GDT_DESC
extern gdt;
extern IDT_DESC
extern idtFill
extern tsss;
extern idt;

%assign i 0
%rep 33
  importarHandler i 					;macro que importa los handlers de las interrupciones de la 0x00 hasta la 0x20
  %assign i i + 1 
%endrep

start:
		cli					;deshabilito las interrupciones
		jmp 	bienvenida

;mensajes
iniciando: db 'Iniciando el kernel ORGA 2 por favor espere...'
iniciando_len equ $ - iniciando		
grupo db "Hola desde el grupo SEGMENTATION FAULT", 0
grupo_len equ $-grupo	

bienvenida:
		IMPRIMIR_MODO_REAL iniciando, iniciando_len, 0x07, 0, 0
	
		; ********************  Ej 1  **********************************
 		
		call enable_A20 		;habilito la A20
		cli 				;deshabilito las interrupciones

		LGDT [GDT_DESC] 
		
		mov eax,cr0 			;lee el registro de control 0.
		or al,1  			;setea el bit PE Protection Enable.
		mov cr0,eax 			;pasa a modo protegido.
		jmp 0x08:modo_protegido

BITS 32
modo_protegido:

		mov 	ax, 0x10		;me muevo 16 para el segmento de datos de la GDT
		mov 	ds, ax			;acomodo el segmento de datos antes de hacer lio
		mov 	ax, 0x18		;me muevo a 24 para el segmento de video de la GDT
		mov 	es, ax			;acomodo el segmento de video antes de hacer lio
		mov 	fs, ax			;acomodo el segmento de video antes de hacer lio
		mov 	gs, ax			;acomodo el segmento de video antes de hacer lio
		mov	ss, ax			;acomodo el segmento de video antes de usarla
		
		mov 	ecx, (25 * 80)		;Calculo la dimension de la pantalla
		xor 	edi, edi		;Vacio edi
		xor 	ebx, ebx		;Vacio edx
		mov 	ebx, 80
		
		ciclocolor:
			mov eax, ecx		;Guardo la posicion actual para realizar las cuentas
			cmp ecx, 80		;Borde inferior
			jbe borde
		
			cmp ecx, 1920   	;Borde superior
			jg borde
			
			xor edx, edx
			div ebx			;Divido por 80 para ver si pertenece al borde
			cmp edx, 0		;Comparo contra cero para ver si es el borde izquierdo (o sea divisible por 80)
			je borde	
			
			mov eax, ecx		;Guardo la posicion actual para realizar las cuentas
			dec eax			;Decremento en 1 la posicion actual para ver si pertenece al borde derecho
			xor edx, edx
			div ebx			;Divido por 80 para ver si pertenece al borde
			cmp edx, 0		;Comparo contra cero para ver si es el borde derecho (o sea posicion actual - 1  divisible por 80)
			je borde	
			
			mov ax, colorFondo	;Pongo el color fondo
			jmp almacenar
		borde:	
			mov ax, colorBorde	;Pongo el color borde
		almacenar:
			stosw			;Imprimo el color
			loop 	ciclocolor
		
		
		; ********************  Ej 2  **********************************
		
 		mov esp, 0x7FFC			;El stack pointer para poder hacer el call debe estar situado en una direccion valida
		call page_init			;esto me inicializa el directorio
		mov eax, page_dir		;cargo la direccion del directorio en cr3
		mov cr3, eax
	
		mov eax, cr0				
		or  eax, 0x80000000		;habilito paginacion
		mov cr0, eax
		
		
		mov ax, 0x18 			; Entramos por el segmento de video, con base
		mov es, ax			; en 0xB8000
		mov ecx, grupo_len
		mov ah, colorMensaje		; pongo el color del mensaje a mostrar
		mov esi, grupo
		xor edi, edi
		add edi, 81 * 2			; ubicamos el texto en la pantalla
		.ciclo:
			lodsb 			; usa ds:esi
			stosw 			; usa es:edi
			loop .ciclo
		
		
		; ********************  Ej 3  **********************************
 		
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

		mov eax, idt	 	;eax es mi puntero a la IDT tabla de descriptores de interrupcion
		%assign i 0
		%rep 33
		  llenarTablaInt i
		  %assign i i + 1
		%endrep 		;con este macro lleno la tabla de descriptores de interrupciones
		
 		lidt [IDT_DESC] 	;cargo en el IR la direccion a la tabla de descriptores de interrupciones
		sti			;inicializo las interrupciones
				
		jmp $

	; ********************  Ej 4  **********************************
 		
		; TODO: Inicializar las TSS
		
		; TODO: Inicializar correctamente los descriptores de TSS en la GDT
		
		; TODO: Cargar el registro TR con el descriptor de la GDT de la TSS actual
		
		; TODO: Habilitar la PIC
		
		; TODO: Habilitar Interrupciones
		
		; TODO: Saltar a la primer tarea
		
		
%include "a20.asm"
%include "kernelPaging.asm"

%define TASK1INIT 0x8000
%define TASK2INIT 0x9000
%define KORG 0x1200

TIMES TASK1INIT - KORG - ($ - $$) db 0x00
incbin "pintor.tsk"
incbin "traductor.tsk"
