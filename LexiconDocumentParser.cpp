
#include "LexiconDocumentParser.h"

LexiconDocumentParser::LexiconDocumentParser() {
    m_doc = std::make_shared<LexiconDocument>();
    m_state = ParserText;
    m_col = 0;
    m_row = 0;
    m_font_index = 0;
}

LexiconDocumentParser::~LexiconDocumentParser() {
}

void LexiconDocumentParser::updatePosition(uint8_t ch) {
    if (ch == '\n') {
        m_col++;
        m_row = 0;
    } else {
        m_row++;
    }
}

void LexiconDocumentParser::processByte(uint8_t ch) {

    wxLogDebug(wxT("[%u:%u] Получен код 0x%02X (состояние = 0x%02X)"), m_col, m_row, ch, m_state);

    // Ообновляем позицию в файле
    updatePosition(ch);

    // Командный режим
    if (m_state == ParserCommand) {
        // Экранирование служебного символа
        if (ch == CommandMode) {
            m_buffer.AppendByte(ch);
            setState(ParserText);
            return;
        }
        // Выбор шрифта
        if (ch == SelectFont0) {
            storeText();
            setFont(0);
            setState(ParserText);
            return;
        } else if (ch == SelectFont1) {
            storeText();
            setFont(1);
            setState(ParserText);
            return;
        } else if (ch == SelectFont2) {
            storeText();
            setFont(2);
            setState(ParserText);
            return;
        } else if (ch == SelectFont3) {
            storeText();
            setFont(3);
            setState(ParserText);
            return;
        } else if (ch == SelectFont4) {
            storeText();
            setFont(4);
            setState(ParserText);
            return;
        } else if (ch == SelectFont5) {
            storeText();
            setFont(5);
            setState(ParserText);
            return;
        } else if (ch == SelectFont6) {
            storeText();
            setFont(6);
            setState(ParserText);
            return;
        } else if (ch == SelectFont7) {
            storeText();
            setFont(7);
            setState(ParserText);
            return;
        } else if (ch == SelectFont8) {
            storeText();
            setFont(8);
            setState(ParserText);
            return;
        } else if (ch == SelectFont9) {
            storeText();
            setFont(9);
            setState(ParserText);
            return;
        }
        // Включение подчеркивания
        if (ch == TextDecorationUnderline) {
            storeText();
            setState(ParserText);
            return;
        }
        // Неизвестная команда
        if (ch == Unknown2E) {
            storeText();
            setState(ParserText);
            return;
        }
        if (ch == UnknownE8) {
            storeText();
            setState(ParserText);
            return;
        }
        // Предцпреждение о неподдерживаемой команде
        wxLogWarning(wxT("Неподдерживаемая команда - 0x%02X"), ch);
    }


    if (m_state == ParserText) {
        if (ch == CommandMode) {
            setState(ParserCommand);
            return;
        }
        if (ch == '\r') {
            // TODO: игнорируем команду возврата каретки (CR).
            return;
        }
        if (ch == '\n') {
            // Обрабатываем операцию перевод строки (LF).
            storeText();
            storeLine();
            setFont(0);
            return;
        }
        m_buffer.AppendByte(ch);
    }

    if (m_state == ParserImage) {
    }

    if (m_state == ParserPadding) {
    }

}

/**
 * Сохраняем текстовый узел в дереве документа (DOM)
 *
 */
void LexiconDocumentParser::storeText() {
    if (m_buffer.IsEmpty()) {
        return;
    }
    wxLogDebug(wxT("Сохраняем клочек текста"));
    m_doc->DrawText(m_buffer, m_font_index);
    m_buffer.Clear();
}

void LexiconDocumentParser::storeLine() {
    wxLogDebug(wxT("Сохраняем перевод каретки"));
    m_doc->DrawNewLine();
}

void LexiconDocumentParser::setState(ParserState state) {
    if (m_state == state) {
        return;
    }
    wxLogDebug(wxT("Смена состояния парсера: %02X -> %02X"), m_state, state);
    m_state = state;
}

void LexiconDocumentParser::setFont(uint8_t font_index) {
    if (m_font_index == font_index) {
        return;
    }
    wxLogDebug(wxT("Смена шрифта: 0x%02X -> 0x%02X"), m_font_index, font_index);
    m_font_index = font_index;
}

bool LexiconDocumentParser::Parse(uint8_t* data, size_t dataSize) {

    // Поступил еще один блок данных
    if (dataSize > 0) {
        wxLogDebug(wxT("Разбираем кусок данных длинной %zu"), dataSize);
        m_data.AppendData(data, dataSize);
    }

    // Парсим данные
    const uint8_t* raw = static_cast<const uint8_t*>(m_data.GetData());
    size_t rawSize = m_data.GetDataLen();
    for (size_t i = 0; i < rawSize; ++i) {
        processByte(raw[i]);
    }
    m_data.Clear();

    return true;
}

std::shared_ptr<LexiconDocument> LexiconDocumentParser::GetDocument() {
    return m_doc;
}
