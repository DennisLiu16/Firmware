/**
 * @file read_input_ppm.c
 * This C file try to read input from <input_rc.msg>
 * @author Dennis <mail@example.com>
 */
#include <px4_platform_common/px4_config.h>
#include <px4_platform_common/tasks.h>
#include <px4_platform_common/posix.h>
#include <px4_platform_common/module.h>

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
static bool _run = 0;
static int  input_rc_sub_fd = 0;
volatile bool _monitor = 0;

//function
int read_input_ppm_task_main(int argc, char *argv[]);
int start(int argc, char *argv[]);
void stop(void);

void monitor(int);
void copy_input_ppm(int);

//export
__EXPORT int read_input_ppm_main(int argc, char *argv[]);



int read_input_ppm_main(int argc, char *argv[]){

	if(argc < 2){
		PX4_ERR("please check number of var\n");
	}

	if(!strcmp(argv[1],"status")){

		printf("\n\n");
		printf("Cmd include\n");
		printf("start : init all\n");
		printf("monitor : check ppm\n");
		printf("status : printf cmd line\n");
		printf("stop : stop this process\n");
		printf("\n\n");

	}
	if(!strcmp(argv[1],"start")){

		start(argc-1,argv+1);
		return OK;

	}

	if (!strcmp(argv[1], "monitor")) {

		if(_run){
			_monitor = true;
		}
		else
			PX4_ERR("didn't start\n");


	}

	if (!strcmp(argv[1], "stop")) {
		stop();
	}

	return 0;

}

void
monitor(int rc_sub_fd){

	/*clean first*/
	printf("\033[2J");

	/*show result*/
	while(_run){
		printf("\033[2J\033[H");
		printf("read values are:");
		copy_input_ppm(rc_sub_fd);

		usleep(100);
	}

	exit(0);

}

void
copy_input_ppm(int rc_sub_fd){

	struct input_rc_s input_rc;
	/* copy the data */
	orb_copy(ORB_ID(input_rc),rc_sub_fd,&input_rc);

	for(int i = 0;i < 1;i++){
		printf(" %u",input_rc.values[i]);
	}
	printf("\n");

}




// cmd function
int
read_input_ppm_task_main(int argc, char *argv[]){

	/* subscribe to input_rc topic */
	input_rc_sub_fd = orb_subscribe(ORB_ID(input_rc));
	/* limit the update rate to 10 Hz */
	orb_set_interval(input_rc_sub_fd,100);
	while(_run){
		if(_monitor){

			PX4_INFO("Try to read ppm\n");
			monitor(input_rc_sub_fd);
		}

	}
	return 0;

}

int
start(int argc, char *argv[]){
	if(_run){
		PX4_ERR("already start\n");
		return 0;
	}

	else{
		/*build new task*/
		int _task = px4_task_spawn_cmd("read_input_ppm",
						SCHED_DEFAULT,
						SCHED_PRIORITY_PARAMS,
						1500,
						(px4_main_t)&read_input_ppm_task_main,
						(char * const *) argv
		);

		if(_task < 0) {
			PX4_ERR("task start failed: %d", errno);
			return -errno;
		}

		/*start flag*/
		_run = true;

		PX4_INFO("read start\n");

	}
	return 0;
}
void
stop(void){
	if(_run){
		_run = false;
		_monitor = false;
		orb_unsubscribe(input_rc_sub_fd);
		printf("\033[2J\033[H");

		PX4_INFO("Leaving\n");

		exit(0);
	}
	else
		PX4_ERR("didn't start\n");
}
