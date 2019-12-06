server:
	gcc -std=c99 -Wall -pthread -Iinclud -o server src/server.c

clean:
	rm server

test:
	@printf "===Testing server===\n"
	@printf "\nTestCase1: One line with no option\n"
	./server 8088
	@printf "\n"
	# @printf "\nTestCase2: Normal Case with option\n"
	# ./wcs 4 ${TST_DIR}/test2.txt -p
	# @printf "\n"
	# @printf "\nTestCase3: Normal Case with option\n"
	# ./wcs 2 ${TST_DIR}/test3.txt -p
	# @printf "\n"
	# @printf "\nTestCase4: Normal Case with option\n"
	# ./wcs 5 ${TST_DIR}/test4.txt -p
	# @printf "\n"

#
# test-extracredits:
# 	@printf "===Testing extra credits===\n"
# 	./mapreduce ${TST_DIR}/TestCase5 5
# 	@printf "\n"
