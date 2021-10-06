#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void carregarTurma();
    void gerarGrupos();
    void numeroAlunosAtualizado(int);
    void salvarGrupos();
private:

    void carregarListaTurma(QString nomeArquivo);
    void limparArvore();

    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
