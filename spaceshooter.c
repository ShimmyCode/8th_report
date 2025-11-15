typedef volatile unsigned short hword;

#define VRAM 0x06000000
#define VRAM2 0x04000130
#define BGR(r, g, b) ((b << 10) + (g << 5) + r)

unsigned int random();

void draw_point(hword, hword, hword);             // 点の描画
void draw_rec(hword, hword, hword, hword, hword); // 四角形の描画
void draw_line(hword,hword,hword);               //線
void draw_title_line(hword,hword,hword);         //タイトルの描画
void draw_screen(hword,hword,hword);

void draw_S(hword,hword,hword);
void draw_P(hword,hword,hword);
void draw_A(hword,hword,hword);
void draw_C(hword,hword,hword);
void draw_E(hword,hword,hword);
void draw_R(hword,hword,hword);
void draw_T(hword,hword,hword);
void draw_O(hword,hword,hword);
void draw_M(hword,hword,hword);
void draw_L(hword,hword,hword);
void draw_I(hword,hword,hword);
void draw_N(hword,hword,hword);
void draw_G(hword,hword,hword);
void draw_V(hword,hword,hword);
void draw_Y(hword,hword,hword);

void draw_SPACE(hword,hword,hword);
void draw_PROTECTOR(hword,hword,hword);
void draw_PRESS_START(hword,hword,hword);
void draw_MISSION(hword,hword,hword);
void draw_CLEAR(hword,hword,hword);
void draw_GAMEOVER(hword,hword,hword);
void draw_TRY_AGAIN(hword,hword,hword);
void draw_shield(hword,hword,hword);

void draw_player(hword, hword, hword);            // プレイヤーの描画
void move_player(int);                            // プレイヤーの移動を反映
void shot_player();                               // プレイヤーの撃つ玉を描画
void draw_enemy_0(hword, hword, hword);           // 敵（青）の描画
void draw_enemy_1(hword, hword, hword);           // 敵（黄色）の描画
void move_enemy();                                // 敵の移動を反映
void shot_enemy();                                // 敵の撃つ玉を描画
void move_bullet();                               // 弾の移動を反映
void check_enemy();                               // 敵の生死を判定
void check_END();                                 // プレイヤーの生死を判定,クリア判定
void reset();                                     // 各変数のリセット
hword check_ABS(hword, hword, hword);            // 敵と玉、プレイヤーと玉の接触判定

hword *ptr;          // 描画用ポインタ
hword i, j, k;       // for文用変数
hword volatile *key; // キー入力用ポインタ
hword UP;         // 右キー
hword DOWN;          // 左キー
hword start;         // スタートキー
hword A;             // Aキー

hword BLACK;
hword PURPLE;
hword YELLOW;
hword RED;
hword CYAN;
hword WHITE;
hword BLUE;
hword WINDOW_BLUE;
hword WINDOW_CYAN;
hword GREEN;
hword DARK_GREEN;
hword LIGHT_GRAY;
hword DARK_GRAY;
hword DARK_PURPLE;




hword x[2][6], y[2][6], live[2][6], bullet_x[2][6], bullet_y[2][6]; 
// 敵の位置と状態、玉の位置を一括管理
hword title_x, title_y;                                      // タイトルの位置
hword player_x, player_y;                                      // プレイヤーの位置
hword bullet_player_x, bullet_player_y;                   // プレイヤーの玉の位置
hword vector;                                              // 敵の移動方向(左右)
int enemy_shooting;
int time_player_move;    // プレイヤーの再描画の間隔
int time_enemy_move;     // 敵の再描画の間隔
int time_player_bullet;  // プレイヤーの玉の再描画の間隔
int time_enemy_shooting; // 敵の玉の再描画の間隔
int time_player_bullet_move;
int time_enemy_bullet_move;
int status;     // 開始判定
int alive;      // 初期化
int GAME_CLEAR; // クリア判定
int player_speed; //プレイヤーのスピード
hword screen_x;
hword screen_y;

int main(void){
    // 描画開始
    ptr = (hword *)0x04000000;
    *ptr = 0x0F03;

    // 使う色を定義/////////////////////////////////////////////////////
    BLACK = BGR(0, 0, 0);
    PURPLE = BGR(31, 0, 31);
    DARK_PURPLE = BGR(31,10,31);
    CYAN = BGR(0, 31, 31);
    RED = BGR(31, 0, 0);
    YELLOW = BGR(31, 31, 0);
    WHITE = BGR(31,31,31);
    BLUE = BGR(0, 0, 31);
    WINDOW_BLUE = BGR(0, 0, 25);
    WINDOW_CYAN = BGR(0, 25, 25);
    GREEN = BGR(0, 31, 0);
    DARK_GRAY = BGR(8, 8, 8);
    LIGHT_GRAY = BGR(12, 12 , 12);
    DARK_GREEN = BGR(14, 26, 10);



    // 最初は0
    status = 0;
    alive = 0;
        
    // 乱数、プレイヤーの初期位置を定義///////////////////////////////////////////////////
    player_x = 30; //変更点
    player_y = 80; //変更点
    title_x = 70;
    title_y= 60;
    bullet_player_x=0;
    screen_x=240;
    screen_y=160;
    // 各キーを定義
    key = (hword *)VRAM2;
    UP = 0x0040;
    DOWN = 0x0080;
    start = 0x0008;
    A = 0x0001;


    // 初期画面（黒）
    for (i = 0; i < 240 * 160; i = i + 1){
        ptr = (hword *)VRAM + i;
        *ptr = BLUE; //////////////////////////////////////////
    }

        draw_title_line(title_x,title_y,WHITE); 
        draw_SPACE(title_x+30,title_y,BLACK);
        draw_PROTECTOR(title_x+12,title_y+15,BLACK);
        draw_PRESS_START(title_x+12,title_y+30,BLACK);
        draw_shield(title_x+42,title_y+15,BLACK);

    // 無限ループ
    while (1){
        // 未クリア時の処理
        if (alive == 0){
            // 初期状態時の処理
            if (status == 0){

                // スタートキーが押されるとゲーム開始
                if ((*key & start) != start){
                    alive = 1;
                    status = 1;
                }
            }
            else if (status == 1){
                draw_player(player_x, player_y, PURPLE); // プレイヤーの描画
                move_enemy();                            // 敵の描画、移動

                if ((*key & UP) != UP){
                    move_player(1); // プレイヤーの移動
                }
                else if ((*key & DOWN) != DOWN){
                    move_player(2); // プレイヤーの移動
                }
                if ((*key & A) != A){
                    shot_player(); // プレイヤーの玉描画
                }
                shot_enemy();  // 敵の玉描画
                move_bullet(); // 玉の移動
                check_enemy(); // 敵の生存判定
                check_END();   // 終了判定
            }
            else if (status == 2){ //ゲームオーバー

                draw_GAMEOVER(title_x+15,title_y+2,WHITE);
                draw_enemy_0(title_x+30,title_y-7,WHITE);
                draw_enemy_1(title_x+47,title_y-7,WHITE);
                draw_enemy_0(title_x+60,title_y-7,WHITE);
                
                draw_enemy_1(title_x+32,title_y+7,WHITE);
                draw_enemy_1(title_x+62,title_y+7,WHITE);
                
                draw_enemy_0(title_x+30,title_y+23,WHITE);
                draw_enemy_1(title_x+47,title_y+23,WHITE);
                draw_enemy_0(title_x+60,title_y+23,WHITE);

                draw_player(title_x+48,title_y+7,BLACK);
              

                draw_TRY_AGAIN(title_x+15,title_y+80,WHITE);

                // スタートキーを押したら再開
                if ((*key & start) != start)
                {
                    alive = 1;
                    status = 0;
                }
            }else { //ゲームクリア
                    draw_screen(screen_x,screen_y,CYAN);
                    draw_line(title_x+23,title_y+85,WHITE); 
                    draw_line(title_x+23,title_y+88,WHITE); 
                    draw_MISSION(title_x+18,title_y,BLACK);
                    draw_CLEAR(title_x+23,title_y+15,BLACK);
                    draw_player(title_x+100,title_y+42,BLACK);
                    draw_point(title_x+99,title_y+41,WHITE);
                    draw_point(title_x+97,title_y+41,WHITE);
                    draw_point(title_x+95,title_y+39,WHITE);
                    draw_point(title_x+93,title_y+37,WHITE);
                    draw_point(title_x+97,title_y+50,WHITE);
                    draw_point(title_x+95,title_y+52,WHITE);
                    draw_point(title_x+93,title_y+54,WHITE);

            while(1){
            if ((*key & start) != start)
                {
                    alive = 1;
                    status = 1;
                    break;
                    
                }
              } 
            }
        }else{

            reset(); // 各値を初期化

            // 待機画面
            for (i = 0; i < 240 * 160; i++)
            {
                ptr = (hword *)VRAM + i;
                *ptr = BLACK;
            }       
            alive = 0; // クリア判定をリセット
            GAME_CLEAR =2;
        }
    }
    return 0;
}

void draw_point(hword x, hword y, hword color){
    ptr = (hword *)(VRAM + (y * 480) + 2 * x);
    *ptr = color;
}

void reset(){
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 6; j++)
        {
            // それぞれの配列の値を初期化
            x[i][j] = 175 + ( 30* i);
            y[i][j] = 20 + 25 * j;
            live[i][j] = 1;
            bullet_y[i][j] = 0;
        }
    }
//////////////////////////////////////////////////////////////////////
    player_x = 30; //変更点
    player_y = 80; //変更点
    bullet_player_y = 0;
    bullet_player_x = 0;
    vector = 1;
    time_player_move = 0;
    time_enemy_move = 0;
    enemy_shooting = 0;
    time_player_bullet = 0;
    time_enemy_shooting = 0;
    time_player_bullet_move = 0;
    time_enemy_bullet_move = 0;
    GAME_CLEAR = 2;
}

//TITLE/////////////////////////////////////////////////////////////////

void draw_screen(hword x, hword y, hword color){
            for (i = 0; i < 160; i++){
            for (j = 0; j < 240; j++){
            draw_point(j, i, color);
        }
            }
}


void draw_line(hword x, hword y, hword color){
    int i,j;
    for(i=y-40; i<y-39;i++){ 
        for(j=x-40; j<x+77;j++){     
            draw_point(j, i, color);
            }
}
}
void draw_title_line(hword x, hword y, hword color){
    int i,j;
    //二つの線
    for(i=y-40; i<y-38;i++){ 
        for(j=x-20; j<x+120;j++){     
            draw_point(j, i, color);
            }
        }
    for(i=y-2; i<y; i++){ 
        for(j=x-20; j<x+120;j++){     
            draw_point(j, i, color);
            }
        }   
}

void draw_S(hword x, hword y, hword color) {
    // S
    int i, j;
    for(i = y - 36; i < y - 34; i++) {
        for(j = x; j < x + 7; j++) {
            draw_point(j, i, color);
        }
    }
    for(i = y - 34; i < y - 31; i++) {
        for(j = x; j < x + 2; j++) {
            draw_point(j, i, color);
        }
    }
    for(i = y - 31; i < y - 29; i++) {
        for(j = x; j < x + 7; j++) {
            draw_point(j, i, color);
        }
    }
    for(i = y - 29; i < y - 26; i++) {
        for(j = x + 5; j < x + 7; j++) {
            draw_point(j, i, color);
        }
    }
    for(i = y - 26; i < y - 24; i++) {
        for(j = x; j < x + 7; j++) {
            draw_point(j, i, color);
        }
    }
}

void draw_P(hword x, hword y, hword color) {
    // P
    int i, j;
    for(i = y - 36; i < y - 24; i++) {
        for(j = x; j < x + 2; j++) {
            draw_point(j, i, color);
        }
    }
    for(i = y - 36; i < y - 34; i++) {
        for(j = x + 2; j < x + 7; j++) {
            draw_point(j, i, color);
        }
    }
    for(i = y - 34; i < y - 31; i++) {
        for(j = x + 5; j < x + 7; j++) {
            draw_point(j, i, color);
        }
    }
    for(i = y - 31; i < y - 29; i++) {
        for(j = x + 2; j < x + 7; j++) {
            draw_point(j, i, color);
        }
    }
}

void draw_A(hword x, hword y, hword color) {
    // A
    int i, j;
    for(i = y - 36; i < y - 24; i++) {
        for(j = x; j < x + 2; j++) {
            draw_point(j, i, color);
        }
    }
    for(i = y - 36; i < y - 34; i++) {
        for(j = x + 2; j < x + 7; j++) {
            draw_point(j, i, color);
        }
    }
    for(i = y - 30; i < y - 28; i++) {
        for(j = x + 2; j < x + 7; j++) {
            draw_point(j, i, color);
        }
    }
    for(i = y - 36; i < y - 24; i++) {
        for(j = x + 5; j < x + 7; j++) {
            draw_point(j, i, color);
        }
    }
}

void draw_C(hword x, hword y, hword color) {
    // C
    int i, j;
    for(i = y - 36; i < y - 34; i++) {
        for(j = x; j < x + 7; j++) {
            draw_point(j, i, color);
        }
    }
    for(i = y - 34; i < y - 26; i++) {
        for(j = x; j < x + 2; j++) {
            draw_point(j, i, color);
        }
    }
    for(i = y - 26; i < y - 24; i++) {
        for(j = x; j < x + 7; j++) {
            draw_point(j, i, color);
        }
    }
}

void draw_E(hword x, hword y, hword color) {
    // E
    int i, j;
    for(i = y - 36; i < y - 34; i++) {
        for(j = x; j < x + 7; j++) {
            draw_point(j, i, color);
        }
    }
    for(i = y - 34; i < y - 31; i++) {
        for(j = x; j < x + 2; j++) {
            draw_point(j, i, color);
        }
    }
    for(i = y - 31; i < y - 29; i++) {
        for(j = x; j < x + 7; j++) {
            draw_point(j, i, color);
        }
    }
    for(i = y - 29; i < y - 26; i++) {
        for(j = x; j < x + 2; j++) {
            draw_point(j, i, color);
        }
    }
    for(i = y - 26; i < y - 24; i++) {
        for(j = x; j < x + 7; j++) {
            draw_point(j, i, color);
        }
    }
}

void draw_R(hword x, hword y, hword color) {
       // R
    int i, j;
    // 縦の線
    for(i = y - 36; i < y - 24; i++) {
        for(j = x; j < x + 2; j++) {
            draw_point(j, i, color);
        }
    }
    // 上の横線
    for(i = y - 36; i < y - 34; i++) {
        for(j = x + 2; j < x + 5; j++) {
            draw_point(j, i, color);
        }
    }
    // 下の横線
    for(i = y - 31; i < y - 29; i++) {
        for(j = x + 2; j < x + 7; j++) {
            draw_point(j, i, color);
        }
    }

    // 右上の縦線
    for(i = y - 34; i < y - 31; i++) {
        for(j = x + 5; j < x + 7; j++) {
            draw_point(j, i, color);
        }
    }
    // 斜め線
    for(i = y - 32; i < y - 29; i++) {
        for(j = x + 4; j < x + 7; j++) {
            draw_point(j, i + (j - x - 2)+1, color);
        }
    }
}

void draw_T(hword x, hword y, hword color) {
    // T
    int i, j;
    for(i = y - 36; i < y - 34; i++) {
        for(j = x; j < x + 7; j++) {
            draw_point(j, i, color);
        }
    }
    for(i = y - 34; i < y - 24; i++) {
        for(j = x + 2; j < x + 5; j++) {
            draw_point(j, i, color);
        }
    }
}

void draw_O(hword x, hword y, hword color) {
    // O
    int i, j;
    for(i = y - 36; i < y - 34; i++) {
        for(j = x; j < x + 7; j++) {
            draw_point(j, i, color);
        }
    }
    for(i = y - 36; i < y - 24; i++) {
        for(j = x; j < x + 2; j++) {
            draw_point(j, i, color);
        }
    }
    for(i = y - 36; i < y - 24; i++) {
        for(j = x + 5; j < x + 7; j++) {
            draw_point(j, i, color);
        }
    }
    for(i = y - 26; i < y - 24; i++) {
        for(j = x; j < x + 7; j++) {
            draw_point(j, i, color);
        }
    }
}

void draw_L(hword x, hword y, hword color) {
    // Draw the vertical part of L
    int i, j;
    for (i = y - 36; i < y - 24; i++) {
        for (j = x; j < x + 2; j++) {
            draw_point(j, i, color);
        }
    }
    
    // Draw the horizontal part of L
    for (i = y - 26; i < y - 24; i++) {
        for (j = x; j < x + 7; j++) {
            draw_point(j, i, color);
        }
    }
}

void draw_M(hword x, hword y, hword color) {
    // Draw the letter M
    int i, j;

    for (i = y - 36; i < y - 24; i++) {
        for (j = x; j < x + 2; j++) {
            draw_point(j-2, i, color);
        }
    }
    for (i = y - 36; i < y - 32; i++) {
        for (j = x + 2; j < x + 4; j++) {
            draw_point(j-2, i, color);
        }    }

    for (i = y - 32; i < y - 28; i++) {
        for (j = x + 4; j < x + 6; j++) {
            draw_point(j-2, i, color);
        }
    }
    
    for (i = y - 36; i < y - 32; i++) {
    for (j = x + 6; j < x + 8; j++) {
            draw_point(j-2, i, color);
        }
    }
    for (i = y - 36; i < y - 24; i++) {
        for (j = x + 8; j < x + 10; j++) {
            draw_point(j-2, i, color);
        }
    }

}

void draw_I(hword x, hword y, hword color) {
    // Draw the letter I
    int i, j;
    for(i = y - 36; i < y - 34; i++) {
        for(j = x; j < x + 7; j++) {
            draw_point(j, i, color);
        }
    }
    for(i = y - 34; i < y - 24; i++) {
        for(j = x + 2; j < x + 5; j++) {
            draw_point(j, i, color);
        }
    }
    for(i = y - 26; i < y - 24; i++) {
        for(j = x; j < x + 7; j++) {
            draw_point(j, i, color);
        }
    }
}

void draw_N(hword x, hword y, hword color) {
    // Draw the letter N
    int i, j;
     for (i = y - 36; i < y - 24; i++) {
        for (j = x; j < x + 2; j++) {
            draw_point(j, i, color);
            }
        }
    for (i = y - 33; i < y - 30; i++) {
        for (j = x + 2; j < x + 4; j++) {
            draw_point(j, i, color);
        }
    }
    for (i = y - 31; i < y - 28; i++) {
        for (j = x + 4; j < x + 6; j++) {
            draw_point(j, i, color);
        }
    }
    for (i = y - 36; i < y - 24; i++) {
        for (j = x + 6; j < x + 8; j++) {
            draw_point(j, i, color);
            }
        }
}

void draw_G(hword x, hword y, hword color) {
    // Cを元に作成
    int i, j;
    for(i = y - 36; i < y - 34; i++) {
        for(j = x; j < x + 7; j++) {
            draw_point(j, i, color);
        }
    }
    for(i = y - 34; i < y - 26; i++) {
        for(j = x; j < x + 2; j++) {
            draw_point(j, i, color);
        }
    }
    //Gの曲がったところ横（１）
    for(i = y - 30; i < y - 28; i++) {
        for(j = x + 3; j < x + 7; j++) {
            draw_point(j, i, color);
        }
    }
    //Gの曲がったところ横（２）
    for(i = y - 28; i < y - 27; i++) {
        for(j = x + 5; j < x + 7; j++) {
            draw_point(j, i, color);
        }
    }
    for(i = y - 27; i < y - 26; i++) {
        for(j = x + 5; j < x + 7; j++) {
            draw_point(j, i, color);
        }
    }

    for(i = y - 26; i < y - 24; i++) {
        for(j = x; j < x + 7; j++) {
            draw_point(j, i, color);
        }
    }
}

void draw_V(hword x, hword y, hword color){

    int i, j;
     for (i = y - 36; i < y - 28; i++) {
        for (j = x; j < x + 2; j++) {
            draw_point(j, i, color);
            }
        }
    for (i = y - 30; i < y - 26; i++) {
        for (j = x + 2; j < x + 4; j++) {
            draw_point(j, i, color);
        }
    }
    for (i = y - 27; i < y - 24; i++) {
        for (j = x + 4; j < x + 6; j++) {
            draw_point(j, i, color);
        }
    }
    for (i = y - 30; i < y - 26; i++) {
        for (j = x + 6; j < x + 8; j++) {
            draw_point(j, i, color);
        }
    }
    for (i = y - 36; i < y - 28; i++) {
        for (j = x + 8; j < x + 10; j++) {
            draw_point(j, i, color);
            }
        }
}

void draw_Y(hword x, hword y, hword color){

    int i, j;
     for (i = y - 36; i < y - 33; i++) {
        for (j = x; j < x + 2; j++) {
            draw_point(j, i, color);
            }
        }
    for (i = y - 33; i < y - 30; i++) {
        for (j = x + 2; j < x + 4; j++) {
            draw_point(j, i, color);
        }
    }
    for (i = y - 30; i < y - 25; i++) {
        for (j = x + 4; j < x + 6; j++) {
            draw_point(j, i, color);
        }
    }
    for (i = y - 33; i < y - 30; i++) {
        for (j = x + 6; j < x + 8; j++) {
            draw_point(j, i, color);
        }
    }
    for (i = y - 36; i < y - 33; i++) {
        for (j = x + 8; j < x + 10; j++) {
            draw_point(j, i, color);
            }
        }
}



void draw_SPACE(hword x, hword y, hword color) {
    hword space = 2;
    draw_S(x, y, color);
    draw_P(x + 7 + space, y, color);
    draw_A(x + 14 + 2 * space, y, color);
    draw_C(x + 21 + 3 * space, y, color);
    draw_E(x + 28 + 4 * space, y, color);
}

void draw_PROTECTOR(hword x, hword y, hword color) {
    hword space = 2;
    draw_P(x, y, color);
    draw_R(x + 7 + space, y, color);
    draw_O(x + 14 + 2 * space, y, color);
    draw_T(x + 21 + 3 * space, y, color);
    draw_E(x + 28 + 4 * space, y, color);
    draw_C(x + 35 + 5 * space, y, color);
    draw_T(x + 42 + 6 * space, y, color);
    draw_O(x + 49 + 7 * space, y, color);
    draw_R(x + 56 + 8*space, y, color);
}

void draw_PRESS_START(hword x, hword y, hword color){
    hword space = 2;
    hword move_down=50;
    draw_P(x-7, y+move_down, color);
    draw_R(x + space, y+move_down, color);
    draw_E(x + 7 + 2 * space, y+move_down, color);
    draw_S(x + 14 + 3 * space, y+move_down, color);
    draw_S(x + 21 + 4 * space, y+move_down, color);

    draw_S(x + 28 + 6 * space, y+move_down, color);
    draw_T(x + 35 + 7 * space, y+move_down, color);
    draw_A(x + 42 + 8 * space, y+move_down, color);
    draw_R(x + 49 + 9 * space, y+move_down, color);
    draw_T(x + 56 + 10 * space, y+move_down, color);
}

void draw_MISSION(hword x, hword y, hword color) {
    hword space = 2;
    draw_M(x, y, color);
    draw_I(x + 7 + space, y, color);
    draw_S(x + 14 + 2 * space, y, color);
    draw_S(x + 21 + 3 * space, y, color);
    draw_I(x + 28 + 4 * space, y, color);
    draw_O(x + 35 + 5 * space, y, color);
    draw_N(x + 42 + 6 * space, y, color);

  /*draw_C(x + 42 + 8 * space, y, color);
    draw_L(x + 49 + 9 * space, y, color);
    draw_E(x + 56 + 10 * space, y, color);
    draw_A(x + 63 + 12 * space, y, color);
    draw_R(x + 70 + 14 * space, y, color);*/
}

void draw_CLEAR(hword x, hword y, hword color) {
    hword space = 2;
    draw_C(x, y, color);
    draw_L(x + 7 + space, y, color);
    draw_E(x + 14 + 2 * space, y, color);
    draw_A(x + 21 + 3 * space, y, color);
    draw_R(x + 28 + 4 * space, y, color);
}

void draw_GAMEOVER(hword x, hword y, hword color) {
    hword space = 2;
    draw_G(x+1, y, color);
    draw_A(x+1 + 7 + space, y, color);
    draw_M(x+2 + 14 + 2 * space, y, color);
    draw_E(x+2 + 21 + 3 * space, y, color);
    draw_O(x+2 + 28 + 4 * space, y, color);
    draw_V(x+1 + 35 + 5 * space, y, color);
    draw_E(x+3 + 42 + 6 * space, y, color);
    draw_R(x+3 + 49 + 7 * space, y, color);
}

void draw_TRY_AGAIN(hword x, hword y, hword color) {
    hword space = 2;
    draw_T(x, y, color);
    draw_R(x + 7 + space, y, color);
    draw_Y(x + 14 + 2 * space, y, color);
    draw_A(x + 21 + 4 * space, y, color);
    draw_G(x + 28 + 5 * space, y, color);
    draw_A(x + 35 + 6 * space, y, color);
    draw_I(x + 42 + 7 * space, y, color);
    draw_N(x + 49 + 8 * space, y, color);
}

void draw_shield(hword x, hword y, hword color){
    // Row 1
    for (j = 0; j < 20; j++) draw_point(x + j, y, WINDOW_CYAN);
    //Row 2
    for (j = 0; j < 20; j++) draw_point(x + j, y + 1, WINDOW_CYAN);
    //Row 3
    for (j = 0; j < 2; j++) draw_point(x + j, y + 2, WINDOW_CYAN);
    for (j = 2; j < 18; j++) draw_point(x + j, y + 2, GREEN);
    for (j = 18; j < 20; j++) draw_point(x + j, y + 2, WINDOW_CYAN);
    //Row 4
    for (j = 0; j < 2; j++) draw_point(x + j, y + 3, WINDOW_CYAN);
    for (j = 2; j < 9; j++) draw_point(x + j, y + 3, GREEN);
    for (j = 9; j < 11; j++) draw_point(x + j, y + 3, WHITE);
    for (j = 11; j < 18; j++) draw_point(x + j, y + 3, GREEN);
    for (j = 18; j < 20; j++) draw_point(x + j, y + 3, WINDOW_CYAN);
    //Row 5
    for (j = 0; j < 2; j++) draw_point(x + j, y + 4, WINDOW_CYAN);
    for (j = 2; j < 9; j++) draw_point(x + j, y + 4, GREEN);
    for (j = 9; j < 11; j++) draw_point(x + j, y + 4, WHITE);
    for (j = 11; j < 18; j++) draw_point(x + j, y + 4, GREEN);
    for (j = 18; j < 20; j++) draw_point(x + j, y + 4, WINDOW_CYAN);
    //Row 6
    for (j = 0; j < 2; j++) draw_point(x + j, y + 5, WINDOW_CYAN);
    for (j = 2; j < 9; j++) draw_point(x + j, y + 5, GREEN);
    for (j = 9; j < 11; j++) draw_point(x + j, y + 5, WHITE);
    for (j = 11; j < 18; j++) draw_point(x + j, y + 5, GREEN);
    for (j = 18; j < 20; j++) draw_point(x + j, y + 5, WINDOW_CYAN);
    //Row 7
    for (j = 0; j < 2; j++) draw_point(x + j, y + 6, WINDOW_CYAN);
    for (j = 2; j < 9; j++) draw_point(x + j, y + 6, GREEN);
    for (j = 9; j < 11; j++) draw_point(x + j, y + 6, WHITE);
    for (j = 11; j < 18; j++) draw_point(x + j, y + 6, GREEN);
    for (j = 18; j < 20; j++) draw_point(x + j, y + 6, WINDOW_CYAN);
    //Row 8
    for (j = 0; j < 2; j++) draw_point(x + j, y + 7, WINDOW_CYAN);
    for (j = 2; j < 9; j++) draw_point(x + j, y + 7, GREEN);
    for (j = 9; j < 11; j++) draw_point(x + j, y + 7, WHITE);
    for (j = 11; j < 18; j++) draw_point(x + j, y + 7, GREEN);
    for (j = 18; j < 20; j++) draw_point(x + j, y + 7, WINDOW_CYAN);
    //Row 9
    for (j = 0; j < 2; j++) draw_point(x + j, y + 8, WINDOW_CYAN);
    for (j = 2; j < 4; j++) draw_point(x + j, y + 8, GREEN);
    for (j = 4; j < 16; j++) draw_point(x + j, y + 8, WHITE);
    for (j = 16; j < 18; j++) draw_point(x + j, y + 8, GREEN);
    for (j = 18; j < 20; j++) draw_point(x + j, y + 8, WINDOW_CYAN);
    //Row 10
    for (j = 0; j < 2; j++) draw_point(x + j, y + 9, WINDOW_CYAN);
    for (j = 2; j < 4; j++) draw_point(x + j, y + 9, GREEN);
    for (j = 4; j < 16; j++) draw_point(x + j, y + 9, WHITE);
    for (j = 16; j < 18; j++) draw_point(x + j, y + 9, GREEN);
    for (j = 18; j < 20; j++) draw_point(x + j, y + 9, WINDOW_CYAN);
    //Row 11
    for (j = 0; j < 2; j++) draw_point(x + j, y + 10, WINDOW_CYAN);
    for (j = 2; j < 9; j++) draw_point(x + j, y + 10, GREEN);
    for (j = 9; j < 11; j++) draw_point(x + j, y + 10, WHITE);
    for (j = 11; j < 18; j++) draw_point(x + j, y + 10, GREEN);
    for (j = 18; j < 20; j++) draw_point(x + j, y + 10, WINDOW_CYAN);
    //Row 12
    for (j = 0; j < 2; j++) draw_point(x + j, y + 11, WINDOW_CYAN);
    for (j = 2; j < 9; j++) draw_point(x + j, y + 11, GREEN);
    for (j = 9; j < 11; j++) draw_point(x + j, y + 11, WHITE);
    for (j = 11; j < 18; j++) draw_point(x + j, y + 11, GREEN);
    for (j = 18; j < 20; j++) draw_point(x + j, y + 11, WINDOW_CYAN);
    //Row 13
    for (j = 1; j < 3; j++) draw_point(x + j, y + 12, WINDOW_CYAN);
    for (j = 2; j < 9; j++) draw_point(x + j, y + 12, GREEN);
    for (j = 9; j < 11; j++) draw_point(x + j, y + 12, WHITE);
    for (j = 11; j < 18; j++) draw_point(x + j, y + 12, GREEN);
    for (j = 18; j < 19; j++) draw_point(x + j, y + 12, WINDOW_CYAN);
    //Row 14
    for (j = 1; j < 3; j++) draw_point(x + j, y + 13, WINDOW_CYAN);
    for (j = 3; j < 9; j++) draw_point(x + j, y + 13, GREEN);
    for (j = 9; j < 11; j++) draw_point(x + j, y + 13, WHITE);
    for (j = 11; j < 17; j++) draw_point(x + j, y + 13, GREEN);
    for (j = 17; j < 19; j++) draw_point(x + j, y + 13, WINDOW_CYAN);
    //Row 15
    for (j = 2; j < 4; j++) draw_point(x + j, y + 14, WINDOW_CYAN);
    for (j = 4; j < 9; j++) draw_point(x + j, y + 14, GREEN);
    for (j = 9; j < 11; j++) draw_point(x + j, y + 14, WHITE);
    for (j = 11; j < 16; j++) draw_point(x + j, y + 14, GREEN);
    for (j = 16; j < 18; j++) draw_point(x + j, y + 14, WINDOW_CYAN);
    //Row 16
    for (j = 3; j < 5; j++) draw_point(x + j, y + 15, WINDOW_CYAN);
    for (j = 5; j < 9; j++) draw_point(x + j, y + 15, GREEN);
    for (j = 9; j < 11; j++) draw_point(x + j, y + 15, WHITE);
    for (j = 11; j < 15; j++) draw_point(x + j, y + 15, GREEN);
    for (j = 15; j < 17; j++) draw_point(x + j, y + 15, WINDOW_CYAN);
    //Row 17
    for (j = 4; j < 7; j++) draw_point(x + j, y + 16, WINDOW_CYAN);
    for (j = 7; j < 13; j++) draw_point(x + j, y + 16, GREEN);
    for (j = 13; j < 16; j++) draw_point(x + j, y + 16, WINDOW_CYAN);
    //Row 18
    for (j = 6; j < 14; j++) draw_point(x + j, y + 17, WINDOW_CYAN);
    //Row 19
    for (j = 7; j < 13; j++) draw_point(x + j, y + 18, WINDOW_CYAN);
    //Row 20
    for (j = 7; j < 13; j++) draw_point(x + j, y + 19, WINDOW_CYAN);




}


void draw_rec(hword ax, hword ay, hword x, hword y, hword color){
    // ax,ayは四角形の中心の座標
    hword xa = ax - x / 2;
    hword xb = ax + x / 2;
    hword ya = ay - y / 2;
    hword yb = ay + y / 2;
    hword a = xa;
    while (ya <= yb)
    {
        xa = a;
        while (xa <= xb)
        {
            draw_point(xa, ya, color);
            xa = xa + 1;
        }
        ya = ya + 1;
    }
}

void draw_player(hword x, hword y, hword color){
    hword i, j;
    // Row 1
    for (j = 2; j < 4; j++) draw_point(x + j, y, DARK_GREEN);
    // Row 2
    for (j = 0; j < 4; j++) draw_point(x, y + 1, DARK_PURPLE);
    for (j = 2; j < 4; j++) draw_point(x + j, y + 1, GREEN);
    for (j = 4; j < 5; j++) draw_point(x + j, y + 1, RED);
    for (j = 5; j < 6; j++) draw_point(x + j, y + 1, DARK_GREEN);

    // Row 3
    for (j = 1; j < 2; j++) draw_point(x+j, y + 2, DARK_PURPLE);
    for (j = 2; j < 4; j++) draw_point(x + j, y + 2, GREEN);
    for (j = 4; j < 6; j++) draw_point(x + j, y + 2, DARK_GREEN);
    for (j = 6; j < 7; j++) draw_point(x + j, y + 2, GREEN);

    // Row 4
    for (j = 0; j < 2; j++) draw_point(x+j, y + 3, DARK_GRAY);
    for (j = 2; j < 3; j++) draw_point(x + j, y + 3, GREEN);
    for (j = 3; j < 4; j++) draw_point(x + j, y + 3, DARK_PURPLE);
    for (j = 4; j < 5; j++) draw_point(x + j, y + 3, DARK_GRAY);
    for (j = 5; j < 7; j++) draw_point(x + j, y + 3, WINDOW_CYAN);
    for (j = 7; j < 8; j++) draw_point(x + j, y + 3, DARK_GREEN);
    for (j = 8; j < 10; j++) draw_point(x +j, y + 3, DARK_GRAY);

    // Row 5
    for (j = 0; j < 2; j++) draw_point(x+j, y + 4, DARK_GRAY);
    for (j = 2; j < 3; j++) draw_point(x + j, y + 4, DARK_GREEN);
    for (j = 3; j < 5; j++) draw_point(x + j, y + 4, GREEN);
    for (j = 5; j < 6; j++) draw_point(x + j, y + 4, WINDOW_CYAN);
    for (j = 6; j < 7; j++) draw_point(x + j, y + 4, WINDOW_BLUE);
    for (j = 7; j < 8; j++) draw_point(x + j, y + 4, DARK_GREEN);
    for (j = 8; j < 9; j++) draw_point(x + j, y + 4, LIGHT_GRAY);
    for (j = 9; j < 10; j++) draw_point(x + j, y + 4, DARK_GRAY);

    // Row 6
    for (j = 1; j < 2; j++) draw_point(x+j, y + 5, DARK_PURPLE);
    for (j = 2; j < 4; j++) draw_point(x + j, y + 5, GREEN);
    for (j = 4; j < 6; j++) draw_point(x + j, y + 5, DARK_GREEN);
    for (j = 6; j < 7; j++) draw_point(x + j, y + 5, GREEN);

    //Row 7
    for (j = 0; j < 4; j++) draw_point(x, y + 6, DARK_PURPLE);
    for (j = 2; j < 4; j++) draw_point(x + j, y + 6, GREEN);
    for (j = 4; j < 5; j++) draw_point(x + j, y + 6, RED);
    for (j = 5; j < 6; j++) draw_point(x + j, y + 6, DARK_GREEN);

    //Row 8    
    for (j = 2; j < 4; j++) draw_point(x + j, y + 7, DARK_GREEN);
}

void move_player(int i)
{
    player_speed = 2;
    int plus=7;
    int minus=-1;

    if (i == 1) // 上キーが押されたとき
    {
        // プレイヤーの再描画の間隔
        if (time_player_move < 30)
        {
            time_player_move = time_player_move + 2;
        }
        else
        {
            time_player_move = 0;
        }

        // time_player_moveが10足されるごとに処理
        if (time_player_move == 0)
        {

            draw_rec(player_x, player_y+plus, 10, 8, BLACK); // 元居た位置を黒にする
            draw_rec(player_x+1, player_y+plus, 10, 8, BLACK); // 元居た位置を黒にする
            draw_rec(player_x+2, player_y+plus, 10, 8, BLACK); // 元居た位置を黒にする
            draw_rec(player_x+3, player_y+plus, 10, 8, BLACK); // 元居た位置を黒にする
            draw_rec(player_x+4, player_y+plus, 10, 8, BLACK); // 元居た位置を黒にする
            draw_rec(player_x+5, player_y+plus, 10, 8, BLACK); // 元居た位置を黒にする
            draw_rec(player_x+6, player_y+plus, 10, 8, BLACK); // 元居た位置を黒にする
            draw_rec(player_x+7, player_y+plus, 10, 8, BLACK); // 元居た位置を黒にする
            draw_rec(player_x+8, player_y+plus, 10, 8, BLACK); // 元居た位置を黒にする
            draw_rec(player_x+9, player_y+plus, 10, 8, BLACK); // 元居た位置を黒にする


                            
            player_y = player_y - player_speed; // 右端まではプレイヤーのx座標を右に更新

            if (player_y < 4)
            {
                player_y = 4; // 右端まではプレイヤーのx座標を右に更新
            }
            draw_player(player_x, player_y, BLACK); // 再描画
            time_player_move = 1;               // 連続で処理が起きないよう値を更新
        }
    }
    else if (i == 2) // 下キーが押されたとき
    {
        if (time_player_move < 30)
        {
            time_player_move = time_player_move + 2;
        }
        else
        {
            time_player_move = 0;
        }
        if (time_player_move == 0)
        {
            
            draw_rec(player_x, player_y+minus, 10, 8, BLACK); // 元居た位置を黒にする
            draw_rec(player_x+1, player_y+minus, 10, 8, BLACK); // 元居た位置を黒にする
            draw_rec(player_x+2, player_y+minus, 10, 8, BLACK); // 元居た位置を黒にする
            draw_rec(player_x+3, player_y+minus, 10, 8, BLACK); // 元居た位置を黒にする
            draw_rec(player_x+4, player_y+minus, 10, 8, BLACK); // 元居た位置を黒にする
            draw_rec(player_x+5, player_y+minus, 10, 8, BLACK); // 元居た位置を黒にする
            draw_rec(player_x+6, player_y+minus, 10, 8, BLACK); // 元居た位置を黒にする
            draw_rec(player_x+7, player_y+minus, 10, 8, BLACK); // 元居た位置を黒にする
            draw_rec(player_x+8, player_y+minus, 10, 8, BLACK); // 元居た位置を黒にする
            draw_rec(player_x+9, player_y+minus, 10, 8, BLACK); // 元居た位置を黒にする

            player_y = player_y + player_speed;

            if (player_y > 147)
            {
                player_y = 147;
            }
            draw_player(player_x, player_y, BLACK);
        }
    }
}
/////////////////////////////////////////////////////////
//水色
void draw_enemy_0(hword x, hword y, hword color)
{
     hword i, j;
    // Row 1
    for (j = 5; j < 9; j++) draw_point(x + j, y, WHITE);
    // Row 2
    for (j = 3; j < 5; j++) draw_point(x + j, y + 1, WHITE);
    for (j = 5; j < 9; j++) draw_point(x + j, y + 1, RED);
    for (j = 9; j < 11; j++) draw_point(x + j, y + 1, WHITE);
    // Row 3
    for (j = 2; j < 3; j++) draw_point(x + j, y + 2, WHITE);
    for (j = 3; j < 4; j++) draw_point(x + j, y + 2, LIGHT_GRAY);
    for (j = 4; j < 10; j++) draw_point(x + j, y + 2, RED);
    for (j = 10; j < 11; j++) draw_point(x + j, y + 2, LIGHT_GRAY);
    for (j = 11; j < 12; j++) draw_point(x + j, y + 2, WHITE);

    // Row 4
    for (j = 1; j < 2; j++) draw_point(x + j, y + 3, WHITE);
    for (j = 2; j < 3; j++) draw_point(x + j, y + 3, DARK_GRAY);
    for (j = 3; j < 4; j++) draw_point(x + j, y + 3, YELLOW);
    for (j = 4; j < 10; j++) draw_point(x + j, y + 3, LIGHT_GRAY);
    for (j = 10; j < 11; j++) draw_point(x + j, y + 3, YELLOW);
    for (j = 11; j < 12; j++) draw_point(x + j, y + 3, DARK_GRAY);
    for (j = 12; j < 13; j++) draw_point(x +j, y + 3, WHITE);

    // Row 5
    for (j = 0; j < 1; j++) draw_point(x + j, y + 4, WHITE);
    for (j = 1; j < 2; j++) draw_point(x + j, y + 4, DARK_GRAY);
    for (j = 2; j < 3; j++) draw_point(x + j, y + 4, YELLOW);
    for (j = 3; j < 4; j++) draw_point(x + j, y + 4, RED);
    for (j = 4; j < 5; j++) draw_point(x + j, y + 4, YELLOW);
    for (j = 5; j < 6; j++) draw_point(x + j, y + 4, DARK_GRAY);
    for (j = 6; j < 8; j++) draw_point(x + j, y + 4, RED);
    for (j = 8; j < 9; j++) draw_point(x + j, y + 4, DARK_GRAY);
    for (j = 9; j < 10; j++) draw_point(x + j, y + 4, YELLOW);
    for (j = 10; j <11; j++) draw_point(x + j, y + 4, RED);
    for (j = 11; j < 12; j++) draw_point(x + j, y + 4, YELLOW);
    for (j = 12; j < 13; j++) draw_point(x + j, y + 4, DARK_GRAY);
    for (j = 13; j < 14; j++) draw_point(x + j, y + 4, WHITE);

    // Row 6
    for (j = 0; j < 1; j++) draw_point(x + j, y + 5, WHITE);
    for (j = 1; j < 3; j++) draw_point(x + j, y + 5, DARK_GRAY);
    for (j = 3; j < 4; j++) draw_point(x + j, y + 5, YELLOW);
    for (j = 4; j < 10; j++) draw_point(x + j, y + 5, DARK_GRAY);
    for (j = 10; j < 11; j++) draw_point(x + j, y + 5, YELLOW);
    for (j = 11; j < 13; j++) draw_point(x + j, y + 5, DARK_GRAY);
    for (j = 13; j < 14; j++) draw_point(x + j, y + 5, WHITE);

    //Row 7
    for (j = 1; j < 3; j++) draw_point(x + j, y + 6, WHITE);
    for (j = 3; j < 6; j++) draw_point(x + j, y + 6, DARK_GRAY);
    for (j = 6; j < 8; j++) draw_point(x + j, y + 6, YELLOW);
    for (j = 8; j < 11; j++) draw_point(x + j, y + 6, DARK_GRAY);
    for (j = 11; j < 13; j++) draw_point(x + j, y + 6, WHITE);
    

    //Row 8    
    for (j = 3; j < 4; j++) draw_point(x + j, y + 7, WHITE);
    for (j = 4; j < 6; j++) draw_point(x + j, y + 7, DARK_GRAY);
    for (j = 6; j < 8; j++) draw_point(x + j, y + 7, YELLOW);
    for (j = 8; j < 10; j++) draw_point(x + j, y + 7, DARK_GRAY);
    for (j = 10; j < 11; j++) draw_point(x + j, y + 7, WHITE);

    //Row 9
    for (j = 4; j < 10; j++) draw_point(x + j, y + 8, WHITE);

}
//黄色
void draw_enemy_1(hword x, hword y, hword color)
{
    hword i, j;
    // Row 1
    for (j = 0; j < 10; j++) draw_point(x + j, y, WINDOW_BLUE);

    // Row 2
    for (j = 0; j < 1; j++) draw_point(x + j, y + 1, WINDOW_BLUE);
    for (j = 1; j < 3; j++) draw_point(x + j, y + 1, WINDOW_CYAN);
    for (j = 3; j < 7; j++) draw_point(x + j, y + 1, GREEN);
    for (j = 7; j < 9; j++) draw_point(x + j, y + 1, WINDOW_CYAN);
    for (j = 9; j < 10; j++) draw_point(x + j, y + 1, WINDOW_BLUE);

    // Row 3
    for (j = 0; j < 1; j++) draw_point(x + j, y + 2, WINDOW_BLUE);
    for (j = 1; j < 2; j++) draw_point(x + j, y + 2, WINDOW_CYAN);
    for (j = 2; j < 3; j++) draw_point(x + j, y + 2, RED);
    for (j = 3; j < 4; j++) draw_point(x + j, y + 2, GREEN);
    for (j = 4; j < 6; j++) draw_point(x + j, y + 2, RED);
    for (j = 6; j < 7; j++) draw_point(x + j, y + 2, GREEN);
    for (j = 7; j < 8; j++) draw_point(x + j, y + 2, RED);
    for (j = 8; j < 9; j++) draw_point(x + j, y + 2, WINDOW_CYAN);
    for (j = 9; j < 10; j++) draw_point(x + j, y + 2, WINDOW_BLUE);
    // Row 4
    for (j = 0; j < 1; j++) draw_point(x + j, y + 3, WINDOW_BLUE);
    for (j = 1; j < 2; j++) draw_point(x + j, y + 3, WINDOW_CYAN);
    for (j = 2; j < 3; j++) draw_point(x + j, y + 3, RED);
    for (j = 3; j < 4; j++) draw_point(x + j, y + 3, GREEN);
    for (j = 4; j < 6; j++) draw_point(x + j, y + 3, RED);
    for (j = 6; j < 7; j++) draw_point(x + j, y + 3, GREEN);
    for (j = 7; j < 8; j++) draw_point(x + j, y + 3, RED);
    for (j = 8; j < 9; j++) draw_point(x + j, y + 3, WINDOW_CYAN);
    for (j = 9; j < 10; j++) draw_point(x + j, y + 3, WINDOW_BLUE);

    // Row 5
    for (j = 0; j < 1; j++) draw_point(x + j, y + 4, WINDOW_BLUE);
    for (j = 1; j < 3; j++) draw_point(x + j, y + 4, WINDOW_CYAN);
    for (j = 3; j < 7; j++) draw_point(x + j, y + 4, GREEN);
    for (j = 7; j < 9; j++) draw_point(x + j, y + 4, WINDOW_CYAN);
    for (j = 9; j < 10; j++) draw_point(x + j, y + 4, WINDOW_BLUE);

    // Row 6
    for (j = 0; j < 10; j++) draw_point(x + j, y + 5, WINDOW_BLUE);

    //Row 7
    for (j = 0; j < 4; j++) draw_point(x + j, y + 6, WINDOW_BLUE);
    for (j = 4; j < 6; j++) draw_point(x + j, y + 6, WINDOW_CYAN);
    for (j = 6; j < 9; j++) draw_point(x + j, y + 6, WINDOW_BLUE);  

    //Row 8    
    for (j = 1; j < 9; j++) draw_point(x + j, y + 7, WINDOW_BLUE);

    //Row 9
    for (j = 2; j < 8; j++) draw_point(x + j, y + 8, WINDOW_BLUE);
}

void move_enemy()
{
    hword *ptr_up, *ptr_down;

    // 敵の再描画の間隔
    if (time_enemy_move < 50)
    {
        time_enemy_move = time_enemy_move + 1;
    }
    else
    {
        time_enemy_move = 0;
    }

    // time_enemy_moveが100足されるごとに処理
    if (time_enemy_move == 0)
    {
        for (i = 0; i < 6; i++)
        {
            // 敵(青）が生存している時
            if (live[0][i] == 1)
            {
                draw_rec(x[0][i]+10, y[0][i]+5, 27, 30, BLACK); // 元居た位置を黒にする
                if (vector == 1)
                {
                    y[0][i] = y[0][i] - 1; // 敵のy座標を上に更新

                }
                else if (vector == 2)
                {
                    y[0][i] = y[0][i] + 1; // 敵のy座標を下に更新
                }
                draw_enemy_0(x[0][i], y[0][i], BLACK); // 再描画
                
            }

            // 敵(黄色）が生存している時
            if (live[1][i] == 1)
            {
                draw_rec(x[1][i]+10, y[1][i]+5, 32, 30, BLACK);
                if (vector == 1)
                {
                    y[1][i] = y[1][i] + 1; // 敵のy座標を上に更新

                }
                else if (vector == 2)
                {
                    y[1][i] = y[1][i] - 1; // 敵のy座標を下に更新

                }
                draw_enemy_1(x[1][i], y[1][i], YELLOW); // 再描画
            }
                x[0][i] = x[0][i] - 1; // 敵のy座標を上に更新
                x[1][i] = x[1][i] - 1; // 敵のy座標を上に更新
        }

        // 敵の移動方向を変更する地点
        for (i = 0; i < 6; i++)
        {
            // 青色が上と下のボーダーに当たったのか確認
            if (live[0][i] == 1 && (y[0][i] <= 5 || y[0][i] >= 160))
            {
                vector = (vector == 1) ? 2 : 1; // 向きを変更
                break;
            }

            // 黄色が上と下のボーダーに当たったのか確認
            if (live[1][i] == 1 && (y[1][i] <= 5 || y[1][i] >= 160))
            {
                vector = (vector == 1) ? 2 : 1; // 向きを変更
                break;
            }
        }
    }
}


/*

vector = (vector == 1) ? 2 : 1; の意味

if (vector == 1) {
    vector = 2; // 下に変更
} else {
    vector = 1; // 上に変更
}


*/

// 疑似乱数　線形合同法
unsigned int random(){
    static unsigned int value = 12345; // Initial value

    // 線形合同法
    value = (value * 1103515245 + 12345) % 32768;

    return value;
}

void shot_enemy(){
    int i;
    if (time_enemy_shooting < 100){
        time_enemy_shooting = time_enemy_shooting + 1;
    }
    else{
        enemy_shooting = enemy_shooting + 1;
        time_enemy_shooting = 0;
    }

    if (time_enemy_shooting == 0){
        unsigned int RANDOM = random() % 9; // 疑似的に数字を得る

        for ( i = 0; i < 5; i++) {
            // 敵が生きている、またその敵が弾を打っていない場合
            if (live[i][RANDOM] == 1 && bullet_x[i][RANDOM] == 0)
            {
                // 敵の座標と弾の座標を一致させる
                bullet_x[i][RANDOM] = x[i][RANDOM] - 10;
                bullet_y[i][RANDOM] = y[i][RANDOM];
                draw_rec(bullet_x[i][RANDOM], bullet_y[i][RANDOM], 10, 2, WHITE); // 敵の弾の描画
                break;
            }
        }
    }
}
////////////////////////////////////////////////////////////////////

void shot_player(){
    if (bullet_player_x==0) // 弾が画面外に出るともう一度撃てる
    {  // プレイヤーより少し上の位置に描画
        bullet_player_x = player_x + 9;
        bullet_player_y = player_y;
        draw_rec(bullet_player_x, bullet_player_y+5, 10, 2, RED); // 玉の描画
    } 
}
void move_bullet(){

    if (bullet_player_x > 0){
        // プレイヤーの弾の再描画の間隔
        if (time_player_bullet_move < 15){
            time_player_bullet_move = time_player_bullet_move + 3;
        }else{
            draw_rec(bullet_player_x, bullet_player_y+5, 10, 2, BLACK); 
// 元居た位置を黒にする
            bullet_player_x = bullet_player_x + 3;   
             // プレイヤーの弾のx座標を更新
             if(bullet_player_x>236){
                bullet_player_x=0;
             }
            draw_rec(bullet_player_x, bullet_player_y+5, 10, 2, RED); // 弾の描画
            time_player_bullet_move = 0;                         // 間隔リセット
        } 
        }


    // 敵の弾の再描画の間隔
    if (time_enemy_bullet_move < 100){
        time_enemy_bullet_move = time_enemy_bullet_move + 5;
    }else{
        time_enemy_bullet_move = 0;
        for (i = 0; i < 2; i++){
            for (j = 0; j < 6; j++){
                // 敵が生存している場合
                if (live[i][j] == 1){
                    if (bullet_x[i][j] > 0 && bullet_x[i][j] < 240){

                        draw_rec(bullet_x[i][j], bullet_y[i][j], 10, 2, BLACK); // 元居た位置を黒にする
                        bullet_x[i][j] = bullet_x[i][j]-2; 
                        // 敵の玉のy座標を下に更新
                        draw_rec(bullet_x[i][j], bullet_y[i][j], 10, 2, WHITE); // 弾を描画
                    }
                    else if (bullet_x[i][j] <= 0){
                        
                        draw_rec(bullet_x[i][j], bullet_y[i][j], 10, 2, BLACK); // 元居た位置を黒にする、範囲外なので描画はしない
                        bullet_x[i][j] = 0;                  // 弾の座標を初期化
                    }
                }
            }
        }
    }
}

void check_enemy(){

    for (i = 0; i < 2; i++){
        for (j = 0; j < 6; j++){
            // 敵が生存していて、玉との位置の差がyが6未満かつxが11未満の場合
            if (live[i][j] == 1 && check_ABS(bullet_player_y, y[i][j], 6) == 1 && check_ABS(bullet_player_x, x[i][j], 11) == 1)
            {
                live[i][j] = 0; // 状態を死にする

                // 敵のいた場所を黒に塗りつぶす
                draw_rec(x[0][j]+13, y[0][j], 27, 25, BLACK);
                draw_rec(x[1][j]+13, y[1][j], 27, 25, BLACK);
                draw_rec(bullet_x[i][j], bullet_y[i][j], 10, 2, BLACK);//敵の弾を黒に塗りつぶす
                draw_rec(bullet_player_x, bullet_player_y+5, 10, 2, BLACK);//プレイヤーの弾を黒に塗りつぶす
                bullet_player_x = 0; // プレイヤーの弾の位置をリセット
            }
        }
    }
}

void check_END(){

    GAME_CLEAR = 1; // 状態をクリア状態にする
    for (i = 0; i < 2; i++){

        for (j = 0; j < 6; j++){

            // プレイヤーの描画と敵の玉のy座標が重なっている場合
            if (bullet_y[i][j] >= (player_y - 8) && bullet_y[i][j] <= (player_y + 9)&& bullet_x[i][j] <= (player_x + 9)&&bullet_x[i][j] >= (player_x - 8))
            {
                // プレイヤーのx座標と敵の玉のx座標の差が6未満の時
                if (check_ABS(player_x, bullet_x[i][j], 11) == 1){

                    // プレイヤーが死亡
                    alive = 1;
                    status = 2;
                }
            }

            // 敵が生存していた場合
            if (live[i][j] == 1){

                GAME_CLEAR = 0; // ゲームは未クリア
            }

        }
    }
    if (GAME_CLEAR == 1) {//敵が全滅している場合
        alive = 0;
        status = 3;
    }
}

//あたり判定
hword check_ABS(hword a, hword b, hword t){
    if (a >= b){

        if (a - b < t){

            return 1;

        }else{
            return 0;
        }

    }else{

        if (b - a < t){

            return 1;

        }else{
            return 0;
        }
    }
}

