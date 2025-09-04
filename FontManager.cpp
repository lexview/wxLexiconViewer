
#include "Font0.h"
#include "Font1.h"
#include "Font2.h"
#include "Font3.h"
#include "Font4.h"
#include "Font5.h"
#include "Font6.h"
#include "Font7.h"
#include "Font8.h"
#include "Font9.h"

#include "FontManager.h"

FontManager::FontManager() {

    RegisterFont(0, bin2c_Font0_fnt);
    RegisterFont(1, bin2c_Font1_fnt);
    RegisterFont(2, bin2c_Font2_fnt);
    RegisterFont(3, bin2c_Font3_fnt);
    RegisterFont(4, bin2c_Font4_fnt);
    RegisterFont(5, bin2c_Font5_fnt);
    RegisterFont(6, bin2c_Font6_fnt);
    RegisterFont(7, bin2c_Font7_fnt);
    RegisterFont(8, bin2c_Font8_fnt);
    RegisterFont(9, bin2c_Font9_fnt);

}

FontManager::~FontManager() {
    // TODO: удалить шрифты...
}

const Font* FontManager::GetFontByName(wxString fontName) {
    for (Font* curFont: m_fonts) {
        if (curFont->m_name == fontName) {
            return curFont;
        }
    }
    wxLogWarning(wxT("Не удалось найти шрифт '%s'. Будет использован шрифт по умолчанию."), fontName);
    return m_fonts[0];
}

/**
 * Поиск и получение шрифта по его индексу
 *
 */
const Font* FontManager::GetFontByIndex(uint8_t indexFont) {
    wxString fontName = wxString::Format("%u", indexFont);
    return GetFontByName(fontName);
}

const uint8_t FontHeight = 19;

/**
 * Рендеринг шрифта в wxBitmap
 *
 */
void FontManager::renderFont(wxMemoryDC& dc, const void* bytesFont) {

    dc.SetPen(wxPen(*wxBLACK, 1));
    for (unsigned index = 0; index < 256; index ++) {
        for (int y = 0; y < FontHeight; y ++) {
            uint8_t* ptr = (uint8_t*)bytesFont + index*FontHeight + y;
            uint8_t row = *ptr;
            if (row & 0b00000001) dc.DrawPoint(index * 8 + 7, y);
            if (row & 0b00000010) dc.DrawPoint(index * 8 + 6, y);
            if (row & 0b00000100) dc.DrawPoint(index * 8 + 5, y);
            if (row & 0b00001000) dc.DrawPoint(index * 8 + 4, y);
            if (row & 0b00010000) dc.DrawPoint(index * 8 + 3, y);
            if (row & 0b00100000) dc.DrawPoint(index * 8 + 2, y);
            if (row & 0b01000000) dc.DrawPoint(index * 8 + 1, y);
            if (row & 0b10000000) dc.DrawPoint(index * 8 + 0, y);
        }
    }

}

/**
 * Регистрация шрифта в FontManager
 *
 */
void FontManager::RegisterFont(uint8_t indexFont, const void* bytesFont) {
    const int width = 256 * 8;
    const int height = 19;

    // Создаем bitmap
    wxBitmap bitmap(width, height, 24);

    // Создаем MemoryDC для рисования
    wxMemoryDC memDC;
    memDC.SelectObject(bitmap);

    // Очищаем фон
    memDC.SetBackground(*wxWHITE_BRUSH);
    memDC.Clear();

    // Рисуем сложную графику
    renderFont(memDC, bytesFont);

    // Отменяем выбор bitmap (важный шаг!)
    memDC.SelectObject(wxNullBitmap);

    // Отладка (сохраняем шрифт в файл)
//    wxString debugName = wxString::Format("Font%u.png", indexFont);
//    bitmap.SaveFile(debugName, wxBITMAP_TYPE_PNG);

    // Регистрируем шрифт в реестре шрифтов
    wxString fontName = wxString::Format("%u", indexFont);
    Font *curFont = new Font(fontName, bitmap);
    m_fonts.push_back(curFont);

}

Font::Font(wxString fontName, wxBitmap bitmapFont) {
    m_name = fontName;
    m_font = bitmapFont;
}
