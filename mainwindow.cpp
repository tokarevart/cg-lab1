#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QGraphicsItem>
#include <QPainter>
#include <QElapsedTimer>
#include <QVector2D>

QPoint MainWindow::random_point(QRect rect) {
    int x0, y0, x1, y1;
    rect.getCoords(&x0, &y0, &x1, &y1);
    return QPoint(rng.bounded(x0, x1), rng.bounded(y0, y1));
}

QPolygon MainWindow::random_triangle(QRect rect) {
    QPolygon polygon;
    for (int i = 0; i < 3; ++i) {
        polygon << random_point(rect);
    }
    return polygon;
}

int cross_product(QPoint v0, QPoint v1) {
    return v0.x() * v1.y() - v0.y() * v1.x();
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(this->size());
    scene = new QGraphicsScene();

    ui->graphicsView->setScene(scene);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

MainWindow::~MainWindow()
{
    delete ui;
}


//void MainWindow::on_gen_btn_clicked()
//{
//    QElapsedTimer t;
//    t.start();

//    QImage image(ui->graphicsView->size(), QImage::Format_RGB888);
//    image.fill(Qt::white);
//    QPainter painter(&image);
//    QPen pen(Qt::black);
//    painter.setPen(pen);

//    auto triangle = random_triangle(QRect({0, 0}, ui->graphicsView->size()));
//    QRect brect = triangle.boundingRect();
//    QPoint evecs[3];
//    evecs[0] = triangle[1] - triangle[0];
//    evecs[1] = triangle[2] - triangle[1];
//    evecs[2] = triangle[0] - triangle[2];

//    int ax, ay, w, h;
//    brect.getRect(&ax, &ay, &w, &h);
//    QPoint a = { ax, ay };
//    for (int y = 0; y < h; ++y) {
//        for (int x = 0; x < w; ++x) {
//            QPoint p = a + QPoint(x, y);
//            int cprods[3];
//            for (int k = 0; k < 3; ++k) {
//                cprods[k] = cross_product(p - triangle[k], evecs[k]);
//            }
//            if (   std::all_of(cprods, cprods + 3, [=](int cp){ return cp >= 0; })
//                || std::all_of(cprods, cprods + 3, [=](int cp){ return cp <= 0; })) {
//                QColor color = QColor::fromRgb(rng.generate());
//                QPen pen(color);
//                painter.setPen(pen);
//                painter.drawPoint(p);
//            }
//        }
//    }
//    painter.end();
//    scene->clear();
//    scene->setSceneRect(ui->graphicsView->rect());
//    scene->addPixmap(QPixmap::fromImage(image))->setPos(0, 0);
//    ui->time_label->setText("Time: " + QString::number(t.elapsed()) + " ms");
//}

void MainWindow::on_gen_btn_clicked()
{
    QElapsedTimer t;
    t.start();

    QImage image(ui->graphicsView->size(), QImage::Format_RGB32);
    image.fill(Qt::white);

    auto triangle = random_triangle(QRect({0, 0}, ui->graphicsView->size()));
    QRect brect = triangle.boundingRect();
    QPoint evecs[3];
    evecs[0] = triangle[1] - triangle[0];
    evecs[1] = triangle[2] - triangle[1];
    evecs[2] = triangle[0] - triangle[2];

    int ax, ay, w, h;
    brect.getRect(&ax, &ay, &w, &h);
    QPoint a = { ax, ay };
    QRgb* bits = reinterpret_cast<QRgb*>(image.bits());
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            QPoint p = a + QPoint(x, y);
            int cprods[3];
            for (int k = 0; k < 3; ++k) {
                cprods[k] = cross_product(p - triangle[k], evecs[k]);
            }
            if (   std::all_of(cprods, cprods + 3, [=](int cp){ return cp >= 0; })
                || std::all_of(cprods, cprods + 3, [=](int cp){ return cp <= 0; })) {
                bits[p.y() * image.width() + p.x()] = rng.generate(); // or QColor(Qt::red).rgb()
            }
        }
    }
    scene->clear();
    scene->setSceneRect(ui->graphicsView->rect());
    scene->addPixmap(QPixmap::fromImage(image))->setPos(0, 0);
    ui->time_label->setText("Time: " + QString::number(t.elapsed()) + " ms");
}

void MainWindow::on_gen_builtin_btn_clicked()
{
    QElapsedTimer t;
    t.start();
    auto triangle = random_triangle(QRect({0, 0}, ui->graphicsView->size()));
    QImage image(ui->graphicsView->size(), QImage::Format_RGB32);
    image.fill(Qt::white);
    QPainter painter(&image);
    QColor color = QColor::fromRgb(rng.generate());
    QBrush brush(color);
    QPen pen(Qt::black);
    painter.setBrush(brush);
    painter.setPen(pen);
    painter.drawPolygon(triangle);
    painter.end();
    scene->clear();
    scene->setSceneRect(ui->graphicsView->rect());
    scene->addPixmap(QPixmap::fromImage(image))->setPos(0, 0);
    ui->builtin_time_label->setText("Built-in time: " + QString::number(t.elapsed()) + " ms");
}

void MainWindow::on_thous_gen_btn_clicked()
{
    QElapsedTimer t;
    t.start();

    QImage image(ui->graphicsView->size(), QImage::Format_RGB32);
    image.fill(Qt::white);
    QPainter painter(&image);
    QPen pen(Qt::black);
    painter.setPen(pen);

    for (int triang_i = 0; triang_i < 1000; ++triang_i) {
        auto triangle = random_triangle(QRect({0, 0}, ui->graphicsView->size()));
        QRect brect = triangle.boundingRect();
        QPoint evecs[3];
        evecs[0] = triangle[1] - triangle[0];
        evecs[1] = triangle[2] - triangle[1];
        evecs[2] = triangle[0] - triangle[2];

        int ax, ay, w, h;
        brect.getRect(&ax, &ay, &w, &h);
        QPoint a = { ax, ay };
        // slow
//        for (int y = 0; y < h; ++y) {
//            for (int x = 0; x < w; ++x) {
//                QPoint p = a + QPoint(x, y);
//                int cprods[3];
//                for (int k = 0; k < 3; ++k) {
//                    cprods[k] = cross_product(p - triangle[k], evecs[k]);
//                }
//                if (   std::all_of(cprods, cprods + 3, [=](int cp){ return cp >= 0; })
//                    || std::all_of(cprods, cprods + 3, [=](int cp){ return cp <= 0; })) {
//                    QColor color = QColor::fromRgb(rng.generate());
//                    QPen pen(color);
//                    painter.setPen(pen);
//                    painter.drawPoint(p);
//                }
//            }
//        }
        QRgb* bits = reinterpret_cast<QRgb*>(image.bits());
        for (int y = 0; y < h; ++y) {
            for (int x = 0; x < w; ++x) {
                QPoint p = a + QPoint(x, y);
                int cprods[3];
                for (int k = 0; k < 3; ++k) {
                    cprods[k] = cross_product(p - triangle[k], evecs[k]);
                }
                if (   std::all_of(cprods, cprods + 3, [=](int cp){ return cp >= 0; })
                    || std::all_of(cprods, cprods + 3, [=](int cp){ return cp <= 0; })) {
                    bits[p.y() * image.width() + p.x()] = rng.generate(); // or = QColor(Qt::red).rgb()
                }
            }
        }
    }
    scene->clear();
    scene->setSceneRect(ui->graphicsView->rect());
    scene->addPixmap(QPixmap::fromImage(image))->setPos(0, 0);
    ui->time_label->setText("Time: " + QString::number(t.elapsed()) + " ms");
}

void MainWindow::on_thous_gen_builtin_btn_clicked()
{
    QElapsedTimer t;
    t.start();
    QImage image(ui->graphicsView->size(), QImage::Format_RGB32);
    image.fill(Qt::white);
    QPainter painter(&image);
    QPen pen(Qt::black);
    painter.setPen(pen);
    scene->setSceneRect(ui->graphicsView->rect());
    scene->clear();
    for (int i = 0; i < 1000; ++i) {
        auto triangle = random_triangle(QRect({0, 0}, ui->graphicsView->size()));
        QColor color = QColor::fromRgb(rng.generate());
        QBrush brush(color);
        painter.setBrush(brush);
        painter.drawPolygon(triangle);
    }
    painter.end();
    scene->addPixmap(QPixmap::fromImage(image))->setPos(0, 0);
    ui->builtin_time_label->setText("Built-in on image time: " + QString::number(t.elapsed()) + " ms");
}

void MainWindow::on_gen_builtin_wid_btn_clicked()
{
    QElapsedTimer t;
    t.start();
    auto triangle = random_triangle(QRect({0, 0}, ui->graphicsView->size()));
    QColor color = QColor::fromRgb(rng.generate());
    QBrush brush(color);
    QPen pen(Qt::black);
    scene->clear();
    scene->setSceneRect(ui->graphicsView->rect());
    scene->addPolygon(triangle, pen, brush)->setPos(0, 0);
    ui->builtin_wid_time->setText("Built-in on widget time: " + QString::number(t.elapsed()) + " ms");
}

void MainWindow::on_thous_gen_builtin_wid_btn_clicked()
{
    QElapsedTimer t;
    t.start();
    QPen pen(Qt::black);
    scene->setSceneRect(ui->graphicsView->rect());
    scene->clear();
    for (int i = 0; i < 1000; ++i) {
        auto triangle = random_triangle(QRect({0, 0}, ui->graphicsView->size()));
        QColor color = QColor::fromRgb(rng.generate());
        QBrush brush(color);
        scene->addPolygon(triangle, pen, brush)->setPos(0, 0);
    }
    ui->builtin_wid_time->setText("Built-in on widget time: " + QString::number(t.elapsed()) + " ms");
}
