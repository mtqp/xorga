./generador.py
cd $2
make grafos
cd ..
cd $3
make grafos
cd ..
./diferencias.py $1 $2 $3
