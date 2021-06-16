#include <qtmodels.h>

N::RenderingSequences:: RenderingSequences ( QWidget * parent , Plan * p )
                      : SMAPsEditor        (           parent ,        p )
{
  Configure ( ) ;
}

N::RenderingSequences::~RenderingSequences (void)
{
}

void N::RenderingSequences::Configure(void)
{
  NewTreeWidgetItem ( head           ) ;
  head -> setText   ( 0 , tr("ID"  ) ) ;
  head -> setText   ( 1 , tr("Name") ) ;
  head -> setText   ( 2 , ""         ) ;
  //////////////////////////////////////
  assignHeaderItems ( head           ) ;
}

bool N::RenderingSequences::FocusIn(void)
{
  return true ;
}

void N::RenderingSequences::prepare(void)
{
  NAMEs           Names                                                ;
  TreeWidgetItems Items                                                ;
  //////////////////////////////////////////////////////////////////////
  clear ( )                                                            ;
  //////////////////////////////////////////////////////////////////////
  Names [ SceneObject::AssignColor        ] = tr("Assign color")       ;
  Names [ SceneObject::RenderPoints       ] = tr("Points"      )       ;
  Names [ SceneObject::RenderColors       ] = tr("Colors"      )       ;
  Names [ SceneObject::RenderSegments     ] = tr("Segments"    )       ;
  Names [ SceneObject::RenderCurves       ] = tr("Curves"      )       ;
  Names [ SceneObject::RenderFaces        ] = tr("Faces"       )       ;
  Names [ SceneObject::RenderSmooth       ] = tr("Smooth"      )       ;
  Names [ SceneObject::RenderSurfaces     ] = tr("Surfaces"    )       ;
  Names [ SceneObject::RenderGrids        ] = tr("Grids"       )       ;
  Names [ SceneObject::RenderTessllations ] = tr("Tessllations")       ;
  Names [ SceneObject::RenderBox          ] = tr("Bounding box")       ;
  Names [ SceneObject::RenderTextures     ] = tr("Textures"    )       ;
  Names [ SceneObject::RenderCeramics     ] = tr("Ceramics"    )       ;
  Names [ SceneObject::RenderPictures     ] = tr("Pictures"    )       ;
  Names [ SceneObject::RenderVoxels       ] = tr("Voxels"      )       ;
  Names [ SceneObject::RenderChildren     ] = tr("Children"    )       ;
  //////////////////////////////////////////////////////////////////////
  for ( int i  = SceneObject::AssignColor                              ;
            i <= SceneObject::RenderChildren                           ;
            i ++                                                     ) {
    NewTreeWidgetItem ( it )                                           ;
    it -> setData ( 0 , Qt::UserRole , -1 )                            ;
    it -> setData ( 1 , Qt::UserRole ,  i )                            ;
    it -> setText ( 1 , Names [ i ]       )                            ;
    Items << it                                                        ;
  }                                                                    ;
  //////////////////////////////////////////////////////////////////////
  if ( NotNull(maps) )                                                 {
    UUIDs U = (*maps) . keys ( )                                       ;
    SUID  u                                                            ;
    foreach ( u , U )                                                  {
      SUID v  = (*maps) [ u ]                                          ;
      if ( v <= SceneObject::RenderChildren )                          {
        QTreeWidgetItem * it = Items  [ v ]                            ;
        it -> setData ( 0 , Qt::UserRole , u   )                       ;
        it -> setText ( 0 , QString::number(u) )                       ;
      }                                                                ;
    }                                                                  ;
  }                                                                    ;
  //////////////////////////////////////////////////////////////////////
  addTopLevelItems ( Items )                                           ;
  emit AutoFit ( )                                                     ;
}

void N::RenderingSequences::doubleClicked(QTreeWidgetItem *item,int column)
{
  if ( column != 0 ) return          ;
  QLineEdit * le = NULL              ;
  le = setLineEdit                   (
         item                        ,
         0                           ,
         SIGNAL(editingFinished())   ,
         SLOT  (orderPressed   ()) ) ;
  le -> setFocus(Qt::TabFocusReason) ;
}

void N::RenderingSequences::orderPressed(void)
{
  QLineEdit * name = Casting   ( QLineEdit  , ItemWidget ) ;
  if (IsNull(name))                                        {
    removeOldItem              (                         ) ;
    return                                                 ;
  }                                                        ;
  QString order = name -> text (                         ) ;
  SUID    ksid  = nTreeUuid    ( ItemEditing , 0         ) ;
  SUID    rsid  = nTreeUuid    ( ItemEditing , 1         ) ;
  SUID    tsid  = -1                                       ;
  bool    okay  = false                                    ;
  if ( order . length ( ) <= 0 )                           {
  } else                                                   {
    tsid = order . toULongLong ( &okay                   ) ;
    if ( ! okay ) tsid = -1                                ;
  }                                                        ;
  if ( ksid >= 0 ) (*maps) . remove ( ksid )               ;
  if ( tsid >= 0 )                                         {
    (*maps) [ tsid ] = rsid                                ;
  } else                                                   {
    order = ""                                             ;
  }                                                        ;
  ItemEditing -> setData       ( 0 , Qt::UserRole , tsid ) ;
  ItemEditing -> setText       ( 0 , order               ) ;
  removeOldItem                (                         ) ;
}
