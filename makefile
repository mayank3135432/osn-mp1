a.out : main.c input.c hop.c pwd.c reveal.c log.c proclore.c seek.c utils.c
	gcc -o a.out main.c input.c hop.c pwd.c reveal.c log.c proclore.c seek.c utils.c
clean:
	rm -f a.out