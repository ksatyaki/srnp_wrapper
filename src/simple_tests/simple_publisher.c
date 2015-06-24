#include <peiskernel/peiskernel_mt.h>

int main(int argn, char** args) {
	peiskmt_initialize(&argn, args);

	peiskmt_setStringTuple("Simple.Publisher_tuple", "SimpleValue");

	while(1 && peiskmt_isRunning())
		sleep(1);

	return 0;
}
