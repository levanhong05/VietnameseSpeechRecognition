#include "htmldelegate.h"

void HtmlDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItemV4 optionV4 = option;
    initStyleOption(&optionV4, index);

    QStyle *style = optionV4.widget ? optionV4.widget->style() : QApplication::style();

    QTextDocument doc;
    doc.setDefaultFont(optionV4.font);
    doc.setHtml(optionV4.text);
    doc.setTextWidth(optionV4.rect.width());
    doc.setDocumentMargin(0);

    //Painting item without text
    optionV4.text = QString();
    style->drawControl(QStyle::CE_ItemViewItem, &optionV4, painter);

    QAbstractTextDocumentLayout::PaintContext ctx;

    //Highlighting text if item is selected
    if (optionV4.state & QStyle::State_Selected) {
        ctx.palette.setColor(QPalette::Text, optionV4.palette.color(QPalette::Active, QPalette::HighlightedText));
    } else {
        ctx.palette.setColor(QPalette::Text, optionV4.palette.color(QPalette::Active, QPalette::Text));
    }

    QRect textRect = style->subElementRect(QStyle::SE_ItemViewItemText, &optionV4);
    painter->save();

    //painter->translate(textRect.left() + 2, textRect.top() - 6);
    painter->translate(textRect.topLeft());
    painter->setClipRect(textRect.translated(-textRect.topLeft()));

    doc.documentLayout()->draw(painter, ctx);
    painter->restore();
}

QSize HtmlDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItemV4 optionV4 = option;
    initStyleOption(&optionV4, index);

    QTextDocument doc;
    doc.setDefaultFont(optionV4.font);
    doc.setHtml(optionV4.text);
    doc.setDocumentMargin(0);
    doc.setTextWidth(optionV4.rect.width());

    return QSize(doc.idealWidth(), doc.size().height());
}
