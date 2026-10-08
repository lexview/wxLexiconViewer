#ifndef LEXICON_DOCUMENT_H
#define LEXICON_DOCUMENT_H

#include <cstdint>
#include <cstddef>
#include <wx/wx.h>
#include <wx/log.h>

#include "LexiconDocument.h"

enum ParserState {
    ParserCommand  = -1,  // Командный режим
    ParserText     =  0,  // Разбираем текстовый блок
    ParserImage    =  1,  // Разбираем путь до PCX картинки
    ParserPadding  =  2,  // Разбираем строковый интервал
};

const uint8_t CommandMode = 0xFF;   // Символ перевода парсера в командный режим

enum Command {
    // Неизвестная команда
    UnderlineStart          = 0x5F,          // Включаем подчеркивание
    UnderlineEnd            = 0x2E,          // Выключаем подчеркивание
    SetLineSpacing          = 0xE8,          // "Шаг" - межстрочный интервал
    SelectFont0             = 0x30,          // Шрифт 0
    SelectFont1             = 0x31,          // Шрифт 1
    SelectFont2             = 0x32,          // Шрифт 2
    SelectFont3             = 0x33,          // Шрифт 3
    SelectFont4             = 0x34,          // Шрифт 4
    SelectFont5             = 0x35,          // Шрифт 5
    SelectFont6             = 0x36,          // Шрифт 6
    SelectFont7             = 0x37,          // Шрифт 7
    SelectFont8             = 0x38,          // Шрифт 8
    SelectFont9             = 0x39,          // Шрифт 9
};

class LexiconDocumentParser {
public:
    LexiconDocumentParser();
    ~LexiconDocumentParser();
    bool Parse(uint8_t* data, size_t length);
    std::shared_ptr<LexiconDocument> GetDocument();
protected:
    void storeText();
    void storeLine();
    void setFont(uint8_t fontIndex);
    void processByte(uint8_t ch);
    void setState(ParserState state);
    void updatePosition(uint8_t ch);
    void setUnderline(bool underline);
protected:
    bool parsePadding(uint8_t ch);             // Разбор управляющей команды "Шаг"
private:
    std::shared_ptr<LexiconDocument> m_doc;    // Выходной документ
    ParserState m_state;                       // Состояние парсера
    wxMemoryBuffer m_buffer;                   // Строковый кеш
    wxMemoryBuffer m_data;                     // Все данные
    wxString m_padding;                        // Аргумент команды «Шаг» (например "1.0")
    uint8_t m_font_index;                      // Текущий шрифт
    uint32_t m_col;                            // Строка
    uint32_t m_row;                            // Столбец
    bool m_swallowLF;                          // Поглотить LF, завершающий строку команды
    bool m_underline;                          // Подчеркивание
};

#endif // LEXICON_DOCUMENT_H
