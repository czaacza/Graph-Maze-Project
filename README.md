# Graph-Maze-Project
C program using weighted graphs, finding the shortest path between two points in a maze.

## How it works?
The program implements graphs as **neighborhood matrix** - an array of integers, which assigns weights to the specific edges of the maze. 

The maze is defined through text document - the list of all available edges, each including a starting and ending point.

The program uses **DFS** - An recursion algorithm used to find all possible paths in a maze.

Each time the program starts, weights values of all the edges are being randomized from 1 to 10. 

## Usage

To call the program correctly, we should give it the argument defining 
- the maze file name
- the length of the labirinth's wall ("3" for 3x3, "4" for 4x4 etc.)
- starting vertex
- ending vertex

Program call examples:

![Call examples](https://github.com/czaacza/Graph-Maze-Project/blob/master/call-examples.PNG)

Program work:

![Program call](https://github.com/czaacza/Graph-Maze-Project/blob/master/program-call.PNG)
