#include <minix/drivers.h>

#include "i8254.h"
#include "timer.h"

static int proc_args(int argc, char *argv[]);
static unsigned long parse_ulong(char *str, int base);
static long parse_long(char *str, int base);
static void print_usage(char *argv[]);

int main(int argc, char **argv) {

  /* Initialize service */
  
  sef_startup();


  if ( argc == 1 ) {
      print_usage(argv);
      return 0;
  } else {   
      proc_args(argc, argv);
  }
  return 0;

}

static void print_usage(char *argv[]) {
  printf("Usage: one of the following:\n"
		  "\t service run %s -args \"config <timer>\" \n"
		  "\t service run %s -args \"square <freq>\" \n"
		  "\t service run %s -args \"int <time>\" \n",
	 argv[0], argv[0], argv[0]);
}

static int proc_args(int argc, char *argv[]) {

	unsigned long timer, freq, time;
	// char *str;
	// long num;

	/* check the function to test: if the first characters match, accept it */
	if (strncmp(argv[1], "config", strlen("config")) == 0) {
		if( argc != 3 ) {
			printf("timer: wrong no of arguments for test of timer_test_config() \n");
			return 1;
		}
		if((timer = parse_ulong(argv[2], 16)) == ULONG_MAX )
				  return 1;
		printf("timer:: timer_test_config()\n"); /* Actually, it was already invoked */
		timer_test_config(timer);
		return 0;
	} else if (strncmp(argv[1], "square", strlen("square")) == 0) {
		if( argc != 3 ) {
			printf("timer: wrong no of arguments for test of timer_test_square() \n");
			return 1;
		}
		if((freq = parse_ulong(argv[2], 16)) == ULONG_MAX )
						  return 1;
		printf("timer:: timer_test_square()\n"); /* Actually, it was already invoked */
		timer_test_square(freq);
		return 0;
	} else if (strncmp(argv[1], "int", strlen("int")) == 0) {
		if( argc != 2 ) {
			printf("timer: wrong no of arguments for test of timer_test_int() \n");
			return 1;
		}
		printf("timer:: timer_test_int()\n"); /* Actually, it was already invoked */
		timer_test_int(time);
		return 0;
	}
}

static unsigned long parse_ulong(char *str, int base) {
  char *endptr;
  unsigned long val;

  val = strtoul(str, &endptr, base);

  if ((errno == ERANGE && val == ULONG_MAX )
	  || (errno != 0 && val == 0)) {
	  perror("strtol");
	  return ULONG_MAX;
  }

  if (endptr == str) {
	  printf("video_txt: parse_ulong: no digits were found in %s \n", str);
	  return ULONG_MAX;
  }

  /* Successful conversion */
  return val;
}

static long parse_long(char *str, int base) {
  char *endptr;
  unsigned long val;

  val = strtol(str, &endptr, base);

  if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN))
	  || (errno != 0 && val == 0)) {
	  perror("strtol");
	  return LONG_MAX;
  }

  if (endptr == str) {
	  printf("video_txt: parse_long: no digits were found in %s \n", str);
	  return LONG_MAX;
  }

  /* Successful conversion */
  return val;
}
