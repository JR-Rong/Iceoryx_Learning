pwd

cd /mnt1/iceoryx/iceoryx

./build/iox-roudi &

./icetest/m2n/build/iox-test-pub -wp150 1 &
./icetest/m2n/build/iox-test-sub -wp150 1 &

./icetest/m2n/build/iox-test-pub -wp150 2 &
./icetest/m2n/build/iox-test-sub -wp150 2 &

./icetest/m2n/build/iox-test-pub -wp150 3 &
./icetest/m2n/build/iox-test-sub -wp150 3 &

./icetest/m2n/build/iox-test-pub -wp150 4 &
./icetest/m2n/build/iox-test-sub -wp150 4 &

./icetest/m2n/build/iox-test-pub -wp150 5 &
./icetest/m2n/build/iox-test-sub -wp150 5 &