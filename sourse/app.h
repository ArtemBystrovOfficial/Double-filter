#ifndef APP_H
#define APP_H

#include <QApplication>
#include <QMainWindow>
#include <QLabel>
#include <QToolBar>
#include <QWidget>
#include <QListWidget>
#include <QVector>
#include <QPushButton>
#include <QLayout>
#include <QMenuBar>
#include <QAction>
#include <QStatusBar>
#include <QMenu>
#include <QPainter>
#include <QWheelEvent>
#include <QFileDialog>
#include <QMessageBox>
#include <QSplitter>
#include <QMimeData>
#include <algorithm>
#include <string>
#include <QInputDialog>
#include <map>
#include <QImage>
#include <QProgressDialog>
#include <QMap>

class App;

struct Color
{
    Color(const QColor & col) :r(col.red()),g(col.green()),b(col.blue())
    {

    }
    Color()
    {

    }
    int r,g,b;
    bool operator==(const Color & o){
        return o.r == this->r && o.g == this->g && o.b == this->b;
    }
    friend QDataStream &operator<<(QDataStream &out, const Color &rhs){
        out << rhs.r << rhs.g<<rhs.b;
        return out;
    }
    friend QDataStream &operator>>(QDataStream &in, Color &rhs){
        in >> rhs.r >> rhs.g >> rhs.b;
        return in;
    }

};

bool operator<(const Color & col1, const Color & col2);
bool operator>=(const Color & col1, const Color & col2);
bool operator==(const Color & col1, const Color & col2);

struct Filter
{
public:
    QIcon icon;
    std::map <Color,Color> map;
    void setMap(const QMap <Color,Color> & mp)
    {
        map.clear();
        for (auto i = mp.begin(); i != mp.end(); i++)
        {
            Color col;
            int r = i.value().r;
            int g = i.value().g;
            int b = i.value().b;
            col.r = r;
            col.g = g;
            col.b = b;

            map[i.key()] = col;
        }
    }
    QMap <Color,Color> getMap()
    {
        QMap <Color,Color> data;
        for (auto i = map.begin(); i != map.end(); i++)
        {
            Color datak;
            Color datav;

            datak.r = i->first.r;
            datak.g = i->first.g;
            datak.b = i->first.b;

            datav.r = i->second.r;
            datav.g = i->second.g;
            datav.b = i->second.b;

            data[datak] = datav;
        }
        return data;
    }
    QString filterName;
};

struct DragLabel : public QLabel
{
    Q_OBJECT
public:   
    DragLabel()
    {
        setAcceptDrops(true);
        drop_back.load("images/DropArea.png");
    }
    void paintEvent(QPaintEvent * event);
    void dragEnterEvent( QDragEnterEvent* event );
    void dropEvent( QDropEvent* event );
    QImage imageDrop;
private:
    QPixmap drop_back;
    bool isImage(const QString & string);
};

class FilterDialog : public QDialog
{
    Q_OBJECT
public:
    FilterDialog();
    std::map<Color,Color> MakeMapFilter();
    DragLabel * dragLabel_first;
    DragLabel * dragLabel_second;
    QPushButton * accept;
    QPushButton * cancel;
};

class FilterArea : public QListWidget
{
    Q_OBJECT
public:
    FilterArea(App * app): app(app){
        setIconSize(QSize(70,70));
        setSelectionMode(QAbstractItemView::SingleSelection);
        setViewMode(QListView::IconMode);
        setAcceptDrops( false );
    }
public slots:
    void Load();
    void Save();
    void New();
    void Rename();   

    int getIndex(QListWidgetItem * item)
    {
        auto it = bridge_index.find(item);
        if(it != bridge_index.end())
            return it->second;
        return -1;
    }
    Filter * getFilter(int index)
    {
        return &filterList[index];
    }
private:
    QVector <Filter> filterList;
    App * app;
    std::map<QListWidgetItem*,int> bridge_index;
};

class StrategyProcess
{
public:
    virtual QImage MakeEffect(const QImage *,const std::map<Color,Color> * ,App *) = 0;
};

class NearblyPixel : public StrategyProcess
{
public:
    QImage MakeEffect(const QImage *,const std::map<Color,Color> * ,App *) override;
};

class ImageArea;

class ImageAreaState
{
public:

    ImageAreaState()
    {

    }


    virtual void Load(ImageArea* state) = 0;
    virtual void Save(ImageArea * state) = 0;
    virtual void Close(ImageArea * state) = 0;
    virtual void Start(ImageArea * state) = 0;
private:

};

class EmptyAreaState : public ImageAreaState
{
    void Load(ImageArea * state) override;
    void Save(ImageArea * state) override;
    void Close(ImageArea * state) override;
    void Start(ImageArea * state) override;
};

class RawAreaState : public ImageAreaState
{
    void Load(ImageArea * state) override;
    void Save(ImageArea * state) override;
    void Close(ImageArea * state) override;
    void Start(ImageArea * state) override;
};

class CompleteAreaState : public ImageAreaState
{
    void Load(ImageArea * state) override;
    void Save(ImageArea * state) override;
    void Close(ImageArea * state) override;
    void Start(ImageArea * state) override;
};

class ImageArea : public QLabel
{
    Q_OBJECT

    friend class ImageAreaState;
    friend class EmptyAreaState;
    friend class RawAreaState;
    friend class CompleteAreaState;

public:
    ImageArea(App * app): app(app){
       state =  new EmptyAreaState ;
    }
    ~ImageArea()
    {
        if(state)
            delete state;
    }

public slots:

    void Load();
    void Save();
    void Start();
    void Close();

private:

    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);;

    void wheelEvent(QWheelEvent * event);
    void paintEvent(QPaintEvent * event);

    float current_scale = 1;

    bool is_raw = false;

    void SetState( ImageAreaState * state);
    QImage current_image;
    QImage raw_image;
    ImageAreaState * state;
    App * app;
};

class ToolImageArea : public QToolBar
{
public:
    ToolImageArea(App * app) : app(app){}
private:
    App * app;
};

class ToolFilterArea : public QToolBar
{
public:
    ToolFilterArea(App * app) : app(app){}
private:
    App * app;
};

class App : public QMainWindow
{
    Q_OBJECT

    QIcon Open{"images/Open_icon.png"};
    QIcon Save{"images/Save_icon.png"};
    QIcon New{"images/New_icon.png"};
    QIcon Rename{"images/Rename_icon.png"};
    QIcon Close{"images/Close_icon.png"};
    QIcon Start{"images/Start_icon.png"};


    ImageArea * imageArea;
    ToolImageArea * toolImageArea;
    ToolFilterArea * toolFilterArea;
    FilterArea * filterArea;

    void mainSettings();
public:
    QApplication * heart;

    StrategyProcess * mainproc;

    auto * getFilterArea()
    {
        return filterArea;
    }
    App(QApplication * heart = nullptr,QWidget *parent = nullptr);
    ~App();


};


#endif // APP_H
