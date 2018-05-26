from PlayAreaPythonLibrary import GameClient
from PlayAreaPythonLibrary import Blocks
import math
import random


def isBlock(block):
    return (block in [Blocks.NONE, Blocks.DESTROYED_WALL, Blocks.DEAD_PLAYER, Blocks.DEAD_MEATCHOPPER, Blocks.BOOM] == False)


def turn(gc):
    val = math.ceil(random.random() * 5 - 0.5)

    if val == 0:
        if isBlock(gc.map[gc.playerY - 1][gc.playerX]) == False:
            gc.up()
    if val == 1:
        if isBlock(gc.map[gc.playerY][gc.playerX + 1]) == False:
            gc.right()
    if val == 2:
        if isBlock(gc.map[gc.playerY + 1][gc.playerX]) == False:
            gc.down()
    if val == 3:
        if isBlock(gc.map[gc.playerY][gc.playerX - 1]) == False:
            gc.left()
    if val == 4:
        gc.act()


random.seed()
gc = GameClient()
gc.on_turn = turn
gc.run('127.0.0.1', 1234, 'PythonClient', 'Password')
