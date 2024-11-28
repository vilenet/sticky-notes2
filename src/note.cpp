#include "note.h"
#include "dout.h"
#include "color.h"
#include "settings.h"
#include "selector.h"

#include <FL/fl_ask.H> 
#include <FL/Fl_Choice.H>


Note::Note(Data* data, App* pApp)
        : m_pApp(pApp)
        , Fl_Window(data->x, data->y, data->w, data->h, data->title.c_str())
        , editor(0, 0, data->w, data->h)
        , m_Flcolor(ColorUtil::str2color(data->str_color))
        , m_data(data)
{
    setState(true);
    resizable(editor);

    buffer.text(m_data->text.c_str());
    editor.buffer(&buffer);
    editor.color(m_Flcolor);

    end();

    Fl_Window::show();
    setIcon("menu_icon.ico", "taskbar_icon.ico");
    createMenu();
    setWinProc();

    editor.callback(textChangedCallback, this);
    Fl::add_timeout(5.0, updateCheckerCallback, this);
}

Note::~Note() {  editor.buffer(nullptr); }

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

// getTers SetTers
int  Note::getId() const { return m_data->id; }

void Note::setState(bool state) { m_data->state = state; }
bool Note::getState() { return m_data->state; }

void Note::setTitle(const std::string& title) {
    m_data->title = title;
    label(m_data->title.c_str());
    redraw();
}
std::string Note::getTitle() const { return m_data->title; }

void Note::setColor(Fl_Color color) {
    m_Flcolor = color;
    editor.color(color);
    editor.redraw();
}
Fl_Color Note::getColor() { return m_Flcolor; }

// Markers
void Note::setColorChanged(bool val) { isColorChanged = val; }
void Note::setTitleChanged(bool val) { isTitleChanged = val; }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Win32 Icon & Menu process methods Implementation of Note
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
HWND Note::getHWND() { return fl_xid(this); }

void Note::setIcon(const std::string& iconMenu, const std::string& iconTaskbar) { 
    HICON hicon_menu = (HICON)LoadImageA(nullptr, iconMenu.c_str(), IMAGE_ICON, 32, 32, LR_LOADFROMFILE);
    HICON hIcon_taskbar = (HICON)LoadImageA(nullptr, iconTaskbar.c_str(), IMAGE_ICON, 64, 64, LR_LOADFROMFILE);

    if (hicon_menu) { SendMessage(getHWND(), WM_SETICON, ICON_SMALL, (LPARAM)hicon_menu); }
    if (hIcon_taskbar) { SendMessage(getHWND(), WM_SETICON, ICON_BIG, (LPARAM)hIcon_taskbar); }
}

void Note::createMenu() { 
    hMenu = CreatePopupMenu();
    AppendMenuW(hMenu, MF_STRING, 1000, L"Settings");
    AppendMenuW(hMenu, MF_SEPARATOR, 0, nullptr);

    AppendMenuW(hMenu, MF_STRING, 1001, L"New");
    AppendMenuW(hMenu, MF_STRING, 1002, L"Open");
    AppendMenuW(hMenu, MF_STRING, 1003, L"Delete");
    AppendMenuW(hMenu, MF_STRING, 1004, L"Exit");
}

void Note::showMenu() { 
    if (!hMenu) { createMenu(); }

    HWND hwnd = getHWND();
    RECT clientRect;
    GetClientRect(hwnd, &clientRect);
    POINT topLeft = { clientRect.left, clientRect.top };
    ClientToScreen(hwnd, &topLeft);

    int cmd = TrackPopupMenu(hMenu, TPM_RETURNCMD | TPM_LEFTBUTTON, topLeft.x, topLeft.y, 0, hwnd, nullptr);
    if (cmd > 0) { handleMenu(cmd); }
}

void Note::handleMenu(int command) { 
    switch (command) {
        case 1000: action_settings(); break;
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
                return 0;
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
void Note::resize(int X, int Y, int W, int H) { 
    if (m_data->x != X || m_data->y != Y || m_data->w != W || m_data->h != H) {
        Fl_Window::resize(X, Y, W, H);
        isPositionChanged = (m_data->x != X || m_data->y != Y);
        isSizeChanged = (m_data->w != W || m_data->h != H);
        UpdateData();
    }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Update, Save, Load methods Implementation of Note
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void Note::UpdateData() { 
    bool needsSave = false;

    if (isStateChanged) {
        isStateChanged = false;
        needsSave = true; 
    }

    if (isTitleChanged) { 
        isTitleChanged = false;
        needsSave = true; 
    }

    if (isColorChanged) {
        m_data->str_color = ColorUtil::color2str(m_Flcolor);
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
void Note::action_close() { 
    setState(false);
    isStateChanged = true;
    UpdateData();
    hide();
    if (m_pApp) m_pApp->DeleteNote(getId(), this);
}

void Note::action_new() {  
    int newX = x() + 20;
    int newY = y() + 20;
    m_pApp->CreateNote(nullptr, newX, newY); 
}

void Note::action_settings() {
    Settings* settings = new Settings(x(), y(), w(), this);
    settings->show();
}

void Note::action_open() {
    COpenNote* pOpenNote = new COpenNote(x(), y(), 300, 300, "Open Note", m_pApp);
    pOpenNote->Run();
}

void Note::action_delete() { 
    int response = fl_choice(
        "Are you sure you want to delete this note?",
        "Delete", //0     
        "Cancel", //1
        nullptr                                       
    );

    if (response == 0) { 
        m_pApp->DeleteNoteData(getId());
    } 
}
void Note::action_exit()   {  exit(0); }
