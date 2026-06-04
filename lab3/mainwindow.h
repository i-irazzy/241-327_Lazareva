#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QDateEdit>
#include <QLabel>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void onGetList();
    void onCreate();
    void onGetById();
    void onUpdate();
    void onDelete();

    void showResponse(const QString &text);
    void showError(const QString &text);

private:
    // ── Поля ввода данных новой записи ──
    QSpinBox      *m_seasonSpin;
    QLineEdit     *m_raceNameEdit;
    QDateEdit     *m_dateEdit;
    QLineEdit     *m_driverEdit;
    QLineEdit     *m_constructorEdit;
    QSpinBox      *m_lapsSpin;
    QSpinBox      *m_positionSpin;
    QDoubleSpinBox*m_pitTimeSpin;
    QDoubleSpinBox*m_trackTempSpin;

    // ── Поле ввода ID (для Get/Update/Delete) ──
    QSpinBox      *m_idSpin;

    // ── Кнопки REST-методов ──
    QPushButton   *m_btnList;
    QPushButton   *m_btnCreate;
    QPushButton   *m_btnGet;
    QPushButton   *m_btnUpdate;
    QPushButton   *m_btnDelete;

    // ── Поле вывода ответа сервера ──
    QTextEdit     *m_output;
};

#endif // MAINWINDOW_H
