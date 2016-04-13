/* Copyright (c) Colorado School of Mines, 2013.*/
/* All rights reserved.                       */


package cseis.seaview;

import cseis.processing.csProcessingAGC;
import cseis.processing.csProcessingFilter;
import cseis.processing.csProcessingDCRemoval;
import cseis.processing.csProcessingInterpolation;
import cseis.seisdisp.csSeisPane;
import java.awt.event.ActionEvent;

import javax.swing.*;

import cseis.seisdisp.csSeisViewPopupMenu;
import java.awt.event.ActionListener;

/**
 * Popup menu for SeaView application.<br>
 * Used to provide functionality in seismic views.
 * @author 2007 Bjorn Olofsson
 */
@SuppressWarnings("serial")
public class csSeaViewPopupMenu extends csSeisViewPopupMenu {
  private csSeisPaneBundle myBundle;
  private final JCheckBoxMenuItem myItemPaintMode;
  private final JCheckBoxMenuItem myItemPickMode;

  csSeaViewPopupMenu( csSeisPaneBundle bundle ) {
    super( bundle.seisView );
    myBundle = bundle;

    JMenu menuZoom = new JMenu("Zoom");
    menuZoom.setIcon(cseis.resources.csResources.getIcon("csZoom.gif"));
    JMenu menuProcessing = new JMenu("Processing");

    removeAll();
    JMenuItem itemAnnotation = new JMenuItem( csSeaViewActions.ACTION_TITLE[csSeaViewActions.SetAnnotationAction], csSeaViewActions.getIcon(csSeaViewActions.SetAnnotationAction) );
    JMenuItem itemOverlay = new JMenuItem( csSeaViewActions.ACTION_TITLE[csSeaViewActions.ShowOverlayAction], csSeaViewActions.getIcon(csSeaViewActions.ShowOverlayAction) );
    JMenuItem itemGraph = new JMenuItem( csSeaViewActions.ACTION_TITLE[csSeaViewActions.ShowGraphAction], csSeaViewActions.getIcon(csSeaViewActions.ShowGraphAction) );
    JMenuItem itemZoomIn = new JMenuItem( csSeaViewActions.ACTION_TITLE[csSeaViewActions.ZoomInAction], csSeaViewActions.getIcon(csSeaViewActions.ZoomInAction) );
    JMenuItem itemZoomInHorz = new JMenuItem( csSeaViewActions.ACTION_TITLE[csSeaViewActions.ZoomInHorzAction], csSeaViewActions.getIcon(csSeaViewActions.ZoomInHorzAction) );
    JMenuItem itemZoomInVert = new JMenuItem( csSeaViewActions.ACTION_TITLE[csSeaViewActions.ZoomInVertAction], csSeaViewActions.getIcon(csSeaViewActions.ZoomInVertAction) );
    JMenuItem itemZoomOut = new JMenuItem( csSeaViewActions.ACTION_TITLE[csSeaViewActions.ZoomOutAction], csSeaViewActions.getIcon(csSeaViewActions.ZoomOutAction) );
    JMenuItem itemZoomOutHorz = new JMenuItem( csSeaViewActions.ACTION_TITLE[csSeaViewActions.ZoomOutHorzAction], csSeaViewActions.getIcon(csSeaViewActions.ZoomOutHorzAction) );
    JMenuItem itemZoomOutVert = new JMenuItem( csSeaViewActions.ACTION_TITLE[csSeaViewActions.ZoomOutVertAction], csSeaViewActions.getIcon(csSeaViewActions.ZoomOutVertAction) );

    JMenuItem itemProcessingClear = new JMenuItem( "Clear" );
    JMenuItem itemProcessingAGC = new JMenuItem( "AGC" );
    JMenuItem itemProcessingInterpolation = new JMenuItem( "Interpolation" );
    JMenuItem itemProcessingFilter = new JMenuItem( "Filter" );
    JMenuItem itemProcessingDCRemoval = new JMenuItem( "DC removal" );

    myItemPaintMode = new JCheckBoxMenuItem( csSeaViewActions.ACTION_TITLE[csSeaViewActions.PaintModeAction], csSeaViewActions.getIcon(csSeaViewActions.PaintModeAction) );
    myItemPickMode  = new JCheckBoxMenuItem( csSeaViewActions.ACTION_TITLE[csSeaViewActions.PickModeAction], csSeaViewActions.getIcon(csSeaViewActions.PickModeAction) );

    itemAnnotation.setToolTipText( csSeaViewActions.ACTION_DESC[csSeaViewActions.SetAnnotationAction] );
    itemOverlay.setToolTipText( csSeaViewActions.ACTION_DESC[csSeaViewActions.ShowOverlayAction] );
    itemGraph.setToolTipText( csSeaViewActions.ACTION_DESC[csSeaViewActions.ShowGraphAction] );
    itemZoomIn.setToolTipText( csSeaViewActions.ACTION_DESC[csSeaViewActions.ZoomInAction] );
    itemZoomInHorz.setToolTipText( csSeaViewActions.ACTION_DESC[csSeaViewActions.ZoomInHorzAction] );
    itemZoomInVert.setToolTipText( csSeaViewActions.ACTION_DESC[csSeaViewActions.ZoomInVertAction] );
    itemZoomOut.setToolTipText( csSeaViewActions.ACTION_DESC[csSeaViewActions.ZoomOutAction] );
    itemZoomOutHorz.setToolTipText( csSeaViewActions.ACTION_DESC[csSeaViewActions.ZoomOutHorzAction] );
    itemZoomOutVert.setToolTipText( csSeaViewActions.ACTION_DESC[csSeaViewActions.ZoomOutVertAction] );

    itemProcessingClear.setToolTipText( "Clear applied processing steps" );
    itemProcessingDCRemoval.setToolTipText( "Remove DC bias" );
    itemProcessingAGC.setToolTipText( "Apply AGC (automatic gain control)" );
    itemProcessingFilter.setToolTipText( "Apply bandpass filter" );
    itemProcessingInterpolation.setToolTipText( "Interpolation (add traces)" );
    
    add( myShowDispSettingsAction );
    add( itemAnnotation );
    addSeparator();
    add( itemOverlay );
    add( itemGraph );
    addSeparator();
    add( menuZoom );
    menuZoom.add( itemZoomIn );
    menuZoom.add( itemZoomOut );
    menuZoom.addSeparator();
    menuZoom.add( itemZoomInHorz );
    menuZoom.add( itemZoomOutHorz );
    menuZoom.addSeparator();
    menuZoom.add( itemZoomInVert );
    menuZoom.add( itemZoomOutVert );
    addSeparator();
    add( menuProcessing );
    menuProcessing.add(itemProcessingClear);
    menuProcessing.addSeparator();
    menuProcessing.add(itemProcessingDCRemoval);
    menuProcessing.add(itemProcessingAGC);
    menuProcessing.add(itemProcessingFilter);
//    menuProcessing.add(itemProcessingInterpolation);
    addSeparator();
    add( myItemPaintMode );
    add( myItemPickMode );
    
    itemAnnotation.addActionListener( new ActionListener() {
      @Override
      public void actionPerformed( ActionEvent e ) {
        myBundle.showAnnotationDialog();
      }
    });
    itemOverlay.addActionListener( new ActionListener() {
      @Override
      public void actionPerformed( ActionEvent e ) {
        myBundle.showOverlayDialog();
      }
    });
    itemGraph.addActionListener( new ActionListener() {
      @Override
      public void actionPerformed( ActionEvent e ) {
        myBundle.showGraphPanel( true );
      }
    });
    itemZoomIn.addActionListener( new ActionListener() {
      @Override
      public void actionPerformed( ActionEvent e ) {
        myBundle.zoom( csSeisPane.ZOOM_IN, csSeisPane.ZOOM_BOTH );
      }
    });
    itemZoomInHorz.addActionListener( new ActionListener() {
      @Override
      public void actionPerformed( ActionEvent e ) {
        myBundle.zoom( csSeisPane.ZOOM_IN, csSeisPane.ZOOM_HORZ );
      }
    });
    itemZoomInVert.addActionListener( new ActionListener() {
      @Override
      public void actionPerformed( ActionEvent e ) {
        myBundle.zoom( csSeisPane.ZOOM_IN, csSeisPane.ZOOM_VERT );
      }
    });
    itemZoomOut.addActionListener( new ActionListener() {
      @Override
      public void actionPerformed( ActionEvent e ) {
        myBundle.zoom( csSeisPane.ZOOM_OUT, csSeisPane.ZOOM_BOTH );
      }
    });
    itemZoomOutHorz.addActionListener( new ActionListener() {
      @Override
      public void actionPerformed( ActionEvent e ) {
        myBundle.zoom( csSeisPane.ZOOM_OUT, csSeisPane.ZOOM_HORZ );
      }
    });
    itemZoomOutVert.addActionListener( new ActionListener() {
      @Override
      public void actionPerformed( ActionEvent e ) {
        myBundle.zoom( csSeisPane.ZOOM_OUT, csSeisPane.ZOOM_VERT );
      }
    });
    itemProcessingClear.addActionListener( new ActionListener() {
      @Override
      public void actionPerformed( ActionEvent e ) {
        myBundle.clearAllProcessingSteps();
      }
    });
    itemProcessingAGC.addActionListener( new ActionListener() {
      @Override
      public void actionPerformed( ActionEvent e ) {
        myBundle.setProcessingStep( csProcessingAGC.NAME );
      }
    });
    itemProcessingFilter.addActionListener( new ActionListener() {
      @Override
      public void actionPerformed( ActionEvent e ) {
        myBundle.setProcessingStep( csProcessingFilter.NAME );
      }
    });
    itemProcessingInterpolation.addActionListener( new ActionListener() {
      @Override
      public void actionPerformed( ActionEvent e ) {
        myBundle.setProcessingStep( csProcessingInterpolation.NAME );
      }
    });
    itemProcessingDCRemoval.addActionListener( new ActionListener() {
      @Override
      public void actionPerformed( ActionEvent e ) {
        myBundle.setProcessingStep( csProcessingDCRemoval.NAME );
      }
    });
    myItemPaintMode.addActionListener( new ActionListener() {
      @Override
      public void actionPerformed( ActionEvent e ) {
        if( myItemPickMode.isSelected() ) {
          myItemPickMode.setSelected(false);
          myBundle.setPickMode( false );
        }
        myBundle.setPaintMode( ((JCheckBoxMenuItem)e.getSource()).isSelected() );
      }
    });
    myItemPickMode.addActionListener( new ActionListener() {
      @Override
      public void actionPerformed( ActionEvent e ) {
        if( myItemPaintMode.isSelected() ) {
          myItemPaintMode.setSelected(false);
          myBundle.setPaintMode( false );
        }
        myBundle.setPickMode( ((JCheckBoxMenuItem)e.getSource()).isSelected() );
      }
    });
  }
  void setPaintMode( boolean doSet ) {
    myItemPaintMode.setSelected( doSet );
  }
  void setPickMode( boolean doSet ) {
    myItemPickMode.setSelected( doSet );
  }
}
