#ifndef LEXICON_FRAME_H
#define LEXICON_FRAME_H

#include <wx/wx.h>
#include <wx/panel.h>
#include <wx/wfstream.h>

#include "LexiconView.h"
#include "LexiconDocumentParser.h"
#include "Version.h"

class MyFrame : public wxFrame {

public:
    MyFrame();

private:
    void OnOpen(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);

private:
    LexiconView* m_canvas; // Владельцем станет компановщик wxWidgets


};

#endif // LEXICON_FRAME_H
