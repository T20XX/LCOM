#include <minix/drivers.h>

#include "handler.h"
//#include "test5.h"
//#include "pixmap.h"
//static int proc_args(int argc, char *argv[]);
//static unsigned long parse_ulong(char *str, int base);
//static long parse_long(char *str, int base);
//static void print_usage(char *argv[]);

int main(int argc, char **argv) {

	/* Initialize service */

	sef_startup();


//	if ( argc == 1 ) {
//		print_usage(argv);
//		return 0;
//	} else {
//		proc_args(argc, argv);
//	}

	mainhandler();

	return 0;

}

//static void print_usage(char *argv[]) {
//	printf("Usage: one of the following:\n"
//			"\t service run %s -args \"init <mode> <delay>\" \n"
//			"\t service run %s -args \"square <x> <y> <size> <color>\" \n"
//			"\t service run %s -args \"line <xi> <yi> <xf> <yf> <color>\" \n"
//			"\t service run %s -args \"xpm <xi> <yi> <xpm>\" \n",
//			"\t service run %s -args \"move <xi> <yi> <xpm> <hor> <delta> <time>\" \n",
//			"\t service run %s -args \"controller\" \n",
//			argv[0], argv[0], argv[0], argv[0], argv[0], argv[0]);
//}
//
//static int proc_args(int argc, char *argv[]) {
//
//	char *xpm;
//	unsigned short mode, delay, x, y, size, xi, yi, xf, yf, hor, time, number;
//	short delta;
//	unsigned long color;
//
//	/* check the function to test: if the first characters match, accept it */
//	if (strncmp(argv[1], "init", strlen("init")) == 0) {
//		if( argc != 4 ) {
//			printf("test5: wrong no of arguments for test of test_init() \n");
//			return 1;
//		}
//		if((mode = parse_ulong(argv[2], 16)) == ULONG_MAX )
//			return 1;
//		if((delay = parse_ulong(argv[3], 10)) == ULONG_MAX )
//			return 1;
//		printf("test5:: test_init()\n"); /* Actually, it was already invoked */
//		test_init(mode, delay);
//		return 0;
//	} else 	if (strncmp(argv[1], "square", strlen("square")) == 0) {
//		if( argc != 6 ) {
//			printf("test5: wrong no of arguments for test of test_square() \n");
//			return 1;
//		}
//		if((x = parse_ulong(argv[2], 10)) == ULONG_MAX )
//			return 1;
//		if((y = parse_ulong(argv[3], 10)) == ULONG_MAX )
//			return 1;
//		if((size = parse_ulong(argv[4], 10)) == ULONG_MAX )
//			return 1;
//		if((color = parse_ulong(argv[5], 10)) == ULONG_MAX )
//			return 1;
//		printf("test5:: test_square()\n"); /* Actually, it was already invoked */
//		test_square(x, y, size, color);
//		return 0;
//	} else 	if (strncmp(argv[1], "line", strlen("line")) == 0) {
//		if( argc != 7 ) {
//			printf("test5: wrong no of arguments for test of test_line() \n");
//			return 1;
//		}
//		if((xi = parse_ulong(argv[2], 10)) == ULONG_MAX )
//			return 1;
//		if((yi = parse_ulong(argv[3], 10)) == ULONG_MAX )
//			return 1;
//		if((xf = parse_ulong(argv[4], 10)) == ULONG_MAX )
//			return 1;
//		if((yf = parse_ulong(argv[5], 10)) == ULONG_MAX )
//			return 1;
//		if((color = parse_ulong(argv[6], 10)) == ULONG_MAX )
//			return 1;
//		printf("test5:: test_line()\n"); /* Actually, it was already invoked */
//		test_line(xi, yi, xf, yf, color);
//		return 0;
//	} else 	if (strncmp(argv[1], "xpm", strlen("xpm")) == 0) {
//		if( argc != 5 ) {
//			printf("test5: wrong no of arguments for test of test_xpm() \n");
//			return 1;
//		}
//		if((xi = parse_ulong(argv[2], 10)) == ULONG_MAX )
//			return 1;
//		if((yi = parse_ulong(argv[3], 10)) == ULONG_MAX )
//			return 1;
//		if((number = parse_ulong(argv[4], 10)) == ULONG_MAX )
//			return 1;
//		if(number == 1)
//			xpm = pic1;
//		if(number == 2)
//			xpm = pic2;
//		if(number == 3)
//			xpm = cross;
//		if(number == 4)
//			xpm = pic3;
//		if(number == 5)
//			xpm = penguin;
//		printf("test5:: test_xpm()\n"); /* Actually, it was already invoked */
//		test_xpm(xi,yi,xpm);
//		return 0;
//	}	else 	if (strncmp(argv[1], "move", strlen("move")) == 0) {
//		if( argc != 8 ) {
//			printf("test5: wrong no of arguments for test of test_move() \n");
//			return 1;
//		}
//		if((xi = parse_ulong(argv[2], 10)) == ULONG_MAX )
//			return 1;
//		if((yi = parse_ulong(argv[3], 10)) == ULONG_MAX )
//			return 1;
//		if((number = parse_ulong(argv[4], 10)) == ULONG_MAX )
//			return 1;
//		if((hor = parse_ulong(argv[5], 10)) == ULONG_MAX )
//			return 1;
//		if((delta = parse_ulong(argv[6], 10)) == ULONG_MAX )
//			return 1;
//		if((time = parse_ulong(argv[7], 10)) == ULONG_MAX )
//			return 1;
//		if(number == 1)
//			xpm = pic1;
//		if(number == 2)
//			xpm = pic2;
//		if(number == 3)
//			xpm = cross;
//		if(number == 4)
//			xpm = pic3;
//		if(number == 5)
//			xpm = penguin;
//		printf("test5:: test_move()\n"); /* Actually, it was already invoked */
//		test_move(xi, yi, xpm, hor, delta, time);
//		return 0;
//	} else if (strncmp(argv[1], "controller", strlen("controller")) == 0) {
//		if( argc != 2) {
//			printf("test5: wrong no of arguments for test of test_controller() \n");
//			return 1;
//		}
//		printf("test5:: test_controller()\n"); /* Actually, it was already invoked */
//		test_controller();
//		return 0;
//	}
//}
//
//static unsigned long parse_ulong(char *str, int base) {
//	char *endptr;
//	unsigned long val;
//
//	val = strtoul(str, &endptr, base);
//
//	if ((errno == ERANGE && val == ULONG_MAX )
//			|| (errno != 0 && val == 0)) {
//		perror("strtol");
//		return ULONG_MAX;
//	}
//
//	if (endptr == str) {
//		printf("test3: parse_ulong: no digits were found in %s \n", str);
//		return ULONG_MAX;
//	}
//
//	/* Successful conversion */
//	return val;
//}
//
//static long parse_long(char *str, int base) {
//	char *endptr;
//	unsigned long val;
//
//	val = strtol(str, &endptr, base);
//
//	if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN))
//			|| (errno != 0 && val == 0)) {
//		perror("strtol");
//		return LONG_MAX;
//	}
//
//	if (endptr == str) {
//		printf("test3: parse_long: no digits were found in %s \n", str);
//		return LONG_MAX;
//	}
//
//	/* Successful conversion */
//	return val;
//}
