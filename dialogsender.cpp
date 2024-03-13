#include "dialogsender.h"
#include "ui_dialogsender.h"

DialogSender::DialogSender(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogSender)
{
    ui->setupUi(this);
}

DialogSender::~DialogSender()
{
    delete ui;
}

void DialogSender::on_pb_send_clicked()
{
    emit sig_sendText(ui->te_input->toPlainText());
}

