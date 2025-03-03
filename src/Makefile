OBJ = alsa-audio
SOURCES = APlayer.o

LDLIBS = -lasound


.PHONY : all
all: ${OBJ}

alsa-audio : ${SOURCES}

.PHONY : clean
clean:
	rm -rf ${OBJ}
	rm -rf *.o
