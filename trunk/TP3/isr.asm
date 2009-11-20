BITS 32
%include "macrosmodoprotegido.mac"
extern pic1_intr_end

; ----------------------------------------------------------------
; Interrupt Service Routines
; TODO: Definir el resto de las ISR
; ----------------------------------------------------------------

%macro RAIMensaje 1
global _isr%1
_isr%1:
	mov edx, msgisr%1
	IMPRIMIR_TEXTO edx, msgisr%1_len, 0x0C, 0, 0, 0x13000
	jmp $
%endmacro

msgisr0: db 'EXCEPCION: Division por cero'
msgisr0_len equ $-msgisr0

msgisr1: db 'EXCEPCION: Reservada'
msgisr1_len equ $-msgisr1

msgisr2: db 'EXCEPCION: Interrupcion NMI'
msgisr2_len equ $-msgisr2

msgisr3: db 'EXCEPCION: Breakpoint'
msgisr3_len equ $-msgisr3

msgisr4: db 'EXCEPCION: Overflow'
msgisr4_len equ $-msgisr4

msgisr5: db 'EXCEPCION: Rango de limite excedido'
msgisr5_len equ $-msgisr5

msgisr6: db 'EXCEPCION: Codigo de operacion no valido'
msgisr6_len equ $-msgisr6

msgisr7: db 'EXCEPCION: FPU no disponible'
msgisr7_len equ $-msgisr7

msgisr8: db 'EXCEPCION: Doble falta'
msgisr8_len equ $-msgisr8

msgisr9: db 'EXCEPCION: Excepcion del co-procesador'
msgisr9_len equ $-msgisr9

msgisr10: db 'EXCEPCION: TSS no valida'
msgisr10_len equ $-msgisr10

msgisr11: db 'EXCEPCION: Segmento no presente'
msgisr11_len equ $-msgisr11

msgisr12: db 'EXCEPCION: Stack-segment fault'
msgisr12_len equ $-msgisr12

msgisr13: db 'EXCEPCION: General Protection Fault'
msgisr13_len equ $-msgisr13

msgisr14: db 'EXCEPCION: Page Fault'
msgisr14_len equ $-msgisr14

msgisr15: db 'EXCEPCION: Reservada'
msgisr15_len equ $-msgisr15

msgisr16: db 'EXCEPCION: Error de FPU'
msgisr16_len equ $-msgisr16

msgisr17: db 'EXCEPCION: Alineamiento'
msgisr17_len equ $-msgisr17

msgisr18: db 'EXCEPCION: #MC'
msgisr18_len equ $-msgisr18

msgisr19: db 'EXCEPCION: Excepcion de punto flotante (SIMD)'
msgisr19_len equ $-msgisr19

%assign i 0
%rep 20
  RAIMensaje i
  %assign i i + 1
%endrep

; Rutina de atencion de interrupcion para el clock
global _isr32
_isr32:
	pushad
	call next_clock	
	mov al, 0x20
	out 0x20, al
	mov al, 0x20
	out 0x20, al
	popad
	iret

global _isr33
_isr33:
	cli
	pushad
	mov al,0x20
	out 0x20,al
	popad
	sti
	iret


; Funcion para dibujar el reloj.
; void next_clock(void)
next_clock:
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
	ret

isrmessage: db 'Clock:'
isrnumero: dd 0x00000000
isrmessage1: db '|'
isrmessage2: db '/'
isrmessage3: db '-'
isrmessage4: db '\'
