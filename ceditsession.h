#ifndef CEDITSESSION_H
#define CEDITSESSION_H

#include <QDialog>

namespace Ui {
    class CEditSession;
}

class CEditSession : public QDialog {
    Q_OBJECT
public:
    CEditSession(QWidget *parent = 0);
    ~CEditSession();

	QString GetDay();
	QString GetDesc();
	QString GetSport();
	void Set(const QStringList& DayList, const QStringList& SportList, const QString& szDay, const QString& szDesc, const QString& szSport);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::CEditSession *m_ui;

private slots:
	void on_buttonBox_accepted();
	void on_buttonBox_rejected();
};

#endif // CEDITSESSION_H
