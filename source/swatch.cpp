#include "swatch.h"
#include <QPainter>
#include <QPixmapCache>


QPixmap colorSwatch(const QColor &color, const QSize &size)
{
    QString key = QString("COLORSWATCH:%1:%2x%3").arg(color.name())
        .arg(size.width()).arg(size.height());
    QPixmap pixmap(size);
#if QT_VERSION >= 0x040600
    if (!QPixmapCache::find(key, &pixmap)) {
#else
    if (!QPixmapCache::find(key, pixmap)) {
#endif
        pixmap.fill(Qt::transparent);
        QPainter painter(&pixmap);
        painter.setRenderHints(QPainter::Antialiasing|
                               QPainter::TextAntialiasing);
        painter.setPen(Qt::NoPen);
        painter.setBrush(color);
        painter.drawEllipse(0, 0, size.width(), size.height());
        if (size.width() > 32) {
            painter.setFont(QFont("Helvetica",
                    qMax(8, size.height() - 6), QFont::Black));
            QColor textColor(color.darker());
            if (color.red() == color.green() &&
                color.red() == color.blue())
                textColor = (color.red() > 90 ? Qt::black
                                              : Qt::white);
            painter.setPen(textColor);
            painter.drawText(
                    QRectF(0, 0, size.width(), size.height()),
                    QObject::tr("C"), QTextOption(Qt::AlignCenter));
        }
        painter.end();
        QPixmapCache::insert(key, pixmap);
    }
    return pixmap;
}


QPixmap brushSwatch(const Qt::BrushStyle style, const QColor &color,
                    const QSize &size)
{
    QString key = QString("BRUSHSTYLESWATCH:%1:%2:%3x%4")
        .arg(static_cast<int>(style)).arg(color.name())
        .arg(size.width()).arg(size.height());
    QPixmap pixmap(size);
#if QT_VERSION >= 0x040600
    if (!QPixmapCache::find(key, &pixmap)) {
#else
    if (!QPixmapCache::find(key, pixmap)) {
#endif
        pixmap.fill(Qt::transparent);
        QPainter painter(&pixmap);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setPen(Qt::NoPen);
        painter.setBrush(QBrush(color, style));
        painter.drawRect(0, 0, size.width(), size.height());
        painter.end();
        QPixmapCache::insert(key, pixmap);
    }
    return pixmap;
}


QPixmap penStyleSwatch(const Qt::PenStyle style, const QColor &color,
                       const QSize &size)
{
    QString key = QString("PENSTYLESWATCH:%1:%2:%3x%4")
        .arg(static_cast<int>(style)).arg(color.name())
        .arg(size.width()).arg(size.height());
    QPixmap pixmap(size);
#if QT_VERSION >= 0x040600
    if (!QPixmapCache::find(key, &pixmap)) {
#else
    if (!QPixmapCache::find(key, pixmap)) {
#endif
        pixmap.fill(Qt::transparent);
        QPainter painter(&pixmap);
        QPen pen(style);
        pen.setColor(color);
        pen.setWidth(2);
        painter.setPen(pen);
        const int Y = size.height() / 2;
        painter.drawLine(0, Y, size.width(), Y);
        painter.end();
        QPixmapCache::insert(key, pixmap);
    }
    return pixmap;
}


QPixmap penCapSwatch(const Qt::PenCapStyle capStyle,
                     const QColor &color, const QSize &size)
{
    QString key = QString("PENCAPSTYLESWATCH:%1:%2:%3x%4")
        .arg(static_cast<int>(capStyle)).arg(color.name())
        .arg(size.width()).arg(size.height());
    QPixmap pixmap(size);
#if QT_VERSION >= 0x040600
    if (!QPixmapCache::find(key, &pixmap)) {
#else
    if (!QPixmapCache::find(key, pixmap)) {
#endif
        pixmap.fill(Qt::transparent);
        QPainter painter(&pixmap);
        painter.setRenderHint(QPainter::Antialiasing);
        QPen pen;
        pen.setCapStyle(Qt::SquareCap);
        pen.setWidthF(size.height() / 2.5);
        pen.setColor(Qt::white);
        painter.setPen(pen);
        const int Y = size.height() / 2;
        painter.drawLine(0, Y, size.width(), Y);
        pen.setColor(color);
        pen.setCapStyle(capStyle);
        painter.setPen(pen);
        painter.drawLine(size.width() / 2.5, Y, size.width(), Y);
        painter.end();
        QPixmapCache::insert(key, pixmap);
    }
    return pixmap;
}


QPixmap penJoinSwatch(const Qt::PenJoinStyle joinStyle,
                      const QColor &color, const QSize &size)
{
    QString key = QString("PENJOINSTYLESWATCH:%1:%2:%3x%4")
        .arg(static_cast<int>(joinStyle)).arg(color.name())
        .arg(size.width()).arg(size.height());
    QPixmap pixmap(size);
#if QT_VERSION >= 0x040600
    if (!QPixmapCache::find(key, &pixmap)) {
#else
    if (!QPixmapCache::find(key, pixmap)) {
#endif
        const double Indent = size.width() / 5.0;
        QPolygonF polygon;
        polygon << QPointF(Indent, Indent)
                << QPointF(Indent, size.height() - Indent)
                << QPointF(size.width() - Indent,
                           size.height() - Indent);
        pixmap.fill(Qt::transparent);
        QPainter painter(&pixmap);
        painter.setRenderHint(QPainter::Antialiasing);
        QPen pen;
        pen.setJoinStyle(joinStyle);
        pen.setColor(color);
        pen.setWidthF(size.height() / 2.5);
        painter.setPen(pen);
        painter.drawPolyline(polygon);
        painter.end();
        QPixmapCache::insert(key, pixmap);
    }
    return pixmap;
}
