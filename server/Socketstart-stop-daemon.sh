#! /bin/sh

echo "Starting script"

case "$1" in
	start)
		echo "Starting daemon"
		start-stop-daemon -S -n aesdsocket -a /usr/bin/aesdsocket -- -d
		/usr/bin/aesdchar_load aesdchar
		;;
	
	stop)
		echo "Stopping daemon"
		/usr/bin/aesdchar_unload aesdchar
		start-stop-daemon -K -n aesdsocket
		;;
	
	*)
		echo "Usage: $0{start|stop}"
	exit 1
	
esac
exit 0	
