@echo off
set PATH=%cd:~0,1%:\PortableSoftware\MinGW\bin;%PATH%
g++ -Wall -Wextra -Werror -Wpedantic -std=c++1z Snake.cpp Main.cpp -o Snake -I E:\PortableSoftware\MinGW\external\include -L E:\PortableSoftware\MinGW\external\lib\static -lpdcurses