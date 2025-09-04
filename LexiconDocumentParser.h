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
    Unknown2E               = 0x2E,          // Неизвестная команда ('.')
    UnknownE8               = 0xE8,          // Неизвестная команда ('?')
    TextDecorationUnderline = 0x5F,          // Включение подчеркивания ('_')
    // Команда выбора шрифта
    SelectFont0             = 0x30,
    SelectFont1             = 0x31,
    SelectFont2             = 0x32,
    SelectFont3             = 0x33,
    SelectFont4             = 0x34,
    SelectFont5             = 0x35,
    SelectFont6             = 0x36,
    SelectFont7             = 0x37,
    SelectFont8             = 0x38,
    SelectFont9             = 0x39,
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
private:
    std::shared_ptr<LexiconDocument> m_doc;    // Выходной документ
    ParserState m_state;                       // Состояние парсера
    wxMemoryBuffer m_buffer;                   // Строковый кеш
    wxMemoryBuffer m_data;                     // Все данные
    uint8_t m_font_index;                      // Текущий шрифт
    uint32_t m_col;                            // Строка
    uint32_t m_row;                            // Столбец
};

#endif // LEXICON_DOCUMENT_H
