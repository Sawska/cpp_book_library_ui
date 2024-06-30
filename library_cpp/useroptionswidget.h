#ifndef USEROPTIONSWIDGET_H
#define USEROPTIONSWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>

class UserOptionsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit UserOptionsWidget(QWidget *parent = nullptr);

private slots:
    void onUpdateUserClicked();
    void onDeleteUserClicked();
    void onLogoutClicked();

private:
    QPushButton *updateUserButton;
    QPushButton *deleteUserButton;
    QPushButton *logoutButton;
};

#endif // USEROPTIONSWIDGET_H
