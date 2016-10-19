#ifndef CEDITTASKS_H
#define CEDITTASKS_H

#include <QDialog>
#include <stdint.h>
#include "ctime.h"

namespace Ui {
    class CEditTaskS;
}

class CEditTaskS : public QDialog {
    Q_OBJECT
public:
    CEditTaskS(QWidget *parent = 0);
    ~CEditTaskS();

	QString GetDesc();
	uint32_t GetCount();
	CTime GetTime();
	uint32_t GetDist();
	QString GetHR();
	uint32_t GetPace();
	CTime GetPauseTime();
	uint32_t GetPauseDist();
	QString GetPauseHR();
	uint32_t GetPausePace();
	void Set(const QStringList& HRList, const QString& szDesc, uint32_t dwCount, const QTime& Time, uint32_t dwDist, const QString& szHR, uint32_t dwPace, const QTime& PauseTime, uint32_t dwPauseDist, const QString& szPauseHR, uint32_t dwPausePace);

protected:
    void changeEvent(QEvent *e);

	void CalcTotal();
	bool	m_bChanging;

private:
    Ui::CEditTaskS *m_ui;

private slots:
	void on_m_lpCount_valueChanged(int );
	void on_m_lpTime_timeChanged(QTime date);
	void on_m_lpDist_valueChanged(int );
	void on_m_lpSpeed_valueChanged(int );
	void on_m_lpPauseTime_timeChanged(QTime date);
	void on_m_lpPauseDist_valueChanged(int );
	void on_m_lpPauseSpeed_valueChanged(int );
	void on_buttonBox_accepted();
	void on_buttonBox_rejected();
};

#endif // CEDITTASKS_H
