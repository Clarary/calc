/*���������  �����ߣ����˺�*/
/*�Դ��ڵ����������£�*/
/*1.һ��ʼʱʱ�����������㣬����Ӱ�����*/
/*2.ÿ����һ������������ʱ�����һ��С���㣬������*/
/*3.�����������̫��̬�����ܳ���double��������ķ�Χ�;��ȣ���Ӧ�ò������񾭲����ҵļ������������°�*/
/*4.�������̫���ῴ����ǰ���*/
/*��ɫ��*/
/*1.������ʶ��������*/
/*2.��ʵ��+,-,*,/����������,���簴1+2+=�õ�5*/
/*3.�ϸ���߼���飺�����롰1.2.3��ʱ��.3������������*/
/*4.�����������ʱ������Ľ�ǰ��ĸ��ǣ���3+*-1���õ�2*/
/*����������Ҫ���������ˣ�֧�ּ������룡����*/
/* ��� +  �Ҽ� -  �ϼ� *  �¼� /  */
/*Enter����ո�� =  Back���˸� Delete�����*/

#include<windows.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include<string>
#include"resource.h"
#include <sstream>

/*�ڴ˶��崰���õı���������*/
static HINSTANCE hIns;
static TCHAR szAppName[] = TEXT("Calculator");
HWND hEditResult, hButtonOptSum0, hButtonOptSum1, hButtonOptSum2, hButtonOptSum3, hButtonOptSum4, hButtonOptSum5, hButtonOptSum6, hButtonOptSum7,
hButtonOptSum8, hButtonOptSum9;
HWND hButtonOptSumDec, hButtonOptAdd, hButtonOptSub, hButtonOptMul, hButtonOptDiv,
hButtonOptMin, hButtonOptPoint, hButtonOptEqu, hButtonOptRec, hButtonCancelEntry, hButtonBack;   //������Ⱥ�Ӵ���
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);				//�������ڹ��̺���

/*�ڴ˶��������õı���������*/
char ss[1024] = "";
char s1[1024] = "";
char s2[1024] = "";
double num1 = 0, num2 = 0;
char optflag[] = "N";		      //�ж��Ƿ��˲��������������������
char *stop;
int numflag = 0;				  //�ж��Ƿ������ּ�
int div0flag = 0;				  //�������Ҫ�õ�flag
bool pointflag = false;			  //�ж��Ƿ��˵������
void NumResult(char *NumData);    //�������ּ�(0��9��С����)�Ĳ���������
void EquResult();                 //���²�����(+,-,*,/,=)������
void EquResult1_x();			  //���²�����1/x������
void EquResult_fu();			  //���²�����+/-������
void Back();					  //�����˸���Ĳ�������

/*�������ּ�(0��9��С����)�Ĳ�������������*/
void NumResult(char *NumData)
{
	if (numflag == 0) SetWindowText(hEditResult, TEXT(""));		//hEditResult��ʾ�ı���
	GetWindowTextA(hEditResult, ss, 1024);	//��ss�洢�ַ���
	strcat_s(ss, NumData);					//�ַ����Ӹ����ּ����ַ�
	if (strcmp(optflag, "N") == 0)			//optflag��ʾ�Ƿ��²���������
											//��û�а����͸�ֵΪ��N��,���ǰ��˾͸�ֵΪ�ò�����
											//����optflag=="N"����
	{
		strcpy_s(s1, ss);
		SetWindowTextA(hEditResult, s1);
	}
	else
	{
		strcpy_s(s2, ss);
		SetWindowTextA(hEditResult, s2);
	}
	numflag = 1;		//���������ּ�
}

/*���²�����(+,-,*,/,=)����������*/
/*��������ܳ��ܸ��ӣ������Һþã����������ڶ�����ע��*/
void EquResult()
{
	if (*optflag == '/'&& strtod(s2, &stop) == 0)	//1.�����ж��Ƿ���ֳ������
	{
		SetWindowTextA(hEditResult, "������󣡰���������㡣");
		ss[0] = '\0';								//���㣬�������C������ͬ
		s1[0] = '\0';
		s2[0] = '\0';
		num1 = 0;
		num2 = 0;
		optflag[0] = 'N';
		numflag = 0;
		pointflag = false;
	}
	else {
		if (strcmp(optflag, "N") == 0)					//�ڲ������������ǰ���£��жϲ������Ƿ���
		{
			num1 = strtod(s1, &stop);
		}
		else
		{
			switch (*optflag)							//���в��������£��ж���ʲô������
			{
			case '+':									//���ֲ������������ƣ����ǰ��ַ��͵�Resultת����˫������
				num1 = strtod(s1, &stop);
				num2 = strtod(s2, &stop);
				num1 = num1 + num2;						//����num1
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
				if (num2 == 0)							//����������ʼ�Ѿ����۹������ǳ������
				{
					break;
				}
				num1 = num1 / num2;
				break;
			}
		}
		std::ostringstream out;
		out << num1;
		strcpy_s(s1, out.str().c_str());				//˫����ת�����ַ���					
		SetWindowTextA(hEditResult, s1);
		numflag = 0;									//��֮�����ּ��͵�������������³�û����
		pointflag = false;
	}
}

/*���²�����1/x����������*/
void EquResult1_x()
{
	int flag = (div0flag == 0);
	switch (flag)
	{
	case 0:
		SetWindowTextA(hEditResult, "������󣡰���������㡣");//�������
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
	default:													//����һС�γ��Ժ���֮��
	{
		if (strcmp(optflag, "N") == 0)					    	//���������δ���£���Result1�������Result2
		{
			num1 = strtod(s1, &stop);
			num1 = 1 / num1;
			//int prec = std::numeric_limits::digits10; // 18
			std::ostringstream out;
			//out.precision(prec);//����Ĭ�Ͼ���
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

/*���²�����+/-����������*/
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

/*�����˸���Ĳ�����������*/
void Back()
{
	int i = 0;
	if (strcmp(optflag, "N") == 0)
	{
		if (s1[0] != 0)				//��仰�޸ĵ�һ���ж��쳣����ֹ��i��Ϊ-1
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

/*������*/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int  iCmdShow)
{
	hIns = hInstance;
	/*���崰����*/
	WNDCLASS wndclass;
	wndclass.hInstance = hInstance;
	wndclass.style = NULL;
	wndclass.hIcon = NULL;
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.lpfnWndProc = WndProc; //���ڹ���
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;

	/*ע�ᴰ��*/
	if (!RegisterClass(&wndclass))//�����ڹ���������������
	{
		MessageBox(NULL, TEXT("ע�ᴰ����ʧ��!"), TEXT("����"), MB_ICONERROR);
		return 0;
	}

	/*��������*/
	HMENU hMenu;
	HWND hwnd; /*���ھ��*/
	hMenu = CreateMenu();         //�����˵�
	HMENU h1 = CreatePopupMenu();
	AppendMenu(hMenu, MF_POPUP, (UINT_PTR)h1, TEXT("�ļ�"));
	AppendMenu(h1, MF_STRING, IDM_ABOUT, TEXT("����"));
	AppendMenu(h1, MF_STRING, IDM_EXIT, TEXT("�˳�"));
	hwnd = CreateWindow(
		szAppName,				//����������
		TEXT("�����Լ����ļ�����"),			//���ڱ���
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX,	//���ڷ��
		700,			//��ʼX����
		400,			//��ʼy����
		415,			//Todo:����
		300,			//Todo:����
		NULL,		    //�����ھ��
		hMenu,			//�˵����
		hInstance,		//ʵ�����
		NULL			//��������NULL					
	);

	/*��ʾ����*/
	ShowWindow(hwnd, iCmdShow); /*�ô������ж���Ļ�ˢ �����ͻ���*/

	/*���´���*/
	UpdateWindow(hwnd); /*�ػ�ͻ���---�򴰿ڹ��̷���WM_PAINT��Ϣ*/

	/*��Ϣѭ��*/
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg); //����Ϣ�ṹ������windows�Խ���ĳЩ������Ϣ��ת��
		DispatchMessage(&msg);  //�ٴν���Ϣ�ش���ϵͳ����ϵͳ���ô��ڹ��̣�������Ϣ���͸����ڹ���
	}

	return 0;
}

/*���崰�ڹ���*/
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	/*��������ܵĽ���������������óߴ�*/
	case WM_CREATE:
		/*�����ı���*/
		hEditResult = CreateWindow(TEXT("EDIT"),
			NULL,
			WS_CHILD | WS_VISIBLE | ES_RIGHT | WS_BORDER | ES_READONLY,      //ES_RIGHT�ı��Ҷ��룬ES_READONLYֻ����WS_CHILD�Ӵ���
			0, 10,
			395, 30,                                  //�ı��򳤿�TMD������ô��ʱ��
			hwnd,                                     //�����ھ��
			(HMENU)IDE_RESULT,
			hIns,
			NULL);
		/*����20��botton*/
		/*10������*/
		hButtonOptSum7 = CreateWindow(TEXT("BUTTON"), //������ť1
			TEXT("1"),
			WS_CHILD | WS_VISIBLE,
			0, 40,                                   //���Ŀ�ʼ���ü����
			80, 50,                                   //��ť����
			hwnd,
			(HMENU)IDB_NUM1,
			hIns,
			NULL);
		hButtonOptSum8 = CreateWindow(TEXT("BUTTON"),  //������ť2
			TEXT("2"),
			WS_CHILD | WS_VISIBLE,
			80, 40,
			80, 50,
			hwnd,
			(HMENU)IDB_NUM2,
			hIns,
			NULL);
		hButtonOptSum9 = CreateWindow(TEXT("BUTTON"),   //������ť3
			TEXT("3"),
			WS_CHILD | WS_VISIBLE,
			160, 40,
			80, 50,
			hwnd,
			(HMENU)IDB_NUM3,
			hIns,
			NULL);
		hButtonOptSum4 = CreateWindow(TEXT("BUTTON"),   //������ť4
			TEXT("4"),
			WS_CHILD | WS_VISIBLE,
			0, 90,
			80, 50,
			hwnd,
			(HMENU)IDB_NUM4,
			hIns,
			NULL);
		hButtonOptSum5 = CreateWindow(TEXT("BUTTON"),   //������ť5
			TEXT("5"),
			WS_CHILD | WS_VISIBLE,
			80, 90,
			80, 50,
			hwnd,
			(HMENU)IDB_NUM5,
			hIns,
			NULL);
		hButtonOptSum6 = CreateWindow(TEXT("BUTTON"),   //������ť6
			TEXT("6"),
			WS_CHILD | WS_VISIBLE,
			160, 90,
			80, 50,
			hwnd,
			(HMENU)IDB_NUM6,
			hIns,
			NULL);
		hButtonOptSum1 = CreateWindow(TEXT("BUTTON"),    //������ť7
			TEXT("7"),
			WS_CHILD | WS_VISIBLE,
			0, 140,
			80, 50,
			hwnd,
			(HMENU)IDB_NUM7,
			hIns,
			NULL);
		hButtonOptSum2 = CreateWindow(TEXT("BUTTON"),   //������ť8
			TEXT("8"),
			WS_CHILD | WS_VISIBLE,
			80, 140,
			80, 50,
			hwnd,
			(HMENU)IDB_NUM8,
			hIns,
			NULL);
		hButtonOptSum3 = CreateWindow(TEXT("BUTTON"),   //������ť9
			TEXT("9"),
			WS_CHILD | WS_VISIBLE,
			160, 140,
			80, 50,
			hwnd,
			(HMENU)IDB_NUM9,
			hIns,
			NULL);
		hButtonOptSum0 = CreateWindow(TEXT("BUTTON"),    //������ť0
			TEXT("0"),
			WS_CHILD | WS_VISIBLE,
			0, 190,
			80, 50,
			hwnd,
			(HMENU)IDB_NUM0,
			hIns,
			NULL);
		/*10������*/
		hButtonOptMin = CreateWindow(TEXT("BUTTON"),  //������ť+/-
			TEXT("+/-"),
			WS_CHILD | WS_VISIBLE,
			80, 190,
			80, 50,
			hwnd,
			(HMENU)IDB_OPTMIN,
			hIns,
			NULL);
		hButtonOptPoint = CreateWindow(TEXT("BUTTON"),  //������ť.
			TEXT("."),
			WS_CHILD | WS_VISIBLE,
			160, 190,
			80, 50,
			hwnd,
			(HMENU)IDB_OPTPOINT,
			hIns,
			NULL);
		hButtonOptAdd = CreateWindow(TEXT("BUTTON"),    //������ť+
			TEXT("+"),
			WS_CHILD | WS_VISIBLE,
			240, 40,
			80, 50,
			hwnd,
			(HMENU)IDB_OPTADD,
			hIns,
			NULL);
		hButtonOptSub = CreateWindow(TEXT("BUTTON"),  //������ť-
			TEXT("-"),
			WS_CHILD | WS_VISIBLE,
			240, 90,
			80, 50,
			hwnd,
			(HMENU)IDB_OPTSUB,
			hIns,
			NULL);
		hButtonOptMul = CreateWindow(TEXT("BUTTON"),   //������ť*
			TEXT("*"),
			WS_CHILD | WS_VISIBLE,
			240, 140,
			80, 50,
			hwnd,
			(HMENU)IDB_OPTMUL,
			hIns,
			NULL);
		hButtonOptDiv = CreateWindow(TEXT("BUTTON"),   //������ť/
			TEXT("/"),
			WS_CHILD | WS_VISIBLE,
			240, 190,
			80, 50,
			hwnd,
			(HMENU)IDB_OPTDIV,
			hIns,
			NULL);
		hButtonOptEqu = CreateWindow(TEXT("BUTTON"),  //������ť=
			TEXT("="),
			WS_CHILD | WS_VISIBLE,
			320, 190,
			80, 50,
			hwnd,
			(HMENU)IDB_OPTEQU,
			hIns,
			NULL);
		hButtonCancelEntry = CreateWindow(TEXT("BUTTON"),  //������ťC
			TEXT("C"),
			WS_CHILD | WS_VISIBLE,
			320, 40,
			80, 50,
			hwnd,
			(HMENU)IDB_CANCLEENTRY,
			hIns,
			NULL);
		hButtonOptRec = CreateWindow(TEXT("BUTTON"),   //������ť1/x
			TEXT("1/x"),
			WS_CHILD | WS_VISIBLE,
			320, 140,
			80, 50,
			hwnd,
			(HMENU)IDB_OPTREC,
			hIns,
			NULL);
		hButtonBack = CreateWindow(TEXT("BUTTON"),   //������ť�˸�
			TEXT("�˸�"),
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
		case 0x25:						//���
		case 0x6B:						//С����+�������ҵĵ������޷�ʹ��
		case 0xBB:
			EquResult();
			strcpy_s(optflag, "+");     
			break;
		case 0x27:						//�Ҽ�
		case 0x6D:						//С����-��
		case 0x6E:						//С����_��
		case 0xBD:
			EquResult();
			strcpy_s(optflag, "-");
			break;
		case 0x26:						//�ϼ�
		case 0x6A:						//С����*��
			EquResult();
			strcpy_s(optflag, "*");
			break;
		case 0x28:						//�¼�
		case 0x6F:						//С����/��
		case 0xBF:
			EquResult();
			strcpy_s(optflag, "/");
			break;
		case 0x0D:						//Enter��
		case 0x20:						//�ո��
			EquResult();		
			strcpy_s(optflag, "N");
			break;
		case 0x2E:						//delete��
			ss[0] = '\0';				//������ֵ����ʼ��
			s1[0] = '\0';
			s2[0] = '\0';
			num1 = 0;
			num2 = 0;
			optflag[0] = 'N';
			numflag = 0;
			pointflag = false;
			SetWindowTextA(hEditResult, s1);
			break;
		case 0xBE:						//.��
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
			/* 0-9����*/
		case IDB_NUM0:
			if (numflag == 0)
			{
				div0flag = 1;
				NumResult("0");
				break;
			}   //��0���տ�ʼʱ��0��ť��������,����¼��div0flag�У��������ʱ����
			div0flag = 0;
			NumResult("0"); 
			break;
		case IDB_NUM1:
			NumResult("1");
			break;	  //��1������9������
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

			/*С����.����*/
		case IDB_OPTPOINT:
			if (pointflag == true) break;  //����Ѱ��˵�Ż�֮ǰû���־ͷ���
			NumResult(".");
			numflag = 1;      //�������ּ�
			pointflag = true;    //���˵������;
			break;

			/*�Ӽ��˳�����*/
		case IDB_OPTADD:
			EquResult();
			strcpy_s(optflag, "+");     //���ð��˲�������+
			break;
		case IDB_OPTSUB:
			EquResult();
			strcpy_s(optflag, "-");
			break;
		case IDB_OPTMUL:
			EquResult();
			strcpy_s(optflag, "*");     //���ð��˲�������*
			break;
		case IDB_OPTDIV:
			EquResult();		//������
			strcpy_s(optflag, "/");
			break;

			/* ���ڰ�ť����*/
		case IDB_OPTEQU:
			EquResult();		//������ڽ��
			strcpy_s(optflag, "N");
			break;

			/*1/x����*/
		case IDB_OPTREC:
			EquResult1_x();
			break;

			/*+/-�ű���*/
		case IDB_OPTMIN:
			EquResult_fu();
			break;

			/*�����C����*/
		case IDB_CANCLEENTRY:
			ss[0] = '\0';		//������ֵ����ʼ��
			s1[0] = '\0';
			s2[0] = '\0';
			num1 = 0;
			num2 = 0;
			optflag[0] = 'N';
			numflag = 0;
			pointflag = false;
			SetWindowTextA(hEditResult, s1);
			break;

			/*�˸������*/
		case IDB_BACK:
			Back();
			break;

		/*�˵������ں��˳�*/
		case IDM_ABOUT:
			MessageBox(hwnd, TEXT("16�����������\n�����Ŷӣ����˺ƣ�������\n֧�ּ������룡����\n��� +  �Ҽ� -  �ϼ� *  �¼� /  \nEnter����ո�� =  Back���˸�  Delete�����"), TEXT("����"), MB_OK);
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
	return DefWindowProc(hwnd, uMsg, wParam, lParam); //�����д��ڹ���û�д������Ϣ����Ĭ�ϴ���
}
