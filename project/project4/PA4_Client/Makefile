client:
	gcc -Wall -std=c99 -pthread -Iinclude -o client src/client.c src/phase1.c

clean:
	rm client
	rm -rf log
	rm -rf MapperInput
test:
	@printf "===Testing server===\n"
	# @printf "\nTestCase1: One line with no option\n"
	# ./client ./Testcases/TestCase1 1 127.0.0.1 4061
	# @printf  "\n"
	@printf "\nTestCase2: Normal Case with option\n"
	./client ./Testcases/TestCase2 5 127.0.0.1 8088
	@printf "\n"
