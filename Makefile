default: dayssince
all: dayssince
dayssince: dayssince.c
	gcc dayssince.c -o dayssince -Wall
clean:
	rm -f dayssince

