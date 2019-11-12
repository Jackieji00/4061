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
	@printf "===Testing mapreduce===\n"
	@printf "\nTestCase1: One line with no option\n"
	./wcs 1 ${TST_DIR}/test1.txt
	@printf "\n"
	# @printf "\nTestCase2_1: Normal Case - One mapper\n"
	# ./mapreduce ${TST_DIR}/TestCase2 2
	# @printf "\n"
	# @printf "\nTestCase2_2: Normal Case - 32 max mapper\n"
	# ./mapreduce ${TST_DIR}/TestCase2 32
	# @printf "\n"
	# @printf "\nTestCase2_3: Normal Case - Single level folder hierarchy with same number of files in each folder\n"
	# ./mapreduce ${TST_DIR}/TestCase2 5
	# @printf "\n"
	# @printf "\nTestCase3: Normal Case - Single level folder hierarchy with carying number of files in each folder\n"
	# ./mapreduce ${TST_DIR}/TestCase3 5
	# @printf "\n"
	# @printf "\nTestCase4: Normal case - Multi level folder hierarchy\n"
	# ./mapreduce ${TST_DIR}/TestCase4 5
	# @printf "\n"
#
# test-extracredits:
# 	@printf "===Testing extra credits===\n"
# 	./mapreduce ${TST_DIR}/TestCase5 5
# 	@printf "\n"

.PHONY: clean

clean:
	rm wcs main.o producer.o consumer.o utils.o
