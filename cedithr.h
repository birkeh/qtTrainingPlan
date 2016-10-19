#ifndef CEDITHR_H
#define CEDITHR_H

#include <QDialog>
#include <stdint.h>


namespace Ui {
    class CEditHR;
}

class CEditHR : public QDialog {
    Q_OBJECT
public:
    CEditHR(QWidget *parent = 0);
    ~CEditHR();

	QString		GetName();
	QString		GetDesc();
	uint32_t	GetMin();
	uint32_t	GetMax();
	void		Set(const QString& szName, const QString& szDesc, uint32_t dwMin, uint32_t dwMax);
protected:
    void changeEvent(QEvent *e);

private:
    Ui::CEditHR *ui;

private slots:
	void on_buttonBox_rejected();
	void on_buttonBox_accepted();
};

#endif // CEDITHR_H
