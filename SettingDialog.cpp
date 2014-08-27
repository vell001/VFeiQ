#include "SettingDialog.h"
#include "ui_SettingDialog.h"

SettingDialog::SettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingDialog),
    mUserService(UserService::getService()),
    myself(UserService::getService()->getMyself()),
    mIconService(IconService::getService())
{
    ui->setupUi(this);
    updateUserInfoView();
    connect(mUserService, SIGNAL(myselfInfoChanged(User*)), this, SLOT(updateUserInfoView()));
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

    ui->userCustomIconButton->setIcon(myself->getIcon());
    ui->userNormalIconComboBox->clear();
    const QHash<QUuid, Icon *> *normalIcons = mIconService->getNormalUserIcons();
    foreach (QUuid uuid, normalIcons->keys()) {
        Icon *icon = normalIcons->value(uuid);
        ui->userNormalIconComboBox->addItem(icon->getIcon(), QString(), uuid.toString());
    }
}

void SettingDialog::on_applyUserInfoButton_clicked()
{
    myself->setName(ui->userNameEdit->text());
    myself->setInfo(ui->userSignatureEdit->text());

    myself->setIcon(ui->userCustomIconButton->icon());
    mUserService->saveMyself();
}

void SettingDialog::on_userCustomIconButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                     QCoreApplication::applicationDirPath(),
                                                     tr("Images (*.png *.jpg)"));
    if(QFile(fileName).exists()) {
        ui->userCustomIconButton->setIcon(QIcon(fileName));
    }
}

void SettingDialog::on_userNormalIconComboBox_activated(int index)
{
    ui->userCustomIconButton->setIcon(ui->userNormalIconComboBox->itemIcon(index));
}
