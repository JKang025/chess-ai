# chess-ai
This is a C++ chess engine built from scratch. This project is still ongoing, but there is two primary overarching goals. Firstly, to build a fast chess legal move generator using [bitboards](https://www.chessprogramming.org/Bitboards) and [magic bitboards](https://www.chessprogramming.org/Magic_Bitboards). Secondly, to use this fast legal move generator to make a chess engine that uses a [Monte-Carlo-Search-Tree](https://en.wikipedia.org/wiki/Monte_Carlo_tree_search). This will allow the engine to make probabalistic decisions. In fact, [Google's AlphaZero chess engine](https://en.wikipedia.org/wiki/AlphaZero) uses MCST in conjunction with a neural network, and was once the strongest engine in world (although it has been dethroned, the strongest chess engine currently is still Stockfish).

## Usage
### Docker
This project uses Docker. While this most likely overkill, considering I am only using c++ and building the project from scratch, it may prove to be useful in the future if I decide to intergrate other libraries or languages. 

As I have not yet finished the project, the image is not stored online. Instead, navigate to this directory and run the following commands to start the project.
```
docker build -t chess
docker-compose build
docker-compose run primary sh
```
### Running the program
After you are in the interactive bash, simply 
```
make all
./all
```