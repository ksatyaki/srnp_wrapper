#include <srnp/srnp_wrapper.h>
#include <srnp/srnp_kernel.h>
#include <srnp/meta_pair_callback.hpp>
#include <boost/shared_array.hpp>

void genericPairCallback(const srnp::Pair::ConstPtr& pair_to_callback, void* userdata, PeisTupleCallback fn) {
	boost::shared_array <char> key = boost::shared_array <char> (new char[pair_to_callback->getKey().size() + 1]);
	strcpy(key.get(), pair_to_callback->getKey().data());	
	boost::shared_array <char> data = boost::shared_array <char> (new char[pair_to_callback->getValue().size() + 1]);
	strcpy(data.get(), pair_to_callback->getValue().data());

	PeisTuple p;
	p.key = key.get();
	p.data = data.get();
	p.datalen = pair_to_callback->getValue().size();
	p.owner = pair_to_callback->getOwner();
	p.ts_write[0] = 0;
	p.ts_write[1] = 0;
	p.ts_expire[0] = 0;
	p.ts_expire[1] = 0;
	fn(&p, userdata);
}

extern "C" {

	int peiskmt_peisid() { return srnp::getOwnerID(); }

	void peiskmt_printTuple(PeisTuple* ptuple) {
		printf("Owner: %d, Key: %s, Value: %s\n", ptuple->owner, ptuple->key, ptuple->data);
	}
	
	void peiskmt_initialize(int *argc,char **args, char **env) {
		srnp::initialize(*argc, args, env);
	}
	
	void peiskmt_setStringTuple(const char* key, const char* value) {
		srnp::setPair(key, value);
	}

	void peiskmt_setRemoteStringTuple(int owner, const char* key, const char* value) {
		srnp::setRemotePair(owner, key, value);
	}

	void peiskmt_setTuple(const char *key, int len, const void *data, const char *mimetype, int encoding) {
		srnp::setPair(key, std::string(static_cast<const char*>(data), len));
	}

	void peiskmt_setMetaTuple(int metaowner, const char* metakey, int owner, const char* key) {
		srnp::setMetaPair(metaowner, metakey, owner, key);
	}

	void peiskmt_setRemoteTuple(int owner, const char *key, int len, const void *data, const char *mimetype, int encoding) {
		srnp::setRemotePair(owner, key, std::string(static_cast<const char*>(data), len));
	}

	int peiskmt_setStringTupleIndirectly(int metaOwner, const char *metaKey, const char *value) {
		return srnp::setPairIndirectly(metaOwner, metaKey, value);
	}

	PeisTuple* peiskmt_getTuple(int owner,const char *key, int flags) {
		srnp::Pair::ConstPtr tuple = srnp::getPair(owner, key);
		if(!tuple) {
			PeisTuple* new_one = new PeisTuple();
			strcpy(new_one->data, tuple->getValue().data());
			strcpy(new_one->key, tuple->getKey().data());
			new_one->datalen = tuple->getValue().size();
			new_one->owner = tuple->getOwner();
			return new_one;
		}
		else {
			return NULL;
		}
	}

	void peiskmt_deAllocatePeisTuple(PeisTuple* tuple) {
		delete tuple;
	}

	PeisTuple* peiskmt_getTupleIndirectly(int metaOwner, const char *metaKey, int flags) {
		srnp::Pair::ConstPtr tuple = srnp::getPairIndirectly(metaOwner, metaKey);
		if(!tuple) {
			PeisTuple* new_one = new PeisTuple();
			strcpy(new_one->data, tuple->getValue().data());
			strcpy(new_one->key, tuple->getKey().data());
			new_one->datalen = tuple->getValue().size();
			new_one->owner = tuple->getOwner();
			return new_one;
		}
		else {
			return NULL;
		}
	}

	PeisSubscriberHandle peiskmt_subscribe(int owner,const char *key) {
		return srnp::registerSubscription(owner, key);
	}

	int peiskmt_unsubscribe(PeisSubscriberHandle handle) {
		srnp::cancelSubscription(handle);
		return 1;
	}

	int peiskmt_isRunning() {
		return true;
	}

	PeisCallbackHandle peiskmt_registerTupleCallback(int owner, const char *key, void *userdata, PeisTupleCallback *fn) {
		return srnp::registerCallback(owner, std::string(key), boost::bind(genericPairCallback, _1, userdata, fn));
	}

	int peiskmt_unregisterTupleCallback(PeisCallbackHandle callback) {
		srnp::cancelCallback(callback);
		return true;
	}

	void peiskmt_registerMetaTupleCallback(int metaOwner, const char* metaKey, void* userdata, PeisTupleCallback *fn) {
		srnp::registerMetaCallback(metaOwner, metaKey, boost::bind(genericPairCallback, _1, userdata, fn));
	}

	void peiskmt_subscirbeIndirectly(int metaOwner, const char* metaKey) {
		srnp::registerMetaSubscription(metaOwner, metaKey);
	}

	void peiskmt_unregisterMetaTupleCallback(int metaowner, const char* metakey) {
		srnp::cancelMetaCallback(metaowner, metakey);
	}
	
	void peiskmt_unsubscribeIndirectly(int metaowner, const char* metakey) {
		srnp::cancelMetaSubscription(metaowner, metakey);
	}
}



