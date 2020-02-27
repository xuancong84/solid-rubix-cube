#include "Base.h"
#include "graphEngine.h"

ID3DXFont* Font = 0;



void CreateFont(IDirect3DDevice9 *Device)
{
	D3DXFONT_DESCA lf;
	ZeroMemory(&lf, sizeof(D3DXFONT_DESCA));

	lf.Height         = 20;    // in logical units
	lf.Width          = 8;    // in logical units       
	lf.Weight         = 500;   // boldness, range 0(light) - 1000(bold)
	lf.Italic         = false;     
	lf.CharSet        = DEFAULT_CHARSET;
	lf.Quality        = 0;
	lf.PitchAndFamily = 0;
	strcpy(lf.FaceName, "Times New Roman"); // font style

	//
	// Create an ID3DXFont based on 'lf'.
	//

	if(FAILED(D3DXCreateFontIndirect(Device, &lf, &Font)))
	{
		::MessageBox(0, "D3DXCreateFontIndirect() - FAILED", 0, 0);
		::PostQuitMessage(0);
	}
}

void TextOut(const char* text, int x, int y)
{
	graphEngine.SetDrawMode(2);
	RECT rect = {x, y, WIDTH, HEIGHT};
	Font->DrawText(
		base::Sprite,
		text, 
		-1, // size of string or -1 indicates null terminating string
		&rect,            // rectangle text is to be formatted to in windows coords
		DT_TOP | DT_LEFT, // draw in the top left corner of the viewport
		D3DCOLOR_XRGB(255,255,0));      // black text
}

void printWinTitle( const char *fmt, ... )
{
	char str[256];
	va_list va;
	va_start(va,fmt);
	vsprintf(str,fmt,va);
	va_end(va);
	SetWindowText(base::hwnd,str);
}

void base::BaseSet()
{
}

void base::Start(IDirect3DDevice9 *Device)
{
	pRubixWindow = new RubixWindow();
	pRubixWindow->Init(hwnd, Device, &hDlg);

	CreateFont(Device);
}

void base::Update(double dwTime)
{
	pRubixWindow->Update(dwTime*base::InvMilliSecond);
}

string msgText;
int px,py;
LONGLONG msgOutTime = 0;
void base::ShowMessage(string text, double timeInSecond, int x, int y)
{
	msgText = text, px=x, py=y;
	msgOutTime = (timeInSecond==0)?0:(currTime+timeInSecond*SECOND);
}

void base::PrintMessage(const char *fmt, ...)
{
	char str[1024];
	va_list va;
	va_start(va,fmt);
	vsnprintf(str, 1024, fmt, va);
	va_end(va);
	msgText = str;
	msgOutTime = currTime+SECOND;
}

float points[]={
	100, 20, 0, 140, 20, 0,
	100, 30, 0, 140, 30, 0,
	100, 40, 0, 140, 40, 0,
	100, 50, 0, 140, 50, 0,
	100, 60, 0, 140, 60, 0,
	100, 70, 0, 140, 70, 0,
	100, 80, 0, 140, 80, 0,
	100, 90, 0, 140, 90, 0,
};
void base::Draw(IDirect3DDevice9 *Device)
{
	if(pRubixWindow){
		graphEngine.SetDrawMode(1);
		pRubixWindow->Draw();
	}

	// Draw FPS
	if(systemIni.showFPS)
	{
		char s[256];
		sprintf(s,StringTable(71).c_str(),
			(float)(base::SECOND/base::meanTimeInterval),
			(float)(base::SECOND*base::totalFramesDrawn/(base::currTime-base::beginTime))
		);
		if(systemIni.showFPS==1)
			SetWindowText(hwnd,s);
		else
			TextOut(s,0,0);
	}

	// Draw short message
	if(msgOutTime>currTime || msgOutTime==0)
		TextOut(msgText.c_str(),px,py);
}

void base::DeleteAll()
{
}

void base::OnMouseEvent(int xPos, int yPos, UINT msg, WPARAM wParam)
{
}

void base::OnKeyEvent( UINT nChar, bool bKeyDown )
{
}
