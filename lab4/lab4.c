#include <minix/drivers.h>

#include "i8042.h"
#include "test4.h"

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
			"\t service run %s -args \"packet <cntg>\" \n"
			"\t service run %s -args \"async <time>\" \n"
			"\t service run %s -args \"config\" \n"
			"\t service run %s -args \"gesture <length> <tolerance>\" \n",
			argv[0], argv[0], argv[0], argv[0]);
}

static int proc_args(int argc, char *argv[]) {

	unsigned short cnt, time, tolerance;
	short length;

	/* check the function to test: if the first characters match, accept it */
	if (strncmp(argv[1], "packet", strlen("packet")) == 0) {
		if( argc != 3 ) {
			printf("test4: wrong no of arguments for test of test_packet() \n");
			return 1;
		}
		if((cnt = parse_ulong(argv[2], 10)) == ULONG_MAX )
			return 1;
		printf("test4:: test_packet()\n"); /* Actually, it was already invoked */
		test_packet(cnt);
		return 0;
	} else 	if (strncmp(argv[1], "async", strlen("async")) == 0) {
		if( argc != 3 ) {
			printf("test4: wrong no of arguments for test of test_async() \n");
			return 1;
		}
		if((time = parse_ulong(argv[2], 10)) == ULONG_MAX )
			return 1;
		printf("test4:: test_async()\n"); /* Actually, it was already invoked */
		test_async(time);
		return 0;
	} else 	if (strncmp(argv[1], "config", strlen("config")) == 0) {
		if( argc != 2 ) {
			printf("test4: wrong no of arguments for test of test_config() \n");
			return 1;
		}
		printf("test4:: test_config()\n"); /* Actually, it was already invoked */
		test_config();
		return 0;
	} else 	if (strncmp(argv[1], "gesture", strlen("gesture")) == 0) {
		if( argc != 4 ) {
			printf("test4: wrong no of arguments for test of test_gesture() \n");
			return 1;
		}
		if((length = parse_ulong(argv[2], 10)) == ULONG_MAX )
			return 1;
		if((tolerance = parse_ulong(argv[3], 10)) == ULONG_MAX )
			return 1;
		printf("test4:: test_gesture()\n"); /* Actually, it was already invoked */
		test_gesture(length, tolerance);
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
		printf("test3: parse_ulong: no digits were found in %s \n", str);
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
		printf("test3: parse_long: no digits were found in %s \n", str);
		return LONG_MAX;
	}

	/* Successful conversion */
	return val;
}
