#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>

#include <QTime>
#include <QList>
#include <QTextStream>
#include <QThread>
#include <random>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // Iniciar o gerador de numero aleatorios
    srand( QTime::currentTime().msec() );

    connect(ui->actionCarregar_Turma,SIGNAL(triggered(bool)),this,SLOT(carregarTurma()));
    connect(ui->btnGerarGrupos,SIGNAL(clicked(bool)),this,SLOT(gerarGrupos()));
    connect(ui->inTamanhoGrupo,SIGNAL(valueChanged(int)),this,SLOT(numeroAlunosAtualizado(int)));
    connect(ui->actionSalvar_Grupo,SIGNAL(triggered(bool)),this,SLOT(salvarGrupos()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::carregarTurma() {

    QString arquivo = QFileDialog::getOpenFileName(this,"Selecionar turma",
                                 QDir::homePath(),// A partir do diretório do usuário
                                 "Arquivo de Turma (*.csv *.txt)");

    if( arquivo.isNull() || arquivo.isEmpty() ) {
        return;
    }

    this->limparArvore();
    carregarListaTurma(arquivo);
}

void MainWindow::carregarListaTurma(QString nomeArquivo) {

    QFile arquivo(nomeArquivo);

    if( !arquivo.open(QIODevice::ReadOnly | QIODevice::Text) ) {
        return;
    }

    ui->listaAlunos->clear();

    while(!arquivo.atEnd()) {
        QString linha = arquivo.readLine();
        ui->listaAlunos->addItem(linha);
    }

    arquivo.close();

    int tamanhoDoGrupo = ui->inTamanhoGrupo->value();
    int numeroAlunos = ui->listaAlunos->count();

    int numeroGrupos = numeroAlunos / tamanhoDoGrupo;
    int sobraAlunos = numeroAlunos % tamanhoDoGrupo;

    ui->lcdNumAlunos->display(numeroAlunos);
    ui->lcdNumGrupos->display(numeroGrupos);
    ui->lcdAlunosExtras->display(sobraAlunos);
}

void MainWindow::numeroAlunosAtualizado(int tamanhoDoGrupo) {
    int numeroAlunos = ui->listaAlunos->count();

    int numeroGrupos = numeroAlunos / tamanhoDoGrupo;
    int sobraAlunos = numeroAlunos % tamanhoDoGrupo;

    ui->lcdNumAlunos->display(numeroAlunos);
    ui->lcdNumGrupos->display(numeroGrupos);
    ui->lcdAlunosExtras->display(sobraAlunos);

}

void MainWindow::gerarGrupos() {
    QMessageBox msg;
    QFont fonte = msg.font();
    fonte.setPointSize(14);
    msg.setFont(fonte);
    msg.setStyleSheet( "QPushButton{border: 2px solid #8f8f91;"
                       "border-radius: 6px;"
                       "min-width: 40px;"
                       "min-height: 30px;"
                       "background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 0, stop: 0 #f6f7fa, stop: 1 #dadbde);"
                       "}"
                       "QPushButton:default{"
                       "border-color: navy;"
                       "}" );

    if( ui->listaAlunos->count() == 0 ) {
        msg.setIcon( QMessageBox::Information );
        msg.setWindowTitle("Sem alunos!");
        msg.setText("Não há alunos listados!");
        msg.setInformativeText("Carregue a turma!");
        msg.exec();
        return;
    }

    limparArvore();

    this->setCursor( Qt::WaitCursor );

    int tamanhoDoGrupo = ui->inTamanhoGrupo->value();
    int numeroAlunos = ui->listaAlunos->count();

    int numeroGrupos = numeroAlunos / tamanhoDoGrupo;
    int sobraAlunos = numeroAlunos % tamanhoDoGrupo;

//    int* qtdAlunosGrupo = new int[numeroGrupos];
    QTreeWidgetItem** items = new QTreeWidgetItem*[numeroGrupos];
    for( int i = 0; i < numeroGrupos; i++ ) {
//        qtdAlunosGrupo[i] = 0;
        items[i] = new QTreeWidgetItem(ui->arvoreGrupos);
        items[i]->setText(0,QString("Grupo %1").arg((i+1)));
    }

    std::random_device rd;   // Seed
    std::mt19937_64 gen(rd()); // Seed for rd (merzenne twister)

/*    std::uniform_int_distribution<int> rng(0, (int)numeroGrupos-1);
    // Sorteando o grupo para cada aluno
    for( int i = 0; i < numeroAlunos-sobraAlunos; i++ ) {
    // Melhor usar qrand()
        int grupoSorteado = rng(gen); // Random
        int grupoSorteado = qrand() % numeroGrupos;

        if( qtdAlunosGrupo[grupoSorteado] >= tamanhoDoGrupo ) {
            i--;
            continue;
        }
        qDebug() << grupoSorteado;

        qtdAlunosGrupo[grupoSorteado]++;
        QTreeWidgetItem* aluno = new QTreeWidgetItem(items[grupoSorteado]);
        aluno->setText(0,ui->listaAlunos->item(i)->text());
        items[grupoSorteado]->addChild( aluno );
    }

    for( int i = numeroAlunos-1; i >= numeroAlunos-sobraAlunos; i-- ) {
        int grupoSorteado = rng(gen);
        if( qtdAlunosGrupo[grupoSorteado] > tamanhoDoGrupo ) {
            i--;
            continue;
        }
        qtdAlunosGrupo[grupoSorteado]++;
        QTreeWidgetItem* aluno = new QTreeWidgetItem(items[grupoSorteado]);
        aluno->setText(0,ui->listaAlunos->item(i)->text());
        items[grupoSorteado]->addChild( aluno );
    }
*/

    std::uniform_int_distribution<int> rng(0, (int)numeroAlunos-1);
    bool* alunoAlocado = new bool[numeroAlunos];
    for( int i = 0; i < numeroAlunos; i++ ) {
        alunoAlocado[i] = false;
    }

    // Sorteando o aluno para cada grupo
    for( int i = 0; i < numeroGrupos; i++ ) {
        for( int x = 0; x < tamanhoDoGrupo; x++ ) {
            int alunoSorteado = rng(gen);
            if( alunoAlocado[alunoSorteado] ) {
                x--;
                continue;
            }
            QTreeWidgetItem* aluno = new QTreeWidgetItem(items[i]);
            aluno->setText(0,ui->listaAlunos->item(alunoSorteado)->text());
            items[i]->addChild( aluno );
            alunoAlocado[alunoSorteado] = true;
        }
    }


    if( sobraAlunos > 0 ) {
        bool* grupoNaoAlocado = new bool[numeroGrupos];
        for( int i = 0; i < numeroGrupos; i++ ) {
            grupoNaoAlocado[i] = true;
        }
        for( int i = 0; i < numeroAlunos; i++ ) {
            if( !alunoAlocado[i] ) {
                int grupoSorteado = rand() % numeroGrupos;
                if( grupoNaoAlocado[grupoSorteado] ) {
                    grupoNaoAlocado[grupoSorteado] = false;
                    QTreeWidgetItem* aluno = new QTreeWidgetItem(items[grupoSorteado]);
                    aluno->setText(0,ui->listaAlunos->item(i)->text());
                    items[grupoSorteado]->addChild(aluno);
                } else {
                    i--;
                }
            }
        }
        delete[] grupoNaoAlocado;
    }
    delete[] alunoAlocado;


    for(int i = 0; i < numeroGrupos; i++) {
        ui->arvoreGrupos->addTopLevelItem(items[i]);
    }

    this->setCursor( Qt::ArrowCursor );
}

void MainWindow::limparArvore() {

    for( int i = 0; i < ui->arvoreGrupos->topLevelItemCount(); i++ ) {
        QTreeWidgetItem* topItem = ui->arvoreGrupos->topLevelItem(i);
        QList<QTreeWidgetItem *> children = topItem->takeChildren();
        for( int x = 0; x < children.size(); x++ ) {
            QTreeWidgetItem* child = children.at(x);
            delete child;
        }
        delete topItem;
    }
    ui->arvoreGrupos->clear();
}

void MainWindow::salvarGrupos() {

    QMessageBox msg;
    QFont fonte = msg.font();
    fonte.setPointSize(14);
    msg.setFont(fonte);
    msg.setStyleSheet( "QPushButton{border: 2px solid #8f8f91;"
                       "border-radius: 6px;"
                       "min-width: 40px;"
                       "min-height: 30px;"
                       "background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 0, stop: 0 #f6f7fa, stop: 1 #dadbde);"
                       "}"
                       "QPushButton:default{"
                       "border-color: navy;"
                       "}" );

    if( ui->arvoreGrupos->topLevelItemCount() == 0 ) {
        msg.setIcon( QMessageBox::Information );
        msg.setWindowTitle("Sem grupos!");
        msg.setText("Não há grupos gerados!");
        msg.setInformativeText("Carregue a turma e gere os grupos!");
        msg.exec();
        return;
    }

    QString nomeArquivo = QFileDialog::getSaveFileName(this,"Salvar arquivo de grupos!",
                                                       QString(),"Arquivo de Grupo (*.csv);;");

    if( nomeArquivo.isEmpty() || nomeArquivo.isNull() ) {
        return;
    }

    QFile arquivo(nomeArquivo);

    if( !arquivo.open(QIODevice::WriteOnly | QIODevice::Text) ) {
        msg.setIcon(QMessageBox::Warning);
        msg.setWindowTitle("Problema ao abrir arquivo!");
        msg.setText("Não foi possível criar o arquivo!");
        msg.setInformativeText( arquivo.errorString() );
        msg.exec();
        return;
    }

    QTextStream ts(&arquivo);

    QLocale sysLocal = QLocale::system();

    QChar separador = ',';
    if( sysLocal.decimalPoint() == ',' ) {
        separador = ';';
    }

    for( int i = 0; i < ui->arvoreGrupos->topLevelItemCount(); i++ ) {
        QTreeWidgetItem* topItem = ui->arvoreGrupos->topLevelItem(i);
        ts << topItem->text(0) << ":" << separador;
        for( int x = 0; x < topItem->childCount(); x++ ) {
            QTreeWidgetItem* child = topItem->child(x);
            ts << child->text(0) << separador;
        }
        ts << '\n';
    }

    arquivo.close();

    msg.setWindowTitle("Grupos salvos!");
    msg.setText("Arquivo de grupos salvo com sucesso!");
    msg.setInformativeText("Tente abrir o arquivo gerado com o Excel.");
    msg.exec();
}
