//gcc mqtt_publish.c -l pthread -l mosquitto -o pubs.out

#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<mosquitto.h>
#include <unistd.h>

struct mosquitto *mosq;
char username[]="mqtttest", password[]="qwerty123";
const char mqtt_topic[]="testmsg";
int mqtt_flag=0, qos=1,publish_flag=0;

void connect_callback(struct mosquitto *mosq, void *obj, int result) {
	printf("\nconnect callback, result = %d , rc=%s", result,mosquitto_connack_string(result));
}

void publish_callback(struct mosquitto *mosq, void *obj, int result) {
	printf("\nCallback: Message published Successfully");
}

void mqtt_thread() {
    mosquitto_lib_init();
    char *clientid=NULL, *hostname="127.0.0.1";
    int flag=0,port=1883;

    mosq = mosquitto_new(clientid, true, clientid);
    if(!mosq) {
        printf("\nMosq instance is Null !!");
        return;
    }
    else {
        printf("\nMosq instance created successfully");
		mosquitto_connect_callback_set(mosq, connect_callback);
		//mosquitto_publish_callback_set (mosq, publish_callback);
        flag = mosquitto_username_pw_set(mosq, username, password);
        if (flag == MOSQ_ERR_SUCCESS)
		    printf("\nSETTING USERNAME & PASSWORD");
        else {
            printf("\nUSERNAME & PASSWORD SETTING FAILED");
            return;
        }
        flag = mosquitto_connect(mosq, hostname, port, 60);
        if (flag == MOSQ_ERR_SUCCESS) {
            printf("\nMQTT Connected successfully");
            mqtt_flag=1;
        }
        else {
            printf("\nConnection failed..");
            return;
        }
        while (true) {
            flag = mosquitto_loop(mosq, -1, 1);
            if (flag != MOSQ_ERR_SUCCESS)
            {   
                flag = mosquitto_connect(mosq, hostname, port, 60);
                if(flag ==MOSQ_ERR_SUCCESS) {
                    mqtt_flag=1;
                }
                else {
                    printf("\nFailed to connect");
                    return;
                } 
            } 
        }
        printf("\nExitted from MQTT loop");
        if (mosquitto_disconnect(mosq)) 
            printf("\nMosquitto Disconnected successfully");
    }
}
void main() {
    pthread_t mosquitto_t;
    pthread_create(&mosquitto_t, NULL, (void*)mqtt_thread,NULL);
    while(!mqtt_flag)
        sleep(1);
    printf("\nMain thread : MQTT connected\n\nType 'quit' to exit\n");
    while(true)
    {   
        char input[100];
        scanf("%s",input);
        const char *msg=input;
        mosquitto_publish (mosq, NULL, mqtt_topic, sizeof(input), (const void*)msg, qos, 0);
        if(strcmp(input,"quit")==0)
        {
            printf("\nDisconnecting from broker.. Have a good day !\n");
            mosquitto_disconnect (mosq);
            mosquitto_destroy(mosq);
            break;
        }
        if (publish_flag != MOSQ_ERR_SUCCESS)
            printf ("\nPUBLISH FAILED, result"); 
    }
}

