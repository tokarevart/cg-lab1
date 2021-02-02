#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QRandomGenerator>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_gen_btn_clicked();
    void on_gen_builtin_btn_clicked();

    void on_thous_gen_builtin_btn_clicked();

    void on_thous_gen_btn_clicked();

    void on_gen_builtin_wid_btn_clicked();

    void on_thous_gen_builtin_wid_btn_clicked();

private:
    Ui::MainWindow *ui;
    QGraphicsScene* scene;
    QRandomGenerator rng;

    QPoint random_point(QRect rect);
    QPolygon random_triangle(QRect rect);
};
#endif // MAINWINDOW_H
