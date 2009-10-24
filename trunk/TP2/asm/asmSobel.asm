%include "macros.asm"

extern apply_lineX	;///void apply_lineX(*src,int selector_linea);	DEJA EN XMM7 res

	
section .data
	acum_lineas : DQ 0,0
	acum_filas : DD 0

section .text
	global asmSobel

asmSobel:
	convencion_C 
					;no hay alineamiento xq es mult 4 imagen
	%define ptr_src		[ebp+8]
	%define ptr_dst		[ebp+12]
	%define width		[ebp+16]
	%define height		[ebp+20]
	%define xorder		[ebp+24]
	%define yorder 		[ebp+28]
	
	xor ecx, ecx			; ecx = 0 [contador de columnas]
	mov esi, ptr_src		; puntero al inicio de línea src
	mov edi, ptr_dst		; puntero al inicio de línea dst
	add edi,width			;xq 1er linea va en negro
	mov eax,xorder			;nunca ambos tienen q ser cero!
	mov ebx,yorder			;xq x implement haria el deriv_xy
	cmp eax,ebx
	je deriv_xy			;nunca deberia pasarse como param 0, 0
	jg deriv_y
;//////////////////////////////////////////////
deriv_x:

ciclo_xorder:
;////////			procesa primer linea
	push esi
	push 0			;para q procese con -101-101
	call apply_lineX
	add esp,8
	mov ebx,acum_lineas
	movdqu [ebx],xmm7	;acumulo linea uno
;////////			procesa segunda linea
	mov eax,esi
	add eax,width
	push eax
	push 1			;para q procese con -202-202
	call apply_lineX
	add esp,8
	movdqu xmm1,[ebx]
	paddsb xmm1,xmm7	;acumulo linea1+linea2
	movdqu [ebx],xmm1
;////////			procesa tercer linea
	shl eax,1
	push eax
	push 0
	call apply_lineX
	add esp,8
	movdqu xmm1,[ebx]
	paddsb xmm1,xmm7	;ya tengo los 14 bytes procesados
				;debo mandar a memoria ahora
	movdqu [edi],xmm1	;primera tira YA en dst!!!!!
	
	add ecx,14
	cmp ecx,width
	je nueva_linea		;qdo justa
suficiente_linea:
	mov edx,ecx
	add edx,14		;si me alcanza para laburar tendria q dar +
	cmp edx,width		;
	
	jl no_tengo_16		;=>NO llegue al borde!
tengo_16:
	add esi,14
	jmp ciclo_xorder
nueva_linea:
	add esi,16
	jmp termine_img
no_tengo_16:
	mov edx,width
	sub edx,16
	mov esi,ptr_src
	add esi,edx
termine_img:
	mov eax,acum_filas
	inc dword [eax]
	mov eax,[eax]
	mov ebx,height
	cmp dword eax,ebx
	je fin_procesar
	xor ecx,ecx
	jmp ciclo_xorder
;///////////////////////////////////////////////
deriv_y:
ciclo_yorder:
	jmp ciclo_yorder
;CONSIDERACIONES:
	;no se puede usar apply_lineaX en deriv_y ya que como en derivacion X
	;tenes -101, x lo tanto elimine la multiplicacion x cero
	;en cambio estas matrices son 121, hay que hacer la suma de los tres
	;y saturar... HARCODEAR nos va a hacer hacer mil codigos ligeramente !=

;//////////////////////////////////////////////
deriv_xy:
ciclo_xyorder:
;CONSIDERACIONES:
	;aca pueden llamarse a las dos apply_linea (X e Y), acumular el res
	;de cada una en algun lugar y mandar a memoria, NO es muy dificil
	jmp ciclo_xyorder
;///////////////////////////////////////////////
fin_procesar:
	convencion_C_fin
	ret
;////////////////////////////////////////////////

;for(¬termineImagen?(ptr_src)){
;	for(i=0;i<tam_linea&&¬(ultimaLevantadaLinea);i+=14){
;		levantarLinea1(ptr_src+i)
;		procesarLinea1(ptr_src+i,ordenDeriv),
;		acumularenXMM7/XMM6
;		levantarLinea2(ptr_src+i)
;		procesarLinea2(ptr_src+i,ordenDeriv),
;		acumularenXMM7/XMM6
;		levantarLinea3(ptr_src+i),
;		procesarLinea3(ptr_src+i,ordenDeriv),
;		acumularenXMM7/XMM6
;		enviarAMemoria(registroAcumulado,ptr_dst)
;	}
;}
		
;bool termineImagen(ptr_src){
;	return apuntaFinDeImagen?(ptr_src)
;}
		
;levantarLineaX(ptr_src){
;	if x==1 then
;		movdqu xmm0,[ptr_src]
;	if x==2 then
;		movdqu xmm0,[ptr_src+line]
;	if x==3 then
;		movdqu xmm0,[ptr_src+2*line]
;
;	desempacar xmm0			;parte baja en xmm0 y parte alta en xmm1
;}
		
;bool ultimaLevantadaLinea(ptr_src,ptr_dst,width,i,ordenDeDeriv){
;	int res = (width-i<14);
;	if (res){
;		mov eax,ptr_src
;		add eax,width
;		sub eax,16
;		while(j=0,j<=3,j+=1{
;			levantarLineaJ(eax)
;			procesarLineaJ(eax,ordenDeDeriv)
;			acumularEnXMM7/XMM6
;		}
;	}
;	ptr_src = ptr_src+line
;	return res;
;}
		
;procesarLineaX(ptr_src,ordenDeDeriv){
;	if X==1 then {
;		if(ordenDeDeriv==x){	;ojo q si es X e Y hay q refrescar xmm0 y xmm1
;			levantarMatriz -1,0,1
;			while(!procese8?){
;				pmullw xmm0,matriz
;				sumar-101entreSi
;				acomodarDosPxProcesados
;				acumularEnXMM7
;				shiftearMatriz
;			}
;			levantarMatriz -1,0,1
;			while(!procese6PAlta??){
;				pmullw xmm1,matriz
;				sumar-101entreSi
;				acomodarDosPxProcesados
;				acumularEnXMM6
;				shiftearMatriz
;			}
;			saturarAByte xmm7
;			saturarAByte xmm6
;			merge xmm6,xmm7
;		}
;		if(ordenDeDeriv==y){
;			levantarMatriz -1-2-1
;			while(!procese8?){
;				pmullw xmm0,matriz
;				sumar-1-2-1entreSi
;				acomodarDosPxProcesados
;				acumularEnXMM5
;				shiftearMatriz
;			}
;			levantarMatriz -1-2-1
;			while(!procese6PAlta?){
;				pmullw xmm1,matriz
;				sumar-1-2-1entreSi
;				acomodarDosPxProcesados
;				acumularEnXMM4
;				shiftearMatriz
;			}
;			saturarAByte xmm5
;			saturarAByte xmm4
;			merge xmm4,xmm5
;		}
;		if(ordenDeDeriv == x + y){
;			merge xmm4,xmm6
;		}
;		acumular en un registro q no se use
;	}
;	if X==2 then
;		lo mismo con la segunda linea de la matriz X e Y
;	if X==3 then
;		lo mismo con la tercer linea de la matriz X e Y
;}
			
