/*
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "config.h"
#include "game.h"
*/

#include "game.h"

char *old_board;

void game_free(Game *game){
	free(old_board);
	free(game->board);
	free(game);
}

char get_char (Game *game,size_t row, size_t col){
	return old_board[col + (game->cols * row)];
}

int game_cell_is_alive(Game *game, size_t row, size_t col){
	if(get_char(game,row, col) == 35)
	   return 1;
	return 0;
}

int game_cell_is_dead(Game *game, size_t row, size_t col){
	if(get_char(game,row, col) == 46) 
		return 1;
	return 0;
}


Game *game_new(void){
	Game *g = malloc(sizeof(g));
	return g;
}

int game_parse_board(Game *game, GameConfig *config){
    int time = 0;
	char *data = NULL, *tmp;
	int ch, bff,i = 0;
	size_t dest_s;
	while(ch != EOF)
	{
    		if(!(tmp = realloc(data, bff + 2)))
    		{
        		free(data);
        		return 1;
    		}
    		data = tmp;
   		ch = fgetc(config->input_file); 
		if(ch == ':' && time == 0){ 
			fscanf(config->input_file, "%d", &i);
			/*i = fgetc(config->input_file) - '0';*/
			game->rows = i;
			time++;
		}
		else if(ch == ':' && time == 1){
			fscanf(config->input_file, "%d", &i);
			/*i = fgetc(config->input_file) - '0';*/
			game->cols = i;
			time++;
		}
		else if(time>1 && (ch == '.' || ch == '#')){
			data[bff++] = ch;
		}
	}
	dest_s = (strlen(data));
	game->board=malloc(dest_s);
	memcpy(game->board,data, dest_s);
	old_board=malloc(dest_s);
	memcpy(old_board,data, dest_s);
	fclose (config->input_file);
	return 1;
}

void game_print_board(Game *game){
	int v,t = 0;
	int bff = strlen(game->board);
	for(t=0; t<bff;){
		for (v = 0; v < game->cols; v++){
			printf("%c",game->board[t++]);
		}
	printf("\n");
	}
}

void game_cell_set_alive(Game *game, size_t row, size_t col){
	game->board[col + (game->cols * row)] =  35;
}
void game_cell_set_dead(Game *game, size_t row, size_t col){
	game->board[col + (game->cols * row)] = 46;
}

int game_tick(Game *game){
	int i= 0;
	int j= 0;
	int t= 0;
	int g = 0;
	int n_alive = 0;
	int n_dead = 0;
	int changes_flag = 0;
	int number_cells_deads = 0;
	size_t dest_s;
	/*for debuging purposes, x=0 no prints, x=1 prints all*/
	int x = 0;
	for (i = 0; i<game->rows; i++){
	   for (j = 0; j<game->cols; j++){
	   		if(game_cell_is_dead(game,i,j) ==1)
	   			number_cells_deads++;
	    	n_alive = 0;
	    	n_dead = 0;
	    	if(x==1)printf("CELL: i=%d ; j=%d\n",i,j);
	    	for(t=-1; t<2; t++){
	    		if(n_alive > 3)/* more than three alive, we already know the destination of the cell */
			   			break;
		 		for(g=-1; g<2; g++){
		 			if(n_alive > 3)
			   			break;
		 			if(t==0 && g==0){/*refers to it self*/
		 				/*do nothing*/
		 			}
		   			else {
		   				/*for the visits of the first cell on first row*/
		 				if(i==0 && j==0) {
		 					/*visiting diagonal neighbour outside of box*/
		 					if(x==1) printf("First row and colum fetch t:%d ; g:%d\n", t,g);
		 					if(g == -1 && t ==-1){
								if(game_cell_is_alive(game, game->rows-1, game->cols-1) == 1){ 
									n_alive++; if(x==1)printf("alive at t:%d ; g:%d\n", t,g); }
								else n_dead++;
							}
		 					/*for the visit on the upper neighbour*/
		 					else if(t==-1 && g>=0){
		 						if(game_cell_is_alive(game, game->rows-1, j+g) == 1) { 
		 							n_alive++; if(x==1)printf("alive at t:%d ; g:%d\n", t,g);}
						 		else n_dead++;}
						 	/*for the visit of the left neighbours*/
		 					else if(t>=0 && g==-1){
		 						if(game_cell_is_alive(game, i+t, game->cols-1) == 1) { 
		 							n_alive++; if(x==1)printf("alive at t:%d ; g:%d\n", t,g);}
						 		else n_dead++;}
						 	/*all others*/
						 	else{
								if(game_cell_is_alive(game, i+t, j+g) == 1) { 
								n_alive++; if(x==1)printf("alive at t:%d ; g:%d\n", t,g);}
								else n_dead++;}
		 				}
		 				/*visits for the last cell on first row*/
		 				else if(i==0 && j==game->cols-1){
		 					if(x==1)printf("First row last colum fetch t:%d ; g:%d\n", t,g);
		 					/*visiting diagonal neighbour outside of box*/
		 					if(t == -1 && g==1){
		 						if(game_cell_is_alive(game, game->rows -1, 0) == 1) { 
		 							n_alive++; if(x==1)printf("alive at t:%d ; g:%d\n", t,g);}
		 						else n_dead++;}
		 					/*visiting the right neighbours*/
		 					else if(g==1 && t>=0){
		 						if(game_cell_is_alive(game, i+t, 0) == 1) { 
		 							n_alive++; if(x==1) printf("alive at t:%d ; g:%d\n", t,g);}
		 						else n_dead++;}
		 					/*visiting the upper neighbours*/
		 					else if(g<1 && t==-1){
		 						if(game_cell_is_alive(game, game->rows-1, j+g) == 1) { 
		 							n_alive++; if(x==1)printf("alive at t:%d ; g:%d\n", t,g);}
		 						else n_dead++;}
		 						/*all others*/
						 	else{
								if(game_cell_is_alive(game, i+t, j+g) == 1) { 
									n_alive++; if(x==1)printf("alive at t:%d ; g:%d\n", t,g);}
								else n_dead++;}
		 				}
						 /*visits for first cell on last row*/
		 				else if(i==game->rows-1 && j==0){
		 					if(x==1)printf("Last row first colum fetch t:%d ; g:%d\n", t,g);
		 					/*visiting diagonal neighbour outside of box*/
		 					if(t == 1 && g==-1){
		 						if(game_cell_is_alive(game, 0, game->cols-1) == 1) { 
		 							n_alive++; if(x==1)printf("alive at t:%d ; g:%d\n", t,g);}
		 						else n_dead++;}
		 					/*visiting the left neighbours*/
		 					else if(g==-1 && t<1){
		 						if(game_cell_is_alive(game, i+t, game->cols-1) == 1) { 
		 							n_alive++; if(x==1)printf("alive at t:%d ; g:%d\n", t,g);}
		 						else n_dead++;}
		 					/*visiting the down neighbours*/
		 					else if(g>=0 && t==1){
		 						if(game_cell_is_alive(game, 0, j+g) == 1) { 
		 							n_alive++; if(x==1)printf("alive at t:%d ; g:%d\n", t,g);}
		 						else n_dead++;}
		 						/*all others*/
						 	else{
								if(game_cell_is_alive(game, i+t, j+g) == 1) { 
									n_alive++; if(x==1)printf("alive at t:%d ; g:%d\n", t,g);}
								else n_dead++;}
		 				}
						 /*visits for last cell on last row row*/
		 				else if(i==game->rows-1 && j==game->cols-1){
		 					if(x==1)printf("Last row last colum fetch t:%d ; g:%d\n", t,g);
		 					/*visiting diagonal neighbour outside of box*/
		 					if(t == 1 && g==1){
		 						if(game_cell_is_alive(game, 0, 0) == 1) { 
		 							n_alive++; if(x==1)printf("alive at t:%d ; g:%d\n", t,g);}
		 						else n_dead++;}
		 					/*visiting the right neighbours*/
		 					else if(g==1 && t<1){
		 						if(game_cell_is_alive(game, i+t, 0) == 1) { 
		 							n_alive++; if(x==1)printf("alive at t:%d ; g:%d\n", t,g);}
		 						else n_dead++;}
		 					/*visiting the down neighbours*/
		 					else if(g<1 && t==1){
		 						if(game_cell_is_alive(game, 0, j+g) == 1) { 
		 							n_alive++; if(x==1)printf("alive at t:%d ; g:%d\n", t,g);}
		 						else n_dead++;}
		 						/*all others*/
						 	else{
								if(game_cell_is_alive(game, i+t, j+g) == 1) { 
									n_alive++; if(x==1)printf("alive at t:%d ; g:%d\n", t,g);}
								else n_dead++;}
		 				}
						 /*First row visiting upper neighbours*/
						else if(i==0 && t==-1){ 
							if(x==1)printf("First row fetch t:%d ; g:%d\n", t,g);
						 	if(game_cell_is_alive(game, game->rows-1, j+g) == 1) { 
						 		n_alive++; if(x==1)printf("alive at t:%d ; g:%d\n", t,g);}
						 	else n_dead++;
						}
						/*Last row visiting down neighbours*/
						else if(i==game->rows-1 && t==1){
							if(x==1)printf("Last row fetch t:%d ; g:%d\n", t,g);
							if(game_cell_is_alive(game, 0, j+g) == 1) { 
								n_alive++; if(x==1)printf("alive at t:%d ; g:%d\n", t,g);}
						 	else n_dead++;
						}
						/*First colum visiting left neighbours*/
						else if(j==0 && g==-1){
							if(x==1)printf("First colum fetch t:%d ; g:%d\n", t,g);
							if(game_cell_is_alive(game, i+t,game->cols-1 ) == 1) { 
								n_alive++; if(x==1)printf("alive at t:%d ; g:%d\n", t,g);}
						 	else n_dead++;
						}
						/*Last colum visiting right neighbours*/
						else if(j==game->cols-1 && g==1){
							if(x==1)printf("Last colum fetch t:%d ; g:%d\n", t,g);
							if(game_cell_is_alive(game, i+t, 0) == 1) { 
								n_alive++; if(x==1)printf("alive at t:%d ; g:%d\n", t,g);}
						 	else n_dead++;
						}
						/*General case*/
						else {
							if(x==1)printf("General case fetch: t=%d ; g=%d\n", t,g);
							if(game_cell_is_alive(game, i+t, j+g) == 1) { 
								n_alive++; if(x==1)printf("alive at t:%d ; g:%d\n", t,g);}
							else n_dead++;}
			   		}
		 	    }
		 	}

		 	if(x==1)printf("Number alive:%d ; Number dead:%d and its alive:%d\n",n_alive, n_dead, game_cell_is_alive(game, i, j));
			if( (game_cell_is_alive(game, i, j) == 1) && (n_alive >= 4 || n_alive<=1)){
		 	 	/*more than three neighbours or less than two -> dies if alive*/
		 	 	if(x==1)printf("Dies\n\n");
	     		game_cell_set_dead(game, i,j);
	     		if(changes_flag == 0)
	     			changes_flag = 1;
	     	}
	    	else if(game_cell_is_dead(game, i, j) == 1 && n_alive == 3){
	    		/*more than three neighbours -> alive if dead*/
	    		if(x==1)printf("Reborn\n\n");
	    		game_cell_set_alive(game, i,j);
	    		if(changes_flag == 0)
	     			changes_flag = 1;
	    	} 
	    	/*cell with two or three neighbours survives*/
	    	else if(x==1){
	    		if( (game_cell_is_alive(game, i, j) == 1))
	    			printf("Survives\n\n");
	    		else printf("Stays dead\n\n");
	    	}

	    }
	}

	if(x==1)printf("i:%d ; j:%d\n",i,j);
	if(changes_flag == 0)
	    return 1;
	if(number_cells_deads == (game->cols*game->rows))
		return 1;
	dest_s = (strlen(game->board));
	memcpy(old_board,game->board, dest_s);
	return 0;
}







