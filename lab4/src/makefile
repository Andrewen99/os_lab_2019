
mex1: ./ex1/find_min_max.c  ./ex1/parallel_min_max.c ./ex1/utils.c
	gcc -o ./ex1/ex1res ./ex1/find_min_max.c  ./ex1/parallel_min_max.c ./ex1/utils.c

mex3: ./ex3/process_memory.c
	gcc -o ./ex3/ex3res ./ex3/process_memory.c


mex5: ./ex5/utils.c ./ex5/parallel_sum.c mlibsum
	gcc -o ex5withlb ./ex5/parallel_sum.c ./ex5/utils.c -std=gnu99 -I./ex5 -L./ex5 -lm -lsum -lpthread

mex5_nl: ./ex5/utils.c ./ex5/parallel_sum.c ./ex5/libsum/sum.c
	gcc -o ex5nolbres ./ex5/parallel_sum.c ./ex5/utils.c ./ex5/libsum/sum.c -std=gnu99 -I./ex5 -I./ex5/libsum -L./ -lm -lpthread

mlibsum: ./ex5/libsum/sum.c
	gcc -o ./ex5/libsum/sum.o -c ./ex5/libsum/sum.c -I./ex5/libsum/
	ar rcs ./ex5/libsum.a ./ex5/libsum/sum.o
	rm ./ex5/libsum/sum.o


clean:
	rm -rf *.out *.o -f *.a
