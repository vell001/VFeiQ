#include "SettingDialog.h"
#include "ui_SettingDialog.h"

SettingDialog::SettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingDialog),
    mUserService(UserService::getService()),
    myself(UserService::getService()->getMyself()),
    mIconService(IconService::getService()),
    mBroadcastService(UserInfoService::getService())
{
    ui->setupUi(this);
    updateUserInfoView();
}

SettingDialog::~SettingDialog()
{
    delete ui;
}

SettingDialog *SettingDialog::getDialog(){
    static SettingDialog dialog;
    return &dialog;
}

void SettingDialog::setCurrentIndex(SettingIndex index){
    ui->settingListWidget->setCurrentRow((int)index);
    ui->settingStackedWidget->setCurrentIndex((int)index);
}

void SettingDialog::on_settingListWidget_currentRowChanged(int currentRow)
{
    ui->settingStackedWidget->setCurrentIndex(currentRow);
}

void SettingDialog::on_settingStackedWidget_currentChanged(int index)
{
    switch (index) {
    case 0:
        updateUserInfoView();
        break;
    default:
        break;
    }
}

void SettingDialog::updateUserInfoView(){
    ui->userNameEdit->setText(myself->getName());
    ui->userSignatureEdit->setText(myself->getInfo());

    ui->userNormalIconComboBox->clear();
    const QHash<QUuid, Icon *> *normalIcons = mIconService->getNormalUserIcons();
    foreach (QUuid uuid, normalIcons->keys()) {
        Icon *icon = normalIcons->value(uuid);
        ui->userNormalIconComboBox->addItem(icon->getIcon(), QString(), uuid.toString());
    }

    for(int i=0; i<ui->userNormalIconComboBox->count(); i++) {
        if(ui->userNormalIconComboBox->itemData(i).toString() == myself->getIconUuid().toString()) {
            ui->userNormalIconComboBox->setCurrentIndex(i);
            return;
        }
    }

}

void SettingDialog::on_applyUserInfoButton_clicked()
{
    myself->setName(ui->userNameEdit->text());
    myself->setInfo(ui->userSignatureEdit->text());

    myself->setIconUuid(QUuid(ui->userNormalIconComboBox->itemData(ui->userNormalIconComboBox->currentIndex()).toString()));
    mUserService->saveMyself();
}
