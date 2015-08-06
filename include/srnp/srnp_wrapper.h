#ifndef SRNP_WRAPPER_H_
#define SRNP_WRAPPER_H_

#ifdef __cplusplus
extern "C" {
#endif

	typedef double PeisCallbackHandle;
	typedef int PeisSubscriberHandle;

	typedef struct PeisTuple {
		int owner;
		char* key;
		char* data;
		int datalen;
		int ts_write[2];
		int ts_expire[2];
	} PeisTuple;

	typedef void PeisTupleCallback(PeisTuple *tuple,void *userdata);
	
	void peiskmt_initialize(int *argc,char **args, char **env);

	int peiskmt_peisid();

	void peiskmt_setStringTuple(const char* key, const char* value);

	void peiskmt_setRemoteStringTuple(int owner, const char* key, const char* value);

	void peiskmt_setMetaTuple(int metaowner, const char* metakey, int owner, const char* key);

	PeisTuple* peiskmt_getTuple(int owner,const char *key, int flags);

	PeisTuple* peiskmt_getTupleIndirectly(int metaOwner, const char *metaKey, int flags);

	void peiskmt_deAllocatePeisTuple(PeisTuple* tuple);

	int peiskmt_setStringTupleIndirectly(int metaOwner, const char *metaKey, const char *value);

	void peiskmt_setTuple(const char *key,int len,const void *data,const char *mimetype,int encoding);

	void peiskmt_setRemoteTuple(int owner, const char *key, int len, const void *data, const char *mimetype, int encoding);

	int peiskmt_isRunning();

	PeisSubscriberHandle peiskmt_subscribe(int owner,const char *key);

	int peiskmt_unsubscribe(PeisSubscriberHandle handle);

	PeisCallbackHandle peiskmt_registerTupleCallback(int owner, const char *key, void *userdata, PeisTupleCallback *fn);

	int peiskmt_unregisterTupleCallback(PeisCallbackHandle callback);

	void peiskmt_registerMetaTupleCallback(int metaOwner, const char* metaKey, void *userdata, PeisTupleCallback *fn);

	void peiskmt_subscribeIndirectly (int metaOwner, const char* metaKey);

	void peiskmt_unregisterMetaTupleCallback(int metaOwner, const char* metaKey);

	void peiskmt_unsubscribeIndirectly(int metaOwner, const char* metaKey);

	void peiskmt_printTuple(PeisTuple* ptuple);	

#ifdef __cplusplus
}
#endif

#endif
