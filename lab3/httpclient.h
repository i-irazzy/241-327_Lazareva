#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include "raceresult.h"

// ──────────────────────────────────────────────
//  ПАТТЕРН «СИНГЛТОН»
//  Единственный экземпляр HTTP-клиента на всё приложение.
//  Доступ через HttpClient::instance()
// ──────────────────────────────────────────────
class HttpClient : public QObject
{
    Q_OBJECT

public:
    // Получить единственный экземпляр (Singleton)
    static HttpClient* instance();

    // ──────────────────────────────────────────
    //  ПАТТЕРН «АДАПТЕР»
    //  Адаптирует высокоуровневые REST-операции
    //  к низкоуровневому Qt Network API.
    //  Каждый метод — один из пяти REST-методов.
    // ──────────────────────────────────────────

    // GET /api/results/          — получить список
    void getList();

    // POST /api/results/         — создать запись
    void create(const RaceResult &r);

    // GET /api/results/{id}/     — получить одну запись
    void getById(int id);

    // PUT /api/results/{id}/     — полное обновление
    void update(int id, const RaceResult &r);

    // DELETE /api/results/{id}/  — удалить запись
    void deleteById(int id);

signals:
    // Асинхронный сигнал: сервер ответил — передаём текст в UI
    void responseReady(const QString &text);
    void errorOccurred(const QString &errorText);

private:
    // Приватный конструктор — запрещает создание снаружи (Singleton)
    explicit HttpClient(QObject *parent = nullptr);

    // Запрещаем копирование (Singleton)
    HttpClient(const HttpClient&)            = delete;
    HttpClient& operator=(const HttpClient&) = delete;

    // ── внутренние вспомогательные методы адаптера ──
    QNetworkRequest makeRequest(const QString &path) const;
    void            connectReply(QNetworkReply *reply);

    QNetworkAccessManager *m_manager;
    const QString          m_baseUrl = "http://localhost:80/api/results/";
};

#endif // HTTPCLIENT_H
