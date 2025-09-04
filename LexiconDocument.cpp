
#include "LexiconDocument.h"

// Документ

LexiconDocument::LexiconDocument() {
    wxLogDebug(wxT("Документ сконструирован"));
}

LexiconDocument::~LexiconDocument() {
    wxLogDebug(wxT("Документ разрушаем"));
    while (!m_chunks.empty()) {
        Node* node = m_chunks.front();
        m_chunks.erase(m_chunks.begin());
        delete node;
    }
}

void LexiconDocument::DrawText(const wxMemoryBuffer& content, uint8_t font_index) {
    TextNode* node = new TextNode(content, font_index);
    m_chunks.push_back(node);
}

void LexiconDocument::DrawNewLine() {
    LayoutNode* node = new LayoutNode();
    m_chunks.push_back(node);
}

// Узлы

TextNode::TextNode(const wxMemoryBuffer &content, uint8_t font_index) {
    m_content.AppendData(content.GetData(), content.GetDataLen());
    m_font_index = font_index;
}

NodeKind TextNode::kind() const {
    return TextType;
}

uint32_t TextNode::GetWidth() const {
    size_t size = m_content.GetDataLen();
    return 8 * size;
}

uint32_t TextNode::GetHeight() const {
    return 19;
}

//ImageNode:ImageNode(const wxString &path) : m_path(path) {
//}

//

LayoutNode::LayoutNode() {
}

NodeKind LayoutNode::kind() const {
    return ControlType;
}

uint32_t LayoutNode::GetWidth() const {
    return 0;
}

uint32_t LayoutNode::GetHeight() const {
    return 0;
}
