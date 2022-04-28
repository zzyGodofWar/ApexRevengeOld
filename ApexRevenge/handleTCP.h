#pragma once
#include "main.h"

extern SOCKET valid_server;

bool ConnectToServer();

void ReceivePacket();
