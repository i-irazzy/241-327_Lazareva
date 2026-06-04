#include "mainwindow.h"
#include "httpclient.h"
#include "raceresult.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QScrollArea>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Lab3 — F1 Race Results REST Client");
    setMinimumSize(750, 700);

    QWidget *central = new QWidget(this);
    setCentralWidget(central);
    QVBoxLayout *root = new QVBoxLayout(central);
    root->setSpacing(10);
    root->setContentsMargins(12, 12, 12, 12);

    // ════════════════════════════════════════════
    //  БЛОК 1: Ввод ID (для Get / Update / Delete)
    // ════════════════════════════════════════════
    QGroupBox *idBox = new QGroupBox("ID записи (для Get / Update / Delete)");
    QHBoxLayout *idLayout = new QHBoxLayout(idBox);
    idLayout->addWidget(new QLabel("ID:"));
    m_idSpin = new QSpinBox;
    m_idSpin->setRange(1, 999999);
    m_idSpin->setValue(1);
    m_idSpin->setFixedWidth(100);
    idLayout->addWidget(m_idSpin);
    idLayout->addStretch();
    root->addWidget(idBox);

    // ════════════════════════════════════════════
    //  БЛОК 2: Поля данных (для Create / Update)
    // ════════════════════════════════════════════
    QGroupBox *dataBox = new QGroupBox("Данные записи (для Create / Update)");
    QGridLayout *grid = new QGridLayout(dataBox);

    auto addRow = [&](int row, const QString &label, QWidget *widget) {
        grid->addWidget(new QLabel(label), row, 0);
        grid->addWidget(widget, row, 1);
    };

    m_seasonSpin = new QSpinBox;
    m_seasonSpin->setRange(1950, 2100);
    m_seasonSpin->setValue(2024);
    addRow(0, "Сезон:", m_seasonSpin);

    m_raceNameEdit = new QLineEdit("Monaco Grand Prix");
    addRow(1, "Гонка:", m_raceNameEdit);

    m_dateEdit = new QDateEdit(QDate::currentDate());
    m_dateEdit->setDisplayFormat("yyyy-MM-dd");
    m_dateEdit->setCalendarPopup(true);
    addRow(2, "Дата:", m_dateEdit);

    m_driverEdit = new QLineEdit("Max Verstappen");
    addRow(3, "Пилот:", m_driverEdit);

    m_constructorEdit = new QLineEdit("Red Bull");
    addRow(4, "Команда:", m_constructorEdit);

    m_lapsSpin = new QSpinBox;
    m_lapsSpin->setRange(0, 100);
    m_lapsSpin->setValue(78);
    addRow(5, "Кругов:", m_lapsSpin);

    m_positionSpin = new QSpinBox;
    m_positionSpin->setRange(1, 20);
    m_positionSpin->setValue(1);
    addRow(6, "Позиция:", m_positionSpin);

    m_pitTimeSpin = new QDoubleSpinBox;
    m_pitTimeSpin->setRange(0.0, 60.0);
    m_pitTimeSpin->setValue(2.4);
    m_pitTimeSpin->setSuffix(" с");
    addRow(7, "Ср. пит-стоп:", m_pitTimeSpin);

    m_trackTempSpin = new QDoubleSpinBox;
    m_trackTempSpin->setRange(-10.0, 70.0);
    m_trackTempSpin->setValue(38.5);
    m_trackTempSpin->setSuffix(" °C");
    addRow(8, "Темп. трассы:", m_trackTempSpin);

    root->addWidget(dataBox);

    // ════════════════════════════════════════════
    //  БЛОК 3: Кнопки REST-методов
    // ════════════════════════════════════════════
    QGroupBox *btnBox = new QGroupBox("REST-методы");
    QHBoxLayout *btnLayout = new QHBoxLayout(btnBox);

    m_btnList   = new QPushButton("GET List");
    m_btnCreate = new QPushButton("POST Create");
    m_btnGet    = new QPushButton("GET by ID");
    m_btnUpdate = new QPushButton("PUT Update");
    m_btnDelete = new QPushButton("DELETE");

    // Стили для наглядности
    m_btnList->setStyleSheet("background:#2e7d32;color:white;font-weight:bold;padding:6px;");
    m_btnCreate->setStyleSheet("background:#1565c0;color:white;font-weight:bold;padding:6px;");
    m_btnGet->setStyleSheet("background:#00695c;color:white;font-weight:bold;padding:6px;");
    m_btnUpdate->setStyleSheet("background:#e65100;color:white;font-weight:bold;padding:6px;");
    m_btnDelete->setStyleSheet("background:#b71c1c;color:white;font-weight:bold;padding:6px;");

    btnLayout->addWidget(m_btnList);
    btnLayout->addWidget(m_btnCreate);
    btnLayout->addWidget(m_btnGet);
    btnLayout->addWidget(m_btnUpdate);
    btnLayout->addWidget(m_btnDelete);
    root->addWidget(btnBox);

    // ════════════════════════════════════════════
    //  БЛОК 4: Вывод ответа сервера
    // ════════════════════════════════════════════
    QGroupBox *outBox = new QGroupBox("Ответ сервера");
    QVBoxLayout *outLayout = new QVBoxLayout(outBox);
    m_output = new QTextEdit;
    m_output->setReadOnly(true);
    m_output->setPlaceholderText("Здесь появится ответ сервера...");
    m_output->setFont(QFont("Courier New", 10));
    outLayout->addWidget(m_output);
    root->addWidget(outBox);

    // ════════════════════════════════════════════
    //  Подключение кнопок к слотам
    // ════════════════════════════════════════════
    connect(m_btnList,   &QPushButton::clicked, this, &MainWindow::onGetList);
    connect(m_btnCreate, &QPushButton::clicked, this, &MainWindow::onCreate);
    connect(m_btnGet,    &QPushButton::clicked, this, &MainWindow::onGetById);
    connect(m_btnUpdate, &QPushButton::clicked, this, &MainWindow::onUpdate);
    connect(m_btnDelete, &QPushButton::clicked, this, &MainWindow::onDelete);

    // ════════════════════════════════════════════
    //  Асинхронные сигналы от HttpClient → UI
    //  Сигнал испускается ПОСЛЕ получения ответа
    //  сервера, не сразу при нажатии кнопки
    // ════════════════════════════════════════════
    connect(HttpClient::instance(), &HttpClient::responseReady,
            this, &MainWindow::showResponse);
    connect(HttpClient::instance(), &HttpClient::errorOccurred,
            this, &MainWindow::showError);
}

// ── Вспомогательный метод: собирает RaceResult из полей формы ──
static RaceResult resultFromForm(
    QSpinBox *season, QLineEdit *raceName, QDateEdit *date,
    QLineEdit *driver, QLineEdit *constructor_,
    QSpinBox *laps, QSpinBox *position,
    QDoubleSpinBox *pitTime, QDoubleSpinBox *trackTemp)
{
    RaceResult r;
    r.season       = season->value();
    r.race_name    = raceName->text();
    r.date         = date->date();
    r.driver       = driver->text();
    r.constructor_ = constructor_->text();
    r.laps         = laps->value();
    r.position     = position->value();
    r.avg_pit_time = pitTime->value();
    r.track_temp   = trackTemp->value();
    return r;
}

// ── Слоты кнопок ────────────────────────────────────────────────────────────

void MainWindow::onGetList()
{
    m_output->setText("Загрузка списка...");
    HttpClient::instance()->getList();          // асинхронный вызов
}

void MainWindow::onCreate()
{
    RaceResult r = resultFromForm(
        m_seasonSpin, m_raceNameEdit, m_dateEdit,
        m_driverEdit, m_constructorEdit,
        m_lapsSpin, m_positionSpin,
        m_pitTimeSpin, m_trackTempSpin);
    m_output->setText("Создание записи...");
    HttpClient::instance()->create(r);          // асинхронный вызов
}

void MainWindow::onGetById()
{
    m_output->setText("Получение записи...");
    HttpClient::instance()->getById(m_idSpin->value());
}

void MainWindow::onUpdate()
{
    RaceResult r = resultFromForm(
        m_seasonSpin, m_raceNameEdit, m_dateEdit,
        m_driverEdit, m_constructorEdit,
        m_lapsSpin, m_positionSpin,
        m_pitTimeSpin, m_trackTempSpin);
    m_output->setText("Обновление записи...");
    HttpClient::instance()->update(m_idSpin->value(), r);
}

void MainWindow::onDelete()
{
    m_output->setText("Удаление записи...");
    HttpClient::instance()->deleteById(m_idSpin->value());
}

// ── Слоты ответа (вызываются АСИНХРОННО после ответа сервера) ───────────────

void MainWindow::showResponse(const QString &text)
{
    m_output->setStyleSheet("");
    m_output->setText(text);
}

void MainWindow::showError(const QString &text)
{
    m_output->setStyleSheet("color: red;");
    m_output->setText(text);
}
