#include "BxMainWindow.h"
#include <QtWidgets>


BxMainWindow::BxMainWindow() :
    QMainWindow()
{
    // create UI elements
    createToolBox(); // populates mainToolBox with widgets
    createActions();
    createMenus();
    createToolBars();

    // scene and view setup
    scene = new BxLevelScene();
    scene->setSceneRect(-4000, -4000, 8000, 8000);
    scene->setBackgroundBrush(Qt::gray);
    view = new BxLevelView(scene);
    view->centerOn(0,0);

    connect(scene, SIGNAL(selectionChanged()), this, SLOT(selectionChanged()));
    connect(scene, SIGNAL(itemInserted(QGraphicsItem*)), this, SLOT(itemInserted(QGraphicsItem*)));

    attributeEditor = new QWidget();
    attributeEditor->setMinimumWidth(200);
    populateAttributeEditor();

    QVBoxLayout* attributePane = new QVBoxLayout();
    attributePane->setAlignment(Qt::AlignTop);
    QLabel* label = new QLabel("Attribute Editor");
    attributePane->addWidget(label);

    attributePane->addWidget(attributeEditor);

    //layout setup
    QHBoxLayout* layout = new QHBoxLayout;
    layout->addWidget(mainToolBox);
    layout->addWidget(view);
    layout->addLayout(attributePane);
//    layout->addWidget(attributeEditor);

    QWidget* mainWidget = new QWidget;
    mainWidget->setLayout(layout);

    setCentralWidget(mainWidget);
    setWindowTitle("Brix");
}

void BxMainWindow::createToolBox()
{
    // hook up button group click signals, so that main window can handle functionality
    actorButtonGroup = new QButtonGroup(this); // widgets need pointer to parent QObject
    actorButtonGroup->setExclusive(false); // not bullet-point mode
    connect(actorButtonGroup, SIGNAL(buttonClicked(QAbstractButton*)),
            this, SLOT(actorButtonGroupClick(QAbstractButton *)));


    // build layout of widgets
    QGridLayout* actorLayout = new QGridLayout;
    actorLayout->setAlignment(Qt::AlignTop);
    QPushButton* button1 = new QPushButton("Add Actor");
    button1->setCheckable(true);
    actorButtonGroup->addButton(button1);
    actorLayout->addWidget(button1);

    // add layout to king widget
    QWidget* actorWidget = new QWidget;
    actorWidget->setLayout(actorLayout);

    mainToolBox = new QToolBox;
    mainToolBox->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Ignored));
    mainToolBox->setMinimumWidth(actorWidget->sizeHint().width());
    mainToolBox->addItem(actorWidget, tr("Create Actors"));

}

void BxMainWindow::createActions() // actions connect UI to functions
{
    aboutAction = new QAction(tr("Info"),this);
    aboutAction->setShortcut(tr("Ctrl+T"));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(aboutMessage()));
}

void BxMainWindow::createMenus()
{
    aboutMenu = menuBar()->addMenu(tr("&Help"));
    aboutMenu->addAction(aboutAction);
}


void BxMainWindow::createToolBars()
{
    aboutToolBar = addToolBar(tr("Edit"));
    aboutToolBar->addAction(aboutAction);

//    toolButtonAbout = new QToolButton;

//    connect(toolButtonAbout, SIGNAL(clicked()), this, SLOT(aboutMessage());

}


void BxMainWindow::populateAttributeEditor()
{
    delete attributeEditor->layout();
    QVBoxLayout* attributeLayout = new QVBoxLayout();
    attributeLayout->setAlignment(Qt::AlignTop);

    if((scene) && !(scene->items().empty()))
    {
        BxActorItem* item = qgraphicsitem_cast<BxActorItem*>(scene->selectedItems().first());
       //DiagramItem *startItem = qgraphicsitem_cast<DiagramItem *>(startItems.first());

        if(item)
        {
            QWidget* test = item->getControls();
            if(test)
                attributeLayout->addWidget(test);
        }
    }
    attributeEditor->setLayout(attributeLayout);
}


void BxMainWindow::selectionChanged()
{
    QLayoutItem* item;
    while (( item = attributeEditor->layout()->takeAt(0)) != NULL )
    {
        delete item->widget();
        delete item;
    }

    if(!(scene->selectedItems().empty()))
    {
        populateAttributeEditor();
    }
}

void BxMainWindow::itemInserted(QGraphicsItem *in)
{
    QList<QAbstractButton *> buttons = actorButtonGroup->buttons();
    foreach (QAbstractButton *button, buttons)
    {
            button->setChecked(false);
    }
}

void BxMainWindow::actorButtonGroupClick(QAbstractButton* button)
{
    scene->setMode(BxLevelScene::insertItem);
}

void BxMainWindow::aboutMessage()
{
    QMessageBox::about(this,tr("About Brix"),tr("Brix Editor v0.01"));
}