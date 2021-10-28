#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <raylib3.5/raylib.h>

#include <z_/types/arr.h>
#include <z_/types/vector.h>
#include <z_/types/type.h>
#include <z_/types/typeof.h>
#include <z_/types/autotype.h>

#include <z_/types/enum.h>
#include <z_/types/string.h>
#include <z_/types/result.h>
#include <z_/types/option.h>


typedef struct {
	z__Vint2 pos;
	z__u8 color;
	z__i8 oriantaion;
	char *name;
	z__u8 data[4][4][4];
} Block;

typedef struct {
	z__Vint2 size;
	z__u8 *data;
	z__u8 *color_data;
} Map;

typedef struct {
	z__u32 score;
	z__float speed;
	z__u32 level;
	z__u64 time;
	z__u8 blockdrops;
} GameState;

Map Map_new(z__u32 x, z__u32 y)
{
	Map map = {
		(z__Vint2){x, y},
		NULL,
		NULL
	};

	map.data = z__New0(z__typeof(*map.data), x*y);
	map.color_data = z__New0(z__typeof(*map.color_data), x*y);

	return map;
}

#define BLOCKS_TOTAL 7
Block
	Tetris_Blocks[BLOCKS_TOTAL] = {

		{
			{0}, 0, 0,
			"Line",
			{
				{
					{0, 1, 0, 0},
					{0, 1, 0, 0},
					{0, 1, 0, 0},
					{0, 1, 0, 0}
				},
				{
					{0, 0, 0, 0},
					{0, 0, 0, 0},
					{0, 0, 0, 0},
					{1, 1, 1, 1}
				},
				{
					{0, 1, 0, 0},
					{0, 1, 0, 0},
					{0, 1, 0, 0},
					{0, 1, 0, 0}
				},
				{
					{0, 0, 0, 0},
					{0, 0, 0, 0},
					{0, 0, 0, 0},
					{1, 1, 1, 1}
				}
			}
		},
		{
			{0}, 0, 0,
			"Square",
			{{
				{0, 0, 0, 0},
				{0, 0, 0, 0},
				{0, 1, 1, 0},
				{0, 1, 1, 0}
			},

			{
				{0, 0, 0, 0},
				{0, 0, 0, 0},
				{0, 1, 1, 0},
				{0, 1, 1, 0}
			},
			{
				{0, 0, 0, 0},
				{0, 0, 0, 0},
				{0, 1, 1, 0},
				{0, 1, 1, 0}
			},

			{
				{0, 0, 0, 0},
				{0, 0, 0, 0},
				{0, 1, 1, 0},
				{0, 1, 1, 0}
			}},

		},
		{
			{0}, 0, 0,
			"Squizle",
			{{
				{0, 0, 0, 0},
				{0, 0, 0, 0},
				{0, 0, 1, 1},
				{0, 1, 1, 0}
			},

			{
				{0, 0, 0, 0},
				{0, 1, 0, 0},
				{0, 1, 1, 0},
				{0, 0, 1, 0}
			},
			{
				{0, 0, 0, 0},
				{0, 0, 0, 0},
				{0, 0, 1 ,1},
				{0, 1, 1, 0}
			},

			{
				{0, 0, 0, 0},
				{0, 1, 0, 0},
				{0, 1, 1, 0},
				{0, 0, 1, 0}
			}}
		},
		{
			{0}, 0, 0,
			"Squizle Reverse",
			{{
				{0, 0, 0, 0},
				{0, 0, 0, 0},
				{0, 1, 1, 0},
				{0, 0, 1, 1}
			},

			{
				{0, 0, 0, 0},
				{0, 0, 1, 0},
				{0, 1, 1, 0},
				{0, 1, 0, 0}
			},
			{
				{0, 0, 0, 0},
				{0, 0, 0, 0},
				{0, 1, 1, 0},
				{0, 0, 1, 1}
			},

			{
				{0, 0, 0, 0},
				{0, 0, 1, 0},
				{0, 1, 1, 0},
				{0, 1, 0, 0}
			}}
		},
		{
			{0}, 0, 0,
			"The L",
			{{
				{0, 1, 0, 0},
				{0, 1, 0, 0},
				{0, 1, 0, 0},
				{0, 1, 1, 0}
			},

			{
				{0, 0, 0, 0},
				{0, 0, 0, 0},
				{0, 0, 0, 1},
				{1, 1, 1, 1}
			},
			{
				{0, 0, 0, 0},
				{0, 0, 0, 0},
				{1, 1, 1, 1},
				{1, 0, 0, 0}
			},

			{
				{0, 1, 1, 0},
				{0, 0, 1, 0},
				{0, 0, 1, 0},
				{0, 0, 1, 0}
			}}
		},
		{
			{0}, 0, 0,
			"The L Reverse",
			{{
				{0, 0, 1, 0},
				{0, 0, 1, 0},
				{0, 0, 1, 0},
				{0, 1, 1, 0}
			},

			{
				{0, 0, 0, 0},
				{0, 0, 0, 0},
				{1, 0, 0, 0},
				{1, 1, 1, 1}
			},
			{
				{0, 0, 0, 0},
				{0, 0, 0, 0},
				{1, 1, 1, 1},
				{0, 0, 0, 1}
			},

			{
				{0, 1, 1, 0},
				{0, 1, 0, 0},
				{0, 1, 0, 0},
				{0, 1, 0, 0}
			}}
		},
		{
			{0}, 0, 0,
			"Knob",
			{{
				{0, 0, 0, 0},
				{0, 1, 0, 0},
				{0, 1, 1, 0},
				{0, 1, 0, 0}
			},

			{
				{0, 0, 0, 0},
				{0, 0, 0, 0},
				{0, 1, 0, 0},
				{1, 1, 1, 0}
			},

			{
				{0, 0, 0, 0},
				{0, 1, 0, 0},
				{1, 1, 0, 0},
				{0, 1, 0, 0}
			},

			{
				{0, 0, 0, 0},
				{0, 0, 0, 0},
				{1, 1, 1, 0},
				{0, 1, 0, 0}
			}}
		}

};

#define COLORS_TOTAL 10
Color color[] = { RED, BLUE, DARKBLUE, BROWN, DARKBLUE, DARKGREEN, GREEN, DARKPURPLE, PURPLE, PINK };


#define map_get(map, x1, y1) (map)->data[(((map)->size.x * (y1)) + x1)]
#define map_color_get(map, x1, y1) (map)->color_data[(((map)->size.x * (y1)) + x1)]
#define block_get(blk, x1, y1)	   (blk)->data[(blk)->oriantaion][y1][x1]

#define WINDOW_X 500
#define WINDOW_Y 600

static int _init(void)
{
	InitWindow(WINDOW_X, WINDOW_Y, "Tetrix");
	

	SetWindowState(FLAG_VSYNC_HINT);
	SetTargetFPS(30);

	//gDefaultFont = LoadFont("data/fonts/FiraCode-Regular.ttf");

	return 0;
}

void draw_screem(Map const * const map, Block const * const blk, Block const * const blk_next, GameState const * const gstate)
{

	z__Vector2 boxsize = {
		.y = WINDOW_Y / map->size.y,
		.x = WINDOW_Y / map->size.y,
	};

	DrawRectangle(0, 0, (boxsize.x * map->size.x) + boxsize.x*2, WINDOW_Y, WHITE);
	DrawRectangle(boxsize.x, 0, (boxsize.x * map->size.x), WINDOW_Y - boxsize.y*2, BLACK);
	for(int i = 0; i < map->size.x; i++) {
		for(int j = 0; j < map->size.y; j++) {
			if(map_get(map, i, j) == 1) {
				DrawRectangle((i * boxsize.x) + boxsize.x, j * boxsize.y, boxsize.x, boxsize.y, color[map_color_get(map, i, j)]);
				DrawRectangleLines((i * boxsize.x) + boxsize.x, j * boxsize.y, boxsize.x, boxsize.y, BLACK);
			}
		}
	}

	z__Vint2 blkp = {(blk->pos.x * boxsize.x) + boxsize.x, (blk->pos.y * boxsize.y)};

	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++) {
			if(block_get(blk, i, j)) {
				DrawRectangle(blkp.x + (boxsize.x * i), blkp.y + (boxsize.y * j), boxsize.x, boxsize.y, color[blk->color]);
				DrawRectangleLines(blkp.x + (boxsize.x * i), blkp.y + (boxsize.y * j), boxsize.x, boxsize.y, BLACK);
			}
		}
	}


	z__Vint2 textpos = {
		(boxsize.x * map->size.x) + boxsize.x*5,
		15
	};

	char msg[128];
	snprintf(msg,  128, "TIME : %lld", gstate->time);
	DrawText(msg, textpos.x, textpos.y, boxsize.x * 1.5, WHITE);

	textpos.y += boxsize.x * 2;

	snprintf(msg,  128, "SCORE : %d", gstate->score);
	DrawText(msg, textpos.x, textpos.y, boxsize.x * 1.5, WHITE);

	textpos.y += boxsize.x * 2;

	snprintf(msg,  128, "LEVEL : %d", gstate->level);
	DrawText(msg, textpos.x, textpos.y, boxsize.x * 1.5, WHITE);

	textpos.y += boxsize.x * 2;

	snprintf(msg,  128, "SPEED : %.2f", gstate->speed);
	DrawText(msg, textpos.x, textpos.y, boxsize.x * 1.5, WHITE);

	textpos.y += boxsize.x * 2;

	snprintf(msg,  128, "DROPS : %d", gstate->blockdrops);
	DrawText(msg, textpos.x, textpos.y, boxsize.x * 1.5, WHITE);


	blkp.x = textpos.x + boxsize.x;
	blkp.y = textpos.y + boxsize.y *3;

	DrawRectangle(textpos.x, textpos.y + boxsize.y*2, boxsize.x * 6, boxsize.y * 6, WHITE);
	DrawRectangle(blkp.x, blkp.y, boxsize.x * 4, boxsize.y * 4, BLACK);
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++) {
			if(block_get(blk_next, i, j)) {
				DrawRectangle(blkp.x + (boxsize.x * i), blkp.y + (boxsize.y * j), boxsize.x, boxsize.y, color[blk_next->color]);
				DrawRectangleLines(blkp.x + (boxsize.x * i), blkp.y + (boxsize.y * j), boxsize.x, boxsize.y, BLACK);
			}
		}
	}



	//DrawRectangle(blk->pos.x * boxsize.x, blk->pos.y * boxsize.y, boxsize.x, boxsize.y, WHITE);
}

Block get_new_block(Map *map)
{

	Block n = Tetris_Blocks[random() % BLOCKS_TOTAL];
	n.pos.x = random() % map->size.x-1;
	n.color = random() % COLORS_TOTAL;

	return n;
}

void move_block_horizontal(Map *map, Block *block, z__i32 changeby)
{
	block->pos.x += changeby;

	z__Vector(int, from, to, from_found) n = {0, 0, 0};
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++) {
			if(block_get(block, i, j)) {
				if(!n.from_found) {
					n.from = i;
					n.to = i;
					n.from_found = 1;
				}
				n.to = i;
			}
		}
	}

	
	if(!(n.from + block->pos.x >= 0 && n.to + block->pos.x < map->size.x)) {
		block->pos.x -= changeby;
		return;
	}


	for(int i = n.from; i < n.to; i++) {
		for(int j = 0; j < 4; j++) {
			int newpos_x = block->pos.x + i;
			if(map_get(map, newpos_x, block->pos.y + j) && block_get(block, i, j)) {
				block->pos.x -= changeby;
				return;
			}
		}
	}
}

int move_next_verticle(Map *map, Block *block)
{
	block->pos.y += 1;

	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++) {
			if(map_get(map, block->pos.x + i, block->pos.y + j) && block_get(block, i, j)) {
				goto _L_fix_the_block;
			}
		}
	}

	return 0;

	_L_fix_the_block: {
		block->pos.y--;
		for(int i = 0; i < 4; i++) {
			for(int j = 3; j >= 0; j--) {
				if(block_get(block, i, j) && !map_get(map, block->pos.x + i, block->pos.y + j))
				map_get(map, block->pos.x + i, block->pos.y + j) = 1;
				map_color_get(map, block->pos.x + i, block->pos.y + j) = block->color;
			}
		}
	}

	return 1;
}

int check_for_clear(Map *map, z__u32 at)
{
	for(z__u32 i = 0; i < map->size.x; i++) {
		if(!map_get(map, i, at)) return 0;
	}
	return 1;
}

void clear_line(Map *map, z__u32 at)
{
	memset(&map_get(map, 0, at), 0, sizeof(*map->data) * map->size.x);
	for(int j = at; j > 0; j--)
		for(int i = 0; i < map->size.x; i++)
			map_get(map, i, j) = map_get(map, i, j-1);
}

void tetrix(void)
{

	Music music = LoadMusicStream("bg_music.mp3");
	PlayMusicStream(music);

	Map map = Map_new(16, 36);
	for(z__u32 i = 0; i < map.size.x; i++) {
		map_get(&map, i, map.size.y-1) = 1;
	}

	Block 
		  block = get_new_block(&map)
		, block_next = get_new_block(&map);

	GameState gstate = {
		.score = 0,
		.level = 1,
		.speed = 1,
		.time = 0
	};

	int is_new_block_required = 1;

	float posnew = 0;


	while(!WindowShouldClose()) {

		UpdateMusicStream(music);

		float dt = GetFrameTime();

		if(is_new_block_required) {
		
			gstate.score += 10 * gstate.level;
			for(int i = block.pos.y; i < block.pos.y + 4; i++) {
				if(check_for_clear(&map, i)) {
					clear_line(&map, i);
					gstate.score += (10 * map.size.x) * gstate.level;
					gstate.speed += .01f;
				}
			}
			
			gstate.blockdrops += 1;

			if(!(gstate.blockdrops % 8)) {
				gstate.level += 1;
				gstate.speed += gstate.level * (gstate.speed / 90.0f);
				gstate.blockdrops = 1;
			}

			block = block_next;
			block_next = get_new_block(&map);
			is_new_block_required ^= 1;
		}

		posnew += gstate.speed * dt;
		if(posnew > 1) {
			is_new_block_required = move_next_verticle(&map, &block);
			gstate.time += 1;
			posnew = 0;
		}

		z__auto key = GetKeyPressed();

		switch(key) {
			case KEY_D: 
				move_block_horizontal(&map, &block, 1);
				break;

			case KEY_A: 
				move_block_horizontal(&map, &block, -1);
				break;

			case KEY_S: 
				is_new_block_required = move_next_verticle(&map, &block);
				break;

			case KEY_E:
				block.oriantaion += 1;
				if(block.oriantaion > 3) block.oriantaion = 0;
				break;

			case KEY_Q:
				block.oriantaion -= 1;
				if(block.oriantaion < 0) block.oriantaion = 3;
				break;
			
		}

		if(IsKeyPressed(KEY_F)){
			while(!IsKeyPressed(KEY_ENTER)) {
				BeginDrawing();
					ClearBackground(BLACK);
					DrawText("PAUSED", (WINDOW_X/2) - 38, (WINDOW_Y/2) - 7, 24, WHITE);
					DrawText("PRESS ENTER TO CONTINUE", (WINDOW_X/2) - (24 * 7), (WINDOW_Y/2) + 24, 24, WHITE);
				EndDrawing();
			}
		}


		BeginDrawing();
			ClearBackground(BLACK);
			draw_screem(&map, &block, &block_next, &gstate);
		EndDrawing();

	}

	UnloadMusicStream(music);

}

int main(int argc, char const *argv[])
{
	_init();
	InitAudioDevice();
	
	
	tetrix();


	CloseAudioDevice();
	CloseWindow();

	return 0;
}

