#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "parameters.h"

int
main(int argc, char **argv)
{
	char parsed = -1;
	while ((parsed = getopt(argc, argv, "AacdFfhiklnqRrSstuw")) != -1) {
		switch (parsed) {
		case 'A':
			break;
		case 'a':
			break;
		case 'c':    // override: -c / -u
			break;
		case 'd':
			break;
		case 'F':
			break;
		case 'f':
			break;
		case 'h':    // Dependency: -s / -l | override: -k
			break;
		case 'i':
			break;
		case 'k':    // Dependency: -s | override: -h
			break;
		case 'l':    // override: -l / -n
			break;
		case 'n':    // override: -l / -n
			break;
		case 'q':    // override: -q / -w
			break;
		case 'R':
			break;
		case 'r':
			break;
		case 'S':
			break;
		case 's':
			break;
		case 't':
			break;
		case 'u':    // override: -c / -u
			break;
		case 'w':    // override: -q / -w
			break;
		}
	}
	return EXIT_SUCCESS;
}