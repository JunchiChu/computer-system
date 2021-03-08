#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "generator.h"

/*
 * Given a direction, get its opposite
 *
 * Parameters:
 *  - dir: direction
 *
 * Returns:
 *  - the opposite direction to dir
 */
Direction get_opposite_dir(Direction dir) {
    // TODO: implement this function
  if (dir == NORTH){
    return SOUTH;
  }else if (dir == SOUTH){
    return NORTH;
  }else if (dir == WEST){
    return EAST;
  }else{
    return WEST;
  }
}

/*
 * Given an array of the four directions, randomizes the order of the directions
 *
 * Parameters:
 *  - directions: an array of Direction enums of size 4
 *
 * Returns:
 *  - nothing - the array should be shuffled in place
 */
void shuffle_array(Direction directions[]) {
    // TODO: implement this function
  //srand(time(NULL));
   int size=4;
   //size=sizeof(directions)/sizeof(directions[0]);
   for (int i = 0; i < size ; i++) {
        int j = rand()%(size);
	//printf("j value: %d\n",j);
	//Direction *dj,*di;
	Direction tem = directions[j];
	directions[j] = directions[i];
        directions[i] = tem;
        //*dj = *di;
        //*di = tem;
   }

  
}

/*
 * Recursive function for stepping through a maze and setting its
 *connections/walls using the drunken_walk algorithm
 *
 * Parameters:
 *  - row: row of the current room
 *  - col: column of the current room
 *  - num_rows: number of rows in the maze
 *  - num_cols: number of columns in the maze
 *  - maze: a 2D array of maze_room structs representing your maze
 * Returns:
 *  - nothing - the state of your maze should be saved in the maze array
 *being passed in (make sure to use pointers correctly!).
 */
void drunken_walk(int row, int col, int num_rows, int num_cols,
                  struct maze_room maze[num_rows][num_cols]) {
    // TODO: implement this function
  int i, out_of_bound, row_offset, col_offset;
  Direction dir[4] = {SOUTH,EAST,WEST,NORTH};
  struct maze_room *r;
  struct maze_room *neighbor;
  //int x,y;
  //for (x=0;x<2;x++){
  //for (y=0;y<2;y++){
  //  printf("maze row-%d col-%d 4 ele %d %d %d %d, vis%d\n",maze[x][y].row,maz
  
  r = &maze[row][col];
  r->visited = 1;
 
  
  shuffle_array(dir);
 
  for (i = 0;i < 4;i++){
      row_offset = col_offset = 0;
      switch(dir[i]) {
         case NORTH :
           row_offset = -1;
           break;
         case SOUTH :
           row_offset = 1;
           break;
         case EAST :
           col_offset = 1;
           break;
         case WEST :
           col_offset = -1;
           break;
         default :
           printf("Invalid offset\n" );
      }
      
      out_of_bound = is_in_range (row+row_offset,col+col_offset, num_rows, num_cols);
      if(out_of_bound == 0) {
	r->walls[3-dir[i]] = 1;
      }else{
	neighbor = get_neighbor(num_rows, num_cols,maze,r,dir[i]);
        if (neighbor->visited == 0){
	    r->walls[3-dir[i]]=0;
	    drunken_walk(neighbor->row, neighbor->col,num_rows,num_cols,maze);
	}else{
	  Direction oppo_dir;
	  oppo_dir= get_opposite_dir(dir[i]);
	  if((neighbor->walls[3-oppo_dir])==0 || (neighbor->walls[3-oppo_dir])==1){
	    r->walls[3-dir[i]] = neighbor->walls[3-oppo_dir];
	  }else{
	    r->walls[3-dir[i]] = 1;
	  }
	}
      }
  }
	
	  
	    
	  
}
  
/*
 * Represents a maze_room struct as an integer based on its connections
 *
 * Parameters:
 *  - room: a struct maze_room to be converted to an integer
 *
 * Returns:
 *  - the integer representation of a room
 */
int encode_room(struct maze_room room) {
  int int_rep=0;
  int_rep= (room.walls[0])*8+(room.walls[1])*4+(room.walls[2])*2+(room.walls[3])*1;
  return int_rep;
}

/*
 * Represents a maze_room array as a hexadecimal array based on its connections
 *
 * Parameters:
 *  - num_rows: number of rows in the maze
 *  - num_cols: number of columns in the maze
 *  - maze: a 2D array of maze_room structs representing the maze
 *  - result: a 2D array of hexadecimal numbers representing the maze
 *
 * Returns:
 *  - nothing - each maze_room in the maze should be represented
 *    as a hexadecimal number  and put in nums at the corresponding index
 */
void encode_maze(int num_rows, int num_cols,
                 struct maze_room maze[num_rows][num_cols],
                 int result[num_rows][num_cols]) {
    // TODO: implement this function
  int row, col;
  for (row=0;row<num_rows;row++){
    for (col=0;col<num_cols;col++){
    result[row][col]=encode_room(maze[row][col]);
    }
  }
}

/*
 * Writes encoded maze to an output file
 *
 * Parameters:
 *  - num_rows: number of rows in the maze
 *  - num_cols: number of columns in the maze
 *  - encoded_maze: a maze_room array as a hexadecimal array based on its
 *    connections
 *  - file_name: the name of the output file for the encoded maze
 *
 * Returns:
 *  - 1 if an error occurs, 0 otherwise
 */
int write_encoded_maze_to_file(int num_rows, int num_cols,
                               int encoded_maze[num_rows][num_cols],
                               char *file_name) {
    int err = 0;

    // open file (create it if necessary)
    FILE *f = fopen(file_name, "w+");
    if (f == NULL) {
        fprintf(stderr, "Error opening file.\n");
        return 1;
    }
    // write each room's hex value into file
    for (int i = 0; i < num_rows; i++) {
        for (int j = 0; j < num_cols; j++) {
            int encoded_room = encoded_maze[i][j];
            err = fprintf(f, "%x", encoded_room);
            if (err < 0) {
                fprintf(stderr, "Writing to file failed: %i\n", err);
                return 1;
            }
        }
        // add a newline between rows
        err = fprintf(f, "\n");
        if (err < 0) {
            fprintf(stderr, "Writing to file failed: %i\n", err);
            return 1;
        }
    }
    // close file
    int close = fclose(f);
    if (close == EOF) {
        fprintf(stderr, "Could not close file.\n");
        return 1;
    }
    return 0;
}

/*
 * Main function
 *
 * Parameters:
 *  - argc: the number of command line arguments - for this function 4
 *  - **argv: a pointer to the first element in the command line
 *            arguments array - for this function:
 *            ["generator", <output file>, <number of rows>, <number of
 *columns>]
 *
 * Returns:
 *  - 0 if program exits correctly, 1 if there is an error
 */

int main(int argc, char **argv) {
    char *file_name;
    int num_rows;
    int num_cols;
    srand(time(NULL));
    if (argc != 4) {
        printf("Incorrect number of arguments.\n");
        printf(
            "./generator <output file> <number of rows> <number of columns>\n");
        return 1;
    } else {
        file_name = argv[1];
        num_rows = atoi(argv[2]);
        num_cols = atoi(argv[3]);
    }
    // TODO: implement this function
    srand(time(NULL));
    if (num_rows <=0 || num_cols <=0){
      return 1;
    }
    int status=5;
    int res_encod_maz[num_rows][num_cols];
    struct maze_room maze[num_rows][num_cols];
    initialize_maze(num_rows,num_cols,maze);
    drunken_walk(0,0,num_rows,num_cols,maze);
    encode_maze(num_rows,num_cols,maze,res_encod_maz);
    status = write_encoded_maze_to_file(num_rows,num_cols,res_encod_maz,file_name);
    if (status==1){
      return 1;
    }else {
      return 0;
    }
    
}
