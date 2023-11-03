#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void updateWindowTitle();
    void showFindDialog();
    void open();
    void updateLineNumber();
    void textModified();
    void save();
    void saveAs();
    void newFile();

    void on_actionOpen_triggered();

    void on_actionFind_triggered();

    void on_actionSave_triggered();

    void on_actionSave_as_triggered();

    void on_actionNew_triggered();

    void on_actionConvert_to_UTF_8_triggered();

    void on_actionConvert_to_ANSI_triggered();

    void on_actionAbout_Me_triggered();

private:
    Ui::MainWindow *ui;
    QAction *saveAsAction;
    QAction *saveAction;
    QAction *newAction;
    QAction *openAction;
    QTextEdit *textEdit;
    QLabel * encodingLabel;
    QLineEdit* findLineEdit;
    QString currentFilePath;
    bool isModified;
    QDialog *findDialog;
    QPushButton *findButton;


};
#endif // MAINWINDOW_H
