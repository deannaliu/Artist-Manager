all:
	gcc -D_POSIX_C_SOURCE -std=c99 -Iinc src/main.c src/defs.c src/artist_ds.c -o artist_manager

fib: 
	gcc -std=c99 part_2/fib.c -o fib
sshell:
	gcc -std=c99 part_2/sshell.c -o simple_shell
sigblock: 
	gcc -D_POSIX_C_SOURCE -std=c99 part_2/shell.c -o shell
	gcc part_2/child.c -o child
clean:
	rm fib
	rm simple_shell
	rm shell
	rm child
	rm artist_manager
