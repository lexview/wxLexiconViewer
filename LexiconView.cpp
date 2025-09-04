
#include "LexiconView.h"

// Определяем таблицу событий
wxBEGIN_EVENT_TABLE(LexiconView, wxScrolledWindow)
    EVT_PAINT(LexiconView::OnPaint)
    EVT_ERASE_BACKGROUND(LexiconView::OnEraseBackground)
    EVT_SIZE(LexiconView::OnSize)
wxEND_EVENT_TABLE()

// Конструктор
LexiconView::LexiconView(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size)
    : wxScrolledWindow(parent, id, pos, size, wxVSCROLL | wxHSCROLL) {

    // Включаем двойную буферизацию
    SetBackgroundStyle(wxBG_STYLE_PAINT);

    // Устанавливаем цвет фона
    SetBackgroundColour(*wxWHITE);

    // Настраиваем прокрутку
    SetScrollbars(10, 10, 0, 0);

    // Инициализируем шрифты
    m_manager = std::make_unique<FontManager>();

    // Зануляем документ
    m_content = nullptr;
    
    // Отображать чанки
    m_mode_show_chunks = false;
}

void LexiconView::SetDocument(std::shared_ptr<LexiconDocument> doc) {

    // Присваиваем документ
    m_content = doc;

    // Вычисляем размер холста документа
    uint32_t width = 8 * 80;
    uint32_t height = 0;
    for (Node *node: doc->m_chunks) {
        if (node->kind() == ControlType) {
            height += 19;
        }
    }
    SetScrollbars(10, 10, width / 10, height / 10);

    // Обоновляем основную страницу
    Refresh();
}

void LexiconView::renderChar(wxDC& dc, uint32_t posX, uint32_t posY, uint8_t ch, wxBitmap source) {

    // Создаем DC для источника
    wxMemoryDC sourceDC;
    sourceDC.SelectObject(source);

    // Копируем прямоугольную область
    uint16_t charPos = 8 * ch;
    wxRect sourceRect(charPos + 0, 0, 8, 19);     // Область для копирования (x, y, width, height)
    wxPoint destPoint(posX, posY);                // Куда копировать

    dc.Blit(
        destPoint.x, destPoint.y,            // Целевые координаты
        sourceRect.width, sourceRect.height, // Размер
        &sourceDC,                           // Источник
        sourceRect.x, sourceRect.y,          // Координаты в источнике
        wxCOPY,                              // Режим копирования
        true                                 // Использовать маску если есть
    );

    // Не забудьте освободить DC
    sourceDC.SelectObject(wxNullBitmap);

}

void LexiconView::renderText(wxDC& dc, uint32_t posX, uint32_t posY, const TextNode* node) {

    uint32_t nodeWidth = node->GetWidth();
    uint32_t nodeHeight = node->GetHeight();

    // Рисуем заглушки для шрифта
    if (m_mode_show_chunks) {
        dc.SetPen(*wxBLACK_PEN);
        dc.SetBrush(*wxGREY_BRUSH);
        dc.DrawRectangle(posX, posY, nodeWidth, nodeHeight);
    }

    // Рендерим символы
    wxMemoryBuffer mem = node->m_content;
    uint8_t *data = static_cast<uint8_t*>(mem.GetData());
    size_t size = mem.GetDataLen();
    const Font *curFont = m_manager->GetFontByIndex(node->m_font_index);
    for (unsigned i=0; i < size; i++) {
        uint8_t ch = data[i];
        renderChar(dc, posX + 8*i, posY, ch, curFont->m_font);
    }

}

void LexiconView::OnDraw(wxDC& dc, const wxRect& visibleRect) {

    // Откладка
//    wxLogDebug(wxT("Рендерим только текст в координатах: (%u, %u) - (%u, %u)"),
//        visibleRect.x, visibleRect.y,
//        visibleRect.width, visibleRect.height
//    );

    // Очистка фона документа
    dc.SetBrush(*wxWHITE_BRUSH);
    dc.Clear();

    // Рендерим документ (в случае если документ существует)
    uint32_t posX = 0;
    uint32_t posY = 0;
    if (m_content != nullptr) {
        for (Node* node: m_content->m_chunks) {
            NodeKind nodeKind = node->kind();
//            wxLogDebug(wxT("Координаты узла: (%u, %u) тип = %x"), posX, posY, nodeKind);

            // Рендеринг текста
            if (nodeKind == TextType) {
                TextNode* textNode = dynamic_cast<TextNode*>(node);
                uint32_t nodeWidth = textNode->GetWidth();

                // Область пересечения
                wxRect nodeRect(posX, posY, nodeWidth, 19);

                // Рендерим текст
                if (nodeRect.Intersects(visibleRect)) {
                    renderText(dc, posX, posY, textNode);
                }

                // Пересчитываем координаты
                posX += nodeWidth;

            }

            // Управляющий рендеринг
            if (nodeKind == ControlType) {
                posX = 0;
                posY += 19;
            }

        }
    }


}

void LexiconView::DrawContent(wxDC& dc) {

    // Получаем размер клиентской области
    wxSize clientSize = GetClientSize();

    // Получаем позицию скролла
    int scrollX, scrollY;
    GetViewStart(&scrollX, &scrollY);

    // Конвертируем scroll units в пиксели
    int pixelsPerUnitX, pixelsPerUnitY;
    GetScrollPixelsPerUnit(&pixelsPerUnitX, &pixelsPerUnitY);

    int viewStartX = scrollX * pixelsPerUnitX;
    int viewStartY = scrollY * pixelsPerUnitY;

    // Вычисляем видимую область в координатах документа
    wxRect visibleRect(
        viewStartX,                     // X начало
        viewStartY,                     // Y начало  
        clientSize.GetWidth(),          // Ширина
        clientSize.GetHeight()          // Высота
    );

    // Теперь рендерим только элементы, попадающие в visibleRect
    OnDraw(dc, visibleRect);

}

void LexiconView::OnPaint(wxPaintEvent& event) {
    wxBufferedPaintDC dc(this);
    DoPrepareDC(dc);
    DrawContent(dc);
}

void LexiconView::OnEraseBackground(wxEraseEvent& event) {
    // Пустая функция, чтобы предотвратить стирание фона по умолчанию
    // Это уменьшает мерцание, особенно на Windows
}

// Обработчик изменения размера
void LexiconView::OnSize(wxSizeEvent& event) {
    Refresh();
    event.Skip();
}
