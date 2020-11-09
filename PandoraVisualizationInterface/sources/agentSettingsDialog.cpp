#include "agentSettingsDialog.h"
#include "ui_agentSettingsDialog.h"


const QString AgentSettingsDialog::colorNames[] = { "Blue",
                                                    "Red",
                                                    "Green",
                                                    "Cyan",
                                                    "Purple",
                                                    "Orange",
                                                    "White",
                                                    "Black" };

AgentSettingsDialog::AgentSettingsDialog(Shape::ShapeSettings &settings, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AgentSettingsDialog)
{   this->settings = settings;
    ui->setupUi(this);
    for (int i = 0; i<numColors; ++i){
        QListWidgetItem *item = new QListWidgetItem(ui->colorList);
        glm::vec3 color = Shape::getColor(Shape::getDefaultColorId(i));
        QPixmap pixmap(100,100);
        pixmap.fill(QColor(color.x*255, color.y*255, color.z*255));
        QIcon colorIcon(pixmap);
        QString name = colorNames[i];
        item->setText(name);
        item->setIcon(colorIcon);
    }
    ui->shapeList->setCurrentRow(settings.shape);
    ui->colorList->setCurrentRow(settings.colorId);

    QObject::connect(ui->shapeList, SIGNAL(clicked(const QModelIndex &)), this, SLOT(shapeSelectionChanged(const QModelIndex &)));
    QObject::connect(ui->colorList, SIGNAL(clicked(const QModelIndex &)), this, SLOT(colorSelectionChanged(const QModelIndex &)));
}

void AgentSettingsDialog::shapeSelectionChanged(const QModelIndex &i){
    settings.shape = i.row();
}
void AgentSettingsDialog::colorSelectionChanged(const QModelIndex &i){
    settings.colorId = i.row();
}

AgentSettingsDialog::~AgentSettingsDialog()
{
    delete ui;
}

void AgentSettingsDialog::on_buttonBox_accepted()
{
    emit settingsChanged(settings);
}

