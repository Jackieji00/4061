test machine: CSELAB_kh4250_07<br />
date: 10/25/2019<br />
name: Jackie Ji, Dian Zhang<br />
x500: ji000011, zhan5999<br />

## Teamname: Fantastic 2
## Jackie Ji  ji000011  Dian Zhang zhan5999

## How to compile the program
If you want to compile the program, you should type the "make" in the terminal first. Then, you should type  ./wcs #consumer filename [option] [#queue_size] in the command line. The second argument “#consumer” is the number of consumers you want to use. The third argument “filename” is the input file name. For example, if the test file in a folder named testing should input "testing/filename". The fourth argument "[option]" has 3 possibilities to choose. If input -p, the program will generate a log file. If input -b, the program will use a bounded buffer. If input -bp, the program will use the bounded buffer and generate a log file. The last option argument is the queue size if using bounded buffer, so you should input a number. 

## Individual contributes
Jackie Ji: Finish the producer and consumer. Debug on the main.
Dian Zhang: Initial the producer and main. Debug on the consumer.

## Assumptions
1. assume the file name will not exceed 100 characters "."<br />
2. In the directory which contains the ./wcs, should not have the log.txt at the beginning.<br />

## What exactly your program does
Our program counts the starting letter for each word in the file and may generate a log file that contains the summary results in alphabet order. We build the program by using the multi-threads. The program will create one producer thread read the data from the file to the shared queue line by line. The consumer threads will get the data from the queue and do the word count. Each consumer will work on one line at a time and delete the data in the queue when they are done. 


## Extra credits 
We didn't do the extra credits.



