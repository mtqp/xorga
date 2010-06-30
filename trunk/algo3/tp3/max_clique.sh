#pequeño script para ejecutar todos los algoritmos!

if [ ! -d out/ ]; then
	mkdir out #if no esta creado crearlo....
	echo "Generando carpeta OUT."
fi

cd exacto
if [ ! -e "./exacto" ]; then
	make
fi
./exacto < ../$1 > ../out/a.out
echo "Exacto 		procesó entrada."

cd ..
cd constructivo
if [ ! -e "./constructivo" ]; then
	make
fi
./constructivo < ../$1 > ../out/const.out #corregir los nombres q estan como la mona
echo "Constructivo 	procesó entrada."

cd ..
cd busqueda_local
if [ ! -e "./busqueda_local" ]; then
	make
fi
./busqueda_local < ../$1 > ../out/busq_lo.out
echo "Búsqueda Local  procesó entrada."

cd ..
cd tabu_search
if [ ! -e "./tabu_search" ]; then
	make
fi
./tabu_search < ../$1 > ../out/tabu.out
echo "Tabú Search 	procesó entrada."
