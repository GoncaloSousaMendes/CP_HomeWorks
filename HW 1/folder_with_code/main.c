#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "config.h"
#include "game.h"



int main (int argc, char **argv){
    GameConfig* gameC;
    Game* g;
    int o,i = 0;
    int criteria = 0;
    gameC = game_config_new_from_cli(argc, argv);


    if(gameC == NULL){
  	 return 0;
    }
    g= game_new();
    o = game_parse_board(g, gameC);
    if(o != 1)
  	 return 0;

    if(gameC->silent == 0){
      printf("First Iteration\n");
    	game_print_board(g);
    }
    for (i = 0; i< game_config_get_generations(gameC); i++){
      criteria =  game_tick(g);
      if(criteria >0){
        break;
    }
      /*print the board if the debug option is available*/
    	if(gameC->debug == 1 && i < game_config_get_generations(gameC)){
        printf("Iteration n:%d\n", i);
        game_print_board(g);   
        printf("\n");     
      }
    }
    
    if(gameC->silent == 0 && gameC->debug == 0){
      printf("Final Iteration:%d\n", i);
      game_print_board(g);
    }
    game_free(g);
    game_config_free(gameC);
  return 0;
}
