#ifndef FONT_MANAGER_H
#define FONT_MANAGER_H

#include <cstdint>
#include <vector>

#include <wx/log.h>
#include <wx/dcbuffer.h>

class Font {
public:
    Font(wxString name, wxBitmap bitmapFont);
public:
    wxString m_name;
    wxBitmap m_font;
private:
};

class FontManager {
public:
    FontManager();
    ~FontManager();
    const Font* GetFontByIndex(uint8_t indexFont);
    const Font* GetFontByName(wxString fontName);
    void RegisterFont(uint8_t indexFont, const void* bytesFont);

protected:
    void renderFont(wxMemoryDC& memDC, const void* bytesFont);

private:
    std::vector<Font*> m_fonts;
};

#endif // FONT_MANAGER_H
