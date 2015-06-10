#include <stdio.h>
#include <unistd.h>
#include <feedback.h>

int main(int argc, char* argv[])
{
	int val;
	int i = -1;

	val = feedback_initialize();
	if (val != FEEDBACK_ERROR_NONE) {
		printf("feedback_initialize error : %d\n", val);
		return -1;
	}

	while(1)
	{
		val = feedback_play(++i);
		printf("[%2d] result : %d\n", i, val);
		if (val < 0) break;

		usleep(100000);
	}

	val = feedback_deinitialize();
	if (val != FEEDBACK_ERROR_NONE) {
		printf("feedback_initialize error : %d\n", val);
		return -1;
	}

	return 0;
}
