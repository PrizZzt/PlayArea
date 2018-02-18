#pragma once

#define MAX_LENGTH 41

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
| 1         | X                     | ������ �����   |   | 20        | ������                | �����          |
|           |                       | �� ����������� |   +-----------+-----------------------+----------------+
+-----------+-----------------------+----------------+   | 20        | ������                | ������         |
| 1         | Y                     | ������ �����   |   +-----------+-----------------------+----------------+
|           |                       | �� ���������   |
+-----------+-----------------------+----------------+
| X * Y * 2 | �������� ������ -     | �����          |
|           | ��� ����� �� ������   |                |
|           | ������ (��� + ID)     |                |
+-----------+-----------------------+----------------+

server_action_e::RESULT = 2
(������������� ��������� �������)
+-----------+-----------------------+----------------+
| ������    | ��������              | ��������       |
| � ������  |                       |                |
+-----------+-----------------------+----------------+
| 1         | C                     | ��� ���������� |
+-----------+-----------------------+----------------+

*/
