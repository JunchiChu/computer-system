#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "common.h"

/*
 * Determines whether or not the room at [row][col] is a valid room within the
 *maze with dimensions num_rows x num_cols
 *
 * Parameters:
 *  - row: row of the current room
 *  - col: column of the current room
 *  - num_rows: number of rows in the maze
 *  - num_cols: number of columns in the maze
 *
 * Returns:
 *  - 0 if room is not in the maze, 1 if room is in the maze
 */
int is_in_range(int row, int col, int num_rows, int num_cols) {
    // TODO: implement function
  if(row >= num_rows || row < 0 || col < 0 || col>=num_cols) {
    return 0;
  }else{
    return 1;
  }
}

/*
 * Given a pointer to the room and a Direction to travel in, return a pointer to
 *the room that neighbors the current room on the given Direction. For example:
 *get_neighbor(&maze[3][4], EAST) should return &maze[3][5]
 *
 * Parameters:
 *  - num_rows: number of rows in the maze
 *  - num_cols: number of columns in the maze
 *  - room: pointer to the current room
 *  - dir: Direction to get the neighbor from
 *  - maze: a 2D array of maze_room structs representing your maze
 * Returns:
 *  - pointer to the neighboring room
 */
struct maze_room *get_neighbor(int num_rows, int num_cols,
                               struct maze_room maze[num_rows][num_cols],
                               struct maze_room *room, Direction dir) {
    // TODO: implement function
  int room_r,room_c;
  room_r=room->row;
  room_c=room->col;
  struct maze_room *maz_pt;
  if (dir == 0){
    maz_pt = &maze[room_r-1][room_c];
  }else if(dir == 1){
    maz_pt = &maze[room_r+1][room_c];
  }else if(dir == 2){
    maz_pt = &maze[room_r][room_c-1];
  }else{
    maz_pt = &maze[room_r][room_c+1];
  }
  return maz_pt;
}





/*
 * Initializes a 2D array of maze rooms with all of the necessary values
 *
 * Parameters:
 *  - num_rows: the number of the rows in the maze
 *  - num_cols: the number of columns in the maze
 *  - maze: a 2D array of uninitialized maze_rooms (to be initialized in
 *     this function)
 *
 * Returns:
 *  - nothing (the initialized maze will be stored in the 'maze' array)
 */
void initialize_maze(int num_rows, int num_cols,
           struct maze_room maze[num_rows][num_cols]) {
    // TODO: implement function
  int row,col;
  for(row=0;row<num_rows;row++){
    for(col=0;col<num_cols;col++){
      //maze_room maze[row][col] = {.row = row,.col = col,.walls[4] = {7,8,9,5},.visited = 0};
      maze[row][col].row = row;
      maze[row][col].col = col;
      maze[row][col].walls[0] = 5;
      maze[row][col].walls[1]=6;
      maze[row][col].walls[2]=7;
      maze[row][col].walls[3]=8;
      maze[row][col].visited = 0;
      maze[row][col].next = NULL;
    }
  }
}

