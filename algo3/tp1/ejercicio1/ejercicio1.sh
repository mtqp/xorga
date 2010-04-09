#!/bin/bash
# Script para ejecutar bn_mod_n fácilmente
# Uso:
# 	ejercicio1 input [output]

if [ $# -eq 2 ]; then
		./bn_mod_n < $1 >> $2
else 
	if [ $# -eq 1 ]; then
		./bn_mod_n < $1
	else
		echo "Uso: ./ejercicio1 input [output]"
	fi
fi
