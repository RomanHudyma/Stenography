#ifndef STENOGRAPHY_H
#define STENOGRAPHY_H
#define ESC_SMB 127

#include <QMainWindow>
#include <QImageReader>
#include <QDebug>
#include <QByteArray>
#include <QTextCodec>

namespace Ui {
class Stenography;
}

class Stenography : public QMainWindow
{
    Q_OBJECT

public:
    explicit Stenography(QWidget *parent = 0);
    ~Stenography();
    QString imageFile;
    QString secretMessage;
    QImageReader *imageReader;
    QString imgExt;
    int secretNumberX;
    int secretNumberY;

    int randomBetween(int low, int high, int seed)
    {
        qsrand(seed); // Setting the base number for counting a random host in qrand
        return (qrand() % ((high + 1) - low) + low);
    }

    int getImageExt() {
        if (this->imgExt.indexOf("jpg", 0, Qt::CaseInsensitive) >= 0) {
            return 1;
        } else if (this->imgExt.indexOf("png", 0, Qt::CaseInsensitive) >= 0) {
            return 2;
        } else if (this->imgExt.indexOf("bmp", 0, Qt::CaseInsensitive) >= 0) {
            return 3;
        }
        return 0;
    }

    QImage encode(QImage container, QString text) {

        int imageWidth = container.width();
        int imageHeight = container.height();

        int randX = this->randomBetween(0, imageWidth, this->secretNumberX);
        int randY = this->randomBetween(0, imageHeight, this->secretNumberY);

        for (int letterPos = 0; letterPos < text.size(); letterPos++) {

            randX = this->randomBetween(0, imageWidth, randX);
            randY = this->randomBetween(0, imageHeight, randY);


            QColor tmpColor = container.pixelColor(randX, randY);

            QChar tmpChar = text.at(letterPos);

            int converted = (int)tmpChar.unicode();

            tmpColor.setBlue(converted);
            container.setPixelColor(randX, randY, tmpColor);

            qInfo () << "Pixel setted at [" + QString::number(randX) +"][" + QString::number(randY) + "]";
        }

        randX = this->randomBetween(0, imageWidth, randX);
        randY = this->randomBetween(0, imageHeight, randY);

        QColor tmpColor = container.pixelColor(randX, randY);
        tmpColor.setBlue(ESC_SMB);
        container.setPixelColor(randX, randY, tmpColor);

        return container;
    }

    QString decode(QImage encodedImage) {

        QString secretString;

        QColor colors;

        int imageWidth = encodedImage.width();
        int imageHeight = encodedImage.height();

        int randX = this->randomBetween(0, imageWidth, this->secretNumberX);
        int randY = this->randomBetween(0, imageHeight, this->secretNumberY);

        do {
            randX = this->randomBetween(0, imageWidth, randX);
            randY = this->randomBetween(0, imageHeight, randY);

            colors = encodedImage.pixelColor(randX, randY);

            if (colors.blue() != ESC_SMB) {
                QChar ch(colors.blue());
                secretString.push_back(ch);
            }
            qInfo () << secretString;
            qInfo () << "Got letter from [" + QString::number(randX) +"][" + QString::number(randY) + "]";

        } while (colors.blue() != ESC_SMB);

        return secretString;
    }

    void setSecretNumber(int number) {
        if (number <= 5) {
            this->secretNumberY = 5;
            this->secretNumberX = 10;
        }
        this->secretNumberX = number + 5;
        this->secretNumberY = number - 5;
    }




private slots:
    void on_browseButton_clicked();

    void on_browseButton_2_clicked();

    void on_browseButton_4_clicked();

    void on_encodeButton_clicked();

    void on_decodeButton_clicked();

    void on_secretNumberDecode_editingFinished();

    void on_secretNumberEncode_editingFinished();

private:
    Ui::Stenography *ui;

};

#endif // STENOGRAPHY_H
