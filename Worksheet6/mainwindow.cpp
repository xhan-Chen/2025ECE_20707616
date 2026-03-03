#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QPushButton>
#include "QFileDialog.h"
#include "dialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->treeView->addAction(ui->actionOpen_File);
    ui->treeView->addAction(ui->actionItemOptions);
    ui->treeView->setContextMenuPolicy(Qt::ActionsContextMenu);


    /*Create / allocate the ModelList */
    this->partList =new ModelPartList("PartsList");
    /*Link it to the treeview in the GUI */
    ui->treeView->setModel(this->partList);
    /*Manually create a model tree -there amuch better andmore flexible ways of doing this ,e.g. with nested functions. This is just a quick example as a starting point. */
    ModelPart *rootItem= this->partList->getRootItem();
    /*Add 3 top level items */
    for (int i = 0; i < 3; i++) {
        /*Create strings for both data columns */
        QString name=QString("TopLevel %1").arg(i);
        QString visible("true");
        /*Create child item */
        ModelPart *childItem=new ModelPart({ name, visible });
        /*Append to tree top-level */
        rootItem->appendChild(childItem);
        /*Add 5 sub-items */
        for (int j = 0; j < 5; j++) {
            QString name=QString("Item%1,%2").arg(i).arg(j);
            QString visible("true");
            ModelPart *childChildItem=new ModelPart({ name, visible });
            /*Append to parent */
            childItem->appendChild(childChildItem);
        }
    }

    //connect(ui->actionItemOptions, &QAction::triggered, this, &MainWindow::on_actionItemOptions_triggered);

    connect( ui->pushButton , &QPushButton :: released , this , &MainWindow :: handleButton );
    connect( ui->pushButton_2 , &QPushButton :: released , this , &MainWindow :: handleButton );

    connect( this , &MainWindow:: statusUpdateMessage , ui->statusBar , &QStatusBar :: showMessage );

    connect(ui->treeView, &QTreeView::clicked, this, &MainWindow::handleTreeClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow:: handleButton() {
    /*
    QMessageBox msgBox;
    msgBox. setText("Add button was clicked") ;
    msgBox. exec () ;
*/
    emit statusUpdateMessage( QString("Add button was clicked") , 0 );
}
void MainWindow:: handleTreeClicked() {
    /* Get the index of the selected item */
    QModelIndex index = ui->treeView->currentIndex () ;
    /* Get a pointer to the item from the index */
    ModelPart *selectedPart = static_cast<ModelPart*>(index . internalPointer ()) ;
    /* In this case , we will retrieve the name string from the internal QVariant data array */
    QString text = selectedPart->data(0).toString () ;
    emit statusUpdateMessage(QString("The selected item is : ")+text , 0) ;
}

void MainWindow::on_actionOpen_File_triggered()
{
    emit statusUpdateMessage( QString("Open File action triggered") , 0 );

    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Open File"),
        "C:\\",
        tr("STL Files (*.stl);;Text Files (*.txt)")
        );


    if (!fileName.isEmpty()) {

        emit statusUpdateMessage(QString("Selected file: ") + fileName, 0);

        QModelIndex index = ui->treeView->currentIndex();
        if (index.isValid()) {
            ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());
            selectedPart->set(0, QFileInfo(fileName).fileName());
            emit ui->treeView->dataChanged(index, index);
        }
    }
}

void MainWindow::on_actionItemOptions_triggered() {
    QModelIndex index = ui->treeView->currentIndex();
    if (!index.isValid()) {
        emit statusUpdateMessage("Warning: No item selected", 0);
        return;
    }

    ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());
    Dialog dialog(this);

    dialog.setName(selectedPart->data(0).toString());
    dialog.setRGB(selectedPart->getColourR(), selectedPart->getColourG(), selectedPart->getColourB());
    dialog.setVisible(selectedPart->visible());

    QString openMsg = QString("Opening options for item: %1").arg(selectedPart->data(0).toString());
    emit statusUpdateMessage(openMsg, 0);

    if (dialog.exec() == QDialog::Accepted) {
        selectedPart->set(0, dialog.getName());

        int r, g, b;
        dialog.getRGB(r, g, b);
        selectedPart->setColour(r, g, b);
        selectedPart->setVisible(dialog.getVisible());

        emit ui->treeView->model()->dataChanged(index, index);

        emit statusUpdateMessage("Changes saved successfully.", 0);
    } else {
        emit statusUpdateMessage("Edit cancelled by user.", 0);
    }
}
