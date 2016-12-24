#define _POSIX_C_SOURCE 2
/*
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
*/
#include "game.h"
#include "config.h"


void game_config_free(GameConfig *config){
	free(config);
}

size_t game_config_get_generations(GameConfig *config){
	return config->generations;
}

GameConfig *game_config_new_from_cli(int argc, char *argv[]){
  int c;
  char *ss;
  GameConfig* gcf = malloc(sizeof(GameConfig));
  gcf->debug = 0;
  gcf->silent = 0;
  gcf->generations = 20;
  opterr = 0;

  while ((c = getopt (argc, argv, "dsn::")) != -1)
    switch (c)
      {
      case 'd':
	gcf->debug = 1;
        break;

     case 'n':
	if(optarg != NULL)
		gcf->generations = atol(optarg);	
	break;

     case 's':
	gcf->silent = 1;
	break;

      case '?':
	if (isprint (optopt))
            fprintf (stderr, "Unknown option `-%c'.\n", optopt);

        else 
            fprintf (stderr,"Unknown option character `\\x%x'.\n",optopt);
	
      default:
        abort ();
      }
	if(gcf->debug == 1 && gcf->silent == 1){
		printf("cannot have both flags!");
		return NULL;
	}
	ss = argv[optind];
	gcf->input_file = fopen(ss, "r");
	if (gcf->input_file == NULL){
    		printf("file does not exists %s\n", ss);
    		return NULL;
	}
return gcf;
}
