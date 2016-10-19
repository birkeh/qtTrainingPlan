#ifndef CEXPORTPDF_H
#define CEXPORTPDF_H

#include <QDialog>
#include <QAbstractButton>


namespace Ui {
    class CExportPDF;
}

class CExportPDF : public QDialog {
    Q_OBJECT
public:
    CExportPDF(QWidget *parent = 0);
    ~CExportPDF();

	int GetFormat();
	int	GetWeeks();
	int GetUnit();
	double GetLeft();
	double GetRight();
	double GetTop();
	double GetBottom();
	QString GetFile();
protected:
    void changeEvent(QEvent *e);

private:
    Ui::CExportPDF *ui;

private slots:
	void on_m_lpOK_clicked();
	void on_m_lpCancel_clicked();
	void on_m_lpFileNameBrowse_clicked();
};

#endif // CEXPORTPDF_H
