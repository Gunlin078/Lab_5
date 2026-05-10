#include "colorutils.h"

ColorUtils::ColorUtils(QWidget *parent)
    : QWidget{parent}
{}

// Извлекает цвет из строки стилей
QColor ColorUtils::extractColorFromStyleSheet(const QString &styleSheet)
{
    // Простой парсинг строки стилей для поиска цвета
    QColor defaultColor(240, 240, 240); // Светло-серый по умолчанию

    if (styleSheet.isEmpty()) {
        return defaultColor;
    }

    // Ищем паттерн background-color: rgb(r,g,b) или background-color: #RRGGBB
    QRegularExpression rgbRegex("background-color: rgb\\((\\d+),\\s*(\\d+),\\s*(\\d+)\\)");
    QRegularExpression hexRegex("background-color: (#[0-9A-Fa-f]{6})");

    // Обработка найденных совпадний
    QRegularExpressionMatch rgbMatch = rgbRegex.match(styleSheet);
    if (rgbMatch.hasMatch()) {
        int r = rgbMatch.captured(1).toInt();//первое число
        int g = rgbMatch.captured(2).toInt();
        int b = rgbMatch.captured(3).toInt();
        return QColor(r, g, b);
    }

    QRegularExpressionMatch hexMatch = hexRegex.match(styleSheet);
    if (hexMatch.hasMatch()/*было ли найдено хотя бы одно соответствие регулярному выражению*/) {
        return QColor(hexMatch.captured(1));//НЕХ код цвета
    }

    return defaultColor;
}

// Генерирует случайный цвет, исключая заданный цвет и его ближайшие оттенки
QColor ColorUtils::generateRandomColor(const QColor &excludeColor, int threshold)
{
    // Если нет цвета для исключения или он невалидный, просто генерируем случайный цвет
    if (!excludeColor.isValid()) {
        return QColor(
            QRandomGenerator::global()->bounded(256),
            QRandomGenerator::global()->bounded(256),
            QRandomGenerator::global()->bounded(256)
            );
    }

    const int MAX_ATTEMPTS = 5; // Максимальное количество попыток
    int attempts = 0;

    while (attempts < MAX_ATTEMPTS) {
        QColor newColor(
            QRandomGenerator::global()->bounded(256),
            QRandomGenerator::global()->bounded(256),
            QRandomGenerator::global()->bounded(256)
            );

        // Проверяем, не слишком ли близок новый цвет к исключаемому
        if (!isColorSimilar(newColor, excludeColor, threshold)) {
            return newColor;
        }

        attempts++;
    }

    // Если не удалось найти подходящий цвет, генерируем комплементарный
    qDebug() << "Could not find sufficiently different color, using complementary color";
    return getComplementaryColor(excludeColor);
}

// Проверяет, похожи ли два цвета
bool ColorUtils::isColorSimilar(const QColor &color1, const QColor &color2, int threshold)
{
    // Вычисляем расстояние в пространстве RGB
    int dr = color1.red() - color2.red();
    int dg = color1.green() - color2.green();
    int db = color1.blue() - color2.blue();

    double distance = std::sqrt(dr*dr + dg*dg + db*db);

    return distance < threshold;
}

// Возвращает комплементарный цвет
QColor ColorUtils::getComplementaryColor(const QColor &color)
{
    return QColor(255 - color.red(), 255 - color.green(), 255 - color.blue());
}

// Рекурсивная функция для записи всех кнопок в контейнер(Имя -> StyleSheet)
void ColorUtils::record_Buttons(QWidget* widget, QMap<QString, QString>& styles) {
    if (!widget) return;

    if (qobject_cast<QPushButton*>(widget)) {

        QString name = widget->objectName();
        styles[name] = widget->styleSheet(); // Создание записи о кнопке
    }

    for (QObject* child : widget->children()) {
        if (QWidget* childWidget = qobject_cast<QWidget*>(child)) { //если QObject преобразуется в QWidget
            record_Buttons(childWidget, styles);
        }
    }
}

// Рекурсивная функция для вставления в кнопки изначальных стилей из контейнера(Имя -> StyleSheet)
void ColorUtils::remove_Buttons(QMap<QString, QString>& original_styles, QWidget* widget) {
    if (!widget) return;

    if (qobject_cast<QPushButton*>(widget)) {

        QString name = widget->objectName();
        widget->setStyleSheet(original_styles[name]); // Создание записи о кнопке
    }

    for (QObject* child : widget->children()) {
        if (QWidget* childWidget = qobject_cast<QWidget*>(child)) { //если QObject преобразуется в QWidget
            remove_Buttons(original_styles, childWidget);
        }
    }
}

// Основная функция
//При нажатии на клавишу '[' меняет цвет всех кнопок в окне на случайный.
//При нажатии на ']' возвращает исходные стили
void ColorUtils::change_color(QWidget* window)
{
    if (!window) {
        qDebug() << "Pointer is not valid";
        return;
    }
    QMap<QString, QString> styles; // Хранилище стилей кнопок
    record_Buttons(window, styles);

    // Создаем динамический обработчик событий для данного окна
    class ColorChangeEventFilter : public QObject {
    public:
        ColorChangeEventFilter(QMap<QString, QString>& buttonStyles, QWidget* parent = nullptr)  //Конструктор
            : m_styles(buttonStyles), QObject(parent), utils(nullptr), m_targetWindow(parent){} //

        ColorUtils* utils;
        QWidget* m_targetWindow;

        // Переопределяем метод фильтрации событий
        bool eventFilter(QObject* obj, QEvent* event) override {
            if (event->type() == QEvent::KeyPress) // Если тип - нажатие клавиши
            {
                QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
                if (keyEvent->key() == Qt::Key_BracketLeft)       // Конкретизация: '['
                {
                    qDebug() << "Changing button colors...";

                    // Получаем текущее окно, на котором произошло событие
                    QWidget* currentWindow = qobject_cast<QWidget*>(obj);//если не является типом qwidget, то nullptr

                    if (!currentWindow) {
                        qDebug() << "Couldn't get window from event object";
                        return false;
                    }

                    QColor currentColor;

                    // Находим все кнопки в текущем окне
                    QList<QPushButton*> allButtons = currentWindow->findChildren<QPushButton*>();

                    // Если в окне есть кнопки, извлекаем цвет из первой попавшейся
                    if (!allButtons.isEmpty() && utils) {
                        QString styleSheet = allButtons.first()->styleSheet();
                        currentColor = utils->extractColorFromStyleSheet(styleSheet);
                        qDebug() << "Current color extracted:" << currentColor.name();
                    } else {
                        qDebug() << "No buttons found in the window";
                        return false; //////
                    }

                    // Генерация нового цвета
                    QColor newColor = utils->generateRandomColor(currentColor);
                    qDebug() << "Generated new color:" << newColor.name();

                    // Применяем новый цвет ко всем кнопкам в окне
                    for (QPushButton* button : allButtons) {
                        QString newStyleSheet = QString("background-color: rgb(%1, %2, %3);")
                        .arg(newColor.red())
                            .arg(newColor.green())
                            .arg(newColor.blue());

                        // Сохраняем существующий стиль, но заменяем цвет
                        QString currentStyle = button->styleSheet();
                        if (!currentStyle.isEmpty()) {
                            currentStyle.remove(QRegularExpression("background-color:[^;]+;"));
                            currentStyle = newStyleSheet + " " + currentStyle;
                            button->setStyleSheet(currentStyle);
                        } else {
                            button->setStyleSheet(newStyleSheet);
                        }
                    }

                    qDebug() << "Button colors changed to:" << newColor.name();
                    return true; // Событие обработано
                }
                else if (keyEvent->key() == Qt::Key_BracketRight) // Конкретизация: ']'
                {
                    utils->remove_Buttons(m_styles, m_targetWindow);
                    qDebug() << "The original color has been returned!";
                    return true;
                }
            }
            return false; // Событие не обработано, передаем дальше
        }
    private:
        QMap<QString, QString> m_styles;
    };

    // Создаем экземпляр фильтра событий
    ColorChangeEventFilter* eventfilter = new ColorChangeEventFilter(styles, window);
    eventfilter->utils = this; // Передаём указатель на текущий экземпляр ColorUtils

    window->installEventFilter(eventfilter); // Устанавливаю фильтр событий

    // Возвращаю фокус на окно и разрешаю ему получать события клавиатуры
    window->setFocusPolicy(Qt::StrongFocus);
}
