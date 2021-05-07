#include <stdio.h>

#include "cmdline.h"

int main (int argc, char *argv[])
{
	struct gengetopt_args_info parser_info;
	struct cmdline_parser_params parser_params = *cmdline_parser_params_create();
	cmdline_parser_init(&parser_info);
	cmdline_parser_config_file("default.conf", &parser_info, &parser_params);
	//cmdline_parser(argc, argv, &parser_info);

	if (parser_info.verbose_flag) {
		puts("Being verbose!");
		puts("Argv contents:");
		printf("  %s", argv[0]);
		for (int i = 1; i < argc; i++)
			printf(" %s", argv[i]);
		printf("\n");
	}

	//cmdline_parser_file_save("default.conf", &parser_info);

	return 0;
}