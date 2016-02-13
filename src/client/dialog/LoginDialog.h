#pragma once

#include <QtWidgets>

namespace SimpleChat {

class LoginDialog : public QDialog {
    Q_OBJECT
public:
    explicit LoginDialog(QWidget *parent = nullptr);

    void setupDialog();
    void setEnableLogin(bool enabled) const;

signals:
    void loginSignal(const QString& address, quint16 port, const QString& name);

private slots:
    void login();
    void enableLoginButton() const;

private:
    QLabel *hostLabel;
    QLabel *portLabel;
    QLabel *nameLabel;
    QLineEdit *hostLineEdit;
    QLineEdit *portLineEdit;
    QLineEdit *nameLineEdit;

    QPushButton *loginButton;
    QPushButton *quitButton;
    QDialogButtonBox *buttonBox;

    void createWidgets();
    void bindEvents() const;
};

} // SimpleChat namespace

