#pragma once

//multiplayer based on sockets
int initSocket(int port);
int sendMove(char* ip, int port, int* move);
std::vector<uint32_t> getOpMove(int servfd);
