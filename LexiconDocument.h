#ifndef LEXICON_DOCUEMNT_H
#define LEXICON_DOCUEMNT_H

#include <vector>
#include <cstdint>
#include <memory>
#include <wx/wx.h>
#include "Debug.h"

enum xFont {
    Font0 = 0,
    Font1 = 1,
    Font2 = 2,
    Font3 = 3,
    Font4 = 4,
    Font5 = 5,
    Font6 = 6,
    Font7 = 7,
    Font8 = 8,
    Font9 = 9
};

enum NodeKind {
    UnknownType = 0,     // Неизвестный тип
    TextType    = 1,     // Текстовый узел
    ControlType = 2,     // Управляющий узел
    ImageType   = 3,     // Узел с изображением
};

class Node {
public:
    virtual NodeKind kind() const = 0;
    virtual uint32_t GetWidth() const = 0;
    virtual uint32_t GetHeight() const = 0;
};

class TextNode : public Node {
public:
    TextNode(const wxMemoryBuffer &content, uint8_t font_index = 0);
    NodeKind kind() const override;
    uint32_t GetWidth() const override;
    uint32_t GetHeight() const override;

private:
public:
    wxMemoryBuffer m_content;       // Буфер с данными текстового узла
    uint8_t m_font_index;           // Идентификатор шрифта в таблице
};

class LayoutNode : public Node {
public:
    LayoutNode();
    NodeKind kind() const override;
    uint32_t GetWidth() const override;
    uint32_t GetHeight() const override;
};

//

class LexiconDocument {
public:
    LexiconDocument();
    ~LexiconDocument();
    void DrawText(const wxMemoryBuffer& content, uint8_t font_index = 0);
    void DrawNewLine();
public:
    std::vector<Node*> m_chunks;
private:

};

#endif // LEXICON_DOCUEMNT_H
