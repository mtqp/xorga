#pequeño script para ejecutar todos los algoritmos!

input=$1
input_length=${#input}
echo $input
echo $input_length
output=${input:0:input_length-3}
echo $output

if [ ! -d out/ ]; then
	mkdir out #if no esta creado crearlo....
	echo "Generando carpeta OUT."
fi

cd exacto
if [ ! -e "./exacto" ]; then
	make
fi
exacto_out=${output}"Exacto.out"
./exacto < ../$input > ../out/$exacto_out
echo "Exacto 		procesó entrada."

cd ..
cd constructivo
if [ ! -e "./constructivo" ]; then
	make
fi
constructivo_out=${output}"Constructivo.out"
./constructivo < ../$input > ../out/$constructivo_out 
echo "Constructivo 	procesó entrada."

cd ..
cd busqueda_local
if [ ! -e "./busqueda_local" ]; then
	make
fi
busqueda_out=${output}"Busqueda_Local.out"
./busqueda_local < ../$input > ../out/$busqueda_out
echo "Búsqueda Local  procesó entrada."

cd ..
cd tabu_search
if [ ! -e "./tabu_search" ]; then
	make
fi
tabu_out=${output}"Tabu_Search.out"
./tabu_search < ../$input > ../out/$tabu_out
echo "Tabú Search 	procesó entrada."
