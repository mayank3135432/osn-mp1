a.out : main.c input.c hop.c pwd.c reveal.c log.c proclore.c seek.c alias.c source.c activities.c ping.c iman.c utils.c pipe.c fgbg.c neonate.c
	gcc -o a.out main.c input.c hop.c pwd.c reveal.c log.c proclore.c seek.c alias.c source.c activities.c ping.c iman.c utils.c pipe.c fgbg.c neonate.c
clean:
	rm -f a.out