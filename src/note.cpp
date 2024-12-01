#include "note.h"
#include "dout.h"
#include "color.h"
#include "settings.h"
#include "open_note.h"

#include <FL/fl_ask.H> 
#include <FL/Fl_Choice.H>


Note::Note(Data* data, App* pApp)
    : Fl_Window(data->x, data->y, data->w, data->h, data->title.c_str())
    , m_Flcolor(ColorUtil::str2color(data->str_color))
    , m_FlEditor(0, 0, data->w, data->h)
    , m_data(data)
    , m_pApp(pApp)
{  
    SetState(true);

    SetupTxtEditor();

    end();

    Fl_Window::show();
    SetIcon("menu_icon.ico", "taskbar_icon.ico");
    CreateMenu();
    SetWinProc();

    Fl::add_timeout(1.0, UpdateCheckerCallback, this);
}

Note::~Note() { 
    m_FlEditor.buffer(nullptr); 
    m_FlBuffer.text(nullptr);
    Fl::remove_timeout(UpdateCheckerCallback, this);
}

void Note::SetupTxtEditor() {  
    if (m_FlEditor.buffer() != nullptr) return;
    resizable(m_FlEditor);
    m_FlBuffer.text(m_data->text.c_str());
    m_FlEditor.buffer(&m_FlBuffer);
    m_FlEditor.color(m_Flcolor);

    m_FlBuffer.add_modify_callback(TextChangedCallback, this);
    m_FlEditor.when(FL_WHEN_CHANGED);
}

// Callbacks
void Note::TextChangedCallback(int pos, int nInserted, int nDeleted, int nRestyled, const char* deletedText, void* userdata) {
    
    Note* note = static_cast<Note*>(userdata);
    note->isTextChanged = true; 
}

void Note::UpdateCheckerCallback(void* userdata) {
    Note* note = static_cast<Note*>(userdata);
    note->UpdateData();
    Fl::add_timeout(0.5, UpdateCheckerCallback, userdata);
}

// GetTers & SetTers
int  Note::GetID() const { return m_data->id; }

void Note::SetState(bool state) { m_data->state = state; }
bool Note::GetState() { return m_data->state; }

void Note::SetTitle(const std::string& title) {
    m_data->title = title;
    label(m_data->title.c_str());
    redraw();
}
std::string Note::GetTitle() const { return m_data->title; }

void Note::SetColor(Fl_Color color) {
    m_Flcolor = color;
    m_FlEditor.color(color);
    m_FlEditor.redraw();
}

Fl_Color Note::GetColor() { return m_Flcolor; }

// Flag Setters
void Note::SetColorChanged(bool val) { isColorChanged = val; }
void Note::SetTitleChanged(bool val) { isTitleChanged = val; }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Win32 Icon & Menu process methods Implementation of Note
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
HWND Note::GetHWND() { return fl_xid(this); }

void Note::SetIcon(const std::string& iconMenu, const std::string& iconTaskbar) {   
    HICON hicon_menu = (HICON)LoadImageA(nullptr, iconMenu.c_str(), IMAGE_ICON, 32, 32, LR_LOADFROMFILE);
    HICON hIcon_taskbar = (HICON)LoadImageA(nullptr, iconTaskbar.c_str(), IMAGE_ICON, 64, 64, LR_LOADFROMFILE);

    if (hicon_menu) { SendMessage(GetHWND(), WM_SETICON, ICON_SMALL, (LPARAM)hicon_menu); }
    if (hIcon_taskbar) { SendMessage(GetHWND(), WM_SETICON, ICON_BIG, (LPARAM)hIcon_taskbar); }
}

void Note::CreateMenu() { 
    hMenu = CreatePopupMenu();
    AppendMenuW(hMenu, MF_STRING, 1000, L"Settings");
    AppendMenuW(hMenu, MF_SEPARATOR, 0, nullptr);

    AppendMenuW(hMenu, MF_STRING, 1001, L"New");
    AppendMenuW(hMenu, MF_STRING, 1002, L"Open");
    AppendMenuW(hMenu, MF_STRING, 1003, L"Delete");
    AppendMenuW(hMenu, MF_STRING, 1004, L"Exit");
}

void Note::ShowMenu() { 
    if (!hMenu) { CreateMenu(); }

    HWND hwnd = GetHWND();
    RECT clientRect;
    GetClientRect(hwnd, &clientRect);
    POINT topLeft = { clientRect.left, clientRect.top };
    ClientToScreen(hwnd, &topLeft);

    int cmd = TrackPopupMenu(hMenu, TPM_RETURNCMD | TPM_LEFTBUTTON, topLeft.x, topLeft.y, 0, hwnd, nullptr);
    if (cmd > 0) { HandleMenu(cmd); }
}

void Note::HandleMenu(int command) { 
    switch (command) {
        case 1000: OpenSettings(); break;
        case 1001: NewNote(); break;
        case 1002: OpenNote(); break;
        case 1003: DeleteNote(); break;
        case 1004: ExitApp(); break;
    }
}

LRESULT CALLBACK Note::WinProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    Note* view = reinterpret_cast<Note*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

    if (view) {
        switch (uMsg) {
            case WM_NCLBUTTONDOWN: if (wParam == HTSYSMENU)               { view->ShowMenu(); return 0; } break;
            case WM_SYSCOMMAND:    if ((wParam & 0xFFF0) == SC_MOUSEMENU) { view->ShowMenu(); return 0; } break;
            case WM_CLOSE: 
                view->CloseNote();
                return 0;
        }
        return CallWindowProc(view->winProc, hwnd, uMsg, wParam, lParam);
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void Note::SetWinProc() {
    HWND hwnd = GetHWND();
    if (hwnd == nullptr) { std::cerr << "Error: Cannot get win descriptor HWND" << std::endl; return; }

    if (winProc == nullptr) {
        winProc = (WNDPROC)SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)WinProc);
        if (winProc == nullptr) { std::cerr << "Error: Failed to set custom win proc" << std::endl;} 
    }

    SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)this);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Override "resize" Implementation of Note
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
        m_data->text = m_FlBuffer.text();
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
void Note::CloseNote() { 
    SetState(false);
    isStateChanged = true;
    UpdateData();
    hide();
    if (m_pApp) m_pApp->DeleteNote(GetID(), this);
}

void Note::NewNote() {  
    int newX = x() + 20;
    int newY = y() + 20;
    m_pApp->CreateNote(nullptr, newX, newY); 
}

void Note::OpenSettings() {
    CSettings* pSettings = new CSettings(x(), y(), w(), 150, "Settings", this);
    pSettings->show();
}

void Note::OpenNote() {
    COpenNote* pOpenNote = new COpenNote(x(), y(), 300, 300, "Open Note", m_pApp);
    pOpenNote->Run();
}

void Note::DeleteNote() { 
    int response = fl_choice(
        "Are you sure you want to delete this note?",
        "Delete", //0     
        "Cancel", //1
        nullptr                                       
    );

    if (response == 0) { 
        m_pApp->DeleteNoteData(GetID());
    } 
}

void Note::ExitApp() { exit(0); }
