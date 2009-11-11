BITS 16

%include "macrosmodoreal.mac"

global start
extern GDT_DESC
extern gdt;
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


bienvenida:
	IMPRIMIR_MODO_REAL iniciando, iniciando_len, 0x07, 0, 0
	; Ejercicios AQUI

		;Habilitar A20
		call 	enable_A20
		call	check_A20
		cli
		
	; Ejercicio 1
		
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

	; Ejercicio 2
		
		; TODO: Habilitar paginacion
	

	;;; INTS!!!!
	;Pic 1
	mov al, 0x11
	out 0x20,al
	mov al,0x8
	out 0x21,al
	mov al,0x04
	out 0x21,al
	mov al, 0x01
	out 0x21,al
	mov al,0xFF
	out 0x21,al

	;Pic 2
	mov al,0x11
	out 0xa0,al
	mov al,0x70
	out 0xa1,al
	mov al,0x02
	out 0xa1,al
	mov al, 0x01
	out 0xa1,al
	
	lidt [IDT_DESC]

	xor eax,eax
	div eax
	;;; INTS!!!!

	jmp $
	
	; Ejercicio 3
	
		; TODO: Inicializar la IDT
		
		; TODO: Resetear la pic
		
		; TODO: Cargar el registro IDTR
				
	; Ejercicio 4
	
		; TODO: Inicializar las TSS
		
		; TODO: Inicializar correctamente los descriptores de TSS en la GDT
		
		; TODO: Cargar el registro TR con el descriptor de la GDT de la TSS actual
		
		; TODO: Habilitar la PIC
		
		; TODO: Habilitar Interrupciones
		
		; TODO: Saltar a la primer tarea
		
		
%include "a20.asm"

%define TASK1INIT 0x8000
%define TASK2INIT 0x9000
%define KORG 0x1200

TIMES TASK1INIT - KORG - ($ - $$) db 0x00
incbin "pintor.tsk"
incbin "traductor.tsk"
