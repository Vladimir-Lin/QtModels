#include <qtmodels.h>

#if   defined(Q_OS_ANDROID)
#define DISABLE_DESKTOP 1
#elif defined(Q_OS_IOS)
#define DISABLE_DESKTOP 1
#else
#define DISABLE_MOBILE 1
#endif

#define Decide decisions[GuiMainId]

#if !defined(DISABLE_DESKTOP)

#endif

#if !defined(DISABLE_MOBILE)

#endif

//////////////////////////////////////////////////////////////////////////////

N::WorldView:: WorldView          ( QWidget * parent , Plan * p , QString IPC )
             : VcfView            (           parent ,        p               )
             , SceneManager       (                           p               )
             , XmlRpcServerMethod ( IPC . toUtf8 ( ) . constData ( ) , NULL   )
             , destructor         ( NULL                                      )
{
  WidgetClass   ;
  Configure ( ) ;
}

N::WorldView::~WorldView (void)
{
  AutoDeletion ( ) ;
}

void N::WorldView::Configure(void)
{
  setAccessibleName ( "N::WorldView"         )                               ;
  setObjectName     ( "N::WorldView"         )                               ;
  ////////////////////////////////////////////////////////////////////////////
  setFunction       ( VeMenu         , true  )                               ;
  setFunction       ( VeUniverse     , true  )                               ;
  setFunction       ( VeBlockade     , false )                               ;
  setFunction       ( VeWindow       , true  )                               ;
  setFunction       ( VeFullScreen   , false )                               ;
  setProperty       ( "AutoDeletion" , true  )                               ;
  ////////////////////////////////////////////////////////////////////////////
  Intervals    [ 13               ] = 15                                     ;
  Booleans     [ "Alive"          ] = true                                   ;
  Booleans     [ "Stopped"        ] = true                                   ;
  Booleans     [ "Finalize"       ] = false                                  ;
  Booleans     [ "Modified"       ] = false                                  ;
  Booleans     [ "Adaptive"       ] = false                                  ;
  Booleans     [ "Painting"       ] = false                                  ;
  Booleans     [ "Moment"         ] = false                                  ;
  Booleans     [ "Blockade"       ] = false                                  ;
  Properties   [ "Lights"         ] = 0                                      ;
  Properties   [ "Interval"       ] = 31                                     ;
  Properties   [ "MotionInterval" ] = 25                                     ;
  Properties   [ "MenuSwitch"     ] = QString("F12")                         ;
  #define EQI(TID,XID) TID [ XID ] = plan -> TID [ XID ]
  EQI ( Equipments   , Plan::DefaultAudioInput  )                            ;
  EQI ( EquipmentIDs , Plan::DefaultAudioInput  )                            ;
  EQI ( Equipments   , Plan::DefaultAudioOutput )                            ;
  EQI ( EquipmentIDs , Plan::DefaultAudioOutput )                            ;
  EQI ( Equipments   , Plan::DefaultCamera      )                            ;
  EQI ( EquipmentIDs , Plan::DefaultCamera      )                            ;
  #undef  EQI
  StartReflush                      = nTimeNow                               ;
  ////////////////////////////////////////////////////////////////////////////
  setupOpenGL    (                                                         ) ;
  setCacheMode   ( QGraphicsView::CacheNone                                ) ;
  ////////////////////////////////////////////////////////////////////////////
  setWindowTitle ( tr("World view")                                        ) ;
  ////////////////////////////////////////////////////////////////////////////
  decisions . Blank ( GuiMainId             )                                ;
  #if !defined(DISABLE_MOBILE)
  allowGesture = true                                                        ;
  decisions . Blank ( GuiMainId             )                                ;
  grabGesture       ( Qt::TapGesture        )                                ;
  grabGesture       ( Qt::TapAndHoldGesture )                                ;
  grabGesture       ( Qt::PanGesture        )                                ;
  grabGesture       ( Qt::PinchGesture      )                                ;
  grabGesture       ( Qt::SwipeGesture      )                                ;
  #endif
  ////////////////////////////////////////////////////////////////////////////
  setAcceptDrops    ( true                  )                                ;
  setMouseTracking  ( true                  )                                ;
  setFocusPolicy    ( Qt::WheelFocus        )                                ;
  ////////////////////////////////////////////////////////////////////////////
  Enablings [ GL_CULL_FACE      ] = true                                     ;
  Enablings [ GL_BLEND          ] = true                                     ;
  Enablings [ GL_LINE_SMOOTH    ] = true                                     ;
  Enablings [ GL_COLOR_MATERIAL ] = true                                     ;
  Enablings [ GL_LIGHTING       ] = true                                     ;
  ////////////////////////////////////////////////////////////////////////////
  QColor bg                                                                  ;
  bg = palette().color(QPalette::Base)                                       ;
  background = bg                                                            ;
  ////////////////////////////////////////////////////////////////////////////
  QKeySequence kkk ( Properties [ "MenuSwitch" ] . toString ( )            ) ;
  PrivateActions [ "UniverseMenu" ]  = new QAction ( tr("Universe") , this ) ;
  PrivateActions [ "UniverseMenu" ] -> setShortcut ( kkk                   ) ;
  ////////////////////////////////////////////////////////////////////////////
  plan -> setFont ( this               )                                     ;
  setDropFlag     ( DropCamera  , true )                                     ;
  setDropFlag     ( DropAudio   , true )                                     ;
  setDropFlag     ( DropVideo   , true )                                     ;
  setDropFlag     ( DropAlbum   , true )                                     ;
  setDropFlag     ( DropPeople  , true )                                     ;
  setDropFlag     ( DropPicture , true )                                     ;
  ////////////////////////////////////////////////////////////////////////////
  addConnector ( "Commando"                                                  ,
                 Commando                                                    ,
                 SIGNAL ( timeout      ( ) )                                 ,
                 this                                                        ,
                 SLOT   ( GuiDispatch  ( ) )                               ) ;
  addConnector ( "Corporeal"                                                 ,
                 SIGNAL ( makeVisible  ( ) )                                 ,
                 SLOT   ( Corporeal    ( ) )                               ) ;
  addConnector ( "Visualize"                                                 ,
                 SIGNAL ( Visualize    ( ) )                                 ,
                 SLOT   ( RenderUpdate ( ) )                               ) ;
  addConnector ( "Busy"                                                      ,
                 SIGNAL ( OnBusy       ( ) )                                 ,
                 SLOT   ( StartBusy    ( ) )                               ) ;
  addConnector ( "Relax"                                                     ,
                 SIGNAL ( GoRelax      ( ) )                                 ,
                 SLOT   ( StopBusy     ( ) )                               ) ;
  addConnector ( "FullScreen"                                                ,
                 SIGNAL ( ForceFullScreen   ( ) )                            ,
                 SLOT   ( FullScreenForcely ( ) )                          ) ;
  ////////////////////////////////////////////////////////////////////////////
  onlyConnector ( "Commando"   )                                             ;
  onlyConnector ( "Corporeal"  )                                             ;
  onlyConnector ( "Visualize"  )                                             ;
  onlyConnector ( "Busy"       )                                             ;
  onlyConnector ( "Relax"      )                                             ;
  onlyConnector ( "FullScreen" )                                             ;
  StartCommando (              )                                             ;
  ////////////////////////////////////////////////////////////////////////////
  setOptimizationFlag          ( DontAdjustForAntialiasing  , true )         ;
  setRenderHint                ( QPainter::Antialiasing     , true )         ;
  setRenderHint                ( QPainter::TextAntialiasing , true )         ;
  setAttribute                 ( Qt::WA_InputMethodEnabled         )         ;
  viewport ( ) -> setAttribute ( Qt::WA_InputMethodEnabled         )         ;
}

//////////////////////////////////////////////////////////////////////////////

void N::WorldView::AutoDeletion(void)
{
  if ( ! Properties . contains ( "AutoDeletion" )   ) return ;
  if (   Properties [ "AutoDeletion" ] . toBool ( ) ) return ;
  ////////////////////////////////////////////////////////////
  if ( NULL != destructor )                                  {
    delete destructor                                        ;
    destructor = NULL                                        ;
  }                                                          ;
}

QSize N::WorldView::sizeHint (void) const
{
  return SizeSuggestion ( QSize ( 640 , 480 ) ) ;
}

QPointF N::WorldView::toPaper(QPoint p)
{
  int w = screen . widthLength  ( p . x ( ) ) ;
  int h = screen . heightLength ( p . y ( ) ) ;
  QPointF P ( w , h )                         ;
  P /= 10                                     ;
  return P                                    ;
}

QPoint N::WorldView::toScreen(QPointF p)
{
  p    *= 10                              ;
  int w = (int)screen.widthPixels (p.x()) ;
  int h = (int)screen.heightPixels(p.y()) ;
  return QPoint ( w , h )                 ;
}

void N::WorldView::setProperty(QString key,QVariant value)
{
  Properties [ key ] = value ;
}

void N::WorldView::KeepPath(QString key,QString path)
{
  QFileInfo F ( path )               ;
  QString   p = F . absolutePath ( ) ;
  plan -> Variables [ key ] = p      ;
}

QString N::WorldView::GetPath(QString key)
{
  QString p                                      ;
  if ( plan  -> Variables . contains ( key ) )   {
    p = plan -> Variables [ key ] . toString ( ) ;
  } else                                         {
    p = plan -> Temporary ( "" )                 ;
  }                                              ;
  return p                                       ;
}

void N::WorldView::execute(N::XmlRpcValue & params,N::XmlRpcValue & result)
{
  if ( params.size() <= 0 )                                                  {
    result = false                                                           ;
    return                                                                   ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  result = true                                                              ;
}

int N::WorldView::JunctionCount(void)
{
  plan -> Rehabilitate ( )       ;
  return plan->junctions.count() ;
}

int N::WorldView::ConduitCount(void)
{
  if (plan->junctions.count()<=0) return 0       ;
  Conduits conduits                              ;
  plan -> Rehabilitate ( )                       ;
  for (int i=0;i<plan->junctions.count();i++)    {
    Junction * junc                              ;
    junc = plan->junctions[i]                    ;
    if (junc->conduits.count()>0)                {
      Conduit * cds                              ;
      for (int j=0;j<junc->conduits.count();j++) {
        cds = junc->conduits[j]                  ;
        if (!conduits.contains(cds))             {
          conduits << cds                        ;
        }                                        ;
      }                                          ;
    }                                            ;
  }                                              ;
  return conduits . count ( )                    ;
}

void N::WorldView::PostSequence(VarArgs args)
{
  mutex [ 1003 ] . lock   ( ) ;
  GuiSequences << args        ;
  mutex [ 1003 ] . unlock ( ) ;
}

void N::WorldView::RunCommand(VarArgs & arguments)
{
}

void N::WorldView::ExecuteCommands(void)
{
  while ( Booleans [ "Alive" ] )       {
    int ds = 15                        ;
    if ( Intervals . contains ( 13 ) ) {
      ds = Intervals [ 13 ]            ;
    }                                  ;
    if ( Sequences . count ( ) > 0 )   {
      RunCommand ( Sequences [ 0 ] )   ;
      Sequences . takeAt ( 0 )         ;
    }                                  ;
    Time::msleep ( ds )                ;
  }                                    ;
}

void N::WorldView::StartBusy(void)
{
  plan -> StartBusy ( ) ;
}

void N::WorldView::StopBusy(void)
{
  plan -> StopBusy  ( ) ;
}

void N::WorldView::ThreadEvent(void)
{
  qApp -> processEvents ( ) ;
}

void N::WorldView::FullScreenForcely(void)
{
  if ( NULL == parent ( ) ) {
    showFullScreen  ( )     ;
  } else                    {
    emit FullScreen ( )     ;
  }                         ;
}

bool N::WorldView::JoinDestructor(SceneNode * node)
{
  if (IsNull(destructor)) return false ;
  (*destructor) += node                ;
  return true                          ;
}

SUID N::WorldView::RequestUniverse(SUID startUuid)
{
  mutex [ 0 ] . lock   ( )                     ;
  while ( Universes . contains ( startUuid ) ) {
    startUuid++                                ;
  }                                            ;
  mutex [ 0 ] . unlock ( )                     ;
  return startUuid                             ;
}

UUIDs N::WorldView::Multiverse(void)
{
  return Universes . keys ( ) ;
}

N::Universe * N::WorldView::GetUniverse(SUID uuid)
{
  nKickOut ( ! Universes . contains ( uuid ) , NULL ) ;
  return Universes [ uuid ]                           ;
}

N::Universe * N::WorldView::removeUniverse(SUID uuid)
{
  if (!Universes.contains(uuid)) return NULL ;
  Universe * u                               ;
  mutex [ 0 ] .lock   (      )               ;
  u = Universes       [ uuid ]               ;
  Universes  . remove ( uuid )               ;
  rUniverses . remove ( u    )               ;
  mutex [ 0 ]. unlock (      )               ;
  return u                                   ;
}

void N::WorldView::EnableDepth(bool enable)
{
  if ( ! enable )                   {
    QtGL::Disable ( GL_DEPTH_TEST ) ;
    return                          ;
  }                                 ;
  ///////////////////////////////////
  QtGL::Enable    ( GL_DEPTH_TEST ) ;
  QtGL::DepthFunc ( GL_LEQUAL     ) ;
}

int N::WorldView::ObtainLights(void)
{
  if ( Properties [ "Lights" ] . toInt ( ) <= 0 )  {
    GLint iLights = 0                              ;
    QtGL::GetInteger ( GL_MAX_LIGHTS , &iLights  ) ;
    setProperty      ( "Lights" , (int) iLights  ) ;
  }                                                ;
  return Properties  [ "Lights" ] . toInt ( )      ;
}

double N::WorldView::ReflushRate(void)
{
  return (double) FrameRate ;
}

int N::WorldView::addUniverse(SUID uuid,Universe * universe)
{
  mutex          [ 0        ] . lock   ( )                     ;
  universe -> MaxLights  = Properties [ "Lights" ] . toInt ( ) ;
  JoinDestructor ( universe )                                  ;
  Universes      [ uuid     ] = universe                       ;
  rUniverses     [ universe ] = uuid                           ;
  mutex          [ 0        ] . unlock ( )                     ;
  return Universes . count ( )                                 ;
}

bool N::WorldView::MoveUniverse(SUID from,SUID to)
{
  nKickOut ( ! Universes . contains ( from ) , false ) ;
  nKickOut (   Universes . contains ( to   ) , false ) ;
  Universe * u = Universes [ from ]                    ;
  nKickOut ( IsNull(u)                       , false ) ;
  mutex [ 0 ] . lock       (      )                    ;
  Universes   . remove     ( from )                    ;
  Universes                [ to   ] = u                ;
  rUniverses               [ u    ] = to               ;
  mutex [ 0 ] . unlock     (      )                    ;
  return true                                          ;
}

bool N::WorldView::DeleteUniverse(Universe * universe)
{
  bool  b = false                           ;
  SUID  u                                   ;
  mutex [ 0 ] . lock   ( )                  ;
  if ( rUniverses . contains ( universe ) ) {
    u = rUniverses      [ universe ]        ;
    Universes  . remove ( u        )        ;
    rUniverses . remove ( universe )        ;
    if ( NotNull ( destructor ) )           {
      destructor -> Destroy ( universe )    ;
    } else                                  {
      delete universe                       ;
    }                                       ;
    Booleans [ "Modified" ] = true          ;
    b = true                                ;
  }                                         ;
  mutex [ 0 ] . unlock ( )                  ;
  return b                                  ;
}

//////////////////////////////////////////////////////////////////////////////

N::Universe * N::WorldView::CreateWorld(QString name)
{
  Universe * wu                                              ;
//  wu = SceneCreator :: createUniverse ( name , plan , this ) ;
  wu = SceneCreator :: createUniverse ( name , plan , NULL ) ;
  JoinDestructor                      ( wu                 ) ;
  return wu                                                  ;
}

//////////////////////////////////////////////////////////////////////////////

void N::WorldView::timerEvent(QTimerEvent * e)
{
  if ( Booleans [ "Finalize" ]  )                                 {
    e -> accept ( )                                               ;
    return                                                        ;
  }                                                               ;
  /////////////////////////////////////////////////////////////////
  QGraphicsView::timerEvent ( e )                                 ;
  if ( Properties . contains ( "Timer" ) )                        {
    if ( Properties [ "Timer" ] . toInt ( ) == e -> timerId ( ) ) {
      scene ( ) -> update ( )                                     ;
    }                                                             ;
  }                                                               ;
  e -> accept ( )                                                 ;
}

void N::WorldView::resizeEvent(QResizeEvent * e)
{
  if ( Booleans [ "Finalize" ]  )  {
    e -> ignore ( )                ;
    return                         ;
  }                                ;
  //////////////////////////////////
  QGraphicsView::resizeEvent ( e ) ;
  Relocation                 (   ) ;
}

void N::WorldView::showEvent(QShowEvent * e)
{
  if ( Booleans [ "Finalize" ]  )  {
    e -> ignore ( )                ;
    return                         ;
  }                                ;
  //////////////////////////////////
  QGraphicsView :: showEvent ( e ) ;
  Relocation                 (   ) ;
}

void N::WorldView::closeEvent(QCloseEvent * event)
{
  shutdown ( ) ? event -> accept( ) : event -> ignore( ) ;
}

void N::WorldView::contextMenuEvent(QContextMenuEvent * e)
{
  if ( Booleans [ "Finalize" ]  )                {
    e -> ignore ( )                              ;
    return                                       ;
  }                                              ;
  ////////////////////////////////////////////////
  if ( isFunction ( VeMenu ) )                   {
    VcfView::contextMenuEvent ( e )              ;
    if ( ! e -> isAccepted ( ) )                 {
      if ( Menu ( e -> pos ( ) ) ) e->accept ( ) ;
    }                                            ;
  } else                                         {
    VcfView::contextMenuEvent ( e )              ;
  }                                              ;
}

bool N::WorldView::removeDrop(void)
{
  UUIDs U = Universes . keys ( )            ;
  SUID  u                                   ;
  bool  b = false                           ;
  ///////////////////////////////////////////
  while ( U . count ( ) > 0 )               {
    u = U . last ( )                        ;
    U . takeLast ( )                        ;
    mutex [ 0 ] . lock   ( )                ;
    b = false                               ;
    if ( Universes . contains ( u ) )       {
      b = Universes [ u ] -> removeDrop ( ) ;
    }                                       ;
    mutex [ 0 ] . unlock ( )                ;
    if ( b ) return true                    ;
  }                                         ;
  ///////////////////////////////////////////
  return false                              ;
}

bool N::WorldView::dragEnter(QDragEnterEvent * e)
{
  UUIDs U = Universes . keys ( )                                             ;
  SUID  u                                                                    ;
  bool  b                                                                    ;
  ////////////////////////////////////////////////////////////////////////////
  while ( U . count ( ) > 0 )                                                {
    u = U . last ( )                                                         ;
    U . takeLast ( )                                                         ;
    mutex [ 0 ] . lock   ( )                                                 ;
    b = false                                                                ;
    if ( Universes . contains ( u ) )                                        {
      b = Universes [ u ] -> dragEnter ( e )                                 ;
    }                                                                        ;
    mutex [ 0 ] . unlock ( )                                                 ;
    if ( b )                                                                 {
      e -> accept ( )                                                        ;
      return true                                                            ;
    }                                                                        ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  nKickOut(!acceptDrop((QWidget *)e->source(),e->mimeData()         ),false) ;
  nKickOut(!dropNew   ((QWidget *)e->source(),e->mimeData(),e->pos()),false) ;
  return true                                                                ;
}

bool N::WorldView::dragMove(QDragMoveEvent  * e)
{
  UUIDs U = Universes . keys ( )                                             ;
  SUID  u                                                                    ;
  bool  b                                                                    ;
  ////////////////////////////////////////////////////////////////////////////
  while ( U . count ( ) > 0 )                                                {
    u = U . last ( )                                                         ;
    U . takeLast ( )                                                         ;
    mutex [ 0 ] . lock   ( )                                                 ;
    b = false                                                                ;
    if ( Universes . contains ( u ) )                                        {
      b = Universes [ u ] -> dragMove ( e )                                  ;
    }                                                                        ;
    mutex [ 0 ] . unlock ( )                                                 ;
    if ( b )                                                                 {
      e -> accept ( )                                                        ;
      return true                                                            ;
    }                                                                        ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  nKickOut(!acceptDrop ((QWidget*)e->source(),e->mimeData()         ),false) ;
  nKickOut(!dropMoving ((QWidget*)e->source(),e->mimeData(),e->pos()),false) ;
  return true                                                                ;
}

bool N::WorldView::drop(QDropEvent * e)
{
  UUIDs U = Universes . keys ( )                                             ;
  SUID  u                                                                    ;
  bool  b                                                                    ;
  ////////////////////////////////////////////////////////////////////////////
  while ( U . count ( ) > 0 )                                                {
    u = U . last ( )                                                         ;
    U . takeLast ( )                                                         ;
    mutex [ 0 ] . lock   ( )                                                 ;
    b = false                                                                ;
    if ( Universes . contains ( u ) )                                        {
      b = Universes [ u ] -> drop ( e )                                      ;
    }                                                                        ;
    mutex [ 0 ] . unlock ( )                                                 ;
    if ( b )                                                                 {
      e -> accept ( )                                                        ;
      return true                                                            ;
    }                                                                        ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  nKickOut(!acceptDrop ((QWidget*)e->source(),e->mimeData()         ),false) ;
  nKickOut(!dropAppend ((QWidget*)e->source(),e->mimeData(),e->pos()),false) ;
  return true                                                                ;
}

void N::WorldView::dragEnterEvent(QDragEnterEvent * e)
{
  if ( Booleans [ "Finalize" ]  )                             {
    e -> ignore ( )                                           ;
    return                                                    ;
  }                                                           ;
  /////////////////////////////////////////////////////////////
  if ( dragEnter ( e ) ) e -> acceptProposedAction ( ) ; else {
    if (PassDragDrop) VcfView::dragEnterEvent ( e )           ;
    else e -> ignore ( )                                      ;
  }                                                           ;
}

void N::WorldView::dragLeaveEvent(QDragLeaveEvent * e)
{
  if ( Booleans [ "Finalize" ]  )                   {
    e -> ignore ( )                                 ;
    return                                          ;
  }                                                 ;
  ///////////////////////////////////////////////////
  if ( removeDrop ( ) ) e -> accept ( ) ; else      {
    if (PassDragDrop) VcfView::dragLeaveEvent ( e ) ;
    else e -> ignore ( )                            ;
  }                                                 ;
}

void N::WorldView::dragMoveEvent(QDragMoveEvent * e)
{
  if ( Booleans [ "Finalize" ]  )                            {
    e -> ignore ( )                                          ;
    return                                                   ;
  }                                                          ;
  ////////////////////////////////////////////////////////////
  if ( dragMove ( e ) ) e -> acceptProposedAction ( ) ; else {
    if ( PassDragDrop ) VcfView::dragMoveEvent ( e )         ;
    else e -> ignore ( )                                     ;
  }                                                          ;
}

void N::WorldView::dropEvent(QDropEvent * e)
{
  if ( Booleans [ "Finalize" ]  )                        {
    e -> ignore ( )                                      ;
    return                                               ;
  }                                                      ;
  ////////////////////////////////////////////////////////
  if ( drop ( e ) ) e -> acceptProposedAction ( ) ; else {
    if (PassDragDrop) VcfView::dropEvent ( e )           ;
    else e -> ignore ( )                                 ;
  }                                                      ;
}

void N::WorldView::mouseDoubleClickEvent(QMouseEvent * e)
{
  if ( Booleans [ "Finalize" ]  )                                 {
    e -> ignore ( )                                               ;
    return                                                        ;
  }                                                               ;
  /////////////////////////////////////////////////////////////////
  VcfView::mouseDoubleClickEvent ( e )                            ;
  if ( e -> isAccepted ( ) ) return                               ;
  /////////////////////////////////////////////////////////////////
  UUIDs U                                                         ;
  SUID  u                                                         ;
  bool  b                                                         ;
  /////////////////////////////////////////////////////////////////
  U = Universes . keys ( )                                        ;
  while ( U . count ( ) > 0 )                                     {
    u = U . last ( )                                              ;
    U . takeLast ( )                                              ;
    mutex [ 0 ] . lock   ( )                                      ;
    b = false                                                     ;
    if ( Universes . contains ( u ) )                             {
      SUID g                                                      ;
      if ( Universes [ u ] -> isMouseGrabbed ( g ) )              {
        b = Universes [ u ] -> mouseDoubleClickEvent ( this , e ) ;
      }                                                           ;
    }                                                             ;
    mutex [ 0 ] . unlock ( )                                      ;
    if ( b )                                                      {
      e -> accept ( )                                             ;
      return                                                      ;
    }                                                             ;
  }                                                               ;
  /////////////////////////////////////////////////////////////////
  U = Universes . keys ( )                                        ;
  while ( U . count ( ) > 0 )                                     {
    u = U . last ( )                                              ;
    U . takeLast ( )                                              ;
    mutex [ 0 ] . lock   ( )                                      ;
    b = false                                                     ;
    if ( Universes . contains ( u ) )                             {
      b = Universes [ u ] -> mouseDoubleClickEvent ( this , e )   ;
    }                                                             ;
    mutex [ 0 ] . unlock ( )                                      ;
    if ( b )                                                      {
      e -> accept ( )                                             ;
      return                                                      ;
    }                                                             ;
  }                                                               ;
}

void N::WorldView::mouseMoveEvent(QMouseEvent * e)
{
  if ( Booleans [ "Finalize" ]  )                           {
    e -> ignore ( )                                         ;
    return                                                  ;
  }                                                         ;
  ///////////////////////////////////////////////////////////
  VcfView::mouseMoveEvent ( e                             ) ;
  setProperty             ( "LastestMouseMove" , nTimeNow ) ;
  ///////////////////////////////////////////////////////////
  UUIDs U                                                   ;
  SUID  u                                                   ;
  bool  b                                                   ;
  ///////////////////////////////////////////////////////////
  U = Universes . keys ( )                                  ;
  while ( U . count ( ) > 0 )                               {
    u = U . last ( )                                        ;
    U . takeLast ( )                                        ;
    mutex [ 0 ] . lock   ( )                                ;
    b = false                                               ;
    if ( Universes . contains ( u ) )                       {
      SUID g                                                ;
      if ( Universes [ u ] -> hasDrag ( ) )                 {
        b = Universes [ u ] -> doDrag ( )                   ;
      }                                                     ;
    }                                                       ;
    mutex [ 0 ] . unlock ( )                                ;
    if ( b )                                                {
      e -> accept ( )                                       ;
      return                                                ;
    }                                                       ;
  }                                                         ;
  ///////////////////////////////////////////////////////////
  U = Universes . keys ( )                                  ;
  while ( U . count ( ) > 0 )                               {
    u = U . last ( )                                        ;
    U . takeLast ( )                                        ;
    mutex [ 0 ] . lock   ( )                                ;
    b = false                                               ;
    if ( Universes . contains ( u ) )                       {
      SUID g                                                ;
      if ( Universes [ u ] -> isMouseGrabbed ( g ) )        {
        b = Universes [ u ] -> mouseMoveEvent ( this , e )  ;
      }                                                     ;
    }                                                       ;
    mutex [ 0 ] . unlock ( )                                ;
    if ( b )                                                {
      e -> accept ( )                                       ;
      return                                                ;
    }                                                       ;
  }                                                         ;
  ///////////////////////////////////////////////////////////
  U = Universes . keys ( )                                  ;
  while ( U . count ( ) > 0 )                               {
    u = U . last ( )                                        ;
    U . takeLast ( )                                        ;
    mutex [ 0 ] . lock   ( )                                ;
    b = false                                               ;
    if ( Universes . contains ( u ) )                       {
      b = Universes [ u ] -> mouseMoveEvent ( this , e )    ;
    }                                                       ;
    mutex [ 0 ] . unlock ( )                                ;
    if ( b )                                                {
      e -> accept ( )                                       ;
      return                                                ;
    }                                                       ;
  }                                                         ;
}

void N::WorldView::mousePressEvent(QMouseEvent * e)
{
  if ( Booleans [ "Finalize" ]  )                           {
    e -> ignore ( )                                         ;
    return                                                  ;
  }                                                         ;
  ///////////////////////////////////////////////////////////
  VcfView::mousePressEvent ( e )                            ;
  if ( e -> isAccepted ( ) ) return                         ;
  ///////////////////////////////////////////////////////////
  UUIDs U                                                   ;
  SUID  u                                                   ;
  bool  b                                                   ;
  ///////////////////////////////////////////////////////////
  U = Universes . keys ( )                                  ;
  while ( U . count ( ) > 0 )                               {
    u = U . last ( )                                        ;
    U . takeLast ( )                                        ;
    mutex [ 0 ] . lock   ( )                                ;
    b = false                                               ;
    if ( Universes . contains ( u ) )                       {
      SUID g                                                ;
      if ( Universes [ u ] -> isMouseGrabbed ( g ) )        {
        b = Universes [ u ] -> mousePressEvent ( this , e ) ;
      }                                                     ;
    }                                                       ;
    mutex [ 0 ] . unlock ( )                                ;
    if ( b )                                                {
      e -> accept ( )                                       ;
      return                                                ;
    }                                                       ;
  }                                                         ;
  ///////////////////////////////////////////////////////////
  U = Universes . keys ( )                                  ;
  while ( U . count ( ) > 0 )                               {
    u = U . last ( )                                        ;
    U . takeLast ( )                                        ;
    mutex [ 0 ] . lock   ( )                                ;
    b = false                                               ;
    if ( Universes . contains ( u ) )                       {
      b = Universes [ u ] -> mousePressEvent ( this , e )   ;
    }                                                       ;
    mutex [ 0 ] . unlock ( )                                ;
    if ( b )                                                {
      e -> accept ( )                                       ;
      return                                                ;
    }                                                       ;
  }                                                         ;
}

void N::WorldView::mouseReleaseEvent(QMouseEvent * e)
{
  if ( Booleans [ "Finalize" ]  )                             {
    e -> ignore ( )                                           ;
    return                                                    ;
  }                                                           ;
  /////////////////////////////////////////////////////////////
  VcfView::mouseReleaseEvent ( e )                            ;
  /////////////////////////////////////////////////////////////
  UUIDs U                                                     ;
  SUID  u                                                     ;
  bool  b                                                     ;
  /////////////////////////////////////////////////////////////
  U = Universes . keys ( )                                    ;
  while ( U . count ( ) > 0 )                                 {
    u = U . last ( )                                          ;
    U . takeLast ( )                                          ;
    mutex [ 0 ] . lock   ( )                                  ;
    b = false                                                 ;
    if ( Universes . contains ( u ) )                         {
      SUID g                                                  ;
      if ( Universes [ u ] -> isMouseGrabbed ( g ) )          {
        b = Universes [ u ] -> mouseReleaseEvent ( this , e ) ;
      }                                                       ;
    }                                                         ;
    mutex [ 0 ] . unlock ( )                                  ;
    if ( b )                                                  {
      e -> accept ( )                                         ;
      return                                                  ;
    }                                                         ;
  }                                                           ;
  /////////////////////////////////////////////////////////////
  U = Universes . keys ( )                                    ;
  while ( U . count ( ) > 0 )                                 {
    u = U . last ( )                                          ;
    U . takeLast ( )                                          ;
    mutex [ 0 ] . lock   ( )                                  ;
    b = false                                                 ;
    if ( Universes . contains ( u ) )                         {
      b = Universes [ u ] -> mouseReleaseEvent ( this , e )   ;
    }                                                         ;
    mutex [ 0 ] . unlock ( )                                  ;
    if ( b )                                                  {
      e -> accept ( )                                         ;
      return                                                  ;
    }                                                         ;
  }                                                           ;
}

void N::WorldView::wheelEvent(QWheelEvent * e)
{
  if ( Booleans [ "Finalize" ]  )                      {
    e -> ignore ( )                                    ;
    return                                             ;
  }                                                    ;
  //////////////////////////////////////////////////////
  VcfView::wheelEvent ( e )                            ;
  if ( e -> isAccepted ( ) ) return                    ;
  //////////////////////////////////////////////////////
  UUIDs U                                              ;
  SUID  u                                              ;
  bool  b                                              ;
  //////////////////////////////////////////////////////
  U = Universes . keys ( )                             ;
  while ( U . count ( ) > 0 )                          {
    u = U . last ( )                                   ;
    U . takeLast ( )                                   ;
    mutex [ 0 ] . lock   ( )                           ;
    b = false                                          ;
    if ( Universes . contains ( u ) )                  {
      SUID g                                           ;
      if ( Universes [ u ] -> isMouseGrabbed ( g ) )   {
        b = Universes [ u ] -> wheelEvent ( this , e ) ;
      }                                                ;
    }                                                  ;
    mutex [ 0 ] . unlock ( )                           ;
    if ( b )                                           {
      e -> accept ( )                                  ;
      return                                           ;
    }                                                  ;
  }                                                    ;
  //////////////////////////////////////////////////////
  U = Universes . keys ( )                             ;
  while ( U . count ( ) > 0 )                          {
    u = U . last ( )                                   ;
    U . takeLast ( )                                   ;
    mutex [ 0 ] . lock   ( )                           ;
    b = false                                          ;
    if ( Universes . contains ( u ) )                  {
      b = Universes [ u ] -> wheelEvent ( this , e )   ;
    }                                                  ;
    mutex [ 0 ] . unlock ( )                           ;
    if ( b )                                           {
      e -> accept ( )                                  ;
      return                                           ;
    }                                                  ;
  }                                                    ;
}

QVariant N::WorldView::inputMethodQuery(Qt::InputMethodQuery q) const
{
  return VcfView::inputMethodQuery ( q ) ;
}

void N::WorldView::inputMethodEvent(QInputMethodEvent * e)
{
  VcfView::inputMethodEvent ( e ) ;
}

void N::WorldView::keyPressEvent(QKeyEvent * e)
{
  VcfView::keyPressEvent ( e ) ;
}

void N::WorldView::keyReleaseEvent(QKeyEvent * e)
{
  VcfView::keyReleaseEvent ( e ) ;
}

void N::WorldView::enterEvent(QEvent * e)
{
  VcfView::enterEvent ( e ) ;
}

void N::WorldView::leaveEvent(QEvent * e)
{
  VcfView::leaveEvent ( e ) ;
}

bool N::WorldView::acceptTap(QTapGesture * gesture)
{ Q_UNUSED ( gesture ) ;
  return false         ;
}

bool N::WorldView::acceptTapHold(QTapAndHoldGesture * gesture)
{ Q_UNUSED ( gesture ) ;
  return false         ;
}

bool N::WorldView::acceptPan(QPanGesture * gesture)
{ Q_UNUSED ( gesture ) ;
  return false         ;
}

bool N::WorldView::acceptPinch(QPinchGesture * gesture)
{ Q_UNUSED ( gesture ) ;
  return false         ;
}

bool N::WorldView::acceptSwipe(QSwipeGesture * gesture)
{ Q_UNUSED ( gesture ) ;
  return false         ;
}

bool N::WorldView::acceptCustom(QGesture * gesture)
{ Q_UNUSED ( gesture ) ;
  return false         ;
}

//////////////////////////////////////////////////////////////////////////////

bool N::WorldView::acceptDrop(QWidget * source,const QMimeData * mime)
{
  return true ;
}

bool N::WorldView::dropNew(QWidget * source,const QMimeData * mime,QPoint pos)
{
  Alert ( Action ) ;
  return true      ;
}

bool N::WorldView::dropMoving(QWidget * source,const QMimeData * mime,QPoint pos)
{
  return true ;
}

bool N::WorldView::dropAppend(QWidget * source,const QMimeData * mime,QPoint pos)
{
  return dropItems ( source , mime , pos ) ;
}

bool N::WorldView::dropCameras(QWidget * source,QPointF pos,const UUIDs & uuids)
{ Q_UNUSED ( source )                        ;
  return dropObjects ( 40006 , pos , uuids ) ;
}

bool N::WorldView::dropAudios(QWidget * source,QPointF pos,const UUIDs & uuids)
{ Q_UNUSED ( source )                        ;
  return dropObjects ( 40005 , pos , uuids ) ;
}

bool N::WorldView::dropVideos(QWidget * source,QPointF pos,const UUIDs & uuids)
{ Q_UNUSED ( source )                        ;
  return dropObjects ( 40004 , pos , uuids ) ;
}

bool N::WorldView::dropAlbums(QWidget * source,QPointF pos,const UUIDs & uuids)
{ Q_UNUSED ( source )                        ;
  return dropObjects ( 40003 , pos , uuids ) ;
}

bool N::WorldView::dropPeople(QWidget * source,QPointF pos,const UUIDs & uuids)
{ Q_UNUSED ( source )                        ;
  return dropObjects ( 40002 , pos , uuids ) ;
}

bool N::WorldView::dropPictures(QWidget * source,QPointF pos,const UUIDs & uuids)
{ Q_UNUSED ( source )                        ;
  return dropObjects ( 40001 , pos , uuids ) ;
}

bool N::WorldView::dropObjects(int id,QPointF pos,const UUIDs & uuids)
{
  return true ;
}

//////////////////////////////////////////////////////////////////////////////

bool N::WorldView::FocusIn(void)
{
  if ( Booleans [ "Finalize" ]  ) return false                         ;
  //////////////////////////////////////////////////////////////////////
  DisableAllActions (                            )                     ;
  AssignAction      ( Label    , windowTitle ( ) )                     ;
  //////////////////////////////////////////////////////////////////////
  QAction * a = PrivateActions [ "UniverseMenu" ]                      ;
  if ( NotNull ( a ) )                                                 {
    QObject::disconnect ( a , SIGNAL ( triggered ( ) ) , NULL , NULL ) ;
    if ( QObject::connect                                              (
         a    , SIGNAL(triggered())                                    ,
         this , SLOT(SwitchUniverseMenu()) ) )                         {
      a -> setEnabled ( true )                                         ;
      QKeySequence kkk ( Properties [ "MenuSwitch" ] . toString ( )  ) ;
      PrivateActions [ "UniverseMenu" ] -> setShortcut ( kkk         ) ;
    }                                                                  ;
  }                                                                    ;
  //////////////////////////////////////////////////////////////////////
  return true                                                          ;
}

bool N::WorldView::FocusOut(void)
{
  if ( Booleans [ "Finalize" ]  ) return false    ;
  /////////////////////////////////////////////////
  QAction * a = PrivateActions [ "UniverseMenu" ] ;
  if ( NotNull ( a ) )                            {
    QObject::disconnect                           (
      a                                           ,
      SIGNAL ( triggered ( ) )                    ,
      NULL                                        ,
      NULL                                      ) ;
    a -> setEnabled ( false )                     ;
  }                                               ;
  /////////////////////////////////////////////////
  return true                                     ;
}

//////////////////////////////////////////////////////////////////////////////

void N::WorldView::run(void)
{
  while ( Booleans [ "Alive" ] ) {
    Time::msleep(10)             ;
  }                              ;
}

void N::WorldView::run(int type,ThreadData * data)
{
  ItemRunner ( type , data ) ;
}

bool N::WorldView::ItemRunner(int type,ThreadData * data)
{
  switch ( type )                                       {
    case 20001                                          :
      Animation       (                   )             ;
    return true                                         ;
    case 20002                                          :
      ExecuteCommands (                   )             ;
    return true                                         ;
    case 91001                                          :
    return true                                         ;
  }                                                     ;
  return false                                          ;
}

void N::WorldView::GuiDispatch(void)
{
  LaunchCommands ( ) ;
}

void N::WorldView::Dispatch(void)
{
}

void N::WorldView::InstallDecisions(void)
{
  if ( plan  -> Booleans [ "Desktop" ] )         {
    Decide . setCondition ( DesktopGui , true  ) ;
    Decide . setCondition ( PadGui     , false ) ;
    Decide . setCondition ( PhoneGui   , false ) ;
  } else
  if ( plan  -> Booleans [ "Pad"     ] )         {
    Decide . setCondition ( DesktopGui , false ) ;
    Decide . setCondition ( PadGui     , true  ) ;
    Decide . setCondition ( PhoneGui   , false ) ;
  } else
  if ( plan  -> Booleans [ "Phone"   ] )         {
    Decide . setCondition ( DesktopGui , false ) ;
    Decide . setCondition ( PadGui     , false ) ;
    Decide . setCondition ( PhoneGui   , true  ) ;
  }
}

bool N::WorldView::startup(void)
{
  if (IsNull(destructor))                {
    destructor = new SceneDestructor ( ) ;
  }                                      ;
  InstallDecisions      (              ) ;
  setTransform          ( Transform    ) ;
  DefaultZooms          (              ) ;
  plan -> processEvents (              ) ;
  Zoom                  (              ) ;
  start                 (              ) ;
  start                 ( 20001        ) ;
  start                 ( 20002        ) ;
  Estimate              ( 0            ) ;
  return true                            ;
}

bool N::WorldView::shutdown(void)
{
  if ( ! Booleans [ "Alive" ] )      {
    Booleans [ "Finalize" ] = true   ;
    return true                      ;
  }                                  ;
  Booleans [ "Alive"    ] = false    ;
  Booleans [ "Finalize" ] = true     ;
  setEnabled ( false )               ;
  Commando -> stop ( )               ;
  finalize ( 100 )                   ;
  while ( ! Booleans [ "Stopped" ] ) {
    plan -> processEvents (     )    ;
    Time :: skip          ( 200 )    ;
  }                                  ;
  return true                        ;
}

bool N::WorldView::shutup(void)
{
  Booleans [ "Alive" ] = false       ;
  setEnabled ( false )               ;
  finalize   ( 100   )               ;
  while ( ! Booleans [ "Stopped" ] ) {
    plan -> processEvents (     )    ;
    Time :: skip          ( 200 )    ;
  }                                  ;
  return true                        ;
}

bool N::WorldView::startup(int type,SUID uuid)
{
  return false ;
}

//////////////////////////////////////////////////////////////////////////////

bool N::WorldView::Relocation(void)
{
  bool relocated = VcfView :: Relocation ( )     ;
  UUIDs U = Multiverse()                         ;
  Booleans [ "Blockade" ] = true                 ;
  if ( U . count ( ) > 0)                        {
    SUID   u                                     ;
    QRectF VW ( 0 , 0 , width ( ) , height ( ) ) ;
    foreach (u,U)                                {
      mutex     [ 0 ]  . lock   (    )           ;
      if ( Universes . contains( u ) )           {
        Universes [ u ] -> Relocation ( VW )     ;
      }                                          ;
      mutex     [ 0 ]  . unlock (    )           ;
    }                                            ;
  }                                              ;
  Booleans [ "Blockade" ] = false                ;
  return relocated                               ;
}

void N::WorldView::Corporeal(void)
{
  Relocation   ( ) ;
  RenderUpdate ( ) ;
}

void N::WorldView::RenderUpdate(void)
{
  scene ( ) -> update        ( ) ;
  plan      -> processEvents ( ) ;
}

void N::WorldView::Rendering(void)
{
  nDropOut    ( Booleans [ "Blockade" ] ) ;
  nDropOut    ( Booleans [ "Finalize" ] ) ;
  nDropOut    ( Booleans [ "Painting" ] ) ;
  Estimate    ( 2                       ) ;
  Prepare     (                         ) ;
  doUniverses (                         ) ;
  Flush       (                         ) ;
  Estimate    ( 3                       ) ;
}

void N::WorldView::Prepare(void)
{
  EnableDepth        ( true                          ) ;
  //////////////////////////////////////////////////////
  QtGL::Enable       ( Enablings                     ) ;
  QtGL::ShadeModel   ( GL_SMOOTH                     ) ;
  QtGL::LineWidth    ( 1.0                           ) ;
  //////////////////////////////////////////////////////
  QtGL::BlendFunc    ( GL_SRC_ALPHA                    ,
                       GL_ONE_MINUS_SRC_ALPHA        ) ;
  QtGL::MatrixMode   ( GL_PROJECTION                 ) ;
  QtGL::LoadIdentity (                               ) ;
  QtGL::MatrixMode   ( GL_MODELVIEW                  ) ;
  //////////////////////////////////////////////////////
  ControlPoint pc = background                         ;
  QtGL::ClearColor   ( pc.x , pc.y , pc.z , 0.00f    ) ;
  QtGL::Clear        ( GL_COLOR_BUFFER_BIT             |
                       GL_DEPTH_BUFFER_BIT             |
                       GL_STENCIL_BUFFER_BIT         ) ;
  QtGL::Hint         ( GL_LINE_SMOOTH_HINT,GL_NICEST ) ;
}

void N::WorldView::Flush(void)
{
  QtGL::Flush ( ) ;
}

void N::WorldView::doUniverses(void)
{
  UUIDs U = Multiverse ( )            ;
  SUID  u                             ;
  foreach ( u , U )                   {
    mutex [ 0 ] . lock   ( )          ;
    if ( Universes . contains ( u ) ) {
      Universes [ u ] -> Flush  ( )   ;
    }                                 ;
    mutex [ 0 ] . unlock ( )          ;
  }                                   ;
}

void N::WorldView::Motion(void)
{
  nDropOut ( Booleans [ "Finalize" ]    ) ;
  nDropOut ( Universes . count ( ) <= 0 ) ;
  UUIDs U = Multiverse ( )                ;
  SUID  u                                 ;
  foreach ( u , U )                       {
    if ( Universes . contains ( u ) )     {
      mutex [ 0 ] . lock   ( )            ;
      Universe * z = Universes [ u ]      ;
      if ( NotNull ( z ) )                {
        if ( z -> Animation ( ) )         {
          Booleans [ "Modified" ] = true  ;
        }                                 ;
      }                                   ;
      mutex [ 0 ] . unlock ( )            ;
    }                                     ;
  }                                       ;
}

void N::WorldView::Animation(void)
{
  QDateTime Now  = nTimeNow                                    ;
  QDateTime Last = nTimeNow                                    ;
  while ( Booleans [ "Alive" ] )                               {
    int ds = Now.msecsTo(Last)                                 ;
    if ( ds < Properties [ "MotionInterval" ] . toInt ( ) )    {
      ds = Properties    [ "MotionInterval" ] . toInt ( ) - ds ;
      if ( ds > 3 ) Time :: msleep ( ds )                      ;
    }                                                          ;
    Now  = nTimeNow                                            ;
    Last = nTimeNow                                            ;
    Motion ( )                                                 ;
  }                                                            ;
}

void N::WorldView::setBackground(QColor & bgColor)
{
  background = bgColor ;
}

void N::WorldView::assignBackground(void)
{
  QColor c = background.Color()                                 ;
  c = QColorDialog::getColor(c,this,tr("Set background color")) ;
  background = c                                                ;
}

void N::WorldView::Estimate(int step)
{
}

//////////////////////////////////////////////////////////////////////////////

void N::WorldView::DeleteVCF(VcfItem * item)
{
  takeItem ( item ) ;
}

void N::WorldView::RemoveVCF(VcfItem * item)
{
  QVariant v = VoidVariant ( item ) ;
  VarArgs  args                     ;
  args << 60001                     ;
  args << v                         ;
  PostSequence ( args )             ;
}

void N::WorldView::RemoveInterface(VcfInterface * vcf)
{
  if ( NotNull(vcf) )                                    {
    if ( Items . contains ( vcf ) )                      {
      int index                                          ;
      index = Items . indexOf ( vcf )                    ;
      if ( index >= 0 ) Items . removeAt ( index )       ;
    }                                                    ;
    if ( Maps . contains ( vcf ) ) Maps . remove ( vcf ) ;
    vcf -> hide ( )                                      ;
  }                                                      ;
}

QPoint N::WorldView::WidgetVcfPos(QWidget * widget)
{
  QPoint pos                                                  ;
  QRect  bg = widget -> geometry ( )                          ;
  /////////////////////////////////////////////////////////////
  pos . setX ( ( width  ( ) / 2 ) - ( bg . width  ( ) / 2 ) ) ;
  pos . setY ( ( height ( ) / 2 ) - ( bg . height ( ) / 2 ) ) ;
  /////////////////////////////////////////////////////////////
  return pos                                                  ;
}

void N::WorldView::PlaceVIF   (
       QPoint         pos     ,
       QWidget      * widget  ,
       VcfInterface * vif     ,
       double         opacity )
{
  QRect   RX = widget -> geometry ( )                               ;
  QRect   RN ( 0 ,  0 , RX . width ( )     , RX . height ( )      ) ;
  QRect   RF ( 0 ,  0 , RN . width ( ) + 4 , RN . height ( ) + 28 ) ;
  QRect   RC ( 2 , 26 , RN . width ( )     , RN . height ( )      ) ;
  QRect   RH ( 2 ,  2 , RN . width ( )     ,                   24 ) ;
  QPointF GP = pos                                                  ;
  GP    = plan -> toCentimeter ( GP . toPoint ( )      )            ;
  vif -> showHeader = true                                          ;
  vif -> Scaling    = false                                         ;
  vif -> HeaderRect = RH                                            ;
  vif -> Title      = widget -> windowTitle (          )            ;
  vif -> setOptions            ( Options , false       )            ;
  vif -> setZValue             ( 1.00f                 )            ;
  vif -> setOpacity            ( opacity               )            ;
  vif -> setWidget             ( widget , GP , RF , RC )            ;
  vif -> adjustTransform       (                       )            ;
  vif -> AdjustHeader          (                       )            ;
  nConnect ( widget , SIGNAL ( EraseVIF ( VcfInterface * )        ) ,
             this   , SLOT   ( EraseVIF ( VcfInterface * )      ) ) ;
}

void N::WorldView::EraseVIF(VcfInterface * vif)
{
  if ( IsNull ( vif ) ) return     ;
  vif -> Finish            (     ) ;
  RemoveInterface          ( vif ) ;
  scene ( ) -> removeItem  ( vif ) ;
  vif       -> deleteLater (     ) ;
  setFocus  ( Qt::TabFocusReason ) ;
  setCursor ( Qt::ArrowCursor    ) ;
}

//////////////////////////////////////////////////////////////////////////////

bool N::WorldView::Menu(QPoint pos)
{
  return true ;
}

void N::WorldView::DisableMenu(void)
{
  setFunction ( VeUniverse , false ) ;
}

void N::WorldView::SwitchUniverseMenu(void)
{
  setFunction ( VeUniverse , ! isFunction ( VeUniverse ) ) ;
}

bool N::WorldView::UniverseMenu(QPoint pos)
{
  UUIDs U = Universes . keys ( )                  ;
  SUID  u                                         ;
  while ( U . count ( ) > 0 )                     {
    u = U . last ( )                              ;
    U . takeLast ( )                              ;
    if ( Universes [ u ] -> Menu ( this , pos ) ) {
      return true                                 ;
    }                                             ;
  }                                               ;
  return false                                    ;
}

void N::WorldView::VexMenu(QMenu * menu)
{
}

int N::WorldView::MenuBinder   (
      MenuManager & mm         ,
      QAction     * aa         ,
      QVariants   & v          ,
      void        * dispatcher )
{
  return 0 ;
}

void N::WorldView::BindingMenu(SceneObject * s)
{
}

void N::WorldView::BindingMenu(Universe * u)
{
}

int N::WorldView::CallFunctions(void * dispatcher,QVariants & args)
{
  return 0 ;
}

void N::WorldView::CorpFunctions(Universe * u)
{
}

void N::WorldView::CorpFunctions(SceneObject * s)
{
}
