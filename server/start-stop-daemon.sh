#!/bin/sh

if [ $# -lt 1 ];
    then
        echo "Specify start or stop"
        exit 1
fi

option="$1"

if [ "$option"=="start" ]
    then
        start-stop-daemon -S -n aesdsocket -a /usr/bin/aesdsocket -- -d
fi

if [ "$option"=="stop" ]
    then
        start-stop-daemon -K -n aesdsocket
fi

exit 0