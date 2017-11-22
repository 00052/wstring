all:
	$(CC) -g -m32 -fno-stack-protector main.c wstring.c tmpbuftable.c -o test
	execstack -s test
clean:
	@rm test
