CC = gcc
CFLAGS = -g -std=c99
CLINK = -lpthread

wcs: main.o producer.o consumer.o utils.o
	$(CC) -o $@ $^ $(CFLAGS) $(CLINK)

main.o: main.c header.h
	$(CC) -o $@ -c main.c $(CFLAGS)

producer.o: producer.c header.h
	$(CC) -o $@ -c producer.c $(CFLAGS)

consumer.o: consumer.c header.h
	$(CC) -o $@ -c consumer.c $(CFLAGS)

utils.o: utils.c header.h
	$(CC) -o $@ -c utils.c $(CFLAGS)

TST_DIR = testing
test:
	# @printf "===Testing mapreduce===\n"
	# @printf "\nTestCase1: One line with no option\n"
	# ./wcs 2 ${TST_DIR}/test1.txt
	# @printf "\n"
	# @printf "\nTestCase2: Normal Case with option\n"
	# ./wcs 4 ${TST_DIR}/test2.txt -p
	# @printf "\n"
	# @printf "\nTestCase3: Normal Case with option\n"
	# ./wcs 2 ${TST_DIR}/test3.txt -p
	# @printf "\n"
	@printf "\nTestCase4: Normal Case with option\n"
	./wcs 5 ${TST_DIR}/test4.txt -p
	@printf "\n"

#
# test-extracredits:
# 	@printf "===Testing extra credits===\n"
# 	./mapreduce ${TST_DIR}/TestCase5 5
# 	@printf "\n"

.PHONY: clean

clean:
	rm wcs main.o producer.o consumer.o utils.o log.txt
