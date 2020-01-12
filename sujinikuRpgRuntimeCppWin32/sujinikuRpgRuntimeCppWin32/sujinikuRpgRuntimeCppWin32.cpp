// sujinikuRpgRuntimeCppWin32.cpp: アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "sujinikuRpgRuntimeCppWin32.h"

#include <stdio.h> // セーブ用
#pragma warning(disable:4996) // fopen

#include <stdlib.h>
#include <time.h>

// nkaAgility[partyNinzu + idTemp] = monster_def_list[i

// #include "resource.h"

// 差分テスト // テスト終わったら、この行を消す
#define MAX_LOADSTRING 100
#define MAX_LENGTH 300 // 要素数に注意 

#define MODE_OP 100 // タイトル画面のモード番号
#define MODE_LOAD 120 

#define MODE_INITIAL 200 // ゲーム開始イベントのモード番号
#define MODE_MAP 300 // マップ画面のモード番号

#define MODE_MENU 400 // メニュー画面のモード番号
#define MODE_ITEM_MENU 410 // アイテムメニューのモード

#define MODE_SAVE_MENU 440 // セーブメニューのモード

#define MODE_BATTLE_COMMAND 500 // 戦闘画面のモード番号
#define MODE_BATTLE_NOW 520 // 戦闘画面の戦闘中のモード番号（コマンド受付け不可能）

#define MODE_BATTLE_WIN 580 // 戦闘勝利のモード番号



int idTemp = 0;

int whoAction = 5; // 0 なら主人公の攻撃。1なら敵の攻撃。試作用のとりあえずのフラグ。

int sankaAgility[20]; // 素早さ配列
int iremonoAgilityHairetu[20]; // 入れ物すばやさ配列
int actuinOrder[20]; // 行動順配列
int iremonoOrderHairetu[20] ; // 入れ物こうどうじゅん配列
int mikataAgility[20]; // 味方の隊列での素早さ配列。「並び替え」で隊列順が変わるので。

// 装備の材質:

enum item_material
{
	mateWood,
	mateLether,
	mateCopper,
	mateBronze,

	mateIron,
	mateRichMetal,
	mateElse,
	mateNothing,
};


enum weapon_type
{
	sword,
	spear,
	axe,
	crab,
	bow,
	rod,
	typeNothing,
	typeElse,

};





// グローバル変数:

int IsNewGame = 1; // 初めてこのゲームを起動するとき、1である。一度でもセーブすれば2になる。


int battleTimeFlag = 0; // 戦闘中のコマンド決定後の自動進行に使用。1だと自動進行。0で停止。

enum resource_embedded_flag { on, off };
enum resource_embedded_flag resource_embedded_var = off;


int partyNinzu=2, enemyNinzu=1;
int sankaNinzu = partyNinzu + enemyNinzu;


int partyNarabijyun[5] = { 0,1,2,3,4 }; // パーティ隊列の並び替えの処理に使う予定
int monsterNarabijyun[5] = { 0,1,2,3,4 }; // モンスターの戦闘中の行動順の処理に使う予定



struct item_def
{
	int item_id;
	TCHAR item_name[MAX_LENGTH];
	int power;
	int item_type;
};


struct item_have
{
	int have_item_id;
	int have_kosuu;
};


struct weapon_have
{
	int have_weapon_id;
};





struct weapon_def
{
	int weapon_id;
	TCHAR weapon_name[MAX_LENGTH];
	int material; 
	int weapon_type; 
	int weaponPower;// 攻撃力


};



int equipWeaponPower;




struct monster_def
{
	int monster_id;
	TCHAR monster_name[MAX_LENGTH];
	int mon_hp_max;
	int mon_agility;

	int mon_gold;
	int mon_exp;
};


// 味方パーティ構造体 (モンスター構造体の流用)
struct heros_def
{
	int heros_id;
	TCHAR heros_name[MAX_LENGTH];
	int heros_hp;
	int heros_hp_max;
	int heros_agility;

	int heros_gold; // ウィザードリィみたいに各キャラごとにゴールドを持てるようになってる。
	int heros_exp;
};



enum next_attack_flag { next_is_hero, next_is_enemy };
enum next_attack_flag next_attack_var = next_is_hero;


static int encount_monters_id = 1;


struct monsterTairetu_def
{
	int monsterTairetu[10];
};






static int selecting_item = 1 ;
static int selecting_item_x = 1;
static int selecting_item_y = 1;

// 戦闘中に使用する変数。 モンスター定義とは別物。
static TCHAR monster_name[30];
static int monster_hp = 10;



static int enemy_alive[2] = { 1, 1}; // 1なら生きてる。0なら死亡。とりあえず2匹ぶん
// カッコ内は敵の番号。0番から数えている。

static int encount_mons_alive = 1;


HINSTANCE hInst;                                // 現在のインターフェイス
WCHAR szTitle[MAX_LOADSTRING];                  // タイトル バーのテキスト
WCHAR szWindowClass[MAX_LOADSTRING];            // メイン ウィンドウ クラス名


TCHAR mojibuf[MAX_LENGTH] = TEXT("はじめから");


static TCHAR filename_temp[100]; // ファイル読み書きで使う一時的なファイル名








static int mode_scene = 100 ;
static int selecting_OP = 1 ;

static int selecting_mainmenu = 1;


static int cursol_stop;
static int your_money =0;



static TCHAR drawTalkString1[MAX_LENGTH] = TEXT("temp_talk1"); // 会話欄の1行目
static TCHAR drawTalkString2[MAX_LENGTH] = TEXT("temp_talk2"); // 会話欄の2行目
static TCHAR drawTalkString3[MAX_LENGTH] = TEXT("temp_talk3"); // 会話欄の3行目


static int game_event1_end;
static int game_event2_end;

static int game_event1_end_dummuy;


static int key_remain = 1;


static int chara_x;
static int chara_y;

static int start_x = 3;
static int start_y = 4;


// マップのサイズの変数
static int map_x_size = 10;
static int map_y_size = 7;


// 進行先の壁判定のアルゴリズム用の変数
static int desti_x; // 進行先(destination)の壁判定のためのx座標変数 "desiti" means the destination.
static int desti_y; // 進行先の壁判定のためのx座標変数


// maptable の初期化 // 中身はとりあえず0. 安全のため、オーバーフロー時の影響を防ぐ。
static int maptable[10][10] = {
	{ 0,0,0,0,0,0,0,0,0,0 }, //0 x
	{ 0,0,0,0,0,0,0,0,0,0 }, //1
	{ 0,0,0,0,0,0,0,0,0,0 }, //2
	{ 0,0,0,0,0,0,0,0,0,0 }, //3
	{ 0,0,0,0,0,0,0,0,0,0 }, //4
	{ 0,0,0,0,0,0,0,0,0,0 }, //5
	{ 0,0,0,0,0,0,0,0,0,0 }  //6
};


// map1 のデータ
int map1table[10][10] = {
	{ 1,1,1,1,1,1,1,1,1,1 }, //0 x
{ 1,0,0,0,0,0,0,0,0,1 }, //1
{ 1,0,0,0,0,0,0,0,0,1 }, //2
{ 1,0,0,0,0,0,0,0,0,1 }, //3
{ 1,0,0,0,0,0,0,0,0,1 }, //4
{ 1,0,0,0,0,0,0,0,0,1 }, //5
{ 1,1,1,1,1,1,0,0,1,1 }  //6
};


int MapTrans_position_x_map1to_map2 = 7;
int MapTrans_position_y_map1to_map2 = 6;


static int MapTrans_position_x = MapTrans_position_x_map1to_map2;
static int MapTrans_position_y = MapTrans_position_y_map1to_map2;


struct MapTrans_def
{
	int MapTrans_id;

	int in_Where_Map;
	int to_Where_Map;

	int positionX; // その遷移のあるX座標
	int positionY;

	int chara_positionX; // 遷移先マップでのキャラの開始時点でのX座標
	int chara_positionY;

};


// マップ遷移用の構造体変数の作成
static struct MapTrans_def MapTrans_def_list[8];





// map2 のデータ

int map2table[10][10] = {
	{ 1,1,0,1,0,0,0,0,0,1 }, //0 x
	{ 0,0,0,0,0,0,0,0,0,0 }, //1
	{ 0,0,0,0,0,0,0,0,0,0 }, //2
	{ 0,0,0,0,0,0,0,0,0,1 }, //3
	{ 0,0,0,0,0,0,0,0,0,0 }, //4
	{ 0,0,0,0,0,0,0,0,0,0 }, //5
	{ 0,1,1,1,1,1,0,0,1,0 }  //6
};


// int MapTrans_position_x_map2to_map1 = 2;
// int MapTrans_position_y_map2to_map1 = 0;






enum direction { upward, rightward, downward , leftward };
enum direction hero1_direction = downward;

static int selecting_battle_mainmenu = 1;

static	int where_map = 1; // 最初のマップ

static int mapTrans_flag_is = 1;


// モンスターの位置。とりあえず2匹ぶん // なおフィールドマップにいる
static int positionX_enemy[5];
static int positionY_enemy[5];


static int iCount = 0; // 主に自動進行（敵の行動など）に使用
static int kCount = 0; // 主にキー入力の時間制限に使用


// マップチップ用ハンドル // チップ画像のロードは WM_CREATE などで行われる。
static HBITMAP hbmp_mapchip1;
static HBITMAP hbmp_mapchip2;


// キャラチップ用ハンドル // チップ画像のロードは WM_CREATE などで行われる。


struct chara_chip_def{

	HBITMAP hbmp_chara_chip_up; // static にしたらエラーになるので、staticにしたら駄目。
	HBITMAP hbmp_chara_chip_down;
	HBITMAP hbmp_chara_chip_right;
	HBITMAP hbmp_chara_chip_left;

};

static struct chara_chip_def chara_chip_list[8];



static HBITMAP hbmp_enemy;
static HBITMAP hbmp_MapTrans;



// マップ画面の描画用の関数
void Draw_map(HDC hdc) {
	
	// 最低限のマップチップ画像のロードは WM_CREATE などで既に行っている。


	// ハンドルなどの定義。 static にしたら駄目（表示されなくなる）。
	//HDC hdc;
	HDC hbackDC = CreateCompatibleDC(hdc); // 裏画面用のハンドル	

	// 中間作業用 （完成しても消したら駄目） static にしたら駄目（表示されなくなる）。
	HDC hMdc = CreateCompatibleDC(hdc); // 中間作業用ハンドル

	static HBITMAP hbmp; // 中間作業用 // こいつは static にしてもオッケー


	// マップサイズの読み込む
	hbmp = CreateCompatibleBitmap(hdc, 640 + 100, 480 + 100);
	SelectObject(hbackDC, hbmp); // hbackDCにマップサイズを読み込ませている


	// 実際にマップを裏画面に描画開始する

	int x_map = 0; // マップ描画の開始位置 // これは消しちゃ駄目。for文の記述の簡略化のため
	int y_map = 0;	

	for (x_map = 0; x_map <= 9; ++x_map)
	{
		for (y_map = 0; y_map <= 6; ++y_map)
		{
			
				switch (maptable[y_map][x_map])
				{
				case (0):
						hbmp = hbmp_mapchip1;
					break;

				case (1):					
						hbmp = hbmp_mapchip2;
					break;
				}
			
			SelectObject(hMdc, hbmp);
			BitBlt(hbackDC, 225 + x_map * 32, 140 + y_map * 32, 32, 32, hMdc, 0, 0, SRCCOPY);
			
			// DeleteDC(hMdc); // これを入れると、マップが表示されない。
		}
	}
	
	

	// 主人公のBMP画像をファイルから読み込む
	
	int chara_id = 0;
	if (hero1_direction == upward) {
			hbmp = chara_chip_list[chara_id].hbmp_chara_chip_up;
	}

	if (hero1_direction == rightward) {
			hbmp = chara_chip_list[chara_id].hbmp_chara_chip_right;
	}

	if (hero1_direction == downward) {
			hbmp = chara_chip_list[chara_id].hbmp_chara_chip_down;
	}

	if (hero1_direction == leftward) {
			hbmp = chara_chip_list[chara_id].hbmp_chara_chip_left;
	}


	SelectObject(hMdc, hbmp); // これを消すと、主人公ドットが表示されない。	
	BitBlt(hbackDC, 320 + (chara_x - start_x) * 32, 270 + (chara_y - start_y) * (32), 170, 180, hMdc, 0, 0, SRCCOPY);


	// マップ上の他キャラ（主人公以外）のBMP画像をファイルから読み込む

	if (where_map == 2) {
			
		for (int i = 0; i <= 1; i=i+1)		
		{				
			if (enemy_alive[i] == 1) {			
				hbmp = hbmp_enemy;
				
				SelectObject(hMdc, hbmp); // これを消すと、ドットが表示されない。				
				BitBlt(hbackDC, 320 + (positionX_enemy[i] - start_x) * 32, 270 + (positionY_enemy[i] - start_y) * (32), 170, 180, hMdc, 0, 0, SRCCOPY);									
			}
		}
	}


	if (mapTrans_flag_is == 1) {
		
		hbmp = hbmp_MapTrans; 
		SelectObject(hMdc, hbmp); // これを消すと、ドットが表示されない。
		BitBlt(hbackDC, 320 + (MapTrans_position_x - start_x) * 32, 270 + (MapTrans_position_y - start_y) * (32), 170, 180, hMdc, 0, 0, SRCCOPY);
	}


	// 裏画面から本画面に転送
	BitBlt(hdc, 0, 0, 700, 500, hbackDC, 0, 0, SRCCOPY); 

	hbmp = NULL; // これで初期化しないとバグり、何故かhbmp_MapTransにhbmp_enemyまたは主人公の向きが入ってる。
		

	// 中間ハンドルからhbmpを解除
	SelectObject(hMdc, NULL);
	SelectObject(hbackDC, NULL);

	// 不要になった中間物を削除	
	DeleteDC(hbackDC);
	DeleteDC(hMdc);

	DeleteObject(hbmp);
}


// マップでのカーソル押された時の移動可能の判定
void check_movable(HWND hWnd) {

	if (desti_x >= 0 && desti_x < map_x_size && desti_y >= 0 && desti_y < map_y_size)
	{

		if (maptable[desti_y][desti_x] == 1) // 進行先が壁
		{
			desti_x = chara_x; // 移動先の破棄
			desti_y = chara_y;
		}
		else if (maptable[desti_y][desti_x] != 1) // 進行先に移動可能
		{
			chara_x = desti_x;
			chara_y = desti_y;

			InvalidateRect(hWnd, NULL, FALSE);
			UpdateWindow(hWnd);
		}

		kCount = 0;

	}
}






// アイテムメニューでのカーソル位置の計算用
void item_select(HWND hWnd) {

	if (selecting_item < 1) {
		selecting_item = 1;
	}

	selecting_item_x = ( (selecting_item - 1) % 2) + 1;
	selecting_item_y = ( (selecting_item - 1) / 2) + 1;

	InvalidateRect(hWnd, NULL, FALSE);
	UpdateWindow(hWnd);

}


// アイテム処理の構造体変数の作成
static struct item_def item_def_list[8]; // アイテム処理の構造体配列の宣言
static struct item_have item_have_list[8];




// ウェポン処理の構造体変数の作成
static struct weapon_def weapon_def_list[5]; // 武器処理の構造体配列の宣言







// 戦闘遭遇用の構造体変数の作成
static struct monster_def monster_def_list[8];

static struct heros_def heros_def_list[8];



static struct monsterTairetu_def monsterTairetu_def_list[50];



// 戦闘への突入の処理 // のちのマップ判定で呼びだすので、戦闘突入とマップ判定の順序は固定のこと。
void battle_start(HWND hWnd) {

	// MessageBox(NULL, TEXT("battle_enemy_startにいる。"), TEXT("キーテスト"), MB_OK);

	// 以下、定型文
	key_remain = 0;

	monster_hp = monster_def_list[encount_monters_id - 1].mon_hp_max;


	encount_mons_alive = 1;
	selecting_battle_mainmenu = 1;

	mode_scene = MODE_BATTLE_COMMAND;

	InvalidateRect(hWnd, NULL, TRUE);
	UpdateWindow(hWnd);

}






// モンスターとの遭遇判定
void check_encount_enemy(HWND hWnd) {
	
	if (where_map == 2) {		
		for (int i = 0; i <= 1; i = i + 1)
		{
			if (enemy_alive[i] == 1) {
				if (chara_x == positionX_enemy[i] && chara_y == positionY_enemy[i]) {

					encount_monters_id = i+1;	// ここに、誰と遭遇したかを記述

					battle_start(hWnd);
				}
			}
		}
	}

}


void check_MapTransition(HWND hWnd) {
	//MessageBox(NULL, TEXT("マップ遷移の判定にいる。"), TEXT("キーテスト"), MB_OK);

	if (chara_x == MapTrans_def_list[(where_map) -1].positionX && chara_y == MapTrans_def_list[where_map -1].positionY)
	{			
		//MessageBox(NULL, TEXT("マップ遷移の直前にいる。"), TEXT("キーテスト"), MB_OK);		

		chara_x = MapTrans_def_list[where_map - 1].chara_positionX;
		chara_y = MapTrans_def_list[where_map - 1].chara_positionY;

		where_map = MapTrans_def_list[ where_map - 1].to_Where_Map; // マップを移動先に上書き
		
		//mapTrans_flag_is = 1; // 移動先にマップ遷移が存在するか
		

		
		desti_x = chara_x; // 移動先のリセット		
		desti_y = chara_y;		



		// ここから先、移動先のマップ描画のための処理

		// 移動先マップにあるマップ遷移の情報を書く
		MapTrans_position_x = MapTrans_def_list[ where_map - 1].positionX;
		MapTrans_position_y = MapTrans_def_list[ where_map - 1].positionY;


		// maptableに移動先マップを代入
		int x_map = 0, y_map = 0;

		for (x_map = 0; x_map <= 9; ++x_map)
		{
			for (y_map = 0; y_map <= 6; ++y_map)
			{
				// 代入内容は、移動先マップの代入をすることに注意

				if (where_map == 1) {
					maptable[y_map][x_map] = map1table[y_map][x_map];
				}

				if (where_map == 2) {
					maptable[y_map][x_map] = map2table[y_map][x_map];
				}
				
			}
		}

		InvalidateRect(hWnd, NULL, FALSE);
		UpdateWindow(hWnd);
	}		
}




static int hero1_already_attack_flag = 0;
static int enemy_already_attack_flag = 0;

static int battle_key = 1;

static int damage_HeroAttack = 1;
static int damage_EnemyAttack = 0;

void hero1_attack(HWND hWnd){

	// 主人公たちの攻撃
	{
		
		/* 乱数の種 */
		// wWinMain で定義済み
		

		/* サイコロ */
		damage_HeroAttack = rand() % 6 + 2 + equipWeaponPower;

		// 敵にダメージ
		monster_hp = monster_hp - damage_HeroAttack;

		// モンスターの死亡の処理
		if (monster_hp < 1) {
			monster_hp = 0;

			enemy_alive[ ( encount_monters_id ) -1] = 0; // フィールド上の敵の除去

			encount_mons_alive = 0; // 現在戦闘中の敵を死亡にフラグ設定

		}

		InvalidateRect(hWnd, NULL, TRUE);
		UpdateWindow(hWnd);
	}	
}



void enemy_attack(HWND hWnd) {

	// 敵たちの攻撃
	{
		
		/* 乱数の種 */
		// wWinMain で定義済み

		/* サイコロ */
		damage_EnemyAttack = rand() % (6/2) + 0;

		// 主人公にダメージ
		heros_def_list[0].heros_hp = heros_def_list[0].heros_hp - damage_EnemyAttack;

		// 主人公の死亡判定
		if (heros_def_list[0].heros_hp < 1) {
			heros_def_list[0].heros_hp = 0;

			// ここにゲームオーバーへのモード遷移

		}
	}

	InvalidateRect(hWnd, NULL, TRUE);
	UpdateWindow(hWnd);

}



static TCHAR strCount[64];

void draw_battle_common_before(HDC hdc) {
	HBRUSH brasi_white_1;
	brasi_white_1 = CreateSolidBrush(RGB(255, 255, 255));
	SelectObject(hdc, brasi_white_1);

	Rectangle(hdc, 0, 0,
		700, 500);
}


void draw_battle_common_after(HDC hdc) {
	
	HPEN pen_blue;

	pen_blue = CreatePen(PS_SOLID, 0, RGB(210, 210, 255));
	SelectObject(hdc, pen_blue);


	HBRUSH blue_thin_1, blue_thin_2;
	blue_thin_1 = CreateSolidBrush(RGB(210, 210, 255));
	blue_thin_2 = (HBRUSH)SelectObject(hdc, blue_thin_1);


	/* カーソル */
	//Rectangle(hdc, 20 + (selecting_battle_mainmenu - 1) * 100, 20,
	//	100 + (selecting_battle_mainmenu - 1) * 100, 70);

	/* コマンド */
	SetBkMode(hdc, TRANSPARENT);
	//lstrcpy(p, TEXT("戦う"));
	//	TextOut(hdc, 20, 20, p, lstrlen(p));

	//	lstrcpy(p, TEXT("逃げる"));
	//	TextOut(hdc, 120, 20, p, lstrlen(p));

	//	lstrcpy(p, TEXT("未定1"));
	//	TextOut(hdc, 220, 20, p, lstrlen(p));

	//	lstrcpy(p, TEXT("未定2"));
	//	TextOut(hdc, 320, 20, p, lstrlen(p));



	/* キャラのウィンドウ欄 */
	SelectObject(hdc, blue_thin_1);

	int chara_window_size_x = 140;
	Rectangle(hdc, 10, 410 - chara_window_size_x,
		10 + chara_window_size_x, 410);


	/* キャラのステータス */
	TextOut(hdc, 20, 410 - chara_window_size_x + 10, heros_def_list[0].heros_name, lstrlen(heros_def_list[0].heros_name));

	lstrcpy(mojibuf, TEXT("HP"));
	TextOut(hdc, 20, 410 - chara_window_size_x + 40, mojibuf, lstrlen(mojibuf));

	_stprintf_s(mojibuf, MAX_LENGTH, TEXT("%d"), heros_def_list[0].heros_hp);
	TextOut(hdc, 50, 410 - chara_window_size_x + 40, mojibuf, lstrlen(mojibuf));

	_stprintf_s(mojibuf, MAX_LENGTH, TEXT("/ %d"), heros_def_list[0].heros_hp_max);
	TextOut(hdc, 50 + 30, 410 - chara_window_size_x + 40, mojibuf, lstrlen(mojibuf));

	lstrcpy(mojibuf, TEXT("素早さ"));
	TextOut(hdc, 20 , 410 - chara_window_size_x + 40 + 30, mojibuf, lstrlen(mojibuf));


	_stprintf_s(mojibuf, MAX_LENGTH, TEXT("%d"), sankaAgility[0]);
	TextOut(hdc, 50  + 30, 410 - chara_window_size_x + 40 + 30, mojibuf, lstrlen(mojibuf));



	// _itot_s(your_money , p,200, 10);



	int offsetBattleX = 170;

	Rectangle(hdc, 10 + offsetBattleX, 410 - chara_window_size_x,
		10 + chara_window_size_x + offsetBattleX, 410);

	/* キャラのステータス */
	TextOut(hdc, 20 + offsetBattleX , 410 - chara_window_size_x + 10, heros_def_list[1].heros_name, lstrlen(heros_def_list[1].heros_name));

	lstrcpy(mojibuf, TEXT("HP"));
	TextOut(hdc, 20 + offsetBattleX , 410 - chara_window_size_x + 40, mojibuf, lstrlen(mojibuf));

	_stprintf_s(mojibuf, MAX_LENGTH, TEXT("%d"), heros_def_list[1].heros_hp);
	TextOut(hdc, 50 + offsetBattleX , 410 - chara_window_size_x + 40, mojibuf, lstrlen(mojibuf));

	_stprintf_s(mojibuf, MAX_LENGTH, TEXT("/ %d"), heros_def_list[1].heros_hp_max);
	TextOut(hdc, 50 + 30 + offsetBattleX, 410 - chara_window_size_x + 40, mojibuf, lstrlen(mojibuf));


	lstrcpy(mojibuf, TEXT("素早さ"));
	TextOut(hdc, 20 + offsetBattleX, 410 - chara_window_size_x + 40 +30, mojibuf, lstrlen(mojibuf));


	_stprintf_s(mojibuf, MAX_LENGTH, TEXT("%d"), sankaAgility[1] );
	TextOut(hdc, 50 + offsetBattleX + 30, 410 - chara_window_size_x + 40 +30, mojibuf, lstrlen(mojibuf));



	// 素早さ配列の表示テスト

	lstrcpy(mojibuf, TEXT("素早さ配列"));
	TextOut(hdc, 370, 180, mojibuf, lstrlen(mojibuf));

	_stprintf_s(mojibuf, MAX_LENGTH, TEXT("%d"), iremonoAgilityHairetu[0] );
	TextOut(hdc, 300+100, 180 +40, mojibuf, lstrlen(mojibuf));


	_stprintf_s(mojibuf, MAX_LENGTH, TEXT("%d"), iremonoAgilityHairetu[1]);
	TextOut(hdc, 300 + 100 +30, 180 +40 , mojibuf, lstrlen(mojibuf));


	_stprintf_s(mojibuf, MAX_LENGTH, TEXT("%d"), iremonoAgilityHairetu[2]);
	//	_stprintf_s(mojibuf, MAX_LENGTH, TEXT("%d"), sankaAgilityHairetu[2]);
	TextOut(hdc, 300 + 100 + 30 +30, 180 +40 , mojibuf, lstrlen(mojibuf));




	/* モンスターの表示 */
	// 試作なので名前とHPを表示。本番ではグラフィックに。
	TextOut(hdc, 270, 150, monster_def_list[encount_monters_id - 1].monster_name, lstrlen(monster_def_list[0].monster_name));

	lstrcpy(mojibuf, TEXT("HP"));
	TextOut(hdc, 270, 180, mojibuf, lstrlen(mojibuf));
 	_stprintf_s(mojibuf, MAX_LENGTH, TEXT("%d"), monster_hp);
	TextOut(hdc, 300, 180, mojibuf, lstrlen(mojibuf));


	/* タイマーのテスト */
	_stprintf_s(strCount, MAX_LENGTH, TEXT("%d"), iCount);
	TextOut(hdc, 510, 110, strCount, lstrlen(strCount));

}



void draw_battle_HeroDamage(HDC hdc) {

	/* 文字の透過設定 */
	SetBkMode(hdc, TRANSPARENT);

	int chara_window_size_x = 140;

	/* キャラの負ったダメージ */
	_stprintf_s(mojibuf, MAX_LENGTH, TEXT("ダメージ %d "), damage_EnemyAttack );
	TextOut(hdc, 20, 410 - chara_window_size_x + 10 -30 , mojibuf, lstrlen(mojibuf));

}


void draw_battle_EnemyDamage(HDC hdc) {

	/* 文字の透過設定 */
	SetBkMode(hdc, TRANSPARENT);

	int chara_window_size_x = 140;

	/* 敵に与えたダメージ */
	_stprintf_s(mojibuf, 300, TEXT("ダメージ %d "), damage_HeroAttack);
	TextOut(hdc, 300, 130 , mojibuf, lstrlen(mojibuf));

}

// マップチップのロード用の一時変数を入れる
static TCHAR chip_temp[MAX_LENGTH] = TEXT("GameData\\");
static TCHAR chip_name[MAX_LENGTH] = TEXT("mapchip_grass.bmp");





// このコード モジュールに含まれる関数の宣言を転送します:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: ここにコードを挿入してください。


	// 敵の位置の読み込み
	positionX_enemy[0] = 6;
	positionY_enemy[0] = 5;

	positionX_enemy[1] = 1;
	positionY_enemy[1] = 4;


	/* 乱数の種 */
	srand((unsigned int)time(NULL));



	//薬草の定義
	lstrcpy(item_def_list[0].item_name , TEXT("薬草"));
	item_def_list[0].item_type = 1;
	item_def_list[0].item_id = 1;




	//武器の定義


	weapon_def_list[0].weapon_id = 0;
	lstrcpy(weapon_def_list[0].weapon_name, TEXT("鉄の剣"));
	weapon_def_list[0].material = mateIron;
	weapon_def_list[0].weapon_type = sword;
	weapon_def_list[0].weaponPower = 7; // 攻撃力


	weapon_def_list[1].weapon_id = 1;
	lstrcpy(weapon_def_list[1].weapon_name, TEXT("なし"));
	weapon_def_list[1].material = mateNothing;
	weapon_def_list[1].weapon_type = typeNothing;
	weapon_def_list[1].weaponPower = 0; // 攻撃力


	//所持アイテムの初期値
	
	item_have_list[0].have_item_id = 1;
	item_have_list[0].have_kosuu = 3;




	//初期装備の武器

	int equipInitialWeaponId = 1;
	equipWeaponPower = weapon_def_list[equipInitialWeaponId].weaponPower;


	// map1 にあるマップ遷移の情報
	MapTrans_def_list[0].MapTrans_id = 1; // そのマップ中で何番目か？
	MapTrans_def_list[0].in_Where_Map = 1; // map 何番?
	MapTrans_def_list[0].to_Where_Map = 2;
	MapTrans_def_list[0].positionX = 7;
	MapTrans_def_list[0].positionY = 6;

	MapTrans_def_list[0].chara_positionX = 2;
	MapTrans_def_list[0].chara_positionY = 1;


	// map2 にあるマップ遷移の情報
	MapTrans_def_list[1].MapTrans_id = 1;
	MapTrans_def_list[1].in_Where_Map = 2; // map 何番?
	MapTrans_def_list[1].to_Where_Map = 1;
	MapTrans_def_list[1].positionX = 2;
	MapTrans_def_list[1].positionY = 0;

	MapTrans_def_list[1].chara_positionX = 7;
	MapTrans_def_list[1].chara_positionY = 4;



	// モンスターの定義
	lstrcpy( monster_def_list[0].monster_name, TEXT("スライム"));
	monster_def_list[0].mon_hp_max = 2;
	monster_def_list[0].mon_agility = 13;
	monster_def_list[0].monster_id = 1;
	monster_def_list[0].mon_gold = 1;
	monster_def_list[0].mon_exp = 2;


	lstrcpy(monster_def_list[1].monster_name, TEXT("コボルト"));
	monster_def_list[1].mon_hp_max = 12;
	monster_def_list[1].mon_agility = 36;
	monster_def_list[1].monster_id = 2;
	monster_def_list[1].mon_gold = 10;
	monster_def_list[1].mon_exp = 5;




	lstrcpy(heros_def_list[0].heros_name, TEXT("エロス"));
	heros_def_list[0].heros_hp = 20;
	heros_def_list[0].heros_hp_max = 25;
	heros_def_list[0].heros_agility = 56;

	heros_def_list[0].heros_exp = 0;


	lstrcpy(heros_def_list[1].heros_name, TEXT("ピエ－ル"));
	heros_def_list[1].heros_hp = 18;
	heros_def_list[1].heros_hp_max = 18;
	heros_def_list[1].heros_agility = 100;

	heros_def_list[1].heros_exp = 0;


	int tempHairetu[21] ;

	tempHairetu[0] = 0;
	tempHairetu[1] = -99;
	memcpy(monsterTairetu_def_list[0].monsterTairetu, tempHairetu, sizeof(tempHairetu));

	tempHairetu[0] = 1;
	tempHairetu[1] = -99;
	memcpy(monsterTairetu_def_list[1].monsterTairetu, tempHairetu, sizeof(tempHairetu));



	
	// ゲーム用にロードする画像ファイル
	static TCHAR file_mapchip1[100] = TEXT("GameData\\charachip\\enemy_dot.bmp");
	static TCHAR file_mapchip2[100] = TEXT("GameData\\charachip\\hero_dot.bmp");

	static TCHAR file_mapchip3[100] = TEXT("GameData\\charachip\\hero_dot_down.bmp");
	static TCHAR file_mapchip4[100] = TEXT("GameData\\charachip\\hero_dot_left.bmp");
	static TCHAR file_mapchip5[100] = TEXT("GameData\\charachip\\hero_dot_right.bmp");
	static TCHAR file_mapchip6[100] = TEXT("GameData\\charachip\\hero_dot_up.bmp");

	static TCHAR file_mapchip7[100] = TEXT("GameData\\mapchip\\grass.bmp");
	static TCHAR file_mapchip8[100] = TEXT("GameData\\mapchip\\mapchip_wall.bmp");
	static TCHAR file_mapchip9[100] = TEXT("GameData\\mapchip\\MapTrans_dot.bmp");




	

	for (idTemp = 0; idTemp <= partyNinzu - 1; idTemp = idTemp + 1)
	{
		mikataAgility[idTemp] = heros_def_list[idTemp].heros_agility;
	}






    // グローバル文字列を初期化しています。
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SUJINIKURPGRUNTIMECPPWIN32, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // アプリケーションの初期化を実行します:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SUJINIKURPGRUNTIMECPPWIN32));

    MSG msg;

    // メイン メッセージ ループ:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  関数: MyRegisterClass()
//
//  目的: ウィンドウ クラスを登録します。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SUJINIKURPGRUNTIMECPPWIN32));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SUJINIKURPGRUNTIMECPPWIN32);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   関数: InitInstance(HINSTANCE, int)
//
//   目的: インスタンス ハンドルを保存して、メイン ウィンドウを作成します。
//
//   コメント:
//
//        この関数で、グローバル変数でインスタンス ハンドルを保存し、
//        メイン プログラム ウィンドウを作成および表示します。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // グローバル変数にインスタンス処理を格納します。

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW ^WS_THICKFRAME,
      CW_USEDEFAULT, 0, 640,480 , nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  関数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    メイン ウィンドウのメッセージを処理します。
//
//  WM_COMMAND  - アプリケーション メニューの処理
//  WM_PAINT    - メイン ウィンドウの描画
//  WM_DESTROY  - 中止メッセージを表示して戻る
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 選択されたメニューの解析:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;

	case WM_CREATE:
		SetTimer(hWnd, 2, 500, NULL);

		// マップ描画
		// 初期化用		// この初期化処理を消すとコンパイルエラーになる。消しちゃ駄目
		// マップチップ画像をファイルから読み込む



		lstrcpy(chip_temp, TEXT("GameData\\mapchip\\") );
		lstrcpy(chip_name, TEXT("mapchip_grass.bmp") );
		lstrcat(chip_temp,chip_name ); // 合成

		hbmp_mapchip1 = (HBITMAP)LoadImage(NULL, chip_temp, IMAGE_BITMAP, 0, 0,
			LR_LOADFROMFILE | LR_CREATEDIBSECTION);

		// hbmp_mapchip1 = (HBITMAP)LoadImage(NULL, TEXT("GameData\\mapchip_grass.bmp"), IMAGE_BITMAP, 0, 0,		
		//	LR_LOADFROMFILE | LR_CREATEDIBSECTION);


		lstrcpy(chip_temp, TEXT("GameData\\mapchip\\"));
		lstrcpy(chip_name, TEXT("mapchip_wall.bmp"));
		lstrcat(chip_temp, chip_name); // 合成

		hbmp_mapchip2 = (HBITMAP)LoadImage(NULL, chip_temp, IMAGE_BITMAP, 0, 0,
			LR_LOADFROMFILE | LR_CREATEDIBSECTION);


		// マップ遷移ドットのロード
		lstrcpy(chip_temp, TEXT("GameData\\mapchip\\"));
		lstrcpy(chip_name, TEXT("MapTrans_dot.bmp"));
		lstrcat(chip_temp, chip_name); // 合成
		
		hbmp_MapTrans = (HBITMAP)LoadImage(NULL, chip_temp, IMAGE_BITMAP, 0, 0,
			LR_LOADFROMFILE | LR_CREATEDIBSECTION);


		// 主人公のキャラチップ用ハンドルのロード		
	
		// static struct chara_chip_def chara_chip_list[8];
		lstrcpy(chip_temp, TEXT("GameData\\charachip\\"));
		lstrcpy(chip_name, TEXT("hero_dot_up.bmp"));
		lstrcat(chip_temp, chip_name); // 合成

		chara_chip_list[0].hbmp_chara_chip_up = (HBITMAP)LoadImage(NULL, chip_temp, IMAGE_BITMAP, 0, 0,
			LR_LOADFROMFILE | LR_CREATEDIBSECTION);


		lstrcpy(chip_temp, TEXT("GameData\\charachip\\"));
		lstrcpy(chip_name, TEXT("hero_dot_right.bmp"));
		lstrcat(chip_temp, chip_name); // 合成

		chara_chip_list[0].hbmp_chara_chip_right = (HBITMAP)LoadImage(NULL, chip_temp, IMAGE_BITMAP, 0, 0,
			LR_LOADFROMFILE | LR_CREATEDIBSECTION);


		lstrcpy(chip_temp, TEXT("GameData\\charachip\\"));
		lstrcpy(chip_name, TEXT("hero_dot_down.bmp"));
		lstrcat(chip_temp, chip_name); // 合成

		chara_chip_list[0].hbmp_chara_chip_down = (HBITMAP)LoadImage(NULL, chip_temp, IMAGE_BITMAP, 0, 0,
			LR_LOADFROMFILE | LR_CREATEDIBSECTION);


		lstrcpy(chip_temp, TEXT("GameData\\charachip\\"));
		lstrcpy(chip_name, TEXT("hero_dot_left.bmp"));
		lstrcat(chip_temp, chip_name); // 合成

		chara_chip_list[0].hbmp_chara_chip_left = (HBITMAP)LoadImage(NULL, chip_temp, IMAGE_BITMAP, 0, 0,
			LR_LOADFROMFILE | LR_CREATEDIBSECTION);
			

		lstrcpy(chip_temp, TEXT("GameData\\charachip\\"));
		lstrcpy(chip_name, TEXT("enemy_dot.bmp"));
		lstrcat(chip_temp, chip_name); // 合成

		// 敵のビットマップのロード GameData\\charachip\\enemy_dot.bmp
		//hbmp_enemy = (HBITMAP)LoadImage(NULL, TEXT("GameData\\charachip\\enemy_dot.bmp"), IMAGE_BITMAP, 0, 0,
	//		LR_LOADFROMFILE | LR_CREATEDIBSECTION);

		hbmp_enemy = (HBITMAP)LoadImage(NULL, chip_temp, IMAGE_BITMAP, 0, 0,
			LR_LOADFROMFILE | LR_CREATEDIBSECTION);




		FILE* fp30;
		{
			fp30 = fopen("OPSetting.txt", "r");

			if ( (fp30 = fopen("OPSetting.txt", "r") ) == NULL) {
				fp30 = fopen("OPSetting.txt", "w");
				{
					fprintf(fp30, "ニューゲームかどうか: %d \n", 1); // 設定ファイルの無い場合「はじめから」に設定
				}
			}
		fclose(fp30); // 上記コードはトリッキーな操作をしてるので、安全のために、いったん閉じる。
		}

		FILE* fp50;
		{
			// MessageBox(NULL, TEXT("fp50 を開きたい。"), TEXT("テスト"), MB_OK);

			fp50 = fopen("OPSetting.txt", "r");
			char buffer10[150];

			char str10[150];
			char str20[150];

			int henkan0 = 0;
			
			// MessageBox(NULL, TEXT("fp50 を開いたところ。"), TEXT("テスト"), MB_OK);

			fgets(buffer10, 150, fp50);
			strncpy(str10, strtok(buffer10, ":"), 150);
			strncpy(str20, strtok(NULL, ":"), 150);
			henkan0 = atoi(str20);

			if (henkan0 == 2) {
				selecting_OP = 2;
			}
		}
		fclose(fp50);


		break;	

	case WM_TIMER:

		if (mode_scene == MODE_MAP) {			
			kCount++;				
		}

		if (mode_scene == MODE_BATTLE_NOW) {

			iCount++; // バトル時以外はカウントしない

			// 敵の先攻で、敵がすでに攻撃した場合、
			if (encount_mons_alive == 1
				&& enemy_already_attack_flag == 1
				&& iCount >= 3)
			{

				if (next_attack_var == next_is_hero && hero1_already_attack_flag == 0) {
					//MessageBox(NULL, TEXT("主人公の攻撃"), TEXT("戦闘デバッグ用"), MB_OK);

					whoAction = 0;
					hero1_attack(hWnd);
					hero1_already_attack_flag = 1; // Heroも既に攻撃した

					if (encount_mons_alive == 0) {
						iCount = 0; // 死んでから数秒後に戦勝の報告画面に移らせるので、いったん0にセット

					}

					InvalidateRect(hWnd, NULL, FALSE);
					break;
				}
			}

			// 5秒たったらフラグをリセットする
			if (enemy_already_attack_flag == 1 && hero1_already_attack_flag == 1
				&& iCount >= 5)
			{
				if (next_attack_var == next_is_hero) {
					//MessageBox(NULL, TEXT("主人公の攻撃"), TEXT("戦闘デバッグ用"), MB_OK);

					enemy_already_attack_flag = 0; // フラグのリセット	
					hero1_already_attack_flag = 0;


					//KillTimer(hWnd, 1);					
					battleTimeFlag = 0;
					iCount = 0;

					mode_scene = MODE_BATTLE_COMMAND;

					InvalidateRect(hWnd, NULL, FALSE);
					break;
				}
			}


			// 主人公の先攻で、主人公がすでに攻撃した場合、
			if (encount_mons_alive == 1
				&& hero1_already_attack_flag == 1
				&& iCount >= 3)
			{

				if (next_attack_var == next_is_enemy && enemy_already_attack_flag == 0) {
					//MessageBox(NULL, TEXT("主人公の攻撃"), TEXT("戦闘デバッグ用"), MB_OK);

					whoAction = 1;
					enemy_attack(hWnd);
					enemy_already_attack_flag = 1; // enemyも既に攻撃した

					InvalidateRect(hWnd, NULL, FALSE);
					break;
				}
			}


			if (hero1_already_attack_flag == 1 && enemy_already_attack_flag == 1
				&& iCount >= 5)
			{

				if (next_attack_var == next_is_enemy) {

					//MessageBox(NULL, TEXT("主人公の攻撃"), TEXT("戦闘デバッグ用"), MB_OK);

					enemy_already_attack_flag = 0; // フラグのリセット	
					hero1_already_attack_flag = 0;

					//KillTimer(hWnd, 1);
					
					battleTimeFlag = 0;
					iCount = 0;

					mode_scene = MODE_BATTLE_COMMAND;
					InvalidateRect(hWnd, NULL, FALSE);
					break;
				}
			}

			//////////////////////////////////

			// 敵が全滅している場合
			if (encount_mons_alive == 0 && iCount >= 3) {

				// カネと経験値の更新
				your_money = your_money + monster_def_list[encount_monters_id - 1].mon_gold;
				
				heros_def_list[0].heros_exp = heros_def_list[0].heros_exp + monster_def_list[encount_monters_id - 1].mon_exp;
				heros_def_list[1].heros_exp = heros_def_list[1].heros_exp + monster_def_list[encount_monters_id - 1].mon_exp;


				// モード遷移
				mode_scene = MODE_BATTLE_WIN;


				enemy_already_attack_flag = 0; // フラグのリセット	
				hero1_already_attack_flag = 0;

				//KillTimer(hWnd, 1);
				battleTimeFlag = 0;
				iCount = 0;

				InvalidateRect(hWnd, NULL, FALSE);
				UpdateWindow(hWnd);
				break;
			}

			// なんらかの理由で上記のiCountリセットがされない場合、
			//		安全のため50カウントで強制リセット
			if (iCount == 50) {
				//KillTimer(hWnd, 1);
				battleTimeFlag = 0;
				iCount = 0;
			}

			InvalidateRect(hWnd, NULL, FALSE);
		}

		break;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: HDC を使用する描画コードをここに追加してください...

			if (mode_scene == MODE_OP || mode_scene == MODE_INITIAL) {








				if (mode_scene == MODE_OP) {
					HPEN pen_blue;

					pen_blue = CreatePen(PS_SOLID, 0, RGB(210, 210, 255));
					SelectObject(hdc, pen_blue);

					HBRUSH blue_thin_1, blue_thin_2;
					blue_thin_1 = CreateSolidBrush(RGB(210, 210, 255));
					blue_thin_2 = (HBRUSH)SelectObject(hdc, blue_thin_1);
					Rectangle(hdc, 120, 150, 120 + 200, 150 + 200);


					HBRUSH brasi_pink_1;
					brasi_pink_1 = CreateSolidBrush(RGB(255, 180, 180));
					SelectObject(hdc, brasi_pink_1);

					SetBkMode(hdc, TRANSPARENT);



					Rectangle(hdc, 150, 200 + (selecting_OP - 1) * 50,
						150 + 80, 200 + 20 + (selecting_OP - 1) * 50);
					lstrcpy(mojibuf, TEXT("はじめから"));
					TextOut(hdc, 150, 200, mojibuf, lstrlen(mojibuf));

					lstrcpy(mojibuf, TEXT("つづきから"));
					TextOut(hdc, 150, 250, mojibuf, lstrlen(mojibuf));

					lstrcpy(mojibuf, TEXT("おわる"));
					TextOut(hdc, 150, 300, mojibuf, lstrlen(mojibuf));

				}





				if (mode_scene == MODE_INITIAL) {


					HPEN pen_blue;

					pen_blue = CreatePen(PS_SOLID, 0, RGB(210, 210, 255));
					SelectObject(hdc, pen_blue);

					HBRUSH blue_thin_1, blue_thin_2;
					blue_thin_1 = CreateSolidBrush(RGB(210, 210, 255));
					blue_thin_2 = (HBRUSH)SelectObject(hdc, blue_thin_1);
					


					HBRUSH brasi_pink_1;
					brasi_pink_1 = CreateSolidBrush(RGB(255, 180, 180));
					SelectObject(hdc, brasi_pink_1);

					SetBkMode(hdc, TRANSPARENT);



					// 会話モードの背景マップ (会話中は移動不可能)
					Draw_map(hdc);


					/* 会話ウィンドウの表示 */
					blue_thin_2 = (HBRUSH)SelectObject(hdc, blue_thin_1);
					Rectangle(hdc, 50, 250,
						50 + 500, 400);

					lstrcpy(mojibuf, drawTalkString1);
					TextOut(hdc, 150, 250, mojibuf, lstrlen(mojibuf));

					lstrcpy(mojibuf, drawTalkString2);
					TextOut(hdc, 150, 300, mojibuf, lstrlen(mojibuf));

					lstrcpy(mojibuf, drawTalkString3);
					TextOut(hdc, 150, 350, mojibuf, lstrlen(mojibuf));
				}

				//DeleteObject(pen_blue); //ペンのメモリ解放

				//DeleteObject(blue_thin_1); //ブラシのメモリ解放
				//DeleteObject(blue_thin_2);
				//DeleteObject(brasi_pink_1);

			}


			if (mode_scene == MODE_MAP ) {
				
				//MessageBox(NULL, TEXT("PAINT の if (mode_scene == MODE_MAP )にいる。"), TEXT("キーテスト"), MB_OK);

				// maptableに移動先マップを代入
				int x_map = 0, y_map = 0;

				for (x_map = 0; x_map <= 9; ++x_map)
				{
					for (y_map = 0; y_map <= 6; ++y_map)
					{
						// 代入内容は、移動先マップの代入をすることに注意

						if (where_map == 1) {
							maptable[y_map][x_map] = map1table[y_map][x_map];
						}

						if (where_map == 2) {
							maptable[y_map][x_map] = map2table[y_map][x_map];
						}

					}
				}



				Draw_map(hdc);
			}


			if (mode_scene == MODE_MENU) {

				/* コマンド用ウィンドウ */
				HPEN pen_blue;

				pen_blue = CreatePen(PS_SOLID, 0, RGB(210, 210, 255));
				SelectObject(hdc, pen_blue);


				HBRUSH blue_thin_1, blue_thin_2;
				blue_thin_1 = CreateSolidBrush(RGB(210, 210, 255));
				blue_thin_2 = (HBRUSH)SelectObject(hdc, blue_thin_1);
				Rectangle(hdc, 10, 10, 610, 80);


				HBRUSH brasi_pink_1;
				brasi_pink_1 = CreateSolidBrush(RGB(255, 180, 180));
				SelectObject(hdc, brasi_pink_1);

				Rectangle(hdc, 20 + (selecting_mainmenu - 1) * 100, 20,
					100 + (selecting_mainmenu - 1) * 100, 70);

				SetBkMode(hdc, TRANSPARENT);
				lstrcpy(mojibuf, TEXT("道具"));
				TextOut(hdc, 20, 20, mojibuf, lstrlen(mojibuf));

				lstrcpy(mojibuf, TEXT("装備"));
				TextOut(hdc, 120, 20, mojibuf, lstrlen(mojibuf));

				lstrcpy(mojibuf, TEXT("技能"));
				TextOut(hdc, 220, 20, mojibuf, lstrlen(mojibuf));

				lstrcpy(mojibuf, TEXT("セーブ"));
				TextOut(hdc, 320, 20, mojibuf, lstrlen(mojibuf));


				/* 所持金の表示欄 */
				SelectObject(hdc, blue_thin_1);

				Rectangle(hdc, 500, 250,
					600, 350);

				lstrcpy(mojibuf, TEXT("所持金"));
				TextOut(hdc, 510, 260, mojibuf, lstrlen(mojibuf));

				_stprintf_s(mojibuf, MAX_LENGTH, TEXT("%d"), your_money);
				TextOut(hdc, 510, 300, mojibuf, lstrlen(mojibuf));

				// _itot_s(your_money , p,200, 10);


				/* キャラのステータス欄 */
				Rectangle(hdc, 10, 100,
					300, 200);

				_stprintf_s(mojibuf, MAX_LENGTH, TEXT("%s"), heros_def_list[0].heros_name);
				TextOut(hdc, 130, 105, mojibuf, lstrlen(mojibuf));


				lstrcpy(mojibuf, TEXT("HP"));
				TextOut(hdc, 130, 130, mojibuf, lstrlen(mojibuf));

				_stprintf_s(mojibuf, MAX_LENGTH, TEXT("%d"), heros_def_list[0].heros_hp);
				TextOut(hdc, 160, 130, mojibuf, lstrlen(mojibuf));

				_stprintf_s(mojibuf, MAX_LENGTH, TEXT("/ %d"), heros_def_list[0].heros_hp_max);
				TextOut(hdc, 190, 130, mojibuf, lstrlen(mojibuf));


				int offset = 120;

				Rectangle(hdc, 10, 100 + offset,
					300, 200 + offset);

				_stprintf_s(mojibuf, MAX_LENGTH, TEXT("%s"), heros_def_list[1].heros_name);
				TextOut(hdc, 130, 105 + offset, mojibuf, lstrlen(mojibuf));


				lstrcpy(mojibuf, TEXT("HP"));
				TextOut(hdc, 130, 130 + offset, mojibuf, lstrlen(mojibuf));

				_stprintf_s(mojibuf, MAX_LENGTH, TEXT("%d"), heros_def_list[1].heros_hp);
				TextOut(hdc, 160, 130 + offset, mojibuf, lstrlen(mojibuf));

				_stprintf_s(mojibuf, MAX_LENGTH, TEXT("/ %d"), heros_def_list[1].heros_hp_max);
				TextOut(hdc, 190, 130 + offset, mojibuf, lstrlen(mojibuf));




			}


			if (mode_scene == MODE_ITEM_MENU) {




				/* アイテムの表示欄 */
				/* コマンド用ウィンドウ */
				HPEN pen_blue;

				pen_blue = CreatePen(PS_SOLID, 0, RGB(210, 210, 255));
				SelectObject(hdc, pen_blue);


				HBRUSH blue_thin_1, blue_thin_2;
				blue_thin_1 = CreateSolidBrush(RGB(210, 210, 255));
				blue_thin_2 = (HBRUSH)SelectObject(hdc, blue_thin_1);
		
				
				Rectangle(hdc, 10, 100,
					600, 400);


				HBRUSH brasi_pink_1;
				brasi_pink_1 = CreateSolidBrush(RGB(255, 180, 180));
				SelectObject(hdc, brasi_pink_1);

				Rectangle(hdc, 20 + (selecting_item_x - 1) * 300, 110 + (selecting_item_y - 1) * 50 ,
					250 + (selecting_item_x - 1) * 300, 150 + (selecting_item_y - 1) * 50);

				

			//	_stprintf_s(p, MAX_LENGTH, TEXT("%s"), heros_def_list[0].heros_name);
			//	TextOut(hdc, 130, 105, p, lstrlen(p));

				SetBkMode(hdc, TRANSPARENT);
				lstrcpy(mojibuf, item_def_list[ (item_have_list[0].have_item_id -1) ].item_name);
				TextOut(hdc, 30, 130, mojibuf, lstrlen(mojibuf));

				_stprintf_s(mojibuf, MAX_LENGTH, TEXT("x %d"), item_have_list[0].have_kosuu );
				TextOut(hdc, 160, 130, mojibuf, lstrlen(mojibuf));

			//	_stprintf_s(pmojibuf, 200, TEXT("/ %d"), heros_def_list[0].heros_hp_max);
				//TextOut(hdc, 190, 130, mojibuf, lstrlen(pmojibuf));



				// デバッグ用
				lstrcpy(mojibuf, TEXT("sele_item :"));
				TextOut(hdc, 430, 200, mojibuf, lstrlen(mojibuf));

				_stprintf_s(mojibuf, MAX_LENGTH, TEXT("%d"), selecting_item);
				TextOut(hdc, 530, 200, mojibuf, lstrlen(mojibuf));


				lstrcpy(mojibuf, TEXT("item_x :"));
				TextOut(hdc, 430, 230, mojibuf, lstrlen(mojibuf));

				_stprintf_s(mojibuf, MAX_LENGTH, TEXT("%d"), selecting_item_x);
				TextOut(hdc, 490, 230, mojibuf, lstrlen(mojibuf));


				lstrcpy(mojibuf, TEXT("item_y :"));
				TextOut(hdc, 430, 280, mojibuf, lstrlen(mojibuf));

				_stprintf_s(mojibuf, MAX_LENGTH, TEXT("%d"), selecting_item_y);
				TextOut(hdc, 490, 280, mojibuf, lstrlen(mojibuf));




				// アイテム効果の確認用


				_stprintf_s(mojibuf, MAX_LENGTH, TEXT("%s"), heros_def_list[0].heros_name);
				TextOut(hdc, 400, 300, mojibuf, lstrlen(mojibuf));


				lstrcpy(mojibuf, TEXT("HP"));
				TextOut(hdc, 400, 300 + 30, mojibuf, lstrlen(mojibuf));

				_stprintf_s(mojibuf, MAX_LENGTH, TEXT("%d"), heros_def_list[0].heros_hp);
				TextOut(hdc, 400 + 30, 300 + 30, mojibuf, lstrlen(mojibuf));

				_stprintf_s(mojibuf, MAX_LENGTH, TEXT("/ %d"), heros_def_list[0].heros_hp_max);
				TextOut(hdc, 400 + 60, 300 + 30, mojibuf, lstrlen(mojibuf));



			}

			////

			if (mode_scene == MODE_SAVE_MENU) {

				/* セーブの表示欄 */

				lstrcpy(mojibuf, TEXT("セーブしました。"));
				TextOut(hdc, 280, 250, mojibuf, lstrlen(mojibuf));

			}






			if (mode_scene == MODE_BATTLE_COMMAND ) {

				draw_battle_common_before(hdc);


				/* コマンドウィンドウ */
				HPEN pen_blue;

				pen_blue = CreatePen(PS_SOLID, 0, RGB(210, 210, 255));
				SelectObject(hdc, pen_blue);


				HBRUSH blue_thin_1, blue_thin_2;
				blue_thin_1 = CreateSolidBrush(RGB(210, 210, 255));
				blue_thin_2 = (HBRUSH)SelectObject(hdc, blue_thin_1);
				Rectangle(hdc, 10, 10, 610, 80);


				HBRUSH brasi_pink_1;
				brasi_pink_1 = CreateSolidBrush(RGB(255, 180, 180));
				SelectObject(hdc, brasi_pink_1);



				/* カーソル */
				Rectangle(hdc, 20 + (selecting_battle_mainmenu - 1) * 100, 20,
					100 + (selecting_battle_mainmenu - 1) * 100, 70);

				/* コマンド */
				SetBkMode(hdc, TRANSPARENT);
				lstrcpy(mojibuf, TEXT("戦う"));
				TextOut(hdc, 20, 20, mojibuf, lstrlen(mojibuf));

				lstrcpy(mojibuf, TEXT("逃げる"));
				TextOut(hdc, 120, 20, mojibuf, lstrlen(mojibuf));

				lstrcpy(mojibuf, TEXT("未定1"));
				TextOut(hdc, 220, 20, mojibuf, lstrlen(mojibuf));

				lstrcpy(mojibuf, TEXT("未定2"));
				TextOut(hdc, 320, 20, mojibuf, lstrlen(mojibuf));



				draw_battle_common_after(hdc); // ステータス


			}



			if (mode_scene == MODE_BATTLE_NOW) {



				draw_battle_common_before(hdc); // 画面全体の背景色など

				if (whoAction == 0) {
					_stprintf_s(mojibuf, MAX_LENGTH, TEXT("主人公の攻撃！ "));
					TextOut(hdc, 50, 410 - 230, mojibuf, lstrlen(mojibuf));
				}

				if (whoAction == 1) {
					_stprintf_s(mojibuf, MAX_LENGTH, TEXT("敵の攻撃！ "));
					TextOut(hdc, 50, 410 - 230, mojibuf, lstrlen(mojibuf));
				}

			

				//draw_battle_common_before(hdc);
				draw_battle_common_after(hdc);

				// ここにダメージ表記の関数を追加。
				if (enemy_already_attack_flag == 1) {
					draw_battle_HeroDamage(hdc);

					draw_battle_common_after(hdc);
					// _stprintf_s(mojibuf, MAX_LENGTH, TEXT("敵はすでに攻撃しました "));
					// TextOut(hdc, 50, 410 - 230, mojibuf, lstrlen(mojibuf));

				}

				if (hero1_already_attack_flag == 1) {
					draw_battle_EnemyDamage(hdc);

					draw_battle_common_after(hdc);
					// _stprintf_s(mojibuf, MAX_LENGTH, TEXT("主人公はすでに攻撃しました "));
					// TextOut(hdc, 50, 410 - 230, mojibuf, lstrlen(mojibuf));


				}


			}



			if (mode_scene == MODE_BATTLE_WIN) {

				draw_battle_common_before(hdc);
				draw_battle_common_after(hdc);

				// ここから先、戦勝の報告メッセージ

				/* ペンとブラシ設定 */
				HPEN pen_blue;

				pen_blue = CreatePen(PS_SOLID, 0, RGB(210, 210, 255));
				SelectObject(hdc, pen_blue);


				HBRUSH blue_thin_1, blue_thin_2;
				blue_thin_1 = CreateSolidBrush(RGB(210, 210, 255));
				blue_thin_2 = (HBRUSH)SelectObject(hdc, blue_thin_1);


				/* 描写: 戦闘終了ウィンドウ背景 */
				Rectangle(hdc, 200, 100, 450, 200);

				SetBkMode(hdc, TRANSPARENT);

				/* 戦闘終了メッセージ */
				lstrcpy(mojibuf, TEXT("戦闘に勝利した。"));
				TextOut(hdc, 270, 110, mojibuf, lstrlen(mojibuf));

				lstrcpy(mojibuf, TEXT("経験値"));
				TextOut(hdc, 210, 110 + 30, mojibuf, lstrlen(mojibuf));

				_stprintf_s(mojibuf, MAX_LENGTH, TEXT("%d  EXP"), monster_def_list[encount_monters_id -1].mon_exp);
				TextOut(hdc, 300, 110 + 30, mojibuf, lstrlen(mojibuf));


				lstrcpy(mojibuf, TEXT("金"));
				TextOut(hdc, 210, 110 + 30*2 , mojibuf, lstrlen(mojibuf));

				_stprintf_s(mojibuf, MAX_LENGTH, TEXT("%d  G"), monster_def_list[encount_monters_id -1].mon_gold);
				TextOut(hdc, 300, 110 + 30 * 2 , mojibuf, lstrlen(mojibuf));
				
			}




            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
	case WM_KEYDOWN:

		if (mode_scene == MODE_OP){
			switch (wParam)
			{
			case 'Z':
				//MessageBox(NULL, TEXT("Zが押されました。"), TEXT("キーテスト"), MB_OK);

				{

					if (selecting_OP == 1)
					{
						//MessageBox(NULL, TEXT("ゲームを開始。"), TEXT("キーテスト"), MB_OK);
						mode_scene = MODE_INITIAL;
					}

					if (selecting_OP == 2)
					{
						// MessageBox(NULL, TEXT("ゲームをロード。"), TEXT("キーテスト"), MB_OK);
						

						FILE* fp1;
						{
							fp1 = fopen("savedata1.txt", "r");

							if ((fp1 = fopen("savedata1.txt", "r")) == NULL) {
								MessageBox(NULL, TEXT("セーブデータがありません。"), TEXT("メッセージ"), MB_OK);
								selecting_OP = 1;
								IsNewGame = 1;

								FILE* fp2;
								fp2 = fopen("OPSetting.txt", "w");
								{
									fprintf(fp2, "ニューゲームかどうか: %d \n", 1); // 設定ファイルの無い場合「はじめから」に設定
								}
								fclose(fp2);

							}
							else {
								// MessageBox(NULL, TEXT("ロードしています。"), TEXT("テスト"), MB_OK);
								mode_scene = MODE_LOAD;

								char buffer1[150];

								char str1[150];
								char str2[150];

								int henkan = 0;

								for (int j = 1; j <= 9; ++j) {

									fgets(buffer1, 150, fp1);
									strncpy(str1, strtok(buffer1, ":"), 150);
									strncpy(str2, strtok(NULL, ":"), 150);
									henkan = atoi(str2);

									if (j == 1) { where_map = henkan; }
									if (j == 2) { chara_x = henkan; }
									if (j == 3) { chara_y = henkan; }
									if (j == 4) { heros_def_list[0].heros_hp = henkan; }
									if (j == 5) { heros_def_list[0].heros_hp_max = henkan; }
									if (j == 6) { your_money = henkan; }
									if (j == 7) { item_have_list[0].have_kosuu = henkan; }
									if (j == 7) { break; }

								}
								fclose(fp1);

								MapTrans_position_x = MapTrans_def_list[where_map - 1].positionX;
								MapTrans_position_y = MapTrans_def_list[where_map - 1].positionY;

								mode_scene = MODE_MAP;

							}
						}
						
						InvalidateRect(hWnd, NULL, FALSE);
						UpdateWindow(hWnd);
						break;
					}

					if (selecting_OP == 3)
					{
						PostQuitMessage(0);
					}
				}

				break;
			case VK_UP:
				{
					selecting_OP = selecting_OP - 1;

					// 初期化。 selecting_menu は 1 以上から 3 以下にする。
					if (selecting_OP > 3) {
						selecting_OP = 3;
					}
					else if (selecting_OP < 1) {
						selecting_OP = 1;
					}

					InvalidateRect(hWnd, NULL, FALSE);
					UpdateWindow(hWnd);
					break;
				}

			case VK_DOWN:
				{
					// MessageBox(NULL, TEXT("↓が押されました。"), TEXT("キーテスト"), MB_OK);
					selecting_OP = selecting_OP + 1;

					// 初期化。 selecting_menu は 1 以上から 3 以下にする。

					if (selecting_OP > 3) {
						selecting_OP = 3;
					}
					else if (selecting_OP < 1) {
						selecting_OP = 1;
					}

					InvalidateRect(hWnd, NULL, FALSE);
					UpdateWindow(hWnd);
					break;
				}

			}

		}

		if (mode_scene == MODE_INITIAL) {
			switch (wParam)
			{
			case 'Z':
				{
					if (game_event1_end != 1) {

						chara_x = start_x;
						chara_y = start_y;


						_stprintf_s(mojibuf, TEXT("%s %s"), TEXT("俺の名は"), heros_def_list[0].heros_name);

						lstrcpy(drawTalkString1, mojibuf);
						lstrcpy(drawTalkString2, TEXT("伝説のラブホテル『エデン』を探す旅に出かける。"));
						lstrcpy(drawTalkString3, TEXT("（決定ボタンを押してください）"));

						InvalidateRect(hWnd, NULL, FALSE);
						UpdateWindow(hWnd);

						game_event1_end = 1;

						key_remain = 0;
					}

					if (game_event1_end == 1 && game_event2_end == 0 && key_remain > 0) {


						lstrcpy(drawTalkString1, TEXT("あなたは金100を手に入れた。"));
						lstrcpy(drawTalkString2, TEXT("（決定ボタンを押してください）"));
						lstrcpy(drawTalkString3, TEXT(""));

						InvalidateRect(hWnd, NULL, FALSE);
						UpdateWindow(hWnd);

						your_money = your_money + 100;
						game_event2_end = 1;

						key_remain = 0;

						//ここでモード遷移したら駄目。遷移すると、「金100を手に入れた」の行が表示されなくなってしまう！
					}


					if (game_event2_end == 1 && key_remain > 0) { // event2の終了作業。
						key_remain = 0;
						mode_scene = MODE_MAP; //ここでモード遷移すると、「金100を手に入れた」の行が表示されなくなってしまう！

						InvalidateRect(hWnd, NULL, FALSE);
						UpdateWindow(hWnd);
					}

				}
				
			 break;


			}

		}



		if (mode_scene == MODE_MAP && key_remain >0 && kCount >0 ) { ///////
			switch (wParam)
			{	
				// メニュー画面に遷移
			case 'X':

				{
					key_remain = 0;

					mode_scene = MODE_MENU;
					InvalidateRect(hWnd, NULL, TRUE);
					UpdateWindow(hWnd);
				}
				break;

			case VK_UP:
				{
					hero1_direction = upward;
					desti_y = chara_y - 1;

					//	移動可否の判定と、移動先座標の更新
					check_movable(hWnd);

					check_encount_enemy(hWnd);					
					check_MapTransition(hWnd);

					break;
				}


			case VK_DOWN:
				{
					hero1_direction = downward;
					desti_y = chara_y + 1;

					//	移動可否の判定と、移動先座標の更新
					check_movable(hWnd);

					check_encount_enemy(hWnd);					
					check_MapTransition(hWnd);

					break;
				}


			case VK_RIGHT:
				{
					hero1_direction = rightward;
					desti_x = chara_x + 1;

					//	移動可否の判定と、移動先座標の更新
					check_movable(hWnd);

					check_encount_enemy(hWnd);					
					check_MapTransition(hWnd);

					break;
				}


			case VK_LEFT:
				{
					hero1_direction = leftward;
					desti_x = chara_x - 1;

					//	移動可否の判定と、移動先座標の更新
					check_movable(hWnd);

					check_encount_enemy(hWnd);					
					check_MapTransition(hWnd);

					break;
				}

				break;
			}
		}


		if (mode_scene == MODE_MENU && key_remain > 0) {
			switch (wParam)
			{
			case 'Z':
				{
					key_remain = 0;

					if (selecting_mainmenu == 1) {
						//	MessageBox(NULL, TEXT("道具を選択しました。"), TEXT("キーテスト"), MB_OK);

						mode_scene = MODE_ITEM_MENU;

						InvalidateRect(hWnd, NULL, FALSE);
						UpdateWindow(hWnd);
					}

					//mode_scene = MODE_MAP; 


					if (selecting_mainmenu == 2) {
						MessageBox(NULL, TEXT("装備をするためのコマンド（※未実装）。"), TEXT("テスト"), MB_OK);

						//mode_scene = MODE_ITEM_MENU;

						InvalidateRect(hWnd, NULL, FALSE);
						UpdateWindow(hWnd);
					}



					if (selecting_mainmenu == 3) {
						MessageBox(NULL, TEXT("特技を使うためのコマンド（※未実装）。"), TEXT("テスト"), MB_OK);

						//mode_scene = MODE_ITEM_MENU;

						InvalidateRect(hWnd, NULL, FALSE);
						UpdateWindow(hWnd);
					}


					if (selecting_mainmenu == 4) {
						
						mode_scene = MODE_SAVE_MENU;

						InvalidateRect(hWnd, NULL, FALSE);
						UpdateWindow(hWnd);


						// OP画面の設定のセーブ
						FILE* fp1;
						
						fp1 = fopen("OPSetting.txt", "w");
						{
							fprintf(fp1, "ニューゲームかどうか: %d \n", 2); // 2なら「つづきから」に設定
						}
						fclose(fp1);



						// ゲーム内データのセーブ
						FILE* fp2;
						{
							fp2 = fopen("savedata1.txt", "w");
							if ((fp2 = fopen("savedata1.txt", "w")) == NULL) {
								MessageBox(NULL, TEXT("セーブが失敗しました。対象ファイルが存在しないようです。"), TEXT("メッセージ"), MB_OK);
								break;
							}
							else {
								fprintf(fp2, "現在マップ番号: %d \n", where_map);
								fprintf(fp2, "キャラ位置x座標: %d \n", chara_x);
								fprintf(fp2, "キャラ位置y座標: %d \n", chara_y);

								fprintf(fp2, "キャラの現HP: %d \n", heros_def_list[0].heros_hp);
								fprintf(fp2, "キャラの最大HP: %d \n", heros_def_list[0].heros_hp_max);

								fprintf(fp2, "所持金: %d \n", your_money);

								char aaa[100];
								WideCharToMultiByte(CP_ACP, 0, item_def_list[0].item_name, -1, aaa, sizeof(aaa), NULL, NULL);

								fprintf(fp2, "%s の個数: %d \n", aaa, item_have_list[0].have_kosuu);

								fclose(fp2);
							}
						}

						mode_scene = MODE_MENU; // セーブ終了後にメニュー画面に復帰。忘れないように。
			
						InvalidateRect(hWnd, NULL, FALSE);
						UpdateWindow(hWnd);					
					}
				}

				break;

			case 'X':
				// マップ画面に戻る
				{
					key_remain = 0;

					mode_scene = MODE_MAP;
					InvalidateRect(hWnd, NULL, TRUE);
					UpdateWindow(hWnd);
				}
				break;

			case VK_RIGHT:
				{
					//MessageBox(NULL, TEXT("rightが押されました。"), TEXT("キーテスト"), MB_OK);

					selecting_mainmenu = selecting_mainmenu + 1;


					if (selecting_mainmenu > 6) {
						selecting_mainmenu = 6;
					}

					if (selecting_mainmenu < 1) {
						selecting_mainmenu = 1;
					}

					InvalidateRect(hWnd, NULL, FALSE);
					UpdateWindow(hWnd);
				}

				break;


			case VK_LEFT:

				{
					selecting_mainmenu = selecting_mainmenu - 1;

					if (selecting_mainmenu > 6) {
						selecting_OP = 6;
					}

					if (selecting_mainmenu < 1) {
						selecting_mainmenu = 1;
					}

					InvalidateRect(hWnd, NULL, FALSE);
					UpdateWindow(hWnd);
				}

				break;
			}

		}


		if (mode_scene == MODE_ITEM_MENU && key_remain > 0) {
			switch (wParam)
			{
			case 'Z':
				{
				key_remain = 0;
					if (selecting_item == 1) {

						if (heros_def_list[0].heros_hp < heros_def_list[0].heros_hp_max) {
							if (item_have_list[0].have_kosuu > 0) {
								heros_def_list[0].heros_hp = heros_def_list[0].heros_hp + 5;

								if (heros_def_list[0].heros_hp > heros_def_list[0].heros_hp_max) {
									heros_def_list[0].heros_hp = heros_def_list[0].heros_hp_max;
								}

								item_have_list[0].have_kosuu = item_have_list[0].have_kosuu - 1;

							}

							InvalidateRect(hWnd, NULL, FALSE);
							UpdateWindow(hWnd);
						}
					}

				}

				break;

			case 'X':
				// メニュー画面に戻る
				{
					mode_scene = MODE_MENU;
					InvalidateRect(hWnd, NULL, TRUE);
					UpdateWindow(hWnd);
				}
				break;

			case VK_UP:
				{
					selecting_item = selecting_item - 2;

					item_select(hWnd);
				}

				break;

			case VK_DOWN:			
				{
					selecting_item = selecting_item + 2;

					item_select(hWnd);

				}
				break;

			case VK_RIGHT:
				{
					selecting_item = selecting_item + 1;

					item_select(hWnd);
				}

				break;

			case VK_LEFT:
				{
					selecting_item = selecting_item - 1;

					item_select(hWnd);
				}
				break;

			}
		}


		if (mode_scene == MODE_BATTLE_COMMAND && key_remain > 0) {
			switch (wParam)
			{
			case 'Z':
				key_remain = 0;
				battle_key = 1;
				 {
					if (selecting_battle_mainmenu == 1)	{
						// SetTimer(hWnd, 1, 1000, NULL);
						battleTimeFlag = 1;




						for(idTemp = 0; idTemp <= partyNinzu -1; idTemp = idTemp + 1)
						{
							sankaAgility[idTemp] = mikataAgility[idTemp];
						}

						for(idTemp = 0; idTemp <= enemyNinzu-1; idTemp = idTemp + 1)
						{
							sankaAgility[partyNinzu + idTemp] = monster_def_list[idTemp].mon_agility;
						}

						for (int loctempQ = 0; loctempQ <= partyNinzu + enemyNinzu -1; ++loctempQ)
						{								
							iremonoAgilityHairetu[loctempQ] = sankaAgility[loctempQ];

							iremonoOrderHairetu[loctempQ] = loctempQ;
						} // 初期値の代入



						// ソートで実装しよう



						for (int loctempB = 0; loctempB <= partyNinzu - 1 + enemyNinzu; ++loctempB)
						{
							for (int loctempA = loctempB; loctempA <= partyNinzu - 1 + enemyNinzu; ++loctempA)
							{
								if (iremonoAgilityHairetu[loctempB] >= iremonoAgilityHairetu[loctempA + loctempB]) {

									// 何もしていない
								//	iremonoAgilityHairetu[loctempB] = iremonoAgilityHairetu[loctempB];
								//	iremonoAgilityHairetu[loctempA + loctempB] = iremonoAgilityHairetu[loctempA + loctempB];

								}
								if (iremonoAgilityHairetu[loctempB] < iremonoAgilityHairetu[loctempA + loctempB]) {

									int tempSwapA, tempSwapB;

									tempSwapA = iremonoAgilityHairetu[loctempB];
									tempSwapB = iremonoAgilityHairetu[loctempA + loctempB];

									iremonoAgilityHairetu[loctempB] = tempSwapB;
									iremonoAgilityHairetu[loctempA + loctempB] = tempSwapA;


									int tempOrederSwapA, tempOrederSwapB;

									tempOrederSwapA = iremonoOrderHairetu[loctempB];
									tempOrederSwapB = iremonoOrderHairetu[loctempA + loctempB];

									iremonoOrderHairetu[loctempB] = tempOrederSwapB;
									iremonoOrderHairetu[loctempA + loctempB] = tempOrederSwapA;
								}
							}

						}

						InvalidateRect(hWnd, NULL, FALSE);
						UpdateWindow(hWnd);



						for (int loctempA = 0; loctempA <= partyNinzu - 1 + enemyNinzu; ++loctempA)
						{
							actuinOrder[loctempA] = iremonoOrderHairetu[loctempA];

//							 = iremonoAgilityHairetu[loctempA] ;

						}
	


						// 主人公の素早さのほうが早い場合
						if (heros_def_list[0].heros_agility >= monster_def_list[encount_monters_id -1].mon_agility) {

							//MessageBox(NULL, TEXT("先攻にいる。"), TEXT("戦闘デバッグ用"), MB_OK);


							if (hero1_already_attack_flag == 0 ) {
								//MessageBox(NULL, TEXT("主人公の攻撃"), TEXT("戦闘デバッグ用"), MB_OK);

								whoAction = 0;
								hero1_attack(hWnd);	
								hero1_already_attack_flag = 1;	

								
								next_attack_var = next_is_enemy;
							}

							InvalidateRect(hWnd, NULL, FALSE);
							UpdateWindow(hWnd);

							mode_scene = MODE_BATTLE_NOW;

						}

						// 敵の素早さのほうが早い場合
						if (heros_def_list[0].heros_agility < monster_def_list[encount_monters_id -1].mon_agility) {

							//MessageBox(NULL, TEXT("後攻にいる。"), TEXT("戦闘デバッグ用"), MB_OK);


							if (encount_mons_alive == 1) {
								// MessageBox(NULL, TEXT("敵の攻撃"), TEXT("戦闘デバッグ用"), MB_OK);

								whoAction = 1;
								enemy_attack(hWnd);
								enemy_already_attack_flag = 1;

								
								next_attack_var = next_is_hero;
								// MessageBox(NULL, TEXT("フラグを next_is_hero にした."), TEXT("戦闘デバッグ用"), MB_OK);

							}

							InvalidateRect(hWnd, NULL, FALSE);
							UpdateWindow(hWnd);

							mode_scene = MODE_BATTLE_NOW;

						}

						InvalidateRect(hWnd, NULL, FALSE);
						UpdateWindow(hWnd);

					}

					if (selecting_battle_mainmenu == 2)
					{
						MessageBox(NULL, TEXT("逃亡（開発中）"), TEXT("戦闘テスト"), MB_OK);
					}

					if (selecting_battle_mainmenu == 3)
					{
						// ;
					}
				}
				break;

			case VK_RIGHT:
				{
					// MessageBox(NULL, TEXT("rightが押されました。"), TEXT("キーテスト"), MB_OK);

					selecting_battle_mainmenu = selecting_battle_mainmenu + 1;


					if (selecting_battle_mainmenu > 6) {
						selecting_battle_mainmenu = 6;
					}

					if (selecting_battle_mainmenu < 1) {
						selecting_battle_mainmenu = 1;
					}

					InvalidateRect(hWnd, NULL, FALSE);
					UpdateWindow(hWnd);
				}

				break;

			case VK_LEFT:
				{
					// MessageBox(NULL, TEXT("rightが押されました。"), TEXT("キーテスト"), MB_OK);

					selecting_battle_mainmenu = selecting_battle_mainmenu - 1;


					if (selecting_battle_mainmenu > 6) {
						selecting_battle_mainmenu = 6;
					}

					if (selecting_battle_mainmenu < 1) {
						selecting_battle_mainmenu = 1;
					}

					InvalidateRect(hWnd, NULL, FALSE);
					UpdateWindow(hWnd);
				}
				break;
			}
		}


		if (mode_scene == MODE_BATTLE_WIN && key_remain > 0) {
			key_remain = 0;

			InvalidateRect(hWnd, NULL, FALSE);
			UpdateWindow(hWnd);

			switch (wParam)
			{
			case 'Z':
				mode_scene = MODE_MAP;

				InvalidateRect(hWnd, NULL, FALSE);
				UpdateWindow(hWnd);
				break;
			}
		}


		key_remain = 1;

		return 0;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}


// バージョン情報ボックスのメッセージ ハンドラーです。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
