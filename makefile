
.PHONY: 1 2 3 4 5 6 7 8 9

1: file1.o
2: file2.o
3: file3.o
4: file4.o
5: file5.o
6: file6.o

.PHONY: clean
clean:
	@rm -f *.o *.out