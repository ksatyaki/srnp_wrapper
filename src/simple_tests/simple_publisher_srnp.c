#include <srnp/srnp_wrapper.h>

int main(int argn, char** args, char** env) {
	peiskmt_initialize(&argn, args, env);

	peiskmt_setStringTuple("Simple.Publisher_tuple", "SimpleValue");

	while(1 && peiskmt_isRunning())
		sleep(1);

	return 0;
}
