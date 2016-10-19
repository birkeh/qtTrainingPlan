#include <QFileDialog>
#include "cexportpdf.h"
#include "ui_cexportpdf.h"


CExportPDF::CExportPDF(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CExportPDF)
{
    ui->setupUi(this);

	ui->m_lpFormat->addItem("A0",			QVariant(5));
	ui->m_lpFormat->addItem("A1",			QVariant(6));
	ui->m_lpFormat->addItem("A2",			QVariant(7));
	ui->m_lpFormat->addItem("A3",			QVariant(8));
	ui->m_lpFormat->addItem("A4",			QVariant(0));
	ui->m_lpFormat->addItem("A5",			QVariant(9));
	ui->m_lpFormat->addItem("A6",			QVariant(10));
	ui->m_lpFormat->addItem("A7",			QVariant(11));
	ui->m_lpFormat->addItem("A8",			QVariant(12));
	ui->m_lpFormat->addItem("A9",			QVariant(13));
	ui->m_lpFormat->addItem("B0",			QVariant(14));
	ui->m_lpFormat->addItem("B1",			QVariant(15));
	ui->m_lpFormat->addItem("B2",			QVariant(17));
	ui->m_lpFormat->addItem("B3",			QVariant(18));
	ui->m_lpFormat->addItem("B4",			QVariant(19));
	ui->m_lpFormat->addItem("B5",			QVariant(1));
	ui->m_lpFormat->addItem("B6",			QVariant(20));
	ui->m_lpFormat->addItem("B7",			QVariant(21));
	ui->m_lpFormat->addItem("B8",			QVariant(22));
	ui->m_lpFormat->addItem("B9",			QVariant(23));
	ui->m_lpFormat->addItem("B10",			QVariant(16));
	ui->m_lpFormat->addItem("C5E",			QVariant(24));
	ui->m_lpFormat->addItem("Comm10E",		QVariant(25));
	ui->m_lpFormat->addItem("DLE",			QVariant(26));
	ui->m_lpFormat->addItem("Executive",	QVariant(4));
	ui->m_lpFormat->addItem("Folio",		QVariant(27));
	ui->m_lpFormat->addItem("Ledger",		QVariant(28));
	ui->m_lpFormat->addItem("Legal",		QVariant(3));
	ui->m_lpFormat->addItem("Letter",		QVariant(2));
	ui->m_lpFormat->addItem("Tabloid",		QVariant(29));
	ui->m_lpFormat->setCurrentIndex(ui->m_lpFormat->findText("A4"));

	ui->m_lpUnit->addItem("Millimeter",		QVariant(0));
	ui->m_lpUnit->addItem("Point",			QVariant(1));
	ui->m_lpUnit->addItem("Inch",			QVariant(2));
	ui->m_lpUnit->addItem("Pica",			QVariant(3));
	ui->m_lpUnit->addItem("Didot",			QVariant(4));
	ui->m_lpUnit->addItem("Cicero",			QVariant(5));
	ui->m_lpUnit->addItem("DevicePixel",	QVariant(6));
	ui->m_lpUnit->setCurrentIndex(0);
}

CExportPDF::~CExportPDF()
{
    delete ui;
}

void CExportPDF::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

int CExportPDF::GetFormat()
{
	return(ui->m_lpFormat->itemData(ui->m_lpFormat->currentIndex(), Qt::UserRole).toInt());
}

int	CExportPDF::GetWeeks()
{
	return(ui->m_lpWeeksPerSheet->value());
}

int CExportPDF::GetUnit()
{
	return(ui->m_lpUnit->itemData(ui->m_lpUnit->currentIndex(), Qt::UserRole).toInt());
}

double CExportPDF::GetLeft()
{
	return(ui->m_lpLeft->value());
}

double CExportPDF::GetRight()
{
	return(ui->m_lpRight->value());
}

double CExportPDF::GetTop()
{
	return(ui->m_lpTop->value());
}

double CExportPDF::GetBottom()
{
	return(ui->m_lpBottom->value());
}

QString CExportPDF::GetFile()
{
	return(ui->m_lpFileName->text());
}

void CExportPDF::on_m_lpFileNameBrowse_clicked()
{
	QString szFileName = QFileDialog::getSaveFileName(this, "Save PDF", QDir::homePath(), tr("PDF (*.pdf)"));
	if(szFileName.length())
		ui->m_lpFileName->setText(szFileName);
}

void CExportPDF::on_m_lpCancel_clicked()
{
	reject();
}

void CExportPDF::on_m_lpOK_clicked()
{
	if(ui->m_lpFileName->text().length())
		accept();
}
