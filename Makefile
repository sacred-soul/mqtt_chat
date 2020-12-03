TARGET:subs_app pubs_app

subs_app:mqtt_subs.c
	gcc mqtt_subs.c -lpthread -lmosquitto -o subs_app

pubs_app:mqtt_publish.c
	gcc mqtt_publish.c -lpthread -lmosquitto -o pubs_app

clean:
	rm -f pubs subs
