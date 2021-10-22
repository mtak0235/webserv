clang++ socket_server_practice.cpp -o server
clang++ socket_client_practice.cpp -o client

ipaddress=$(ipconfig getifaddr en0)


./server 3550 & ./client $ipaddress 3550


rm -rf client server

