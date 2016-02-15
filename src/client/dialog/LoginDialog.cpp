#include <QtNetwork>

#include "LoginDialog.h"

namespace SimpleChat {

LoginDialog::LoginDialog(QWidget* parent) :
        QDialog(parent) {

}

void LoginDialog::setupDialog() {
    createWidgets();
    bindEvents();

    auto mainLayout = new QGridLayout;
    mainLayout->addWidget(hostLabel, 0, 0);
    mainLayout->addWidget(hostLineEdit, 0, 1);
    mainLayout->addWidget(portLabel, 1, 0);
    mainLayout->addWidget(portLineEdit, 1, 1);
    mainLayout->addWidget(nameLabel, 2, 0);
    mainLayout->addWidget(nameLineEdit, 2, 1);
    mainLayout->addWidget(buttonBox, 3, 0, 1, 2);
    setLayout(mainLayout);

    setWindowTitle(tr("Chat login"));
    nameLineEdit->setFocus();
}

void LoginDialog::setEnableLogin(bool enabled) const {
    loginButton->setEnabled(enabled);
}

void LoginDialog::enableLoginButton() const {
    auto enabled = !hostLineEdit->text().isEmpty() &&
            !portLineEdit->text().isEmpty() &&
            !nameLineEdit->text().isEmpty();

    setEnableLogin(enabled);
}

void LoginDialog::login() {
    loginButton->setEnabled(false);

    emit loginSignal(hostLineEdit->text(),
                     static_cast<quint16>(portLineEdit->text().toInt()),
                     nameLineEdit->text());
}

void LoginDialog::bindEvents() const {
    connect(hostLineEdit, SIGNAL(textChanged(QString)),
            this, SLOT(enableLoginButton()));
    connect(portLineEdit, SIGNAL(textChanged(QString)),
            this, SLOT(enableLoginButton()));
    connect(nameLineEdit, SIGNAL(textChanged(QString)),
            this, SLOT(enableLoginButton()));

    connect(loginButton, SIGNAL(clicked()),
            this, SLOT(login()));
    connect(quitButton, SIGNAL(clicked()),
            this, SLOT(close()));
}

void LoginDialog::createWidgets() {
    hostLabel = new QLabel(tr("&Server name:"));
    portLabel = new QLabel(tr("S&erver port:"));
    nameLabel = new QLabel(tr("N&ickname:"));

    QString ipRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";

    QRegularExpression ipRegex("^" + ipRange 
                               + "\\." + ipRange
                               + "\\." + ipRange
                               + "\\." + ipRange + "$");

    hostLineEdit = new QLineEdit;
    hostLineEdit->setText("127.0.0.1");
    hostLineEdit->setValidator(new QRegularExpressionValidator(
        ipRegex, this));

    portLineEdit = new QLineEdit;
    portLineEdit->setText("4441");
    portLineEdit->setValidator(new QIntValidator(1, 65535, this));

    nameLineEdit = new QLineEdit;
    nameLineEdit->setValidator(new QRegularExpressionValidator(
        QRegularExpression("\\w+"), this));

    hostLabel->setBuddy(hostLineEdit);
    portLabel->setBuddy(portLineEdit);
    nameLabel->setBuddy(nameLineEdit);

    loginButton = new QPushButton(tr("Login"));
    loginButton->setDefault(true);
    loginButton->setEnabled(false);

    quitButton = new QPushButton(tr("Quit"));

    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(loginButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(quitButton, QDialogButtonBox::RejectRole);
}

} // SimpleChat namespace
