#ifndef DIALOGSENDER_H
#define DIALOGSENDER_H

#include <QDialog>

namespace Ui {
class DialogSender;
}

class DialogSender : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSender(QWidget *parent = nullptr);
    ~DialogSender();

signals:
    void sig_sendText(const QString& text);

private slots:
    void on_pb_send_clicked();

private:
    Ui::DialogSender *ui;
};

#endif // DIALOGSENDER_H
