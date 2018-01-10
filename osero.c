#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define WALL 0
#define BLACK 1
#define WHITE -1
#define SPACE 2

int PlayerSelect(int);
int PlayerChange(int);
int Number();
int Value(int,int);
void BoardPrint();
void Reverse(int,int);
void push(int);
int pop();
int Search(int,int);
int Left(int,int);
int Right(int,int);
int Up(int,int);
int Down(int,int);
int UpLeft(int,int);
int UpRight(int,int);
int DownRight(int,int);
int DownLeft(int,int);
void AI_Reverse(int,int);
void Undo();
int MinMax(int,int,int,int);
int AlhpaBeta(int,int,int,int,int,int);
int Pass(int);
void finish();
void NumberPrint(int);

int board[] = {
  WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,
  WALL,SPACE,SPACE,SPACE,SPACE,SPACE,SPACE,SPACE,SPACE,
  WALL,SPACE,SPACE,SPACE,SPACE,SPACE,SPACE,SPACE,SPACE,
  WALL,SPACE,SPACE,SPACE,SPACE,SPACE,SPACE,SPACE,SPACE,
  WALL,SPACE,SPACE,SPACE,BLACK,WHITE,SPACE,SPACE,SPACE,
  WALL,SPACE,SPACE,SPACE,WHITE,BLACK,SPACE,SPACE,SPACE,
  WALL,SPACE,SPACE,SPACE,SPACE,SPACE,SPACE,SPACE,SPACE,
  WALL,SPACE,SPACE,SPACE,SPACE,SPACE,SPACE,SPACE,SPACE,
  WALL,SPACE,SPACE,SPACE,SPACE,SPACE,SPACE,SPACE,SPACE,
  WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL
};
int BP[] = {
  WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,
  WALL,120,-40,20,5,5,20,-40,120,
  WALL,-40,-60,-5,-5,-5,-5,-60,-40,
  WALL,20,-5,15,-1,-1,15,-5,20,
  WALL,5,-5,-1,0,0,-1,-5,5,
  WALL,5,-5,-1,0,0,-1,-5,5,
  WALL,20,-5,15,-1,-1,15,-5,20,
  WALL,-40,-60,-5,-5,-5,-5,-60,-40,
  WALL,120,-40,20,5,5,20,-40,120,
  WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL
};

int canPut[8];
int stack[1000];
int sp=0;


int main(void){
  int player=0;
  int AI = 0;
  int number=0;
  int isPut=0;
  int pass = 0;
  int AINum;
  int a;int b;
  int count = 4; //何手めか把握するため
  //先行後攻を決める
  while(player==0){
    player = PlayerSelect(player);
    AI = -player;
  }
  //盤面表示
  BoardPrint();

  if(player == -1){
    player = 1;
    AINum =AlhpaBeta(5,player,AI,-100,100,count);
    NumberPrint(AINum);

    b = Search(AINum,player);
    Reverse(AINum,player);
    count++;
    player = PlayerChange(player);
    BoardPrint();
  }


  //置く場所を選択
  int passCounter = 0;
  while(1){
    passCounter = 0;
    pass = Pass(player);
    if(pass == 0){
      while(isPut!=1){
        number = Number();
        isPut = Search(number,player);
      }
      isPut = 0;
      Reverse(number,player);
      count++;
      player = PlayerChange(player);
      BoardPrint();
    }else{
      printf("パス\n");
      player = PlayerChange(player);
      passCounter++;
      }

    pass = Pass(player);
    if(pass == 0){
      AINum = AlhpaBeta(5,player,AI,-10000,10000,count);
      NumberPrint(AINum);
      b = Search(AINum,player);
      Reverse(AINum,player);
      count++;
      player = PlayerChange(player);
      BoardPrint();
    }else{
      printf("パス\n");
      player = PlayerChange(player);
      passCounter++;
      }
    if(passCounter == 2){
      finish();
      return 0;
    }
    }
  return 0;
}

//順番を決める
int PlayerSelect(int player){
  char buf[10];
  char *f;
  printf("先行(x)か後攻(o)か選んでください。");
  f = fgets(buf,10,stdin);
  if(*f == 'x'){
    /*先行*/
    return 1;
  }else if(*f == 'o'){
    /*後攻*/
    return -1;
  }else{
    return 0;
  }
}
void push(int i){
  sp++;
  stack[sp] = i;
}

int pop(){
  sp--;
  return stack[sp];
}
//打つ
int Number(){
  char buf[10];
  char row;
  int column;
  char *f;
  char *p;
  printf("石を置く場所を入力してください。");
  p = fgets(buf,10,stdin);
  buf[strlen(buf)-1] = '\0';
  //改行消去
  f = strtok(p, ",");
  if(f!=NULL){
  row = *f;
  }

  f = strtok(NULL, ",");
  if(f!=NULL){
    column = *f;
  }
  switch(row){
    case 'a':
      row = 1;
      break;
    case 'b':
      row = 2;
      break;
    case 'c':
      row = 3;
      break;
    case 'd':
      row = 4;
      break;
    case 'e':
      row = 5;
      break;
    case 'f':
      row = 6;
      break;
    case 'g':
      row = 7;
      break;
    case 'h':
      row = 8;
      break;
    default:
      return 0;
      break;
  }
  switch(column){
    case '1':
      column = 9;
      break;
    case '2':
      column = 18;
      break;
    case '3':
      column = 27;
      break;
    case '4':
      column = 36;
      break;
    case '5':
      column = 45;
      break;
    case '6':
      column = 54;
      break;
    case '7':
      column = 63;
      break;
    case '8':
      column = 72;
      break;
    default:
      return 0;
      break;
  }

  return row+column;
}

void finish(){
  int i;
  int black = 0;
  int white = 0;
  for(i=10;i<81;i++){
    if(board[i]==BLACK){
      black++;
    }else if(board[i]==WHITE){
      white++;
    }
  }
  if(black>white){
    printf("x:%d\no:%d\nwinner:x",black,white);
  }else if(black<white){
    printf("x:%d\no:%d\nwinner:o",black,white);
  }else{
    printf("x:%d\no:%d\n引き分け",black,white);
  }
}

void NumberPrint(int number){
  int i,box;
  printf("コンピュータの手");
  for(i=10;i<100;i+=9){
    box = number - i;
    if(box<10){
      switch(box){
        case 0:
          printf("a,");
          break;
        case 1:
          printf("b,");
          break;
        case 2:
          printf("c,");
          break;
        case 3:
          printf("d,");
          break;
        case 4:
          printf("e,");
          break;
        case 5:
          printf("f,");
          break;
        case 6:
          printf("g,");
          break;
        case 7:
          printf("h,");
          break;
      }
      switch(i){
        case 10:
          printf("1\n");
          break;
        case 19:
          printf("2\n");
          break;
        case 28:
          printf("3\n");
          break;
        case 37:
          printf("4\n");
          break;
        case 46:
          printf("5\n");
          break;
        case 55:
          printf("6\n");
          break;
        case 64:
          printf("7\n");
          break;
        case 73:
          printf("8\n");
          break;
      }
    break;
    }
  }
}
int MinMax(int depth,int player,int AI,int count){
  int i,a;
  int isPut = 0;
  int value;
  int childValue;
  int bestNumber;

  //末端に来た時
  if(depth == 0){
    return Value(player,count);
  }
  if(player == AI){
    value = -100;
  }else{
    value = 100;
  }
  for(i=10;i<81;i++){
    isPut = Search(i,player);
    if(isPut == 1){
      //探索
      AI_Reverse(i,player);
      childValue = MinMax(depth-1,-player,AI,count+1);
      if(player == AI){
        if(childValue >value){
          value = childValue;
          bestNumber = i;
        }
      }else{
        if(childValue < value){
          value = childValue;
          bestNumber = i;
        }
      }
    Undo();
    }

  }
  if(depth == 6){
    return bestNumber;
  }else{
    return value;
  }
}

//打つ場所を決める
int AlhpaBeta(int depth,int player,int AI,int alhpa,int beta,int count){
  int i,a;
  int isPut = 0;
  int value;
  int childValue;
  int bestNumber;
  int pass;

  //末端に来た時
  if(depth == 0){
    return Value(player,count);
  }
  pass=Pass(player);
  if(pass==1){

    player = -player;
    pass = Pass(player);
    if(pass==1){
      return Value(player,count);
    }
  }
  if(player == AI){
    value = -1000;
  }else{
    value = 1000;
  }
  for(i=10;i<81;i++){
    isPut = Search(i,player);
    if(isPut == 1){
      //探索
      AI_Reverse(i,player);

      childValue = AlhpaBeta(depth-1,-player,AI,alhpa,beta,count+1);
      if(player == AI){
        if(childValue >value){
          value = childValue;
          alhpa = value;

          bestNumber = i;


        }
        if(value>beta){
          Undo();
          return value;
        }
      }else{
        if(childValue < value){
          value = childValue;
          beta = value;

          bestNumber = i;

        }
        if(value < alhpa){
          Undo();
          return value;
        }
      }
    Undo();
    }
  }
  if(depth == 5){
    return bestNumber;
  }else{
    return value;
  }
}

//評価
int Value(int player,int count){
  int value = 0;
  int put = 0;
  int i;
  int sum = 0;
  for(i = 10;i<81;i++){
    put += Search(i,player);
    if(board[i]==player){
      value = board[i]*BP[i] + value;
      sum++;
    }
  }

  if(player == 1){
    if(count < 40){
      //printf("黒の評価%d\n",value*3+put*5-sum);
      return -value*3-2*put+sum;
    }else{
      return -value*3-2*put-2*sum;
    }
  }else{
    if(count < 40){
      //printf("白の評価%d\n",-value);
      return value*3+2*put-sum;
    }else{
      return value*3+2*put+2*sum;
    }
}
}

//もとに戻す
void Undo(){
  int i;
  for(i = 0;i<1000;i++){
    int number = pop();
    if(number == 0){
      break;
    }else if(stack[sp-1] == 0){
      board[number] = 2;
    }else{
      board[number] = -board[number];
    }
  }
}
//AIひっくり返す
void AI_Reverse(int number,int player){
  int i;
  board[number] = player;
  push(number);
  if(canPut[0]==1){//左側
        for(i=1;i>0;i++){
          if(board[number-i] != player){
            board[number-i] = player;
            push(number-i);

          }else{
            break;
          }
        }
      }
  if(canPut[1]==1){//右側
        for(i=1;i>0;i++){
          if(board[number+i] != player){
            board[number+i] = player;
            push(number+i);
          }else{
            break;
          }
        }
      }
  if(canPut[2]==1){//上
        for(i=9;i>0;i+=9){
          if(board[number-i] != player){
            board[number-i] = player;
            push(number-i);
          }else{
            break;
          }
        }
      }
  if(canPut[3]==1){//下
        for(i=9;i>0;i+=9){
          if(board[number+i] != player){
            board[number+i] = player;
            push(number+i);
          }else{
            break;
          }
        }
      }
  if(canPut[4]==1){
        for(i=10;i>0;i+=10){
          if(board[number-i] != player){
            board[number-i] = player;
            push(number-i);
          }else{
            break;
          }
        }
      }
  if(canPut[5]==1){
        for(i=8;i>0;i+=8){
          if(board[number-i] != player){
            board[number-i] = player;
            push(number-i);
          }else{
            break;
          }
        }
      }
  if(canPut[6]==1){
        for(i=10;i>0;i+=10){
          if(board[number+i] != player){
            board[number+i] = player;
            push(number+i);
          }else{
            break;
          }
        }
      }
  if(canPut[7]==1){
        for(i=8;i>0;i+=8){
          if(board[number+i] != player){
            board[number+i] = player;
            push(number+i);
          }else{
            break;
          }
        }
      }
  push(0);
}

//先行
void Reverse(int number,int player){
  int i;
  board[number] = player;
  if(canPut[0]==1){//左側
    for(i=1;i>0;i++){
      if(board[number-i] != player){
        board[number-i] = player;
      }else{
        break;
      }
    }
  }
  if(canPut[1]==1){//右側
    for(i=1;i>0;i++){
      if(board[number+i] != player){
        board[number+i] = player;
      }else{
        break;
      }
    }
  }
  if(canPut[2]==1){//上
    for(i=9;i>0;i+=9){
      if(board[number-i] != player){
        board[number-i] = player;
      }else{
        break;
      }
    }
  }
  if(canPut[3]==1){//下
    for(i=9;i>0;i+=9){
      if(board[number+i] != player){
        board[number+i] = player;
      }else{
        break;
      }
    }
  }
  if(canPut[4]==1){
    for(i=10;i>0;i+=10){
      if(board[number-i] != player){
        board[number-i] = player;
      }else{
        break;
      }
    }
  }
  if(canPut[5]==1){
    for(i=8;i>0;i+=8){
      if(board[number-i] != player){
        board[number-i] = player;
      }else{
        break;
      }
    }
  }
  if(canPut[6]==1){
    for(i=10;i>0;i+=10){
      if(board[number+i] != player){
        board[number+i] = player;
      }else{
        break;
      }
    }
  }
  if(canPut[7]==1){
    for(i=8;i>0;i+=8){
      if(board[number+i] != player){
        board[number+i] = player;
      }else{
        break;
      }
    }
  }
}
//順番交代
int PlayerChange(int player){
  return player * -1;
}

int Pass(int player){
  int i;
  for(i = 10;i < 81;i++){
    if(Search(i,player)==1){
      return 0;
    }
  }

  return 1;
}
//着手できる場所を探す
int Search(int number,int player){
  canPut[0] = Left(number,player);
  canPut[1] = Right(number,player);
  canPut[2] = Up(number,player);
  canPut[3] = Down(number,player);
  canPut[4] = UpLeft(number,player);
  canPut[5] = UpRight(number,player);
  canPut[6] = DownRight(number,player);
  canPut[7] = DownLeft(number,player);
  int i;
  for(i=0;i<8;i++){
    if(canPut[i]==1){
      return 1;
    }
  }
  return 0;
}
//左探索
int Left(int number,int player){
  if(board[number]!=SPACE){
    return 0;
  }
  int i;
  if(board[number-1] == player){
    return 0;
  }else if(board[number-1] == SPACE || board[number-1] == WALL){
    return 0;
  }else{
    for(i = number-2; i > 1; i--){
      if(board[i] == player){
        return 1;
      }else if(board[i] == WALL || board[i] == SPACE){
        return 0;
      }
    }
  }
  return 0;
}

//右探索
int Right(int number,int player){
  if(board[number]!=SPACE){
    return 0;
  }
  int i;
  if(board[number+1] == player){
    return 0;
  }else if(board[number+1] == SPACE || board[number+1] == WALL){
    return 0;
  }else{
    for(i = number+2; i > 1; i++){
      if(board[i]== player){
        return 1;
      }else if(board[i] == WALL || board[i] == SPACE){
        return 0;
      }
    }
  }
  return 0;
}

//上探索
int Up(int number,int player){
  if(board[number]!=SPACE){
    return 0;
  }
  int i;
  if(board[number-9] == player){
    return 0;
  }else if(board[number-9] == SPACE || board[number-9] == WALL){
    return 0;
  }else{
    for(i = number-18; i > 1; i-=9){
      if(board[i]== player){
        return 1;
      }else if(board[i] == WALL || board[i] == SPACE){
        return 0;
      }
    }
  }
  return 0;
}

//下探索
int Down(int number,int player){
  if(board[number]!=SPACE){
    return 0;
  }
  int i;
  if(board[number+9] == player){
    return 0;
  }else if(board[number+9] == SPACE || board[number+9] == WALL){
    return 0;
  }else{
    for(i = number+18; i > 1; i+=9){
      if(board[i]== player){
        return 1;
      }else if(board[i] == WALL || board[i] == SPACE){
        return 0;
      }
    }
  }
  return 0;
}

//左上探索
int UpLeft(int number,int player){
  if(board[number]!=SPACE){
    return 0;
  }
  int i;
  if(board[number-10] == player){
    return 0;
  }else if(board[number-10] == SPACE || board[number-10] == WALL){
    return 0;
  }else{
    for(i = number-20; i > 1; i-=10){
      if(board[i]== player){
        return 1;
      }else if(board[i] == WALL || board[i] == SPACE){
        return 0;
      }
    }
  }
  return 0;
}

//右上探索
int UpRight(int number,int player){
  if(board[number]!=SPACE){
    return 0;
  }
  int i;
  if(board[number-8] == player){
    return 0;
  }else if(board[number-8] == SPACE || board[number-8] == WALL){
    return 0;
  }else{
    for(i = number-16; i > 1; i-=8){
      if(board[i]== player){
        return 1;
      }else if(board[i] == WALL || board[i] == SPACE){
        return 0;
      }
    }
  }
  return 0;
}

//右下探索
int DownRight(int number,int player){
  if(board[number]!=SPACE){
    return 0;
  }
  int i;
  if(board[number+10] == player){
    return 0;
  }else if(board[number+10] == SPACE || board[number+10] == WALL){
    return 0;
  }else{
    for(i = number+20; i < 1000; i+=10){
      if(board[i]== player){
        return 1;
      }else if(board[i] == WALL || board[i] == SPACE){
        return 0;
      }
    }
  }
  return 0;
}

//左下探索
int DownLeft(int number,int player){
  if(board[number]!=SPACE){
    return 0;
  }
  int i;
  if(board[number+8] == player){
    return 0;
  }else if(board[number+8] == SPACE || board[number+8] == WALL){
    return 0;
  }else{
    for(i = number+16; i > 1; i+=8){
      if(board[i]== player){
        return 1;
      }else if(board[i] == WALL || board[i] == SPACE){
        return 0;
      }
    }
  }
  return 0;
}


//盤面描写
void BoardPrint(){
  int i;
  for(i=0;i< 90;i++){
    if(board[i] == SPACE){
      printf("- ");
    }else if(board[i] == BLACK){
      printf("x ");
    }else if(board[i] == WHITE){
      printf("o ");
    }else{
      if(i==89){
        printf("\n");
      }
      if(i == 0){
        printf("  ");
      }else if(i == 1){
        printf("a ");
      }else if(i == 2){
        printf("b ");
      }else if(i == 3){
        printf("c ");
      }else if(i == 4){
        printf("d ");
      }else if(i == 5){
        printf("e ");
      }else if(i == 6){
        printf("f ");
      }else if(i == 7){
        printf("g ");
      }else if(i == 8){
        printf("h ");
      }else if(i == 9){
        printf("\n1 ");
      }else if(i == 18){
        printf("\n2 ");
      }else if(i == 27){
        printf("\n3 ");
      }else if(i == 36){
        printf("\n4 ");
      }else if(i == 45){
        printf("\n5 ");
      }else if(i == 54){
        printf("\n6 ");
      }else if(i == 63){
        printf("\n7 ");
      }else if(i == 72){
        printf("\n8 ");
      }
    }
  }
}
