getrate : ReadNet.o
	gcc -o $@ $^


.PHONY:clean

clean :
	rm -f getrate *.o
