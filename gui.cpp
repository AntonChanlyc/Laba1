
#include <windows.h>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <memory>
#include <stack>
#include <ctime>
#include <climits>

#include "Calculator.h"
#include "Complex.h"
#include "Quaternion.h"
#include "HistorySet.h"


double __complex_re = 0.0;
double __complex_im = 0.0;
bool __complex_ok = false;

double __quat_w = 0.0;
double __quat_x = 0.0;
double __quat_y = 0.0;
double __quat_z = 0.0;
bool __quat_ok = false;


using History = HistorySet<std::string, std::stack<std::shared_ptr<Number>>>;

static Calculator gui_calc;
static History gui_history;

HWND hList = NULL;
HWND hHistoryWindow = NULL;
HWND hHistoryList = NULL;

// forward
LRESULT CALLBACK HistoryWndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK ComplexDlgProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK QuaternionDlgProc(HWND, UINT, WPARAM, LPARAM);


static std::string numberToString(const std::shared_ptr<Number>& n) {
    std::ostringstream ss;
    if (n) ss << *n;
    return ss.str();
}


void RefreshStack() {
    if (!hList) return;
    SendMessage(hList, LB_RESETCONTENT, 0, 0);
    auto st = gui_calc.snapshot();
    std::vector<std::shared_ptr<Number>> v;
    while (!st.empty()) { v.push_back(st.top()); st.pop(); }
    for (int i = (int)v.size() - 1; i >= 0; --i) {
        std::string s = numberToString(v[i]);
        SendMessageA(hList, LB_ADDSTRING, 0, (LPARAM)s.c_str());
    }
}


void RefreshHistoryWindow() {
    if (!hHistoryList) return;
    SendMessage(hHistoryList, LB_RESETCONTENT, 0, 0);
    auto keys = gui_history.keys();
    for (auto &k : keys) SendMessageA(hHistoryList, LB_ADDSTRING, 0, (LPARAM)k.c_str());
}

void ShowStackContent(const std::string &key) {
    if (!gui_history.contains(key)) { MessageBoxA(NULL, "Нет такой записи", "Error", MB_OK | MB_ICONERROR); return; }
    auto st = gui_history[key];
    std::ostringstream ss;
    ss << "Stack (" << key << "):\n\n";
    while (!st.empty()) { ss << *st.top() << "\n"; st.pop(); }
    std::string out = ss.str();
    MessageBoxA(NULL, out.c_str(), "Stack content", MB_OK);
}



#pragma pack(push,1)
struct DLGTEMPLATE_HEADER {
    DLGTEMPLATE dlgtemplate;
    WORD menu;      
    WORD windowClass; 
    WCHAR title[1]; 
};
#pragma pack(pop)


static BYTE* Align4(BYTE* p) {
    uintptr_t v = (uintptr_t)p;
    uintptr_t a = (v + 3) & ~((uintptr_t)3);
    return (BYTE*)a;
}


static WCHAR* write_wstr(BYTE*& buf, const wchar_t* s) {
    WCHAR* dest = (WCHAR*)buf;
    while (*s) { *dest++ = *s++; }
    *dest++ = 0;
    buf = (BYTE*)dest;
    return dest;
}


static DLGTEMPLATE* CreateDialogTemplate(const char* title, const std::vector<const char*>& labels, int dlgWidth = 260, int dlgHeight = 80) {
    
    size_t est = 1024 + labels.size() * 256;
    BYTE* buf = new BYTE[est];
    BYTE* p = buf;
    
    p = Align4(p);

    
    DLGTEMPLATE* dt = (DLGTEMPLATE*)p;
    dt->style = WS_POPUP | WS_BORDER | WS_SYSMENU | DS_SETFONT | DS_MODALFRAME;
    dt->dwExtendedStyle = 0;
    dt->cdit = (WORD)labels.size() + 2; 
    dt->x = 10; dt->y = 10; dt->cx = dlgWidth; dt->cy = dlgHeight;
    p += sizeof(DLGTEMPLATE);

    
    *(WORD*)p = 0; p += sizeof(WORD);
    *(WORD*)p = 0; p += sizeof(WORD);

    
    const wchar_t* wt = L"";
    * (WCHAR*)p = 0; p += sizeof(WCHAR);

    
    *(WORD*)p = 8; p += sizeof(WORD);
    const wchar_t* fontname = L"MS Shell Dlg";
    WCHAR* wdest = (WCHAR*)p;
    const wchar_t* s = fontname;
    while (*s) { *wdest++ = *s++; }
    *wdest++ = 0;
    p = (BYTE*)wdest;

    
    int baseY = 10;
    int labelX = 10, editX = 100;
    int labelW = 80, editW = 140, height = 20;
    for (size_t i = 0; i < labels.size(); ++i) {
        
        p = Align4(p);
        DLGITEMTEMPLATE* dit = (DLGITEMTEMPLATE*)p;
        dit->style = WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_GROUP;
        dit->dwExtendedStyle = 0;
        dit->x = labelX;
        dit->y = baseY + (int)i * 30;
        dit->cx = labelW;
        dit->cy = height;
        dit->id = 0xFFFF; 
        p += sizeof(DLGITEMTEMPLATE);
        
        *(WORD*)p = 0xFFFF; p += 2;
        *(WORD*)p = 0x0082; p += 2;
        
        const wchar_t* lbl = L"";
        
        std::u16string tmp;
        std::string s8 = labels[i];
        
        std::wstring wlab;
        for (char ch : s8) wlab.push_back((wchar_t)ch);
        WCHAR* wd = (WCHAR*)p;
        for (wchar_t wc : wlab) { *wd++ = wc; }
        *wd++ = 0;
        p = (BYTE*)wd;
        
        *(WORD*)p = 0; p += 2;
        

        
        p = Align4(p);
        DLGITEMTEMPLATE* dit2 = (DLGITEMTEMPLATE*)p;
        dit2->style = WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL;
        dit2->dwExtendedStyle = 0;
        dit2->x = editX;
        dit2->y = baseY + (int)i * 30;
        dit2->cx = editW;
        dit2->cy = height;
        dit2->id = 1001 + (WORD)i; 
        p += sizeof(DLGITEMTEMPLATE);
        
        *(WORD*)p = 0xFFFF; p += 2;
        *(WORD*)p = 0x0081; p += 2;
        
        *(WORD*)p = 0; p += 2;
        
        *(WORD*)p = 0; p += 2;
    }

    
    p = Align4(p);
    DLGITEMTEMPLATE* but1 = (DLGITEMTEMPLATE*)p;
    but1->style = WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_DEFPUSHBUTTON;
    but1->dwExtendedStyle = 0;
    but1->x = 40;
    but1->y = baseY + (int)labels.size() * 30 + 10;
    but1->cx = 60; but1->cy = 22;
    but1->id = IDOK;
    p += sizeof(DLGITEMTEMPLATE);
    *(WORD*)p = 0xFFFF; p += 2;
    *(WORD*)p = 0x0080; p += 2; 
    
    const wchar_t* okw = L"OK";
    WCHAR* wptr = (WCHAR*)p;
    *wptr++ = L'O'; *wptr++ = L'K'; *wptr++ = 0;
    p = (BYTE*)wptr;
    *(WORD*)p = 0; p += 2;

    
    p = Align4(p);
    DLGITEMTEMPLATE* but2 = (DLGITEMTEMPLATE*)p;
    but2->style = WS_CHILD | WS_VISIBLE | WS_TABSTOP;
    but2->dwExtendedStyle = 0;
    but2->x = 120; but2->y = baseY + (int)labels.size() * 30 + 10;
    but2->cx = 60; but2->cy = 22;
    but2->id = IDCANCEL;
    p += sizeof(DLGITEMTEMPLATE);
    *(WORD*)p = 0xFFFF; p += 2;
    *(WORD*)p = 0x0080; p += 2;
    WCHAR* wptr2 = (WCHAR*)p;
    *wptr2++ = L'C'; *wptr2++ = L'a'; *wptr2++ = L'n'; *wptr2++ = L'c'; *wptr2++ = L'e'; *wptr2++ = 0;
    p = (BYTE*)wptr2;
    *(WORD*)p = 0; p += 2;

    
    return (DLGTEMPLATE*)buf;
}



bool AskComplexModal(HWND parent, double &re, double &im) {
    
    const char* labs[] = { "Re:", "Im:" };
    std::vector<const char*> labels(labs, labs + 2);
    DLGTEMPLATE* tmpl = CreateDialogTemplate("Complex", labels, 260, 140);

    
    struct Param { double re, im; bool ok; } param;
    param.ok = false; param.re = param.im = 0.0;

    INT_PTR ret = DialogBoxIndirectParamA(NULL, tmpl, parent, ComplexDlgProc, (LPARAM)&param);

    
    delete[] (BYTE*)tmpl;

    if (ret == 1) {
        // retrieve values from param stored in ComplexDlgProc via globals - we'll store into static variables
        // We'll use GetDlgItemText inside proc and store into globals.
        // For simplicity, ComplexDlgProc will fill global static variables complex_re, complex_im.
        // So here we read them from globals defined below via functions.
        // To avoid globals confusion, we'll set them as static in proc and expose getters — but simpler:
        // in this code we will use GetProp on parent? To simplify, ComplexDlgProc will set a static pair accessible via functions.
    }

    
    HANDLE h1 = GetPropA(NULL, "COMPLEX_RE");
    HANDLE h2 = GetPropA(NULL, "COMPLEX_IM");
    if (h1 && h2) {
        double r = 0, imv = 0;
        memcpy(&r, &h1, sizeof(double) < sizeof(HANDLE) ? sizeof(double) : sizeof(HANDLE)); // fallback
        
    }

    
    extern double __complex_re;
    extern double __complex_im;
    extern bool __complex_ok;
    if (__complex_ok) {
        re = __complex_re; im = __complex_im;
        return true;
    }
    return false;
}





INT_PTR CALLBACK ComplexDlgProc(HWND dlg, UINT msg, WPARAM w, LPARAM l) {
    (void)l;
    switch (msg) {
    case WM_INITDIALOG:
        
        SetFocus(GetDlgItem(dlg, 1001));
        return TRUE;
    case WM_COMMAND:
        if (LOWORD(w) == IDOK) {
            char buf[128];
            GetDlgItemTextA(dlg, 1001, buf, sizeof(buf)); __complex_re = atof(buf);
            GetDlgItemTextA(dlg, 1002, buf, sizeof(buf)); __complex_im = atof(buf);
            __complex_ok = true;
            EndDialog(dlg, 1);
            return TRUE;
        } else if (LOWORD(w) == IDCANCEL) {
            __complex_ok = false;
            EndDialog(dlg, 0);
            return TRUE;
        }
        break;
    }
    return FALSE;
}


INT_PTR CALLBACK QuaternionDlgProc(HWND dlg, UINT msg, WPARAM w, LPARAM l) {
    (void)l;
    switch (msg) {
    case WM_INITDIALOG:
        SetFocus(GetDlgItem(dlg, 1001));
        return TRUE;
    case WM_COMMAND:
        if (LOWORD(w) == IDOK) {
            char buf[128];
            GetDlgItemTextA(dlg, 1001, buf, sizeof(buf)); __quat_w = atof(buf);
            GetDlgItemTextA(dlg, 1002, buf, sizeof(buf)); __quat_x = atof(buf);
            GetDlgItemTextA(dlg, 1003, buf, sizeof(buf)); __quat_y = atof(buf);
            GetDlgItemTextA(dlg, 1004, buf, sizeof(buf)); __quat_z = atof(buf);
            __quat_ok = true;
            EndDialog(dlg, 1);
            return TRUE;
        } else if (LOWORD(w) == IDCANCEL) {
            __quat_ok = false;
            EndDialog(dlg, 0);
            return TRUE;
        }
        break;
    }
    return FALSE;
}



bool DoComplexDialog(HWND parent, double &re, double &im) {
    
    const char* labs[] = { "Re:", "Im:" };
    std::vector<const char*> labels(labs, labs + 2);
    DLGTEMPLATE* tmpl = CreateDialogTemplate("Complex", labels, 280, 150);
    __complex_ok = false;
    INT_PTR res = DialogBoxIndirectParamA(NULL, tmpl, parent, ComplexDlgProc, 0);
    delete[] (BYTE*)tmpl;
    if (res == 1 && __complex_ok) { re = __complex_re; im = __complex_im; return true; }
    return false;
}

bool DoQuaternionDialog(HWND parent, double &w, double &x, double &y, double &z) {
    const char* labs[] = { "W:", "X:", "Y:", "Z:" };
    std::vector<const char*> labels(labs, labs + 4);
    DLGTEMPLATE* tmpl = CreateDialogTemplate("Quaternion", labels, 320, 220);
    __quat_ok = false;
    INT_PTR res = DialogBoxIndirectParamA(NULL, tmpl, parent, QuaternionDlgProc, 0);
    delete[] (BYTE*)tmpl;
    if (res == 1 && __quat_ok) { w = __quat_w; x = __quat_x; y = __quat_y; z = __quat_z; return true; }
    return false;
}



LRESULT CALLBACK HistoryWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE: {
        hHistoryList = CreateWindowA("listbox", "", WS_CHILD | WS_VISIBLE | WS_BORDER | LBS_NOTIFY | WS_VSCROLL, 10,10,330,300, hwnd, (HMENU)500, NULL, NULL);
        CreateWindowA("button", "Показать", WS_CHILD | WS_VISIBLE, 350, 10, 100, 30, hwnd, (HMENU)501, NULL, NULL);
        CreateWindowA("button", "Удалить", WS_CHILD | WS_VISIBLE, 350, 50, 100, 30, hwnd, (HMENU)502, NULL, NULL);
        CreateWindowA("button", "Сохранить", WS_CHILD | WS_VISIBLE, 350, 90, 100, 30, hwnd, (HMENU)503, NULL, NULL);
        CreateWindowA("button", "Загрузить", WS_CHILD | WS_VISIBLE, 350, 130, 100, 30, hwnd, (HMENU)504, NULL, NULL);
        RefreshHistoryWindow();
        break;
    }
    case WM_COMMAND: {
        int id = LOWORD(wParam);
        if (id == 501) {
            int sel = (int)SendMessageA(hHistoryList, LB_GETCURSEL, 0, 0);
            if (sel == LB_ERR) break;
            char buf[512]; SendMessageA(hHistoryList, LB_GETTEXT, sel, (LPARAM)buf);
            ShowStackContent(std::string(buf));
        } else if (id == 502) {
            int sel = (int)SendMessageA(hHistoryList, LB_GETCURSEL, 0, 0);
            if (sel == LB_ERR) break;
            char buf[512]; SendMessageA(hHistoryList, LB_GETTEXT, sel, (LPARAM)buf);
            gui_history.remove(std::string(buf));
            RefreshHistoryWindow();
        } else if (id == 503) {
            gui_history.save("history.txt");
            MessageBoxA(hwnd, "Saved to history.txt", "OK", MB_OK);
        } else if (id == 504) {
            gui_history.load("history.txt");
            RefreshHistoryWindow();
            MessageBoxA(hwnd, "Loaded from history.txt", "OK", MB_OK);
        }
        break;
    }
    case WM_CLOSE:
        DestroyWindow(hwnd);
        hHistoryWindow = NULL;
        hHistoryList = NULL;
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE: {
        CreateWindowA("button", "Add Complex", WS_CHILD | WS_VISIBLE, 10,10,120,30, hwnd, (HMENU)1, NULL, NULL);
        CreateWindowA("button", "Add Quaternion", WS_CHILD | WS_VISIBLE, 140,10,150,30, hwnd, (HMENU)2, NULL, NULL);
        CreateWindowA("button", "+", WS_CHILD | WS_VISIBLE, 10,50,40,30, hwnd, (HMENU)3, NULL, NULL);
        CreateWindowA("button", "-", WS_CHILD | WS_VISIBLE, 60,50,40,30, hwnd, (HMENU)4, NULL, NULL);
        CreateWindowA("button", "*", WS_CHILD | WS_VISIBLE, 110,50,40,30, hwnd, (HMENU)5, NULL, NULL);
        CreateWindowA("button", "/", WS_CHILD | WS_VISIBLE, 160,50,40,30, hwnd, (HMENU)6, NULL, NULL);
        CreateWindowA("button", "Save to history", WS_CHILD | WS_VISIBLE, 210,50,150,30, hwnd, (HMENU)7, NULL, NULL);
        CreateWindowA("button", "History", WS_CHILD | WS_VISIBLE, 370,50,80,30, hwnd, (HMENU)200, NULL, NULL);
        hList = CreateWindowA("listbox", "", WS_CHILD | WS_VISIBLE | WS_BORDER | LBS_NOTIFY | WS_VSCROLL, 10,100,440,350, hwnd, (HMENU)100, NULL, NULL);
        RefreshStack();
        break;
    }
    case WM_COMMAND: {
        int id = LOWORD(wParam);
        try {
            if (id == 1) {
                double re=0, im=0;
                if (DoComplexDialog(hwnd, re, im)) {
                    gui_calc.push(std::make_shared<Complex>(re, im));
                    RefreshStack();
                }
            } else if (id == 2) {
                double w=0,x=0,y=0,z=0;
                if (DoQuaternionDialog(hwnd, w, x, y, z)) {
                    gui_calc.push(std::make_shared<Quaternion>(w,x,y,z));
                    RefreshStack();
                }
            } else if (id == 3) { gui_calc.performOperation('+'); RefreshStack(); }
            else if (id == 4) { gui_calc.performOperation('-'); RefreshStack(); }
            else if (id == 5) { gui_calc.performOperation('*'); RefreshStack(); }
            else if (id == 6) { gui_calc.performOperation('/'); RefreshStack(); }
            else if (id == 7) {
                time_t t = time(nullptr);
                char tb[64]; strftime(tb, sizeof(tb), "%Y-%m-%d %H:%M:%S", localtime(&t));
                gui_history.add(std::string(tb), gui_calc.snapshot());
                MessageBoxA(hwnd, "Saved to history", "OK", MB_OK);
            }
            else if (id == 200) {
                if (!hHistoryWindow) {
                    WNDCLASSA wc = {0};
                    wc.lpfnWndProc = HistoryWndProc;
                    wc.lpszClassName = "HistWClass2";
                    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
                    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
                    RegisterClassA(&wc);
                    hHistoryWindow = CreateWindowA("HistWClass2", "History", WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME, 520, 180, 480, 380, NULL, NULL, NULL, NULL);
                    ShowWindow(hHistoryWindow, SW_SHOW);
                    UpdateWindow(hHistoryWindow);
                } else {
                    ShowWindow(hHistoryWindow, SW_SHOW);
                }
            }
        } catch (const std::exception& ex) {
            MessageBoxA(hwnd, ex.what(), "Error", MB_OK | MB_ICONERROR);
        }
        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}


int start_gui_mode() {
    WNDCLASSA wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.lpszClassName = "CalcMainClass";
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    RegisterClassA(&wc);
    HWND wnd = CreateWindowA("CalcMainClass", "Calculator (GUI)", WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME, 200,100,520,580, NULL, NULL, NULL, NULL);
    ShowWindow(wnd, SW_SHOW);
    UpdateWindow(wnd);
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}
