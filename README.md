# chess-ai
This is a C++ chess engine built from scratch, including the legal move generation. The engine itself is based on a Monte-Carlo-Search-Tree. This will allow the engine to make probabalistic decisions. 

In fact, [Google's AlphaZero chess engine](https://en.wikipedia.org/wiki/AlphaZero) uses MCST in conjunction with a neural network, and was once the strongest engine in world (although it has been dethroned, the strongest chess engine currently is still Stockfish).

## Usage
### Docker
This project uses Docker. As this project is still in development, a image is not yet available. Instead, navigate to this directory and run the following commands to start the project.
```
docker build -t chess .
docker-compose build
docker-compose run primary sh
```
### Running the program
After you are in the interactive bash, simply 
```
make all
./all
```