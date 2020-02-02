// sujiniku3DviewerNormal.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "framework.h"
#include "math.h"

#include "sujiniku3DviewerNormal.h"



#define MAX_LOADSTRING 100

// グローバル変数:
HINSTANCE hInst;                                // 現在のインターフェイス
WCHAR szTitle[MAX_LOADSTRING];                  // タイトル バーのテキスト
WCHAR szWindowClass[MAX_LOADSTRING];            // メイン ウィンドウ クラス名


static double Camera_x_Zahyou = 150;
double Camera_z_Zahyou=100;

double screen_z_Zahyou=250;


double kaiten_arrow_x;
double kaiten_arrow_z;




double hisyatai_n1p1_X = 200; // n1p1 とは1番目(nの直後の下図)の被写体の第1（pの直後）の点という意味。
double hisyatai_n1p1_Y = 50;
double hisyatai_n1p1_Z = 100;


double hisyatai_n1p2_X = 300; // 1番目の被写体の第2の点。
double hisyatai_n1p2_Y = 50;
double hisyatai_n1p2_Z = 100; // 500;



double hisyatai_n2p1_X = 300;
double hisyatai_n2p1_Y = 50;
double hisyatai_n2p1_Z = 50;


double hisyatai_n2p2_X = 400;
double hisyatai_n2p2_Y = 50;
double hisyatai_n2p2_Z = 50; // 500;



double Camera_x = 250;
double Camera_y = 50;
double Camera_z = 300;




int Camera_x_deffer = 0;
int Camera_y_deffer = 0;
int Camera_z_deffer = 0;

static TCHAR  mojiBuffer[200];


struct point_zahyou
{
    double x_zahyou;
    double y_zahyou;
    double z_zahyou;
};


static struct point_zahyou point_zahyou_list[30];
static struct point_zahyou touei_zahyou_list[30];
static struct point_zahyou kaiten_zahyou_list[30];

double   kaitenKaku = 0; // +(3.141 / 4) + (3.141 / 1);



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






    point_zahyou_list[1].x_zahyou = hisyatai_n1p1_X;
    point_zahyou_list[1].y_zahyou = hisyatai_n1p1_Y;
    point_zahyou_list[1].z_zahyou = hisyatai_n1p1_Z;


    point_zahyou_list[2].x_zahyou = hisyatai_n1p2_X;
    point_zahyou_list[2].y_zahyou = hisyatai_n1p2_Y;
    point_zahyou_list[2].z_zahyou = hisyatai_n1p2_Z;



    point_zahyou_list[3].x_zahyou = hisyatai_n2p1_X;
    point_zahyou_list[3].y_zahyou = hisyatai_n2p1_Y;
    point_zahyou_list[3].z_zahyou = hisyatai_n2p1_Z;


    point_zahyou_list[4].x_zahyou = hisyatai_n2p2_X;
    point_zahyou_list[4].y_zahyou = hisyatai_n2p2_Y;
    point_zahyou_list[4].z_zahyou = hisyatai_n2p2_Z;


    static struct point_zahyou screen_zahyou_list[30];


    touei_zahyou_list[1].z_zahyou = screen_z_Zahyou;
    touei_zahyou_list[2].z_zahyou = screen_z_Zahyou;


    touei_zahyou_list[3].z_zahyou = screen_z_Zahyou;
    touei_zahyou_list[4].z_zahyou = screen_z_Zahyou;





    // グローバル文字列を初期化する
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SUJINIKU3DVIEWERNORMAL, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // アプリケーション初期化の実行:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SUJINIKU3DVIEWERNORMAL));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SUJINIKU3DVIEWERNORMAL));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SUJINIKU3DVIEWERNORMAL);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   関数: InitInstance(HINSTANCE, int)
//
//   目的: インスタンス ハンドルを保存して、メイン ウィンドウを作成します
//
//   コメント:
//
//        この関数で、グローバル変数でインスタンス ハンドルを保存し、
//        メイン プログラム ウィンドウを作成および表示します。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // グローバル変数にインスタンス ハンドルを格納する

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

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
//  目的: メイン ウィンドウのメッセージを処理します。
//
//  WM_COMMAND  - アプリケーション メニューの処理
//  WM_PAINT    - メイン ウィンドウを描画する
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

            
            _stprintf_s(mojiBuffer, 200, TEXT("カメラx座標: %d"), (int)Camera_x); // デバッグ用メッセージ 
            TextOut(hdc, 450, 160 + 20, mojiBuffer, lstrlen(mojiBuffer));


            double jyoumen_x = 300;   double jyoumen_z = 100; // 上面図の基準点
            
            double arrowSiri_X = jyoumen_x + Camera_x; 
            double arrowSiri_Z = jyoumen_z + Camera_z;
           
            double arrowLength = 50;

            double arrowHead_X0 = arrowSiri_X;
            double arrowHead_Z0 = arrowSiri_Z - arrowLength;
            
            double arrowHeadRight_X = arrowHead_X0 + 10;
            double arrowHeadRight_Z = arrowHead_Z0 + 10;

           


            MoveToEx(hdc, arrowHead_X0 , arrowHead_Z0 , NULL); // 矢軸の矢先側
            LineTo(hdc, arrowSiri_X, arrowSiri_Z); // 矢の尻 // カメラはここにいる

            MoveToEx(hdc, arrowHead_X0, arrowHead_Z0, NULL); // 矢先
            LineTo(hdc, arrowHeadRight_X , arrowHeadRight_Z );// 矢先の右側


          //  kaitenKaku = 0;

            kaiten_arrow_x = cos(kaitenKaku) * (0) + (-1) * sin(kaitenKaku) * (-1 * arrowLength);
           // kaiten_zahyou_list[1].y_zahyou = point_zahyou_list[1].y_zahyou;
            kaiten_arrow_z = sin(kaitenKaku) * (0) + cos(kaitenKaku) * (-1 * arrowLength);





            // 回転デバッグのモニター
            int offsetRotX = 150;
            MoveToEx(hdc, offsetRotX + arrowSiri_X + kaiten_arrow_x, arrowSiri_Z + kaiten_arrow_z , NULL); // 矢軸の矢先側
            LineTo(hdc, offsetRotX + arrowSiri_X, arrowSiri_Z ); // 矢の尻 // カメラはここにいる

            HBRUSH brasi_parts_2;
            

            brasi_parts_2 = CreateSolidBrush(RGB(100, 255, 100)); // 壁の表示用のgreen色のブラシを作成
            SelectObject(hdc, brasi_parts_2); // ウィンドウhdcと、さきほど作成したブラシを関連づけ


            Rectangle(hdc,
              offsetRotX + arrowSiri_X, arrowSiri_Z,
              offsetRotX + arrowSiri_X + 5 , arrowSiri_Z + 5
            );


         //   MoveToEx(hdc, arrowHead_X, arrowHead_Z, NULL); // 矢先
         //   LineTo(hdc, arrowHeadRight_X, arrowHeadRight_Z);// 矢先の右側

            TCHAR mojibuf2[100];
            _stprintf_s(mojibuf2, 100, TEXT("回転角  %d"), (int)kaitenKaku);
            TextOut(hdc, 360, 260, mojibuf2, lstrlen(mojibuf2));





            MoveToEx(hdc, jyoumen_x + hisyatai_n1p1_X, jyoumen_z + hisyatai_n1p1_Z, NULL); // 被写体1の片側
            LineTo(hdc, jyoumen_x + hisyatai_n1p2_X, jyoumen_z + hisyatai_n1p1_Z);


            MoveToEx(hdc, jyoumen_x + hisyatai_n2p1_X, jyoumen_z + hisyatai_n2p1_Z, NULL); // 被写体11の片側
            LineTo(hdc, jyoumen_x + hisyatai_n2p2_X, jyoumen_z + hisyatai_n2p1_Z);



            MoveToEx(hdc, jyoumen_x + 150, jyoumen_z + screen_z_Zahyou, NULL); // スクリーンの片側
            LineTo(hdc, jyoumen_x + 300, jyoumen_z + screen_z_Zahyou);

     

            //            int pro1X;



            kaiten_zahyou_list[1].x_zahyou = cos(kaitenKaku) * (point_zahyou_list[1].x_zahyou - Camera_x) + (-1) * sin(kaitenKaku) * (point_zahyou_list[1].z_zahyou - Camera_z) + Camera_x;
            kaiten_zahyou_list[1].y_zahyou = point_zahyou_list[1].y_zahyou ;
            kaiten_zahyou_list[1].z_zahyou = sin(kaitenKaku) * (point_zahyou_list[1].x_zahyou - Camera_x) + cos (kaitenKaku) * (point_zahyou_list[1].z_zahyou - Camera_z) + Camera_z;

            kaiten_zahyou_list[2].x_zahyou = cos(kaitenKaku) * (point_zahyou_list[2].x_zahyou - Camera_x) + (-1) * sin(kaitenKaku) * (point_zahyou_list[2].z_zahyou - Camera_z) + Camera_x ;
            kaiten_zahyou_list[2].y_zahyou = point_zahyou_list[2].y_zahyou;
            kaiten_zahyou_list[2].z_zahyou = sin(kaitenKaku) * (point_zahyou_list[2].x_zahyou - Camera_x) + cos(kaitenKaku) * (point_zahyou_list[2].z_zahyou - Camera_z) + Camera_z;


            kaiten_zahyou_list[3].x_zahyou = cos(kaitenKaku) * (point_zahyou_list[3].x_zahyou - Camera_x) + (-1) * sin(kaitenKaku) * (point_zahyou_list[3].z_zahyou - Camera_z) + Camera_x;
            kaiten_zahyou_list[3].y_zahyou = point_zahyou_list[3].y_zahyou;
            kaiten_zahyou_list[3].z_zahyou = sin(kaitenKaku) * (point_zahyou_list[3].x_zahyou - Camera_x) + cos(kaitenKaku) * (point_zahyou_list[3].z_zahyou - Camera_z) + Camera_z;

            kaiten_zahyou_list[4].x_zahyou = cos(kaitenKaku) * (point_zahyou_list[4].x_zahyou - Camera_x) + (-1) * sin(kaitenKaku) * (point_zahyou_list[4].z_zahyou - Camera_z) + Camera_x;
            kaiten_zahyou_list[4].y_zahyou = point_zahyou_list[4].y_zahyou;
            kaiten_zahyou_list[4].z_zahyou = sin(kaitenKaku) * (point_zahyou_list[4].x_zahyou - Camera_x) + cos(kaitenKaku) * (point_zahyou_list[4].z_zahyou - Camera_z) + Camera_z;



            //int pro1
            touei_zahyou_list[1].x_zahyou = (screen_z_Zahyou - Camera_z) / (point_zahyou_list[1].z_zahyou - Camera_z) * (kaiten_zahyou_list[1].x_zahyou - Camera_x)  ;
            touei_zahyou_list[1].y_zahyou = (screen_z_Zahyou - Camera_z) / (point_zahyou_list[1].z_zahyou - Camera_z) * (kaiten_zahyou_list[1].y_zahyou - Camera_y)  ;


            //    int pro2X 
            touei_zahyou_list[2].x_zahyou = (screen_z_Zahyou - Camera_z) / (point_zahyou_list[2].z_zahyou - Camera_z) * (kaiten_zahyou_list[2].x_zahyou - Camera_x) ;
            touei_zahyou_list[2].y_zahyou = (screen_z_Zahyou - Camera_z) / (point_zahyou_list[2].z_zahyou - Camera_z) * (kaiten_zahyou_list[2].y_zahyou - Camera_y) ;



            // int pro3
            touei_zahyou_list[3].x_zahyou = (screen_z_Zahyou - Camera_z) / (point_zahyou_list[3].z_zahyou - Camera_z) * (kaiten_zahyou_list[3].x_zahyou - Camera_x) ;
            touei_zahyou_list[3].y_zahyou = (screen_z_Zahyou - Camera_z) / (point_zahyou_list[3].z_zahyou - Camera_z) * (kaiten_zahyou_list[3].y_zahyou - Camera_y) ;

            // int pro4
            touei_zahyou_list[4].x_zahyou = (screen_z_Zahyou - Camera_z) / (point_zahyou_list[4].z_zahyou - Camera_z) * (kaiten_zahyou_list[4].x_zahyou - Camera_x) ;
            touei_zahyou_list[4].y_zahyou = (screen_z_Zahyou - Camera_z) / (point_zahyou_list[4].z_zahyou - Camera_z) * (kaiten_zahyou_list[4].y_zahyou - Camera_y) ;




           // HBRUSH brasi_parts_2;
            int hisyatai_onViewX = 350;  int hisyatai_onViewY = 150;  


            brasi_parts_2 = CreateSolidBrush(RGB(100, 255, 100)); // 壁の表示用のgreen色のブラシを作成
            SelectObject(hdc, brasi_parts_2); // ウィンドウhdcと、さきほど作成したブラシを関連づけ

            Rectangle(hdc,
                hisyatai_onViewX + touei_zahyou_list[3].x_zahyou, hisyatai_onViewY + touei_zahyou_list[3].y_zahyou,
                hisyatai_onViewX + touei_zahyou_list[4].x_zahyou, hisyatai_onViewY + touei_zahyou_list[4].y_zahyou + 40); // 基準の状態			



            brasi_parts_2 = CreateSolidBrush(RGB(100, 100, 255)); // 壁の表示用のブルー色のブラシを作成
            SelectObject(hdc, brasi_parts_2); // ウィンドウhdcと、さきほど作成したブラシを関連づけ

            Rectangle(hdc, 
                hisyatai_onViewX + touei_zahyou_list[1].x_zahyou, hisyatai_onViewY + touei_zahyou_list[1].y_zahyou,
                hisyatai_onViewX + touei_zahyou_list[2].x_zahyou, hisyatai_onViewY + touei_zahyou_list[2].y_zahyou + 30); // 基準の状態			


 



            TCHAR mojibuf[100];
            _stprintf_s(mojibuf, 100, TEXT("pro1X %d"), (int) touei_zahyou_list[1].x_zahyou);
            TextOut(hdc, 550 , 260 , mojibuf, lstrlen(mojibuf));

            _stprintf_s(mojibuf, 100, TEXT("pro2X %d"), (int)touei_zahyou_list[2].x_zahyou);
            TextOut(hdc, 550, 260 +30, mojibuf, lstrlen(mojibuf));

            _stprintf_s(mojibuf, 100, TEXT("pro2X - pro1X  %d"), (int) touei_zahyou_list[2].x_zahyou - (int)touei_zahyou_list[1].x_zahyou );
            TextOut(hdc, 550, 260 + 30 + 30 , mojibuf, lstrlen(mojibuf));




            _stprintf_s(mojibuf, 100, TEXT("pro1Y  %d"), (int) touei_zahyou_list[1].y_zahyou );
            TextOut(hdc, 550 + 160, 260, mojibuf, lstrlen(mojibuf));

            _stprintf_s(mojibuf, 100, TEXT("pro2Y  %d"), (int) touei_zahyou_list[2].y_zahyou );
            TextOut(hdc, 550 + 160, 260 + 30, mojibuf, lstrlen(mojibuf));


   



            brasi_parts_2 = CreateSolidBrush(RGB(255, 100, 100)); // 壁の表示用のピンク色のブラシを作成
            SelectObject(hdc, brasi_parts_2); // ウィンドウhdcと、さきほど作成したブラシを関連づけ

            /*
            Rectangle(hdc, 
                hisyatai_onViewX - (hisyatai_n1p2_Z / screen_z_Zahyou)  * Camera_x_deffer, hisyatai_onViewY,
                hisyatai_onViewX + 10 - (hisyatai_n1p2_Z / screen_z_Zahyou) * Camera_x_deffer, hisyatai_onViewY + 20); // 基準の状態			
                
            */

           // MessageBox(NULL, TEXT("battle_enemy_startにいる。"), TEXT("キーテスト"), MB_OK);





            ///// ここから比較用

            brasi_parts_2;
            brasi_parts_2 = CreateSolidBrush(RGB(100, 100, 255)); // 壁の表示用のブルー色のブラシを作成
            SelectObject(hdc, brasi_parts_2); // ウィンドウhdcと、さきほど作成したブラシを関連づけ

            hisyatai_onViewX = 200;  hisyatai_onViewY = 250;  //　これは単なるウィンドウ内での初期の表示位置の調整用

            Rectangle(hdc, hisyatai_onViewX - (point_zahyou_list[1].z_zahyou / screen_z_Zahyou) * Camera_x_deffer, hisyatai_onViewY, hisyatai_onViewX + 20 - (point_zahyou_list[1].z_zahyou / screen_z_Zahyou) * Camera_x_deffer, hisyatai_onViewY + 30); // 基準の状態			



            brasi_parts_2 = CreateSolidBrush(RGB(255, 100, 100)); // 壁の表示用のピンク色のブラシを作成
            SelectObject(hdc, brasi_parts_2); // ウィンドウhdcと、さきほど作成したブラシを関連づけ


            Rectangle(hdc, hisyatai_onViewX - (point_zahyou_list[2].z_zahyou / screen_z_Zahyou) * Camera_x_deffer, hisyatai_onViewY, hisyatai_onViewX + 10 - (point_zahyou_list[2].z_zahyou / screen_z_Zahyou) * Camera_x_deffer, hisyatai_onViewY + 20); // 基準の状態	







            EndPaint(hWnd, &ps);
        }
        break;

    case WM_KEYDOWN:

        //if (game_mode == opening_mode) {
            switch (wParam) {
            case VK_UP: // 「上キーが入力されたら」という意味
             Camera_z = Camera_z -5;
             screen_z_Zahyou = screen_z_Zahyou - 5;

                /* ここに上（↑）キー入力後の処理を書く*/
                break; // これは VK_UP: からのbreak

            case VK_DOWN:
              Camera_z = Camera_z + 5; 
              screen_z_Zahyou = screen_z_Zahyou + 5;

                /* ここに下（↓）キー入力後の処理を書く*/
                break;  // これは VK_DOWN: からのbreak
            case VK_RIGHT:
                /* ここに右（→）キー入力後の処理を書く*/
             //   MessageBox(NULL, TEXT("battle_enemy_startにいる。"), TEXT("キーテスト"), MB_OK);


                Camera_x = Camera_x + 10;


               Camera_x_deffer = Camera_x_deffer + 10;

                break; // これは VK_RIGHT からのbreak
            case VK_LEFT:
                /* ここに左（←）キー入力後の処理を書く*/

                Camera_x = Camera_x - 10;

               Camera_x_deffer = Camera_x_deffer - 10;

                break; // これは VK_LEFT: からのbreak



            case 'W':
                /* ここにWキー入力後の処理を書く*/
                kaitenKaku = kaitenKaku + 0.2;

                break; // これは VK_LEFT: からのbreak

            case 'Q':
                /* ここにQキー入力後の処理を書く*/
                kaitenKaku = kaitenKaku - 0.2;

                break; // これは VK_LEFT: からのbreak



            }  // これは switch (wParam) の終わりのカッコ

        //}

           // MessageBox(NULL, TEXT("aaaaaatにいる。"), TEXT("キーテスト"), MB_OK);

        InvalidateRect(hWnd, NULL, TRUE);
        UpdateWindow(hWnd);


        break; // これは WM_KEYDOWN: からのbreak



    case WM_DESTROY:
        PostQuitMessage(0);
        break;
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
