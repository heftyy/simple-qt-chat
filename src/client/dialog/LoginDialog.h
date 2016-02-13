#pragma once

#include <QtWidgets>

namespace SimpleChat {

class LoginDialog : public QDialog {
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = 0);

    void setupDialog();

    void setEnableLogin(bool enabled);

signals:
    void loginSignal(const QString& address, quint16 port, const QString& name);

private slots:
    void login();
    void enableLoginButton();

private:
    QLabel *hostLabel;
    QLabel *portLabel;
    QLabel *nameLabel;
    QComboBox *hostCombo;
    QLineEdit *portLineEdit;
    QLineEdit *nameLineEdit;

    QPushButton *loginButton;
    QPushButton *quitButton;
    QDialogButtonBox *buttonBox;

    void createWidgets();
    void bindEvents();
};

} // SimpleChat namespace

