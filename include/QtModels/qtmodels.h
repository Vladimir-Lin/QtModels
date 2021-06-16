/****************************************************************************
 *                                                                          *
 * Copyright (C) 2001~2016 Neutrino International Inc.                      *
 *                                                                          *
 * Author : Brian Lin <lin.foxman@gmail.com>, Skype: wolfram_lin            *
 *                                                                          *
 ****************************************************************************/

#ifndef QT_MODELS_H
#define QT_MODELS_H

#include <QtVCF>
#include <QtWorld>
#include <DataWidgets>

QT_BEGIN_NAMESPACE

#ifndef QT_STATIC
#    if defined(QT_BUILD_QTMODELS_LIB)
#      define Q_MODELS_EXPORT Q_DECL_EXPORT
#    else
#      define Q_MODELS_EXPORT Q_DECL_IMPORT
#    endif
#else
#    define Q_MODELS_EXPORT
#endif

namespace N
{

class Q_MODELS_EXPORT RenderingSequences ;
class Q_MODELS_EXPORT WorldView          ;

class Q_MODELS_EXPORT RenderingSequences : public SMAPsEditor
{
  Q_OBJECT
  public:

    explicit     RenderingSequences (StandardConstructor) ;
    virtual     ~RenderingSequences (void) ;

  protected:

    virtual void Configure          (void) ;

    virtual bool FocusIn            (void) ;

  private:

  public slots:

    virtual void prepare            (void) ;

  protected slots:

    virtual void doubleClicked      (QTreeWidgetItem *item,int column) ;
    virtual void orderPressed       (void) ;

  private slots:

  signals:

} ;

class Q_MODELS_EXPORT WorldView : public VcfView
                                , public SceneManager
                                , public XmlRpcServerMethod
{
  Q_OBJECT
  public:

    enum                      {
      VeOpenGL     = 100001   ,
      VeMenu       = 100002   ,
      VeUniverse   = 100003   ,
      VeBlockade   = 100004   ,
      VeWindow     = 100005   ,
      VeFullScreen = 100006 } ;

    SceneDestructor * destructor ;

    explicit       WorldView               (StandardConstructor,QString IPC = "World") ;
    virtual       ~WorldView               (void) ;

    virtual QSize  sizeHint                (void) const ;

    virtual void   execute                 (XmlRpcValue & params   ,
                                            XmlRpcValue & result ) ;

    virtual void   setProperty             (QString key,QVariant value) ;

    bool           JoinDestructor          (SceneNode * node) ;

    SUID           RequestUniverse         (SUID startUuid) ;
    UUIDs          Multiverse              (void) ;
    Universe *     GetUniverse             (SUID uuid) ;
    Universe *     removeUniverse          (SUID uuid) ;

    virtual int    ObtainLights            (void) ;
    virtual double ReflushRate             (void) ;

  protected:

    VarArgLists           GuiSequences   ;
    LMAPs                 Booleans       ;
    UMAPs                 Values         ;
    WMAPs                 Properties     ;
    BMAPs                 Enablings      ;
    IMAPs                 Intervals      ;
    IMAPs                 Behaviors      ;
    NAMEs                 Equipments     ;
    RMAPs                 EquipmentIDs   ;
    NAMEs                 ObjectNames    ;
    QDateTime             StartReflush   ;
    QDateTime             FlushTime      ;
    Frequency             FrameRate      ;
    Frequency             RenderCost     ;
    ControlPoint          background     ;
    NamedActions          PrivateActions ;
    QMap<SUID,Universe *> Universes      ;
    QMap<Universe *,SUID> rUniverses     ;
    QMap<QString,AMAPs  > Pointers       ;
    QMap<QString,HMAPs  > OUIDs          ;
    QMap<SUID,QImage   *> ImageDepot     ;
    QMap<QString,UUIDs  > MUIDs          ;
    QMap<QString,void  *> PLANs          ;
    DMAPs                 Volume         ;
    Mutexz                mutex          ;
    QMap<SUID,Camera   *> Cameras        ;
    QMap<Camera *,SUID  > CameraUUIDs    ;

    virtual void   timerEvent              (QTimerEvent       * e) ;
    virtual void   resizeEvent             (QResizeEvent      * e) ;
    virtual void   showEvent               (QShowEvent        * e) ;
    virtual void   closeEvent              (QCloseEvent       * e) ;
    virtual void   contextMenuEvent        (QContextMenuEvent * e) ;
    virtual void   dragEnterEvent          (QDragEnterEvent   * e) ;
    virtual void   dragLeaveEvent          (QDragLeaveEvent   * e) ;
    virtual void   dragMoveEvent           (QDragMoveEvent    * e) ;
    virtual void   dropEvent               (QDropEvent        * e) ;
    virtual void   mouseDoubleClickEvent   (QMouseEvent       * e) ;
    virtual void   mouseMoveEvent          (QMouseEvent       * e) ;
    virtual void   mousePressEvent         (QMouseEvent       * e) ;
    virtual void   mouseReleaseEvent       (QMouseEvent       * e) ;
    virtual void   wheelEvent              (QWheelEvent       * e) ;
    virtual void   inputMethodEvent        (QInputMethodEvent * e) ;
    virtual void   keyPressEvent           (QKeyEvent         * e) ;
    virtual void   keyReleaseEvent         (QKeyEvent         * e) ;
    virtual void   enterEvent              (QEvent            * e) ;
    virtual void   leaveEvent              (QEvent            * e) ;

    virtual QVariant inputMethodQuery      (Qt::InputMethodQuery query) const ;

    virtual bool   acceptTap               (QTapGesture        * gesture) ;
    virtual bool   acceptTapHold           (QTapAndHoldGesture * gesture) ;
    virtual bool   acceptPan               (QPanGesture        * gesture) ;
    virtual bool   acceptPinch             (QPinchGesture      * gesture) ;
    virtual bool   acceptSwipe             (QSwipeGesture      * gesture) ;
    virtual bool   acceptCustom            (QGesture           * gesture) ;

    virtual bool   removeDrop              (void) ;
    virtual bool   dragEnter               (QDragEnterEvent * event) ;
    virtual bool   dragMove                (QDragMoveEvent  * event) ;
    virtual bool   drop                    (QDropEvent      * event) ;

    virtual bool   acceptDrop              (nDeclWidget,const QMimeData * mime);
    virtual bool   dropNew                 (nDeclWidget,const QMimeData * mime,QPoint pos);
    virtual bool   dropMoving              (nDeclWidget,const QMimeData * mime,QPoint pos);
    virtual bool   dropAppend              (nDeclWidget,const QMimeData * mime,QPoint pos);

    virtual bool   dropCameras             (nDeclWidget,QPointF pos,const UUIDs & Uuids) ;
    virtual bool   dropAudios              (nDeclWidget,QPointF pos,const UUIDs & Uuids) ;
    virtual bool   dropVideos              (nDeclWidget,QPointF pos,const UUIDs & Uuids) ;
    virtual bool   dropAlbums              (nDeclWidget,QPointF pos,const UUIDs & Uuids) ;
    virtual bool   dropPeople              (nDeclWidget,QPointF pos,const UUIDs & Uuids) ;
    virtual bool   dropPictures            (nDeclWidget,QPointF pos,const UUIDs & Uuids) ;

    virtual bool   dropObjects             (int id,QPointF pos,const UUIDs & uuids) ;

    virtual void   ThreadEvent             (void) ;

    virtual void   Configure               (void) ;
    virtual void   AutoDeletion            (void) ;

    virtual bool   FocusIn                 (void) ;
    virtual bool   FocusOut                (void) ;

    virtual bool   Relocation              (void) ;

    virtual void   EnableDepth             (bool enable) ;

    virtual bool   ItemRunner              (int type,ThreadData * data) ;
    virtual void   run                     (int Type,ThreadData * Data) ;
    virtual void   run                     (void) ;

    virtual void    KeepPath               (QString key,QString path) ;
    virtual QString GetPath                (QString key) ;

    virtual QPoint WidgetVcfPos            (QWidget * widget) ;

    virtual QPointF toPaper                (QPoint  p) ;
    virtual QPoint  toScreen               (QPointF p) ;

    virtual int    JunctionCount           (void) ;
    virtual int    ConduitCount            (void) ;

    virtual Universe * CreateWorld         (QString name) ;

  private:

  public slots:

    virtual void   InstallDecisions        (void) ;

    virtual bool   startup                 (int type,SUID uuid) ;
    virtual bool   startup                 (void) ;
    virtual bool   shutdown                (void) ;
    virtual bool   shutup                  (void) ;

    virtual void   Dispatch                (void) ;

    virtual void   RenderUpdate            (void) ;
    virtual void   Rendering               (void) ;
    virtual void   Animation               (void) ;
    virtual void   Prepare                 (void) ;
    virtual void   doUniverses             (void) ;
    virtual void   Flush                   (void) ;
    virtual void   Motion                  (void) ;
    virtual void   Corporeal               (void) ;
    virtual void   Estimate                (int step) ;

    void           setBackground           (QColor & bgColor) ;
    void           assignBackground        (void) ;

    int            addUniverse             (SUID uuid,Universe * universe) ;
    bool           MoveUniverse            (SUID from,SUID to) ;
    virtual bool   DeleteUniverse          (Universe * universe) ;

    virtual void   ExecuteCommands         (void) ;
    virtual void   PostSequence            (VarArgs args) ;
    virtual void   RunCommand              (VarArgs & arguments) ;

    virtual void   VexMenu                 (QMenu * menu) ;
    virtual int    MenuBinder              (MenuManager & mm           ,
                                            QAction     * action       ,
                                            QVariants   & v            ,
                                            void        * dispatcher ) ;
    virtual void   BindingMenu             (SceneObject * node) ;
    virtual void   BindingMenu             (Universe    * node) ;

    virtual int    CallFunctions           (void * dispatcher,QVariants & values) ;
    virtual void   CorpFunctions           (Universe * universe) ;
    virtual void   CorpFunctions           (SceneObject * scene) ;

  protected slots:

    virtual bool   Menu                    (QPoint pos) ;

    virtual void   DisableMenu             (void) ;
    virtual void   SwitchUniverseMenu      (void) ;
    virtual bool   UniverseMenu            (QPoint pos) ;

    virtual void   GuiDispatch             (void) ;

    virtual void   StartBusy               (void) ;
    virtual void   StopBusy                (void) ;

    virtual void   DeleteVCF               (VcfItem * item) ;
    virtual void   RemoveVCF               (VcfItem * item) ;
    virtual void   RemoveInterface         (VcfInterface * item) ;
    virtual void   PlaceVIF                (QPoint         pos       ,
                                            QWidget      * widget    ,
                                            VcfInterface * vif       ,
                                            double         opacity ) ;
    virtual void   EraseVIF                (VcfInterface * vif) ;

  private slots:

    virtual void   FullScreenForcely       (void) ;

  signals:

    void           OnBusy                  (void) ;
    void           GoRelax                 (void) ;
    void           makeVisible             (void) ;
    void           Visualize               (void) ;
    void           ForceFullScreen         (void) ;
    void           FullScreen              (void) ;
    void           Normal                  (void) ;

} ;

}

QT_END_NAMESPACE

#endif
