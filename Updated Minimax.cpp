//
//  minimaxV2.cpp
//
//
//  Created by Andrew Wang, Nicholas Makharinets and Connor Byers on 2019-11-20.
//

#include <stdio.h>
#include <iostream>
#include <math.h>
#include <vector>
#include <random>
#include <time.h>
#include <stdlib.h>
using namespace std;

class Node {
public:
   //ATTRIBUTES//

   int boardState[8][8];
   int place[2];
   int depth;
   int maxDepth;
   float value;
   bool isMax;
   int childIndex;
   vector <Node> children;

   //CONSTRUCTOR//

   Node(int a, int b, int state[8][8]) {
       maxDepth = a;
       depth = b;
       childIndex = 0;
       place[0] = 0;
       place[1] = 0;

       if (depth % 2 == 0) {//if the tree depth is even, it's the player's turn
           isMax = 0;
       } else { //otherwise it's the computer's turn
           isMax = 1;
       }

       for (int i = 0; i < 8; i++) { //Idk if we should use memcpy
           for (int j = 0; j < 8; j++) {
               boardState[i][j] = state[i][j];
           }
       }

   }

   //METHODS//

   float evaluate() { //Dummy eval function
       float val =rand() % 100; // note the smaller the range of numbers, the faster
       return val;
   }

   void getBoard(int** board) { //dummy getBoard function
       //NICK'S CODE

       //END OF NICK'S CODE
       /*int board[8][8]={
       {0,2,0,2,0,2,0,2},//red side of the board initially
       {2,0,2,0,2,0,2,0},
       {0,2,0,2,0,2,0,2},
       {0,0,0,0,0,0,0,0},//middle of the board is initially empty
       {0,0,0,0,0,0,0,0},
       {1,0,1,0,1,0,1,0},//white side of the board initially
       {0,1,0,1,0,1,0,1},
       {1,0,1,0,1,0,1,0}};*/
       int newBoard[8][8];

       for (int i = 0; i < 8; i++) {
           for (int j = 0; j < 8; j++) {
               newBoard[i][j] = board[i][j];
           }
       }

       Node *child;
       child = new Node (maxDepth,depth+1,newBoard);
       children.push_back(*child);
       delete child;
   }

   //NICK'S CODE//
   int validMove(int *action, int currentstate[8][8], int turn ){//a version of checkmove used in the minimax that doesn't move any pieces
       int cr, cc, mr, mc;//current row, column and move row, column
       int columnDirection=0, rowDirection=0;

       cc=action[0];//current row&column is the first entry from player
       cr=action[1];

       mc=action[2];//move row&column is the second entry from player
       mr=action[3];

       if(mr>7 || mr<0){
           return -1;
       }

       if(mc>7 || mc<0){
           return -1;
       }

       //removed the check of choosing an empty square

       if(currentstate[mr][mc]!=0){
           return -1;//this -1 return value indicates that the selected move spot is already occupied and thus the move is not valid
       }
       if(currentstate[cr][cc]==1){//if the spot selected to move has a white piece
           if(turn==1){//only player one can move odd numbered checkers, thus the move number must be even since p1 goes first
               if(mr>=cr){//white checkers can only move upwards
                   return -1;//they need to enter another move
               }
               if(mr==cr-1){//if the player demands to move their white checker to the row directly above
                   if(mc==cc+1 || mc==cc-1){
                       //removed the auto king
                       return 1;//this move is thus valid because the right player is moving the right piece to an empty index in the right direction
                   }
               }
               if(mr==cr-2){//if the player demands to move their checker two rows up (taking something)
                   columnDirection=mc-cc;//the column direction of the move is determined via the difference in mc and cc
                   if(columnDirection>0){
                       columnDirection=1;
                   }
                   else {
                       columnDirection=-1;
                   }
                   if(currentstate[cr-1][cc+columnDirection]==2 || currentstate[cr-1][cc+columnDirection]==4){//if the diagonally adjacent spot contains an opposite piece or king
                       //took out the automatic piece removal
                       return 1;//the move is valid, and the piece has been removed thus return 1 and move the piece in main
                   }
               }
           }
           else{
               return -1; //-1 return value indicated an illegal inital selection, in this case the player didnt choose their own piece
           }
       }
       if(currentstate[cr][cc]==2){//if the selected spot contains a red checker
           if(turn==0){//only player two can move even numbered checkers, thus the move count must be odd since they go second
               if(mr<=cr){//red checkers can only move downwards
                   return -1;
               }
               if(mr==cr+1){//if the player demands to move their red piece down to the next row
                   if(mc==cc+1 || mc==cc-1){
                       //removed the auto king
                       return 1;//this 3x3 move is thus valid for same reasons as mentioned above
                   }
               }
               if(mr==cr+2){//if the player demands to move their checker two rows down (taking something)
                   columnDirection=mc-cc;//the column direction of the move is determined via the difference in mc and cc

                   if(columnDirection>0){
                       columnDirection=1;
                   }
                   else{
                       columnDirection=-1;
                   }

                   if(currentstate[cr+1][cc+columnDirection]==1 || currentstate[cr+1][cc+columnDirection]==3){//if the diagonally adjacent spot contains an opposite piece or king
                       //removed the auto piece removal
                       return 1;//the move is valid, and the piece has been removed thus return 1 and move the piece in main
                   }
               }
           }
           else {
               return -1;//-1 return value indicated an illegal inital selection, in this case the player didnt choose their own piece
           }
       }
       else if(currentstate[cr][cc]==3){//if a white king is selected for movement
           if(turn==1){//the turn must match white player
               if(abs(mc-cc)==1 && abs(mr-cr)==1){//if the king is trying to move in 3x3 allow
                   return 1;
               }
               else if(abs(mc-cc)==2 && abs(mr-cr)==2){//if a 5x5 jump move is requested
                   columnDirection=mc-cc;//determine the move direction as positive or negative
                       if(columnDirection>0){
                           columnDirection=1;
                       }
                       else{
                           columnDirection=-1;
                       }

                   rowDirection=mr-cr;
                       if(rowDirection>0){
                           rowDirection=1;
                       }
                       else{
                           rowDirection=-1;
                       }
                   if(currentstate[cr+rowDirection][cc+columnDirection]==2 || currentstate[cr+rowDirection][cc+columnDirection]==4){
                   //if the space being hopped over belongs to the opponent

                       //removed the auto piece removal

                       return 1;
                   }
               }
           }
       }
       else if(currentstate[cr][cc]==4){//if a red king is selected for movement
           if(turn==0){//the turn must match red player
               if(abs(mc-cc)==1 && abs(mr-cr)==1){//if the king is trying to move in 3x3 allow
                   return 1;
               }
               else if(abs(mc-cc)==2 && abs(mr-cr)==2){//if a 5x5 jump move is requested
                   columnDirection=mc-cc;//determine the move direction as positive or negative
                       if(columnDirection>0){
                           columnDirection=1;
                       }
                       else{
                           columnDirection=-1;
                       }

                   rowDirection=mr-cr;
                       if(rowDirection>0){
                           rowDirection=1;
                       }
                       else{
                           rowDirection=-1;
                       }

                   if(currentstate[cr+rowDirection][cc+columnDirection]==1 || currentstate[cr+rowDirection][cc+columnDirection]==3){
                       //if the space hopped over belongs to the opponent

                       //removed the auto piece removal

                       return 1;
                   }
               }
           }
       }
   return -1;
   }


   void removePiece(int **arr,int cc,int cr){
   arr[cr][cc]=0;
   }

   void movePiece(int **arr, int cc, int cr, int nc, int nr){ //gets board, current row, current column, new row, new column indices
   int piece=arr[cr][cc];
   arr[cr][cc]=0;
   arr[nr][nc]=piece;
       cout << "\nAfter\n";
       for (int i = 0; i < 8; i++) { //Idk if we should use memcpy
           for (int j = 0; j < 8; j++) {
               cout << arr[i][j] << " ";
           }
           cout << "\n";
       }

   }



   void newBoard( int *move, int **newState, int turn){

       int cc=move[0];//current row&column is the first entry from computer
       int cr=move[1];

       int mc=move[2];//move row&column is the second entry from computer
       int mr=move[3];

       cout << "Moving " << cr << " "<< cc << " to " << mr << " " << mc << "\n";



       int columnDirection =0, rowDirection=0;
       if(mr==cr+1){//if the approved move wants to move into a king postion

           if(mc==cc+1 || mc==cc-1){

               if(mr==7){
                 if(newState[cr][cc]==2){
                   newState[mr][mc]=4;//king that spot
                   removePiece(newState,cc,cr);//remove the pawn
                 }
               }else{
               movePiece(newState, cc, cr, mc, mr);//moves the piece, this move is already valid,otherwise the funciton would not have run
               }
           }
       } else if(mr==cr-1){//if the approved move wants to move into a king postion

           if(mc==cc+1 || mc==cc-1){
               if(mr==0){
                 if(newState[cr][cc]==1){
                   newState[mr][mc]=3;//king that spot
                   removePiece(newState,cc,cr);//remove the pawn
                 }
               }else{
               movePiece(newState, cc, cr, mc, mr);//moves the piece, this move is already valid,otherwise the funciton would not have run
               }

           }
       } else {//move the piece normally
           if(abs(mc-cc)==2 && abs(mr-cr)==2){//if a 5x5 jump move is requested
                   columnDirection=mc-cc;//determine the move direction as positive or negative
                       if(columnDirection>0){
                           columnDirection=1;
                       }
                       else{
                           columnDirection=-1;
                       }
                   rowDirection=mr-cr;
                       if(rowDirection>0){
                           rowDirection=1;
                       }
                       else{
                           rowDirection=-1;
                       }
                   if(newState[cr+rowDirection][cc+columnDirection]==1 || newState[cr+rowDirection][cc+columnDirection]==3){
                       //if the space hopped over belongs to the opponent
                       removePiece(newState, cc+columnDirection, cr+rowDirection);//removes that piece
                       //the attacking piece is actually moved below
                   }
                   else if(newState[cr+rowDirection][cc+columnDirection]==2 || newState[cr+rowDirection][cc+columnDirection]==4){
                       //if the space hopped over belongs to the opponent
                       removePiece(newState, cc+columnDirection, cr+rowDirection);//removes that piece
                       //the attacking piece is actually moved below
                   }

               }
           movePiece(newState, cc, cr, mc, mr);//moves the piece, this move is already valid,otherwise the funciton would not have run
       }


   }

   void findNext(int *position, int currentState[8][8], int turn){

     int i=0, j=0, finishing=1, cut=0;

       cout<<"Starting at "<< position[1]<< " " << position[0]<<"\n";
       cout<<"the turn number right now is "<<turn<<"\n";

     if(position[0]>=7 && position[1]>=7){//if the position is currently on the very last piece of the board, do nothing at all in the projection state
        cout<<"Reached the end of the board\n";
         return;
     }

     for(i=position[1];i<8 && cut!=1;i++){
       if(finishing==1){//you must continue searching the current row the first time through
           cout<<"Now finishing\n";

           if(position[1]==0 && position[0]==0){//if its starting from the very first spot dont increment j before starting
               cout<<"Starting at index 0 0\n";
               for(j=position[0];j<8 && cut!=1; j++){
                   if(currentState[i][j]!=0){//finds a non empty square
                       if(currentState[i][j]==1 || currentState[i][j]==3){//it belongs to the player

                           if(turn==1){

                               position[1]=i;
                               position[0]=j;
                               cut=1;
                               break;
                           }
                       }
                       else if(currentState[i][j]==2 || currentState[i][j]==4){

                           if(turn==0){
                               position[1]=i;
                               position[0]=j;
                               cut=1;
                               break;
                           }
                       }
                   }
               }
           } else {//if the spot isnt 0,0 increment j to prevent calling on the same piece as before
               for(j=position[0]+1;j<8 && cut!=1; j++){
                   if(currentState[i][j]!=0){//finds a non empty square
                       if(currentState[i][j]==1 || currentState[i][j]==3){//it belongs to the player
                           if(turn==1){
                               position[1]=i;
                               position[0]=j;
                               cut=1;
                               break;
                           }
                       }
                       else if(currentState[i][j]==2 || currentState[i][j]==4){
                           if(turn==0){
                               position[1]=i;
                               position[0]=j;
                               cut=1;
                               break;
                           }
                       }
                   }
               }
           }
       } else {//it is now on a completely new row from the one that it started on so index j from 0
           for(j=0;j<8 && cut!=1;j++){
               if(currentState[i][j]!=0){//finds a non empty square
                   if(currentState[i][j]==1 || currentState[i][j]==3){//it belongs to the player
                       if(turn==1){
                           position[1]=i;
                           position[0]=j;
                           cut=1;
                           break;
                       }
                   }
                   else if(currentState[i][j]==2 || currentState[i][j]==4){
                       if(turn==0){
                           position[1]=i;
                           position[0]=j;
                           cut=1;
                           break;
                       }
                   }
               }
           }
       }
       if(i>=7 && j>=7){
         position[0]=8;
         position[1]=8;
       }
       finishing=0;//after one iteration of the column check loop, it must have finished the current column so never begin j from 0;
     }



     cout<<"Position is now "<< position[1]<< " " << position[0]+1<<"\n";
     //position[0]++;
   }

   void project(int *position, int currentState[8][8], int turn) {
       //position is the "place" current state of the parent board

       int i=0, j=0, births = 0;

       //now a new piece has been found and its moves can be projected

       int *action= new int[4];//sets the initial piece position to the first two spots for action pointer
       action[0]=position[0];
       action[1]=position[1];

       /*
       0 - move up right
       1- move up left
       2 - move down right
       3 - move down left
       4 - jump up right
       5 - jump up left
       6 - jump down right
       7 - jump down left
       //in order to implement double attacks add the 8th case where it can stay in one spot

       */

       while (births == 0 && position[1]<=7 && position[0]<=7) {
           findNext(position, currentState, turn);


           action[0]=position[0];
           action[1]=position[1];

           if(currentState[position[1]][position[0]]!=0){
               for(i=0;i<8;i++){//tests each of the 8 possible moves
                   switch(i){
                   case 0: action[2]=position[0]+1;
                           action[3]=position[1]-1;
                           break;
                   case 1: action[2]=position[0]-1;
                           action[3]=position[1]-1;
                           break;
                   case 2: action[2]=position[0]+1;
                           action[3]=position[1]+1;
                           break;
                   case 3: action[2]=position[0]-1;
                           action[3]=position[1]+1;
                           break;
                   case 4: action[2]=position[0]+2;
                           action[3]=position[1]-2;
                           break;
                   case 5: action[2]=position[0]-2;
                           action[3]=position[1]-2;
                           break;
                   case 6: action[2]=position[0]+2;
                           action[3]=position[1]+2;
                           break;
                   case 7: action[2]=position[0]-2;
                           action[3]=position[1]+2;
                           break;

                   }
                   //cout <<"Got here\n";
                   if(validMove(action, currentState, turn)==1){

                       births++;
                       //cout <<"Got here\n";
                       int **childBoard = new int*[8]; //allocates the new board array on the heap ,8x8 continuous memory block
                       for (int i = 0; i < 8; i++){
                           childBoard[i] = new int[8];
                       }
                       int a=0, b=0;
                       //use newBoard funciton
                       for(a=0;a<8;a++){
                           for(b=0;b<8;b++){
                               childBoard[a][b]=currentState[a][b];//copies the entire current state into this newboard
                           }
                       }


                       newBoard(action, childBoard, turn);


                       //CALL GET BOARD
                       getBoard(childBoard);

                       for (int i = 0; i < 8; i++) {
                           delete [] childBoard[i];
                       }
                       delete[] childBoard;//frees the pointer since its already been transfered
                   }
               }
           }
       }
   }


   //END OF NICK'S CODE//
};

void printBoard(Node node, int childNum) {

   for (int i = 0; i < 8; i++) {
       for (int j = 0; j < 8; j++) {
           cout << node.children[childNum].boardState[i][j] << " ";
       }
       cout << "\n";
   }
}

int abs(int x){
 return x>=0 ? x : -x;
}

int canattack(int arr[8][8], int cr, int cc){
 if(arr[cr][cc]==1){ //normal white piece
   if(cc==0||cc==1){//if the piece is one the left side of the board
       if((arr[cr-1][cc+1]==2||arr[cr-1][cc+1]==4)&&(arr[cr-2][cc+2]==0)){
           return 1;
       }
       else{
         return 0;
       }

   }
   else if(cc==7||cc==6){ //if its on the right
       if((arr[cr-1][cc-1]==2||arr[cr-1][cc-1]==4)&&(arr[cr-2][cc-2]==0)){
         return 1;
       }
       else{
         return 0;
       }
   }
   else{ //no index restriction on checking to the left or right
       if(((arr[cr-1][cc+1]==2||arr[cr-1][cc+1]==4)&&arr[cr-2][cc+2]==0)||((arr[cr-1][cc-1]==2||arr[cr-1][cc-1]==4)&&(arr[cr-2][cc-2]==0))){
         return 1;
       }
       else{
         return 0;
       }
   }
} //there could be an extra paranthesis somwhere
else if(arr[cr][cc]==2){ //normal red piece
   if(cc==0||cc==1){//if the piece is one the left side of the board
       if((arr[cr+1][cc+1]==1||arr[cr+1][cc+1]==3)&&(arr[cr+2][cc+2]==0)){
         return 1;
       }
       else{ //if there isnt an available jump
           return 0;
       }
   }
   else if(cc==7||cc==6){ //if its on the right YOU NEED TO ADD IT HERE AS THIS IS WHERE IT IS
       if((arr[cr+1][cc-1]==1||arr[cr+1][cc-1]==3)&&(arr[cr+2][cc-2]==0)){
         return 1;
       }
       else{ //if there isnt an available jump
           return 0;
       }
   }
   else{ //no index restriction on checking to the left or right
       if(((arr[cr+1][cc+1]==1||arr[cr+1][cc+1]==3)&&arr[cr+2][cc+2]==0)||((arr[cr+1][cc-1]==1||arr[cr+1][cc-1]==3)&&(arr[cr+2][cc-2]==0))){
         return 1;
       }
       else{ //if there isnt an available jump
           return 0;
       }
   }
}
else if(arr[cr][cc]==3){ //white king
   if(cc==0||cc==1){
       if(((arr[cr-1][cc+1]==2||arr[cr-1][cc+1]==4)&&arr[cr-2][cc+2]==0)||((arr[cr+1][cc+1]==2||arr[cr+1][cc+1]==4)&&arr[cr+2][cc+2]==0)){
       return 1;
   }
   else{
       return 0;
   }
   }
   else if(cc==6||cc==7){
       if(((arr[cr-1][cc-1]==2||arr[cr-1][cc-1]==4)&&(arr[cr-2][cc-2]==0))||((arr[cr+1][cc-1]==2||arr[cr+1][cc-1]==4)&&(arr[cr+2][cc-2]==0))){
       return 1;
   }
   else{
       return 0;
   }
   }
   else{
   if(((arr[cr-1][cc+1]==2||arr[cr-1][cc+1]==4)&&arr[cr-2][cc+2]==0)||((arr[cr-1][cc-1]==2||arr[cr-1][cc-1]==4)&&(arr[cr-2][cc-2]==0))||((arr[cr+1][cc+1]==2||arr[cr+1][cc+1]==4)&&arr[cr+2][cc+2]==0)||((arr[cr+1][cc-1]==2||arr[cr+1][cc-1]==4)&&(arr[cr+2][cc-2]==0))){
       return 1;
   }
   else{
       return 0;
   }
   }
}
else if(arr[cr][cc]==4){ //red king
   if(cc==0||cc==1){
       if(((arr[cr-1][cc+1]==1||arr[cr-1][cc+1]==3)&&arr[cr-2][cc+2]==0)||((arr[cr+1][cc+1]==1||arr[cr+1][cc+1]==3)&&arr[cr+2][cc+2]==0)){
       return 1;
   }
   else{
       return 0;
   }
   }
   else if(cc==6||cc==7){
       if(((arr[cr-1][cc-1]==1||arr[cr-1][cc-1]==3)&&(arr[cr-2][cc-2]==0))||((arr[cr+1][cc-1]==1||arr[cr+1][cc-1]==3)&&(arr[cr+2][cc-2]==0))){
       return 1;
   }
   else{
       return 0;
   }
   }
   else{
   if(((arr[cr-1][cc+1]==1||arr[cr-1][cc+1]==3)&&arr[cr-2][cc+2]==0)||((arr[cr-1][cc-1]==1||arr[cr-1][cc-1]==3)&&(arr[cr-2][cc-2]==0))||((arr[cr+1][cc+1]==1||arr[cr+1][cc+1]==3)&&arr[cr+2][cc+2]==0)||((arr[cr+1][cc-1]==1||arr[cr+1][cc-1]==3)&&(arr[cr+2][cc-2]==0))){
       return 1;
   }
   else{
       return 0;
   }
   }
}
return 0; //just in case
}

float averageDistance(int x, int y, int arr[], int n){
   float total = 0.0;
   int dx;
   int dy;
   for(int m = 0; m<n;m+=2){
     dx = abs(x-arr[m]);
     dy = abs(y-arr[m+1]);
     total+=sqrtf(dx*dx + dy*dy);
   }
   return total;
 }

float evaluate(int arr[8][8]){
 int boardNormal[8][8] = {
   {0,0,1,3,3,1,0,0},
   {0,0,0,0,0,0,0,0},
   {0,1,1,2,2,1,1,0},
   {0,0,3,4,4,3,0,0},
   {0,0,3,4,4,3,0,0},
   {0,0,1,2,2,1,0,0},
   {2,2,2,2,2,2,2,2},
   {0,0,0,0,0,0,0,0}};
 int boardKing[8][8] = {
   {0,0,0,0,0,0,0,0},
   {0,0,0,0,0,0,0,0},
   {0,1,1,2,2,1,1,0},
   {0,1,3,4,4,3,1,0},
   {0,1,3,4,4,3,1,0},
   {0,1,1,2,2,1,1,0},
   {0,0,0,0,0,0,0,0},
   {0,0,0,0,0,0,0,0}};
 float total = 0;
 float tempPiece = 0;
 int mycount=0;
 int theircount=0;
 int mykingcount=0;
 int theirkingcount=0;
 int mykingpos[24];
 int mykingposIndex =0;
 int theirpiecepos[24];
 int theirpieceposIndex = 0;
 int mypiecepos[24];
 int mypieceposIndex = 0;
 int theirkingpos[24];
 int theirkingposIndex = 0;
 int switch1 = 0;
 int switch2 = 0;
 for(int y = 1; y<8;y+=2){
     if((switch1==1)&&(switch2==1)){
       break;
     }
     if(arr[0][y]==0){
       switch1 = 1;
     }
     if(arr[7][y-1]==0){
       switch2 = 1;
     }
 }
 if(switch1==0){ //if back row is full
   total+=5;
 }
 if(switch2==0){ //if back row is full
   total-=5;
 }
 for(int x = 0; x< 8;x++){
     for(int y = 0; y<8; y++){
       if(arr[x][y]==0){
         continue;
       }
       tempPiece = 0;
       if(arr[x][y]==2){
         mypiecepos[mypieceposIndex] = x;
         mypiecepos[mypieceposIndex+1] = y;
         mypieceposIndex+=2;
           if(canattack(arr,x,y)==1){
             if(x==5){
               tempPiece+=30;
             }
             else{
               tempPiece+=8;
             }
           }
           if(y>0&&y<7){
             if(arr[x+1][y+1]==0){
               tempPiece+=0.5;
             }
             if(arr[x+1][y-1]==0){
               tempPiece+=0.5;
             }
           }
           if(y==0){
             if(arr[x+1][y+1]==0){
               tempPiece+=0.5;
             }
           }
           if(y==7){
             if(arr[x+1][y-1]==0){
               tempPiece+=0.5;
             }
           }

         tempPiece+=10;
         ++mycount;
         tempPiece+=boardNormal[x][y];
         total+=tempPiece;
       }
       if(arr[x][y]==4){
         mypiecepos[mypieceposIndex] = x;
         mypiecepos[mypieceposIndex+1] = y;
         mypieceposIndex+=2;
         mykingpos[mykingposIndex] = x;
         mykingpos[mykingposIndex+1] = y;
         mykingposIndex+=2;
         if(canattack(arr,x,y)){
               tempPiece+=6;
           }
           if(y>0&&y<7){
             if(arr[x+1][y+1]==0){
               tempPiece+=0.25;
             }
             if(arr[x+1][y-1]==0){
               tempPiece+=0.25;
             }
             if(arr[x-1][y+1]==0){
               tempPiece+=0.25;
             }
             if(arr[x-1][y-1]==0){
               tempPiece+=0.25;
             }
           }
           if(y==0){
             if(arr[x+1][y+1]==0){
               tempPiece+=0.25;
             }
             if(arr[x-1][y+1]==0){
               tempPiece+=0.25;
             }
           }
           if(y==7){
             if(arr[x+1][y-1]==0){
               tempPiece+=0.5;
             }
             if(arr[x-1][y-1]==0){
               tempPiece+=0.5;
             }
           }
         tempPiece+=20;
         ++mykingcount;
         tempPiece+=boardKing[x][y];
         total+=tempPiece;
       }
       if(arr[x][y]==1){
         theirpiecepos[theirpieceposIndex] = x;
         theirpiecepos[theirpieceposIndex+1] = y;
         theirpieceposIndex+=2;
         if(canattack(arr,x,y)){
             if(x==2){
               tempPiece+=30;
             }
             else{
               tempPiece+=8;
             }
           }
           if(y>0&&y<7){
             if(arr[x-1][y+1]==0){
               tempPiece+=0.5;
             }
             if(arr[x-1][y-1]==0){
               tempPiece+=0.5;
             }
           }
           if(y==0){
             if(arr[x-1][y+1]==0){
               tempPiece+=0.5;
             }
           }
           if(y==7){
             if(arr[x-1][y-1]==0){
               tempPiece+=0.5;
             }
           }
         tempPiece+=10;
         tempPiece+=boardNormal[7-x][y];
         tempPiece*=-1;
         ++theircount;
         total+=tempPiece;
       }
       if(arr[x][y]==3){
         theirpiecepos[theirpieceposIndex] = x;
         theirpiecepos[theirpieceposIndex+1] = y;
         theirpieceposIndex+=2;
         theirkingpos[theirkingposIndex] = x;
         theirkingpos[theirkingposIndex+1] = y;
         theirkingposIndex+=2;
         if(canattack(arr,x,y)){
               tempPiece+=6;
           }
           if(y>0&&y<7){
             if(arr[x+1][y+1]==0){
               tempPiece+=0.25;
             }
             if(arr[x+1][y-1]==0){
               tempPiece+=0.25;
             }
             if(arr[x-1][y+1]==0){
               tempPiece+=0.25;
             }
             if(arr[x-1][y-1]==0){
               tempPiece+=0.25;
             }
           }
           if(y==0){
             if(arr[x+1][y+1]==0){
               tempPiece+=0.25;
             }
             if(arr[x-1][y+1]==0){
               tempPiece+=0.25;
             }
           }
           if(y==7){
             if(arr[x+1][y-1]==0){
               tempPiece+=0.5;
             }
             if(arr[x-1][y-1]==0){
               tempPiece+=0.5;
             }
           }
         tempPiece+=20;
         tempPiece+=boardKing[7-x][y];
         tempPiece*=-1;
         ++theirkingcount;
         total+=tempPiece;
       }



     }
 }

 if((mykingcount>theirkingcount)&&(theircount+theirkingcount<6)){
   float dtotal = 0;
   for(int x = 0; x<mykingposIndex;x+=2){
     dtotal+=averageDistance(mykingpos[x],mykingpos[x+1],theirpiecepos,theirpieceposIndex);
   }
   total-=dtotal;
 }
 if((mykingcount<theirkingcount)&&(mycount+mykingcount<6)){
   float dtotal = 0;
   for(int x = 0; x<mykingposIndex;x+=2){
     dtotal+=averageDistance(mykingpos[x],mykingpos[x+1],theirpiecepos,theirpieceposIndex);
   }
   total+=dtotal;
 }
 return total;
}


float minimax(Node node, int childNum, float alpha, float beta) { //childNum keeps track of the index of the children
   cout << "\nDepth:" << node.depth << "\n";
   for (int i = 0; i < 8; i++) {
       for (int j = 0; j < 8; j++) {
           cout << node.boardState[i][j] << " ";
       }
       cout << "\n";
   }

   if (node.depth == node.maxDepth) { //Base case, need to also consider when game is won or there are no more valid moves
       node.value = evaluate(node.boardState);
       cout <<"Value is " << node.value  << " child num is " << childNum << "\n\n";
       return node.value;
   }
   if (node.isMax) {
       cout << "Is max\n";
       node.value = -10000;
       //for each child of node recursively max out the values
       int start = 1;
       int upper = node.children.size();
       for (int i = 0; i < upper || start == 1; i++) { //terminating cond should be i < node.children.size()
       start = 0;

           node.project(node.place, node.boardState, node.isMax);
           upper = node.children.size();

           cout<<"Size of child array is : "<<node.children.size()<<"\n";

           node.value = max(node.value, minimax(node.children[i],i,alpha,beta));
         //THIS IS A TEST, REPEATS BELOW ASWELL
           //node.project(node.place, node.boardState, node.isMax);


           alpha = max(alpha, node.value);
           if (alpha >= beta) {
               cout << "Pruned cause isMax is guaranteed " << alpha << " and isMin is guaranteed " << beta << "\n";
               break;
           }
       }
       cout << "Chose the max value as " << node.value << "\n";
       return node.value;
   } else {
       cout << "Is min\n";
       node.value = 10000;
       //for each child of node recursively min out the values
       int start = 1;
       int upper = node.children.size();
       for (int i = 0; i < upper || start == 1; i++) { // i < node.children.size()

           start = 0;
           //upper = node.children.size();
           node.project(node.place, node.boardState, node.isMax);
           upper = node.children.size();

           cout<<"Size of child array is : "<<node.children.size()<<"\n";

           int oldV = node.value;
           node.value = min(node.value, minimax(node.children[i],i,alpha,beta));

           //node.project(node.place, node.boardState, node.isMax);

           if (oldV != node.value) node.childIndex++;
           beta = min(beta, node.value);

           if (alpha >= beta) {
               cout << "Pruned cause isMin is guaranteed " << beta << " and isMax is guaranteed " << alpha << "\n";
               break;
           }
       }
       if (node.depth == 0) {
           --node.childIndex; //prevent off by one error
           cout << "Chose the min value as " << node.value << "\n";
           cout << "The index is " << node.childIndex << "\n";
           return node.childIndex;
       }

       cout << "Chose the min value as " << node.value << "\n";
       return node.value;
   }
}



int main(void) {
   srand(time(NULL));
   int board[8][8]={
   {0,2,0,2,0,2,0,2},//red side of the board initially
   {2,0,2,0,2,0,2,0},
   {0,2,0,2,0,2,0,2},
   {0,0,0,0,0,0,0,0},//middle of the board is initially empty
   {0,0,0,0,0,0,0,0},
   {1,0,1,0,1,0,1,0},//white side of the board initially
   {0,1,0,1,0,1,0,1},
   {1,0,1,0,1,0,1,0}};

   Node f(2,0,board);

   cout << minimax(f,f.childIndex,-10001,10001) << "\n";
   return 0;
}

