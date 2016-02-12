#include "LoginDialog.h"

namespace SimpleChat {

LoginDialog::LoginDialog(QWidget* parent) {

}

void LoginDialog::setupDialog() {
    createWidgets();
    bindEvents();

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(hostLabel, 0, 0);
    mainLayout->addWidget(hostCombo, 0, 1);
    mainLayout->addWidget(portLabel, 1, 0);
    mainLayout->addWidget(portLineEdit, 1, 1);
    mainLayout->addWidget(nameLabel, 2, 0);
    mainLayout->addWidget(nameLineEdit, 2, 1);
    mainLayout->addWidget(buttonBox, 3, 0, 1, 2);
    setLayout(mainLayout);

    setWindowTitle(tr("Chat login"));
    hostCombo->setFocus();
}

void LoginDialog::setEnableLogin(bool enabled) {
    loginButton->setEnabled(enabled);
}

void LoginDialog::enableLoginButton() {
    auto enabled = !hostCombo->currentText().isEmpty() &&
            !portLineEdit->text().isEmpty() &&
            !nameLineEdit->text().isEmpty();

    setEnableLogin(enabled);
}

void LoginDialog::login() {
    loginButton->setEnabled(false);

    emit loginSignal(hostCombo->currentText(),
                     static_cast<quint16>(portLineEdit->text().toInt()),
                     nameLineEdit->text());
}

void LoginDialog::bindEvents() {
    connect(hostCombo, SIGNAL(editTextChanged(QString)),
            this, SLOT(enableLoginButton()));
    connect(portLineEdit, SIGNAL(textChanged(QString)),
            this, SLOT(enableLoginButton()));
    connect(nameLineEdit, SIGNAL(textChanged(QString)),
            this, SLOT(enableLoginButton()));

    connect(loginButton,
            SIGNAL(clicked()),
            this,
            SLOT(login()));

    connect(quitButton, SIGNAL(clicked()),
            this, SLOT(close()));
}

void LoginDialog::createWidgets() {
    hostLabel = new QLabel(tr("&Server name:"));
    portLabel = new QLabel(tr("S&erver port:"));
    nameLabel = new QLabel(tr("N&ickname:"));

    hostCombo = new QComboBox;
    hostCombo->setEditable(true);
    // find out name of this machine
    QString name = QHostInfo::localHostName();
    if (!name.isEmpty()) {
        hostCombo->addItem(name);
        QString domain = QHostInfo::localDomainName();
        if (!domain.isEmpty())
            hostCombo->addItem(name + QChar('.') + domain);
    }
    if (name != QString("localhost"))
        hostCombo->addItem(QString("localhost"));
    // find out IP addresses of this machine
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // add non-localhost addresses
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (!ipAddressesList.at(i).isLoopback())
            hostCombo->addItem(ipAddressesList.at(i).toString());
    }
    // add localhost addresses
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i).isLoopback())
            hostCombo->addItem(ipAddressesList.at(i).toString());
    }

    portLineEdit = new QLineEdit;
    portLineEdit->setValidator(new QIntValidator(1, 65535, this));

    QRegularExpression rx("\\w+");
    nameLineEdit = new QLineEdit;
    nameLineEdit->setValidator(new QRegularExpressionValidator(rx));

    hostLabel->setBuddy(hostCombo);
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
