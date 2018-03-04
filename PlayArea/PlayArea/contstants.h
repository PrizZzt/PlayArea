#pragma once

#define MAX_LENGTH        41                           // ������������ ����� ��������� �� �������
#define MAX_LENGTH_SERVER (256 * (1 + 1 + 20)) + 1 + 1 // ������������ ����� ��������� �� �������
#define MAX_PLAYERS_COUNT 0xff                         // ������������ ���������� �������

/*

     ////////////////////////////////////////////             ////////////////////////////////////////////
     // ��������� ��������� ������� �� ������� //             // ��������� ��������� ������� �� ������� //
     ////////////////////////////////////////////             ////////////////////////////////////////////
+-----------+-----------------------+----------------+   +-----------+-----------------------+----------------+
| ������    | ��������              | ��������       |   | ������    | ��������              | ��������       |
| � ������  |                       |                |   | � ������  |                       |                |
+-----------+-----------------------+----------------+   +-----------+-----------------------+----------------+
| 1         | server_action_e       | ��� ���������  |   | 1         | client_action_e       | ��� ���������  |
+-----------+-----------------------+----------------+   +-----------+-----------------------+----------------+
| ~         | |                     | ���������      |   | ~         | |                     | ���������      |
|           | V                     | ���������      |   |           | V                     | ���������      |
+-----------+-----------------------+----------------+   +-----------+-----------------------+----------------+

server_action_e::MAP = 1 (�����)                         client_action_e::CREDENTIALS = 6 (�����������/����)
+-----------+-----------------------+----------------+   +-----------+-----------------------+----------------+
| ������    | ��������              | ��������       |   | ������    | ��������              | ��������       |
| � ������  |                       |                |   | � ������  |                       |                |
+-----------+-----------------------+----------------+   +-----------+-----------------------+----------------+
| 1         | X (map_t::size_x)     | ������ �����   |   | 20        | ������                | �����          |
|           |                       | �� ����������� |   +-----------+-----------------------+----------------+
+-----------+-----------------------+----------------+   | 20        | ������                | ������         |
| 1         | Y (map_t::size_y)     | ������ �����   |   +-----------+-----------------------+----------------+
|           |                       | �� ���������   |
+-----------+-----------------------+----------------+
| X * Y * 2 | �������� ������ -     | �����          |
|           | ��� ����� �� ������   |                |
|           | ������ (��� + ID)     |                |
+-----------+-----------------------+----------------+

server_action_e::MESSAGE = 2
(��������� � ����������� �������)
+-----------+-----------------------+----------------+
| ������    | ��������              | ��������       |
| � ������  |                       |                |
+-----------+-----------------------+----------------+
| 1         |                       | ��� ���������� |
+-----------+-----------------------+----------------+

server_action_e::NAMES_LIST = 3
(������ ���� �������)
+-----------+-----------------------+----------------+
| ������    | ��������              | ��������       |
| � ������  |                       |                |
+-----------+-----------------------+----------------+
| 1         | C                     | ����������     |
|           |                       | �������        |
+-----------+-----------------------+----------------+
| ~         | |                     | ������ ��      |
|           | V                     | ������ ������� |
+-----------+-----------------------+----------------+

+-----------+-----------------------+----------------+
| 1         |                       | ����� ������   |
+-----------+-----------------------+----------------+
| 1         | L                     | ����� �����    |
|           |                       | ������         |
+-----------+-----------------------+----------------+
| min(L;20) | player_t::login       | ��� ������     |
+-----------+-----------------------+----------------+

server_action_e::POINTS_LIST = 4
(������ ����� �������)
+-----------+-----------------------+----------------+
| ������    | ��������              | ��������       |
| � ������  |                       |                |
+-----------+-----------------------+----------------+
| 1         | C                     | ����������     |
|           |                       | �������        |
+-----------+-----------------------+----------------+
| � * 5     | |                     | ������ ��      |
|           | V                     | ����� �������  |
+-----------+-----------------------+----------------+

+-----------+-----------------------+----------------+
| 1         |                       | ����� ������   |
+-----------+-----------------------+----------------+
| 4         | player_t::score       | ���� ������    |
+-----------+-----------------------+----------------+
*/
