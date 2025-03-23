# c-mtdf-chess-engine
c++ mtdf chess engine based in numbfish python chess engine translatated to c++ like an exercise to introducing to chess engines. ELO is around 2500.

There are 2 versions, the one that in the mtdf search uses the same letter mode for the board that the original python numbfish, and the second that uses disservin chess board. In this type of search I notice that sometimes that disservin does not launches assert when there some king is not in the board (comment that line in the library if needed).  
The disservin one calculates after book openning calculates around more 1/2 million nodes in 5 seconds, the other one around 350000. But because it does not imoplement prunning does not go so far deep in depth. 

uses:
dissservin SHL chess library for board
Nnueprobe library 
Syzygy Online from SHL library 
"reader.hpp" for openning .bin books (Shreyas Deo). 
It has a 2 codeblocks. .cbp project. Look at the project properties-> project build options-> linker settings(nnuprobe) and -pthread and search directories for dissevin source files. 

After buiding you can test it with a nice python interface that I use to test engines:  
https://github.com/jesusmontera/Python-interface-for-chess-uci-engines-battles
