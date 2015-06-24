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

	PeisSubscriberHandle s = peiskmt_subscribe(1, "Simple.Publisher_tuple");
	struct simple p;

	pthread_t id;
	pthread_create(&id, NULL, &valueThread, &p);

	printf("CALL\n");
	PeisCallbackHandle c;
	c = peiskmt_registerTupleCallback(1, "Simple.Publisher_tuple", &c, &callback);

	int i = 10;
	while(i--) {
		sleep(1);
	}

	printf("NO CALL\n");
	peiskmt_unregisterTupleCallback(c);

	i = 5;
	while(i--) {
		sleep(1);
	}

	printf("CALL AGAIN\n");
	c = peiskmt_registerTupleCallback(1, "Simple.Publisher_tuple", &c, &callback);

	i = 5;
	while(i--) {
		sleep(1);
	}

	printf("NO CALL AGAIN\n");
	peiskmt_unsubscribe(s);

    pthread_join(id, NULL);
	
	return 0;
}
