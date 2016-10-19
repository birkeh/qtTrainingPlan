#ifndef CEDITDAY_H
#define CEDITDAY_H

#include <QDialog>

namespace Ui {
    class CEditDay;
}

class CEditDay : public QDialog {
    Q_OBJECT
public:
    CEditDay(QWidget *parent = 0);
    ~CEditDay();

	int GetDay();
	QString GetDesc();
	void Set(int iDay, const QString& szDesc);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::CEditDay *m_ui;

private slots:
	void on_buttonBox_rejected();
	void on_buttonBox_accepted();
};

#endif // CEDITDAY_H
