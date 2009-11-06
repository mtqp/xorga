#include "gdt.h"
#include "tss.h"


/*
 * Definicion de la GDT
 */
gdt_entry gdt[GDT_COUNT] = {
	/* Descriptor nulo*/
	(gdt_entry){ 
		(unsigned short) 0x0000, 
		(unsigned short) 0x0000,
		(unsigned char) 0x00, 
		(unsigned char) 0x0, 
		(unsigned char) 0, 
		(unsigned char) 0, 
		(unsigned char) 0, 
		(unsigned char) 0x0000,
		(unsigned char) 0,  
		(unsigned char) 0,  
		(unsigned char) 0,  
		(unsigned char) 0, 
		(unsigned char) 0x00 
	},

	/** CODIGO BASE=0x00000000 LIMIT=0xFFFFFFFF **/
	(gdt_entry){ 
		(unsigned short) 0xFFFF, //limit 0 15
		(unsigned short) 0x0000, // base 0 15
		(unsigned char) 0x00,   // base 23 16
		(unsigned char) 0xA,    //type
		(unsigned char) 1, 	//s -> 1=code/data 0=system
		(unsigned char) 0x00, 	//dpl -> privilegio
		(unsigned char) 1, 	//p ->presente
		(unsigned char) 0xF,//limit 16 19
		(unsigned char) 0,  	//avl
		(unsigned char) 0,  	//l ->64bit
		(unsigned char) 1,  	//db
		(unsigned char) 1, 	//g
		(unsigned char) 0x00 	//base 31 24
	},

	/** DATOS BASE=0x00000000 LIMIT=0xFFFFFFFF **/
   	(gdt_entry){ 
		(unsigned short) 0xFFFF,	// limit 0 15
		(unsigned short) 0x0000,	// base 0 15
		(unsigned char) 0x00,		// base 23 16
		(unsigned char) 0x2,		// type
		(unsigned char) 1,		// s -> 1=code/data 0=system
		(unsigned char) 0x00, 		// dpl -> privilegio
		(unsigned char) 1, 		// p -> presente
		(unsigned char) 0xF,		// limit 16 19
		(unsigned char) 0,  		// avl
		(unsigned char) 0,  		// l -> 64bit
		(unsigned char) 1,  		// db
		(unsigned char) 1, 		// g
		(unsigned char) 0x00 		// base 31 24
	},	

	/** VIDEO BASE=0x000B8000 LIMIT=0x00001000 **/
    	(gdt_entry){ 
		(unsigned short) 0x0001,	// Limit 0-15 = 4k = una página
		(unsigned short) 0x8000,	// base 0-15
		(unsigned char) 0x0B,		// base 23-16
		(unsigned char) 0x2, 		// type
		(unsigned char) 1, 		// s-> 1=code/data 0=system
		(unsigned char) 0,		// privilegio de kernel
		(unsigned char) 1, 		// presente
		(unsigned char) 0x0,		// limit 16-19
		(unsigned char) 0,  		// avl
		(unsigned char) 0,  		// 
		(unsigned char) 1,  		//
		(unsigned char) 0, 		//
		(unsigned char) 0x00 		// base 31-24
	}


};

/*
 * Definicion del GDTR
 */ 
gdt_descriptor GDT_DESC = {sizeof(gdt)-1, (unsigned int)&gdt};
