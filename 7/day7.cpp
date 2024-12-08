#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const size_t _max_digits_in_long_int_ = 21;

typedef struct {
	unsigned long outcome;
	unsigned long *values;
	size_t count;
	size_t size;
	bool possible;
} equation_t;

typedef struct {
	equation_t* equations;
	size_t count;
	size_t size;
} equation_list_t;

void add_value(equation_t* equation, unsigned long value) {
	if (equation->count >= equation->size) {
		equation->size = (equation->size == 0 ? 1 : 2 * equation->size);
		equation->values = (unsigned long *) realloc(equation->values, equation->size * sizeof(unsigned long));
		if (!equation->values) {
			exit(EXIT_FAILURE);
		}
	}
	(equation->values)[equation->count++] = value;
}

long int concat(long int left, long int right) {
	char ch_l[_max_digits_in_long_int_];	
	snprintf(ch_l, _max_digits_in_long_int_, "%li", left);	

	char ch_r[_max_digits_in_long_int_];	
	snprintf(ch_r, _max_digits_in_long_int_, "%li", right);	

	size_t out_sz = strlen(ch_l) + strlen(ch_r) + 1;
	char out[out_sz];

	snprintf(out, out_sz, "%s%s", ch_l, ch_r);

	return strtol(out, NULL, 10);
}

void validate_combinations(equation_t* equation, size_t index, unsigned long out) {
	if (index == equation->count - 1) {
		if (out == equation->outcome) {
			equation->possible = true;
		}
		return;
	}
	validate_combinations(equation, index + 1, out + equation->values[index + 1]);
	validate_combinations(equation, index + 1, out * equation->values[index + 1]);
	validate_combinations(equation, index + 1, concat(out, equation->values[index + 1]));
}

int main()
{
	FILE *fp = fopen("input.txt", "r");
	if (fp == NULL) {
		exit(EXIT_FAILURE);
	}
	
	char *line = NULL;
	size_t length = 0;
	unsigned long total = 0;

  while (getline(&line, &length, fp) != -1) {

		equation_t equation = {
			.outcome = 0,
			.values = NULL,
			.count = 0,
			.size = 0,
			.possible = false
		};

		char *unparsed_equation = strdup(line);

		char *tokens = strtok(unparsed_equation, ":"); 
		equation.outcome = strtoul(tokens, NULL, 10);

		tokens = strtok(NULL, " ");
		while (tokens) {
			add_value(&equation, strtoul(tokens, NULL, 10));
			tokens = strtok(NULL, " ");
		}

		validate_combinations(&equation, 0, equation.values[0]);

		if (equation.possible) {
			total += equation.outcome;
		} 

		free(unparsed_equation);
		free(equation.values);
	}
	printf("total = %li\n", total);
	
	// cleanup
	free(line);
	fclose(fp);
}
