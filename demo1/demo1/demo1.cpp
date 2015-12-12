// demo1.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "demo1.h"

// ȫ�ֱ���:
static int Ic;
static HWND searchbtn;                          //
static HWND combo;
HINSTANCE hInst;														// ��ǰʵ��
TCHAR szTitle[MAX_LOADSTRING];								// �������ı�
TCHAR szWindowClass[MAX_LOADSTRING];				// ����������
std::wstring TemplteStr=_T("");
LogFile *lg=NULL;
ToolFile *tf=NULL;
ViewLog *vl=NULL;


// �˴���ģ���а����ĺ�����ǰ������:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    SearchEditProc(HWND, UINT, WPARAM, LPARAM);          //SEARCHEDIT�´��ڹ���
WNDPROC OldSearchEditProc;																			//SEARCHEDIT�ɴ��ڹ���


int APIENTRY _tWinMain(	HINSTANCE hInstance,HINSTANCE hPrevInstance,LPTSTR lpCmdLine,int nCmdShow	)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
 	// TODO: �ڴ˷��ô��롣
	MSG msg;
	HACCEL hAccelTable;

	// ��ʼ��ȫ���ַ���
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_DEMO1, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ִ��Ӧ�ó����ʼ��:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}
	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DEMO1));
	// ����Ϣѭ��:
	while (GetMessage(&msg, NULL, 0, 0))
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
//  ����: MyRegisterClass()
//  Ŀ��: ע�ᴰ���ࡣ
//
//  ע��:
//  ����ϣ��
//  �˴�������ӵ� Windows 95 �еġ�RegisterClassEx��
//  ����֮ǰ�� Win32 ϵͳ����ʱ������Ҫ�˺��������÷������ô˺���ʮ����Ҫ��
//  ����Ӧ�ó���Ϳ��Ի�ù�����
//  ����ʽ��ȷ�ġ�Сͼ�ꡣ
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DEMO1));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= CreateSolidBrush(GetSysColor(COLOR_BTNFACE));//(HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	=MAKEINTRESOURCE(IDC_DEMO1); //NULL
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON1));//IDI_SMALL

	return RegisterClassEx(&wcex);
}

//
//   ����: InitInstance(HINSTANCE, int)
//
//   Ŀ��: ����ʵ�����������������
//
//   ע��:
//
//   �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//   ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;
   hInst = hInstance; 
   hWnd = CreateWindow(szWindowClass, szTitle,WS_OVERLAPPEDWINDOW|WS_MAXIMIZE,
						CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
      
   if (!hWnd)
   {
      return FALSE;
   }
   
   //ShowWindow(hWnd, nCmdShow);
   ShowWindow(hWnd,SW_SHOWMAXIMIZED);                   //��ʼ��� 
   UpdateWindow(hWnd);
   return TRUE;
}

//
//  ����: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��: ���������ڵ���Ϣ��
//
//  WM_COMMAND	- ����Ӧ�ó���˵�
//  WM_PAINT	- ����������
//  WM_DESTROY	- �����˳���Ϣ������
//
//���ڹ��̺�����
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{	
	static OPENFILENAME ofn;
    static TCHAR szFileName[MAX_PATH];
    static TCHAR szTitleName[MAX_PATH];
    static TCHAR szFilter[]=TEXT("��־�ļ�(*.gen)\0*.gen\0ѹ���ļ�(*.tra)\0*.tra\0\0");

	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	static HWND searchEdit,openfilebtn,level,hsearch,module,allbtn,infobtn,warnningbtn,errorbtn,hlistview,hitem,hshowitem;
    int iIndex;

	switch (message)
	{
	case WM_CREATE:      
		{


			SetWindowText(hWnd,TEXT("Log Analysis"));
			searchEdit= CreateWindowEx(WS_EX_CLIENTEDGE,TEXT("edit"),NULL,WS_CHILD|WS_VISIBLE|WS_BORDER|ES_LEFT|ES_AUTOHSCROLL|ES_WANTRETURN,0,0,0,0,hWnd,(HMENU)ID_SEARCHEDIT,((LPCREATESTRUCT)lParam)->hInstance,NULL);
			openfilebtn=CreateWindow(TEXT("button"),TEXT("openLog"),WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,0,0,0,0,hWnd,(HMENU)ID_OPENFILEBTN,((LPCREATESTRUCT)lParam)->hInstance,NULL);
			searchbtn=CreateWindow(TEXT("button"),TEXT("run"),WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,0,0,0,0,hWnd,(HMENU)ID_SEARCHBTN,((LPCREATESTRUCT)lParam)->hInstance,NULL);
			level=CreateWindow(TEXT("static"),TEXT("Level :"),WS_CHILD|WS_VISIBLE|SS_LEFT,0,0,0,0,hWnd,(HMENU)ID_LEVEL,((LPCREATESTRUCT)lParam)->hInstance,NULL);
			module=CreateWindow(TEXT("static"),TEXT("Built-in Module"),WS_CHILD|WS_VISIBLE|SS_LEFT,0,0,0,0,hWnd,(HMENU)ID_MODULE,((LPCREATESTRUCT)lParam)->hInstance,NULL);
			hitem=CreateWindow(TEXT("static"),TEXT("Item :"),WS_CHILD|WS_VISIBLE|SS_LEFT,0,0,0,0,hWnd,(HMENU)ID_ITEM,((LPCREATESTRUCT)lParam)->hInstance,NULL);
			hsearch=CreateWindow(TEXT("static"),TEXT("Search :"),WS_CHILD|WS_VISIBLE|SS_LEFT,0,0,0,0,hWnd,(HMENU)ID_SEARCH,((LPCREATESTRUCT)lParam)->hInstance,NULL);
			hshowitem=CreateWindow(TEXT("static"),TEXT("��"),WS_CHILD|WS_VISIBLE|SS_LEFT,0,0,0,0,hWnd,(HMENU)ID_ITEMSHOW,((LPCREATESTRUCT)lParam)->hInstance,NULL);
			allbtn=CreateWindow(TEXT("button"),TEXT("All"),WS_CHILD|WS_VISIBLE|BS_AUTORADIOBUTTON,0,0,0,0,hWnd,(HMENU)ID_ALL,((LPCREATESTRUCT)lParam)->hInstance,NULL);
			infobtn=CreateWindow(TEXT("button"),TEXT("Info"),WS_CHILD|WS_VISIBLE|BS_AUTORADIOBUTTON,0,0,0,0,hWnd,(HMENU)ID_INFO,((LPCREATESTRUCT)lParam)->hInstance,NULL);
			warnningbtn=CreateWindow(TEXT("button"),TEXT("Warn"),WS_CHILD|WS_VISIBLE|BS_AUTORADIOBUTTON,0,0,0,0,hWnd,(HMENU)ID_WARNNING,((LPCREATESTRUCT)lParam)->hInstance,NULL);
			errorbtn=CreateWindow(TEXT("button"),TEXT("Error"),WS_CHILD|WS_VISIBLE|BS_AUTORADIOBUTTON,0,0,0,0,hWnd,(HMENU)ID_ERROR,((LPCREATESTRUCT)lParam)->hInstance,NULL);
			combo=CreateWindow(TEXT("combobox"),TEXT("combo"),WS_CHILD|WS_VISIBLE|CBS_DROPDOWNLIST,0,0,0,0,hWnd,(HMENU)ID_COMB,((LPCREATESTRUCT)lParam)->hInstance,NULL);
            hlistview=CreateWindowEx(WS_EX_CLIENTEDGE,TEXT("syslistview32"),TEXT("combo"),WS_VISIBLE|WS_CHILD|WS_BORDER| LVS_REPORT | LVS_SHOWSELALWAYS/*| WS_VSCROLL*/, 0, 0,0,0,hWnd, (HMENU)ID_LISTVIEW, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
			
			OldSearchEditProc=(WNDPROC)SetWindowLong(searchEdit,GWL_WNDPROC,(LONG)SearchEditProc);         //SEARCHEDIT�ɴ��ڹ���

			SendMessage(combo,CB_INSERTSTRING,0,LPARAM(TEXT("all")));
			SendMessage(combo, CB_SETCURSEL,0,LPARAM(TEXT("all")));

			//SetFocus(hlistview);																							//��ý���
			//ListView_SetItemState(hlistview,0,LVIS_SELECTED,LVIS_SELECTED);                    //ʹ��һ�д���ѡ����״̬
			//��������ѡ����
			ListView_SetExtendedListViewStyle(hlistview,LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);                   
			
//******************************************************************************************************************
//ListView������
			TCHAR szlistname[4][10] = {_T("Time"), _T("Thread"),_T("Level"), _T("Log")};
			LV_COLUMN lvcolumn;
			for(int i=0;i<4;i++)
			{
				lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT 
				| LVCF_WIDTH | LVCF_ORDER;
				lvcolumn.fmt = LVCFMT_LEFT;
				lvcolumn.pszText = szlistname[i];
				lvcolumn.iSubItem = i;
				lvcolumn.iOrder = i;
				if(i<3){lvcolumn.cx = 100 ; }
				else
					lvcolumn.cx =2000;
				ListView_InsertColumn(hlistview, i, &lvcolumn);
			}
//********************************************************************************************************************                                  
			//����json�ļ�����combo��Ŀ
			Json::Value OpenJson();
			for(int i=0;i<(OpenJson()).size();i++)
			{
				OpenJson()[i]["name"].asString();
				SendMessage(combo,CB_INSERTSTRING,i+1,LPARAM((LPTSTR)tf->AnsiToUnicode(OpenJson()[i]["name"].asString().c_str()).c_str()));		
			}
		}
		return 0;

	case WM_SIZE:
		{	
			int cxChar=HIWORD(GetDialogBaseUnits());
			int cyChar=LOWORD(GetDialogBaseUnits());
			int x=(LOWORD (lParam))/(2*cxChar);
			int y=(HIWORD (lParam))/(7*cyChar);

			MoveWindow(searchEdit,6*cxChar,8*cyChar,10*cxChar,3*cyChar,TRUE);
			MoveWindow(openfilebtn,28*cxChar,2*cyChar,7*cxChar,3*cyChar,TRUE);
			MoveWindow(searchbtn,17*cxChar,8*cyChar,4*cxChar,3*cyChar,TRUE);
			MoveWindow(level,1*cxChar,(2.5)*cyChar,5*cxChar,3*cyChar,TRUE);
			MoveWindow(module,22*cxChar,(8.5)*cyChar,7*cxChar,3*cyChar,TRUE);
			MoveWindow(hsearch,cxChar,8*cyChar,5*cxChar,3*cyChar,TRUE);
			MoveWindow(allbtn,6*cxChar,2*cyChar,5*cxChar,3*cyChar,TRUE);
			MoveWindow(infobtn,11*cxChar,2*cyChar,5*cxChar,3*cyChar,TRUE);
			MoveWindow(warnningbtn,16*cxChar,2*cyChar,6*cxChar,3*cyChar,TRUE);
			MoveWindow(errorbtn,22*cxChar,2*cyChar,5*cxChar,3*cyChar,TRUE);
			MoveWindow(combo,29*cxChar,8*cyChar,6*cxChar,22*cyChar,TRUE);
			MoveWindow(hlistview,cxChar,13*cyChar,31.8*x,48*y,TRUE);
			MoveWindow(hshowitem,(2*x-3)*cxChar,9*cyChar,5*cxChar,3*cyChar,TRUE);
			MoveWindow(hitem,(2*x-6)*cxChar,9*cyChar,5*cxChar,3*cyChar,TRUE);
			break;
		}
		case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// �����˵�ѡ��:
		switch (wmId)
		{	
		case  ID_OPENFILEBTN:
         {
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner=hWnd;
			ofn.hInstance=NULL;
			ofn.lpstrFilter=szFilter;													//������
			ofn.lpstrCustomFilter=NULL;
			ofn.nMaxCustFilter=0;
			ofn.nFilterIndex=0;                                                      //����������
			ofn.lpstrFile=szFileName;												//�ļ���    (��·��)
			ofn.nMaxFile=MAX_PATH;
			ofn.lpstrFileTitle=szTitleName;										//�ļ���
			ofn.nMaxFileTitle=MAX_PATH;
			ofn.lpstrInitialDir=NULL;
			ofn.lpstrTitle=NULL;
			ofn.Flags=OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			ofn.nFileOffset=0;
			ofn.nFileExtension=0;
			ofn.lpstrDefExt=TEXT("txt");
			ofn.lCustData=0L;
			ofn.lpfnHook=NULL;
			ofn.lpTemplateName=NULL;
			if (GetOpenFileName(&ofn)==TRUE)
			{	
				if(ofn.nFilterIndex==2)
				{
					HZIP hz = OpenZip(szFileName,"2009.ZDKJ@gensee.com"); 		
					ZIPENTRY ze; 
					GetZipItem(hz,-1,&ze);
					int numitems=ze.index; 			
					for (int i=0; i<numitems; i++)   
					{ 
						GetZipItem(hz,i,&ze);     
						UnzipItem(hz,i,ze.name); 	
					} 
				std::wstring JieyaStr=TEXT(" -  ��ѹ�ɹ� ! ");
				MessageBox(NULL,(szFileName+JieyaStr).c_str(),TEXT(" �� ѹ"),MB_ICONINFORMATION);
				CloseZip(hz);		
				}
				if (ofn.nFilterIndex==1)
				{
					//SetWindowText(hWnd,szFileName);
					std::wstring szfn=szFileName;
					std::wstring headStr=TEXT("Log Analysis �� ");
					SetWindowText(hWnd,(headStr+szfn).c_str());
					CheckRadioButton(hWnd,ID_ALL,ID_ERROR,ID_ALL);
					if (NULL==lg )															//4.27
					{
						delete lg;
						lg=NULL;
					}
					lg = new LogFile(szFileName);
					lg->parseA();		
					SendMessage(allbtn,BM_CLICK,0,0);                        //Ĭ�ϴ�ALL			                                                  
				}
			}
			else
			{
				MessageBox(hWnd,TEXT("�ļ��򿪴���ʧ �ܣ�! ��ѡ����ȷ���ļ� "),TEXT(" ERROR "),MB_ICONINFORMATION|MB_OK);
				SetWindowText(hWnd,TEXT("һ �� Ч �� �� �� · �� ��һ"));
				break;
			}
            return 0;                 
		 }	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		case ID_ALL:
			Ic=0;
			vl->ShowListView(searchEdit,hlistview,hshowitem,Ic,TemplteStr,lg);
			return 0;
		case ID_INFO:
			Ic=1;
			vl->ShowListView(searchEdit,hlistview,hshowitem,Ic,TemplteStr,lg);
		return 0;
		case ID_WARNNING:
			Ic=2;
			vl->ShowListView(searchEdit,hlistview,hshowitem,Ic,TemplteStr,lg);
		return 0;
		case ID_ERROR:
			Ic=3;
			vl->ShowListView(searchEdit,hlistview,hshowitem,Ic,TemplteStr,lg);
		return 0;
		case  ID_SEARCHBTN:
			vl->ShowListView(searchEdit,hlistview,hshowitem,Ic,TemplteStr,lg);
		return 0;
		case ID_MENUOPEN: SendMessage(openfilebtn,BM_CLICK,0,0); return 0;
		case ID_MENUALL: SendMessage(allbtn,BM_CLICK,0,0); return 0;
		case ID_MENUINFO: SendMessage(infobtn,BM_CLICK,0,0); return 0;
		case ID_MENUWARN: SendMessage(warnningbtn,BM_CLICK,0,0); return 0;
		case ID_MENUERROR: SendMessage(errorbtn,BM_CLICK,0,0);	return 0;
			
		case ID_COMB:
		if (wmEvent==CBN_SELCHANGE)
		{
			Ic=9;
			Json::Value OpenJson();
			iIndex=SendMessage(combo,CB_GETCURSEL,0,0);
			//OpenJson()[iIndex]["name"].asString();
			OpenJson()[iIndex]["nameof"].asString();
			TemplteStr=tf->AnsiToUnicode((OpenJson()[iIndex-1]["nameof"].asString()).c_str()).c_str();
			vl->ShowListView(searchEdit,hlistview,hshowitem,Ic,TemplteStr,lg);
		}
		return 0;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
	    break;
	case IDM_EXIT:
		{
			DestroyWindow(hWnd);		
// 				HMENU hmenu=GetMenu(hWnd);						
// 				DestroyMenu (hmenu);
// 				DrawMenuBar (hWnd);		
			
			}
		break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	    break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);	
		EndPaint(hWnd, &ps);
        break;
    case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

//�����ڡ������Ϣ�������
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

//*****************************************************************************
//SearchEdit �´��ڹ���
LRESULT CALLBACK SearchEditProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_KEYDOWN:
		if (wParam==VK_RETURN)
		{
			SendMessage(searchbtn,BM_CLICK,0,0);
		}
	}
	return CallWindowProc(OldSearchEditProc,hWnd,message,wParam,lParam);
}


