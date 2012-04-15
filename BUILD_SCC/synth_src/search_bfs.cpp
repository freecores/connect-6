
/*

connectk -- UMN CSci 5512W project

*/

//#include "config.h"
//#include <string.h>
//#include <glib.h>
//#include <iostream>
//#include <stdio.h>
#include "./shared.h"
#include "pico.h"
//#include "../connectk.h"

/* Variables required to check for cache hits */
//static int cache_id = -1, cache_depth = -1, cache_branch = -1;
//static SEARCH cache_search = -1;
//static AIMoves *cache_moves = NULL;
//static AIWEIGHT cache_best;
//static Player *cache_player;
//static AIFunc cache_func;
//static BCOORD cache_size;

int ai_stop=0;
int mini(int x,int y){
	return (x<=y)?x:y;
}
int maxi(int x,int y){
	return (x>=y)?x:y;
}
static AIWEIGHT df_search(Board *b, AIMoves *moves,index_array *index, Player *player,
                          int depth, int cache_index,
                          PIECE searched, AIWEIGHT alpha, AIWEIGHT beta)
/* Depth is in _moves_ */
{
		#pragma internal_fast index
        int i, j;
	#pragma bitsize i 16
	#pragma bitsize j 16
        Board b_next[5][16];
	#pragma internal_blockram b_next
        AIMoves moves_next[5][16];
	#pragma internal_fast moves_next
	AIWEIGHT utility[5][16];
	int branch=player->branch;

        board_copy(b, &b_next[0][0]);
	ai_threats(b_next,0,0,moves_next,index);


        ///* Search only the top moves beyond the minimum */
        ////aimoves_sort(moves);
        //if (moves->len > player->branch) {
        //        //for (i = player->branch; i < moves->len; i++)
        //        //        if (moves->data[i].weight != moves->data[0].weight)
        //        //                break;
        //        //moves->len = i;
        //        moves->len = player->branch;
        //}

        /* No moves left -- its a draw */
	
        if (moves_next[0][0].len < 1)                //"(%s)", bcoords_to_string(aim->x, aim->y));

                return AIW_DRAW;
                //board_copy(b, &b_next[0][0]);

        /* Search each move available in depth first order */
	for(j=0;j<depth;j++){
		int k,branches=1;
		for(k=0;k<j+1;k++) branches*=branch;
		//int branches=(player->branch)^j;
		//printf("branches %d\n",branches);
        for (i = 0; i < branches; i++) {
		//if(!(moves_next[j][i>>1].utility==AIW_WIN || moves_next[j][i>>1].utility==-AIW_WIN)){
		if(!(utility[j][i>>1]==AIW_WIN || utility[j][i>>1]==-AIW_WIN)){
                AIMove aim = *(moves_next[j][i>>1].data + (i % branch));
		//printf ("aim->utility %d \n",moves_next[j][i>>1].utility);
		
                board_copy(&b_next[j][i>>1], &b_next[j+1][i]);
		//if(moves_next[j][i/2].len<branch) printf ("caca");
		//printf("%d %d\n",aim.x,aim.y);

                /* Did we get a bad move? */
                if (!piece_empty(piece_at(&b_next[j+1][i], aim.x, aim.y))) {
                        //g_warning("DFS utility function suggested a bad move "
                                  //"(%s)", bcoords_to_string(aim->x, aim->y));
			//printf("bad move\n");
                        continue;
                }

                /* Already searched here? */
                ///////////////////////////if (piece_at(&b_next[j+1][i], aim->x, aim->y) == searched){
		///////////////////////////	moves_next[j+1][i].utility=moves_next[j+1][i>>1].utility;
                ///////////////////////////        continue;
		///////////////////////////}
                ///////////////////////////place_piece_type(&b_next[j+1][i], aim->x, aim->y, searched);

                //b_next = board_new();
                place_piece(&b_next[j+1][i], aim.x, aim.y);
                        AIWEIGHT next_alpha = alpha, next_beta = beta;
                        //AIFunc func;


                        /* Player has changed */
                        if (b_next[j+1][i].moves_left <= 0) {
                                b_next[j+1][i].moves_left = place_p;
                                b_next[j+1][i].turn = other_player(b->turn);
                                searched++;
                                next_alpha = -beta;
                                next_beta = -alpha;
                        }
                        b_next[j+1][i].moves_left--;

                /* Did we win? */
		
                if (check_win_full(&b_next[j+1][i], aim.x, aim.y,0,0,0,0)){
                        aim.weight = AIW_WIN;
	        	moves_next[j+1][i].utility=AIW_WIN;
	        	utility[j+1][i]=AIW_WIN;
			

	        }else if(moves_next[j][i>>1].utility==AIW_WIN || moves_next[j][i>>1].utility==-AIW_WIN ){
	        	//moves_next[j+1][i].utility=AIW_WIN;
	        	utility[j+1][i]=AIW_WIN;
                /* Otherwise, search deeper */
                }else  {

                        //func = ai(player->ai)->func;
                        //if (!func) {
                        //        g_warning("DFS player has no AI function");
                        //        return moves->utility;
                        //}
                        //moves_next = func(b_next);
	        	ai_threats(b_next,j+1,i,moves_next,index);
			utility[j+1][i]=moves_next[j+1][i].utility;
	        	
                        //aim->weight = df_search(&b_next, &moves_next, index,player,
                        //                        depth - 1, next_ci, searched,
                        //                        next_alpha, next_beta);
                        //aimoves_free(moves_next);
                }
                        if (b_next[j+1][i].turn != b->turn)
	        		//moves_next[j+1][i].utility=-moves_next[j+1][i].utility;
	        		utility[j+1][i]=-moves_next[j+1][i].utility;
	        	//if (moves_next[j+1][i].utility >= AIW_WIN)
	        	//	moves_next[j+1][i].utility=AIW_WIN;

                /* Debug search */
                //if (opt_debug_dfsc) {
                //        for(j = MAX_DEPTH - depth; j > 0; j--)
                //                //g_print("-");
                //        //g_print("> d=%d, %s, u=%d, a=%d, b=%d %s\n",
                //        //        depth, bcoords_to_string(aim->x, aim->y),
                //        //        aim->weight, alpha, beta,
                //        //        piece_to_string(b->turn));
                //}

                //board_free(b_next);
                //if (aim->weight > alpha) {
                //        alpha = aim->weight;
                //        //cache_set(cache_index, aim);

                //        /* Victory abort */
                //        if (alpha >= AIW_WIN)
                //                return AIW_WIN;

                //        /* Alpha-beta pruning */
                //        if (alpha >= beta)
                //                return alpha;
                //}
	//printf("%d %d %d\n",j,i,moves_next[j+1][i].utility);
		}else //moves_next[j+1][i].utility=AIW_WIN;
			utility[j+1][i]=AIW_WIN;
        }
	}
	for(j=depth-1;j>0;j--){
		int k,branches=1;
		for(k=0;k<j+1;k++) branches*=branch;
		//int branches=(player->branch)^j;
		//printf("branches %d player %d\n",branches,b_next[j+1][i].turn);
        for (i = 0; i < branches; i=i+2) {
		if (b_next[j+1][i].turn != b->turn)
		//moves_next[j][i>>1].utility=mini(moves_next[j+1][i].utility,moves_next[j+1][i+1].utility);
		utility[j][i>>1]=mini(utility[j+1][i],utility[j+1][i+1]);
		else 
		//moves_next[j][i>>1].utility=maxi(moves_next[j+1][i].utility,moves_next[j+1][i+1].utility);
		utility[j][i>>1]=maxi(utility[j+1][i],utility[j+1][i+1]);
	
	//printf("%d %d\n",moves_next[j+1][i].utility,moves_next[j+1][i+1].utility);
	}
	}
	
	//for(i=0;i<branch;i++){
	//moves_next[0][0].data[i].x=moves->data[i].x;
	//moves_next[0][0].data[i].y=moves->data[i].y;
	//moves_next[0][0].data[i].weight=moves->data[i].weight;
	//}
	//moves_next[0][0].utility=moves->utility;
	//moves_next[0][0].len=branch;
	for(i=0;i<branch;i++){
	moves->data[i].x=moves_next[0][0].data[i].x;
	moves->data[i].y=moves_next[0][0].data[i].y;
	//moves->data[i].weight=moves_next[1][i].utility;
	moves->data[i].weight=utility[1][i];
	}
	moves->len=branch;

        return alpha;
}

int  search(Board *b, AIMove *move, Player *player)
{
	AIMoves moves;
	#pragma internal_blockram moves
	moves.len=0;
        Board copy;
	#pragma internal_blockram copy
	index_array index={0};
		#pragma internal_fast index
        //AIFunc move_func = ai(player->ai)->func;

        /* Player is not configured to search */
        //if (player->search == SEARCH_NONE)
        //        return;

        /* Moves list does not need to be searched */
        //if (moves->len <= b->moves_left) {
        ////        if (opt_debug_dfsc)
        ////                g_debug("DFS no choice abort");
        //        return;
        //}

        ///* Board size changed, cache is invalidated */
        //if (board_size != cache_size)
        //        cache_moves = NULL;
        //cache_size = board_size;

        ///* Cache hit, last or same board */
        //if (player->cache && cache_moves && cache_moves->len &&
        //    cache_search == player->search &&
        //    cache_depth == player->depth &&
        //    cache_player == player &&
        //    cache_func == move_func &&
        //    cache_branch == player->branch) {
        //        if (b->parent && cache_id == b->parent->ac.id) {
        //                aimoves_remove(cache_moves, b->parent->move_x,
        //                               b->parent->move_y);
        //                cache_id = b->ac.id;
        //        }
        //        if (cache_id == b->ac.id && cache_moves->len) {
        //                if (cache_moves->len) {
        //                        aimoves_copy(cache_moves, moves);
        //                        if (opt_debug_dfsc)
        //                                g_debug("DFS cache HIT");
        //                        return;
        //                }
        //                aimoves_free(cache_moves);
        //                cache_moves = NULL;
        //        }
        //}

        /* Cache miss */
        //if (opt_debug_dfsc)
        //        g_debug("DFS cache MISS");
        //cache_id = b->ac.id;
        //if (!cache_moves)
        //        cache_moves = aimoves_new();
        //cache_moves->len = 0;
        //cache_best = AIW_MIN;
        //copy = board_new();
        board_copy(b, &copy);
	//ai_threats(&copy,&moves,&index);

        //if (player->search == SEARCH_DFS) {
                df_search(&copy, &moves, &index,player, player->depth, 0,
                          PIECE_SEARCHED, AIW_LOSE, AIW_WIN);
	//printf("%d %d \n",moves.data[0].weight,moves.data[1].weight);
	int ret_val;
	ret_val=aimoves_choose(&moves, move,&index);
	if (!ret_val)
		return 0;
	else return 1;		
          //      if (cache_moves->len)
          //              aimoves_copy(cache_moves, moves);
        //} else {
        //        board_free(copy);
        //        g_warning("Unsupported search type %d", player->search);
        //        return;
        //}
        //board_free(copy);

        ///* Debug DFS search */
        //if (opt_debug_dfsc)
        //        dfs_cache_dump();

        ///* Save params so we can check if we have a hit later */
        //cache_player = player;
        //cache_search = player->search;
        //cache_depth = player->depth;
        //cache_branch = player->branch;
        //cache_func = move_func;
}
