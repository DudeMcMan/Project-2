Smart Campus Navigation and Facility Analytics System
=====================================================
CSCE Foundations of Data Structures, Project 2 (Milestone 1)
Team: Ryker, Mason, Owen

PURPOSE
-------
A C++ console program that loads campus data from text files and lets the
user explore it through a menu. It has three parts:
  1. Facilities, stored in a custom hash table keyed by facility ID.
  2. Campus buildings and walkways, stored as an undirected graph with a
     breadth first search (BFS) for reachability and walking distance.
  3. The department hierarchy, stored as a general tree.

BUILD
-----
From the project root (the folder that contains src/ and include/):

  g++ -std=c++11 -Wall -Iinclude -o campus $(ls src/*.cpp | grep -v test.cpp)

(src/test.cpp is a stand alone test file with its own main, so it is left
out of the build.)

RUN
---
  ./campus            uses the data/ folder
  ./campus somefolder reads the four data files from somefolder instead

The data folder must contain buildings.txt, connections.txt, facilities.txt
and departments.txt. A missing or malformed file prints an error or warning
and the program keeps running.

MENU
----
  1  Display all facilities (hash table order)
  2  Search a facility by ID
  3  Display the campus graph
  4  Display graph statistics
  5  BFS from a starting building
  6  Add a department
  7  Search a department
  8  Display the department hierarchy
  9  Display the preorder traversal
  0  Exit

DIRECTORY LAYOUT
----------------
  include/             header files
    CampusGraph.h        graph class (Mason)
    DepartmentTree.h     tree class (Ryker)
    Utils.h              shared string helpers (Ryker)
    facilities.h, facilitiesHashTable.h, faclilitiesManager.h   (Owen)
  src/                 source files, same names as the headers, plus main.cpp
  tests/test_tree.cpp  unit style checks for the department tree
  data/                the instructor provided input files
  testdata/            extra input sets used for testing (bad records,
                       collisions, Windows line endings, empty files,
                       missing file, disconnected graph, root only tree)
  test_results.txt     results of the Milestone 1 tests
  design_document.txt  hash function, collision strategy, graph design
  CONTRIBUTIONS.txt    who built what
  .gitignore
