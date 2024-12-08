#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void add_equation_shallow(equation_list_t* equation_list, equation_t equation) {
	if (equation_list->count >= equation_list->size) {
		equation_list->size = (equation_list->size == 0 ? 1 : 2 * equation_list->size);
		equation_list->equations = (equation_t *) realloc(equation_list->equations, equation_list->size * sizeof(equation_t));
		if (!equation_list->equations) {
			exit(EXIT_FAILURE);
		}
	}
	(equation_list->equations)[equation_list->count++] = equation;
}

constexpr size_t linecount = 9;

const char* unparsed_equations[linecount] = {
	"190: 10 19",
	"3267: 81 40 27",
	"83: 17 5",
	"156: 15 6",
	"7290: 6 8 6 15",
	"161011: 16 10 13",
	"192: 17 8 14",
	"21037: 9 7 18 13",
	"292: 11 6 16 20"
};

void print_equation(equation_t *equation) {
	printf("%li:", equation->outcome);	
	for (size_t i = 0; i != equation->count; i++) {
		printf(" %ld", equation->values[i]);
	}
	printf(equation->possible ? " -> possible" : " -> impossible");
	printf("\n");
};

void print_equation_list(equation_list_t* equation_list) {
	for (size_t i = 0; i != equation_list->count; i++) {
		equation_t *equation = &equation_list->equations[i];
		print_equation(equation);
	}
};

bool validate(equation_t* equation) {
		for (unsigned int n = 0; n < (1u << equation->count); ++n) {
			unsigned int num = n;
			unsigned long long outcome = equation->values[0];
			for (unsigned int bit = 1; bit != equation->count; ++bit) {
				int flag = num & 0x01;
				if (flag == 0) {
					outcome += equation->values[bit];
				} else {
					outcome *= equation->values[bit];
				}
				num = num >> 1;
			}
			if (outcome == equation->outcome) {
				equation->possible = true;
				return true;
			}
		}	
		equation->possible = false;
		return false;
}

int main()
{
	equation_list_t equation_list = {
		.equations = NULL,
		.count = 0,
		.size = 0
	};

	FILE *fp = fopen("input.txt", "r");
	if (fp == NULL) {
		exit(EXIT_FAILURE);
	}
	
	char *line = NULL;
	size_t length = 0;
	ssize_t read;
	unsigned long total = 0;
	/* for (int i = 0; i != linecount; i++) { */
  while ((read = getline(&line, &length, fp)) != -1) {
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

		validate(&equation);

		if (equation.possible) {
			total += equation.outcome;
		}

		add_equation_shallow(&equation_list, equation);
	}
	/* print_equation_list(&equation_list); */
	printf("total = %li\n", total);
}
