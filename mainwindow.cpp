#include <QMessageBox>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "common.h"
#include "cedithr.h"
#include "ceditsport.h"
#include "cedittraining.h"
#include "ceditday.h"
#include "ceditsession.h"
#include "cedittask.h"
#include "cedittasks.h"
#include "cexportpdf.h"


#define COL_DESC			0
#define COL_SPORT			1
#define COL_COUNT			2
#define COL_TIME			3
#define COL_DIST			4
#define COL_HR				5
#define COL_PACESPEED		6
#define COL_PAUSETIME		7
#define COL_PAUSEDIST		8
#define COL_PAUSEHR			9
#define COL_PAUSEPACESPEED	10

#define COL_HRSPORT			0
#define COL_HRMAXHR			1
#define COL_HRNAME			0
#define COL_HRDESC			2
#define COL_HRMIN			3
#define COL_HRMAX			4

#define TXT_WINDOW_TITLE	tr("qtTrainingPlan")
#define TXT_NEW				tr("new")


enum INDENT_PLAN
{
	INDENT_TRAINING	= 0,
	INDENT_DAY		= 1,
	INDENT_SESSION	= 2,
	INDENT_TASK		= 3,
};

enum INDENT_SPORT
{
	INDENT_SPORT	= 0,
	INDENT_HR		= 1,
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
	Changed(false);
	m_szFileName			= "";
	m_lpPlanRoot			= 0;
	m_bDateChanging			= false;
	ui->setupUi(this);
	setWindowTitle(TXT_WINDOW_TITLE+" - "+TXT_NEW);

	m_lpStatusBar = new QStatusBar;
	setStatusBar(m_lpStatusBar);

	showMaximized();

	InitPlanView();
	InitSportView();
	InitCalendarView();
}

MainWindow::~MainWindow()
{
	if(m_lpStatusBar)
		delete m_lpStatusBar;

	delete ui;
}

void MainWindow::on_action_New_triggered()
{
	if(!MayClose())
		return;
	Clear();
	setWindowTitle(TXT_WINDOW_TITLE+" - "+TXT_NEW);
}

void MainWindow::on_actionNew_from_triggered()
{
	if(!MayClose())
		return;
	QString	szFileName = QFileDialog::getOpenFileName(this, tr("Open Training"), QDir::homePath(), tr("Training Files (*.qtp);;XML Files (*.xml)"));
	if(szFileName.length())
	{
		Clear();
		m_szFileName	= szFileName;
		LoadPlan(szFileName, true, true, false);
	setWindowTitle(TXT_WINDOW_TITLE+" - "+TXT_NEW+" *");
	}
}

void MainWindow::on_action_Open_triggered()
{
	if(!MayClose())
		return;
	QString	szFileName = QFileDialog::getOpenFileName(this, tr("Open Training"), QDir::homePath(), tr("Training Files (*.qtp);;XML Files (*.xml)"));
	if(szFileName.length())
	{
		Clear();
		m_szFileName	= szFileName;
		LoadPlan(szFileName);
		setWindowTitle(QString(TXT_WINDOW_TITLE+" - ")+m_szFileName);
	}
}

void MainWindow::on_action_Save_triggered()
{
	if(!m_szFileName.length())
		on_actionSave_as_triggered();
	else
		SavePlan(m_szFileName);
}

void MainWindow::on_actionSave_as_triggered()
{
	QString szFileName = QFileDialog::getSaveFileName(this, tr("Save Training"), QDir::homePath(), tr("Training Files (*.qtp);;XML Files (*.xml)"));
	if(szFileName.length())
		SavePlan(szFileName);
}

void MainWindow::on_actionE_xit_triggered()
{
	if(MayClose())
		close();
}

void MainWindow::on_m_lpFirstName_textChanged(QString )
{
	HasChanged();
}

void MainWindow::on_m_lpLastName_textChanged(QString )
{
	HasChanged();
}

void MainWindow::on_m_lpDayOfBirth_dateChanged(QDate)
{
	HasChanged();
}

void MainWindow::on_m_lpHeight_valueChanged(int )
{
	HasChanged();
}

void MainWindow::on_m_lpWeight_valueChanged(int )
{
	HasChanged();
}

void MainWindow::on_m_lpTrainingStart_dateChanged(QDate date)
{
	if(m_bDateChanging)
	{
		m_bDateChanging = false;
		return;
	}
	m_bDateChanging		= true;
	Changed(true);
	uint32_t	dwDays	= GetMaxDays();
	ui->m_lpTargetDate->setDate(date.addDays(dwDays-1));
	DrawCalendar();
}

void MainWindow::on_m_lpTargetDate_dateChanged(QDate date)
{
	if(m_bDateChanging)
	{
		m_bDateChanging = false;
		return;
	}
	m_bDateChanging		= true;
	Changed(true);
	uint32_t	dwDays	= GetMaxDays();
	ui->m_lpTrainingStart->setDate(date.addDays(-dwDays+1));
	DrawCalendar();
}

void MainWindow::on_m_lpCalendar_linkClicked(QUrl url)
{
	QString	szDay		= "";
	QString	szSession	= "";

	szDay	= url.path().mid(url.path().indexOf("DAY")+3);
	if(url.path().indexOf("SESSION"))
	{
		szDay		= szDay.left(szDay.indexOf("SESSION"));
		szSession	= url.path().mid(url.path().indexOf("SESSION")+7);
	}

	QTreeWidgetItem*	lpItem	= 0;
	if(szSession.length())
		lpItem = FindSession(0, szDay.toLong(), szSession.toLong());
	else
		lpItem = FindDay(0, szDay.toLong());

	if(lpItem)
	{
		ui->m_lpPlanView->setCurrentItem(lpItem);
		ui->m_lpMainTab->setCurrentIndex(0);
	}
}

void MainWindow::on_m_lpSportView_customContextMenuRequested(QPoint pos)
{
	QTreeWidgetItem*	lpSelected	= ui->m_lpSportView->currentItem();
	QAction*			lpAction;
	QMenu*				lpMenu		= new QMenu(tr("test"), this);
	int					iIndent;

	if(!lpSelected)
	{
		lpAction	= lpMenu->addAction(tr("Add sport..."), this, SLOT(AddSport(bool)));
		lpMenu->exec(ui->m_lpSportView->mapToGlobal(pos));
		return;
	}

	iIndent	= GetIndent(lpSelected);

	switch(iIndent)
	{
	case INDENT_SPORT:
		lpAction	= lpMenu->addAction(tr("Add sport..."), this, SLOT(AddSport(bool)));
		lpAction	= lpMenu->addAction(tr("Edit sport..."), this, SLOT(EditSport(bool)));
		lpAction	= lpMenu->addAction(tr("Delete sport..."), this, SLOT(DeleteSport(bool)));
		lpAction	= lpMenu->addSeparator();
		lpAction	= lpMenu->addAction(tr("Add hr..."), this, SLOT(AddHR(bool)));
		break;
	case INDENT_HR:
		lpAction	= lpMenu->addAction(tr("Add hr..."), this, SLOT(AddHR(bool)));
		lpAction	= lpMenu->addAction(tr("Edit hr..."), this, SLOT(EditHR(bool)));
		lpAction	= lpMenu->addAction(tr("Delete hr..."), this, SLOT(DeleteHR(bool)));
		break;
	}
	lpMenu->exec(ui->m_lpSportView->mapToGlobal(pos));
}

void MainWindow::on_m_lpPlanView_customContextMenuRequested(QPoint pos)
{
	QTreeWidgetItem*	lpSelected	= ui->m_lpPlanView->currentItem();
	QAction*			lpAction;
	QMenu*				lpMenu		= new QMenu(tr("test"), this);
	int					iIndent;

	if(!lpSelected && !m_lpPlanRoot)
	{
		lpAction	= lpMenu->addAction(tr("Add training..."), this, SLOT(AddTraining(bool)));
		lpMenu->exec(ui->m_lpPlanView->mapToGlobal(pos));
		return;
	}

	iIndent		= GetIndent(lpSelected);

	switch(iIndent)
	{
		case INDENT_TRAINING:
			lpAction	= lpMenu->addAction(tr("Add day..."), this, SLOT(AddDay(bool)));
			lpAction	= lpMenu->addSeparator();
			lpAction	= lpMenu->addAction(tr("Edit training..."), this, SLOT(EditPlan(bool)));
			lpAction	= lpMenu->addAction(tr("Delete training..."), this, SLOT(DeleteTraining(bool)));
			break;
		case INDENT_DAY:
			lpAction	= lpMenu->addAction(tr("Add day..."), this, SLOT(AddDay(bool)));
			lpAction	= lpMenu->addAction(tr("Edit day..."), this, SLOT(EditPlan(bool)));
			lpAction	= lpMenu->addAction(tr("Delete day..."), this, SLOT(DeleteDay(bool)));
			lpAction	= lpMenu->addSeparator();
			lpAction	= lpMenu->addAction(tr("Add session..."), this, SLOT(AddSession(bool)));
			break;
		case INDENT_SESSION:
			lpAction	= lpMenu->addAction(tr("Add day..."), this, SLOT(AddDay(bool)));
			lpAction	= lpMenu->addSeparator();
			lpAction	= lpMenu->addAction(tr("Add session..."), this, SLOT(AddSession(bool)));
			lpAction	= lpMenu->addAction(tr("Edit session..."), this, SLOT(EditPlan(bool)));
			lpAction	= lpMenu->addAction(tr("Delete session..."), this, SLOT(DeleteSession(bool)));
			lpAction	= lpMenu->addSeparator();
			lpAction	= lpMenu->addAction(tr("Add task..."), this, SLOT(AddTask(bool)));
			lpAction	= lpMenu->addSeparator();
			lpAction	= lpMenu->addAction(tr("Move up"), this, SLOT(MoveUp(bool)));
			lpAction	= lpMenu->addAction(tr("Move down"), this, SLOT(MoveDown(bool)));
			break;
		case INDENT_TASK:
			lpAction	= lpMenu->addAction(tr("Add day..."), this, SLOT(AddDay(bool)));
			lpAction	= lpMenu->addSeparator();
			lpAction	= lpMenu->addAction(tr("Add session..."), this, SLOT(AddSession(bool)));
			lpAction	= lpMenu->addSeparator();
			lpAction	= lpMenu->addAction(tr("Add task..."), this, SLOT(AddTask(bool)));
			lpAction	= lpMenu->addAction(tr("Edit task..."), this, SLOT(EditPlan(bool)));
			lpAction	= lpMenu->addAction(tr("Delete task..."), this, SLOT(DeleteTask(bool)));
			lpAction	= lpMenu->addSeparator();
			lpAction	= lpMenu->addAction(tr("Move up"), this, SLOT(MoveUp(bool)));
			lpAction	= lpMenu->addAction(tr("Move down"), this, SLOT(MoveDown(bool)));
			break;
	}
	lpMenu->exec(ui->m_lpPlanView->mapToGlobal(pos));
}

void MainWindow::on_m_lpPlanView_itemDoubleClicked(QTreeWidgetItem* item, int )
{
	EditPlan(item);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	if(MayClose())
		event->accept();
	else
		event->ignore();
}

bool MainWindow::MayClose()
{
	if(m_bHasChanged)
	{
		int ret = QMessageBox::question(this, TXT_WINDOW_TITLE, tr("Training has been changed. Dou you want to save?"), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
		if(ret == QMessageBox::Yes)
			on_action_Save_triggered();
		else if(ret == QMessageBox::Cancel)
		return(false);
	}
	return(true);
}

void MainWindow::Clear()
{
	ui->m_lpSportView->clear();
	ui->m_lpPlanView->clear();
	m_lpPlanRoot		= 0;

	ui->m_lpFirstName->setText("");
	ui->m_lpLastName->setText("");
	ui->m_lpDayOfBirth->setDate(QDate::currentDate());
	ui->m_lpHeight->setValue(0);
	ui->m_lpWeight->setValue(0);

	ui->m_lpCalendar->setHtml("");

	Changed(false);
}

void MainWindow::InitPlanView()
{
	QStringList		szLabels;
	szLabels << tr("Description") << tr("Sport") << tr("Count") << tr("Time") << tr("Dist") << tr("HR") << tr("Pace/Speed") << tr("P.Time") << tr("P.Dist") << tr("P.HR") << tr("P.Pace/Speed");
	ui->m_lpPlanView->setHeaderLabels(szLabels);

	QHeaderView*	lpHeader	= ui->m_lpPlanView->header();
	lpHeader->setStretchLastSection(false);
	lpHeader->setSectionResizeMode( 0, QHeaderView::Stretch);
	lpHeader->setSectionResizeMode( 1, QHeaderView::ResizeToContents);
	lpHeader->setSectionResizeMode( 2, QHeaderView::ResizeToContents);
	lpHeader->setSectionResizeMode( 3, QHeaderView::ResizeToContents);
	lpHeader->setSectionResizeMode( 4, QHeaderView::ResizeToContents);
	lpHeader->setSectionResizeMode( 5, QHeaderView::ResizeToContents);
	lpHeader->setSectionResizeMode( 6, QHeaderView::ResizeToContents);
	lpHeader->setSectionResizeMode( 7, QHeaderView::ResizeToContents);
	lpHeader->setSectionResizeMode( 8, QHeaderView::ResizeToContents);
	lpHeader->setSectionResizeMode( 9, QHeaderView::ResizeToContents);
	lpHeader->setSectionResizeMode(10, QHeaderView::ResizeToContents);
}

void MainWindow::InitSportView()
{
	QStringList		szLabels;
	szLabels << tr("Sport") << tr("MaxHR") << tr("HR") << tr("Min-Max (%)") << tr("Min-Max");
	ui->m_lpSportView->setHeaderLabels(szLabels);

	QHeaderView*	lpHeader	= ui->m_lpSportView->header();
	lpHeader->setStretchLastSection(false);
	lpHeader->setSectionResizeMode( 0, QHeaderView::ResizeToContents);
	lpHeader->setSectionResizeMode( 1, QHeaderView::ResizeToContents);
	lpHeader->setSectionResizeMode( 2, QHeaderView::Stretch);
	lpHeader->setSectionResizeMode( 4, QHeaderView::ResizeToContents);
	//lpHeader->setSectionResizeMode( 5, QHeaderView::ResizeToContents);
}

void MainWindow::InitCalendarView()
{
	ui->m_lpCalendar->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
}

bool MainWindow::LoadPlan(const QString& szFile, bool bPerson, bool bSport, bool bPlan)
{
	CTraining*		lpTraining = new CTraining(szFile.toLatin1().data());

	if(bPerson)
		LoadPerson(lpTraining);
	if(bSport)
		LoadSport(lpTraining);
	if(bPlan)
		LoadPlan(lpTraining);

	Changed(false);

	m_bDateChanging		= true;
	uint32_t	dwDays	= GetMaxDays();
	ui->m_lpTargetDate->setDate(ui->m_lpTrainingStart->date().addDays(dwDays-1));

	DrawCalendar();
	return(true);
}

bool MainWindow::LoadPerson(CTraining* lpTraining)
{
	QString			szFirstName;
	QString			szLastName;
	QDate			DayOfBirth;
	uint32_t		dwHeight;
	uint32_t		dwWeight;

	if(lpTraining->GetPerson(szFirstName, szLastName, DayOfBirth, dwHeight, dwWeight))
	{
		ui->m_lpFirstName->setText(szFirstName);
		ui->m_lpLastName->setText(szLastName);
		ui->m_lpDayOfBirth->setDate(DayOfBirth);
		ui->m_lpHeight->setValue(dwHeight);
		ui->m_lpWeight->setValue(dwWeight);
	}
	return(true);
}

bool MainWindow::LoadSport(CTraining* lpTraining)
{
	QString			szSport;
	uint32_t		dwMaxHR;
	bool			bPace;
	QString			szName;
	QString			szDesc;
	uint32_t		dwMin;
	uint32_t		dwMax;

	if(lpTraining->GetFirstSport(szSport, dwMaxHR, bPace))
	{
		do
		{
			QTreeWidgetItem*	lpSport	= AddSport(szSport, dwMaxHR, bPace);
			if(lpTraining->GetFirstHR(szName, szDesc, dwMin, dwMax))
			{
				do
				{
					/*QTreeWidgetItem*	lpHR = */AddHR(lpSport, szName, szDesc, dwMaxHR, dwMin, dwMax);
				}	while(lpTraining->GetNextHR(szName, szDesc, dwMin, dwMax));
			}
		}	while(lpTraining->GetNextSport(szSport, dwMaxHR, bPace));
	}
	return(true);
}

bool MainWindow::LoadPlan(CTraining* lpTraining)
{
	QString			szName;
	QString			szDesc;
	QString			szSport;
	QString			szHR;
	int				iTraining;
	int				iDay;
	int				iSession;
	int				iTask;
	uint32_t		dwCount;
	uint32_t		dwTime;
	uint32_t		dwDist;
	uint32_t		dwPace;
	uint32_t		dwPauseTime;
	uint32_t		dwPauseDist;
	QString			szPauseHR;
	uint32_t		dwPausePace;
	QDate			TrainingStart;


	if(lpTraining->GetFirstTraining(szName, szDesc, TrainingStart))
	{
		iTraining = 1;
		ui->m_lpTrainingStart->setDate(TrainingStart);

		AddTraining(iTraining,  szName);
		if(lpTraining->GetFirstDay(iDay, szDesc))
		{
			do
			{
				AddDay(iTraining, iDay, QString("(%1) %2").arg(iDay).arg(szDesc));
				if(lpTraining->GetFirstSession(iSession, szDesc, szSport))
				{
					do
					{
						AddSession(iTraining, iDay, iSession,  szDesc, szSport);
						if(lpTraining->GetFirstTask(iTask, szDesc, dwCount, dwTime, dwDist, szHR, dwPace, dwPauseTime, dwPauseDist, szPauseHR, dwPausePace))
						{
							do
							{
								AddTask(iTraining, iDay, iSession, iTask, szDesc, szSport, dwCount, dwTime, dwDist, szHR, dwPace, dwPauseTime, dwPauseDist, szPauseHR, dwPausePace);
							}	while(lpTraining->GetNextTask(iTask, szDesc, dwCount, dwTime, dwDist, szHR, dwPace, dwPauseTime, dwPauseDist, szPauseHR, dwPausePace));
						}
					}	while(lpTraining->GetNextSession(iSession, szDesc, szSport));
				}
			}	while(lpTraining->GetNextDay(iDay, szDesc));
		}
	}
	return(true);
}

CClientData* MainWindow::GetClientData(QTreeWidgetItem* lpItem)
{
	QVariant			data	= lpItem->data(0, Qt::UserRole);
	CClientData*		lpData	= (CClientData*)data.toInt();
	return(lpData);
}

QTreeWidgetItem* MainWindow::AddChild(QTreeWidgetItem* lpRoot, const QString& szText, CClientData* lpClientData, Qt::AlignmentFlag iAlign, const QColor& Color, const QColor& BGColor, int iPos)
{
	if(!lpRoot)
		return(0);

	QVariant			data((qlonglong)lpClientData);
	QTreeWidgetItem*	lpChild;
	if(iPos != -1)
	{
		lpChild	= new QTreeWidgetItem();
		lpRoot->insertChild(iPos, lpChild);
	}
	else
		lpChild	= new QTreeWidgetItem(lpRoot);
	lpChild->setText(0, szText);
	lpChild->setData(0, Qt::UserRole, data);
	lpChild->setTextAlignment(0, iAlign);
	for(int i = 0;i < ui->m_lpPlanView->columnCount();i++)
	{
		lpChild->setForeground(i, QBrush(Color));
		lpChild->setBackground(i, QBrush(BGColor));
	}
	return(lpChild);
}

QTreeWidgetItem* MainWindow::AddChild(QTreeWidget* lpRoot, const QString& szText, CClientData* lpClientData, Qt::AlignmentFlag iAlign, const QColor& Color, const QColor& BGColor)
{
	if(!lpRoot)
		return(0);

	QVariant			data((qlonglong)lpClientData);
	QTreeWidgetItem*	lpChild = new QTreeWidgetItem(lpRoot);
	lpChild->setText(0, szText);
	lpChild->setData(0, Qt::UserRole, data);
	lpChild->setTextAlignment(0, iAlign);
	for(int i = 0;i < ui->m_lpPlanView->columnCount();i++)
	{
		lpChild->setForeground(i, QBrush(Color));
		lpChild->setBackground(i, QBrush(BGColor));
	}
	return(lpChild);
}

QTreeWidgetItem* MainWindow::SetChildText(QTreeWidgetItem* lpItem, int iCol, const QString& szText, Qt::AlignmentFlag iAlign)
{
	lpItem->setText(iCol, szText);
	lpItem->setTextAlignment(iCol, iAlign);
	return(lpItem);
}

QTreeWidgetItem* MainWindow::FindSport(const QString& szSport)
{
	for(int i = 0;i < ui->m_lpSportView->topLevelItemCount();i++)
	{
		QTreeWidgetItem*	lpChild	= ui->m_lpSportView->topLevelItem(i);
		CClientData*		lpData	= GetClientData(lpChild);
		if(lpData->GetSportName() == szSport)
			return(lpChild);
	}
	return(0);
}

QTreeWidgetItem* MainWindow::FindHR(const QString& szSport, const QString& szHR)
{
	QTreeWidgetItem*	lpSport	= FindSport(szSport);
	if(!lpSport)
		return(0);

	for(int i = 0;i < lpSport->childCount();i++)
	{
		QTreeWidgetItem*	lpChild	= lpSport->child(i);
		CClientData*		lpData	= GetClientData(lpChild);
		if(lpData->GetHRName() == szHR)
			return(lpChild);
	}
	return(0);
}

QTreeWidgetItem* MainWindow::AddSport(const QString& szSport, uint32_t dwMaxHR, bool bPace)
{
	CClientData*	lpClientData;

	lpClientData	= new CClientData(szSport, dwMaxHR, bPace);
	if(!dwMaxHR)
		dwMaxHR = 180;
	QTreeWidgetItem*	lpSport	= AddChild(ui->m_lpSportView, szSport, lpClientData, Qt::AlignLeft, QColor(0, 0, 0), QColor(255, 255, 255));
	ui->m_lpSportView->addTopLevelItem(lpSport);
	SetChildText(lpSport, COL_HRMAXHR, QString("%1").arg(dwMaxHR), Qt::AlignRight);
	SetChildText(lpSport, COL_HRMAXHR+1, bPace ? tr("Pace") : tr("Speed"), Qt::AlignLeft);
	return(lpSport);
}

QTreeWidgetItem* MainWindow::AddHR(QTreeWidgetItem* lpRoot, const QString& szName, const QString& szDesc, uint32_t dwMaxHR, uint32_t dwMin, uint32_t dwMax)
{
	CClientData*	lpClientData;

	lpClientData	= new CClientData(szName, szDesc, dwMin, dwMax);
	QTreeWidgetItem*	lpHR = AddChild(lpRoot, szName, lpClientData, Qt::AlignLeft, QColor(0, 0, 0), QColor(255, 255, 255));
	SetChildText(lpHR, COL_HRDESC, szDesc, Qt::AlignLeft);
	SetChildText(lpHR, COL_HRMIN, QString("%1\% - %2\%").arg(dwMin).arg(dwMax), Qt::AlignRight);
	SetChildText(lpHR, COL_HRMAX, QString("%1 - %2").arg(dwMaxHR*dwMin/100).arg(dwMaxHR*dwMax/100), Qt::AlignRight);
	return(lpHR);
}

bool MainWindow::AddTraining(int iTraining, const QString& szName)
{
	if(m_lpPlanRoot)
		return(false);

	iTraining		= iTraining;
	m_lpPlanRoot	= AddChild(ui->m_lpPlanView, szName, 0, Qt::AlignLeft, QColor(0, 0, 0), QColor(255, 255, 255));
	m_lpPlanRoot->setExpanded(true);
	return(true);
}

QTreeWidgetItem* MainWindow::FindTraining(int iTraining)
{
	iTraining = iTraining;
	return(m_lpPlanRoot);
}

QTreeWidgetItem* MainWindow::FindTraining(const QString& /*szTraining*/)
{
	return(m_lpPlanRoot);
}

QTreeWidgetItem* MainWindow::AddDay(int iTraining, int iDay, const QString& szDesc)
{
	QTreeWidgetItem*	lpTraining	= FindTraining(iTraining);
	if(!lpTraining)
		return(false);

	CClientData*		lpClientData	= new CClientData(iDay, szDesc);
	return(AddChild(lpTraining, szDesc, lpClientData, Qt::AlignLeft, QColor(0, 0, 0), QColor(223, 223, 223)));
}

QTreeWidgetItem* MainWindow::AddDay(QTreeWidgetItem* lpRoot, int iDay, const QString& szDesc, int iPos)
{
	CClientData*		lpClientData	= new CClientData(iDay, szDesc);
	return(AddChild(lpRoot, szDesc, lpClientData, Qt::AlignLeft, QColor(0, 0, 0), QColor(223, 223, 223), iPos));
}

QTreeWidgetItem* MainWindow::FindDay(int iTraining, int iDay)
{
	QTreeWidgetItem*	lpTraining	= FindTraining(iTraining);
	if(!lpTraining)
		return(0);

	for(int i = 0;i < lpTraining->childCount();i++)
	{
		QTreeWidgetItem*	lpChild	= lpTraining->child(i);
		CClientData*		lpData	= GetClientData(lpChild);
		if(lpData->GetDay() == iDay)
			return(lpChild);
	}
	return(0);
}

QTreeWidgetItem* MainWindow::FindDay(const QString& szTraining, const QString& szDay)
{
	QTreeWidgetItem*	lpTraining	= FindTraining(szTraining);
	if(!lpTraining)
		return(0);

	for(int i = 0;i < lpTraining->childCount();i++)
	{
		QTreeWidgetItem*	lpChild	= lpTraining->child(i);
		CClientData*		lpData	= GetClientData(lpChild);
		if(lpData->GetDayDesc() == szDay || lpChild->text(0) == szDay)
			return(lpChild);
	}
	return(0);
}

QTreeWidgetItem* MainWindow::AddSession(int iTraining, int iDay, int iSession, const QString& szDesc, const QString& szSport)
{
	QTreeWidgetItem*	lpDay	= FindDay(iTraining, iDay);
	if(!lpDay)
		return(0);

	CClientData*		lpClientData	= new CClientData(iSession, szDesc, szSport);
	QTreeWidgetItem*	lpChild	= AddChild(lpDay, szDesc, lpClientData, Qt::AlignLeft, QColor(0, 0, 255), QColor(255, 255, 255));
	SetChildText(lpChild, COL_SPORT, szSport, Qt::AlignLeft);

	return(lpChild);
}

QTreeWidgetItem* MainWindow::AddSession(QTreeWidgetItem* lpRoot, int iSession, const QString& szDesc, const QString& szSport)
{
	CClientData*		lpClientData	= new CClientData(iSession, szDesc, szSport);
	QTreeWidgetItem*	lpChild	= AddChild(lpRoot, szDesc, lpClientData, Qt::AlignLeft, QColor(0, 0, 255), QColor(255, 255, 255));
	SetChildText(lpChild, COL_SPORT, szSport, Qt::AlignLeft);

	return(lpChild);
}

QTreeWidgetItem* MainWindow::FindSession(int iTraining, int iDay, int iSession)
{
	QTreeWidgetItem*	lpDay	= FindDay(iTraining, iDay);
	if(!lpDay)
		return(0);

	for(int i = 0;i < lpDay->childCount();i++)
	{
		QTreeWidgetItem*	lpChild	= lpDay->child(i);
		CClientData*		lpData	= GetClientData(lpChild);
		if(lpData->GetSession() == iSession)
			return(lpChild);
	}
	return(0);
}

QTreeWidgetItem* MainWindow::FindSession(const QString& szTraining, const QString& szDay, const QString& szSession)
{
	QTreeWidgetItem*	lpDay	= FindDay(szTraining, szDay);
	if(!lpDay)
		return(0);

	for(int i = 0;i < lpDay->childCount();i++)
	{
		QTreeWidgetItem*	lpChild	= lpDay->child(i);
		CClientData*		lpData	= GetClientData(lpChild);
		if(lpData->GetSessionDesc() == szSession)
			return(lpChild);
	}
	return(0);
}

QTreeWidgetItem* MainWindow::AddTask(int iTraining, int iDay, int iSession, int iTask, const QString& szDesc, const QString& szSport, uint32_t dwCount, uint32_t dwTime, uint32_t dwDist, const QString& szHR, uint32_t dwPace, uint32_t dwPauseTime, uint32_t dwPauseDist, const QString& szPauseHR, uint32_t dwPausePace)
{
	QTreeWidgetItem*	lpSession	= FindSession(iTraining, iDay, iSession);
	if(!lpSession)
		return(false);

	CTime Time(dwTime);
	CTime PauseTime(dwPauseTime);
	CClientData*		lpClientData	= new CClientData(iTask, szDesc, dwCount, Time, dwDist, szHR, dwPace, PauseTime, dwPauseDist, szPauseHR, dwPausePace);
	QTreeWidgetItem*	lpChild			= AddChild(lpSession, szDesc, lpClientData, Qt::AlignLeft, QColor(0, 0, 0), QColor(255, 255, 255));
	SetChildText(lpChild, COL_COUNT,			QString("%1").arg(dwCount), Qt::AlignRight);
	SetChildText(lpChild, COL_TIME,				CTime(dwTime).Format(), Qt::AlignRight);
	SetChildText(lpChild, COL_DIST,				DWORD2String(dwDist), Qt::AlignRight);
	SetChildText(lpChild, COL_HR,				FormatHR(szSport, szHR), Qt::AlignRight);
	SetChildText(lpChild, COL_PACESPEED,		FormatPace(szSport, dwPace), Qt::AlignRight);
	if(dwCount > 1)
	{
		SetChildText(lpChild, COL_PAUSETIME,		CTime(dwPauseTime).Format(), Qt::AlignRight);
		SetChildText(lpChild, COL_PAUSEDIST,		DWORD2String(dwPauseDist), Qt::AlignRight);
		SetChildText(lpChild, COL_PAUSEHR,			FormatHR(szSport, szPauseHR), Qt::AlignRight);
		SetChildText(lpChild, COL_PAUSEPACESPEED,	FormatPace(szSport, dwPausePace), Qt::AlignRight);
	}

	CalculateSession(lpSession);

	return(lpChild);
}

QTreeWidgetItem* MainWindow::AddTask(QTreeWidgetItem* lpRoot, int iTask, const QString& szDesc, const QString& szSport, uint32_t dwCount, uint32_t dwTime, uint32_t dwDist, const QString& szHR, uint32_t dwPace, uint32_t dwPauseTime, uint32_t dwPauseDist, const QString& szPauseHR, uint32_t dwPausePace)
{
	CTime Time(dwTime);
	CTime PauseTime(dwPauseTime);
	CClientData*		lpClientData	= new CClientData(iTask, szDesc, dwCount, Time, dwDist, szHR, dwPace, PauseTime, dwPauseDist, szPauseHR, dwPausePace);
	QTreeWidgetItem*	lpChild			= AddChild(lpRoot, szDesc, lpClientData, Qt::AlignLeft, QColor(0, 0, 0), QColor(255, 255, 255));
	SetChildText(lpChild, COL_COUNT,			QString("%1").arg(dwCount), Qt::AlignRight);
	SetChildText(lpChild, COL_TIME,				CTime(dwTime).Format(), Qt::AlignRight);
	SetChildText(lpChild, COL_DIST,				DWORD2String(dwDist), Qt::AlignRight);
	SetChildText(lpChild, COL_HR,				FormatHR(szSport, szHR), Qt::AlignRight);
	SetChildText(lpChild, COL_PACESPEED,		FormatPace(szSport, dwPace), Qt::AlignRight);
	if(dwCount > 1)
	{
		SetChildText(lpChild, COL_PAUSETIME,		CTime(dwPauseTime).Format(), Qt::AlignRight);
		SetChildText(lpChild, COL_PAUSEDIST,		DWORD2String(dwPauseDist), Qt::AlignRight);
		SetChildText(lpChild, COL_PAUSEHR,			FormatHR(szSport, szPauseHR), Qt::AlignRight);
		SetChildText(lpChild, COL_PAUSEPACESPEED,	FormatPace(szSport, dwPausePace), Qt::AlignRight);
	}

	CalculateSession(lpRoot);

	return(lpChild);
}

bool MainWindow::GetTaskData(QTreeWidgetItem* lpItem, QString& szDesc, uint32_t& dwTaskCount, uint32_t& dwTaskTime, uint32_t& dwTaskDist, QString& szTaskHR, uint32_t& dwTaskPace, uint32_t& dwTaskPauseTime, uint32_t& dwTaskPauseDist, QString& szTaskPauseHR, uint32_t& dwTaskPausePace)
{
	szDesc		= lpItem->text(0);
	dwTaskCount	= String2DWORD(lpItem->text(COL_COUNT).toLatin1().data());
	dwTaskTime	= CTime(lpItem->text(COL_TIME)).toDWORD();
	dwTaskDist	= String2DWORD(lpItem->text(COL_DIST).toLatin1().data());
	szTaskHR	= lpItem->text(COL_HR);
	dwTaskPace	= CTime(lpItem->text(COL_PACESPEED)).toDWORD();
	if(dwTaskCount > 1)
	{
		dwTaskPauseTime	= CTime(lpItem->text(COL_PAUSETIME)).toDWORD();
		dwTaskPauseDist	= String2DWORD(lpItem->text(COL_PAUSEDIST).toLatin1().data());
		szTaskPauseHR	= lpItem->text(COL_PAUSEHR);
		dwTaskPausePace	=CTime(lpItem->text(COL_PAUSEPACESPEED)).toDWORD();
	}
	else
	{
		dwTaskPauseTime	= 0;
		dwTaskPauseDist	= 0;
		szTaskPauseHR	= "";
		dwTaskPausePace	= 0;
	}
	return(true);
}

void MainWindow::SetTaskData(QTreeWidgetItem* lpItem, const QString& szDesc, uint32_t dwCount, uint32_t dwTime, uint32_t dwDist, const QString& szHR, uint32_t dwPace, uint32_t dwPauseTime, uint32_t dwPauseDist, const QString& szPauseHR, uint32_t dwPausePace, const QString& szSport)
{
	CTime Time(dwTime);
	CTime PauseTime(dwPauseTime);
	lpItem->setText(0, szDesc);
	SetChildText(lpItem, COL_COUNT,				QString("%1").arg(dwCount), Qt::AlignRight);
	SetChildText(lpItem, COL_TIME,				QDate(HOURS(dwTime), MINUTES(dwTime), SECONDS(dwTime)).toString(tr("yyyyMMdd")), Qt::AlignRight);
	SetChildText(lpItem, COL_DIST,				DWORD2String(dwDist), Qt::AlignRight);
	SetChildText(lpItem, COL_HR,				FormatHR(szSport, szHR), Qt::AlignRight);
	SetChildText(lpItem, COL_PACESPEED,			FormatPace(szSport, dwPace), Qt::AlignRight);
	SetChildText(lpItem, COL_PAUSETIME,			QDate(HOURS(dwPauseTime), MINUTES(dwPauseTime), SECONDS(dwPauseTime)).toString(tr("yyyyMMdd")), Qt::AlignRight);
	SetChildText(lpItem, COL_PAUSEDIST,			DWORD2String(dwPauseDist), Qt::AlignRight);
	SetChildText(lpItem, COL_PAUSEHR,			FormatHR(szSport, szPauseHR), Qt::AlignRight);
	SetChildText(lpItem, COL_PAUSEPACESPEED,	FormatPace(szSport, dwPausePace), Qt::AlignRight);

	CalculateSession(lpItem->parent());
}

void MainWindow::CalculateSession(QTreeWidgetItem* lpSession)
{
	if(!lpSession)
		return;

	QString		szDesc;
	uint32_t	dwTaskCount;
	uint32_t	dwTaskTime;
	uint32_t	dwTaskDist;
	QString		szTaskHR;
	uint32_t	dwTaskPace;
	uint32_t	dwTaskPauseTime;
	uint32_t	dwTaskPauseDist;
	QString		szTaskPauseHR;
	uint32_t	dwTaskPausePace;
	uint32_t	dwTime		= 0;
	uint32_t	dwDist		= 0;

	for(int i = 0;i < lpSession->childCount();i++)
	{
		QTreeWidgetItem*	lpChild	= lpSession->child(i);

		GetTaskData(lpChild, szDesc, dwTaskCount, dwTaskTime, dwTaskDist, szTaskHR, dwTaskPace, dwTaskPauseTime, dwTaskPauseDist, szTaskPauseHR, dwTaskPausePace);

		dwTime	+= dwTaskTime	* dwTaskCount	+ dwTaskPauseTime	* (dwTaskCount-1);
		dwDist	+= dwTaskDist	* dwTaskCount	+ dwTaskPauseDist	* (dwTaskCount-1);
	}
	if(dwTime)
		SetChildText(lpSession, COL_TIME,	CTime(dwTime).Format(), Qt::AlignRight);
	if(dwDist)
		SetChildText(lpSession, COL_DIST,	DWORD2String(dwDist), Qt::AlignRight);

	QTreeWidgetItem*	lpDay	= lpSession->parent();
	CalculateDay(lpDay);
}

void MainWindow::CalculateDay(QTreeWidgetItem* lpDay)
{
	uint32_t	dwDayTime;
	uint32_t	dwDayDist;
	uint32_t	dwTime		= 0;
	uint32_t	dwDist		= 0;

	for(int i = 0;i < lpDay->childCount();i++)
	{
		QTreeWidgetItem*	lpChild	= lpDay->child(i);
		QString sz = lpChild->text(COL_TIME);
		dwDayTime	= CTime(lpChild->text(COL_TIME)).toDWORD();
		dwDayDist	= String2DWORD(lpChild->text(COL_DIST).toLatin1().data());

		dwTime	+= dwDayTime;
		dwDist	+= dwDayDist;
	}
	if(dwTime)
		SetChildText(lpDay, COL_TIME,	CTime(dwTime).Format(), Qt::AlignRight);
	if(dwDist)
		SetChildText(lpDay, COL_DIST,	DWORD2String(dwDist), Qt::AlignRight);
}

int MainWindow::GetIndent(QTreeWidgetItem* lpItem)
{
	int					iIndent		= 0;
	QTreeWidgetItem*	lpParent;

	lpParent = lpItem->parent();
	while(lpParent)
	{
		iIndent++;
		lpParent = lpParent->parent();
	}
	return(iIndent);
}

QString MainWindow::FormatHR(const QString& szSport, const QString& szHR)
{
	QString				szRet	= szHR;
	QTreeWidgetItem*	lpSport	= FindSport(szSport);
	QTreeWidgetItem*	lpHR	= FindHR(szSport, szHR);

	if(!lpSport)
		return(szRet);
	if(!lpHR)
		return(szRet);

	CClientData*		lpSportData	= GetClientData(lpSport);
	CClientData*		lpHRData	= GetClientData(lpHR);
	uint32_t			dwMaxHR		= lpSportData->GetSportMaxHR();
	if(!dwMaxHR)
		dwMaxHR = 180;

	if(lpHRData->GetHRMin() != lpHRData->GetHRMax())
		szRet	= QString("%1 (%2 - %3)").arg(szHR).arg(lpHRData->GetHRMin()*dwMaxHR/100).arg(lpHRData->GetHRMax()*dwMaxHR/100);
	else
		szRet	= QString("%1 (%2)").arg(szHR).arg(lpHRData->GetHRMin()*dwMaxHR/100);

	return(szRet);
}

QString MainWindow::FormatPace(const QString& szSport, uint32_t dwPace)
{
	QString				szRet	= QString("");
	QTreeWidgetItem*	lpSport	= FindSport(szSport);

	if(!dwPace)
		return(szRet);
	if(!lpSport)
		return(szRet);

	CClientData*		lpSportData	= GetClientData(lpSport);
	if(lpSportData->GetSportPace())
		szRet = CTime(dwPace).Format()+QString(tr(" min/km"));
	else
		szRet = QString(tr("%1 km/h")).arg(3600/(double)dwPace, 0, 'f');

	return(szRet);
}

void MainWindow::EditPlan(bool)
{
	QTreeWidgetItem*	lpSelected	= ui->m_lpPlanView->currentItem();
	if(!lpSelected)
		return;
	EditPlan(lpSelected, true);
}

void MainWindow::AddSport(bool)
{
	CEditSport*	lpNewSport	= new CEditSport(this);
	if(lpNewSport->exec() == QDialog::Accepted)
	{
		QString		szName	= lpNewSport->GetName();
		uint32_t	dwMaxHR	= lpNewSport->GetMaxHR();
		bool		bPace	= lpNewSport->GetPace();
		AddSport(szName, dwMaxHR, bPace);
	}
}

void MainWindow::EditSport(bool)
{
	QTreeWidgetItem*	lpSelected	= ui->m_lpSportView->currentItem();
	if(!lpSelected)
		return;
	EditSport(lpSelected, true);
}

void MainWindow::DeleteSport(bool)
{
	if(MESSAGE_QUESTION(tr("Are you sure you want to delete this sport?")) == QMessageBox::Yes)
	{
		QTreeWidgetItem*	lpSelected	= ui->m_lpSportView->currentItem();
		delete lpSelected;
	}
}

void MainWindow::AddHR(bool)
{
	QTreeWidgetItem*	lpSelected	= ui->m_lpSportView->currentItem();
	if(!lpSelected)
		return;

	CEditHR*	lpNewHR	= new CEditHR(this);
	if(lpNewHR->exec() == QDialog::Accepted)
	{
		QString			szName			= lpNewHR->GetName();
		QString			szDesc			= lpNewHR->GetDesc();
		uint32_t		dwMin			= lpNewHR->GetMin();
		uint32_t		dwMax			= lpNewHR->GetMax();
		CClientData*	lpClientData	= GetClientData(lpSelected);
		uint32_t		dwMaxHR			= lpClientData->GetSportMaxHR();
		AddHR(lpSelected, szName, szDesc, dwMaxHR, dwMin, dwMax);
	}
}

void MainWindow::EditHR(bool)
{
	QTreeWidgetItem*	lpSelected	= ui->m_lpSportView->currentItem();
	if(!lpSelected)
		return;
	EditHR(lpSelected, true);
}

void MainWindow::DeleteHR(bool)
{
	if(MESSAGE_QUESTION(tr("Are you sure you want to delete this HR?")) == QMessageBox::Yes)
	{
		QTreeWidgetItem*	lpSelected	= ui->m_lpSportView->currentItem();
		delete lpSelected;
	}
}

void MainWindow::AddTraining(bool)
{
	CEditTraining*	lpNewTraining	= new CEditTraining(this);
	if(lpNewTraining->exec() == QDialog::Accepted)
	{
		QString	szName	= lpNewTraining->GetTraining();
		AddTraining(0, szName);
	}
	delete lpNewTraining;
}

void MainWindow::AddDay(bool)
{
	CEditDay*	lpNewDay	= new CEditDay(this);
	if(lpNewDay->exec() == QDialog::Accepted)
	{
		int		iDay	= lpNewDay->GetDay();
		QString	szDesc	= lpNewDay->GetDesc();
		if(FindDay(0, iDay))
		{
			MESSAGE(tr("Day allready exists!"));
			delete lpNewDay;
			return;
		}

		QTreeWidgetItem*	lpTraining	= FindTraining(0);
		if(!lpTraining)
		{
			MESSAGE(tr("No training available!"));
			delete lpNewDay;
			return;
		}

		int					i;
		for(i = lpTraining->childCount()-1;i >= 0;i--)
		{
			QTreeWidgetItem*	lpInsert	= lpTraining->child(i);
			CClientData*		lpData		= GetClientData(lpInsert);
			if(lpData->GetDay() < iDay)
				break;
		}
		QTreeWidgetItem* lpDay = AddDay(lpTraining, iDay, QString("(%1) %2").arg(iDay).arg(szDesc), i+1);
		ui->m_lpPlanView->setCurrentItem(lpDay);
		HasChanged();
	}
	delete lpNewDay;
}

void MainWindow::AddSession(bool)
{
	CEditSession*		lpNewSession	= new CEditSession(this);
	QStringList			DayList;
	QStringList			SportList;
	QTreeWidgetItem*	lpDay			= ui->m_lpPlanView->currentItem();
	CClientData*		lpClientData;
	int					i;

	while(GetIndent(lpDay) > INDENT_DAY)
		lpDay = lpDay->parent();
	lpClientData	= GetClientData(lpDay);

	for(i = 0;i < m_lpPlanRoot->childCount();i++)
		DayList << m_lpPlanRoot->child(i)->text(0);

	for(int i = 0;i < ui->m_lpSportView->topLevelItemCount();i++)
		SportList << ui->m_lpSportView->topLevelItem(i)->text(0);

	lpNewSession->Set(DayList, SportList, lpDay->text(0), "", "");
	if(lpNewSession->exec() == QDialog::Accepted)
	{
		QString	szDay	= lpNewSession->GetDay();
		QString	szDesc	= lpNewSession->GetDesc();
		QString	szSport	= lpNewSession->GetSport();

		lpDay	= FindDay(QString(""), szDay);
		int					iSession	= -1;
		QTreeWidgetItem*	lpSession;
		for(i = 0;i < lpDay->childCount();i++)
		{
			lpClientData = GetClientData(lpDay->child(i));
			if(lpClientData->GetSession() > iSession)
				iSession = lpClientData->GetSession();
		}
		lpSession = AddSession(lpDay, iSession+1, szDesc, szSport);
		ui->m_lpPlanView->setCurrentItem(lpSession);
		HasChanged();
	}
	delete lpNewSession;
}

void MainWindow::AddTask(bool)
{
	QTreeWidgetItem*	lpSession	= ui->m_lpPlanView->currentItem();
	CClientData*		lpClientData;
	CClientData*		lpClientDataSport;
	QStringList			HRList;
	int					i;

	while(GetIndent(lpSession) > INDENT_SESSION)
		lpSession = lpSession->parent();
	lpClientData	= GetClientData(lpSession);

	QTreeWidgetItem*	lpSport		= FindSport(lpClientData->GetSessionSport());
	lpClientDataSport = GetClientData(lpSport);

	for(i = 0;i < lpSport->childCount();i++)
		HRList << QString("%1 (%2)").arg(lpSport->child(i)->text(0)).arg(lpSport->child(i)->text(4));

	QString sz = lpClientData->GetSessionSport();
	if(lpClientDataSport->GetSportPace())
	{
		CEditTask*		lpNewTask	= new CEditTask(this);
		lpNewTask->Set(HRList, "", 1, CTime(0, 0, 0), 0, 0, 0, CTime(0, 0, 0), 0, 0, 0);
		if(lpNewTask->exec() == QDialog::Accepted)
		{
			int					iTask	= -1;
			QTreeWidgetItem*	lpTask;
			for(i = 0;i < lpSession->childCount();i++)
			{
				lpClientData = GetClientData(lpSession->child(i));
				if(lpClientData->GetTask() > iTask)
					iTask = lpClientData->GetTask();
			}
			lpTask = AddTask(lpSession, iTask+1, lpNewTask->GetDesc(), lpSession->text(COL_SPORT), lpNewTask->GetCount(), lpNewTask->GetTime().toDWORD(), lpNewTask->GetDist(), lpNewTask->GetHR(), lpNewTask->GetPace(), lpNewTask->GetPauseTime().toDWORD(), lpNewTask->GetPauseDist(), lpNewTask->GetPauseHR(), lpNewTask->GetPausePace());
			ui->m_lpPlanView->setCurrentItem(lpTask);
		}
		delete lpNewTask;
		HasChanged();
	}
	else
	{
		CEditTaskS*		lpNewTask	= new CEditTaskS(this);
		lpNewTask->Set(HRList, "", 1, CTime(0, 0, 0), 0, 0, 0, CTime(0, 0, 0), 0, 0, 0);
		if(lpNewTask->exec() == QDialog::Accepted)
		{
			int					iTask	= -1;
			QTreeWidgetItem*	lpTask;
			for(i = 0;i < lpSession->childCount();i++)
			{
				lpClientData = GetClientData(lpSession->child(i));
				if(lpClientData->GetTask() > iTask)
					iTask = lpClientData->GetTask();
			}
			lpTask = AddTask(lpSession, iTask+1, lpNewTask->GetDesc(), lpSession->text(COL_SPORT), lpNewTask->GetCount(), lpNewTask->GetTime().toDWORD(), lpNewTask->GetDist(), lpNewTask->GetHR(), lpNewTask->GetPace(), lpNewTask->GetPauseTime().toDWORD(), lpNewTask->GetPauseDist(), lpNewTask->GetPauseHR(), lpNewTask->GetPausePace());
			ui->m_lpPlanView->setCurrentItem(lpTask);
		}
		delete lpNewTask;
		HasChanged();
	}
}

void MainWindow::DeleteTraining(bool)
{
	if(MESSAGE_QUESTION(tr("Are you sure you want to delete this training?")) == QMessageBox::Yes)
	{
		QTreeWidgetItem*	lpSelected	= ui->m_lpPlanView->currentItem();
		delete lpSelected;
		HasChanged();
	}
}

void MainWindow::DeleteDay(bool)
{
	if(MESSAGE_QUESTION(tr("Are you sure you want to delete this day?")) == QMessageBox::Yes)
	{
		QTreeWidgetItem*	lpSelected	= ui->m_lpPlanView->currentItem();
		delete lpSelected;
		HasChanged();
	}
}

void MainWindow::DeleteSession(bool)
{
	if(MESSAGE_QUESTION(tr("Are you sure you want to delete this session?")) == QMessageBox::Yes)
	{
		QTreeWidgetItem*	lpSelected	= ui->m_lpPlanView->currentItem();
		QTreeWidgetItem*	lpDay		= lpSelected->parent();
		delete lpSelected;
		HasChanged();
		CalculateDay(lpDay);
	}
}

void MainWindow::DeleteTask(bool)
{
	if(MESSAGE_QUESTION(tr("Are you sure you want to delete this task?")) == QMessageBox::Yes)
	{
		QTreeWidgetItem*	lpSelected	= ui->m_lpPlanView->currentItem();
		QTreeWidgetItem*	lpSession	= lpSelected->parent();
		delete lpSelected;
		HasChanged();
		CalculateSession(lpSession);
	}
}

void MainWindow::MoveUp(bool)
{
	QTreeWidgetItem*	lpSelected	= ui->m_lpPlanView->currentItem();
	QTreeWidgetItem*	lpParent	= lpSelected->parent();
	int					iIndex		= lpParent->indexOfChild(lpSelected)-1;

	if(iIndex < 0)
		return;

	lpParent->removeChild(lpSelected);
	lpParent->insertChild(iIndex, lpSelected);
	ui->m_lpPlanView->setCurrentItem(lpSelected);
	HasChanged();
}

void MainWindow::MoveDown(bool)
{
	QTreeWidgetItem*	lpSelected	= ui->m_lpPlanView->currentItem();
	QTreeWidgetItem*	lpParent	= lpSelected->parent();
	int					iIndex		= lpParent->indexOfChild(lpSelected)+1;

	if(iIndex >= lpParent->childCount())
		return;

	lpParent->removeChild(lpSelected);
	lpParent->insertChild(iIndex, lpSelected);
	ui->m_lpPlanView->setCurrentItem(lpSelected);
	HasChanged();
}

bool MainWindow::EditSport(QTreeWidgetItem* lpItem, bool bRightClick)
{
	int	iIndent	= GetIndent(lpItem);

	switch(iIndent)
	{
	case INDENT_SPORT:
		if(bRightClick)
		{
			CEditSport*	lpEditSport	= new CEditSport(this);
			lpEditSport->Set(lpItem->text(0), lpItem->text(1).toInt(), lpItem->text(2) == "Pace" ? true : false);
			if(lpEditSport->exec() == QDialog::Accepted)
			{
				CClientData*	lpClientData	= GetClientData(lpItem);
				QString			szName			= lpEditSport->GetName();
				uint32_t		dwMaxHR			= lpEditSport->GetMaxHR();
				bool			bPace			= lpEditSport->GetPace();
				lpItem->setText(0, szName);
				lpItem->setText(1, QString("%1").arg(dwMaxHR));
				lpItem->setText(2, bPace ? "Pace" : "Speed");
				lpClientData->SetSportName(szName);
				lpClientData->SetSportMaxHR(dwMaxHR);
				lpClientData->SetSportPace(bPace);
			}
			delete lpEditSport;
		}
		break;
	}
	return(true);
}

bool MainWindow::EditHR(QTreeWidgetItem* lpItem, bool bRightClick)
{
	int	iIndent	= GetIndent(lpItem);

	switch(iIndent)
	{
	case INDENT_HR:
		if(bRightClick)
		{
			CClientData*	lpClientData	= GetClientData(lpItem);
			CEditHR*		lpEditHR	= new CEditHR(this);
			lpEditHR->Set(lpItem->text(0), lpItem->text(2), lpClientData->GetHRMin(), lpClientData->GetHRMax());

			if(lpEditHR->exec() == QDialog::Accepted)
			{
				QString			szName			= lpEditHR->GetName();
				QString			szDesc			= lpEditHR->GetDesc();
				uint32_t		dwMin			= lpEditHR->GetMin();
				uint32_t		dwMax			= lpEditHR->GetMax();
				CClientData*	lpParentData	= GetClientData(lpItem->parent());
				uint32_t		dwMaxHR			= lpParentData->GetSportMaxHR();
				lpItem->setText(0, szName);
				lpItem->setText(2, szDesc);
				lpItem->setText(3, QString("%1\% - %2\%").arg(dwMin).arg(dwMax));
				lpItem->setText(4, QString("%1 - %2").arg(dwMaxHR*dwMin/100).arg(dwMaxHR*dwMax/100));
				lpClientData->SetHRName(szName);
				lpClientData->SetHRDesc(szDesc);
				lpClientData->SetHRMin(dwMin);
				lpClientData->SetHRMax(dwMax);
			}
			delete lpEditHR;
		}
		break;
	}
	return(true);
}

bool MainWindow::EditPlan(QTreeWidgetItem* lpItem, bool bRightClick)
{
	int	iIndent	= GetIndent(lpItem);

	switch(iIndent)
	{
		case INDENT_TRAINING:
			if(bRightClick)
			{
				CEditTraining*	lpEditTraining	= new CEditTraining(this);
				lpEditTraining->SetTraining(lpItem->text(0));
				if(lpEditTraining->exec() == QDialog::Accepted)
				{
					QString	szTraining	= lpEditTraining->GetTraining();
					if(szTraining.length())
						lpItem->setText(0, szTraining);
					HasChanged();
				}
				delete lpEditTraining;
			}
			break;
		case INDENT_DAY:
			if(bRightClick)
			{
				CEditDay*		lpEditDay		= new CEditDay(this);
				CClientData*	lpClientData	= GetClientData(lpItem);
				int				iOldDay			= lpClientData->GetDay();

				lpEditDay->Set(iOldDay, lpItem->text(0).mid(lpItem->text(0).indexOf(")")+2));
				if(lpEditDay->exec() == QDialog::Accepted)
				{
					QString				szDesc	= lpEditDay->GetDesc();
					int					iDay	= lpEditDay->GetDay();
					QTreeWidgetItem*	lpDay	= FindDay(0, iDay);

					if(lpDay && lpDay != lpItem)
					{
						QTreeWidgetItem*	lpChild;
						QTreeWidgetItem*	lpChild1;
						QTreeWidgetItem*	lpOldDay	= FindDay(0, iOldDay);
						CClientData*		lpTemp;
						for(int i = 0;i < lpOldDay->childCount();i++)
						{
							lpChild	= lpOldDay->child(i);
							lpTemp	= GetClientData(lpChild);
							QString	szSport = lpTemp->GetSessionSport();
							QTreeWidgetItem*	lpSession	= AddSession(lpDay, lpTemp->GetSession(), lpTemp->GetSessionDesc(), lpTemp->GetSessionSport());
							for(int j = 0;j < lpChild->childCount();j++)
							{
								lpChild1	= lpChild->child(j);
								lpTemp		= GetClientData(lpChild1);
								CTime		Time		= lpTemp->GetTaskTime();
								uint32_t	dwTime		= (uint32_t)Time.second()+(uint32_t)Time.minute()*60+(uint32_t)Time.hour()*3600;
								CTime		PauseTime	= lpTemp->GetTaskPauseTime();
								uint32_t	dwPauseTime	= (uint32_t)PauseTime.second()+(uint32_t)PauseTime.minute()*60+(uint32_t)PauseTime.hour()*3600;

								AddTask(lpSession, lpTemp->GetTask(), lpTemp->GetTaskDesc(), szSport, lpTemp->GetTaskCount(), dwTime, lpTemp->GetTaskDist(), lpTemp->GetTaskHR(), lpTemp->GetTaskPace(), dwPauseTime, lpTemp->GetTaskPauseDist(), lpTemp->GetTaskPauseHR(), lpTemp->GetTaskPausePace());
							}
						}
						delete lpOldDay;
					}
					else
					{
						lpClientData->SetDay(iDay);
						lpClientData->SetDayDesc(szDesc);
						lpItem->setText(0, QString("(%1) %2").arg(iDay).arg(szDesc));
					}
					HasChanged();
				}
				delete lpEditDay;
			}
			break;
		case INDENT_SESSION:
			if(bRightClick)
			{
				CEditSession*		lpEditSession	= new CEditSession(this);
				QString				szDesc			= lpItem->text(0);
				QString				szSport			= lpItem->text(1);
				QTreeWidgetItem*	lpDay			= lpItem->parent();
				int					iDay;
				QString				szDayDesc;
				QStringList			DayList;
				QStringList			SportList;
				CClientData*		lpClientData;
				int					i;

				lpClientData	= GetClientData(lpDay);
				iDay			= lpClientData->GetDay();
				szDayDesc		= lpDay->text(0).mid(lpItem->text(0).indexOf(")")+2);

				for(i = 0;i < m_lpPlanRoot->childCount();i++)
					DayList << m_lpPlanRoot->child(i)->text(0);

				for(int i = 0;i < ui->m_lpSportView->topLevelItemCount();i++)
					SportList << ui->m_lpSportView->topLevelItem(i)->text(0);

				lpEditSession->Set(DayList, SportList, lpDay->text(0), szDesc, szSport);
				if(lpEditSession->exec() == QDialog::Accepted)
				{
					QString			szNewDay	= lpEditSession->GetDay();
					QString			szNewDesc	= lpEditSession->GetDesc();
					QString			szNewSport	= lpEditSession->GetSport();

					lpClientData	= GetClientData(lpItem);
					lpClientData->SetSessionDesc(szNewDesc);
					lpClientData->SetSessionSport(szNewSport);
					lpItem->setText(0, szDesc);
					lpItem->setText(1, szSport);

					if(szDayDesc != szNewDay)
					{
						QString szEmpty("");
						QTreeWidgetItem*	lpNewDay	= FindDay(szEmpty, szNewDay);
						if(lpNewDay)
						{
							QTreeWidgetItem*	lpSession	= AddSession(lpNewDay, lpClientData->GetSession(), lpClientData->GetSessionDesc(), lpClientData->GetSessionSport());
							for(int j = 0;j < lpItem->childCount();j++)
							{
								QTreeWidgetItem*	lpChild	= lpItem->child(j);
								lpClientData			= GetClientData(lpChild);
								CTime		Time		= lpClientData->GetTaskTime();
								uint32_t	dwTime		= (uint32_t)Time.second()+(uint32_t)Time.minute()*60+(uint32_t)Time.hour()*3600;
								CTime		PauseTime	= lpClientData->GetTaskPauseTime();
								uint32_t	dwPauseTime	= (uint32_t)PauseTime.second()+(uint32_t)PauseTime.minute()*60+(uint32_t)PauseTime.hour()*3600;

								AddTask(lpSession, lpClientData->GetTask(), lpClientData->GetTaskDesc(), szSport, lpClientData->GetTaskCount(), dwTime, lpClientData->GetTaskDist(), lpClientData->GetTaskHR(), lpClientData->GetTaskPace(), dwPauseTime, lpClientData->GetTaskPauseDist(), lpClientData->GetTaskPauseHR(), lpClientData->GetTaskPausePace());
							}
							delete lpItem;
						}
					}
					HasChanged();
				}
				delete lpEditSession;
			}
			break;
		case INDENT_TASK:
			{
				CClientData*		lpClientData		= GetClientData(lpItem);
				QTreeWidgetItem*	lpSession			= lpItem->parent();
				QStringList			HRList;
				int					i;
				QTreeWidgetItem*	lpSport				= FindSport(lpSession->text(COL_SPORT));
				CClientData*		lpClientDataSport	= GetClientData(lpSport);

				for(i = 0;i < lpSport->childCount();i++)
					HRList << QString("%1 (%2)").arg(lpSport->child(i)->text(0)).arg(lpSport->child(i)->text(4));

				if(lpClientDataSport->GetSportPace())
				{
					CEditTask*		lpEditTask		= new CEditTask(this);
					lpEditTask->Set(HRList, lpClientData->GetTaskDesc(), lpClientData->GetTaskCount(), lpClientData->GetTaskTime(), lpClientData->GetTaskDist(), lpClientData->GetTaskHR(), lpClientData->GetTaskPace(), lpClientData->GetTaskPauseTime(), lpClientData->GetTaskPauseDist(), lpClientData->GetTaskPauseHR(), lpClientData->GetTaskPausePace());
					if(lpEditTask->exec() == QDialog::Accepted)
					{
						lpClientData->SetTaskDesc(lpEditTask->GetDesc());
						lpClientData->SetTaskCount(lpEditTask->GetCount());
						lpClientData->SetTaskTime(lpEditTask->GetTime());
						lpClientData->SetTaskDist(lpEditTask->GetDist());
						lpClientData->SetTaskHR(lpEditTask->GetHR());
						lpClientData->SetTaskPace(lpEditTask->GetPace());
						lpClientData->SetTaskPauseTime(lpEditTask->GetPauseTime());
						lpClientData->SetTaskPauseDist(lpEditTask->GetPauseDist());
						lpClientData->SetTaskPauseHR(lpEditTask->GetPauseHR());
						lpClientData->SetTaskPausePace(lpEditTask->GetPausePace());
						SetTaskData(lpItem, lpClientData->GetTaskDesc(), lpClientData->GetTaskCount(), lpClientData->GetTaskTime().toDWORD(), lpClientData->GetTaskDist(), lpClientData->GetTaskHR(), lpClientData->GetTaskPace(), lpClientData->GetTaskPauseTime().toDWORD(), lpClientData->GetTaskPauseDist(), lpClientData->GetTaskPauseHR(), lpClientData->GetTaskPausePace(), lpSession->text(COL_SPORT));
					}
					HasChanged();
					delete lpEditTask;
				}
				else
				{
					CEditTaskS*		lpEditTask		= new CEditTaskS(this);
					lpEditTask->Set(HRList, lpClientData->GetTaskDesc(), lpClientData->GetTaskCount(), lpClientData->GetTaskTime(), lpClientData->GetTaskDist(), lpClientData->GetTaskHR(), lpClientData->GetTaskPace(), lpClientData->GetTaskPauseTime(), lpClientData->GetTaskPauseDist(), lpClientData->GetTaskPauseHR(), lpClientData->GetTaskPausePace());
					if(lpEditTask->exec() == QDialog::Accepted)
					{
						lpClientData->SetTaskDesc(lpEditTask->GetDesc());
						lpClientData->SetTaskCount(lpEditTask->GetCount());
						lpClientData->SetTaskTime(lpEditTask->GetTime());
						lpClientData->SetTaskDist(lpEditTask->GetDist());
						lpClientData->SetTaskHR(lpEditTask->GetHR());
						lpClientData->SetTaskPace(lpEditTask->GetPace());
						lpClientData->SetTaskPauseTime(lpEditTask->GetPauseTime());
						lpClientData->SetTaskPauseDist(lpEditTask->GetPauseDist());
						lpClientData->SetTaskPauseHR(lpEditTask->GetPauseHR());
						lpClientData->SetTaskPausePace(lpEditTask->GetPausePace());
						SetTaskData(lpItem, lpClientData->GetTaskDesc(), lpClientData->GetTaskCount(), lpClientData->GetTaskTime().toDWORD(), lpClientData->GetTaskDist(), lpClientData->GetTaskHR(), lpClientData->GetTaskPace(), lpClientData->GetTaskPauseTime().toDWORD(), lpClientData->GetTaskPauseDist(), lpClientData->GetTaskPauseHR(), lpClientData->GetTaskPausePace(), lpSession->text(COL_SPORT));
					}
					HasChanged();
					delete lpEditTask;
				}
			}
			break;
	}
	return(true);
}

bool MainWindow::SavePlan(const QString& szFile)
{
	if(!m_lpPlanRoot)
	{
		MESSAGE(tr("No training to save!"));
		return(false);
	}

	CTraining*	lpTraining	= new CTraining();
	lpTraining->SetTraining(m_lpPlanRoot->text(0).toLatin1().data(), ui->m_lpTrainingStart->date());
	lpTraining->SetPerson(ui->m_lpFirstName->text(), ui->m_lpLastName->text(), ui->m_lpDayOfBirth->date(), ui->m_lpHeight->value(), ui->m_lpWeight->value());

	int					i;
	int					j;
	int					k;
	CClientData*		lpSportData;
	CClientData*		lpHRData;
	CClientData*		lpDayData;
	CClientData*		lpSessionData;
	CClientData*		lpTaskData;
	QTreeWidgetItem*	lpSport;
	QTreeWidgetItem*	lpHR;
	QTreeWidgetItem*	lpDay;
	QTreeWidgetItem*	lpSession;
	QTreeWidgetItem*	lpTask;
	int					iSession;
	int					iTask;

	for(i = 0;i < ui->m_lpSportView->topLevelItemCount();i++)
	{
		lpSport		= ui->m_lpSportView->topLevelItem(i);
		lpSportData	= GetClientData(lpSport);
		lpTraining->AddSport(lpSportData->GetSportName().toLatin1().data(), lpSportData->GetSportMaxHR(), lpSportData->GetSportPace());
		for(j = 0;j < lpSport->childCount();j++)
		{
			lpHR		= lpSport->child(j);
			lpHRData	= GetClientData(lpHR);
			lpTraining->AddHR(lpSportData->GetSportName().toLatin1().data(), lpHRData->GetHRName().toLatin1().data(), lpHRData->GetHRDesc().toLatin1().data(), lpHRData->GetHRMin(), lpHRData->GetHRMax());
		}
	}

	for(i = 0;i < m_lpPlanRoot->childCount();i++)
	{
		lpDay		= m_lpPlanRoot->child(i);
		lpDayData	= GetClientData(lpDay);
		lpTraining->AddDay(lpDayData->GetDay(), lpDayData->GetDayDesc(false));
		iSession = 0;
		for(j = 0;j < lpDay->childCount();j++)
		{
			lpSession		= lpDay->child(j);
			lpSessionData	= GetClientData(lpSession);
			lpTraining->AddSession(lpDayData->GetDay(), iSession, lpSessionData->GetSessionDesc().toLatin1().data(), lpSessionData->GetSessionSport().toLatin1().data());
			iTask = 0;
			for(k = 0;k < lpSession->childCount();k++)
			{
				lpTask		= lpSession->child(k);
				lpTaskData	= GetClientData(lpTask);
				lpTraining->AddTask(lpDayData->GetDay(), iSession, iTask, lpTaskData->GetTaskDesc().toLatin1().data(), lpTaskData->GetTaskCount(), lpTaskData->GetTaskTime().toDWORD(), lpTaskData->GetTaskDist(), lpTaskData->GetTaskHR().toLatin1().data(), lpTaskData->GetTaskPace(), lpTaskData->GetTaskPauseTime().toDWORD(), lpTaskData->GetTaskPauseDist(), lpTaskData->GetTaskPauseHR().toLatin1().data(), lpTaskData->GetTaskPausePace());
				iTask++;
			}
			iSession++;
		}
	}

	if(!lpTraining->Save(szFile.toLatin1().data()))
	{
		MESSAGE(tr("Could not save training!"));
		delete lpTraining;
		return(false);
	}
	m_szFileName = szFile;
	delete lpTraining;
	Changed(false);
	setWindowTitle(QString(tr("qtTrainingPlan - "))+szFile);
	return(true);
}

void MainWindow::HasChanged()
{
	Changed(true);
	DrawCalendar();
}

void MainWindow::DrawCalendar()
{
	if(!m_lpPlanRoot)
		return;

	QTreeWidgetItem*	lpDay;
	QTreeWidgetItem*	lpSession;
	CClientData*		lpDayData;
	CClientData*		lpSessionData;
	int					iOld		= 1;
	int					i;
	int					iDay		= 0;
	QString				szText("<table style='color: #ffffff;' width='100%'>");
	QDate				Date(2009, 11, 16);
	QDate				Day	= ui->m_lpTrainingStart->date();

	szText	= QString("<body><table border=1 style='color: #ffffff;' width='100%'><tr>");
	szText	+= QString("<tr height=100>");

	Day			= Day.addDays(-Day.dayOfWeek()+1);
	while(Day < ui->m_lpTrainingStart->date())
	{
		szText	+= QString("<td width='14%'>");
		szText	+= QString("<div style='height:100px; width=100%; overflow:auto; solid #840;'>");
		szText	+= QString("<table width='100%'>");
		szText	+= QString("<tr>");
		szText	+= QString("<td colspan='2' align='center' bgcolor='#000000' style='color: #ffffff;'>")+Day.toString("ddd, dd.MM.yyyy")+QString("</td>");
		szText	+= QString("</tr>");
		szText	+= QString("</table>");
		szText	+= QString("</div>");
		szText	+= QString("</td>");
		iDay++;
		Day	= Day.addDays(1);
	}

	for(i = 0;i < m_lpPlanRoot->childCount();i++)
	{
		lpDay		= m_lpPlanRoot->child(i);
		lpDayData	= GetClientData(lpDay);

		while(iOld < lpDayData->GetDay())
		{
			szText	+= QString("<td width='14%'>");
			szText	+= QString("<div style='height:100px; width=100%; overflow:auto; solid #840;'>");
			szText	+= QString("<table width='100%'>");
			szText	+= QString("<tr>");
			szText	+= QString("<td colspan='2' align='center' bgcolor='#000000' style='color: #ffffff;'>")+Day.toString("ddd, dd.MM.yyyy")+QString("</td>");
			szText	+= QString("</tr>");
			szText	+= QString("</table>");
			szText	+= QString("</div>");
			szText	+= QString("</td>");

			Day	= Day.addDays(1);
			iDay++;
			if(iDay == 7)
			{
				iDay = 0;
				szText += QString("</tr>");
				szText += QString("<tr height=100>");
			}
			iOld++;
		}

		szText	+= QString("<td width='14%'>");
		szText	+= QString("<div style='height:100px; width=100%; overflow:auto; solid #840;'>");
		szText	+= QString("<table width='100%'>");
		szText	+= QString("<tr>");
		szText	+= QString("<td colspan='2' align='center' bgcolor='#000000' style='color: #ffffff;'>")+Day.toString("ddd, dd.MM.yyyy")+QString("</td>");
		szText	+= QString("</tr>");
		szText	+= QString("<tr>");
		szText	+= QString("<td colspan='2' align='center' bgcolor='#0000ff' style='color: #ffffff;'><a href='DAY")+QString("%1").arg(lpDayData->GetDay())+QString("' style='color:white'>")+lpDay->text(0).mid(lpDay->text(0).indexOf(")")+2)+QString("</td>");
		szText	+= QString("</tr>");

		for(int j = 0;j < lpDay->childCount();j++)
		{
			lpSession		= lpDay->child(j);
			lpSessionData	= GetClientData(lpSession);

			szText	+= QString("<tr style='font-size:12'>");
			szText	+= QString("<td bgcolor='#f0f0f0'><a href='DAY")+QString("%1").arg(lpDayData->GetDay())+QString("SESSION")+QString("%1").arg(lpSessionData->GetSession())+QString("' style='color:black'>")+lpSession->text(0)+QString("</td><td bgcolor='#f0f0f0' align='right'><a href='DAY")+QString("%1").arg(lpDayData->GetDay())+QString("SESSION")+QString("%1").arg(lpSessionData->GetSession())+QString("' style='color:black'>")+lpSession->text(1)+QString("</td>");
			szText	+= QString("</tr>");
			szText	+= QString("<tr style='font-size:10'>");
			szText	+= QString("<td><a href='DAY")+QString("%1").arg(lpDayData->GetDay())+QString("SESSION")+QString("%1").arg(lpSessionData->GetSession())+QString("' style='color:black'>")+lpSession->text(3)+QString("</td><td align='right'><a href='DAY")+QString("%1").arg(lpDayData->GetDay())+QString("SESSION")+QString("%1").arg(lpSessionData->GetSession())+QString("' style='color:black'>")+lpSession->text(4)+QString("</td>");
			szText	+= QString("</tr>");
		}
		szText	+= QString("</table>");
		szText	+= QString("</div>");
		szText	+= QString("</td>");

		Day	= Day.addDays(1);
		iDay++;
		if(iDay == 7)
		{
			iDay = 0;
			szText += QString("</tr>");
			szText += QString("<tr height=100>");
		}
		iOld++;
	}
	szText += QString("</tr>");
	szText += QString("</table></body");
	ui->m_lpCalendar->setHtml(szText);
}

uint32_t MainWindow::GetMaxDays()
{
	if(!m_lpPlanRoot)
		return(1);

	uint32_t		iMaxDay = 0;
	uint32_t		iDay;
	CClientData*	lpDayData;

	for(int i = 0;i < m_lpPlanRoot->childCount();i++)
	{
		lpDayData	= GetClientData(m_lpPlanRoot->child(i));
		iDay		= lpDayData->GetDay();
		if(iDay > iMaxDay)
			iMaxDay = iDay;
	}
	return(iMaxDay);
}

void MainWindow::Changed(bool bChanged)
{
	QString	szTitle	= windowTitle();
	m_bHasChanged	= bChanged;
	if(bChanged)
	{
		if(szTitle.right(1) != QString("*"))
			szTitle	= szTitle+QString(" *");
	}
	else
	{
		if(szTitle.right(1) == QString("*"))
			szTitle	= szTitle.left(szTitle.length()-2);
	}
	setWindowTitle(szTitle);
}

void MainWindow::on_action_Print_triggered()
{
}


#define MY_BLACK	QColor(2, 2, 2)
#define MY_WHITE	QColor(255, 255, 255)
#define MY_BLUE		QColor(0, 0, 255)
#define MY_LTBLUE	QColor(127, 127, 255)
#define MY_GREY		QColor(191, 191, 191)
#define MY_RED		QColor(255, 0, 0)
#define MY_YELLOW	QColor(255, 255, 0)

void Draw(QPainter* lpPainter, const QColor& Pen, int iX, int iY, int iW)
{
	lpPainter->setPen(Pen);
	lpPainter->drawLine(iX, iY, iX+iW-1, iY);
}

void Draw(QPainter* lpPainter, const QColor& Pen, const QColor& Brush, int iX, int iY, int iW, int iH)
{
	lpPainter->setPen(Pen);
	lpPainter->setBrush(QBrush(Brush));
	lpPainter->drawRect(iX, iY, iW, iH);
}

void Draw(QPainter* lpPainter, const QColor& Pen, const QColor& Brush, int iX, int iY, int iW, int iH, const QColor& FontColor, const QFont& Font, int iFlags, const QString& szText)
{
	Draw(lpPainter, Pen, Brush, iX, iY, iW, iH);

	lpPainter->setPen(FontColor);
	lpPainter->setFont(Font);
	lpPainter->drawText(QRect(iX, iY, iW, iH), iFlags, szText);
}

void Draw(QPainter* lpPainter, const QColor& Brush, int iX, int iY, int iW, int iH, const QColor& FontColor, const QFont& Font, int iFlags, const QString& szText)
{
	lpPainter->setBrush(QBrush(Brush));
	lpPainter->setPen(FontColor);
	lpPainter->setFont(Font);
	lpPainter->drawText(QRect(iX, iY, iW, iH), iFlags, szText);
}

void MainWindow::on_action_PDF_triggered()
{
	QPrinter printer(QPrinter::HighResolution);

	CExportPDF*	lpExportPDF	= new CExportPDF(this);
	if(lpExportPDF->exec() != QDialog::Accepted)
	{
		delete lpExportPDF;
		return;
	}

	int		iFormat			= lpExportPDF->GetFormat();
	int		iWeeksPerSheet	= lpExportPDF->GetWeeks();
	int		iUnit			= lpExportPDF->GetUnit();
	double	dLeft			= lpExportPDF->GetLeft();
	double	dRight			= lpExportPDF->GetRight();
	double	dTop			= lpExportPDF->GetTop();
	double	dBottom			= lpExportPDF->GetBottom();
	QString	szFileName		= lpExportPDF->GetFile();
	delete lpExportPDF;

	if(szFileName.indexOf(".pdf", 0, Qt::CaseInsensitive) == -1)
		szFileName.append(".pdf");

	QFile	File(szFileName);
	if(!File.open(QIODevice::ReadWrite))
	{
		QMessageBox::critical(this, tr("Export PDF"), QString(tr("Can't export to %1").arg(szFileName)));
		return;
	}
	File.close();

	printer.setOutputFormat(QPrinter::PdfFormat);
	printer.setOrientation(QPrinter::Landscape);
	printer.setPaperSize((QPrinter::PageSize)iFormat);
	printer.setPageSize((QPrinter::PageSize)iFormat);
	printer.setPageMargins(dLeft, dTop, dRight, dBottom, (QPrinter::Unit)iUnit);
	printer.newPage();

	printer.setOutputFileName(szFileName);
	QPainter painter;

	painter.begin(&printer);

	QRect	PrintArea	= QRect(0, 0, printer.pageRect().right()-printer.pageRect().left(), printer.pageRect().bottom()-printer.pageRect().top());
	int		iCalendarWidth	= PrintArea.width()/22*3;
	int		iCalendarHeight	= PrintArea.height()/iWeeksPerSheet;
	int		iSumWidth		= PrintArea.width()-iCalendarWidth*7;
	int		iDateHeight		= iCalendarHeight/8;
	int		iDayHeight		= iCalendarHeight/8;
	int		iSumHeight		= iCalendarHeight/8;
	int		iRowHeight		= (iCalendarHeight-iDateHeight-iDayHeight-iSumHeight)/6;
	int		iCol1			= (iCalendarWidth-2)/5;
	int		iCol3			= iCol1*2;
	int		iCol2			= iCalendarWidth-2-iCol1-iCol3;

	QDate				Day		= ui->m_lpTrainingStart->date();
	int					iDay	= 0;
	int					iWeek	= 0;
	int					i;
	QTreeWidgetItem*	lpDay;
	QTreeWidgetItem*	lpSession;
	QTreeWidgetItem*	lpTask;
	CClientData*		lpDayData;
	CClientData*		lpSessionData;
	CClientData*		lpTaskData;
	int					iOld		= 1;

	uint32_t			dwWeekTime	= 0;
	uint32_t			dwWeekDist	= 0;

	QFont				DefaultFont	= painter.font();

	QFont				DateFont	= DefaultFont;
	DateFont.setPixelSize(iDateHeight*2/3);
	DateFont.setBold(true);

	QFont				DayFont		= DefaultFont;
	DayFont.setPixelSize(iDayHeight*2/3);

	QFont				SumFont		= DefaultFont;
	SumFont.setPixelSize(iSumHeight*2/3);
	SumFont.setBold(true);

	QFont				RowFont		= DefaultFont;
	RowFont.setPixelSize(iRowHeight*2/3);


	Day			= Day.addDays(-Day.dayOfWeek()+1);
	while(Day < ui->m_lpTrainingStart->date())
	{
		Draw(&painter, MY_BLACK, MY_WHITE, iDay*iCalendarWidth, iWeek*iCalendarHeight, iCalendarWidth, iCalendarHeight);
		Draw(&painter, MY_BLACK, MY_BLACK, iDay*iCalendarWidth, iWeek*iCalendarHeight, iCalendarWidth, iDateHeight, MY_WHITE, DateFont, Qt::AlignCenter | Qt::AlignHCenter, Day.toString("ddd, dd.MM.yyyy"));

		iDay++;
		if(iDay >= 7)
		{
			CTime WeekTime(dwWeekTime);

			Draw(&painter, MY_BLACK, MY_YELLOW, 7*iCalendarWidth, iWeek*iCalendarHeight, iSumWidth, iCalendarHeight);
			Draw(&painter, MY_YELLOW, 7*iCalendarWidth, iWeek*iCalendarHeight, iSumWidth, iDateHeight, MY_BLACK, DateFont, Qt::AlignCenter | Qt::AlignHCenter, "Total");

			painter.save();
			painter.translate(7*iCalendarWidth, (iWeek+1)*iCalendarHeight);
			painter.rotate(270);
			Draw(&painter, MY_YELLOW, 0, 0, iCalendarHeight-iDateHeight-iDayHeight, iSumWidth/2, MY_BLACK, SumFont, Qt::AlignCenter | Qt::AlignHCenter, QString("%1").arg(dwWeekDist));
			painter.restore();

			painter.save();
			painter.translate(7*iCalendarWidth+iSumWidth/2, (iWeek+1)*iCalendarHeight);
			painter.rotate(270);
			Draw(&painter, MY_YELLOW, 0, 0, iCalendarHeight-iDateHeight-iDayHeight, iSumWidth/2, MY_BLACK, SumFont, Qt::AlignCenter | Qt::AlignHCenter, WeekTime.Format());
			painter.restore();

			dwWeekTime = 0;
			dwWeekDist = 0;

			iDay = 0;
			iWeek++;
			if(iWeek >= iWeeksPerSheet)
			{
				iWeek = 0;
				printer.newPage();
			}
		}
		Day	= Day.addDays(1);
	}

	for(i = 0;i < m_lpPlanRoot->childCount();i++)
	{
		lpDay		= m_lpPlanRoot->child(i);
		lpDayData	= GetClientData(lpDay);

		while(iOld < lpDayData->GetDay())
		{
			Draw(&painter, MY_BLACK, MY_WHITE, iDay*iCalendarWidth, iWeek*iCalendarHeight, iCalendarWidth, iCalendarHeight);
			Draw(&painter, MY_BLACK, MY_BLACK, iDay*iCalendarWidth, iWeek*iCalendarHeight, iCalendarWidth, iDateHeight, MY_WHITE, DateFont, Qt::AlignCenter | Qt::AlignHCenter, Day.toString("ddd, dd.MM.yyyy"));

			Day	= Day.addDays(1);
			iDay++;
			if(iDay >= 7)
			{
				CTime WeekTime(dwWeekTime);

				Draw(&painter, MY_BLACK, MY_YELLOW, 7*iCalendarWidth, iWeek*iCalendarHeight, iSumWidth, iCalendarHeight);
				Draw(&painter, MY_YELLOW, 7*iCalendarWidth, iWeek*iCalendarHeight, iSumWidth, iDateHeight, MY_BLACK, DateFont, Qt::AlignCenter | Qt::AlignHCenter, "Total");

				painter.save();
				painter.translate(7*iCalendarWidth, (iWeek+1)*iCalendarHeight);
				painter.rotate(270);
				Draw(&painter, MY_YELLOW, 0, 0, iCalendarHeight-iDateHeight-iDayHeight, iSumWidth/2, MY_BLACK, SumFont, Qt::AlignCenter | Qt::AlignHCenter, QString("%1").arg(dwWeekDist));
				painter.restore();

				painter.save();
				painter.translate(7*iCalendarWidth+iSumWidth/2, (iWeek+1)*iCalendarHeight);
				painter.rotate(270);
				Draw(&painter, MY_YELLOW, 0, 0, iCalendarHeight-iDateHeight-iDayHeight, iSumWidth/2, MY_BLACK, SumFont, Qt::AlignCenter | Qt::AlignHCenter, WeekTime.Format());
				painter.restore();

				dwWeekTime = 0;
				dwWeekDist = 0;

				iDay = 0;
				iWeek++;
				if(iWeek >= iWeeksPerSheet)
				{
					iWeek = 0;
					printer.newPage();
				}
			}
			iOld++;
		}

		Draw(&painter, MY_BLACK, MY_WHITE, iDay*iCalendarWidth, iWeek*iCalendarHeight, iCalendarWidth, iCalendarHeight);
		Draw(&painter, MY_BLACK, MY_BLACK, iDay*iCalendarWidth, iWeek*iCalendarHeight, iCalendarWidth, iDateHeight, MY_WHITE, DateFont, Qt::AlignCenter | Qt::AlignHCenter, Day.toString("ddd, dd.MM.yyyy"));

		Draw(&painter, MY_BLUE, MY_BLUE, iDay*iCalendarWidth+1, iWeek*iCalendarHeight+iDateHeight, iCalendarWidth-2, iDayHeight, MY_WHITE, DayFont, Qt::AlignCenter | Qt::AlignHCenter, lpDayData->GetDayDesc(false));


		uint32_t	dwTotalTime	= 0;
		uint32_t	dwTotalDist	= 0;
		uint32_t	dwCount;
		int			iTask		= 0;

		for(int j = 0;j < lpDay->childCount();j++)
		{
			lpSession		= lpDay->child(j);
			lpSessionData	= GetClientData(lpSession);

			Draw(&painter, MY_BLACK, MY_LTBLUE, iDay*iCalendarWidth+1, iWeek*iCalendarHeight+iDateHeight+iDayHeight+iRowHeight*iTask, iCalendarWidth-2, iRowHeight, MY_BLACK, RowFont, Qt::AlignCenter | Qt::AlignHCenter, lpSession->text(1));
			iTask++;

			for(int k = 0;k < lpSession->childCount();k++)
			{
				lpTask		= lpSession->child(k);
				lpTaskData	= GetClientData(lpTask);


				dwCount		= lpTaskData->GetTaskCount();
				dwTotalTime	+= lpTaskData->GetTaskTime().toDWORD()*dwCount;
				dwTotalDist	+= lpTaskData->GetTaskDist()*dwCount;
				if(dwCount > 1)
				{
					dwTotalTime	+= lpTaskData->GetTaskPauseTime().toDWORD()*(dwCount-1);
					dwTotalDist	+= lpTaskData->GetTaskDist()*(dwCount-1);
				}

				Draw(&painter, MY_WHITE, iDay*iCalendarWidth+1,				iWeek*iCalendarHeight+iDateHeight+iDayHeight+iRowHeight*iTask, iCol1, iRowHeight, MY_BLACK, RowFont, Qt::AlignRight | Qt::AlignHCenter, QString("%1x").arg(dwCount));
				Draw(&painter, MY_WHITE, iDay*iCalendarWidth+1+iCol1,		iWeek*iCalendarHeight+iDateHeight+iDayHeight+iRowHeight*iTask, iCol2, iRowHeight, MY_BLACK, RowFont, Qt::AlignRight | Qt::AlignHCenter, QString("%1").arg(lpTaskData->GetTaskDist()));
				Draw(&painter, MY_WHITE, iDay*iCalendarWidth-5+iCol1+iCol2,	iWeek*iCalendarHeight+iDateHeight+iDayHeight+iRowHeight*iTask, iCol3, iRowHeight, MY_BLACK, RowFont, Qt::AlignRight | Qt::AlignHCenter, DWORD2Time(lpTaskData->GetTaskPace()));
				iTask++;

				if(dwCount > 1)
				{
					Draw(&painter, MY_WHITE, iDay*iCalendarWidth+1,				iWeek*iCalendarHeight+iDateHeight+iDayHeight+iRowHeight*iTask, iCol1, iRowHeight, MY_BLACK, RowFont, Qt::AlignRight | Qt::AlignHCenter, "P");
					Draw(&painter, MY_WHITE, iDay*iCalendarWidth+1+iCol1,		iWeek*iCalendarHeight+iDateHeight+iDayHeight+iRowHeight*iTask, iCol2, iRowHeight, MY_BLACK, RowFont, Qt::AlignRight | Qt::AlignHCenter, QString("%1").arg(lpTaskData->GetTaskPauseDist()));
					Draw(&painter, MY_WHITE, iDay*iCalendarWidth-5+iCol1+iCol2,	iWeek*iCalendarHeight+iDateHeight+iDayHeight+iRowHeight*iTask, iCol3, iRowHeight, MY_BLACK, RowFont, Qt::AlignRight | Qt::AlignHCenter, DWORD2Time(lpTaskData->GetTaskPausePace()));
					iTask++;
					if(iWeek >= iWeeksPerSheet)
					{
						iWeek = 0;
						printer.newPage();
					}
				}
				Draw(&painter, MY_BLACK, iDay*iCalendarWidth+1,				iWeek*iCalendarHeight+iDateHeight+iDayHeight+iRowHeight*iTask, iCalendarWidth-2);
			}
		}
		CTime TotalTime(dwTotalTime);
		Draw(&painter, MY_BLACK, MY_GREY, iDay*iCalendarWidth+1, iWeek*iCalendarHeight+iCalendarHeight-iSumHeight, iCalendarWidth-2, iSumHeight);
		Draw(&painter, MY_GREY, iDay*iCalendarWidth+1, iWeek*iCalendarHeight+iCalendarHeight-iSumHeight, iCol1, iSumHeight, MY_BLACK, SumFont, Qt::AlignRight | Qt::AlignHCenter, "Tot");
		Draw(&painter, MY_GREY, iDay*iCalendarWidth+1+iCol1, iWeek*iCalendarHeight+iCalendarHeight-iSumHeight, iCol2, iSumHeight, MY_BLACK, SumFont, Qt::AlignRight | Qt::AlignHCenter, QString("%1").arg(dwTotalDist));
		Draw(&painter, MY_GREY, iDay*iCalendarWidth+1+iCol1+iCol2, iWeek*iCalendarHeight+iCalendarHeight-iSumHeight, iCol3, iSumHeight, MY_BLACK, SumFont, Qt::AlignRight | Qt::AlignHCenter, TotalTime.Format());

		dwWeekTime += dwTotalTime;
		dwWeekDist += dwTotalDist;

		Day	= Day.addDays(1);
		iDay++;
		if(iDay >= 7)
		{
			CTime WeekTime(dwWeekTime);

			Draw(&painter, MY_BLACK, MY_YELLOW, 7*iCalendarWidth, iWeek*iCalendarHeight, iSumWidth, iCalendarHeight);
			Draw(&painter, MY_YELLOW, 7*iCalendarWidth, iWeek*iCalendarHeight, iSumWidth, iDateHeight, MY_BLACK, DateFont, Qt::AlignCenter | Qt::AlignHCenter, "Total");

			painter.save();
			painter.translate(7*iCalendarWidth, (iWeek+1)*iCalendarHeight);
			painter.rotate(270);
			Draw(&painter, MY_YELLOW, 0, 0, iCalendarHeight-iDateHeight-iDayHeight, iSumWidth/2, MY_BLACK, SumFont, Qt::AlignCenter | Qt::AlignHCenter, QString("%1").arg(dwWeekDist));
			painter.restore();

			painter.save();
			painter.translate(7*iCalendarWidth+iSumWidth/2, (iWeek+1)*iCalendarHeight);
			painter.rotate(270);
			Draw(&painter, MY_YELLOW, 0, 0, iCalendarHeight-iDateHeight-iDayHeight, iSumWidth/2, MY_BLACK, SumFont, Qt::AlignCenter | Qt::AlignHCenter, WeekTime.Format());
			painter.restore();

			dwWeekTime = 0;
			dwWeekDist = 0;

			iDay = 0;
			iWeek++;
			if(iWeek >= iWeeksPerSheet)
			{
				iWeek = 0;
				printer.newPage();
			}
		}
		iOld++;
	}

	painter.end();
}
