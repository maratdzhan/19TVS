#include "stdafx.h"
#include "Error.h"
#include "19TVS.h"
#include "SessionInfo.h"
#include "Path_Finder.h"
#include "MainLinker.h"
#include "ExtractingResultFile.h"
#include "KK_KQ_Extracting.h"
#include "GetParam.h"
#include "NDMaking.h"
#include "GetMCUResult.h"
#include "FuelRodsBelongs.h"
#include "resource.h"
#include "AdditionalCalculatings_1.h"
#include "NtCommon.h"

#include <string>
#include <Windows.h>
#include <vector>
using namespace std;




///////////////////////////////////////
/*/ ОПИСАНИЕ ГРАФИКА ЕДИНИЧНОЙ ТВС /*/
HDC hdc;
HINSTANCE hInst_ChildProcGraph1;
TCHAR WinClassGraphName[] = _T("ChildClass");
LRESULT CALLBACK WndGraph(HWND, UINT, WPARAM, LPARAM);
HWND hGraph;
HWND hWndP;
#include <xstring>
typedef std::basic_string<TCHAR, std::char_traits<TCHAR>,
	std::allocator<TCHAR> > String;


ATOM RegisterGraphClass()
{
	WNDCLASSEX wcgraph = { 0 };
	wcgraph.cbSize = sizeof(WNDCLASSEX);
	wcgraph.style = CS_HREDRAW | CS_VREDRAW;
	wcgraph.lpfnWndProc = WndGraph;
	wcgraph.hInstance = hInst_ChildProcGraph1;
	wcgraph.hCursor = LoadCursor(NULL, IDC_CROSS);
	wcgraph.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcgraph.lpszClassName = WinClassGraphName;
	wcgraph.hIconSm = LoadIcon(hInst_ChildProcGraph1, MAKEINTRESOURCE(IDI_ICON1));
	return RegisterClassEx(&wcgraph);

}
static int n, kx, ky, sx = 600, sy = 800;
static String str;
vector<short> stateNumber;
//////////////////////////////////////
// ВСЕ ПАРАМЕТРЫ ИЗВЛЕКАЮТСЯ ЛОКАЛЬНО - ЭТО ПЛОХО, НУЖЕН ВЫВОД НА ГЛОБАЛЬНЫЙ УРОВЕНЬ
void ClearingFunction();

int GetSessionInfo(HWND hWnd_1)
{
	LoggerClass lg(__FUNCTION__);

	string s;
	
	int ndNumber = 0;
	int gsNumber = 0;
	int cycle = 0;
	short complexity = false;
	s = ReturnMainPath("sltifs"); 


	ifstream slt(s);


	
	//////////////////
	// Not in cycle
	
	slRead();
	FirstLaunchNumeration();
	FakeTvelDef(false);
//	newMcuMatrixNumeration(); // берет файлы и делает новый массив соответветствий
	SotvLinkMcu();
	//////////////////
	

	do
	{
		getline(slt, s);
		if (!s.empty())
		{

			
			if (ReturnTableParam(1, 0, 0, 0))
				GetTableParam(1, 1, 0, stateNumber[(stateNumber.size() - 1)]);

			complexity = GetParam(s, 2);
			if (complexity)
				stateNumber.push_back(GetParam(s,3));

			stateNumber.push_back(GetParam(s, 1));

			cycle = ndMaking(stateNumber[(stateNumber.size() - 1)], 2);
			// Номер файла newData
			ndNumber = ndMaking(stateNumber[(stateNumber.size()-1)], 3);
			// Переопределение complexity для того, чтобы 
			//complexity = 0;
			////////////////////////////////////
			// Запуск записи файла newdata:
			short sostNumber = ndMaking(stateNumber[(stateNumber.size() - 1)], 5);
			for (unsigned short i = 0; i < stateNumber.size(); i++)
			{
				// Номер вариант зазоров
				gsNumber = ndMaking(stateNumber[i], 4);

				DefineKinf(cycle,sostNumber);
				GetStateName(stateNumber[i]);
				GetGapSize(stateNumber[i], gsNumber);
				

				// Запись в файл? или в временный массив
				bool complex = false;
				if ((stateNumber.size() - 1) == i)
				{
					complex = true;
					ndWritting(ndNumber);
				}
				short error_matrix_permpar = MainLinker(stateNumber[i], complex, i);

				if (error_matrix_permpar != 0)
					printf("Error");
				
				/*  
				и в начале цикла вместо St_val => 0;
				if ((stateNumber.size() - 1) == i)
				{
					complex = true;
					ndWritting(ndNumber);
				}
				MainLinker(stateNumber[i], complex, i);
				////*/
			}
			// Запуск PERMAK-A
			system("PERMAK2007I.exe perm");
			// Номер извлекаемого состояния
			ExtractingResultFile(sostNumber, stateNumber[(stateNumber.size() - 1)]);
			KK_KQ_Extracting(stateNumber[(stateNumber.size() - 1)]);
			GetMcuResult(stateNumber[(stateNumber.size() - 1)], ResultPathForMcu("", 1));
			
			
			KrCalculatings(stateNumber[(stateNumber.size() - 1)]);

			RegisterGraphClass();
			hGraph = CreateWindow(WinClassGraphName, _T("Картограмма кассеты"),
				WS_SYSMENU | WS_POPUP | WS_VISIBLE | WS_THICKFRAME | WS_CAPTION,
				sx / 6, sy / 10, sx, sy, hWndP, 0, hInst_ChildProcGraph1, NULL);
			UpdateWindow(hGraph);
			DestroyWindow(hGraph);	
			GetKQKeff(false, false, false, true, 1, 0);
			ClearLinkerFunction();
			Clearing_Temporary();
			stateNumber.clear();
		}
		/*
		Создается поток номер два
		Поиск и обработка файлов MCU
		Проведение сопоставлений
		*/

		/*
		Основной поток:
		Переход к новому состоянию
		*****
		*/
	} while (!slt.eof());

	WriteKeff();
	WriteKq(false);
	slt.close();
	s.clear();
	FakeTvelDef(true);
	ClearingFunction();

//	MessageBox(NULL,L"Расчет окончен",L"Информация",MB_ICONINFORMATION|MB_OK);

	return 0;
}

void ClearingFunction()
{
	ClearingInitial();
	ClearGaps();
	ClearPermakERs();
}
