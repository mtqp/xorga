#include "isr.h"
#include "idt.h"


#define IDT_ENTRY(numero) \
	idt[numero].offset_0_15 = (unsigned short) ((unsigned int)(&_isr ## numero) & (unsigned int) 0xFFFF); \
	idt[numero].segsel = (unsigned short) 0x0008; \
	idt[numero].attr = (unsigned short) 0x8E00; \
	idt[numero].offset_16_31 = (unsigned short) ((unsigned int)(&_isr ## numero) >> 16 & (unsigned int) 0xFFFF);\

/*
 * Metodo para inicializar los descriptores de la IDT 
 */
void idtFill() {
    
    IDT_ENTRY(0);
    IDT_ENTRY(1);
    IDT_ENTRY(2);
    IDT_ENTRY(3);
    IDT_ENTRY(4);
    IDT_ENTRY(5);
    IDT_ENTRY(6);
    IDT_ENTRY(7);
    IDT_ENTRY(8);
    IDT_ENTRY(9);
    IDT_ENTRY(10);
    IDT_ENTRY(11);
    IDT_ENTRY(12);
    IDT_ENTRY(13);
    IDT_ENTRY(14);
    IDT_ENTRY(15);
    IDT_ENTRY(16);
    IDT_ENTRY(17);
    IDT_ENTRY(18);
    IDT_ENTRY(19);
    IDT_ENTRY(20);
    IDT_ENTRY(21);
    IDT_ENTRY(22);
    IDT_ENTRY(23);
    IDT_ENTRY(24);
    IDT_ENTRY(25);
    IDT_ENTRY(26);
    IDT_ENTRY(27);
    IDT_ENTRY(28);
    IDT_ENTRY(29);
    IDT_ENTRY(30);
    IDT_ENTRY(31);
    IDT_ENTRY(32);
}
/*
 * IDT
 */ 
idt_entry idt[255] = {};

/*
 * Descriptor de la IDT (para cargar en IDTR)
 */
idt_descriptor IDT_DESC = {sizeof(idt)-1, (unsigned int)&idt};

