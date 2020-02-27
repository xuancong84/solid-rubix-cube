#include <string.h>
#include <fstream>
#include <sstream>

#include "defines.h"
#include "Base.h"
#include "resource.hpp"

#pragma warning (disable : 4996)

SystemIni systemIni;

// This is the standard 16:9 aspect ratio, 480x272 is not exactly 16:9
// Window sizes: 0-480*270 1-720*405 2-1024*576 3-Fullscreen 4-Custom
const int g_windowSizesN = 3;
int g_windowSizes[g_windowSizesN+1][2] = {{480,270},{720,405},{1024,576},{0,0}};
RECT g_defaultRect = {0,0,480,272};

// Language specific texts
const int g_numLanguages = 2;
int getMaxSkin();
string g_StringTable[MAX_STRING_INDEX][g_numLanguages]={
	{"����ϰ�ߣ�", "Keyboard Style:"},												//0
	{"���ڴ�С��", "Window Size:"},
	{"��Ч������", "Volume:"},
	{"û�м�⵽\n���������Ϸ\n�޷�����", "Cannot locate\ngame file\nCannot play"},
	{"����\n���������Ϸ", "Choose songs to play, preview PV or Demo\n"},
	{"����Diva Online Map Wiki\n�ϲ������µĸ���׷�Ӱ�\n�����ת��Wiki",			//5
	"Search for newest version song packages\nat Diva Online Map Wiki\nClick to goto the Wiki page"},
	{"��Diva OL�������Ժ������ϵ�\n����ڸ���ģʽ��һ����Ϸ\n�����ϸ��Ϣ",
	"In Diva OL, you can play with\nother players through the Internet\nClick for details"},
	{"����������úʹ��ڴ�С\n�Լ���Ч������", "Config keyboard layout,\nwindow size and sound volume"},
	{"���Լ���\n��Ϸ�Ĳ廭��Ӱ��", "View Miku's pictures and\nphoto album"},
	{"��ʾ��Ϸ����\nhttp://www.gamemastercn.com\n��ԭ���زİ�Ȩ��SEGA����",
	 "Show Game Manual\nhttp://www.gamemastercn.com\nCopyright belongs to SEGA"},
	{"������Ϸ", "Quit Game"},														//10
	{"���Ե����Ĵ��ڴ�С��", "Available window sizes:"},
	{"\n���Ը���\n��Ч��������������\n��<Enter>����������", "\nChange the volume\nof sound effects\nPress <Enter> to hear hit sound"},
	{"\n\n�ָ��������õ�Ĭ��", "\n\nReset all settings to default"},
	{"��ͣ", "PAUSE"},
	{"����", "CONTINUE"},															//15
	{"����", "RESTART"},
	{"���ز˵�", "BACK TO MENU"},
	{"û�и���", "List is Empty"},
	{"����ģʽ", "Custom Game"},
	{"����ģʽ��û�и�����", "Custom Game (empty list)"},								//20
	{"���ľ���ģʽ��δ������", "Career Mode (not available)"},
	{"�����սģʽ��δ������", "Multiplayer Mode (not available)"},
	{"����", "Settings"},
	{"CG����", "CG Preview"},
	{"��Ϸ����", "Game Help"},												//25
	{"�˳���Ϸ", "Exit Game"},
	{"�˵�", "MENU"},
	{"����", "Author"},
	{"����", "Easy"},
	{"��ͨ", "Normal"},																//30
	{"����", "Hard"},
	{"����", "Extra"},
	{"����", "Die"},
	{"��", "Star"},
	{"�Ѷȷ���:%s\n�Ѷ��Ǽ�:%d��\n\n  ��߷�:%.7d\n�������:%.4d\n�������:%s\nͨ�ش���:%d",	//35
	"Difficulty:%s\nStar Level:%d Star\n\nBest score:%.7d\nCombo Max:%.4d\nBest result:%s\n# Times Passed:%d"},
	{"��ȫ����", "(Full Screen)"},
	{"����", "RETRY"},
	{"����", "RETURN"},
	{"�ָ�Ĭ������", "Reset to Default"},
	{"", ""},																				//40
	{"���ԣ�", "Language:"},
	{"����", "English"},
	{"����Ч����", "Particle Effect:"},
	{"\n\n������ʾ���ԣ�\n����\nӢ��", "\n\nChange displayed language:\nChinese\nEnglish"},
	{"��������Ч����\n0: ������Ч������죩\n1: ��β���죩\n2: ˫β����ͨ��\n3: ����β�ͣ�������",	//45
	"Change particle system:\n0: no particle (fastest)\n1: single tail (fast)\n2: double tail (normal)\n3: comet tail (slowest)"},
	//����������������
	{"��<Enter>����ʼ����\n\n��ͬʱ���¶������\n�������Ϊͬһ����", "Press <Enter> to change\n\n[Holding down multiple keys\nto map multiple keys\nto the same key]"},
	{"����", "Times New Roman"},
	{"����", "Courier New"},
	{"����", "Arial"},
	{"���ޣ�", "N/A"},																		//50
	{"ʧ��", "Misstake"},
	{"�ϸ�", "Cheap"},
	{"һ��", "Standard"},
	{"����", "Great"},
	{"����", "Perfect"},																	//55
	{"����", "Times New Roman"},	//��������Ͽ���û������
	{"�ر�", "OFF"},
	{"��", "LOW"},
	{"��", "MEDIUM"},
	{"��", "HIGH"},																			//60
	{"ȫ�ּ��̹�סʧ�ܣ��޷���׼ȷ��ʱ�Ĳ�����̡����з���ǽ������������",
	"Global keyboard hook failed, high-precision keyboard input capture is not available.\r\n"
	"If a firewall notice has pop up, please allow!"},
	{"��������", "Key Capture"},
	{"��ʾ֡Ƶ","Show FPS"},
	{"������", "Enabled"},
	{"�ѽ���", "Disabled"},																	//65
	{"����ʾ", "OFF"},
	{"��ʾ", "ON"},
	{"������ʾ", "ON(OSD)"},
	{"��ʾÿ����֡��\n1.����ʾ\n2.���ڱ�����ʾ\n3.������ʾ",
	"Show FPS\n(Frames per second)\n1. Don't show\n2. Show on window title\n3. On-screen display"},
	{"\n\n0: ��ͨ���� WM_KEYDOWN\n1: ȫ�ּ��̹� WH_KEYBOARD_LL\n2: ʹ�� DirectInput",
	"\n\n0: Normal key input WM_KEYDOWN\n1: Global Keyhook WH_KEYBOARD_LL\n2: Use DirectInput"},		//70
	{"��ǰFPS=%.2f, ƽ��FPS=%.2f", "CurrFPS=%.2f, AvgFPS=%.2f"},
	{"����ʧ��", "Failed"},
	{"�Զ���ͣ", "Auto Pause"},
	{"�Ӳ�", "Never"},
	{"��ʱ", "PlayOnly"},																	//75
	{"����", "Always"},
	{"�Զ���ͣ��ʽ��\n1. �Ӳ���ͣ\n2. ֻ����Ϸģʽ����ͣ\n3. ������ͣ",
	"Auto pause method:\n1. Never pause\n2. In play-mode only\n3. Alway pause"},
	{"��Ƶ����","Video Quality"},
	{"������MP4��ƵתΪ����AVI\n�Խ�ʡCPU��Դ\n0: ����MP4 (��ת��)\n1: �����AVI\n9: �����AVI",
	"Convert video from MP4 to AVI\nto save CPU\n0: MP4 (no conversion)\n1: highest quality AVI\n9: lowest quality AVI"},
	{"��Ϸ����","Game UI"},																						//80
	{"\nѡ�����Ƥ��:\n�鿴 gamedata*.txt\n��ǰ���Χ��1��"+ToString(getMaxSkin())+"\n��<CapsLock>Ԥ��",
	"\nChoose UI skin:\nSee gamedata*.txt\nCurrent Range: 1~"+ToString(getMaxSkin())+"\nTurn on <CapsLock> to preview"},
	{"AVI �ֱ��ʣ�\n1��3��1024��576\n4��6��640��360\n7��9��480��270",
	"AVI Resolution��\n1~3��1024x576\n4~6��640x360\n7~9��480x270"},
	{"�Զ���", "Custom"},
	{"�밴˳�����ΰ��¡���������������",
		"Please press ���������������� in order"},
	{"��<ESC>��ȡ����<Backspace>�ص�ǰһ��", "Press <ESC> to cancel, <Backspace> to previous key"}, //85
	{"��<Enter>��ȷ������<ESC>��ȡ��", "Press <Enter> to confirm, press <ESC> to cancel"},
	{"����", "Font"},
	{"\n�ı�������壺\n����\n����\n����", "\nChange UI Font:\nTimes New Roman\nCourier New\nArial"},
	{"�ո�", "Space"},
	{"��", " or "},			// 90
	{"�����", "Antialiasing"},
	{"�����ģʽ��������\n0��(��)�����㻭(������)\n1��(��)Ӳ�����(���)\n...\nN��(��)Ӳ�����(����)\n\n��ǰѡ�\n",
	"Anti-aliasing mode:\n0: (OFF) software draw strip (SLOWEST)\n1: (ON) hardware lowest (fastest)\n...\nN: (ON) hardware highest (slowest)\n\nCurrent option:\n"},
	{"�ж�ģʽ", "Judgement"},
	{"��׼", "Standard"},
	{"�ϸ�", "Strict"},			// 95
	{"�����ж���ʽ\n��׼������ļ�����\n������һ����\n�ϸ��ж�ʱ���ڰ���ļ�\n�������һ����",
	"Scoring key method:\nStandard: wrongly pressed key\nWILL NOT cancel the next note\nStrict: wrongly pressed key\nWILL cancel the next note\nif within time frame"},
	{"(��)\n������������", "(OFF)\nVery slow in drawing long strips"},
	{"�����ӳٵ���", "Note Delay Adjust"},
	{"�����ӳٲ���", "Keyboard Delay Compensation"},
	{"�ӳٲ���", "DelayCompen"},		// 100
	{"\n���������ź��ӳٲ���\n��λ������\n��Ӱ�찴�����ӳ�\n��Ϸ�а�F5/F6Ҳ�ɵ���", "\nKeyboard Delay Compensation\nUnit: in milliseconds\nDoes NOT affect key sound delay\nPress F5/F6 during game does same"},
	{"1. ��Ϸ�п���CapsLock (��дָʾ��)����Ѫ����������ֻҪѪ������0��Ѫ�۾ͻ��ڣ�����֮��һ���� MISTAKE\n"
	 "2. ��Ϸ�а�F1~F4���������浱ǰmap�������ӳ٣������ļ����е�calibration.inf������F5/F6���������水���ź��ӳ٣���PageUp/PageDown���������浱ǰ��������\n"
	 "3. ��+/-���Ըı����ֽ��ࣨ��������������F11/F12���Ըı����������ֽ��ࣩ����F7/F8����β���ƶ��ٶ�\n"
	 "4. ����������ر�ʱ���ı����ֽ����ͬʱҲ��ı�����������Ϊֻ�ܸĲ������Ĳ����ʣ���������ͬ���ʸߡ��ӳ�С\n"
	 "5. ��config.ini�м���\"songPath = [�����ļ���Ŀ¼]\\\" �����޸ĸ���Ŀ¼��ַ\n"
	 "6. �����ļ����п��԰������ļ��У�������������������\n"
	 "7. ������Ȳۿ����ƶ�����ͬ���ŵ㣬��<Tab>���ƶ���ǰһ�����ŵ㣬����Ϸ����һ���� MISTAKE",
	 "1. During game, turn on CapsLock to prevent 'death'. However, once HP has gone below 0, HP gauge will turn black, final result will always be 'MISTAKE'\n"
	 "2. During game, press F1~F4 to adjust and save note delay to the current map (see calibration.inf in that song's folder), press F5/F6 to adjust and save keyboard delay compensation, press PageUp/PageDown to adjust and save the volume of the current song\n"
	 "3. Press +/- to adjust music tempo (maintain the same pitch), press F11/F12 to adjust music pitch (maintain the same tempo), press F7/F8 to adjust the speed of tail movement\n"
	 "4. When PitchShiftDSP is disabled, the pitch cannot be changed independently from the tempo, (only the sampling rate is changed), but the music latency/delay is minimal\n"
	 "5. To change the song folder address, add the following line to config.ini, \"songPath = [my_song_folder_path]\\\"\n"
	 "6. The song folder may contain sub-folders now so that you can reorganize your collection of songs\n"
	 "7. Click on the progress bar to seek track to that position, press <Tab> to seek track to previous seek-track position; however, doing so will result in 'MISTAKE' for the final score"},
	{"��Ϸ˵��", "Game Help"},
	{"����", "Volume"},
	{"�����ٶ�", "Play Speed"},		//105
	{"β���ٶ�", "Tail Speed"},
	{"��������", "PitchShiftDSP"},
	{"\n\n\n���ֽ���ı�ʱ��������\n�������ò�ᵼ�²�ͬ��",
	 "Maintain the same pitch\nwhen music tempo changes\n\nMay cause music delay\non slow computers"},
	{"����", "Pitch"},
	{"����", "Tempo"},				//110
	{"����", "Semitone(s)"},
	{"����ϵͳ", "SoundSystem"},
	{"��������ϵͳ��\nFMOD ��: ���ٺ�ʱ��ƫ��С\n �޷�ʵ���������ϱ��\nBASS ��: ��ʵ���������ϱ��\n ���ٺ�ʱ��ƫ���",
	 "Change sound system:\nFMOD has better temporal accuracy\n pitch-shift ratio cannot exceed 2\nBASS has wider pitch-shift ratio\n but poorer temporal accuracy"},
	{"ʵ��������ħ��", "Solid Volume Rubix Cube"},
	{"������\nF2��ָʾ��λ�ķ���\nF4������\nF5����λ\n1��9�������������n��\n~��ȡ������\n����������һ��\n�ң���һ��\n+/=����������һ��\n-/_����������һ��\nPageUp��ת������\nPageDown��ת������",	// 115
	 "Help:\nF2: blink displaced cubes\nF4: shuffle\nF5: reset cube\n1~9: highlight Nth outermost cube\n~: cancel highlight\nBackspace/Left: Previous Move\nRight: Next Move\n+/=: highlight outer layer\n-/_: highlight inner layer\nPageUp: rotate faster\nPageDown: rotate slower"},
	{"ħ������ (NxNxN):", "Order (NxNxN):"},
	{"����Զ��", "Dim Distant Cubes"},
	{"�滭ģʽ", "Draw Mode"},
	{"���˵���ʼ״̬", "Rollback to initial"},
	{"�ù����ʽ���", "Solve using orbit formula"},	//120
	{"����", "fast"},
	{"׼ȷ", "accurate"},
	{"ȷ��/��λ", "OK/Reset"},
	{"�浵", "Save"},
	{"����", "Load"},								//125
	{"����", "Shuffle"},
	{"�����ɣ�ף�أ�", "Solved! Congratulations!"},
	{"ȥ����ʷ", "Goto History"},
	{"����", "Directional"},
	{"ʵ��", "Solid"},								//130
	{"", ""},
	{"", ""},
	{"", ""},
	{"", ""},
	{"", ""},
};

wstring str2wstr(string &s)
{
	wchar_t *buf = new wchar_t[ s.size() ];
	size_t num_chars = mbstowcs( buf, s.c_str(), s.size() );
	wstring ws( buf, num_chars );
	delete[] buf;
	return ws;
}

void null(){}

void ErrorExit(string error_infor,int exit_code)
{
	MessageBox(NULL,error_infor.data(),"DIVA",MB_OK);
	exit(exit_code);
}

LONGLONG GetCurrentCount(void)
{
	LARGE_INTEGER tmp;
	QueryPerformanceCounter(&tmp);
	return tmp.QuadPart;
}
RECT MakeRect(int px,int py,int width,int height)
{
	RECT ret;
	ret.top=py;
	ret.left=px;
	ret.right=px+width;
	ret.bottom=py+height;
	return ret;
}
fRECT MakefRect(int px,int py,int width,int height)
{
	fRECT ret;
	ret.top=py;
	ret.left=px;
	ret.right=px+width;
	ret.bottom=py+height;
	return ret;
}
RECT MakeRect(const fRECT &rect)
{
	RECT ret;
	ret.top = rect.top+0.5f;
	ret.left = rect.left+0.5f;
	ret.right = rect.right+0.5f;
	ret.bottom = rect.bottom+0.5f;
	return ret;
}
fRECT MakefRect(const RECT &rect)
{
	fRECT ret;
	ret.top = rect.top;
	ret.left = rect.left;
	ret.right = rect.right;
	ret.bottom = rect.bottom;
	return ret;
}
bool InsideRect(float x, float y, const fRECT &rect)
{
	return x>=rect.left && x<rect.right && y>=rect.top && y<rect.bottom;
}
bool InsideRect(int x, int y, const RECT &rect)
{
	return x>=rect.left && x<rect.right && y>=rect.top && y<rect.bottom;
}
void SetRectPos(int px,int py,RECT &src)
{
	RECT ret;
	ret.left = px;
	ret.top = py;
	ret.right = px + src.right - src.left;
	ret.bottom = py + src.bottom - src.top;
	src = ret;
}

void RandomDir(float &dx, float &dy){
	float angle = (float)rand()/RAND_MAX*6.2831853f;
	dx = cos(angle);
	dy = sin(angle);
}
float UnitLength(float &dx, float &dy){
	float length = sqrt(dx*dx+dy*dy);
	if(length>0){
		dx /= length;
		dy /= length;
	}
	return length;
}
void SetLength(float &dx, float &dy, float set_length){
	float length = sqrt(dx*dx+dy*dy);
	if(length>0){
		length = set_length/length;
		dx *= length;
		dy *= length;
	}
}

DWORD RandomColor(){
	BYTE rgb[4]={rand()&0xff,rand()&0xff,rand()&0xff,rand()&0xff};
	return *(DWORD*)rgb;
}


///-------------------------------------
///��stringת��ΪLPCWSTR
///-------------------------------------
LPCWSTR SToW(string tex)
{
	LPWSTR pszOut = NULL;
	int nInputStrLen = strlen (tex.c_str()); 
	 // Double NULL Termination 
	 int nOutputStrLen = MultiByteToWideChar(CP_ACP, 0, tex.c_str(), nInputStrLen, NULL, 0) + 2; 
	 pszOut = new WCHAR [nOutputStrLen]; 
	 if (pszOut) 
	 { 
		memset (pszOut, 0x00, sizeof (WCHAR)*nOutputStrLen); 
		MultiByteToWideChar (CP_ACP, 0, tex.c_str(), nInputStrLen, pszOut, nInputStrLen); 
	 }
	 wcheckTail0(pszOut,nOutputStrLen);
	 return pszOut;
}

wstring Ansi2WChar(LPCSTR pszSrc, int nLen)
{
	int nSize = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)pszSrc, nLen, 0, 0);
	if(nSize <= 0) return NULL;
	WCHAR *pwszDst = new WCHAR[nSize+1];
	if( NULL == pwszDst) return NULL;
	MultiByteToWideChar(CP_ACP, 0,(LPCSTR)pszSrc, nLen, pwszDst, nSize);
	pwszDst[nSize] = 0;
	if( pwszDst[0] == 0xFEFF) // skip Oxfeff
	for(int i = 0; i < nSize; i ++) 
	pwszDst[i] = pwszDst[i+1]; 
	wstring wcharString(pwszDst);
	delete pwszDst;
	return wcharString;
}

wstring s2ws(const string& s){ return Ansi2WChar(s.c_str(),s.size());}

void checkTail0(char *s,int maxl)
{
	int l=0;
	while(l<maxl)
	{
		if(s[l]==10)
		{
			s[l]=0;
			break;
		}
		l++;
	}
}

void wcheckTail0(wchar_t *s,int maxl)
{
	int l=0;
	while(l<maxl)
	{
		if(s[l]==10)
		{
			s[l]=0;
			break;
		}
		l++;
	}
}

bool isImage(string filename)
{
	string ret;
	for(int i=filename.length()-1;i>=0;i--)
	{
		if(filename[i]=='.')
			break;

		ret=char((filename[i]>='A'&&filename[i]<='Z')?(filename[i]-('A'-'a')):(filename[i]))+ret;
	}
	
	if(ret=="jpg"||ret=="png"||ret=="bmp"||ret=="dds"||ret=="tga")
		return true;
	else
		return false;
}

bool operator==(const RECT &rect1,const RECT &rect2)
{
	return (rect1.left==rect2.left&&rect1.bottom==rect2.bottom&&rect1.right==rect2.right&&rect1.top==rect2.top);
}


bool hasChineseLocale(){
	char str[LOCALE_NAME_MAX_LENGTH];
	if(!GetLocaleInfo(LOCALE_SYSTEM_DEFAULT,LOCALE_SISO639LANGNAME,str,200)) return 1;
	return !strcmp(str,"zh");
}

Option *options = NULL;
const int cfg_menu_option_idN = 17;
int cfg_menu_option_id[cfg_menu_option_idN]={0,1,4,5,6,10,11,7,8,9,12,13,14,18,17,23,15};	// Add into 2nd last posi

int getMaxSkin(){
	for(int x=1; true; x++){
		ostringstream oss;
		oss << "gamedata" << x << ".txt";
		ifstream fin(oss.str().c_str());
		if(!fin) return x-1;
	}
}
int unused;

BYTE default_key_maps[key_mapsN][8]={
	{'D', 'A', 'S', 'W', KeyRight, KeyLeft, KeyDown, KeyUp},
	{KeyRight, KeyLeft, KeyDown, KeyUp, 'D', 'A', 'S', 'W'},
	{'F', 'S', 'D', 'E', 'L', 'J', 'K', 'I'},
	{'L', 'J', 'K', 'I', 'F', 'S', 'D', 'E'},
};
char current_key_map[256];	// maps virtual-key code to UNIT_TYPE (0~7), global, currently in use
char *key_map = current_key_map;

GUIMessageBox *lock_msgbox = NULL, *old_lock_msgbox = NULL;
string printKey(UINT nChar){
	ostringstream oss;
	switch(nChar){
	case VK_UP:			oss << "��"; break;
	case VK_DOWN:		oss << "��"; break;
	case VK_LEFT:		oss << "��"; break;
	case VK_RIGHT:		oss << "��"; break;
	case VK_HOME:		oss << "HOME"; break;
	case VK_END:		oss << "END"; break;
	case VK_PRIOR:		oss << "PageUp"; break;
	case VK_NEXT:		oss << "PageDown"; break;
	case VK_SPACE:		oss << StringTable(89); break;
	case VK_OEM_COMMA:	oss << ","; break;
	case VK_OEM_PERIOD:	oss << "."; break;
	case VK_OEM_MINUS:	oss << "-"; break;
	case VK_OEM_PLUS:	oss << "="; break;
	case VK_OEM_2:		oss << "/"; break;
	case VK_OEM_4:		oss << "["; break;
	case VK_OEM_5:		oss << "\\"; break;
	case VK_OEM_6:		oss << "]"; break;
	case VK_OEM_1:		oss << ";"; break;
	case VK_OEM_7:		oss << "'"; break;
	case VK_NUMPAD0:	oss << "NumPad0"; break;
	case VK_NUMPAD1:	oss << "NumPad1"; break;
	case VK_NUMPAD2:	oss << "NumPad2"; break;
	case VK_NUMPAD3:	oss << "NumPad3"; break;
	case VK_NUMPAD4:	oss << "NumPad4"; break;
	case VK_NUMPAD5:	oss << "NumPad5"; break;
	case VK_NUMPAD6:	oss << "NumPad6"; break;
	case VK_NUMPAD7:	oss << "NumPad7"; break;
	case VK_NUMPAD8:	oss << "NumPad8"; break;
	case VK_NUMPAD9:	oss << "NumPad9"; break;
	case VK_MULTIPLY:	oss << "NumPad*"; break;
	case VK_ADD:      	oss << "NumPad+"; break;
	case VK_SEPARATOR:	oss << "NumPad;"; break;
	case VK_SUBTRACT:	oss << "NumPad-"; break;
	case VK_DECIMAL: 	oss << "NumPad."; break;
	case VK_DIVIDE:		oss << "NumPad/"; break;
	default:
		if(nChar>='A'&&nChar<='Z') oss << (char)nChar;
		else if(nChar>='0'&&nChar<='9') oss << (char)nChar;
		else oss<<"#"<<nChar;
	}
	return oss.str();
}
string printKeys(int key, int maxlen){
	int found = 0;
	string out;
	for(int i=0; i<256; i++){
		if(key_map[i]==key){
			if(found)
				out += StringTable(90);
			found ++;
			out += printKey(i);
		}
	}
	if(found>=7)
		found++;
	return out;
}


string ToString(int num)
{
	char buffer[20];
	itoa(num,buffer,10);
	return string(buffer);
}

string ToString(double num,const char *_Format)
{
	char buffer[20];
	sprintf(buffer,_Format,num);
	return string(buffer);
}


Point Bezier(const Point &p0, const Point &p1, const Point &p2, const float &t)
{
	float tt = 1-t;
	return p0*tt*tt+p1*2*t*tt+p2*t*t;
}

Point Bezier(const Point &p0, const Point &p1, const Point &p2, const Point &p3, const float &t)
{
	float tt = 1-t;
	return p0*tt*tt*tt+p1*3*t*tt*tt+p2*3*t*t*tt+p3*t*t*t;
}

Point BezierDir(const Point &p0, const Point &p1, const Point &p2, const Point &p3, const float &t)
{
	float tt = 1-t;
	return (p1-p0)*tt*tt*3 + (p2-p1)*t*tt*6 + (p3-p2)*t*t*3;
}

void to_integer_point(double &val)
{
	if(abs(val-(LONGLONG)val)<1e-8)
		val = (LONGLONG)val;
}

void to_integer_point(float &val)
{
	if(abs(val-(LONGLONG)val)<1e-8)
		val = (LONGLONG)val;
}


