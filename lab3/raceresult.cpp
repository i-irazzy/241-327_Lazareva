#include "raceresult.h"
#include <QJsonObject>
#include <QJsonValue>

// Десериализация: QJsonObject → RaceResult
// Используем QJsonDocument / QJsonObject / QJsonValue согласно заданию ЛР3
RaceResult RaceResult::fromJson(const QJsonObject &obj)
{
    RaceResult r;
    r.id           = obj.value("id").toInt();
    r.season       = obj.value("season").toInt();
    r.race_name    = obj.value("race_name").toString();
    r.date         = QDate::fromString(obj.value("date").toString(), Qt::ISODate);
    r.driver       = obj.value("driver").toString();
    r.constructor_ = obj.value("constructor").toString();
    r.laps         = obj.value("laps").toInt();
    r.position     = obj.value("position").toInt();
    r.avg_pit_time = obj.value("avg_pit_time").toDouble();
    r.track_temp   = obj.value("track_temp").toDouble();
    return r;
}

// Сериализация: RaceResult → QJsonObject для тела запроса
QJsonObject RaceResult::toJson() const
{
    QJsonObject obj;
    obj["season"]       = season;
    obj["race_name"]    = race_name;
    obj["date"]         = date.toString(Qt::ISODate);
    obj["driver"]       = driver;
    obj["constructor"]  = constructor_;
    obj["laps"]         = laps;
    obj["position"]     = position;
    obj["avg_pit_time"] = avg_pit_time;
    obj["track_temp"]   = track_temp;
    return obj;
}

QString RaceResult::toString() const
{
    return QString(
        "ID: %1\n"
        "Сезон: %2\n"
        "Гонка: %3\n"
        "Дата: %4\n"
        "Пилот: %5\n"
        "Команда: %6\n"
        "Кругов: %7\n"
        "Позиция: %8\n"
        "Ср. пит-стоп: %9 с\n"
        "Темп. трассы: %10 °C"
    )
    .arg(id).arg(season).arg(race_name)
    .arg(date.toString("dd.MM.yyyy"))
    .arg(driver).arg(constructor_)
    .arg(laps).arg(position)
    .arg(avg_pit_time).arg(track_temp);
}
