test machine: CSELAB_vole_05<br />
date: 10/09/2019<br />
name: Jackie Ji, Dian Zhang<br />
x500: ji000011, zhan5999<br />

## Purpose of our program
Our task is to generate an executable ’mymake’ from the main c program. 'mymake' should parse the makefile , build a dependency graph for the provided target and execute the recipes for these targets.

## Compile and excute the program
If you type gcc main.c -o mymake in the terminal, it will compile our program and generate a mymake file. And then you can type ./mymake with one or two arguments in the command line. This function only takes at least one and no more than 2 arguments. It can be excuted by following ways:
$ ./mymake filename [target]
$ ./mymake [-p] filename
$ ./mymake [-r] filename


## How does our program work
$ ./mymake filename [target]
The program will excute the recipes of 1st target and associated targets with only one argument which is the filename. In addition, with the second argument which is the specific target, it will excute the recipes of that specfic target and associated ones.

$ ./mymake [-p] filename
The programe will print the target, dependencies, and recipes in the makefile.
$ ./mymake [-r] filename
The programe will print recipe order of the target.

## Assumptions
The progame assumed that there has a blank line between the recipe and the next target in the makefile.
Only one dependencies in each target except the first one can be referred to other targets.
We assume the every dependencies will exist and work.

## Teamname: Fantastic 2
## Jackie Ji  ji000011  Dian Zhang zhan5999

## Individual contributes
Jackie Ji: I finished the part 1 and debug the part 3.
Dian Zhang: I did the part 2 and basic constucted the part 3.

## Extra credits 
We did the extra credits. The programe can handle the situation when the targets have mutiple recipes and when one line recipe is separated by the comma.


