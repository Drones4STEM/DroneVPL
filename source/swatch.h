#ifndef SWATCH_H
#define SWATCH_H


#include <QPixmap>
#include <QSize>

class QColor;


static const QSize DefaultSwatchSize(24, 24);


QPixmap colorSwatch(const QColor &color,
                    const QSize &size=DefaultSwatchSize);
QPixmap brushSwatch(const Qt::BrushStyle style,
                    const QColor &color=Qt::black,
                    const QSize &size=DefaultSwatchSize);
QPixmap penStyleSwatch(const Qt::PenStyle style,
                       const QColor &color=Qt::black,
                       const QSize &size=DefaultSwatchSize);
QPixmap penCapSwatch(const Qt::PenCapStyle capStyle,
                     const QColor &color=Qt::black,
                     const QSize &size=DefaultSwatchSize);
QPixmap penJoinSwatch(const Qt::PenJoinStyle joinStyle,
                      const QColor &color=Qt::black,
                      const QSize &size=DefaultSwatchSize);

#endif // SWATCH_H
