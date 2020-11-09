#include "configLoader.h"



TiXmlDocument loadConfig(string path){
    TiXmlDocument file(path.c_str());
    file.LoadFile();
    return file;

}

void loadParameters(QWidget* parameterContainer, TiXmlElement* root){
    for (TiXmlElement* node = root->FirstChildElement(); node != NULL; node = node->NextSiblingElement()){
        QGroupBox* container = treeRecursion(node);
        parameterContainer->layout()->addWidget(container);
    }
    QSpacerItem* spacer = new QSpacerItem(0,0, QSizePolicy::Minimum, QSizePolicy::Expanding);
    parameterContainer->layout()->addItem(spacer);


}

QGroupBox* treeRecursion(TiXmlElement* root ){

    if (root == NULL) return NULL;
    QGroupBox* nodeGroupBox = new QGroupBox;
    nodeGroupBox->setObjectName("Node");
    nodeGroupBox->setTitle(root->Value());
    QVBoxLayout* nodeLayout = new QVBoxLayout;
    //NODE ATTRIBUTES
    QWidget* attributePanel = new QWidget;
    QGridLayout* attributeLayout = new QGridLayout;
    int elementCount = 0;
    bool hasAttributes = false;
    for(TiXmlAttribute* attrib = root->FirstAttribute(); attrib != NULL; attrib = attrib->Next()){
        QGroupBox* attribBox = attributeGroupBox(attrib);
        attribBox->setObjectName("Attribute");
        attributeLayout->addWidget(attribBox, elementCount/3, elementCount%3);
        elementCount++;
        hasAttributes = true;
    }
    //WRAP THE ATTRIBUTES TO ALWAYS HAVE A 3 COLUMN GRID
    for (int i=elementCount; i<3; ++i){
        QWidget* spacer = new QWidget();
        spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
        attributeLayout->addWidget(spacer, 0, i);
    }
    attributePanel->setLayout(attributeLayout);
    if (hasAttributes) nodeLayout->addWidget(attributePanel);
    //NODE ELEMENTS
    for ( TiXmlElement* node = root->FirstChildElement(); node != NULL; node = node->NextSiblingElement()){
        QGroupBox* childGroupBox = treeRecursion(node);
        if (childGroupBox != NULL)nodeLayout->addWidget(childGroupBox);
    }
    nodeGroupBox->setLayout(nodeLayout);
    return nodeGroupBox;
}

void setToZero(TiXmlElement* root){
    if (root == NULL) return;
    //NODE ATTRIBUTES
    for(TiXmlAttribute* attrib = root->FirstAttribute(); attrib != NULL; attrib = attrib->Next()){
        if(isNumeric(attrib->Value()))attrib->SetValue("0");
    }
    //NODE ELEMENTS
    for ( TiXmlElement* node = root->FirstChildElement(); node != NULL; node = node->NextSiblingElement()){
        setToZero(node);
    }
}



QGroupBox* attributeGroupBox(TiXmlAttribute* attrib ){
    QGroupBox* attribBox = new QGroupBox;
    attribBox->setTitle(attrib->Name());
    string value = attrib->Value();
    QVBoxLayout* atribLayout = new QVBoxLayout();
    attribBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    atribLayout->setAlignment(Qt::AlignCenter);
    if (isNumeric(value)){
        QSlider* slider = new QSlider(Qt::Horizontal);
        CustomLineEdit* valueLine = new CustomLineEdit();
        slider->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
        slider->setTickPosition(QSlider::TicksBelow);
        wheelEventFilter* wheelFilter = new wheelEventFilter();
        slider->installEventFilter(wheelFilter);
        valueLine->setMaximumSize(60,60);
        valueLine->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
        valueLine->setValidator(new QDoubleValidator());
        QWidget* valueLineAux = new QWidget();
        QHBoxLayout* vLLayout = new QHBoxLayout();
        QLabel* sliderMin = new QLabel();
        QLabel* sliderMax = new QLabel();
        double rValue = atof(value.c_str());
        valueLine->setText(value.c_str());
        sliderMin->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
        sliderMax->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
        sliderMin->setAlignment(Qt::AlignLeft);
        sliderMax->setAlignment(Qt::AlignRight);
        double minValue = powf(10.0f, floorf(log10f(rValue)));
        double maxValue = powf(10.0f, floorf(log10f(rValue)+1));
        if (rValue == 0){
            minValue = 0;
            maxValue = 10;
        }
        valueLine->setMaxValue(maxValue);
        valueLine->setMinValue(minValue);
        slider->setValue((rValue - minValue)/(maxValue - minValue)*100);
        sliderMin->setNum(minValue);
        sliderMax->setNum(maxValue);
        QObject::connect(slider, SIGNAL(valueChanged(int)),valueLine, SLOT(setCustomValue(int)));
        QObject::connect(valueLine, SIGNAL(editingFinished()),valueLine, SLOT(checkValues()));
        QObject::connect(valueLine, SIGNAL(customValueChanged(int)),slider, SLOT(setValue(int)));
        QObject::connect(valueLine, SIGNAL(minValueChanged(double)),sliderMin, SLOT(setNum(double)));
        QObject::connect(valueLine, SIGNAL(maxValueChanged(double)),sliderMax, SLOT(setNum(double)));
        vLLayout->addWidget(sliderMin);
        vLLayout->addWidget(valueLine);
        vLLayout->addWidget(sliderMax);
        vLLayout->setSpacing(0);
        vLLayout->setMargin(0);
        QWidget* incrementWidget = new QWidget();
        QHBoxLayout* incrementLayout = new QHBoxLayout();
        QLineEdit* incrementLine = new QLineEdit();
        QLabel* incrementLabel = new QLabel();
        incrementLabel->setText("Increment:");
        incrementLabel->setAlignment(Qt::AlignRight);
        incrementLabel->setMargin(5);
        incrementLabel->setToolTip("Increment of the variable for each simulation.\nThe default value is 0, meaning that the variable will not change during all the simulations.\nIt has no impact on single run simulations.");
        incrementLine->setMaximumSize(60,60);
        incrementLine->setValidator(new QDoubleValidator());
        incrementLine->setText("0");
        incrementLine->setAlignment(Qt::AlignRight);
        incrementLayout->addWidget(incrementLabel);
        incrementLayout->addWidget(incrementLine);
        incrementLayout->setSpacing(0);
        incrementLayout->setMargin(0);
        incrementWidget->setLayout(incrementLayout);
        incrementWidget->setVisible(false);
        valueLineAux->setLayout(vLLayout);
        atribLayout->addWidget(incrementWidget);
        atribLayout->addWidget(slider);
        atribLayout->addWidget(valueLineAux);
    }
    else{
         QLineEdit* valueLine = new QLineEdit();
         QString s = attrib->Value();
         valueLine->setText(s);
         valueLine->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
         atribLayout->addWidget(valueLine);
    }
    attribBox->setLayout(atribLayout);
    return attribBox;
}



void writeRecursion(TiXmlElement* node, QObject* box){
    TiXmlAttribute* xAttr = node->FirstAttribute();
    TiXmlElement* xElm = node->FirstChildElement();
    for(QObject* o:box->children()){
            if (o->objectName()=="Node"){
                QGroupBox* g = (QGroupBox*) o;
                writeRecursion(xElm, g);
                xElm = xElm ->NextSiblingElement();
            }
            else if(o->objectName()=="Attribute"){
                QGroupBox* a = (QGroupBox*) o;
                QString attributeName = a->title();

                QString attributeValue;
                QList<QObject*> children = a->children();
                if (children.size() == 2){     //String Attribute
                    QLineEdit* valueLine =(QLineEdit*) children[1];
                    attributeValue = valueLine->text();
                    xAttr->SetValue(attributeValue.toStdString().c_str());
                }
                else if(children.size() == 4){     //Numeric Attribute
                    QObject* qo = children[3];
                    QList<QObject*> childrenAux = qo->children();
                    QLineEdit* valueLine =(QLineEdit*) childrenAux[2];
                    attributeValue = valueLine->text();
                    xAttr->SetValue(attributeValue.toStdString().c_str());

                }
                xAttr = xAttr->Next();

            }
            else{
                writeRecursion(node, o);
            }
    }
}

void writeChanges(TiXmlDocument* doc, QWidget* container){
    TiXmlElement* node = doc->RootElement()->FirstChildElement();
    for(QObject* o:container->children()){
            if (o->objectName()=="Node"){
                QGroupBox* g = (QGroupBox*) o;
                writeRecursion(node, g);
                node = node->NextSiblingElement();
            }
    }
}

void writeIncrement(TiXmlDocument* doc, QWidget* container){
    TiXmlElement* node = doc->RootElement()->FirstChildElement();
    for(QObject* o:container->children()){
            if (o->objectName()=="Node"){
                QGroupBox* g = (QGroupBox*) o;
                writeIncrementRecursion(node, g);
                node = node->NextSiblingElement();
            }
    }
}

void writeIncrementRecursion(TiXmlElement* node, QObject* box){
    TiXmlAttribute* xAttr = node->FirstAttribute();
    TiXmlElement* xElm = node->FirstChildElement();
    for(QObject* o:box->children()){
            if (o->objectName()=="Node"){
                QGroupBox* g = (QGroupBox*) o;
                writeIncrementRecursion(xElm, g);
                xElm = xElm ->NextSiblingElement();
            }
            else if(o->objectName()=="Attribute"){
                QGroupBox* a = (QGroupBox*) o;
                QString attributeName = a->title();

                QString attributeValue;
                QList<QObject*> children = a->children();
                if(children.size() == 4){     //Numeric Attribute
                    QObject* qo = children[1];
                    QList<QObject*> childrenAux = qo->children();
                    QLineEdit* valueLine =(QLineEdit*) childrenAux[2];
                    attributeValue = valueLine->text();
                    xAttr->SetValue(attributeValue.toStdString().c_str());

                }
                xAttr = xAttr->Next();

            }
            else{
                writeIncrementRecursion(node, o);
            }
    }
}


void showIncrementWidget(QWidget* container, bool show){
    for(QObject* o:container->children()){
            if (o->objectName()=="Node"){
                QGroupBox* g = (QGroupBox*) o;
                showIncrementRecursion(g, show);
            }
    }
}


void showIncrementRecursion(QObject* box, bool show){
    for(QObject* o:box->children()){
            if (o->objectName()=="Node"){
                QGroupBox* g = (QGroupBox*) o;
                showIncrementRecursion(g, show);
            }
            else if(o->objectName()=="Attribute"){
                QGroupBox* a = (QGroupBox*) o;
                QList<QObject*> children = a->children();
                if(children.size() == 4){     //Numeric Attribute
                    QWidget* incrementWidget = (QWidget*) children[1];
                    incrementWidget->setVisible(show);
                }

            }
            else{
                showIncrementRecursion(o, show);
            }
    }
}

void increaseConfigFile(TiXmlElement* configNode, TiXmlElement* incrementNode){
    TiXmlAttribute* incAttr = incrementNode->FirstAttribute();
    for (TiXmlAttribute* cfgAttr = configNode->FirstAttribute(); cfgAttr != NULL; cfgAttr = cfgAttr->Next()){
       if(isNumeric(cfgAttr->Value())){
            int incValue = atof(cfgAttr->Value()) + atof(incAttr->Value());
            cfgAttr->SetValue(to_string(incValue).c_str());
       }
       incAttr = incAttr->Next();
    }
    TiXmlElement* incNode = incrementNode->FirstChildElement();
    for (TiXmlElement* cfgNode = configNode->FirstChildElement(); cfgNode != NULL; cfgNode = cfgNode->NextSiblingElement()){
        increaseConfigFile(cfgNode, incNode);
        incNode = incNode->NextSiblingElement();
    }
}


int getNumSteps(TiXmlElement* root){
    TiXmlElement* node = root->FirstChildElement("numSteps");
    return atoi(node->Attribute("value"));
};

QString getDirPath(QString filePath){
    int posLastSlash = filePath.lastIndexOf("/");
    QString folder = filePath.left(posLastSlash);
    return folder;
}


QString findExecutable(QString dir){
    QFile file(dir.append("/SConstruct"));
    QString executableName = "executableError"; //value in case the executable name is not found
    if(!file.open(QIODevice::ReadOnly)) {
        executableName = "sconsError";
        QMessageBox::information(0,"Error","SConstruct file for model compilation not found!");
    }
    else{
        QTextStream in(&file);
        QString line;
        bool finished = false;
        QRegularExpression pattern("(\\s)*nameProgram(\\s)*=(\\s)*'(?<name>.*)'(\\s)*");
        while(not finished and not(line = in.readLine()).isNull()){
            QRegularExpressionMatch match = pattern.match(line);
            if (match.hasMatch()){
                executableName = match.captured("name");
                finished = true;
            }
        }
        file.close();
    }
    return executableName;
}

bool fileExists(QString filePath){
    QFileInfo file(filePath);
    if (file.exists() && file.isFile()) return true;
    else return false;
}
