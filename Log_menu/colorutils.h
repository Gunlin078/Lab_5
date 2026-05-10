#ifndef COLORUTILS_H
#define COLORUTILS_H

#include <QWidget>
#include <QPushButton>
#include <QRandomGenerator>
#include <QKeyEvent>          // Для обработки событий на клавиатуре
#include <QRegularExpression> // Для работы с регулярными выражениями
#include <QResource>          // Для работы с ресурсами
#include <QMap>               // Для хранения исходных стилей

class ColorUtils : public QWidget
{
    Q_OBJECT
public:
    explicit ColorUtils(QWidget *parent = nullptr);
    // Основная функция
    void change_color(QWidget* window);

private:
    void record_Buttons(QWidget* widget, QMap<QString, QString>& styles);          // Запись всех кнопок в контейнер(Имя -> StyleSheet)
    void remove_Buttons(QMap<QString, QString>& original_styles, QWidget* widget); // Вставления в кнопки изначальных стилей из контейнера
    bool   isColorSimilar(const QColor &color1, const QColor &color2, int threshold = 150); // Проверяет, похожи ли два цвета
    QColor extractColorFromStyleSheet(const QString &styleSheet);                           // Извлекает цвет из строки стилей
    QColor generateRandomColor(const QColor &excludeColor, int threshold = 150);            // Генерирует случайный цвет
    QColor getComplementaryColor(const QColor &color);                                     // Возвращает комплементарный цвет
};

#endif // COLORUTILS_H
