all: intervalos

intervalos: intervalos.cpp
	g++ intervalos.cpp -o intervalos

run: intervalos
	./intervalos

time: intervalos test.py test.in
	./test.py --time

count: intervalos test.py
	./test.py --count

plot_time: plot_times time_err.dat
	./plot_times

plot_count: plot_counts counts.dat
	./plot_counts

clean:
	rm time_err.dat counts.dat

test.in: test
	echo 100000 1000 | ./test > test.in

test: test.cpp
	g++ -o test test.cpp

time_err.dat: intervalos test.py test.in
	./test.py --time

counts.dat: intervalos test.py
	./test.py --count
