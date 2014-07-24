#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include <QDialog>
#include "service/UserService.h"
#include "service/IconService.h"
#include "service/UserInfoService.h"

namespace Ui {
class SettingDialog;
}

class SettingDialog : public QDialog
{
    Q_OBJECT

public:
    enum SettingIndex{
        UserInfo,
        SystemSetting
    };
    static SettingDialog *getDialog();
    void setCurrentIndex(SettingIndex index);
    ~SettingDialog();
private slots:
    void on_settingListWidget_currentRowChanged(int currentRow);

    void on_settingStackedWidget_currentChanged(int index);

    void on_applyUserInfoButton_clicked();

private:
    explicit SettingDialog(QWidget *parent = 0);
    Ui::SettingDialog *ui;

    User *myself;
    void updateUserInfoView();
    IconService *mIconService;
    UserService *mUserService;
    UserInfoService *mBroadcastService;
};

#endif // SETTINGDIALOG_H
