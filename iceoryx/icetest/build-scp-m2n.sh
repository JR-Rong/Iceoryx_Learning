mkdir m2n/build || ture
cd m2n/build

cmake -DCMAKE_TOOLCHAIN_FILE=~/iceoryx/iceoryx/tools/toolchains/qnx/qnx_sdp70_aarch64le.cmake .. && 
make clean && 
make &&
expect -c "
spawn scp -r ../build root@10.42.0.42:/mnt1/iceoryx/iceoryx/icetest/m2n
expect {
    \"*password\"
                {
                    set timeout ;
                    send \"root\r\";
                }
    \"yes/no\"
                {
                    send \"yes\r\"; exp_continue;}
                }
expect eof"

cd ../..