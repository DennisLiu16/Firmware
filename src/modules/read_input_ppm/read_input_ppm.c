/**
 * @file read_input_ppm.c
 * This C file try to read input from <input_rc.msg>
 * @author Dennis <mail@example.com>
 */
#include <px4_platform_common/px4_config.h>
#include <px4_platform_common/tasks.h>
#include <px4_platform_common/posix.h>
#include <unistd.h>
#include <stdio.h>
#include <poll.h>
#include <string.h>
#include <math.h>

#include <uORB/uORB.h>
#include <uORB/topics/input_rc.h>

/*
	1. New Thread : 不打擾別的指令
	2. stop 有效



*/





//var
volatile bool Exit = 0;
//function
void monitor(int);
void copy_input_ppm(int);

//export
__EXPORT int read_input_ppm_main(int argc, char *argv[]);



int read_input_ppm_main(int argc, char *argv[]){

	if(argc < 2){
		goto out;
	}

	/* subscribe to input_rc topic */
	int input_rc_sub_fd = orb_subscribe(ORB_ID(input_rc));
	/* limit the update rate to 10 Hz */
	orb_set_interval(input_rc_sub_fd,100);

	if (!strcmp(argv[1], "monitor")) {

		PX4_INFO("Try to read ppm\n");

		monitor(input_rc_sub_fd);

		Exit = 0;
		PX4_INFO("Leaving\n");
		exit(0);
	}

	if (!strcmp(argv[1], "stop")) {

		Exit = true;

		/*unsubscribe*/

		exit(0);
	}

out:
	PX4_ERR("command wrong , please use monitor / stop only");
	return 0;

}



void
monitor(int rc_sub_fd){

	/*clean first*/
	printf("\033[2J");
	printf("read values are:\n");

	/*show result*/
	while(!Exit){
		printf("\033[2J\033[H");
		printf("\n");

		copy_input_ppm(rc_sub_fd);

		//可以改成一個sleep 一個poll_up試試

		usleep(200);
	}

}

void
copy_input_ppm(int rc_sub_fd){

	struct input_rc_s input_rc;
	/* copy the data */
	orb_copy(ORB_ID(input_rc),rc_sub_fd,&input_rc);

	for(int i = 0;i < 1;i++){
		printf(" %u",input_rc.values[i]);
	}

}
