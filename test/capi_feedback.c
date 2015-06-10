#include <stdio.h>
#include <feedback.h>

static void print_menu(void)
{
	printf("Which do you want to do?\n");
	printf("\t0 : Play\n");
	printf("\t1 : Change the path\n");
	printf("\t2 : Reset the path\n");
	printf("\tothers : quit\n");
}

static void print_play_menu(void)
{
	printf("Which do you want to do?\n");
	printf("\t0. all\n");
	printf("\t1. sound\n");
	printf("\t2. vibration\n");
	printf("\t3. led\n");
	printf("\tothers. quit\n");
}

static void play(void)
{
	int a, b, val;

	while(1) {
		print_play_menu();
		printf("Please enter the play menu : ");
		scanf("%d", &a);
		if (a < 0 || a > 4)
			break;
		printf("Please input value (exit:-1) : ");
		scanf("%d", &b);
		if (b == -1)
			break;

		if (a == 0)
			val = feedback_play(b);
		else
			val = feedback_play_type(a, b);

		printf("ret value : %d\n", val);
	}
}

int main(int argc, char* argv[])
{
	char buf[4096] = {0,};
	int val;
	int a, b;

	val = feedback_initialize();
	if (val != FEEDBACK_ERROR_NONE) {
		printf("feedback_initialize error : %d\n", val);
		return -1;
	}

	while(1)
	{
		print_menu();
		printf("Please input value : ");
		scanf("%d", &a);

		switch(a) {
		case 0:
			play();
			break;
		case 1:
			printf("Please input type(sound:0,vib:1), enum, new path  : ");
			scanf("%d %d %s", &a, &b, buf);
			val = feedback_set_resource_path( (a == 0) ? FEEDBACK_TYPE_SOUND : FEEDBACK_TYPE_VIBRATION, b, buf);
			printf("ret value : %d\n", val);
			break;
		case 2:
			printf("Please input type(sound:0,vib:1), enum  : ");
			scanf("%d %d", &a, &b);
			val = feedback_set_resource_path( (a == 0) ? FEEDBACK_TYPE_SOUND : FEEDBACK_TYPE_VIBRATION, b, NULL);
			printf("ret value : %d\n", val);
			break;
		default:
			goto exit;
		}
	}

exit:
	val = feedback_deinitialize();
	if (val != FEEDBACK_ERROR_NONE) {
		printf("feedback_initialize error : %d\n", val);
		return -1;
	}

	return 0;
}
