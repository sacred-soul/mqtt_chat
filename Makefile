TARGET:subs pubs

subs:mqtt_subs.c
	gcc mqtt_subs.c -lpthread -lmosquitto -o subs

pubs:mqtt_publish.c
	gcc mqtt_publish.c -lpthread -lmosquitto -o pubs

clean:
	rm -f pubs subs