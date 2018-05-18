#include "stenography.h"
#include "ui_stenography.h"
#include <QFileDialog>
#include <QDir>
#include <QFile>
#include <QPixmap>
#include <QDialog>
#include <QDebug>
#include <QImage>
#include <QImageReader>
#include <QImageWriter>
#include <QColor>
#include <QChar>
#include <QTextStream>


Stenography::Stenography(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Stenography)
{
    ui->setupUi(this);
    ui->secretNumberDecode->setValidator( new QIntValidator(0, 1000000, this));
    ui->secretNumberEncode->setValidator( new QIntValidator(0, 1000000, this));
}

Stenography::~Stenography()
{
    delete ui;
}



void Stenography::on_browseButton_clicked()
{

    this->imageFile = QFileDialog::getOpenFileName(this, tr("Open File..."),
                                                QDir::homePath(),
                                                tr("Images (*.png *.jpg *.bmp)"));
    ui->imageToEncodePath->setText(this->imageFile);

    this->imageReader = new QImageReader(this->imageFile);

    if (!this->imageReader->canRead()) {
        statusBar()->showMessage("Image can not be opened.", 2000);
    } else {
        statusBar()->showMessage("Image succesfully opened.", 2000);
    }

    QFileInfo imageInfo(this->imageFile);

    this->imgExt = imageInfo.suffix();

}

void Stenography::on_browseButton_2_clicked()
{
    this->secretMessage = QFileDialog::getOpenFileName(this, tr("Open File..."),
                                                QDir::homePath(),
                                                tr("Text Files (*.txt)"));
    ui->fileToEncode->setText(this->secretMessage);

}

void Stenography::on_browseButton_4_clicked()
{
    this->imageFile = QFileDialog::getOpenFileName(this, tr("Open File..."),
                                                QDir::homePath(),
                                                tr("Images (*.png *.jpg *.bmp)"));
    ui->imageToDecodePath->setText(this->imageFile);

    this->imageReader = new QImageReader(this->imageFile);

    if (!this->imageReader->canRead()) {
        statusBar()->showMessage("Image can not be opened.", 2000);
    } else {
        statusBar()->showMessage("Image succesfully opened.", 2000);
    }
}

void Stenography::on_encodeButton_clicked()
{
    QImage imageContainer = this->imageReader->read();

    QFile secretFile(this->secretMessage);

    if(!secretFile.open(QFile::ReadOnly | QIODevice::Text)) {
        statusBar()->showMessage("Secret file can not be opened.", 2000);
    }

    QTextStream in(&secretFile);
    QString secretText = in.readAll();


    QImage newImage = this->encode(imageContainer, secretText);

// ////////
    QString activeFilter;

    switch (this->getImageExt()) {
    case 1:
        activeFilter = tr("JPEG (*.jpg)");
        break;
    case 2:
        activeFilter = tr("PNG (*.png)");
        break;
    case 3:
        activeFilter = tr("BMP (*.bmp)");
        break;
    default:
        activeFilter = tr("PNG (*.png)");
        break;
    }

    QString saveImage = QFileDialog::getSaveFileName(this, tr("Save Image..."),
                                                     QDir::homePath(),
                                                     tr("JPEG (*.jpg *.jpeg);;PNG (*.png);;BMP (*.bmp)"),
                                                     &activeFilter);

    if(!newImage.save(saveImage)) {
        statusBar()->showMessage("Error creting image.", 2000);
    };
}


void Stenography::on_decodeButton_clicked()
{
    QImage encodedImage = this->imageReader->read();

    QString message = this->decode(encodedImage);

    QString activeFilter = tr("TXT (*.txt)");

    QString saveMessage = QFileDialog::getSaveFileName(this, tr("Save File..."),
                                                     QDir::homePath(),
                                                     tr("TXT (*.txt)"),
                                                     &activeFilter);

    QFile resultFile(saveMessage);

    if(!resultFile.open(QFile::ReadWrite| QIODevice::Text)) {
        statusBar()->showMessage("Secret file can not be opened.", 2000);
    }

    QTextStream out(&resultFile);

    out << message;

    resultFile.close();
}


void Stenography::on_secretNumberDecode_editingFinished()
{
    QString tmpString = ui->secretNumberDecode->text();

    int tmpInt = tmpString.toInt();

    this->setSecretNumber(tmpInt);
}

void Stenography::on_secretNumberEncode_editingFinished()
{
    QString tmpString = ui->secretNumberEncode->text();

    int tmpInt = tmpString.toInt();

    this->setSecretNumber(tmpInt);
}
