#ifndef HTMLDELEGATE_H
#define HTMLDELEGATE_H

#include <QRect>
#include <QSize>
#include <QPainter>
#include <QModelIndex>
#include <QApplication>
#include <QTextDocument>
#include <QStyledItemDelegate>
#include <QStyleOptionViewItem>
#include <QStyleOptionViewItemV4>
#include <QAbstractTextDocumentLayout>

class HtmlDelegate : public QStyledItemDelegate
{
protected:
    void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    QSize sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const;
};

#endif // HTMLDELEGATE_H
