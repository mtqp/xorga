echo "Generando casos"
./generador.py 100 5
cd $2
echo "Generando grafos"
make
make test
#make grafos
cd ..
cd $3
make
make test
#make grafos
cd ..
echo "Calculando diferencias"
./diferencias.py $1 $2 $3
