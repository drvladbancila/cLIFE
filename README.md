# cLIFE
## A simulator of John Conway's "Game of Life" written in C.

This is a C implementation of the "Game of Life".  
When you start it you can either give it a file as a map or the flag "-r" which randomises the grid for you, if you're not willing to create your own (lazy). 
The living cells will be these green asterisks '*' jumping all over the place. If a cell lives more than four generations then it gets promoted as "stable" and therefore becomes red.  

### Rules
After you execute cLIFE you can see the various cells moving and evolving
following exactly the rules of the original game:  
* A living cell with only one neighbour or less dies (as from isolation)  
* A living cell with more than three neighbours dies (as from overpopulation)  
* A living cell with two or three neighbours keeps living for another generation  
* A dead cell with three neighbors becomes alive in the next generation  

### How to install
(you may need "sudo" for some of these)
`git clone --recursive https://github.com/drvladbancila/cLIFE`  
`cd cLIFE`  
`chmod +x install.sh`  
`./install.sh`  
Then execute with `cLIFE`  

### Maps
The program can run in 'random mode' or it can use a map. You can find some examples
in the 'maps' folder.  
However, if you want to write one you must create a text file with no more than 80 columns
and 30 rows representing a living cell with a one '1' and a dead cell with a dot '.'
(for the dead cells you can actually use any character as it will be ignored, but a dot  s
makes the ones stand out, trust me).  

Example of a simple map: 
```
...........
.....1.....
....111....
...........
```
### Screenshot
![alt text](https://github.com/drvladbancila/cLIFE/blob/master/clifedemo.png)
Image generated with the wonderful [cool-retro-terminal](https://github.com/Swordfish90/cool-retro-term) by Swordfish90.
