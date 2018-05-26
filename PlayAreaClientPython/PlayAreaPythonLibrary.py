import socket
from enum import Enum


class Blocks(Enum):
    NONE = 0

    UNDESTRUCTIBLE_WALL = 1
    DESTRUCTIBLE_WALL = 2
    DESTROYED_WALL = 3

    PLAYER = 4
    PLAYER_WITH_BOMB_1 = 5
    PLAYER_WITH_BOMB_2 = 6
    PLAYER_WITH_BOMB_3 = 7
    PLAYER_WITH_BOMB_4 = 8
    PLAYER_WITH_BOMB_5 = 9
    DEAD_PLAYER = 10

    MEATCHOPPER = 11
    DEAD_MEATCHOPPER = 12

    BOOM = 13
    BOMB_1 = 14
    BOMB_2 = 15
    BOMB_3 = 16
    BOMB_4 = 17
    BOMB_5 = 18


class GameClient:

    def __init__(self):
        self.playerID = -1
        self.playerX = -1
        self.playerY = -1

    def run(self, host, port, login, password):
        self.sock = socket.socket()
        self.sock.connect(('127.0.0.1', 1234))
        credentials = chr(6) + login.ljust(20, chr(0)) + \
            password.ljust(20, chr(0))
        self.sock.send(bytes(credentials, 'utf-8'))
        result = self.sock.recv(2)
        if result[0] == 2:
            self.playerID = result[1]
        else:
            raise Exception('Login failed!')
        while True:
            data = self.sock.recv(3)
            self.sizeX = data[1]
            self.sizeY = data[2]
            mapSize = self.sizeX * self.sizeY
            mapData = self.sock.recv(mapSize * 2)

            self.map = [[0 for x in range(self.sizeX)]
                        for y in range(self.sizeY)]
            c = 0
            for j in range(self.sizeY):
                for i in range(self.sizeX):
                    self.map[j][i] = Blocks.NONE
                    for k in Blocks:
                        if mapData[c] == k.value:
                            self.map[j][i] = k
                            if mapData[c+1] == self.playerID and k in [Blocks.PLAYER, Blocks.PLAYER_WITH_BOMB_1, Blocks.PLAYER_WITH_BOMB_2, Blocks.PLAYER_WITH_BOMB_3, Blocks.PLAYER_WITH_BOMB_4, Blocks.PLAYER_WITH_BOMB_5, Blocks.DEAD_PLAYER]:
                                self.playerX = i
                                self.playerY = j
                    c = c + 2

            self.on_turn(self)
        self.sock.close()

    def up(self):
        self.sock.send(bytes([1]))
        self.sock.recv(2)

    def down(self):
        self.sock.send(bytes([2]))
        self.sock.recv(2)

    def right(self):
        self.sock.send(bytes([3]))
        self.sock.recv(2)

    def left(self):
        self.sock.send(bytes([4]))
        self.sock.recv(2)

    def act(self):
        self.sock.send(bytes([5]))
        self.sock.recv(2)
