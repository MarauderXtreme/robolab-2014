CC=gcc
SRC= Communication.c aud.c RobotProxy.c Urlcode.c
FLAGS=-I/usr/include/python2.7 -g
LIBS=-lpython2.7
EXEC=aud
ALL:
	$(CC) $(FLAGS) -o $(EXEC) $(SRC) $(LIBS)
clean:
	rm -f $(EXEC)
.PHONY:
	clean
