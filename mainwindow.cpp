#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QKeyEvent>

#include "Joc/laberint.h"

using namespace std;

std::vector<sf::VideoMode> videos;

void actualitzar(Ui::MainWindow* ui, bool canviar = true) {
    ui->cmbresol->clear();
    ui->comboconfig->clear();

    QDir dir = QDir("./");
    auto x = dir.entryList(QStringList() << "*.txt" << "*.TXT", QDir::Files);

    bool darrer = false;
    for (auto &_x : x) {
        if (_x.contains("Darrera partida")) {
            darrer = true;
            continue;
        }
        _x.replace(".txt", "");
        _x.replace(".TXT", "");
        ui->comboconfig->addItem(_x);
    }
    if (darrer) {
        ui->comboconfig->insertItem(0, "Darrera partida");
        if (canviar) ui->comboconfig->setCurrentIndex(0);
    }

    // combobox de resolucions
    videos = sf::VideoMode::getFullscreenModes();
    for (auto x : videos) {
        QString str = QString("%1x%2, %3 bits/píxel").arg(x.width).arg(x.height).arg(x.bitsPerPixel);
        ui->cmbresol->addItem(str);
    }
    if (canviar) ui->cmbresol->setCurrentIndex(0);
}

void guardar(Ui::MainWindow* ui, bool darrer = false) {
    int sel = ui->cmbresol->currentIndex();
    auto vel = QVariant(ui->numvel->value()).toString();
    auto monedes = QVariant(ui->nummon->value()).toString();
    auto vides = QVariant(ui->numvides->value()).toString();
    auto enes = QVariant(ui->numenes->value()).toString();
    auto x = QVariant(ui->numx->value()).toString();
    auto nom = ui->txtnom->text();

    if (darrer) nom = "Darrera partida";

    QFile f(nom + ".txt");
    if (f.exists()) f.remove();
    f.close();

    QFile f2(nom + ".txt");
    if (f2.open(QIODevice::ReadWrite)) {
        QTextStream stream(&f2);
        stream << videos[sel].width << endl;
        stream << videos[sel].height << endl;
        stream << videos[sel].bitsPerPixel << endl;
        stream << vel << endl;
        stream << monedes << endl;
        stream << vides << endl;
        stream << enes << endl;
        stream << x << endl;
        stream << (ui->checkpantalla->isChecked() ? "true" : "false");

        f2.close();
        actualitzar(ui, false);

        ui->cmbresol->setCurrentIndex(sel);
        for (int i = 0; i < ui->comboconfig->count(); i++) {
            if (nom == ui->comboconfig->itemText(i)) {
                ui->comboconfig->setCurrentIndex(i);
                break;
            }
        }
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    actualitzar(ui);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_btncomencar_clicked()
{
    // començam sa partida amb sa configuració actual
    guardar(ui, true);

    int sel = ui->cmbresol->currentIndex();
    int vel = ui->numvel->value();
    int monedes = ui->nummon->value();
    int vides = ui->numvides->value();
    int enes = ui->numenes->value();
    int x = ui->numx->value();

    auto ruta = QDir::currentPath();
    string struta("");
    for (int i = 0; i < ruta.size(); i++) {
        struta += ruta.at(i).toLatin1();
    }

    joc(struta + "/",
        x,
        videos[sel].width,
        videos[sel].height,
        videos[sel].bitsPerPixel,
        monedes,
        vides,
        4000 - 0.8 * (float)vel,
        enes,
        ui->checkpantalla->isChecked()
    );
}

void MainWindow::on_btndesar_clicked()
{
    guardar(ui);
}

void MainWindow::on_btnesborrar_clicked()
{
    auto nom = ui->txtnom->text() + ".txt";
    if (QFile::exists(nom)) {
        QFile::remove(nom);
        actualitzar(ui, false);
    }
}

void MainWindow::on_comboconfig_currentIndexChanged(const QString &arg1)
{
    QFile f(arg1 + ".txt");
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) return;

    QTextStream in(&f);
    QString linia, resolucio("");
    int i = 0;

    ui->txtnom->setText(arg1);
    while (!in.atEnd()) {
        linia = in.readLine();
        if (i == 0) resolucio = linia + "x";
        else if (i == 1) resolucio += linia + ", ";
        else if (i == 2) {
            resolucio += linia +" bits/píxel";

            for (int i = 0; i < videos.size(); i++) {
                if (ui->cmbresol->itemText(i) == resolucio) {
                    ui->cmbresol->setCurrentIndex(i);
                    break;
                }
            }
        }
        else if (i == 3) ui->numvel->setValue(linia.toInt());
        else if (i == 4) ui->nummon->setValue(linia.toInt());
        else if (i == 5) ui->numvides->setValue(linia.toInt());
        else if (i == 6) ui->numenes->setValue(linia.toInt());
        else if (i == 7) ui->numx->setValue(linia.toInt());
        else if (i == 8) ui->checkpantalla->setChecked(linia == "true" ? true : false);

        i++;
    }
}
