
#include "LexiconFrame.h"


MyFrame::MyFrame() : wxFrame(NULL, wxID_ANY, "Lexicon Viewer v2.0a") {

    // Указываем размер окна
    SetSize(wxSize(640, 480));

    // Главное меню
    wxMenuBar *menuBar = new wxMenuBar;

    // Меню "Файл"
    wxMenu *menuFile = new wxMenu();
    menuFile->Append(wxID_OPEN, wxT("&Открыть...\tCtrl-O"), "Открыть файл");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);
    menuBar->Append(menuFile, wxT("&Файл"));

  // Меню "Правка"
//    wxMenu* editMenu = new wxMenu();
//    editMenu->Append(wxID_COPY, "&Копировать\tCtrl+C");
//    editMenu->Append(wxID_PASTE, "&Вставить\tCtrl+V");
//    menuBar->Append(editMenu, wxT("&Правка"));

    // Меню "Помощь"
    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT, wxT("&О программе"));
    menuBar->Append(menuHelp, wxT("&Справка"));

    SetMenuBar( menuBar );

    // Строка состояния
    CreateStatusBar();
    SetStatusText(wxT("Добро пожаловать в Lexicon Viewer v2.0 для " OS_NAME));

    // Привязка функций
    Bind(wxEVT_MENU, &MyFrame::OnOpen, this, wxID_OPEN);
    Bind(wxEVT_MENU, &MyFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &MyFrame::OnExit, this, wxID_EXIT);

    // Панель для отрисовки Lexicon документа
    m_canvas = new LexiconView(this);
    m_canvas->SetBackgroundColour(wxColour(240, 240, 240));

    // Комановщик (передаем во владение наш просмотр документа)
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(m_canvas, 1, wxEXPAND | wxALL, 5);

    SetSizer(mainSizer);

}

void MyFrame::OnExit(wxCommandEvent& event) {
    Close(true);
}

void MyFrame::OnAbout(wxCommandEvent& event) {
    wxMessageBox(
        wxT("Разработано на C++ для факультета Информационных технологий\nРазработчик: Витольд Седышев <vit1251@mail.ru>"),
        wxT("About Lexicon Viewer v2.0"),
        wxOK | wxICON_INFORMATION
    );
}

void MyFrame::OnOpen(wxCommandEvent& event) {
    wxFileDialog openFileDialog(this, 
        wxT("Открыть файл"),
        "",
        "",
        wxT(
            "Документы Lexicon (*.lex)|*.lex|"
            "Текстовые файлы (*.txt)|*.txt|"
            "Все файлы (*.*)|*.*"
        ),
        wxFD_OPEN | wxFD_FILE_MUST_EXIST
    );
    if (openFileDialog.ShowModal() == wxID_CANCEL) {
        return;
    }

    // Получаем путь
    wxString path = openFileDialog.GetPath();
    if (path.IsEmpty()) {
        return;
    }

    // Разбираем Lexicon документ
    wxFile file(path, wxFile::read);
    if (!file.IsOpened()) {
        wxMessageBox(
            wxT("Не удалось открыть файл для чтения!"),
            wxT("Ошибка чтения документа"),
            wxOK | wxICON_ERROR
        );
        return;
    }

    // Читаем документ
    wxFileOffset size = file.Length();
    wxMemoryBuffer buffer(size);
    void* ptr = buffer.GetWriteBuf(size);
    ssize_t bytesRead = file.Read(ptr, size);
    // TODO - нужна обработка ошибок чтения файла...
    LexiconDocumentParser parser;
    if (!parser.Parse((uint8_t *)ptr, bytesRead)) {
        while (!parser.Parse(nullptr, 0));
    }

    // Сохраняем документ
    std::shared_ptr<LexiconDocument> doc = parser.GetDocument();
    m_canvas->SetDocument(doc);
    Refresh();

}
