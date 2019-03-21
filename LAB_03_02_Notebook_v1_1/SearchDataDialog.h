#ifndef SEARCHDATADIALOG_H
#define SEARCHDATADIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QCheckBox>

#include "common/searchstruct.h"

class SearchDataDialog : public QDialog
{
    Q_OBJECT
private:
    QLineEdit *edits[7];
    QCheckBox *email_check, *phone_check;
    bool ok_pressed;

    void setWidgets();
public:
    explicit SearchDataDialog(QWidget *parent = 0);

    searchStruct getSearchData();
    searchStruct getSearchData(searchStruct &startData);

private slots:
    void slot_buttonOk();
protected:
    void showEvent(QShowEvent *);
};

#endif // SEARCHDATADIALOG_H
