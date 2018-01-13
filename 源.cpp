/*课设计算器  制作者：李仕浩*/
/*仍存在的问题有如下：*/
/*1.一开始时时可以输入多个零，但不影响计算*/
/*2.每输完一个数按操作符时会出现一个小数点，不美观*/
/*3.输入的数不能太变态（不能超过double型数允许的范围和精度），应该不会有神经病用我的计算器做这种事吧*/
/*4.输入的数太长会看不到前面的*/
/*特色：*/
/*1.完美的识别除零错误*/
/*2.能实现+,-,*,/的连续运算,例如按1+2+=得到5*/
/*3.严格的逻辑审查：如输入“1.2.3”时“.3”将不能输入*/
/*4.输入多个运算符时，后面的将前面的覆盖，如3+*-1最后得到2*/
/*！！！最重要的亮点来了：支持键盘输入！！！*/
/* 左键 +  右键 -  上键 *  下键 /  */
/*Enter键与空格键 =  Back键退格 Delete键清除*/

#include<windows.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include<string>
#include"resource.h"
#include <sstream>

/*在此定义窗口用的变量及函数*/
static HINSTANCE hIns;
static TCHAR szAppName[] = TEXT("Calculator");
HWND hEditResult, hButtonOptSum0, hButtonOptSum1, hButtonOptSum2, hButtonOptSum3, hButtonOptSum4, hButtonOptSum5, hButtonOptSum6, hButtonOptSum7,
hButtonOptSum8, hButtonOptSum9;
HWND hButtonOptSumDec, hButtonOptAdd, hButtonOptSub, hButtonOptMul, hButtonOptDiv,
hButtonOptMin, hButtonOptPoint, hButtonOptEqu, hButtonOptRec, hButtonCancelEntry, hButtonBack;   //声明那群子窗口
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);				//声明窗口过程函数

/*在此定义运算用的变量及函数*/
char ss[1024] = "";
char s1[1024] = "";
char s2[1024] = "";
double num1 = 0, num2 = 0;
char optflag[] = "N";		      //判断是否按了操作符键并贮存操作符号
char *stop;
int numflag = 0;				  //判断是否按了数字键
int div0flag = 0;				  //除零错误要用的flag
bool pointflag = false;			  //判断是否按了点操作符
void NumResult(char *NumData);    //按下数字键(0到9和小数点)的操作处理函数
void EquResult();                 //按下操作符(+,-,*,/,=)处理函数
void EquResult1_x();			  //按下操作符1/x处理函数
void EquResult_fu();			  //按下操作符+/-处理函数
void Back();					  //按下退格键的操作函数

/*按下数字键(0到9和小数点)的操作处理函数定义*/
void NumResult(char *NumData)
{
	if (numflag == 0) SetWindowText(hEditResult, TEXT(""));		//hEditResult表示文本框
	GetWindowTextA(hEditResult, ss, 1024);	//用ss存储字符串
	strcat_s(ss, NumData);					//字符串加该数字键的字符
	if (strcmp(optflag, "N") == 0)			//optflag表示是否按下操作符键，
											//如没有按过就赋值为“N”,如是按了就赋值为该操作符
											//可用optflag=="N"代替
	{
		strcpy_s(s1, ss);
		SetWindowTextA(hEditResult, s1);
	}
	else
	{
		strcpy_s(s2, ss);
		SetWindowTextA(hEditResult, s2);
	}
	numflag = 1;		//按下了数字键
}

/*按下操作符(+,-,*,/,=)处理函数定义*/
/*这个函数很长很复杂，想了我好久，大致流程在定义中注释*/
void EquResult()
{
	if (*optflag == '/'&& strtod(s2, &stop) == 0)	//1.首先判断是否出现除零错误
	{
		SetWindowTextA(hEditResult, "除零错误！按任意键清零。");
		ss[0] = '\0';								//清零，与清零键C代码相同
		s1[0] = '\0';
		s2[0] = '\0';
		num1 = 0;
		num2 = 0;
		optflag[0] = 'N';
		numflag = 0;
		pointflag = false;
	}
	else {
		if (strcmp(optflag, "N") == 0)					//在不发生除零错误前提下，判断操作键是否按下
		{
			num1 = strtod(s1, &stop);
		}
		else
		{
			switch (*optflag)							//如有操作键按下，判断是什么操作键
			{
			case '+':									//四种操作键流程类似，都是把字符型的Result转换成双精度型
				num1 = strtod(s1, &stop);
				num2 = strtod(s2, &stop);
				num1 = num1 + num2;						//更新num1
				break;
			case '-':
				num1 = strtod(s1, &stop);
				num2 = strtod(s2, &stop);
				num1 = num1 - num2;
				break;
			case '*':
				num1 = strtod(s1, &stop);
				num2 = strtod(s2, &stop);
				num1 = num1*num2;
				break;
			case '/':
				num1 = strtod(s1, &stop);
				num2 = strtod(s2, &stop);
				if (num2 == 0)							//这种情况况最开始已经讨论过，就是除零错误
				{
					break;
				}
				num1 = num1 / num2;
				break;
			}
		}
		std::ostringstream out;
		out << num1;
		strcpy_s(s1, out.str().c_str());				//双精度转化回字符串					
		SetWindowTextA(hEditResult, s1);
		numflag = 0;									//这之后数字键和点操作符键都更新成没按下
		pointflag = false;
	}
}

/*按下操作符1/x处理函数定义*/
void EquResult1_x()
{
	int flag = (div0flag == 0);
	switch (flag)
	{
	case 0:
		SetWindowTextA(hEditResult, "除零错误！按任意键清零。");//除零错误
		ss[0] = '\0';
		s1[0] = '\0';
		s2[0] = '\0';
		num1 = 0;
		num2 = 0;
		optflag[0] = 'N';
		numflag = 0;
		pointflag = false;
		div0flag = 0;
		break;
	default:													//以下一小段出自汉文之手
	{
		if (strcmp(optflag, "N") == 0)					    	//如果操作符未按下，改Result1，否则改Result2
		{
			num1 = strtod(s1, &stop);
			num1 = 1 / num1;
			//int prec = std::numeric_limits::digits10; // 18
			std::ostringstream out;
			//out.precision(prec);//覆盖默认精度
			out << num1;
			//_gcvt_s(s1, num1, sizeof(s1));
			strcpy_s(s1, out.str().c_str());
			SetWindowTextA(hEditResult, s1);
		}
		else
		{
			num2 = strtod(s2, &stop);
			num2 = 1 / num2;
			std::ostringstream out;
			out << num2;
			strcpy_s(s2, out.str().c_str());
			SetWindowTextA(hEditResult, s2);
		}
		numflag = 0;
		pointflag = false;
	}
	}
}

/*按下操作符+/-处理函数定义*/
void EquResult_fu()
{
	if (strcmp(optflag, "N") == 0)
	{
		num1 = strtod(s1, &stop);
		num1 = -1 * num1;
		_gcvt_s(s1, num1, 15);
		SetWindowTextA(hEditResult, s1);
	}
	else
	{
		num2 = strtod(s2, &stop);
		num2 = -1 * num2;
		_gcvt_s(s2, num2, 15);
		SetWindowTextA(hEditResult, s2);
	}
	pointflag = true;
	numflag = 0;
}

/*按下退格键的操作函数定义*/
void Back()
{
	int i = 0;
	if (strcmp(optflag, "N") == 0)
	{
		if (s1[0] != 0)				//这句话修改掉一个中断异常，防止了i减为-1
		{
			while (s1[i] != '\0') i++;
			s1[i - 1] = '\0';
		}
		SetWindowTextA(hEditResult, s1);
	}
	else
	{
		if (s2[0] != 0)
		{
			while (s2[i] != '\0') i++;
			s2[i - 1] = '\0';
		}
		SetWindowTextA(hEditResult, s2);
	}
}

//void KeyDown(HWND hwnd)
//{
//	if (GetAsyncKeyState(VK_BACK)&&!keydown)
//	{
//		Back();
//		keydown = 1;
//	}
//	if (GetAsyncKeyState(48)&& !keydown)
//	{
//		NumResult("0");
//		keydown = 1;
//	}
//
//};

/*主函数*/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int  iCmdShow)
{
	hIns = hInstance;
	/*定义窗口类*/
	WNDCLASS wndclass;
	wndclass.hInstance = hInstance;
	wndclass.style = NULL;
	wndclass.hIcon = NULL;
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.lpfnWndProc = WndProc; //窗口过程
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;

	/*注册窗口*/
	if (!RegisterClass(&wndclass))//将窗口过程与程序关联起来
	{
		MessageBox(NULL, TEXT("注册窗口类失败!"), TEXT("错误"), MB_ICONERROR);
		return 0;
	}

	/*创建窗口*/
	HMENU hMenu;
	HWND hwnd; /*窗口句柄*/
	hMenu = CreateMenu();         //建立菜单
	HMENU h1 = CreatePopupMenu();
	AppendMenu(hMenu, MF_POPUP, (UINT_PTR)h1, TEXT("文件"));
	AppendMenu(h1, MF_STRING, IDM_ABOUT, TEXT("关于"));
	AppendMenu(h1, MF_STRING, IDM_EXIT, TEXT("退出"));
	hwnd = CreateWindow(
		szAppName,				//窗口类名称
		TEXT("绝对自己做的计算器"),			//窗口标题
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX,	//窗口风格
		700,			//初始X坐标
		400,			//初始y坐标
		415,			//Todo:调试
		300,			//Todo:调试
		NULL,		    //父窗口句柄
		hMenu,			//菜单句柄
		hInstance,		//实例句柄
		NULL			//创建参数NULL					
	);

	/*显示窗口*/
	ShowWindow(hwnd, iCmdShow); /*用窗口类中定义的画刷 擦除客户区*/

	/*更新窗口*/
	UpdateWindow(hwnd); /*重绘客户区---向窗口过程发送WM_PAINT消息*/

	/*消息循环*/
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg); //将消息结构返还给windows以进行某些键盘消息的转换
		DispatchMessage(&msg);  //再次将消息回传给系统，由系统调用窗口过程，即将消息发送给窗口过程
	}

	return 0;
}

/*定义窗口过程*/
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	/*计算器框架的建立，贼烦，得算好尺寸*/
	case WM_CREATE:
		/*建立文本框*/
		hEditResult = CreateWindow(TEXT("EDIT"),
			NULL,
			WS_CHILD | WS_VISIBLE | ES_RIGHT | WS_BORDER | ES_READONLY,      //ES_RIGHT文本右对齐，ES_READONLY只读，WS_CHILD子窗口
			0, 10,
			395, 30,                                  //文本框长宽，TMD调了这么长时间
			hwnd,                                     //父窗口句柄
			(HMENU)IDE_RESULT,
			hIns,
			NULL);
		/*建立20个botton*/
		/*10个数字*/
		hButtonOptSum7 = CreateWindow(TEXT("BUTTON"), //建立按钮1
			TEXT("1"),
			WS_CHILD | WS_VISIBLE,
			0, 40,                                   //从哪开始，得计算好
			80, 50,                                   //按钮长宽
			hwnd,
			(HMENU)IDB_NUM1,
			hIns,
			NULL);
		hButtonOptSum8 = CreateWindow(TEXT("BUTTON"),  //建立按钮2
			TEXT("2"),
			WS_CHILD | WS_VISIBLE,
			80, 40,
			80, 50,
			hwnd,
			(HMENU)IDB_NUM2,
			hIns,
			NULL);
		hButtonOptSum9 = CreateWindow(TEXT("BUTTON"),   //建立按钮3
			TEXT("3"),
			WS_CHILD | WS_VISIBLE,
			160, 40,
			80, 50,
			hwnd,
			(HMENU)IDB_NUM3,
			hIns,
			NULL);
		hButtonOptSum4 = CreateWindow(TEXT("BUTTON"),   //建立按钮4
			TEXT("4"),
			WS_CHILD | WS_VISIBLE,
			0, 90,
			80, 50,
			hwnd,
			(HMENU)IDB_NUM4,
			hIns,
			NULL);
		hButtonOptSum5 = CreateWindow(TEXT("BUTTON"),   //建立按钮5
			TEXT("5"),
			WS_CHILD | WS_VISIBLE,
			80, 90,
			80, 50,
			hwnd,
			(HMENU)IDB_NUM5,
			hIns,
			NULL);
		hButtonOptSum6 = CreateWindow(TEXT("BUTTON"),   //建立按钮6
			TEXT("6"),
			WS_CHILD | WS_VISIBLE,
			160, 90,
			80, 50,
			hwnd,
			(HMENU)IDB_NUM6,
			hIns,
			NULL);
		hButtonOptSum1 = CreateWindow(TEXT("BUTTON"),    //建立按钮7
			TEXT("7"),
			WS_CHILD | WS_VISIBLE,
			0, 140,
			80, 50,
			hwnd,
			(HMENU)IDB_NUM7,
			hIns,
			NULL);
		hButtonOptSum2 = CreateWindow(TEXT("BUTTON"),   //建立按钮8
			TEXT("8"),
			WS_CHILD | WS_VISIBLE,
			80, 140,
			80, 50,
			hwnd,
			(HMENU)IDB_NUM8,
			hIns,
			NULL);
		hButtonOptSum3 = CreateWindow(TEXT("BUTTON"),   //建立按钮9
			TEXT("9"),
			WS_CHILD | WS_VISIBLE,
			160, 140,
			80, 50,
			hwnd,
			(HMENU)IDB_NUM9,
			hIns,
			NULL);
		hButtonOptSum0 = CreateWindow(TEXT("BUTTON"),    //建立按钮0
			TEXT("0"),
			WS_CHILD | WS_VISIBLE,
			0, 190,
			80, 50,
			hwnd,
			(HMENU)IDB_NUM0,
			hIns,
			NULL);
		/*10个符号*/
		hButtonOptMin = CreateWindow(TEXT("BUTTON"),  //建立按钮+/-
			TEXT("+/-"),
			WS_CHILD | WS_VISIBLE,
			80, 190,
			80, 50,
			hwnd,
			(HMENU)IDB_OPTMIN,
			hIns,
			NULL);
		hButtonOptPoint = CreateWindow(TEXT("BUTTON"),  //建立按钮.
			TEXT("."),
			WS_CHILD | WS_VISIBLE,
			160, 190,
			80, 50,
			hwnd,
			(HMENU)IDB_OPTPOINT,
			hIns,
			NULL);
		hButtonOptAdd = CreateWindow(TEXT("BUTTON"),    //建立按钮+
			TEXT("+"),
			WS_CHILD | WS_VISIBLE,
			240, 40,
			80, 50,
			hwnd,
			(HMENU)IDB_OPTADD,
			hIns,
			NULL);
		hButtonOptSub = CreateWindow(TEXT("BUTTON"),  //建立按钮-
			TEXT("-"),
			WS_CHILD | WS_VISIBLE,
			240, 90,
			80, 50,
			hwnd,
			(HMENU)IDB_OPTSUB,
			hIns,
			NULL);
		hButtonOptMul = CreateWindow(TEXT("BUTTON"),   //建立按钮*
			TEXT("*"),
			WS_CHILD | WS_VISIBLE,
			240, 140,
			80, 50,
			hwnd,
			(HMENU)IDB_OPTMUL,
			hIns,
			NULL);
		hButtonOptDiv = CreateWindow(TEXT("BUTTON"),   //建立按钮/
			TEXT("/"),
			WS_CHILD | WS_VISIBLE,
			240, 190,
			80, 50,
			hwnd,
			(HMENU)IDB_OPTDIV,
			hIns,
			NULL);
		hButtonOptEqu = CreateWindow(TEXT("BUTTON"),  //建立按钮=
			TEXT("="),
			WS_CHILD | WS_VISIBLE,
			320, 190,
			80, 50,
			hwnd,
			(HMENU)IDB_OPTEQU,
			hIns,
			NULL);
		hButtonCancelEntry = CreateWindow(TEXT("BUTTON"),  //建立按钮C
			TEXT("C"),
			WS_CHILD | WS_VISIBLE,
			320, 40,
			80, 50,
			hwnd,
			(HMENU)IDB_CANCLEENTRY,
			hIns,
			NULL);
		hButtonOptRec = CreateWindow(TEXT("BUTTON"),   //建立按钮1/x
			TEXT("1/x"),
			WS_CHILD | WS_VISIBLE,
			320, 140,
			80, 50,
			hwnd,
			(HMENU)IDB_OPTREC,
			hIns,
			NULL);
		hButtonBack = CreateWindow(TEXT("BUTTON"),   //建立按钮退格
			TEXT("退格"),
			WS_CHILD | WS_VISIBLE,
			320, 90,
			80, 50,
			hwnd,
			(HMENU)IDB_BACK,
			hIns,
			NULL);

		SetWindowText(hEditResult, TEXT(""));
		return 0;

	case WM_KEYUP:
		switch (wParam)
		{
		case 0x30:
			NumResult("0");
			break;
		case 0x31:
			NumResult("1");
			break;
		case 0x32:
			NumResult("2");
			break;
		case 0x33:
			NumResult("3");
			break;
		case 0x34:
			NumResult("4");
			break;
		case 0x35:
			NumResult("5");
			break;
		case 0x36:
			NumResult("6");
			break;
		case 0x37:
			NumResult("7");
			break;
		case 0x38:
			NumResult("8");
			break;
		case 0x39:
			NumResult("9");
			break;
		case 0x08:
			Back();
			break;
		case 0x25:						//左键
		case 0x6B:						//小键盘+键，在我的电脑上无法使用
		case 0xBB:
			EquResult();
			strcpy_s(optflag, "+");     
			break;
		case 0x27:						//右键
		case 0x6D:						//小键盘-键
		case 0x6E:						//小键盘_键
		case 0xBD:
			EquResult();
			strcpy_s(optflag, "-");
			break;
		case 0x26:						//上键
		case 0x6A:						//小键盘*键
			EquResult();
			strcpy_s(optflag, "*");
			break;
		case 0x28:						//下键
		case 0x6F:						//小键盘/键
		case 0xBF:
			EquResult();
			strcpy_s(optflag, "/");
			break;
		case 0x0D:						//Enter键
		case 0x20:						//空格键
			EquResult();		
			strcpy_s(optflag, "N");
			break;
		case 0x2E:						//delete键
			ss[0] = '\0';				//将所有值都初始化
			s1[0] = '\0';
			s2[0] = '\0';
			num1 = 0;
			num2 = 0;
			optflag[0] = 'N';
			numflag = 0;
			pointflag = false;
			SetWindowTextA(hEditResult, s1);
			break;
		case 0xBE:						//.键
			if (pointflag == true) break;  
			NumResult(".");
			numflag = 1;      
			pointflag = true;    
			break;
		}
		return 0;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
			/* 0-9被按*/
		case IDB_NUM0:
			if (numflag == 0)
			{
				div0flag = 1;
				NumResult("0");
				break;
			}   //按0，刚开始时按0按钮不起作用,但记录在div0flag中，除零错误时会用
			div0flag = 0;
			NumResult("0"); 
			break;
		case IDB_NUM1:
			NumResult("1");
			break;	  //按1，后面9个类似
		case IDB_NUM2:
			NumResult("2"); break;
		case IDB_NUM3:
			NumResult("3"); break;
		case IDB_NUM4:
			NumResult("4"); break;
		case IDB_NUM5:
			NumResult("5"); break;
		case IDB_NUM6:
			NumResult("6"); break;
		case IDB_NUM7:
			NumResult("7"); break;
		case IDB_NUM8:
			NumResult("8"); break;
		case IDB_NUM9:
			NumResult("9"); break;

			/*小数点.被按*/
		case IDB_OPTPOINT:
			if (pointflag == true) break;  //如果已按了点号或之前没数字就返回
			NumResult(".");
			numflag = 1;      //按了数字键
			pointflag = true;    //按了点操作符;
			break;

			/*加减乘除被按*/
		case IDB_OPTADD:
			EquResult();
			strcpy_s(optflag, "+");     //设置按了操作符号+
			break;
		case IDB_OPTSUB:
			EquResult();
			strcpy_s(optflag, "-");
			break;
		case IDB_OPTMUL:
			EquResult();
			strcpy_s(optflag, "*");     //设置按了操作符号*
			break;
		case IDB_OPTDIV:
			EquResult();		//算出结果
			strcpy_s(optflag, "/");
			break;

			/* 等于按钮被按*/
		case IDB_OPTEQU:
			EquResult();		//算出等于结果
			strcpy_s(optflag, "N");
			break;

			/*1/x被按*/
		case IDB_OPTREC:
			EquResult1_x();
			break;

			/*+/-号被按*/
		case IDB_OPTMIN:
			EquResult_fu();
			break;

			/*清除键C被按*/
		case IDB_CANCLEENTRY:
			ss[0] = '\0';		//将所有值都初始化
			s1[0] = '\0';
			s2[0] = '\0';
			num1 = 0;
			num2 = 0;
			optflag[0] = 'N';
			numflag = 0;
			pointflag = false;
			SetWindowTextA(hEditResult, s1);
			break;

			/*退格键被按*/
		case IDB_BACK:
			Back();
			break;

		/*菜单：关于和退出*/
		case IDM_ABOUT:
			MessageBox(hwnd, TEXT("16级课设计算器\n制作团队：李仕浩，付靖文\n支持键盘输入！！！\n左键 +  右键 -  上键 *  下键 /  \nEnter键与空格键 =  Back键退格  Delete键清除"), TEXT("关于"), MB_OK);
			break;
		case IDM_EXIT:
			DestroyWindow(hwnd);
			break;
		}
		while(GetFocus()!= hwnd)
			SetFocus(hwnd);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam); //对所有窗口过程没有处理的消息进行默认处理
}
