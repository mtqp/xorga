mkdir out #if no esta creado crearlo....
cd exacto
make		#if no esta makeado
./exacto < ../$1 > ../out/a.out
echo "Exacto procesó entrada"
cd ..
cd constructivo
make		#if no esta makeado
./constructivo < ../$1 > ../out/const.out #corregir los nombres q estan como la mona
echo "Constructivo procesó entrada"
cd ..
cd busqueda_local
make		#if no esta makeado
./busqueda_local < ../$1 > ../out/busq_lo.out
echo "Búsqueda local procesó entrada"
cd ..
cd tabu_search
make		#if no esta makeado
./tabu_search < ../$1 > ../out/tabu.out
echo "Tabú Search procesó entrada"
