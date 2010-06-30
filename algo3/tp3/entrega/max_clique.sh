#pequeño script para ejecutar todos los algoritmos!

INPUT=$1
COMPLETE_INPUT=$1
INPUT=${INPUT##*/}
INPUT_length=${#INPUT}
OUTPUT=${INPUT:0:$INPUT_length-3}

if [ ! -d out/ ]; then
	mkdir out #if no esta creado crearlo....
	echo "Generando carpeta OUT."
fi

cd exacto
if [ ! -e "./exacto" ]; then
	make
fi
exacto_out=${OUTPUT}"Exacto.out"
./exacto < ../$COMPLETE_INPUT > ../out/$exacto_out
echo "Exacto 		procesó entrada."

cd ..
cd constructivo
if [ ! -e "./constructivo" ]; then
	make
fi
constructivo_out=${OUTPUT}"Constructivo.out"
./constructivo < ../$COMPLETE_INPUT > ../out/$constructivo_out 
echo "Constructivo 	procesó entrada."

cd ..
cd busqueda_local
if [ ! -e "./busqueda_local" ]; then
	make
fi
busqueda_out=${OUTPUT}"Busqueda_Local.out"
./busqueda_local < ../$COMPLETE_INPUT > ../out/$busqueda_out
echo "Búsqueda Local  procesó entrada."

cd ..
cd tabu_search
if [ ! -e "./tabu_search" ]; then
	make
fi
tabu_out=${OUTPUT}"Tabu_Search.out"
./tabu_search < ../$COMPLETE_INPUT > ../out/$tabu_out
echo "Tabú Search 	procesó entrada."
