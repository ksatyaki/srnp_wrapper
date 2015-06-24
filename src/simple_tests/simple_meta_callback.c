#include <srnp/srnp_wrapper.h>
#include <pthread.h>

struct simple {
	int num;
};

void* valueThread (void* data) {
	((struct simple*)data)->num = 1;
	while(peiskmt_isRunning()) {
		((struct simple*)data)->num++ ;
		sleep(1);
	}
}

void callback(PeisTuple* p, void* data) {
	printf("Callback called with key: %s, owner: %d, data: %s, value: %lf\n", p->key, p->owner, p->data, *(PeisCallbackHandle*)data);
}


int main(int argn, char** args, char** env) {
	peiskmt_initialize(&argn, args, env);

//	PeisSubscriberHandle s = peiskmt_subscribe(1, "Simple.Publisher_tuple");
	struct simple p;

	pthread_t id;
	pthread_create(&id, NULL, &valueThread, &p);

	printf("CALL\n");
	//PeisCallbackHandle c;
	peiskmt_registerMetaTupleCallback(1, "simple", &p, &callback);

	int i = 20;
	while(i--) {
		sleep(1);
	}

	printf("NO CALL\n");
	peiskmt_unregisterMetaTupleCallback(1, "simple");

	i = 5;
	while(i--) {
		sleep(1);
	}

	printf("CALL AGAIN\n");
	peiskmt_registerTupleCallback(1, "simple", &p, &callback);

	i = 20;
	while(i--) {
		sleep(1);
	}

	printf("NO CALL AGAIN\n");
//	peiskmt_unsubscribe(s);

    pthread_join(id, NULL);
	
	return 0;
}
