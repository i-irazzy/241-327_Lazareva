#include "httpclient.h"
#include <QNetworkRequest>
#include <QUrl>
#include <QDebug>
#include <QSslConfiguration>
#include <QSslSocket>

// ══════════════════════════════════════════════
//  СИНГЛТОН — реализация
// ══════════════════════════════════════════════

HttpClient* HttpClient::instance()
{
    // Статическая локальная переменная — создаётся один раз,
    // уничтожается при завершении программы (thread-safe в C++11+)
    static HttpClient self;
    return &self;
}

HttpClient::HttpClient(QObject *parent)
    : QObject(parent)
    , m_manager(new QNetworkAccessManager(this))
{
}

// ══════════════════════════════════════════════
//  АДАПТЕР — реализация пяти REST-методов
// ══════════════════════════════════════════════

// Вспомогательный метод: строит QNetworkRequest с нужными заголовками
QNetworkRequest HttpClient::makeRequest(const QString &path) const
{
    QNetworkRequest req(QUrl(m_baseUrl + path));
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QSslConfiguration sslConfig = QSslConfiguration::defaultConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    req.setSslConfiguration(sslConfig);

    return req;
}

// Вспомогательный метод: подключает сигнал finished к обработчику ответа.
// Именно здесь реализуется АСИНХРОННОСТЬ:
// sendRequest() возвращает управление немедленно,
// а слот вызывается позже — когда придёт ответ от сервера.
void HttpClient::connectReply(QNetworkReply *reply)
{
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        reply->deleteLater();   // освобождаем память после обработки

        if (reply->error() != QNetworkReply::NoError) {
            QString err = QString("Ошибка сети: %1").arg(reply->errorString());
            qDebug() << "[HttpClient] ERROR:" << err;
            emit errorOccurred(err);
            return;
        }

        QByteArray raw = reply->readAll();   // читаем тело ответа
        qDebug() << "[HttpClient] RAW response:" << raw;

        // ── Десериализация JSON ──────────────────────────────
        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(raw, &parseError);

        if (parseError.error != QJsonParseError::NoError) {
            // Ответ не JSON (например, 204 No Content при DELETE)
            QString msg = raw.isEmpty()
                ? "Операция выполнена успешно (нет тела ответа)"
                : QString::fromUtf8(raw);
            emit responseReady(msg);
            return;
        }

        QString output;

        if (doc.isArray()) {
            // GET /api/results/ → массив объектов
            QJsonArray arr = doc.array();
            output += QString("Получено записей: %1\n\n").arg(arr.size());
            for (const QJsonValue &val : arr) {
                RaceResult r = RaceResult::fromJson(val.toObject());
                output += r.toString() + "\n" + QString(40, '-') + "\n";
                qDebug() << "[HttpClient] Record:" << r.toString();
            }
        } else if (doc.isObject()) {
            // POST / GET {id} / PUT {id} → один объект
            RaceResult r = RaceResult::fromJson(doc.object());
            output = r.toString();
            qDebug() << "[HttpClient] Record:" << r.toString();
        }

        emit responseReady(output);
    });
}

// ── 1. LIST ─────────────────────────────────────────────────────────────────
// GET /api/results/
void HttpClient::getList()
{
    qDebug() << "[HttpClient] GET" << m_baseUrl;
    QNetworkReply *reply = m_manager->get(makeRequest(""));
    connectReply(reply);
}

// ── 2. CREATE ────────────────────────────────────────────────────────────────
// POST /api/results/
void HttpClient::create(const RaceResult &r)
{
    QByteArray body = QJsonDocument(r.toJson()).toJson(QJsonDocument::Compact);
    qDebug() << "[HttpClient] POST" << m_baseUrl << body;
    QNetworkReply *reply = m_manager->post(makeRequest(""), body);
    connectReply(reply);
}

// ── 3. RETRIEVE ──────────────────────────────────────────────────────────────
// GET /api/results/{id}/
void HttpClient::getById(int id)
{
    QString path = QString("%1/").arg(id);
    qDebug() << "[HttpClient] GET" << m_baseUrl + path;
    QNetworkReply *reply = m_manager->get(makeRequest(path));
    connectReply(reply);
}

// ── 4. UPDATE ────────────────────────────────────────────────────────────────
// PUT /api/results/{id}/
void HttpClient::update(int id, const RaceResult &r)
{
    QString path = QString("%1/").arg(id);
    QByteArray body = QJsonDocument(r.toJson()).toJson(QJsonDocument::Compact);
    qDebug() << "[HttpClient] PUT" << m_baseUrl + path << body;
    QNetworkReply *reply = m_manager->put(makeRequest(path), body);
    connectReply(reply);
}

// ── 5. DELETE ────────────────────────────────────────────────────────────────
// DELETE /api/results/{id}/
void HttpClient::deleteById(int id)
{
    QString path = QString("%1/").arg(id);
    qDebug() << "[HttpClient] DELETE" << m_baseUrl + path;
    QNetworkReply *reply = m_manager->deleteResource(makeRequest(path));
    connectReply(reply);
}
