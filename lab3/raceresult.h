#ifndef RACERESULT_H
#define RACERESULT_H

#include <QString>
#include <QDate>
#include <QJsonObject>

// Модель данных — аналог Django-модели RaceResult из ЛР1
class RaceResult {
public:
    int      id         = 0;
    int      season     = 0;
    QString  race_name;
    QDate    date;
    QString  driver;
    QString  constructor_;   // 'constructor' — зарезервировано в C++
    int      laps       = 0;
    int      position   = 0;
    double   avg_pit_time = 0.0;
    double   track_temp   = 0.0;

    // Десериализация из JSON-объекта (ответ сервера)
    static RaceResult fromJson(const QJsonObject &obj);

    // Сериализация в JSON для отправки на сервер
    QJsonObject toJson() const;

    // Красивый вывод для qDebug / UI
    QString toString() const;
};

#endif // RACERESULT_H
