#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "solver.h"

/*
 * Given a pointer to a maze_room, set its connections in all four directions
 *based on the hex value passed in.
 *
 * For example:
 *
 *      create_room_connections(&maze[3][4], 0xb)
 *
 * 0xb is 1011 in binary, which means we have walls on all sides except the
 *EAST. This means that every connection in maze[3][4] should be set to 1,
 *except the EAST connection, which should be set to 0.
 *
 * See the handout for more details about our hexadecimal representation, as
 *well as examples on how to use bit masks to extract information from this
 *hexadecimal representation.
 *
 * Parameters:
 *  - room: pointer to the current room
 *  - hex: hexadecimal integer (between 0-15 inclusive) that represents the
 *connections in all four directions from the given room.
 *
 * Returns:
 *  - nothing. The connections should be saved in the maze_room struct
 *pointed to by the parameter (make sure to use pointers correctly!).
 */
void create_room_connections(struct maze_room *room, unsigned int hex) {
    // TODO: implement this function
  
  if (hex & 4) {
    room -> walls[1] = 1;
  }else{
    room -> walls[1] = 0;
  }

  if (hex & 2) {
    room -> walls[2] = 1;
  }else{
    room -> walls[2] = 0;
  }

  if (hex & 1) {
    room -> walls[3] = 1;
  }else{
    room -> walls[3] = 0;
  }

  if (hex & 8) {
    room -> walls[0] = 1;
  }else{
    room -> walls[0] = 0;
  }


}

/*
 * Recursive depth-first search algorithm for solving your maze.
 * This function should also print out either every visited room as it goes, or
 * the final pruned solution, depending on whether the FULL macro is set.
 *
 * Note that you may need to add extra parameters to this function's signature,
 * depending on how you choose to store the pruned solution.
 *
 * See handout for more details, as well as a pseudocode implementation.
 *
 * Parameters:
 *  - row: row of the current room
 *  - col: column of the current room
 *  - goal_row: row of the goal room
 *  - goal_col: col of the goal room
 *  - num_rows: number of rows in the maze
 *  - num_cols: number of columns in the maze
 *  - maze: a 2d array of maze_room structs representing your maze
 *  - file: the file to write the solution to
 *
 * Returns:
 *  - 1 if the current branch finds a valid solution, 0 if no branches are
 *valid.
 */
int dfs(int row, int col, int goal_row, int goal_col, int num_rows,
        int num_cols, struct maze_room maze[num_rows][num_cols], FILE *file) {
    Direction dir[4] = { NORTH, SOUTH, EAST, WEST };
    int i, row_offset, col_offset, out_of_bound;
    struct maze_room *r;
    struct maze_room *neighbor;
    
    r = &maze[row][col];
    
    if (row == goal_row && col == goal_col){
      
      fprintf(file, "%d, %d\n",row,col);
      return 1;
    }
    r->visited = 1;
    for (i=0; i<4 ;i++){
    
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

       out_of_bound = is_in_range (row+row_offset,col+col_offs\
et, num_rows, num_cols);

       if (out_of_bound != 0) {
	
          neighbor = get_neighbor(num_rows, num_cols,maze,r,dir[i]);
          r->next=neighbor;
	
          if (r->walls[3-dir[i]] ==0 && neighbor->visited == 0){
	  
	     fprintf(file,"%d, %d\n",r->row,r->col);
	     if (dfs(neighbor->row,neighbor->col,goal_row,goal_col,num_rows,num_cols,maze,file)==1){
	      
	         return 1;
	     }
	     
	  }
       }
    }
    
    fprintf(file,"%d, %d\n",row,col);
    return 0;
}

/*
 * Decodes an encoded maze and stores the resulting maze room data in the
 * passed 'maze' variable.
 *
 * Parameters:
 *  - num_rows: number of rows in the maze
 *  - num_cols: number of columns in the maze
 *  - maze: a 2d array of maze_room structs (where to store the decoded
 *maze)
 *  - encoded_maze: a 2d array of numbers representing a maze
 *
 * Returns:
 *  - nothing; the decoded maze room is stored in the `maze` variable
 */
void decode_maze(int num_rows, int num_cols,
                 struct maze_room maze[num_rows][num_cols],
                 int encoded_maze[num_rows][num_cols]) {
  
  int r,c;
  for (r=0;r<num_rows;r++){
    for (c=0;c<num_cols;c++){
       create_room_connections( &maze[r][c], encoded_maze[r][c]);
    }
  }
}

/*
 * Recursively prints the pruned solution path (using the current maze room
 * and its next pointer)
 *
 * Parameters:
 *  - room: a pointer to the current maze room
 *  - file: the file where to print the path
 *
 * Returns:
 *  - 1 if an error occurs, 0 otherwise
 */
int print_pruned_path(struct maze_room *room, FILE *file) {
   
     struct maze_room *head;
     head = room;
    
     while(head != NULL){
      fprintf(file, "%d, %d\n",head->row,head->col);
      head = head -> next;
    }
     

    return 0;
}

/*
 * Reads encoded maze from the file passed in
 *
 * Parameters:
 *  - num_rows: number of rows in the maze
 *  - num_cols: number of columns in the maze
 *  - encoded_maze: a maze_room array as a hexadecimal array based on its
 *    connections
 *  - file_name: input file to read the encoded maze from
 *
 * Returns:
 *  - 1 if an error occurs, 0 otherwise
 */
int read_encoded_maze_from_file(int num_rows, int num_cols,
                                int encoded_maze[num_rows][num_cols],
                                char *file_name) {
    int err = 0;

    // open file
    FILE *f = fopen(file_name, "r");
    if (f == NULL) {
        fprintf(stderr, "Error opening file.\n");
        return 1;
    }
    // read each hex value into 2D array
    for (int i = 0; i < num_rows; i++) {
        for (int j = 0; j < num_cols; j++) {
            unsigned int encoded_room;
            err = fscanf(f, "%1x", &encoded_room);
            encoded_maze[i][j] = encoded_room;
            if (err < 0) {
                fprintf(stderr, "Reading from file failed");
                return 1;
            }
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
 *  - argc: the number of command line arguments - for this function 9
 *  - **argv: a pointer to the first element in the command line
 *            arguments array - for this function:
 *            ["solver", <input maze file>, <number of rows>, <number of
 *columns> <output path file>, <starting row>, <starting column>, <ending row>,
 *<ending column>]
 *
 * Returns:
 *  - 0 if program exits correctly, 1 if there is an error
 */

int main(int argc, char **argv) {
    int num_rows, num_cols, start_row, start_col, goal_row, goal_col;
    char *maze_file_name;
    char *path_file_name;
   
    int close;
    if (argc != 9) {
        printf("Incorrect number of arguments.\n");
        printf(
            "./solver <input maze file> <number of rows> <number of columns>");
        printf(" <output path file> <starting row> <starting column>");
        printf(" <ending row> <ending column>\n");
        return 1;
    } else {
        maze_file_name = argv[1];
        num_rows = atoi(argv[2]);
        num_cols = atoi(argv[3]);
        path_file_name = argv[4];
        start_row = atoi(argv[5]);
        start_col = atoi(argv[6]);
        goal_row = atoi(argv[7]);
        goal_col = atoi(argv[8]);
    }
    if (goal_row < 0 || goal_col < 0 || start_row < 0 || start_col <0 ){
      return 1;
    }
    if (goal_row >= num_rows || goal_col >= num_cols ){
      return 1;
    }

    if (start_row >= num_rows || start_col >= num_cols){
      return 1;
    }
    
    int i, j, read_encodemaz, dfs_solu;
    int encod_maz[num_rows][num_cols];
    for (i=0 ;i<num_rows;i++){
      for( j=0; j<num_cols;j++){
	encod_maz[i][j]=0;
      }
    }
    read_encodemaz =  read_encoded_maze_from_file (num_rows, num_cols, encod_maz,maze_file_name);
    if (read_encodemaz == 1){
      printf("error in reading encode maze\n");
      return 1;
    }
  
    struct maze_room maze[num_rows][num_cols];
    initialize_maze(num_rows,num_cols,maze);
    decode_maze(num_rows, num_cols, maze, encod_maz);
    FILE *filept = fopen(path_file_name,"w");
    
    if (filept == NULL){
      fprintf(stderr,"error. Cannot open file");
      return 1;
    }else{
      fprintf(filept, "FULL\n");  
      dfs_solu = dfs(start_row, start_col, goal_row, goal_col,num_rows,num_cols,maze,filept);
    }
    
    close=fclose(filept);
    if (close == EOF) {
        fprintf(stderr, "Could not close file.\n");
        return 1;
    }

    #ifdef FULL
    printf("We are doing FULL MODE. \n");
    
    #else
    printf("We are doing PRUNED MODE. \n");
    FILE *fileptt = fopen(path_file_name,"w");
    
    if (fileptt == NULL){
     fprintf(stderr,"error. Cannot open file");
      return 1;
     }else{
       struct maze_room *head;
       head = &maze[start_row][start_col];
       fprintf(fileptt, "PRUNED\n");
       print_pruned_path(head, fileptt);
    }
    

    
    
    close=fclose(fileptt);
    if (close == EOF) {
       fprintf(stderr, "Could not close file.\n");
       return 1;
     }

    return 0;
    #endif

    

    
    if (dfs_solu == 1) {
      return 0;
    }
    //else{
      // return 1;
      //}
    
    return 0;
}
