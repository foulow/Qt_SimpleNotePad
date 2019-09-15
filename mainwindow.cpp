#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setCentralWidget(ui->textEdit);
    Qt::WindowFlags flags = windowFlags();
    Qt::WindowFlags closeFlag = Qt::WindowCloseButtonHint;
    flags = flags & (~closeFlag);
    setWindowFlags(flags);
    currentFile = "Unknown ";
    setWindowTitle(currentFile);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionNew_triggered()
{
    if (currentFile[currentFile.length()-1] == "*")
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Warning", "Are you sure, you want to discard the changes?",
                                      QMessageBox::Yes | QMessageBox::No);
        if(reply == QMessageBox::No)
        {
            return;
        }
    }
    currentFile = "Unknown ";
    setWindowTitle(currentFile);
    lastTextSaved.clear();
    ui->textEdit->setText(QString());
}

void MainWindow::on_actionOpen_triggered()
{
    if (currentFile[currentFile.length()-1] == "*")
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Warning", "Are you sure, you want to discard the changes?",
                                      QMessageBox::Yes | QMessageBox::No);
        if(reply == QMessageBox::No)
        {
            return;
        }
    }
    QString fileName = QFileDialog::getOpenFileName(this, "Open the file");
    QFile file(fileName);

    if(!file.open(QIODevice::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this, "Warning", "Cannot open file : " + file.errorString());
        currentFile = "Unknown ";
        setWindowTitle(currentFile);
        return;
    }
    currentFile = QString(fileName + " ");
    setWindowTitle(currentFile);
    QTextStream in(&file);
    QString text = in.readAll();
    lastTextSaved = text;
    ui->textEdit->setText(text);
    file.close();
}

void MainWindow::on_actionSave_as_triggered()
{
    QString text = ui->textEdit->toPlainText();
    if (lastTextSaved == text)
    {
        return;
    }
    QString fileName;
    if (currentFile == "Unknown " || currentFile == "Unknown*" )
    {
        fileName = QFileDialog::getSaveFileName(this, "Save as");
    }
    else
    {
        fileName = currentFile.left(currentFile.length()-1);
    }
    QFile file(fileName);

    if(!file.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this, "Warning", "Cannot save file : " + file.errorString());
        return;
    }
    currentFile = QString(fileName + " ");
    setWindowTitle(currentFile);
    QTextStream out(&file);
    out << text;
    lastTextSaved = text;
    file.close();
}

void MainWindow::on_actionPrint_triggered()
{
    QPrinter printer;
    printer.setPrinterName("Printer Name");
    QPrintDialog pDialog(&printer, this);

    if(pDialog.exec() == QDialog::Rejected)
    {
        QMessageBox::warning(this, "Warning", "Cannot access printer");
        return;
    }
    ui->textEdit->print(&printer);
}

void MainWindow::on_actionExit_triggered()
{
    if (currentFile[currentFile.length()-1] == "*")
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Warning", "Are you sure, you want to discard the changes?",
                                      QMessageBox::Yes | QMessageBox::No);
        if(reply == QMessageBox::No)
        {
            return;
        }
    }
    QApplication::exit();
}

void MainWindow::on_actionCopy_triggered()
{
    ui->textEdit->copy();
}

void MainWindow::on_actionPaste_triggered()
{
    ui->textEdit->paste();
}

void MainWindow::on_actionCut_triggered()
{
    ui->textEdit->cut();
}

void MainWindow::on_actionUndo_triggered()
{
    ui->textEdit->undo();
}

void MainWindow::on_actionRedo_triggered()
{
    ui->textEdit->redo();
}

void MainWindow::on_textEdit_textChanged()
{
    bool isEqualLastTextSaved = (lastTextSaved == ui->textEdit->toPlainText());
    if (isEqualLastTextSaved && currentFile[currentFile.length()-1] == "*")
    {
        currentFile.replace(currentFile.length()-1, 1, " ");
        setWindowTitle(currentFile);
    }
    else if (!isEqualLastTextSaved && currentFile[currentFile.length()-1] == " ")
    {
        currentFile.replace(currentFile.length()-1, 1, "*");
        setWindowTitle(currentFile);
    }
}

void MainWindow::on_actionQt_SimpleNotePad_triggered()
{
    QMessageBox::information(this, "Information",
    "App Name: Qt_SimpleNotePad\nApp Vertion: v1.0.0\nDeveloped by: Jeffrey I. Jose\nDate: September 15, 2019",
                             QMessageBox::Ok);
}
