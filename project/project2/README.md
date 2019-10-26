test machine: CSELAB_kh4250_07<br />
date: 10/25/2019<br />
name: Jackie Ji, Dian Zhang<br />
x500: ji000011, zhan5999<br />



## Purpose of our program
Our task is to use the method of the Mapreduce to count the initial character of each words in different files and report the result in the alphabetical order.  We should get two output files and a folder. These two files will have the same result. The folder should store the files which be assinged to each mapper.


## How to compile the program
First we should open the Makefile and only choose one test and comment the other tests. Then if you type make test-mapreduce in the terminal, it will compile our program and generate two outfiles. 


## What exactly your program does
Our programe is consist of master, mapper, reducer and redirection. In the phase1, we should have a MapperInput folder and divide all files in the input folder into differnt mappers. All mappers should take the same amout of files and if the total amounts of files is not a multiple of number of mappers, we will add the extra file in the order of mappers until there is no remaining file. The file of each mapper have will show on the the MapperInput folder. Then we set up pipes in the master to link the phase 2 and phase3. The phase 2 will give the count results of different files and the reducer in phase 3 will combile all the results from each single pipe togther. Then it will generate a text file called ReducerResult. When we finish the phase 3, the master will call the phase 4. When we print the result of ReducerResult.txt the output will be redirected to a file "FinalResult.txt" by using dup2() and execlp(). Then the FinalResult.txt will have the same contents with the ReducerResult.txt.


## Assumptions
1. assume the file name will not have "."<br />
2. assume each path length will less than 1000.<br />
3. assume file numbers will less than 10000.<br />
4. assume mapper input will not in the current directory before the program start.<br />

## Teamname: Fantastic 2
## Jackie Ji  ji000011  Dian Zhang zhan5999

## Individual contributes
Jackie Ji: I finished the phase1 and phase2 and the main. Debug on phase 3 and 4.
Dian Zhang: I did the phase 3 and phase4. Debug on the main.

## Extra credits 
We didn't do the extra credits.


