#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDate>
#include <QWebView>
#include <QTreeWidget>
#include <QFileDialog>
#include <QDir>
#include <QCloseEvent>
#include <QPrinter>
#include <stdint.h>
#include "cclientdata.h"
#include "ctraining.h"


namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
protected:
	bool				m_bDateChanging;

	QTreeWidgetItem*	m_lpPlanRoot;
	QString				m_szFileName;
	bool				m_bHasChanged;

	QStatusBar*			m_lpStatusBar;

	bool				GetTaskData(QTreeWidgetItem* lpItem, QString& szDesc, uint32_t& dwTaskCount, uint32_t& dwTaskTime, uint32_t& dwTaskDist, QString& szTaskHR, uint32_t& dwTaskPace, uint32_t& dwTaskPauseTime, uint32_t& dwTaskPauseDist, QString& szTaskPauseHR, uint32_t& dwTaskPausePace);
	void				SetTaskData(QTreeWidgetItem* lpItem, const QString& szDesc, uint32_t dwCount, uint32_t dwTime, uint32_t dwDist, const QString& szHR, uint32_t dwPace, uint32_t dwPauseTime, uint32_t dwPauseDist, const QString& szPauseHR, uint32_t dwPausePace, const QString& szSport);

	int					GetIndent(QTreeWidgetItem* lpItem);

	void				CalculateDay(QTreeWidgetItem* lpDay);
	void				CalculateSession(QTreeWidgetItem* lpSession);

	QTreeWidgetItem*	SetChildText(QTreeWidgetItem* lpItem, int iCol, const QString& szText, Qt::AlignmentFlag iAlign);
	QTreeWidgetItem*	AddChild(QTreeWidget* lpRoot, const QString& szText, CClientData* lpClientData, Qt::AlignmentFlag iAlign, const QColor& Color, const QColor& BGColor);
	QTreeWidgetItem*	AddChild(QTreeWidgetItem* lpRoot, const QString& szText, CClientData* lpClientData, Qt::AlignmentFlag iAlign, const QColor& Color, const QColor& BGColor, int iPos = -1);

	QTreeWidgetItem*	AddTask(int iTraining, int iDay, int iSession, int iTask, const QString& szDesc, const QString& szSport, uint32_t dwCount, uint32_t dwTime, uint32_t dwDist, const QString& szHR, uint32_t dwPace, uint32_t dwPauseTime, uint32_t dwPauseDist, const QString& szPauseHR, uint32_t dwPausePace);
	QTreeWidgetItem*	AddTask(QTreeWidgetItem* lpRoot, int iTask, const QString& szDesc, const QString& szSport, uint32_t dwCount, uint32_t dwTime, uint32_t dwDist, const QString& szHR, uint32_t dwPace, uint32_t dwPauseTime, uint32_t dwPauseDist, const QString& szPauseHR, uint32_t dwPausePace);

	QTreeWidgetItem*	FindSession(int iTraining, int iDay, int iSession);
	QTreeWidgetItem*	FindSession(const QString& szTraining, const QString& szDay, const QString& szSession);
	QTreeWidgetItem*	AddSession(int iTraining, int iDay, int iSession, const QString& szDesc, const QString& szSport);
	QTreeWidgetItem*	AddSession(QTreeWidgetItem* lpRoot, int iSession, const QString& szDesc, const QString& szSport);

	QTreeWidgetItem*	FindDay(int iTraining, int iDay);
	QTreeWidgetItem*	FindDay(const QString& szTraining, const QString& szDay);
	QTreeWidgetItem*	AddDay(int iTraining, int iDay, const QString& szDesc);
	QTreeWidgetItem*	AddDay(QTreeWidgetItem* lpRoot, int iDay, const QString& szDesc, int iPos = -1);

	QTreeWidgetItem*	FindTraining(int iTraining);
	QTreeWidgetItem*	FindTraining(const QString& szTraining);
	bool				AddTraining(int iTraining, const QString& szName);

	QTreeWidgetItem*	AddHR(QTreeWidgetItem* lpRoot, const QString& szName, const QString& szDesc, uint32_t dwMaxHR, uint32_t dwMin, uint32_t dwMax);

	QTreeWidgetItem*	AddSport(const QString& szSport, uint32_t dwMaxHR, bool bPace);

	bool				LoadPlan(const QString& szFile, bool bPerson = true, bool bSport = true, bool bPlan = true);
	bool				SavePlan(const QString& szFile);
	bool				LoadPerson(CTraining* lpTraining);
	bool				LoadSport(CTraining* lpTraining);
	bool				LoadPlan(CTraining* lpTraining);
	void				Clear();
	void				InitSportView();
	void				InitPlanView();
	void				InitCalendarView();
	void				closeEvent(QCloseEvent *event);
	bool				MayClose();

	CClientData*		GetClientData(QTreeWidgetItem* lpItem);

	QTreeWidgetItem*	FindSport(const QString& szSport);
	QTreeWidgetItem*	FindHR(const QString& szSport, const QString& szHR);

	QString 			FormatHR(const QString& szSport, const QString& szHR);
	QString 			FormatPace(const QString& szSport, uint32_t dwPace);

	bool				EditPlan(QTreeWidgetItem* lpItem, bool bRightClick = false);
	bool				EditSport(QTreeWidgetItem* lpItem, bool bRightClick = false);
	bool				EditHR(QTreeWidgetItem* lpItem, bool bRightClick);

	uint32_t			GetMaxDays();

	void				Changed(bool bChanged);
	void				HasChanged();
	void				DrawCalendar();

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

private slots:
	void on_m_lpSportView_customContextMenuRequested(QPoint pos);
	void on_action_PDF_triggered();
	void on_action_Print_triggered();
	void on_m_lpCalendar_linkClicked(QUrl url);
	void on_m_lpTargetDate_dateChanged(QDate date);
	void on_m_lpTrainingStart_dateChanged(QDate date);
	void on_m_lpPlanView_itemDoubleClicked(QTreeWidgetItem* item, int column);
	void on_m_lpPlanView_customContextMenuRequested(QPoint pos);
	void on_m_lpWeight_valueChanged(int );
	void on_m_lpHeight_valueChanged(int );
	void on_m_lpDayOfBirth_dateChanged(QDate date);
	void on_m_lpLastName_textChanged(QString );
	void on_m_lpFirstName_textChanged(QString );
	void on_actionE_xit_triggered();
	void on_actionSave_as_triggered();
	void on_action_Save_triggered();
	void on_action_Open_triggered();
	void on_actionNew_from_triggered();
	void on_action_New_triggered();

	void EditPlan(bool);

	void AddSport(bool);
	void EditSport(bool);
	void DeleteSport(bool);

	void AddHR(bool);
	void EditHR(bool);
	void DeleteHR(bool);

	void AddTraining(bool);

	void AddDay(bool);
	void AddSession(bool);
	void AddTask(bool);

	void DeleteTraining(bool);
	void DeleteDay(bool);
	void DeleteSession(bool);
	void DeleteTask(bool);

	void MoveUp(bool);
	void MoveDown(bool);
};

#endif // MAINWINDOW_H
