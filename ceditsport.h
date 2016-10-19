#ifndef CEDITSPORT_H
#define CEDITSPORT_H

#include <QDialog>

namespace Ui {
    class CEditSport;
}

class CEditSport : public QDialog {
    Q_OBJECT
public:
    CEditSport(QWidget *parent = 0);
    ~CEditSport();

	QString GetName();
	int GetMaxHR();
	bool GetPace();
	void Set(const QString& szName, int iMaxHR, bool bPace);
protected:
    void changeEvent(QEvent *e);

private:
    Ui::CEditSport *ui;

private slots:
	void on_buttonBox_rejected();
	void on_buttonBox_accepted();
};

#endif // CEDITSPORT_H
