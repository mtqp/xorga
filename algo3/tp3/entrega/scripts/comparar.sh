#!/usr/bin/bash
# Script que compara dos métodos y grafica
# las instancias diferentes
# 	Requiere estar en la carpeta principal
#
# uso: ./comparar.sh {input} {metodo1} {metodo2}
#
ENTRADA=$1
BASENAME=${ENTRADA##*/}
NOMBRE=${BASENAME%.*}
cd $2
echo "Buscando cliques"
make test
cd ..
cd $3
make test
cd ..
echo "Calculando diferencias"
./diferencias.py $1 $2 $3
echo "$2/$NOMBRE.dot"
dot -Tps $2/$NOMBRE.dot > $2/$NOMBRE.eps
echo "Generado $2/$NOMBRE.eps"
dot -Tps $3/$NOMBRE.dot > $3/$NOMBRE.eps
echo "Generado $3/$NOMBRE.eps"
rm $2/$NOMBRE.dot
rm $3/$NOMBRE.dot
