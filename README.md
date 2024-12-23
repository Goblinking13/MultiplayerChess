# MultiplayerChess
Simple console chess program with the ability to play on local network.
# How to start
1. Build program.
```
git clone git@github.com:Goblinking13/MultiplayerChess.git
cd MultiplayerChess
make
```
2. Start game
Run without arguments will start the game on a single computer.
```
make run 
```
or
```
./chess
```
Run with arguments to play on different machines.
```
make run <your port> <opponent port> <your side>
```
or
```
./chess <your port> <opponent port> <your side>
```
