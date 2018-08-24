// sujinikuRpgRuntimeCppWin32.cpp: アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "sujinikuRpgRuntimeCppWin32.h"

#include <stdlib.h>
#include <time.h>

#define MAX_LOADSTRING 100
#define MAX_LENGTH 200 // 要素数に注意 

#define MODE_OP 100 // オープニング画面のモード番号
#define MODE_INITIAL 200 // ゲーム開始イベントのモード番号
#define MODE_MAP 300 // マップ画面のモード番号

#define MODE_MENU 400 // メニュー画面のモード番号
#define MODE_ITEM_MENU 410 // アイテムメニューのモード



#define MODE_BATTLE 500 // 戦闘画面のモード番号
#define MODE_BATTLE_WIN 580 // 戦闘勝利のモード番号


// グローバル変数:

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


struct monster_def
{
	int monster_id;
	TCHAR monster_name[MAX_LENGTH];
	int mon_hp_max;
	int mon_agility;

	int mon_gold;
	int mon_exp;
};

static int encount_monters_id = 1;

static int selecting_item = 1 ;
static int selecting_item_x = 1;
static int selecting_item_y = 1;

// 戦闘中に使用する変数。 モンスター定義とは別物。
static TCHAR monster_name[30];
static int monster_hp = 10;


static int enemy1_alive = 1; // 1なら生きてる。0なら死亡。
static int enemy2_alive = 1;

static int encount_mons_alive = 1;


HINSTANCE hInst;                                // 現在のインターフェイス
WCHAR szTitle[MAX_LOADSTRING];                  // タイトル バーのテキスト
WCHAR szWindowClass[MAX_LOADSTRING];            // メイン ウィンドウ クラス名


TCHAR p[MAX_LENGTH] = TEXT("はじめから");

static TCHAR hero_name[30] = TEXT("ゴンザレス"); // これは static にしないと、カーソル移動時に消えてしまう

static int mode_scene = 100 ;
static int selecting_OP = 1 ;

static int selecting_mainmenu = 1;


static int cursol_stop;
static int your_money =0;

static int hero_hp = 20;
static int hero_hp_max = 25;
static int hero_agility = 5;

static int hero_exp = 0;


static TCHAR drawTalkString1[MAX_LENGTH] = TEXT("temp1"); // 会話欄の1行目
static TCHAR drawTalkString2[MAX_LENGTH] = TEXT("temp2"); // 会話欄の2行目
static TCHAR drawTalkString3[MAX_LENGTH] = TEXT("temp3"); // 会話欄の3行目


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
static int desti_x; // 進行先の壁判定のためのx座標変数 "desiti" means the destination.
static int desti_y; // 進行先の壁判定のためのx座標変数


int maptable[7][10] = {
	{ 1,1,0,1,1,1,1,1,1,1 }, //0 x
	{ 1,0,0,0,0,0,0,0,0,1 }, //1
	{ 1,0,0,0,0,0,0,0,0,1 }, //2
	{ 1,0,0,0,0,0,0,0,0,1 }, //3
	{ 1,0,0,0,0,0,0,0,0,1 }, //4
	{ 1,0,0,0,0,0,0,0,0,1 }, //5
	{ 1,1,1,1,1,1,0,0,1,1 }  //6
};

int enemy1_position_x = 6;
int enemy1_position_y = 5;


int enemy2_position_x = 1;
int enemy2_position_y = 4;


enum direction { upward, rightward, downward , leftward };
enum direction hero1_direction = downward;

static int selecting_battle_mainmenu = 1;


// マップ画面の描画用の関数
void Draw_map(HDC hdc) {
	static HDC hbackDC; // 裏画面用のハンドル
	hbackDC = CreateCompatibleDC(hdc);

	static HDC hMdc; // 中間作業用ハンドル
	HBITMAP hbmp;

	// マップサイズの読み込む
	hbmp = CreateCompatibleBitmap(hdc, 640+100, 480+100);
	SelectObject(hbackDC, hbmp); // hbackDCにマップサイズを読み込ませている

	DeleteObject(hbmp);

	// マップチップ画像をファイルから読み込む
	hbmp = (HBITMAP)LoadImage(NULL, TEXT("mapchip_grass.bmp"), IMAGE_BITMAP, 0, 0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION);

	hMdc = CreateCompatibleDC(hdc);
	DeleteObject(hbmp);

	int x_map = 0;
	int y_map = 0;

	for (x_map = 0; x_map <= 9; ++x_map)
	{
		for (y_map = 0; y_map <= 6; ++y_map)
		{
			switch (maptable[y_map][x_map])
			{
			case (0):
				hbmp = (HBITMAP)LoadImage(NULL, TEXT("mapchip_grass.bmp"), IMAGE_BITMAP, 0, 0,
					LR_LOADFROMFILE | LR_CREATEDIBSECTION);
				break;

			case (1):
				hbmp = (HBITMAP)LoadImage(NULL, TEXT("mapchip_wall.bmp"), IMAGE_BITMAP, 0, 0,
					LR_LOADFROMFILE | LR_CREATEDIBSECTION);
				break;
			}
			SelectObject(hMdc, hbmp);
			BitBlt(hbackDC, 225 + x_map * 32, 140 + y_map * 32, 32, 32, hMdc, 0, 0, SRCCOPY);

			DeleteObject(hbmp);
			// DeleteDC(hMdc); // これを入れると、マップが表示されない。
		}
	}

	// 主人公のBMP画像をファイルから読み込む
	
	if(hero1_direction == upward ){
		hbmp = (HBITMAP)LoadImage(NULL, TEXT("hero_dot_up.bmp"), IMAGE_BITMAP, 0, 0,
			LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	}
	
	if (hero1_direction == rightward) {
		hbmp = (HBITMAP)LoadImage(NULL, TEXT("hero_dot_right.bmp"), IMAGE_BITMAP, 0, 0,
			LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	}

	if (hero1_direction == downward) {
		hbmp = (HBITMAP)LoadImage(NULL, TEXT("hero_dot_down.bmp"), IMAGE_BITMAP, 0, 0,
			LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	}

	if (hero1_direction == leftward) {
		hbmp = (HBITMAP)LoadImage(NULL, TEXT("hero_dot_left.bmp"), IMAGE_BITMAP, 0, 0,
			LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	}

	SelectObject(hMdc, hbmp); // これを消すと、主人公ドットが表示されない。
	DeleteObject(hbmp);

	BitBlt(hbackDC, 320 + (chara_x - start_x) * 32, 270 + (chara_y - start_y) * (32), 170, 180, hMdc, 0, 0, SRCCOPY);


	// マップ上キャラのBMP画像をファイルから読み込む

	if (enemy1_alive == 1) {
		hbmp = (HBITMAP)LoadImage(NULL, TEXT("enemy_dot.bmp"), IMAGE_BITMAP, 0, 0,
			LR_LOADFROMFILE | LR_CREATEDIBSECTION);

		SelectObject(hMdc, hbmp); // これを消すと、ドットが表示されない。
		DeleteObject(hbmp);

		BitBlt(hbackDC, 320 + (enemy1_position_x - start_x) * 32, 270 + (enemy1_position_y - start_y) * (32), 170, 180, hMdc, 0, 0, SRCCOPY);
	}


	if (enemy2_alive == 1) {
		hbmp = (HBITMAP)LoadImage(NULL, TEXT("enemy_dot.bmp"), IMAGE_BITMAP, 0, 0,
			LR_LOADFROMFILE | LR_CREATEDIBSECTION);

		SelectObject(hMdc, hbmp); // これを消すと、ドットが表示されない。
		DeleteObject(hbmp);

		BitBlt(hbackDC, 320 + (enemy2_position_x - start_x) * 32, 270 + (enemy2_position_y - start_y) * (32), 170, 180, hMdc, 0, 0, SRCCOPY);
	}


	BitBlt(hdc, 0, 0, 700, 500, hbackDC, 0, 0, SRCCOPY); // 裏画面から本画面に転送のつもり。

	DeleteObject(hbmp);
	DeleteDC(hbackDC);
	DeleteDC(hMdc);

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


// 戦闘遭遇用の構造体変数の作成
static struct monster_def monster_def_list[8];


static int enemy_position_x = 1;

static int enemy_position_y = 1;


// 戦闘への突入の処理
void battle_start(HWND hWnd) {

	// MessageBox(NULL, TEXT("battle_enemy_startにいる。"), TEXT("キーテスト"), MB_OK);

			// 以下、定型文
			key_remain = 0;

			monster_hp = monster_def_list[encount_monters_id - 1].mon_hp_max;

			encount_mons_alive = 1;
			selecting_battle_mainmenu = 1;

			mode_scene = MODE_BATTLE;

			InvalidateRect(hWnd, NULL, TRUE);
			UpdateWindow(hWnd);

}

// モンスターとの遭遇判定
void check_encount_enemy1(HWND hWnd) {
	if (enemy1_alive == 1) {
		if (chara_x == enemy1_position_x && chara_y == enemy1_position_y) {

			encount_monters_id = 1;	// ここに、誰と遭遇したかを記述
			
			battle_start(hWnd);
		}
	}
}


void check_encount_enemy2(HWND hWnd) {
		if (enemy2_alive == 1) {
			if (chara_x == enemy2_position_x && chara_y == enemy2_position_y) {

				encount_monters_id = 2;	// ここに、誰と遭遇したかを記述
								
				battle_start(hWnd);
			}
		}
}



static int hero_already_attack_flag = 0;
static int enemy_already_attack_flag = 0;

static int battle_key = 1;


void hero_attack(HWND hWnd){

	// 主人公たちの攻撃
	{
		int s;

		/* 乱数の種 */
		srand((unsigned int)time(NULL));

		/* サイコロ */
		s = rand() % 6 + 1;

		// 敵にダメージ
		monster_hp = monster_hp - s;

		// モンスターの死亡の処理
		if (monster_hp < 1) {
			monster_hp = 0;


			if (encount_monters_id == 1) {
				enemy1_alive = 0;
				encount_mons_alive = 0;
			}

			if (encount_monters_id == 2) {
				enemy2_alive = 0;
				encount_mons_alive = 0;
			}
		}

		InvalidateRect(hWnd, NULL, TRUE);
		UpdateWindow(hWnd);
	}	
}



void enemy_attack(HWND hWnd) {

	// 敵たちの攻撃
	{
		int s;

		/* 乱数の種 */
		srand((unsigned int)time(NULL));

		/* サイコロ */
		s = rand() % (6/2) +5;

		// 主人公にダメージ
		hero_hp = hero_hp - s;

		// 主人公の死亡判定
		if (hero_hp < 1) {
			hero_hp = 0;

			// ここにゲームオーバーへのモード遷移

		}
	}

	InvalidateRect(hWnd, NULL, TRUE);
	UpdateWindow(hWnd);

}










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

	    //薬草の定義
	lstrcpy(item_def_list[0].item_name , TEXT("薬草"));
	item_def_list[0].item_type = 1;
	item_def_list[0].item_id = 1;

	    //所持アイテムの初期値
	
	item_have_list[0].have_item_id = 1;
	item_have_list[0].have_kosuu = 3;


		// モンスターの定義
	lstrcpy( monster_def_list[0].monster_name, TEXT("スライム"));
	monster_def_list[0].mon_hp_max = 3;
	monster_def_list[0].mon_agility = 2;
	monster_def_list[0].monster_id = 1;
	monster_def_list[0].mon_gold = 1;
	monster_def_list[0].mon_exp = 2;


	lstrcpy(monster_def_list[1].monster_name, TEXT("コボルト"));
	monster_def_list[1].mon_hp_max = 5;
	monster_def_list[1].mon_agility = 6;
	monster_def_list[1].monster_id = 2;
	monster_def_list[1].mon_gold = 10;
	monster_def_list[1].mon_exp = 5;




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
					lstrcpy(p, TEXT("はじめから"));
					TextOut(hdc, 150, 200, p, lstrlen(p));

					lstrcpy(p, TEXT("つづきから"));
					TextOut(hdc, 150, 250, p, lstrlen(p));

					lstrcpy(p, TEXT("おわる"));
					TextOut(hdc, 150, 300, p, lstrlen(p));

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

					lstrcpy(p, drawTalkString1);
					TextOut(hdc, 150, 250, p, lstrlen(p));

					lstrcpy(p, drawTalkString2);
					TextOut(hdc, 150, 300, p, lstrlen(p));

					lstrcpy(p, drawTalkString3);
					TextOut(hdc, 150, 350, p, lstrlen(p));
				}

				//DeleteObject(pen_blue); //ペンのメモリ解放

				//DeleteObject(blue_thin_1); //ブラシのメモリ解放
				//DeleteObject(blue_thin_2);
				//DeleteObject(brasi_pink_1);

			}


			if (mode_scene == MODE_MAP ) {
				
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
				Rectangle(hdc, 10, 10, 610 , 80 );


				HBRUSH brasi_pink_1;
				brasi_pink_1 = CreateSolidBrush(RGB(255, 180, 180));
				SelectObject(hdc, brasi_pink_1);
							
				Rectangle(hdc, 20 + (selecting_mainmenu - 1) * 100, 20, 
					100 + (selecting_mainmenu - 1) * 100,  70 );

				SetBkMode(hdc, TRANSPARENT);
				lstrcpy(p, TEXT("道具"));
				TextOut(hdc, 20, 20, p, lstrlen(p));

				lstrcpy(p, TEXT("装備"));
				TextOut(hdc, 120, 20, p, lstrlen(p));

				lstrcpy(p, TEXT("技能"));
				TextOut(hdc, 220, 20, p, lstrlen(p));

				lstrcpy(p, TEXT("セーブ"));
				TextOut(hdc, 320, 20, p, lstrlen(p));


				/* 所持金の表示欄 */
				SelectObject(hdc, blue_thin_1);

				Rectangle(hdc, 500 , 250,
					600 , 350);

				lstrcpy(p, TEXT("所持金"));
				TextOut(hdc, 510, 260, p, lstrlen(p));

				_stprintf_s(p, MAX_LENGTH,TEXT("%d"), your_money);
				TextOut(hdc, 510, 300, p, lstrlen(p));

				// _itot_s(your_money , p,200, 10);


				/* キャラのステータス欄 */
				Rectangle(hdc, 10, 100,
					300, 200);

				_stprintf_s(p, MAX_LENGTH, TEXT("%s"), hero_name);
				TextOut(hdc, 130, 105, p, lstrlen(p));


				lstrcpy(p, TEXT("HP"));
				TextOut(hdc, 130, 130, p, lstrlen(p));

				_stprintf_s(p, MAX_LENGTH, TEXT("%d"), hero_hp);
				TextOut(hdc, 160, 130, p, lstrlen(p));

				_stprintf_s(p, MAX_LENGTH, TEXT("/ %d"), hero_hp_max );
				TextOut(hdc, 190, 130, p, lstrlen(p));
		
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

				

			//	_stprintf_s(p, MAX_LENGTH, TEXT("%s"), hero_name);
			//	TextOut(hdc, 130, 105, p, lstrlen(p));

				SetBkMode(hdc, TRANSPARENT);
				lstrcpy(p, item_def_list[ (item_have_list[0].have_item_id -1) ].item_name);
				TextOut(hdc, 30, 130, p, lstrlen(p));

				_stprintf_s(p, MAX_LENGTH, TEXT("x %d"), item_have_list[0].have_kosuu );
				TextOut(hdc, 160, 130, p, lstrlen(p));

			//	_stprintf_s(p, MAX_LENGTH, TEXT("/ %d"), hero_hp_max);
				//TextOut(hdc, 190, 130, p, lstrlen(p));



				// デバッグ用
				lstrcpy(p, TEXT("sele_item :"));
				TextOut(hdc, 430, 200, p, lstrlen(p));

				_stprintf_s(p, MAX_LENGTH, TEXT("%d"), selecting_item);
				TextOut(hdc, 530, 200, p, lstrlen(p));


				lstrcpy(p, TEXT("item_x :"));
				TextOut(hdc, 430, 230, p, lstrlen(p));

				_stprintf_s(p, MAX_LENGTH, TEXT("%d"), selecting_item_x);
				TextOut(hdc, 490, 230, p, lstrlen(p));


				lstrcpy(p, TEXT("item_y :"));
				TextOut(hdc, 430, 280, p, lstrlen(p));

				_stprintf_s(p, MAX_LENGTH, TEXT("%d"), selecting_item_y);
				TextOut(hdc, 490, 280, p, lstrlen(p));




				// アイテム効果の確認用


				_stprintf_s(p, MAX_LENGTH, TEXT("%s"), hero_name);
				TextOut(hdc, 400, 300, p, lstrlen(p));


				lstrcpy(p, TEXT("HP"));
				TextOut(hdc, 400, 300 + 30, p, lstrlen(p));

				_stprintf_s(p, MAX_LENGTH, TEXT("%d"), hero_hp);
				TextOut(hdc, 400 + 30, 300 + 30, p, lstrlen(p));

				_stprintf_s(p, MAX_LENGTH, TEXT("/ %d"), hero_hp_max);
				TextOut(hdc, 400 + 60, 300 + 30, p, lstrlen(p));



			}





			if (mode_scene == MODE_BATTLE ) {

				HBRUSH brasi_white_1;
				brasi_white_1 = CreateSolidBrush(RGB(255, 255, 255) );
				SelectObject(hdc, brasi_white_1);

				Rectangle(hdc,0, 0,
					700, 500);




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
				lstrcpy(p, TEXT("戦う"));
				TextOut(hdc, 20, 20, p, lstrlen(p));

				lstrcpy(p, TEXT("逃げる"));
				TextOut(hdc, 120, 20, p, lstrlen(p));

				lstrcpy(p, TEXT("未定1"));
				TextOut(hdc, 220, 20, p, lstrlen(p));

				lstrcpy(p, TEXT("未定2"));
				TextOut(hdc, 320, 20, p, lstrlen(p));


				/* キャラのウィンドウ欄 */
				SelectObject(hdc, blue_thin_1);

				int chara_window_size_x = 140;
				Rectangle(hdc, 10, 410- chara_window_size_x,
					10 + chara_window_size_x, 410);


				/* キャラのステータス */
				TextOut(hdc, 20, 410 - chara_window_size_x + 10, hero_name, lstrlen(hero_name));

				lstrcpy(p, TEXT("HP"));
				TextOut(hdc, 20, 410 - chara_window_size_x +40, p, lstrlen(p));

				_stprintf_s(p, MAX_LENGTH, TEXT("%d"), hero_hp);
				TextOut(hdc, 50, 410 - chara_window_size_x +40, p, lstrlen(p));

				_stprintf_s(p, MAX_LENGTH, TEXT("/ %d"), hero_hp_max);
				TextOut(hdc, 50+30 , 410 - chara_window_size_x + 40, p, lstrlen(p));

				// _itot_s(your_money , p,200, 10);


				/* モンスターの表示 */
				// 試作なので名前とHPを表示。本番ではグラフィックに。
				TextOut(hdc, 270, 150, monster_def_list[encount_monters_id - 1].monster_name, lstrlen(monster_def_list[0].monster_name ) );

				lstrcpy(p, TEXT("HP"));
				TextOut(hdc, 270, 180, p, lstrlen(p));

				_stprintf_s(p, MAX_LENGTH, TEXT("%d"), monster_hp);
				TextOut(hdc, 300, 180, p, lstrlen(p));

			}



			if (mode_scene == MODE_BATTLE_WIN) {

				HBRUSH brasi_white_1;
				brasi_white_1 = CreateSolidBrush(RGB(255, 255, 255));
				SelectObject(hdc, brasi_white_1);

				Rectangle(hdc, 0, 0,
					700, 500);




				/* コマンドウィンドウ */
				HPEN pen_blue;

				pen_blue = CreatePen(PS_SOLID, 0, RGB(210, 210, 255));
				SelectObject(hdc, pen_blue);


				HBRUSH blue_thin_1, blue_thin_2;
				blue_thin_1 = CreateSolidBrush(RGB(210, 210, 255));
				blue_thin_2 = (HBRUSH)SelectObject(hdc, blue_thin_1);

				/* 戦闘終了ウィンドウ背景 */
				Rectangle(hdc, 200, 100, 450, 200);

				SetBkMode(hdc, TRANSPARENT);

				/* 戦闘終了メッセージ */
				lstrcpy(p, TEXT("戦闘に勝利した。"));
				TextOut(hdc, 270, 110, p, lstrlen(p));

				lstrcpy(p, TEXT("経験値"));
				TextOut(hdc, 210, 110 + 30, p, lstrlen(p));

				_stprintf_s(p, MAX_LENGTH, TEXT("%d  EXP"), monster_def_list[encount_monters_id -1].mon_exp);
				TextOut(hdc, 300, 110 + 30, p, lstrlen(p));


				lstrcpy(p, TEXT("金"));
				TextOut(hdc, 210, 110 + 30*2 , p, lstrlen(p));

				_stprintf_s(p, MAX_LENGTH, TEXT("%d  G"), monster_def_list[encount_monters_id -1].mon_gold);
				TextOut(hdc, 300, 110 + 30 * 2 , p, lstrlen(p));



				/* キャラのウィンドウ欄 */
				SelectObject(hdc, blue_thin_1);

				int chara_window_size_x = 140;
				Rectangle(hdc, 10, 410 - chara_window_size_x,
					10 + chara_window_size_x, 410);


				/* キャラのステータス */
				TextOut(hdc, 20, 410 - chara_window_size_x + 10, hero_name, lstrlen(hero_name));

				lstrcpy(p, TEXT("HP"));
				TextOut(hdc, 20, 410 - chara_window_size_x + 40, p, lstrlen(p));

				_stprintf_s(p, MAX_LENGTH, TEXT("%d"), hero_hp);
				TextOut(hdc, 50, 410 - chara_window_size_x + 40, p, lstrlen(p));

				_stprintf_s(p, MAX_LENGTH, TEXT("/ %d"), hero_hp_max);
				TextOut(hdc, 50 + 30, 410 - chara_window_size_x + 40, p, lstrlen(p));

				// _itot_s(your_money , p,200, 10);

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
						MessageBox(NULL, TEXT("ゲームをロード。"), TEXT("キーテスト"), MB_OK);
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

					if (selecting_OP < 1) {
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

					if (selecting_OP < 1) {
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

						lstrcpy(drawTalkString1, TEXT("王様「おお、勇者よ、よくぞ 来てくれた。"));
						lstrcpy(drawTalkString2, TEXT("そなたに、金100、さずけてあげよう。"));
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



		if (mode_scene == MODE_MAP && key_remain >0) { ///////
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

					check_encount_enemy1(hWnd);
					check_encount_enemy2(hWnd);

					break;
				}


			case VK_DOWN:

				{
					hero1_direction = downward;
					desti_y = chara_y + 1;

					//	移動可否の判定と、移動先座標の更新
					check_movable(hWnd);

					check_encount_enemy1(hWnd);
					check_encount_enemy2(hWnd);

					break;
				}


			case VK_RIGHT:

				{
					hero1_direction = rightward;
					desti_x = chara_x + 1;

					//	移動可否の判定と、移動先座標の更新
					check_movable(hWnd);

					check_encount_enemy1(hWnd);
					check_encount_enemy2(hWnd);

					break;
				}


			case VK_LEFT:

				{
					hero1_direction = leftward;
					desti_x = chara_x - 1;

					//	移動可否の判定と、移動先座標の更新
					check_movable(hWnd);

					check_encount_enemy1(hWnd);
					check_encount_enemy2(hWnd);

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

						if (hero_hp < hero_hp_max) {
							if (item_have_list[0].have_kosuu > 0) {
								hero_hp = hero_hp + 5;

								if (hero_hp > hero_hp_max) {
									hero_hp = hero_hp_max;
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


		if (mode_scene == MODE_BATTLE && key_remain > 0) {
			switch (wParam)
			{
			case 'Z':
				key_remain = 0;
				battle_key = 1;
				 {
					if (selecting_battle_mainmenu == 1)	{

						if ( hero_agility >= monster_def_list[0].mon_agility) {

							if (hero_already_attack_flag == 0 && battle_key == 1 ) {
									hero_attack(hWnd);
									hero_already_attack_flag = 1;
									battle_key = 0;
							}

							// キーボード入力しないかぎり
							//		下記コードを実行しないようにしたい

							if(encount_mons_alive == 1 
								&& hero_already_attack_flag == 1
								&& battle_key == 1
								){
								enemy_attack(hWnd);
								hero_already_attack_flag = 0; // フラグのリセット
								battle_key = 1;
							}		
						}


						if (hero_agility < monster_def_list[0].mon_agility) {

							if (encount_mons_alive == 1 && battle_key == 1) {
								enemy_attack(hWnd);
								enemy_already_attack_flag = 1;
								battle_key = 0;
							}

							// キーボード入力しないかぎり
							//		下記コードを実行しないようにしたい

							if (encount_mons_alive == 1 
								&& enemy_already_attack_flag == 1
								&& battle_key == 1) 
							{
								hero_attack(hWnd);
								enemy_already_attack_flag = 0; // フラグのリセット
								battle_key = 1;
							}

						}



						if (encount_mons_alive == 0) {

							// カネと経験値の更新
							your_money = your_money + monster_def_list[encount_monters_id - 1].mon_gold;
							hero_exp = hero_exp + monster_def_list[encount_monters_id - 1].mon_exp;

							// モード遷移
							mode_scene = MODE_BATTLE_WIN;

							InvalidateRect(hWnd, NULL, FALSE);
							UpdateWindow(hWnd);
							break;
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
