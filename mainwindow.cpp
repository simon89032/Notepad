#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTextCursor>
#include <QTextBlock>
#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QAction>
#include <QShortcut>
#include <QSaveFile>
#include <QVBoxLayout>
#include <QTextCursor>
#include <QTextCharFormat>
#include <QTextCodec>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    textEdit = ui->textEdit;
    this->setCentralWidget(textEdit);

    ui->actionNew->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_N));
    ui->actionOpen->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_O));
    ui->actionSave->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_S));
    ui->actionSave_As->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_S));
    //ui->actionFind->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_F));

    QFont ccode("Cascadia Code", 10);
    ui->statusbar->showMessage(QString("Ln 1, Col 1"));

    encodingLabel = new QLabel();
    encodingLabel->setObjectName("encodingLabel");

    statusBar()->addPermanentWidget(encodingLabel);
    QTextCodec* codec = QTextCodec::codecForLocale();
    QString encoding = codec->name();

    encodingLabel->setText("Encoding: " + encoding);
    encodingLabel->setFont(ccode);

    connect(ui->textEdit, &QTextEdit::cursorPositionChanged, this, &MainWindow::updateLineNumber);
    connect(textEdit, &QTextEdit::textChanged, this, &MainWindow::textModified);

    QShortcut *shortcut = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_F), this);
    connect(shortcut, &QShortcut::activated, this, &MainWindow::showFindDialog);

    updateMenuItemWithShortcut(ui->actionNew);
    updateMenuItemWithShortcut(ui->actionOpen);
    updateMenuItemWithShortcut(ui->actionSave);
    updateMenuItemWithShortcut(ui->actionSave_As);
    updateMenuItemWithShortcut(ui->actionFind);

    statusBar()->setSizeGripEnabled(false);

    updateWindowTitle();
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showFindDialog()
{
    findDialog = new QDialog(this);
    findDialog->setWindowTitle("Find Text");

    findLineEdit = new QLineEdit(findDialog);

    findButton = new QPushButton("Find", findDialog);

    QVBoxLayout *layout = new QVBoxLayout(findDialog);
    layout->addWidget(findLineEdit);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(findButton);

    layout->addLayout(buttonLayout);

    connect(findButton, &QPushButton::clicked, this, &MainWindow::find);

    findDialog->show();
}

void MainWindow::textModified()
{
    isModified = true;
    updateWindowTitle();
}

void MainWindow::updateLineNumber()
{
    int line = ui->textEdit->textCursor().blockNumber() + 1;
    int column = ui->textEdit->textCursor().columnNumber() + 1;
    ui->statusbar->showMessage(QString("Ln %1, Col %2").arg(line).arg(column));
}

void MainWindow::updateWindowTitle()
{
    QString title = "Notepad";

    if (!currentFilePath.isEmpty())
    {
        title += " - " + currentFilePath;
    }

    if (isModified)
    {
        title += " *";
    }

    setWindowTitle(title);
}

void MainWindow::updateMenuItemWithShortcut(QAction *action)
{
    QString shortcutText = action->shortcut().toString();
    if (!shortcutText.isEmpty()) {
        QString originalText = action->text();
        action->setText(originalText + "\t" + shortcutText);
    }
}

void MainWindow::on_actionOpen_triggered()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Open File", "", "Text Files (*.txt)");

    if (!filePath.isEmpty())
    {
        QFile file(filePath);
        if (file.open(QFile::ReadOnly | QFile::Text))
        {
            QTextStream in(&file);
            textEdit->setPlainText(in.readAll());
            file.close();
            currentFilePath = filePath;
        }
    }

    isModified = false;
    updateWindowTitle();
}

void MainWindow::on_actionSave_triggered()
{
    if (currentFilePath.isEmpty())
    {
        on_actionSave_As_triggered();
    }
    else
    {
        QFile file(currentFilePath);
        if (file.open(QFile::WriteOnly | QFile::Text))
        {
            QTextStream out(&file);
            out << textEdit->toPlainText();
            file.close();
        }
    }

    isModified = false;
    updateWindowTitle();
}

void MainWindow::on_actionSave_As_triggered()
{
    QString filePath = QFileDialog::getSaveFileName(this, "Save File As", "", "Text Files (*.txt)");

    if (!filePath.isEmpty())
    {
        QFile file(filePath);
        if (file.open(QFile::WriteOnly | QFile::Text))
        {
            QTextStream out(&file);
            out << textEdit->toPlainText();
            file.close();
            currentFilePath = filePath;
        }
    }

    isModified = false;
    updateWindowTitle();
}

void MainWindow::on_actionConvert_to_UTF_8_triggered()
{
    QString text = ui->textEdit->toPlainText();
    QByteArray utf8Data = text.toUtf8();
    QString encoding = "UTF-8";

    ui->textEdit->setPlainText(utf8Data);
    encodingLabel->setText("Encoding: " + encoding);
}

void MainWindow::on_actionConvert_to_ANSI_triggered()
{
    QString text = ui->textEdit->toPlainText();
    QString encoding = "ANSI";
    QByteArray ansiData = text.toLocal8Bit();

    ui->textEdit->setPlainText(ansiData);
    encodingLabel->setText("Encoding: " + encoding);
}

void MainWindow::on_actionAbout_me_triggered()
{
    QDialog aboutDialog(this);
    aboutDialog.setWindowTitle("About");
    QLabel* label = new QLabel(&aboutDialog);
    label->setText("Notepad Developed by Simeon!\n\n"
                   "Release date: 10/11/2023\n"
                   "Current version: 0.2\n");

    QFont font("Cascadia Code", 10, QFont::Bold);
    label->setFont(font);

    aboutDialog.setFixedSize(300, 200);

    aboutDialog.exec();
}

void MainWindow::find()
{
    QString searchString = findLineEdit->text().trimmed();

    if (!findLineEdit || !ui->textEdit)
    {
        qDebug() << "UI elements not properly initialized.";
        return;
    }

    if (searchString.isEmpty())
    {
        ui->textEdit->insertPlainText("Search string is empty.");
        //qDebug() << "Search string is empty.";
        return;
    }

    QTextDocument *document = ui->textEdit->document();

    if (!document || document->isEmpty())
    {
        QMessageBox::warning(this, tr("Find"), tr("No active file open."));
        return;
    }

    if (!searchString.isEmpty())
    {
        QTextCursor cursor = ui->textEdit->textCursor();
        QTextDocument *document = ui->textEdit->document();

        if (!document || document->isEmpty())
        {
            QMessageBox::warning(this, tr("Find"), tr("No active file open."));
            return;
        }

        QTextCharFormat highlightFormat;
        highlightFormat.setBackground(Qt::yellow);

        QTextCursor clearCursor(document);
        clearCursor.select(QTextCursor::Document);
        clearCursor.setCharFormat(QTextCharFormat());

        int count = 0;

        QTextCursor highlightCursor(document);

        // Iterate over each block (paragraph) in the text document
        for (QTextBlock block = document->begin(); block.isValid(); block = block.next())
        {
            QString text = block.text();
            int index = text.indexOf(searchString);

            while (index != -1)
            {
                count++;

                // Move the highlight cursor to the appropriate position
                highlightCursor.setPosition(block.position() + index);
                highlightCursor.setPosition(block.position() + index + searchString.length(), QTextCursor::KeepAnchor);
                highlightCursor.setCharFormat(highlightFormat);

                // Find the next occurrence of the search string in the same block
                index = text.indexOf(searchString, index + searchString.length());
            }
        }

        if (count > 0)
        {
            QMessageBox::information(this, tr("Find"), tr("Occurrences found: %1").arg(count));
        }
        else
        {
            QMessageBox::information(this, tr("Find"), tr("No occurrences found."));
        }
    }
}
void MainWindow::on_actionFind_triggered()
{
    showFindDialog();
}

