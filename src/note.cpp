#include "note.h"
#include "dout.h"
#include "color.h"


Note::Note(Data* data, App* pApp)
        : m_pApp(pApp)
        , Fl_Window(data->x, data->y, data->w, data->h, data->title.c_str())
        , editor(0, 0, data->w, data->h)
        , fl_color(ColorUtil::str2color(data->str_color))
        , m_data(data)
{
    setState(true);
    resizable(editor);

    buffer.text(m_data->text.c_str());
    editor.buffer(&buffer);
    editor.color(fl_color);

    end();

    Fl_Window::show();
    setIcon("menu_icon.ico");
    createMenu();
    setWinProc();

    editor.callback(textChangedCallback, this);
    Fl::add_timeout(5.0, updateCheckerCallback, this);
}

Note::~Note() { dbgout(""); editor.buffer(nullptr); }

//Callbacks
void Note::textChangedCallback(Fl_Widget* widget, void* userdata) {
    Note* note = static_cast<Note*>(userdata);
    note->isTextChanged = true;  
    note->UpdateData();
}

void Note::updateCheckerCallback(void* userdata) {
    Note* note = static_cast<Note*>(userdata);
    note->UpdateData();
    Fl::add_timeout(5.0, updateCheckerCallback, userdata);
}

int  Note::getId() const { return m_data->id; }
void Note::setState(bool state) { m_data->state = state; }
bool Note::getState() { return m_data->state; }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Win32 Icon & Menu process methods Implementation of Note
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
HWND Note::getHWND() { return fl_xid(this); }

void Note::setIcon(const std::string& iconPath) { dbgout("");
    HICON hIcon = (HICON)LoadImageA(nullptr, iconPath.c_str(), IMAGE_ICON, 32, 32, LR_LOADFROMFILE);
    if (hIcon) {
        SendMessage(getHWND(), WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
        SendMessage(getHWND(), WM_SETICON, ICON_BIG, (LPARAM)hIcon);
    }
}

void Note::createMenu() { dbgout("");
    hMenu = CreatePopupMenu();
    AppendMenuW(hMenu, MF_STRING, 1001, L"New");
    AppendMenuW(hMenu, MF_STRING, 1002, L"Open");
    AppendMenuW(hMenu, MF_STRING, 1003, L"Delete");
    AppendMenuW(hMenu, MF_STRING, 1004, L"Exit");
}

void Note::showMenu() { dbgout("");
    if (!hMenu) { createMenu(); }

    HWND hwnd = getHWND();
    RECT clientRect;
    GetClientRect(hwnd, &clientRect);
    POINT topLeft = { clientRect.left, clientRect.top };
    ClientToScreen(hwnd, &topLeft);

    int cmd = TrackPopupMenu(hMenu, TPM_RETURNCMD | TPM_LEFTBUTTON, topLeft.x, topLeft.y, 0, hwnd, nullptr);
    if (cmd > 0) { handleMenu(cmd); }
}

void Note::handleMenu(int command) { dbgout("");
    switch (command) {
        case 1001: action_new();    break;
        case 1002: action_open();   break;
        case 1003: action_delete(); break;
        case 1004: action_exit();   break;
    }
}

LRESULT CALLBACK Note::WinProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    Note* view = reinterpret_cast<Note*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

    if (view) {
        switch (uMsg) {
            case WM_NCLBUTTONDOWN: if (wParam == HTSYSMENU)               { view->showMenu(); return 0; } break;
            case WM_SYSCOMMAND:    if ((wParam & 0xFFF0) == SC_MOUSEMENU) { view->showMenu(); return 0; } break;
            case WM_CLOSE: 
                view->action_close();
                return DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
        return CallWindowProc(view->winProc, hwnd, uMsg, wParam, lParam);
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void Note::setWinProc() {
    HWND hwnd = getHWND();
    if (hwnd == nullptr) { std::cerr << "Error: Cannot get win descriptor HWND" << std::endl; return; }

    if (winProc == nullptr) {
        winProc = (WNDPROC)SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)WinProc);
        if (winProc == nullptr) { std::cerr << "Error: Failed to set custom win proc" << std::endl;} 
    }

    SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)this);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Resize  Implementation of Note
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void Note::resize(int X, int Y, int W, int H) { dbgout("");
    if (m_data->x != X || m_data->y != Y || m_data->w != W || m_data->h != H) {
        Fl_Window::resize(X, Y, W, H);
        isPositionChanged = (m_data->x != X || m_data->y != Y);
        isSizeChanged = (m_data->w != W || m_data->h != H);
        
        // m_data->x = X;m_data->y = Y;m_data->w = W;m_data->h = H;
        UpdateData();
    }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Update, Save, Load methods Implementation of Note
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void Note::UpdateData() { dbgout("");
    bool needsSave = false;

    if (isStateChanged) { needsSave = true; }

    if (isColorChanged) { 
        m_data->str_color = ColorUtil::color2str(fl_color);
        isColorChanged = false;
        needsSave = true;
    }

    if (isTextChanged) {
        m_data->text = buffer.text();
        isTextChanged = false;
        needsSave = true;
    }

    if (isPositionChanged) {
        m_data->x = x();
        m_data->y = y();
        isPositionChanged = false;
        needsSave = true;
    }

    if (isSizeChanged) {
        m_data->w = w();
        m_data->h = h();
        isSizeChanged = false;
        needsSave = true;
    }

    if (needsSave) { m_pApp->UpdateData(m_data); }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Close & Menu Action methods Implementation of Note
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void Note::action_close() { dbgout("");
    setState(false); 
    isStateChanged = true; 
    UpdateData();
    hide();
}
void Note::action_new()    { dbgout(""); 
    int newX = x() + 20;
    int newY = y() + 20;
    m_pApp->CreateNote(nullptr, newX, newY); 
}

void Note::action_open()   { dbgout(""); m_pApp->OpenNote(); }
void Note::action_delete() { dbgout(""); m_pApp->DeleteNote(getId()); }
void Note::action_exit()   { dbgout(""); exit(0); }
