
expect -c "
spawn ssh root@10.42.0.42
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

