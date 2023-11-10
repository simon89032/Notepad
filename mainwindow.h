#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

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
    void updateLineNumber();


    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_actionSave_As_triggered();

    void on_actionConvert_to_UTF_8_triggered();

    void on_actionConvert_to_ANSI_triggered();

    void on_actionAbout_me_triggered();

    void on_actionFind_triggered();

private:
    QTextEdit *textEdit;
    QLabel * encodingLabel;
    QLineEdit* findLineEdit;
    QString currentFilePath;
    bool isModified;
    QDialog *findDialog;
    QPushButton *findButton;

    Ui::MainWindow *ui;
    QAction *newAction;

    void updateMenuItemWithShortcut(QAction *action);
    void updateWindowTitle();
    void textModified();
    void showFindDialog();
    void find();

};
#endif // MAINWINDOW_H
