section .data
	;sobel_mask: DW 1,2,1,1,2,1,1,2,1
	sobel_X_linea1: DW -1,0,1,-1,0,1,-1,0
section .text
	global asmSobel

asmSobel:
	convencion_C 
					;no hay alineamiento xq es mult 4 imagen
	%define line		[ebp-4]
	%define align		4
	%define ptr_src		[ebp+8]
	%define ptr_dst		[ebp+12]
	%define width		[ebp+16]
	%define height		[ebp+20]

	xor ecx, ecx			; ecx = 0 [contador de columnas]
	mov esi, ptr_src		; puntero al inicio de línea src
	mov edi, ptr_dst		; puntero al inicio de línea dst

	pxor xmm7,xmm7			;acumulador parte baja
	pxor xmm6,xmm6			;acumulador parte alta
	movdqu xmm0,[esi]		;levanto la 1er linea
	movdqu xmm1,xmm0
	punpcklbw xmm0,xmm7		;tengo las primeras ocho words bajas
	punpckhbw xmm1,xmm6		;tengo las 1ras ocho words altas
				
	;TRABAJO PRIMERO CON PARTE BAJA, PARTE ALTA ANALOGA
	movdqu xmm2,xmm0		;salvo x un momento la matriz
	movdqu xmm5,[sobel_x_linea1]

	pumullw xmm0,xmm5		;[o,B,o,o,E,o,o,o]
					;b<-a+b+c,E<-d+e+f (b y e son ceros!)
	shufALGO xmm3,xmm0,imm8	;xmm3 == [C,fruta,fruta,F,fruta,fruta,gdaf,graf]
				;xmm0 == [A,frefr,frefr,D,frefr,frefr,fref,fref]
	paddw xmm0,xmm3		;xmm0 == [B,fruta,fruit,E,fruit,fruit,fruit,fru]
	shufALGO xmm0,xmm0,imm8;;xmm0 == [frut,B,fruit,fruto,E,fruti,frtu,friut]
	pandw xmm0,mask_borraFRUTA	;xmm0 == [0,B,0,0,E,0,0,0]
	paddw xmm7,xmm0			;xmm7 == xmm0
					
	movdqu xmm0,xmm2
	shiftDOUBLEQUADR xmm5,unaWORD	;xmm5 == [0,-1,0,1,-1,0,1,-1]
	pmullw xmm0,xmm7		;[o,o,C,o,o,F,o,o]
					;C<-b+c+d,F<-e+f+g	(c y f == 0)
	shufALGO xmm3,xmm0,newimm8	;xmm0 == [-,b,-,-,e,-,-,-]
					;xmm3 == [-,d,-,-,g,-,-,-]
	paddw xmm0,xmm3			;xmm0 == [-,C,-,-,F,-,-,-]
	shufALGONEW xmm0,xmm0,imm8	;xmm0 == [-,-,C,-,-,F,-,-]
	pandw xmm0,msk_borrarFRUTNEW	;xmm0 == [0,0,C,0,0,F,0,0]
	paddw xmm7,xmm0			;xmm7 == [0,b,c,0,e,f,0,0]
					
	movdqu xmm0,xmm2
	shiftDOUBLEQUADR xmm5,otraW	;xmm5 ==  [0,0,-1,0,1,-1,0,1]
	pmull xmm0,xmm7
	shufALGO xmm3,xmm0,imm8
	paddw xmm0,xmm3
	pshufalgo2 xmm0,xmm0,imm8
	pandw xmm0,msk_frutenborren
	paddw xmm7,xmm0			;xmm7 == [0,b,c,d,e,f,g,0]
					
				;falta ver mas signific de la parte baja
				;y el menos sig de la parte alta
(+sigPBaja)^(-sigPAlta):		;en xmm1 estan las ocho words altas
	movdqu xmm0,xmm2
	pandw xmm0,DEJARDOSULTIMOS	;xmm0 == [0,0,0,0,0,0,g,h]
	shiftDOUBLEQL xmm0,16		;xmm0 == [0,0,0,0,0,g,h,0]
	mov xmm2,xmm1			;xmm2 == parteALTA
	pandw xmm2,mk_dejarAB		;xmm2 == [aH,bH,0,0,0,0,0,0]
	paddq xmm0,xmm2			;xmm0 == [aH,bH,0,0,0,g,h,0]
	shufALGO xmm0,xmm0,imm8		;xmm0 == [h,aH,bH,0,0,g,h,aH]
	shufAGLO xmm3,xmm0,imm8		;xmm3 == [BH,f,ff,f,f,AH,f,f]
	pandw xmm0,xmm3			;xmm0 == [aH,f,ff,f,f,H,ff,f]
	movdqu xmm6,xmm0
	pandw xmm6,ffff0000000000	;xmm6 == [aH,0,0,0,0,0,0,0]
	pshufALGO xmm0,xmm0,imm8	;xmm0 == [0,0,0,0,0,0,0,h]
	paddw xmm7,xmm0			;xmm7 == [0,b,c,d,e,f,g,h]
	
	////HABRIA Q HACER LO MISMO PARA LA PARTE ALTA/////
	////ESTO ES SOLO UNA LINEA, MEDIO Q ES UN TOQUE INEFICIENTE///
	///FALTAN HACER LAS DOS LINEAS SIGUIENTES DE LA MATRIZ///
for(¬termineImagen?(ptr_src)){
	for(i=0;i<tam_linea&&¬(ultimaLevantadaLinea);i+=14){
		levantarLinea1(ptr_src+i)
		procesarLinea1(ptr_src+i,ordenDeriv),
		acumularenXMM7/XMM6
		levantarLinea2(ptr_src+i)
		procesarLinea2(ptr_src+i,ordenDeriv),
		acumularenXMM7/XMM6
		levantarLinea3(ptr_src+i),
		procesarLinea3(ptr_src+i,ordenDeriv),
		acumularenXMM7/XMM6
		enviarAMemoria(registroAcumulado,ptr_dst)
	}
}
		
bool termineImagen(ptr_src){
	return apuntaFinDeImagen?(ptr_src)
}
		
levantarLineaX(ptr_src){
	if x==1 then
		movdqu xmm0,[ptr_src]
	if x==2 then
		movdqu xmm0,[ptr_src+line]
	if x==3 then
		movdqu xmm0,[ptr_src+2*line]

	desempacar xmm0			;parte baja en xmm0 y parte alta en xmm1
}
		
bool ultimaLevantadaLinea(ptr_src,ptr_dst,line,i,ordenDeDeriv){
	int res = (line-i<14);
	if (res){
		mov eax,ptr_src
		add eax,line
		sub eax,16
		while(j=0,j<=3,j+=1{
			levantarLineaJ(eax)
			procesarLineaJ(eax,ordenDeDeriv)
			acumularEnXMM7/XMM6
		}
	}
	ptr_src = ptr_src+line
	return res;
}
		
procesarLineaX(ptr_src,ordenDeDeriv){
	if X==1 then {
		if(ordenDeDeriv==x){	;ojo q si es X e Y hay q refrescar xmm0 y xmm1
			levantarMatriz -1,0,1
			while(!procese8?){
				pmullw xmm0,matriz
				sumar-101entreSi
				acomodarDosPxProcesados
				acumularEnXMM7
				shiftearMatriz
			}
			levantarMatriz -1,0,1
			while(!procese6PAlta??){
				pmullw xmm1,matriz
				sumar-101entreSi
				acomodarDosPxProcesados
				acumularEnXMM6
				shiftearMatriz
			}
			saturarAByte xmm7
			saturarAByte xmm6
			merge xmm6,xmm7
		}
		if(ordenDeDeriv==y){
			levantarMatriz -1-2-1
			while(!procese8?){
				pmullw xmm0,matriz
				sumar-1-2-1entreSi
				acomodarDosPxProcesados
				acumularEnXMM5
				shiftearMatriz
			}
			levantarMatriz -1-2-1
			while(!procese6PAlta?){
				pmullw xmm1,matriz
				sumar-1-2-1entreSi
				acomodarDosPxProcesados
				acumularEnXMM4
				shiftearMatriz
			}
			saturarAByte xmm5
			saturarAByte xmm4
			merge xmm4,xmm5
		}
		if(ordenDeDeriv == x + y){
			merge xmm4,xmm6
		}
		acumular en un registro q no se use
	}
	if X==2 then
		lo mismo con la segunda linea de la matriz X e Y
	if X==3 then
		lo mismo con la tercer linea de la matriz X e Y
}
			
	convencion_C_fin
