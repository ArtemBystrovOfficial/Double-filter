#include "app.h"


App::App(QApplication * heart,QWidget *parent)
    : QMainWindow(parent),heart(heart)
{    
    mainSettings();
}

App::~App()
{       
    delete mainproc;
}

void App::mainSettings()
{

    mainproc = new NearblyPixel;

    imageArea = new ImageArea{this};
    toolImageArea = new ToolImageArea{this};
    toolFilterArea =  new ToolFilterArea{this};
    filterArea =  new FilterArea{this};

    auto * menuBar = new QMenuBar;

    //\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\

    auto * imageArealayout =  new  QVBoxLayout;
    auto * filtersArealayout =  new  QVBoxLayout;

    auto * imageAreaGroup =  new QWidget;
    auto * filtersAreaGroup =  new QWidget;

    imageAreaGroup->setLayout(imageArealayout);
    filtersAreaGroup->setLayout(filtersArealayout);

    auto * mainLayout =  new QHBoxLayout;

    auto * split = new QSplitter(Qt::Horizontal);
    split->addWidget(imageAreaGroup);
    split->addWidget(filtersAreaGroup);
    split->QSplitter::setStretchFactor(0, 2);
    split->QSplitter::setStretchFactor(1, 1);

    imageArealayout->addWidget(toolImageArea);
    filtersArealayout->addWidget(toolFilterArea);
    imageArealayout->addWidget(imageArea);
    filtersArealayout->addWidget(filterArea);

    mainLayout->addWidget(split);

    this->setCentralWidget(new QWidget);

    //\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\

    auto * menuFile =  new QMenu("&File");
    auto * menuSettings =  new QMenu("&Settings");
    auto * menuHelp =  new QMenu("&Help");

    auto * SavePreset = new QAction("Save filter",0);

    SavePreset->setText("&Save filter");
    SavePreset->setShortcut(QKeySequence("CTRL+S+P"));
    SavePreset->setToolTip("Save current preset");
    SavePreset->setStatusTip("Save current preset");
    connect(SavePreset,SIGNAL(triggered()),filterArea,SLOT(Save()));

    auto * OpenPreset = new QAction("Open filter",0);

    OpenPreset->setText("&Open filter");
    OpenPreset->setShortcut(QKeySequence("CTRL+O+P"));
    OpenPreset->setToolTip("Open your filter from file");
    OpenPreset->setStatusTip("Open your filter from file");
    connect(OpenPreset,SIGNAL(triggered()),filterArea,SLOT(Load()));

    auto * NewPreset = new QAction("New filter",0);

    NewPreset->setText("&New filter");
    NewPreset->setShortcut(QKeySequence("CTRL+N"));
    NewPreset->setToolTip("Make new filter");
    NewPreset->setStatusTip("Make new filter");
    connect(NewPreset,SIGNAL(triggered()),filterArea,SLOT(New()));

    auto * RenameFilter = new QAction("Rename filter",0);

    RenameFilter->setText("&Rename filter");
    RenameFilter->setShortcut(QKeySequence("CTRL+R"));
    RenameFilter->setToolTip("Rename current filter");
    RenameFilter->setStatusTip("Rename current filter");
    connect(RenameFilter,SIGNAL(triggered()),filterArea,SLOT(Rename()));

    auto * ClosePhoto = new QAction("Close image",0);

    ClosePhoto->setText("&Close image");
    ClosePhoto->setShortcut(QKeySequence("CTRL+Q"));
    ClosePhoto->setToolTip("Close current image");
    ClosePhoto->setStatusTip("Close current image");
    connect(ClosePhoto,SIGNAL(triggered()),imageArea,SLOT(Close()));

    auto * OpenPhoto = new QAction("Open image",0);

    OpenPhoto->setText("&Open image");
    OpenPhoto->setShortcut(QKeySequence("CTRL+O"));
    OpenPhoto->setToolTip("Open your image");
    OpenPhoto->setStatusTip("Open your image");
    connect(OpenPhoto,SIGNAL(triggered()),imageArea,SLOT(Load()));

    auto * SavePhoto = new QAction("Save image",0);

    SavePhoto->setText("&Save image");
    SavePhoto->setShortcut(QKeySequence("CTRL+S"));
    SavePhoto->setToolTip("Save your image");
    SavePhoto->setStatusTip("Save your image");
    connect(SavePhoto,SIGNAL(triggered()),imageArea,SLOT(Save()));

    auto * StartImage = new QAction("Start",0);

    StartImage->setText("&Start");
    StartImage->setShortcut(QKeySequence("F5"));
    StartImage->setToolTip("Start rendering with current image");
    StartImage->setStatusTip("Start rendering with current image");
    connect(StartImage,SIGNAL(triggered()),imageArea,SLOT(Start()));

    SavePreset->setIcon(Save);
    OpenPreset->setIcon(Open);
    NewPreset->setIcon(New);
    RenameFilter->setIcon(Rename);
    ClosePhoto->setIcon(Close);
    OpenPhoto->setIcon(Open);
    SavePhoto->setIcon(Save);
    StartImage->setIcon(Start);
    //\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\

    menuFile->addAction(SavePreset);
    menuFile->addAction(OpenPreset);
    menuFile->addSeparator();
    menuFile->addAction(ClosePhoto);
    menuFile->addAction(OpenPhoto);
    menuFile->addAction(SavePhoto);

    toolImageArea->addAction(OpenPhoto);
    toolImageArea->addAction(SavePhoto);
    toolImageArea->addAction(ClosePhoto);
    toolImageArea->addAction(StartImage);

    toolFilterArea->addAction(OpenPreset);
    toolFilterArea->addAction(SavePreset);
    toolFilterArea->addAction(NewPreset);
    toolFilterArea->addAction(RenameFilter);

    //\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\

    menuBar->addMenu(menuFile);
    menuBar->addMenu(menuSettings);
    menuBar->addSeparator();
    menuBar->addMenu(menuHelp);

    this->centralWidget()->setLayout(mainLayout);

    this->setMenuBar(menuBar);

    this->statusBar()->showMessage("Ready");


}
// EMPTYAREASTATE

void EmptyAreaState::Load(ImageArea *area)
{
    auto file = QFileDialog::getOpenFileName(0,"Open Image","Image","*.png ;; *.jpg ;; *.bmp");
    if(!file.isNull())
    {
        area->raw_image.load("");
        area->current_image.load(file);
        area->SetState(new RawAreaState);
    }
}

void EmptyAreaState::Save(ImageArea *area)
{
    QMessageBox::warning(area,"Warning","Empty work Space");
}

void EmptyAreaState::Close(ImageArea *area)
{

}

void EmptyAreaState::Start(ImageArea *area)
{
    QMessageBox::warning(area,"Warning","Load photo and choose filter");
}

// RAWAREASTATE

void RawAreaState::Load(ImageArea *area)
{
    auto file = QFileDialog::getOpenFileName(0,"Open Image","Image","*.png ;; *.jpg ;; *.bmp");
    if(!file.isNull())
    {
        area->raw_image.load("");
        area->current_image.load(file);
    }
}

void RawAreaState::Save(ImageArea *area)
{
    QMessageBox::warning(area,"Warning","Filter not use, raw image");
}

void RawAreaState::Close(ImageArea *area)
{
    area->current_image.load("");
    area->repaint();
    area->SetState(new EmptyAreaState);
}

void RawAreaState::Start(ImageArea *area)
{
    if(area->raw_image.isNull())
    {
        area->raw_image = area->current_image;
    }

    auto vec = area->app->getFilterArea()->selectionModel()->selectedIndexes();
        if(!vec.isEmpty())
        {
            int index = area->app->getFilterArea()->getIndex
                    (area->app->getFilterArea()->itemFromIndex(vec.back()));
                index--;

            area->current_image = area->app->mainproc->MakeEffect(&area->raw_image,
                                                                  &area->app->getFilterArea()->getFilter(index)->map,
                                                                  area->app
                                                                  );

            area->SetState(new CompleteAreaState);
        }
        else
        {
            QMessageBox::warning(area,"Warning","Choose filter");
        }
}

// COMPLETEAREASTATE

void CompleteAreaState::Load(ImageArea *area)
{
    QMessageBox msgBox(area);
    msgBox.setText("The document has been modified.");
    msgBox.setInformativeText("Do you want to save your changes?");
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);
    int ret = msgBox.exec();

    switch(ret)
    {
        case QMessageBox::Save:
            area->Save();
        break;
    }

    auto file = QFileDialog::getOpenFileName(0,"Open Image","Image","*.png ;; *.jpg ;; *.bmp");
    if(!file.isNull())
    {
        area->raw_image.load("");
        area->current_image.load(file);
        area->SetState(new RawAreaState);
    }

}

void CompleteAreaState::Save(ImageArea *area)
{
     auto file = QFileDialog::getSaveFileName(0,"Save Image","Image","*.png ;; *.jpg ;; *.bmp");
     if(!file.isEmpty())
         area->current_image.save(file);
}

void CompleteAreaState::Close(ImageArea *area)
{

    QMessageBox msgBox(area);
    msgBox.setText("The document has been modified.");
    msgBox.setInformativeText("Do you want to save your changes?");
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Save);
    int ret = msgBox.exec();
    switch(ret)
    {
        case QMessageBox::Save:
           area->Save();
        break;

    }
    area->current_image.load("");
    area->repaint();
    area->SetState(new EmptyAreaState);

}

void CompleteAreaState::Start(ImageArea *area)
{
    if(area->raw_image.isNull())
    {
        area->raw_image = area->current_image;
    }

    auto vec = area->app->getFilterArea()->selectionModel()->selectedIndexes();
        if(!vec.isEmpty())
        {
            int index = area->app->getFilterArea()->getIndex
                    (area->app->getFilterArea()->itemFromIndex(vec.back()));
                index--;

                area->current_image = area->app->mainproc->MakeEffect(&area->raw_image,
                                                                      &area->app->getFilterArea()->getFilter(index)->map,
                                                                      area->app
                                                                      );

        }
        else
        {
            QMessageBox::warning(area,"Warning","Choose filter");
        }
}

// IMAGEAREA

void ImageArea::Load()
{
    state->Load(this);
}

void ImageArea::Save()
{
    state->Save(this);
}

void ImageArea::Start()
{
    state->Start(this);
}

void ImageArea::Close()
{
    state->Close(this);
}

void ImageArea::mousePressEvent(QMouseEvent * event)
{
    is_raw = true;
    repaint();
}

void ImageArea::mouseReleaseEvent(QMouseEvent * event)
{
    is_raw = false;
    repaint();
}

void ImageArea::wheelEvent(QWheelEvent *event)
{

  if(event->angleDelta().y()>0)
  {
        current_scale+=0.05;
  }
  else
  {
    if(current_scale-0.05>0)
    {
        current_scale-=0.05;
    }
  }
  repaint();
}

void ImageArea::paintEvent(QPaintEvent *event)
{
    QPainter paint(this);

    auto widthArea = this->width();

    auto heightArea = this->height();

    int widthImg = 0;

    int heightImg = 0;

    QRadialGradient radialGrad(QPointF(widthArea/2, heightArea/2), 1000);
    radialGrad.setColorAt(0, QColor::fromRgb(45,45,45));
    radialGrad.setColorAt(0.3, QColor::fromRgb(30,30,30));
    radialGrad.setColorAt(1, QColor::fromRgb(30,30,30));


    paint.setBrush(QBrush(radialGrad));
    paint.setPen(QColor::fromRgb(248,243,43));

    paint.drawRect(-2,-2,widthArea+2,heightArea+2);

    if(!current_image.isNull())
    {
        QImage img;

        if(!is_raw)
        {
            img = current_image.scaled(current_image.width()*current_scale,
                                            current_image.height()*current_scale,
                                            Qt::IgnoreAspectRatio,
                                            Qt::SmoothTransformation);
        }
        else
        {
            img = raw_image.scaled(raw_image.width()*current_scale,
                                            raw_image.height()*current_scale,
                                            Qt::IgnoreAspectRatio,
                                            Qt::SmoothTransformation);
        }

        widthImg = img.width();

        heightImg = img.height();

        paint.drawImage((widthArea - widthImg)/2,
                       (heightArea - heightImg)/2,
                        img,0,0,
                        widthImg, heightImg);
    }

}

void ImageArea::SetState(ImageAreaState *_state)
{
    if(state)
        delete state;
    state = _state;
}


//FILTERAREA

void FilterArea::Load()
{

    auto file = QFileDialog::getOpenFileName(0,"Open Filter","Filter","*.dfil");

    if(!file.isNull())
    {
        QFile path(file);
        if(path.open(QIODevice::ReadOnly))
        {
           QDataStream stream(&path);
           stream.setVersion(QDataStream::Qt_6_2);

           Filter filter;
           QMap<Color,Color> col;

           stream >> filter.icon
                   >> col
                   >> filter.filterName;

           if(stream.status() != QDataStream::Ok)
           {
               QMessageBox::critical(this,"Error","Error write filter");

           }
           else
           {
               filter.setMap(col);

               filterList.push_back(filter);

               QListWidgetItem * pitem = new QListWidgetItem;
               bridge_index[pitem] = filterList.size();

               if(!filterList.isEmpty())
               {
                   pitem->setIcon(filterList[filterList.size()-1].icon);
                   pitem->setText(filterList[filterList.size()-1].filterName);
                   pitem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
               }
               this->addItem(pitem);
            }
        }

    }

}

void FilterArea::Save()
{
    auto file = QFileDialog::getSaveFileName(0,"Save Filter","Filter","*.dfil");

    auto vec = this->selectionModel()->selectedIndexes();
    if(!file.isNull())
    {
        if(!vec.isEmpty())
        {
            int index = getIndex(this->itemFromIndex(vec.back()));
                index--;
                QFile path(file);
                if(path.open(QIODevice::WriteOnly))
                {
                   QDataStream stream(&path);
                   stream.setVersion(QDataStream::Qt_6_2);
                   stream << filterList[index].icon
                          << filterList[index].getMap()
                          << filterList[index].filterName;
                   if(stream.status() != QDataStream::Ok)
                   {
                       QMessageBox::critical(this,"Error","Error write save filter");
                   }
                }

        }
        else
        {
            QMessageBox::warning(this,"Warning","Filter not selection");
        }
    }
}

void FilterArea::New()
{
   FilterDialog * dialog = new FilterDialog;
   if(dialog->exec() == QDialog::Accepted)
   {
       if(!dialog->dragLabel_first->imageDrop.isNull()
       && !dialog->dragLabel_second->imageDrop.isNull())
       {
           if((dialog->dragLabel_first->imageDrop.width()!=
            dialog->dragLabel_second->imageDrop.width()) ||
              (dialog->dragLabel_first->imageDrop.height()!=
            dialog->dragLabel_second->imageDrop.height())
                   )
           {
              QMessageBox::warning(this,"Warning","Drop two images with same sizes");
           }
           else
           {

           Filter filter;
           filter.map = dialog->MakeMapFilter();

           auto pix = QPixmap::fromImage(dialog->dragLabel_second->imageDrop);

           pix = pix.scaled(70,70,Qt::KeepAspectRatio);

           QIcon icon(pix);

           filter.filterName = "Filter #" + QString::number(filterList.size());          
           filter.icon = icon;
           filterList.push_back(filter);

           QListWidgetItem * pitem = new QListWidgetItem;
           bridge_index[pitem] = filterList.size();


           if(!filterList.isEmpty())
           {
               pitem->setIcon(icon);
               pitem->setText(filterList[filterList.size()-1].filterName);
               pitem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
           }
           this->addItem(pitem);
           }
       }
   }


}

void FilterArea::Rename()
{
   auto vec = this->selectionModel()->selectedIndexes();
   if(!vec.isEmpty())
   {
       auto * item = this->itemFromIndex(vec.back());
       int index = getIndex(item);

       index--;

       bool bOk;
       auto text = QInputDialog::getText(this,
                                         "Rename",
                                         "New tag:",
                                         QLineEdit::Normal,
                                         "",
                                         &bOk
                                         );
       if(bOk)
       {
            filterList[index].filterName = text;
            item->setText(text);
       }
   }


}

// MAKEFILTERDIALOG

FilterDialog::FilterDialog()
{
    this->setWindowTitle("Double Filter");
    dragLabel_first = new DragLabel;
    dragLabel_second = new DragLabel;

    accept = new QPushButton("&Ok");
    cancel = new QPushButton("&Cancel");

    auto * layout =  new QGridLayout;

    auto * label = new QLabel();
    QPixmap px;
    px.load("images/To_icon.png");
    label->setPixmap(px);
    label->setFixedWidth(50);
    layout->addWidget(dragLabel_first,0,0);
    layout->addWidget(dragLabel_second,0,2);
    layout->addWidget(label,0,1);
    layout->addWidget(accept,1,0);
    layout->addWidget(cancel,1,2);

    connect(accept,SIGNAL(clicked()),SLOT(accept()));
    connect(cancel,SIGNAL(clicked()),SLOT(reject()));

    this->setLayout(layout);
    this->setFixedSize(480,270);
}

std::map<Color,Color> FilterDialog::MakeMapFilter()
{
    std::map<Color,Color> mp;

    int filWid = dragLabel_first->width();
    int filHeight = dragLabel_first->height();

    for(int i=0;i<filWid;i++)
    {
        for(int j=0;j<filHeight;j++)
        {
            Color mask;

            auto cola = dragLabel_second->imageDrop.pixelColor(i,j);
            auto colb = dragLabel_first->imageDrop.pixelColor(i,j);

            mask.r = cola.red() - colb.red();
            mask.g = cola.green() - colb.green() ;
            mask.b = cola.blue() - colb.blue();

            mp[dragLabel_first->imageDrop.pixelColor(i,j)] = mask;

            //progress->setValue(++progressId);
            //unblock->processEvents();

        }
    }
    return mp;
}

//DRAGLABEL

bool DragLabel::isImage(const QString &_str)
{
    auto str = _str.toStdString();
    auto it = std::find(str.rbegin(),str.rend(),'.');
    std::string after_point;
    if(it != str.rend())
    {
       std::copy(str.rbegin(),it, std::back_inserter(after_point));
    }
    reverse(after_point.begin(),after_point.end());

    if(after_point== "png" ||
       after_point== "jpg" ||
       after_point== "jpeg" ||
       after_point== "bmp" ) return true;

    return false;

}

void DragLabel::paintEvent(QPaintEvent *event)
{
    QPainter paint(this);

    auto widthArea = width();
    auto heightArea = height();
    auto widthImg = imageDrop.width();
    auto heightImg = imageDrop.height();

    QRect sizeW(-1,-1,this->width()+1,this->height()+1);

    if(imageDrop.isNull())
    {
        paint.setOpacity(0.4);
        paint.drawPixmap(QRect(0,0,widthArea,heightArea),drop_back);
        QPen pen("black");
        pen.setStyle(Qt::PenStyle::DashLine);
        pen.setWidth(3);
        paint.setPen(pen);
        paint.setBrush(QBrush());
        paint.drawRect(10,10,widthArea-20,heightArea-20);

        paint.setPen(QPen("white"));
        paint.setFont(QFont("Helvetica",20,400));
        paint.drawText(70,120,"Drop");

        paint.setOpacity(1);



    }
    else
    {
        QSize size(widthImg,heightImg);
        size.scale(widthArea,heightArea,Qt::KeepAspectRatio);

        auto imageOut = imageDrop.scaled(size);

        paint.setBrush(QBrush(QColor("black")));
        paint.drawRect(sizeW);
        paint.drawImage((widthArea - size.width())/2-1,
                       (heightArea - size.height())/2-1,
                        imageOut,-1,-1,
                        size.width()+1,size.height()+1);
    }
}

void DragLabel::dragEnterEvent(QDragEnterEvent *pe)
{
    if(pe->mimeData()->hasUrls())
    {
        auto list = pe->mimeData()->urls();
        if(list.size()==1)
        {
            QString url = list[0].toString();
            if(isImage(url)){
               pe->acceptProposedAction();
            }
        }
    }
}

void DragLabel::dropEvent(QDropEvent *pe)
{
    auto urls = pe->mimeData()->urls();
    imageDrop.load(urls[0].toLocalFile());
    this->repaint();
}


bool operator<(const Color & col1, const Color & col2)
{
    return col1.r + col1.g + col1.b < col2.r + col2.g + col2.b;
}

bool operator>=(const Color & col1, const Color & col2)
{
    return col1.r + col1.g + col1.b >= col2.r + col2.g + col2.b;
}

bool operator==(const Color & col1, const Color & col2)
{
    return col1.r + col1.g + col1.b == col2.r + col2.g + col2.b;
}

QImage NearblyPixel::MakeEffect(const QImage * image, const std::map<Color,Color> * map, App * app)
{

    QApplication * unblock = app->heart;
    // Rule of 3
    QImage copy(*image);
    //near pixel -> mask

    int progressResult = image->width()*image->height();

    QProgressDialog * progress = new QProgressDialog("Processing the data...",
                                                     "&Cancel",0, progressResult,app
                                                     );

    progress->setWindowModality(Qt::WindowModal);
    progress->setMinimumDuration(0);
    progress->setWindowTitle("Please wait");
    progress->setCancelButton(0);
    progress->setAttribute(Qt::WA_DeleteOnClose);
    progress->setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);

    int progressId=0;

    for(int i = 0 ; i < image->width();i++)
    {

        for(int j = 0 ; j < image->height();j++)
        {
            QColor current_color(image->pixelColor(i,j));

            auto it = map->lower_bound(current_color);

            if(it !=  map->end())
            {
               QColor rgb;

               auto imgRgb = image->pixelColor(i,j);

               int r = imgRgb.red()+it->second.r;
               if(r<=255)
               {
                   if(r>=0)
                     rgb.setRed(r);
                   else
                     rgb.setRed(0);
               }
               else
               {
                   rgb.setRed(255);
               }

               int g = imgRgb.green()+it->second.g;
               if(g<=255)
               {
                   if(g>=0)
                     rgb.setGreen(g);
                   else
                     rgb.setGreen(0);
               }
               else
               {
                   rgb.setGreen(255);
               }

               int b = imgRgb.blue()+it->second.b;
               if(b<=255)
               {
                   if(b>=0)
                     rgb.setBlue(b);
                   else
                     rgb.setBlue(0);
               }
               else
               {
                   rgb.setBlue(255);
               }

               copy.setPixelColor(i,j,rgb);
               }
             progress->setValue(++progressId);
             unblock->processEvents();
        }
    }
    progress->setValue(progressResult);
    delete progress;
    return copy;
}
