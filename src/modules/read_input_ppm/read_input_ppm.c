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


__EXPORT int read_input_ppm_main(int argc, char *argv[]);

int read_input_ppm_main(int argc, char *argv[]){

	PX4_INFO("Try to read ppm\n");

	/* subscribe to input_rc topic */
	int input_rc_sub_fd = orb_subscribe(ORB_ID(input_rc));
	/* limit the update rate to 10 Hz */
	orb_set_interval(input_rc_sub_fd,100);

	struct input_rc_s input_rc;
	/* copy the data */
	orb_copy(ORB_ID(input_rc),input_rc_sub_fd,&input_rc);


	/*show result*/
	for(int i = 0;i < 18;i++){
		PX4_INFO(" %u",input_rc.values[i]);
	}

	printf("\n");

	PX4_INFO("read ppm end\n");

	return 0;


}
