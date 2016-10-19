#ifndef CEDITTRAINING_H
#define CEDITTRAINING_H

#include <QDialog>

namespace Ui {
    class CEditTraining;
}

class CEditTraining : public QDialog {
    Q_OBJECT
public:
    CEditTraining(QWidget *parent = 0);
    ~CEditTraining();

	QString GetTraining();
	void SetTraining(const QString& szTraining);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::CEditTraining *m_ui;

private slots:
	void on_buttonBox_rejected();
	void on_buttonBox_accepted();
};

#endif // CEDITTRAINING_H
