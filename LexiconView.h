#ifndef LEXICON_VIEW_H
#define LEXICON_VIEW_H

#include <wx/wx.h>
#include <wx/panel.h>
#include <wx/dcbuffer.h>

#include "LexiconDocument.h"
#include "FontManager.h"
#include "Debug.h"

class LexiconView : public wxScrolledWindow {
public:
    LexiconView(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize);
    void SetDocument(std::shared_ptr<LexiconDocument> doc);

    void OnPaint(wxPaintEvent& event);
    void OnEraseBackground(wxEraseEvent& event);
    void OnSize(wxSizeEvent& event);

protected:
    void DrawContent(wxDC& dc);
    void OnDraw(wxDC& dc, const wxRect& visibleRect);
    void renderText(wxDC& dc, uint32_t posX, uint32_t posY, const TextNode* node);
    void renderChar(wxDC& dc, uint32_t posX, uint32_t posY, uint8_t ch, wxBitmap source);

private:
    wxDECLARE_EVENT_TABLE();

protected:
    std::shared_ptr<LexiconDocument> m_content;
    std::unique_ptr<FontManager> m_manager;
    bool m_mode_show_chunks;
};

#endif // LEXICON_VIEW_H
