clang++ multiplexing_echo_server.cpp -o server

list="1 2 3 4 5 6 7 8 9 10"
for var in $list
do
    clang++ chatting_client.cpp -o $var
done

# ./server &
# for var in $list
# do
#     ./$var &
# done

