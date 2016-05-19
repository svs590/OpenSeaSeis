/* Copyright (c) Colorado School of Mines, 2013.*/
/* All rights reserved.                       */


package cseis.seisdisp;

import cseis.seis.csISeismicTraceBuffer;

import java.awt.*;
import java.awt.event.*;
import java.awt.image.*;
import javax.swing.*;

/**
 * An extension of csSeisView that plots two data sets on top of each other
 * For example a seismic image with a velocity field in the background
 */
@SuppressWarnings("serial")
public class csBackgroundSeisView extends csSeisView {
  csSeisView mySeisViewBackground;
  int myCompositeRule;
  float myAlpha;

  public csBackgroundSeisView( JFrame parentFrame ) {
    super( parentFrame );
    mySeisViewBackground = new csSeisView(parentFrame);
    myAlpha = 0.5f;
    csSeisDispSettings ds = new csSeisDispSettings();
    ds.scaleType   = csSeisDispSettings.SCALE_TYPE_RANGE;
    ds.viType      = csSeisDispSettings.VA_TYPE_2DSPLINE;
    ds.isVIDisplay = true;
    ds.showWiggle  = false;
    ds.isNegFill   = false;
    ds.isPosFill   = false;
    mySeisViewBackground.updateDispSettings(ds);
    myCompositeRule = AlphaComposite.SRC_OVER;

    myPopupMenu = new csBackgroundSeisViewPopupMenu( this );

    mySeisViewBackground.addSeisViewListener( new csISeisViewListener() {
      @Override
      public void changedSettings(csSeisDispSettings settings) {
        repaint();
      }
      @Override
      public void vertScrollChanged(int scrollValue) {
      }
      @Override
      public void horzScrollChanged(int scrollValue) {
      }
      @Override
      public void sizeChanged(Dimension size) {
      }
      @Override
      public void traceBufferChanged(csISeismicTraceBuffer traceBuffer) {
      }
    });
  }
  public void setCompositeRule( int compositeRule ) {
    myCompositeRule = compositeRule;
  }
  public void setAlpha( float alpha ) {
    myAlpha = alpha;
  }
  public static Image makeColorTransparent( BufferedImage image, final Color color ) {
    ImageFilter filter = new RGBImageFilter() {
      // Set alpha bytes to opaque
      public int markerRGB = color.getRGB() | 0xFF000000;
      @Override
      public final int filterRGB(int x, int y, int rgb) {
        if ( ( rgb | 0xFF000000 ) == markerRGB ) {
          // Mark alpha bytes as zero (=transparent)
          return 0x00FFFFFF & rgb;
        }
        else {
          return rgb;
        }
      }
    }; 
    ImageProducer ip = new FilteredImageSource( image.getSource(), filter );
    return Toolkit.getDefaultToolkit().createImage(ip);
  }
  public void showBackgroundSettingsDialog() {
    mySeisViewBackground.showDispSettingsDialog();
  }
  @Override
  public void paintComponent( Graphics g ) {
    Graphics2D g2 = (Graphics2D)g;
    Rectangle rectBackground = mySeisViewBackground.getVisibleRect();
    if( rectBackground.width > 0 && rectBackground.height > 0 ) {
      // (1) Paint velocity background to bitmap
      mySeisViewBackground.resetScreenBuffer();
      mySeisViewBackground.myMarginLeftRight = myMarginLeftRight;
      mySeisViewBackground.myIsFullRepaint = true;
      mySeisViewBackground.repaintStep1( false );
      // (2) ...if seismic foreground does not include VI display:
      //     a) Paint seismic foreground (wiggle + fill) to bitmap
      //     b) Make white background of bitmap transparent
      //     c) Paint velocity background bitmap to Graphics object
      //     d) Paint seismic foreground bitmap on top
      if( !mySettings.isVIDisplay ) {
        super.paintComponent( g2 );
        Image imageSeismicWhiteTransparent = csBackgroundSeisView.makeColorTransparent( super.myBitmap, Color.white );
        g2.drawImage( mySeisViewBackground.myBitmap, 0, 0, this );
        g2.setComposite( AlphaComposite.getInstance( myCompositeRule, myAlpha ) );
        g2.drawImage( imageSeismicWhiteTransparent, 0, 0, null );
      }
      // (3) ...if seismic foreground includes VI display:
      //     a) Paint velocity background to Graphics object
      //     b) Paint seismic foreground bitmap on top
      else {
        g2.drawImage( mySeisViewBackground.myBitmap, 0, 0, this );
        g2.setComposite( AlphaComposite.getInstance( myCompositeRule, myAlpha ) );
        super.paintComponent( g2 );
      }
    }
    // (4) If velocity background image does not exist, just draw seismic
    else {
      super.paintComponent( g2 );
    }
  }
  @Override
  public void zoom( float zoomVert, float zoomHorz ) {
    super.zoom(zoomVert, zoomHorz);
    mySeisViewBackground.zoom(zoomVert, zoomHorz);
  }
  @Override
  public void zoom( float zoomVert, float zoomHorz, float traceIndexCentre, float sampleIndexCentre ) {
    super.zoom(zoomVert, zoomHorz, traceIndexCentre, sampleIndexCentre);
    mySeisViewBackground.zoom(zoomVert, zoomHorz, traceIndexCentre, sampleIndexCentre);
  }
  public void updateBackground( csISeismicTraceBuffer buffer, double sampleInt ) {
    mySeisViewBackground.updateTraceBuffer( buffer, sampleInt, true );
  }
  @Override
  protected synchronized void resetViewPositionVert( int scrollValue ) {
    mySeisViewBackground.myIsScrolling = false; // Fudge
    mySeisViewBackground.resetViewPositionVert( scrollValue );
    super.resetViewPositionVert( scrollValue );
  }
  @Override
  protected synchronized void resetViewPositionHorz( int scrollValue ) {
    mySeisViewBackground.myIsScrolling = false; // Fudge
    mySeisViewBackground.resetViewPositionHorz( scrollValue );
    super.resetViewPositionHorz( scrollValue );
  }
  //==============================================================================
  //
  public class csBackgroundSeisViewPopupMenu extends csSeisViewPopupMenu {
    csBackgroundSeisViewPopupMenu( csBackgroundSeisView seisView ) {
      super( seisView );
      removeAll();
      JMenuItem itemBackgroundSettings = new JMenuItem("Display settings (background)...");
      add( myShowDispSettingsAction );
      add( itemBackgroundSettings );
      itemBackgroundSettings.addActionListener( new ActionListener() {
        @Override
        public void actionPerformed( ActionEvent e ) {
          csBackgroundSeisView.this.showBackgroundSettingsDialog();
        }
      });
    }
  }

}
