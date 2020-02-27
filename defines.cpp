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
	{"按键习惯：", "Keyboard Style:"},												//0
	{"窗口大小：", "Window Size:"},
	{"音效音量：", "Volume:"},
	{"没有检测到\n演奏节奏游戏\n无法进入", "Cannot locate\ngame file\nCannot play"},
	{"可以\n演奏节奏游戏", "Choose songs to play, preview PV or Demo\n"},
	{"可在Diva Online Map Wiki\n上查找最新的歌曲追加包\n点击跳转到Wiki",			//5
	"Search for newest version song packages\nat Diva Online Map Wiki\nClick to goto the Wiki page"},
	{"在Diva OL中您可以和网络上的\n玩家在各种模式中一起游戏\n点击详细信息",
	"In Diva OL, you can play with\nother players through the Internet\nClick for details"},
	{"变更按键设置和窗口大小\n以及音效的音量", "Config keyboard layout,\nwindow size and sound volume"},
	{"可以鉴赏\n游戏的插画和影像", "View Miku's pictures and\nphoto album"},
	{"显示游戏帮助\nhttp://www.gamemastercn.com\n非原创素材版权归SEGA所有",
	 "Show Game Manual\nhttp://www.gamemastercn.com\nCopyright belongs to SEGA"},
	{"结束游戏", "Quit Game"},														//10
	{"可以调整的窗口大小：", "Available window sizes:"},
	{"\n可以更改\n音效、按键音的音量\n按<Enter>键听节拍音", "\nChange the volume\nof sound effects\nPress <Enter> to hear hit sound"},
	{"\n\n恢复所有设置到默认", "\n\nReset all settings to default"},
	{"暂停", "PAUSE"},
	{"继续", "CONTINUE"},															//15
	{"重试", "RESTART"},
	{"返回菜单", "BACK TO MENU"},
	{"没有歌曲", "List is Empty"},
	{"自由模式", "Custom Game"},
	{"自由模式（没有歌曲）", "Custom Game (empty list)"},								//20
	{"生涯剧情模式（未开启）", "Career Mode (not available)"},
	{"网络对战模式（未开启）", "Multiplayer Mode (not available)"},
	{"设置", "Settings"},
	{"CG鉴赏", "CG Preview"},
	{"游戏帮助", "Game Help"},												//25
	{"退出游戏", "Exit Game"},
	{"菜单", "MENU"},
	{"作者", "Author"},
	{"容易", "Easy"},
	{"普通", "Normal"},																//30
	{"困难", "Hard"},
	{"超难", "Extra"},
	{"最难", "Die"},
	{"星", "Star"},
	{"难度分类:%s\n难度星级:%d星\n\n  最高分:%.7d\n最高连击:%.4d\n最高评价:%s\n通关次数:%d",	//35
	"Difficulty:%s\nStar Level:%d Star\n\nBest score:%.7d\nCombo Max:%.4d\nBest result:%s\n# Times Passed:%d"},
	{"（全屏）", "(Full Screen)"},
	{"重试", "RETRY"},
	{"返回", "RETURN"},
	{"恢复默认设置", "Reset to Default"},
	{"", ""},																				//40
	{"语言：", "Language:"},
	{"中文", "English"},
	{"粒子效果：", "Particle Effect:"},
	{"\n\n更改显示语言：\n中文\n英文", "\n\nChange displayed language:\nChinese\nEnglish"},
	{"更改粒子效果：\n0: 无粒子效果（最快）\n1: 单尾（快）\n2: 双尾（普通）\n3: 彗星尾巴（最慢）",	//45
	"Change particle system:\n0: no particle (fastest)\n1: single tail (fast)\n2: double tail (normal)\n3: comet tail (slowest)"},
	//↑↓←→△×□○
	{"按<Enter>键开始设置\n\n【同时按下多键可以\n将多键设为同一键】", "Press <Enter> to change\n\n[Holding down multiple keys\nto map multiple keys\nto the same key]"},
	{"宋体", "Times New Roman"},
	{"楷体", "Courier New"},
	{"黑体", "Arial"},
	{"（无）", "N/A"},																		//50
	{"失败", "Misstake"},
	{"合格", "Cheap"},
	{"一般", "Standard"},
	{"优秀", "Great"},
	{"完美", "Perfect"},																	//55
	{"宋体", "Times New Roman"},	//老外电脑上可能没宋体字
	{"关闭", "OFF"},
	{"低", "LOW"},
	{"中", "MEDIUM"},
	{"高", "HIGH"},																			//60
	{"全局键盘钩住失败，无法更准确及时的捕获键盘。如有防火墙跳出，请允许！",
	"Global keyboard hook failed, high-precision keyboard input capture is not available.\r\n"
	"If a firewall notice has pop up, please allow!"},
	{"按键捕获", "Key Capture"},
	{"显示帧频","Show FPS"},
	{"已启用", "Enabled"},
	{"已禁用", "Disabled"},																	//65
	{"不显示", "OFF"},
	{"显示", "ON"},
	{"画面显示", "ON(OSD)"},
	{"显示每秒钟帧数\n1.不显示\n2.窗口标题显示\n3.画面显示",
	"Show FPS\n(Frames per second)\n1. Don't show\n2. Show on window title\n3. On-screen display"},
	{"\n\n0: 普通按键 WM_KEYDOWN\n1: 全局键盘钩 WH_KEYBOARD_LL\n2: 使用 DirectInput",
	"\n\n0: Normal key input WM_KEYDOWN\n1: Global Keyhook WH_KEYBOARD_LL\n2: Use DirectInput"},		//70
	{"当前FPS=%.2f, 平均FPS=%.2f", "CurrFPS=%.2f, AvgFPS=%.2f"},
	{"启用失败", "Failed"},
	{"自动暂停", "Auto Pause"},
	{"从不", "Never"},
	{"玩时", "PlayOnly"},																	//75
	{"总是", "Always"},
	{"自动暂停方式：\n1. 从不暂停\n2. 只在游戏模式下暂停\n3. 总是暂停",
	"Auto pause method:\n1. Never pause\n2. In play-mode only\n3. Alway pause"},
	{"视频质量","Video Quality"},
	{"将高清MP4视频转为低清AVI\n以节省CPU资源\n0: 高清MP4 (不转换)\n1: 最高质AVI\n9: 最低质AVI",
	"Convert video from MP4 to AVI\nto save CPU\n0: MP4 (no conversion)\n1: highest quality AVI\n9: lowest quality AVI"},
	{"游戏界面","Game UI"},																						//80
	{"\n选择界面皮肤:\n查看 gamedata*.txt\n当前最大范围：1～"+ToString(getMaxSkin())+"\n打开<CapsLock>预览",
	"\nChoose UI skin:\nSee gamedata*.txt\nCurrent Range: 1~"+ToString(getMaxSkin())+"\nTurn on <CapsLock> to preview"},
	{"AVI 分辨率：\n1～3：1024×576\n4～6：640×360\n7～9：480×270",
	"AVI Resolution：\n1~3：1024x576\n4~6：640x360\n7~9：480x270"},
	{"自定义", "Custom"},
	{"请按顺序依次按下↑↓←→△×□○",
		"Please press ↑↓←→△×□○ in order"},
	{"按<ESC>键取消，<Backspace>回到前一个", "Press <ESC> to cancel, <Backspace> to previous key"}, //85
	{"按<Enter>键确定，按<ESC>键取消", "Press <Enter> to confirm, press <ESC> to cancel"},
	{"字体", "Font"},
	{"\n改变界面字体：\n宋体\n楷体\n黑体", "\nChange UI Font:\nTimes New Roman\nCourier New\nArial"},
	{"空格", "Space"},
	{"或", " or "},			// 90
	{"反锯齿", "Antialiasing"},
	{"反锯齿模式画长条：\n0：(关)软件逐点画(最最慢)\n1：(开)硬件最低(最快)\n...\nN：(开)硬件最高(最慢)\n\n当前选项：\n",
	"Anti-aliasing mode:\n0: (OFF) software draw strip (SLOWEST)\n1: (ON) hardware lowest (fastest)\n...\nN: (ON) hardware highest (slowest)\n\nCurrent option:\n"},
	{"判定模式", "Judgement"},
	{"标准", "Standard"},
	{"严格", "Strict"},			// 95
	{"按键判定方式\n标准：按错的键不会\n抵消下一个键\n严格：判定时间内按错的键\n会抵消下一个键",
	"Scoring key method:\nStandard: wrongly pressed key\nWILL NOT cancel the next note\nStrict: wrongly pressed key\nWILL cancel the next note\nif within time frame"},
	{"(关)\n画长条键很慢", "(OFF)\nVery slow in drawing long strips"},
	{"音键延迟调整", "Note Delay Adjust"},
	{"按键延迟补偿", "Keyboard Delay Compensation"},
	{"延迟补偿", "DelayCompen"},		// 100
	{"\n调整按键信号延迟补偿\n单位：毫秒\n不影响按键音延迟\n游戏中按F5/F6也可调整", "\nKeyboard Delay Compensation\nUnit: in milliseconds\nDoes NOT affect key sound delay\nPress F5/F6 during game does same"},
	{"1. 游戏中开着CapsLock (大写指示灯)，掉血不死，但是只要血减到过0，血槽就会变黑，结束之后一律算 MISTAKE\n"
	 "2. 游戏中按F1~F4调整并保存当前map的音键延迟（音乐文件夹中的calibration.inf），按F5/F6调整并保存按键信号延迟，按PageUp/PageDown调整并保存当前歌曲音量\n"
	 "3. 按+/-可以改变音乐节奏（保持音调），按F11/F12可以改变音调（保持节奏），按F7/F8调节尾巴移动速度\n"
	 "4. 当音调计算关闭时，改变音乐节奏的同时也会改变音调，（因为只能改播放器的采样率），但音乐同步率高、延迟小\n"
	 "5. 在config.ini中加入\"songPath = [歌曲文件夹目录]\\\" 可以修改歌曲目录地址\n"
	 "6. 歌曲文件夹中可以包含子文件夹，方便玩家重新整理歌曲\n"
	 "7. 点击进度槽可以移动到不同播放点，按<Tab>键移动到前一个播放点，但游戏分数一律算 MISTAKE",
	 "1. During game, turn on CapsLock to prevent 'death'. However, once HP has gone below 0, HP gauge will turn black, final result will always be 'MISTAKE'\n"
	 "2. During game, press F1~F4 to adjust and save note delay to the current map (see calibration.inf in that song's folder), press F5/F6 to adjust and save keyboard delay compensation, press PageUp/PageDown to adjust and save the volume of the current song\n"
	 "3. Press +/- to adjust music tempo (maintain the same pitch), press F11/F12 to adjust music pitch (maintain the same tempo), press F7/F8 to adjust the speed of tail movement\n"
	 "4. When PitchShiftDSP is disabled, the pitch cannot be changed independently from the tempo, (only the sampling rate is changed), but the music latency/delay is minimal\n"
	 "5. To change the song folder address, add the following line to config.ini, \"songPath = [my_song_folder_path]\\\"\n"
	 "6. The song folder may contain sub-folders now so that you can reorganize your collection of songs\n"
	 "7. Click on the progress bar to seek track to that position, press <Tab> to seek track to previous seek-track position; however, doing so will result in 'MISTAKE' for the final score"},
	{"游戏说明", "Game Help"},
	{"音量", "Volume"},
	{"播放速度", "Play Speed"},		//105
	{"尾巴速度", "Tail Speed"},
	{"音调计算", "PitchShiftDSP"},
	{"\n\n\n音乐节奏改变时保持音调\n电脑配置差会导致不同步",
	 "Maintain the same pitch\nwhen music tempo changes\n\nMay cause music delay\non slow computers"},
	{"音调", "Pitch"},
	{"节奏", "Tempo"},				//110
	{"半音", "Semitone(s)"},
	{"声音系统", "SoundSystem"},
	{"更改声音系统：\nFMOD 库: 变速后时间偏差小\n 无法实现两倍以上变调\nBASS 库: 能实现两倍以上变调\n 变速后时间偏差大",
	 "Change sound system:\nFMOD has better temporal accuracy\n pitch-shift ratio cannot exceed 2\nBASS has wider pitch-shift ratio\n but poorer temporal accuracy"},
	{"实心体有向魔方", "Solid Volume Rubix Cube"},
	{"帮助：\nF2：指示错位的方块\nF4：打乱\nF5：复位\n1～9：高亮从外起第n层\n~：取消高亮\n撤消、左：上一步\n右：下一步\n+/=：高亮向外一层\n-/_：高亮向内一层\nPageUp：转动加速\nPageDown：转动减速",	// 115
	 "Help:\nF2: blink displaced cubes\nF4: shuffle\nF5: reset cube\n1~9: highlight Nth outermost cube\n~: cancel highlight\nBackspace/Left: Previous Move\nRight: Next Move\n+/=: highlight outer layer\n-/_: highlight inner layer\nPageUp: rotate faster\nPageDown: rotate slower"},
	{"魔方阶数 (NxNxN):", "Order (NxNxN):"},
	{"近亮远暗", "Dim Distant Cubes"},
	{"绘画模式", "Draw Mode"},
	{"回退到初始状态", "Rollback to initial"},
	{"用轨道公式求解", "Solve using orbit formula"},	//120
	{"快速", "fast"},
	{"准确", "accurate"},
	{"确定/复位", "OK/Reset"},
	{"存档", "Save"},
	{"读档", "Load"},								//125
	{"打乱", "Shuffle"},
	{"求解完成！祝贺！", "Solved! Congratulations!"},
	{"去到历史", "Goto History"},
	{"有向", "Directional"},
	{"实心", "Solid"},								//130
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
///将string转换为LPCWSTR
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
	case VK_UP:			oss << "↑"; break;
	case VK_DOWN:		oss << "↓"; break;
	case VK_LEFT:		oss << "←"; break;
	case VK_RIGHT:		oss << "→"; break;
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


