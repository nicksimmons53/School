assign1Objects = wordcount.o

wordcount.o: wordcount.c
	gcc -g -c wordcount.c -o wordcount.o

wordcount: ${assign1Objects}
	gcc -g -o wordcount ${assign1Objects}

clean:
	rm -f ${assign1Objects}
