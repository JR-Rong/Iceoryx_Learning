pwd

cd /mnt1/iceoryx/iceoryx

./build/iox-roudi &

./icetest/one2one/build/iox-test-pub > ./result/one2one-pub.txt &
./icetest/one2one/build/iox-test-sub > ./result/one2one-sub.txt