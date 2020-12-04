TARGET:sub_app pub_app

sub_app:mqtt_subscribe.c
	gcc mqtt_subs.c -lpthread -lmosquitto -o sub_app

pub_app:mqtt_publish.c
	gcc mqtt_publish.c -lpthread -lmosquitto -o pub_app

clean:
	rm -f pub_app sub_app
