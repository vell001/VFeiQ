#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include <QDialog>
#include "service/UserService.h"
#include "service/IconService.h"
#include "service/UserInfoService.h"
#include <QFileDialog>

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
    void updateUserInfoView();

    void on_settingListWidget_currentRowChanged(int currentRow);

    void on_settingStackedWidget_currentChanged(int index);

    void on_applyUserInfoButton_clicked();

    void on_userCustomIconButton_clicked();

    void on_userNormalIconComboBox_currentIndexChanged(int index);

private:
    explicit SettingDialog(QWidget *parent = 0);
    Ui::SettingDialog *ui;

    User *myself;
    IconService *mIconService;
    UserService *mUserService;
    UserInfoService *mBroadcastService;

    int userIconNormalOrCustom; // 0: normal, 1: custom
};

#endif // SETTINGDIALOG_H
