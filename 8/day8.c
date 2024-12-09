#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>

int main(){
	FILE *fp = fopen("cal_input.txt", "r");
	if (fp == NULL) {
		exit(EXIT_FAILURE);
	}
	
	char *line = NULL;
	size_t length = 0;

  while (getline(&line, &length, fp) != -1) {
		printf("line: %s \n", line);
	}
}
