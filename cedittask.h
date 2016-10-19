#ifndef CEDITTASK_H
#define CEDITTASK_H

#include <QDialog>
#include "ctime.h"

namespace Ui {
    class CEditTask;
}

class CEditTask : public QDialog {
    Q_OBJECT
public:
    CEditTask(QWidget *parent = 0);
    ~CEditTask();

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
	bool m_bChanging;

private:
    Ui::CEditTask *m_ui;

private slots:
	void on_m_lpTime_timeChanged(QTime date);
	void on_m_lpDist_valueChanged(int );
	void on_m_lpPace_timeChanged(QTime date);
	void on_m_lpPauseTime_timeChanged(QTime date);
	void on_m_lpPauseDist_valueChanged(int );
	void on_m_lpPausePace_timeChanged(QTime date);
	void on_m_lpCount_valueChanged(int );
	void on_buttonBox_accepted();
	void on_buttonBox_rejected();
};

#endif // CEDITTASK_H
