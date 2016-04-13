/* Copyright (c) Colorado School of Mines, 2013.*/
/* All rights reserved.                       */


package cseis.seisdisp;

import cseis.seis.csISeismicTraceBuffer;
import java.awt.BasicStroke;
import java.awt.Graphics2D;
import java.awt.Point;
import java.awt.RenderingHints;
import java.awt.event.KeyEvent;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;
import java.awt.geom.Ellipse2D;
import java.awt.geom.GeneralPath;
import java.util.ArrayList;
import javax.swing.SwingUtilities;

/**
 */
public class csPickOverlay implements csISeisOverlay, MouseMotionListener, MouseListener, csIKeyListener {
  public static int MODE_PEAK   = 0;
  public static int MODE_TROUGH = 1;
  public static int MODE_NONE   = 2;
  public static final String TEXT_NONE     = "none";
  public static final String TEXT_PEAK     = "peak";
  public static final String TEXT_TROUGH   = "trough";
  public static final String[] MODE_TEXT_FIELDS = {
    TEXT_PEAK,
    TEXT_TROUGH,
    TEXT_NONE
  };
  public static final float NO_VALUE = -1.0f;
  private final csSeisView mySeisView;
  private Float myPicks[];
  private final ArrayList<Float[]> myPickList;
  private final ArrayList<csPickAttr> myPickAttrList;
  private int myActiveHorizonIndex;
  private boolean myDoInterpolate;
  private int myPrevPickTrace;

  public csPickOverlay( csSeisView seisView ) {
    mySeisView  = seisView;
    myPickList = new ArrayList();
    myPickAttrList = new ArrayList();
    myPicks     = null; //new Float[mySeisView.getTraceBuffer().numTraces()];
    myActiveHorizonIndex = -1;
    myDoInterpolate = true;
    myPrevPickTrace = -1;
  }
  public int getNumTraces() {
    return mySeisView.getTraceBuffer().numTraces();
  }
  public void activate() {
    mySeisView.addMouseListener( this );
    mySeisView.addMouseMotionListener( this );
    mySeisView.getEventHandler().addKeyListener( this );
    if( myPicks == null ) {
      myPicks = new Float[mySeisView.getTraceBuffer().numTraces()];
      for( int i = 0; i < myPicks.length; i++ ) {
        myPicks[i] = csPickOverlay.NO_VALUE;
      }
    }
  }
  public void deactivate() {
    mySeisView.removeMouseListener( this );
    mySeisView.removeMouseMotionListener( this );
  }
  public void updateActive( int id ) {
    for( int i = 0; i < myPickAttrList.size(); i++ ) {
      if( myPickAttrList.get(i).getID() == id ) {
        myActiveHorizonIndex = i;
        return;
      }
    }
  }
  public Float[] getPicks( int id ) {
    for( int i = 0; i < myPickAttrList.size(); i++ ) {
      if( myPickAttrList.get(i).getID() == id ) {
        return myPickList.get(i);
      }
    }
    return null;
  }
  public void setActivePicks( Float[] picks ) {
    if( myActiveHorizonIndex < 0 || myActiveHorizonIndex >= myPickList.size() ) return;
    myPickList.set( myActiveHorizonIndex, picks );
  }
  public double getSampleInt() {
    return mySeisView.getSampleInt();
  }
  public void removeHorizon( int id ) {
    for( int i = 0; i < myPickAttrList.size(); i++ ) {
      if( myPickAttrList.get(i).getID() == id ) {
        myPickAttrList.remove( i );
        myPickList.remove( i );
        if( myActiveHorizonIndex > i ) {
          myActiveHorizonIndex -= 1;
        }
        if( myActiveHorizonIndex >= myPickAttrList.size() ) {
          myActiveHorizonIndex = myPickAttrList.size()-1;
        }
      }
    }
  }
  public void addHorizon( csPickAttr attr_in ) {
    for( csPickAttr attr : myPickAttrList ) {
      if( attr.getID() == attr_in.getID() ) {
        update( attr_in );
        return;
      }
    }
    myPickAttrList.add( attr_in );
    Float[] picks = new Float[mySeisView.getTraceBuffer().numTraces()];
    for( int i = 0; i < picks.length; i++ ) {
      picks[i] = csPickOverlay.NO_VALUE;
    }
    myPickList.add( picks );
  }
  public void update( csPickAttr attr_in ) {
    for( int i = 0; i < myPickAttrList.size(); i++ ) {
      if( myPickAttrList.get(i).getID() == attr_in.getID() ) {
        myPickAttrList.set( i, attr_in );
        mySeisView.repaint();
        break;
      }
    }
  }
  @Override
  public void draw( csSeisView seisview, Graphics2D g ) {
    if( seisview.getTraceBuffer() == null ) return;
    RenderingHints rhints_save = g.getRenderingHints();
    g.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);

    g.setStroke( new BasicStroke(2.0f) );
    for( int ihor = 0; ihor < myPickList.size(); ihor++ ) {
      csPickAttr attr = myPickAttrList.get(ihor);
      Float[] picks   = myPickList.get(ihor);
      g.setColor( attr.color );
      int pickIndex = 0;
      while( pickIndex < picks.length ) {
        while( pickIndex < picks.length && picks[pickIndex] == NO_VALUE ) pickIndex++;
        if( pickIndex == picks.length ) break;
        if( pickIndex == picks.length-1 || picks[pickIndex+1] == NO_VALUE ) {
         // plotPoint( picks, attr, pickIndex );
          float xView = seisview.xModel2View( pickIndex );
          float yView = seisview.yModel2View( picks[pickIndex] );
          g.draw( new Ellipse2D.Double(xView-5, yView-5, 11, 11) );
          pickIndex += 1;
          continue;
        }
        int pickIndex2 = pickIndex+1;
        while( pickIndex2 < picks.length && picks[pickIndex2] != NO_VALUE ) pickIndex2++;
        pickIndex2 -= 1;
        GeneralPath path = new GeneralPath();
        for( int itrc = pickIndex; itrc <= pickIndex2; itrc++ ) {
          float xView = seisview.xModel2View( itrc );
          float yView = seisview.yModel2View( picks[itrc] );
          if( itrc > pickIndex ) {
            path.lineTo( xView, yView );
          }
          else {
            path.moveTo( xView, yView );
          }
        }
        g.draw( path );
        pickIndex = pickIndex2 + 1;
        //plotLine( picks, attr, pickIndex, pickIndex2 );
      }
    }
    g.setRenderingHints(rhints_save);
  }
  //-----------------------------------------------------------
  // Mouse is dragged in seismic view
  //
  @Override
  public void mouseDragged( MouseEvent e ) {
    if( myActiveHorizonIndex < 0 ) return;
    pickMouseEvent( e, true );
  }
  private void pickMouseEvent( MouseEvent e, boolean isDrag ) {
    Float[] picks = myPickList.get(myActiveHorizonIndex);
    int pickMode = myPickAttrList.get(myActiveHorizonIndex).mode;
    Point pointView = e.getPoint();
    csSampleInfo sInfo = mySeisView.getSampleInfo( pointView.x, pointView.y );
    if( sInfo.trace < 0 || sInfo.trace >= mySeisView.getTraceBuffer().numTraces() ) return;
    if( SwingUtilities.isLeftMouseButton(e) ) {
      if( pickMode == csPickOverlay.MODE_NONE ) {
        picks[sInfo.trace] = (float)sInfo.sampleDouble;
      }
      else {
        picks[sInfo.trace] = pickPeakTrough( sInfo.trace, sInfo.sample, pickMode );
      }
      if( isDrag || myDoInterpolate ) {
        if( myPrevPickTrace >= 0 ) pickInterpolate( myPrevPickTrace, sInfo.trace );
        myPrevPickTrace = sInfo.trace;
      }
      mySeisView.repaint();
    }
    else if( SwingUtilities.isMiddleMouseButton(e) ) {
      picks[sInfo.trace] = csPickOverlay.NO_VALUE;
      if( myDoInterpolate || isDrag ) {
        if( myPrevPickTrace >= 0 ) {
          int trace1 = myPrevPickTrace+1;
          int trace2 = sInfo.trace-1;
          if( myPrevPickTrace > sInfo.trace ) {
            trace1 = sInfo.trace+1;
            trace2 = Math.min( picks.length-1, myPrevPickTrace-1 );
          }
          for( int itrc = trace1; itrc <= trace2; itrc++ ) {
            picks[itrc] = csPickOverlay.NO_VALUE;
          }
        }
        myPrevPickTrace = sInfo.trace;
      }
      mySeisView.repaint();
    }
  }
//---------------------------------------------------------
  
  public void pickInterpolate( int traceOld, int traceNew ) {
    if( Math.abs(traceOld - traceNew) < 2 ) return;
    Float[] picks = myPickList.get(myActiveHorizonIndex);
    int pickMode = myPickAttrList.get(myActiveHorizonIndex).mode;
    float timeOld = picks[traceOld]; //*(float)mySeisView.getSampleInt();
    float timeNew = picks[traceNew]; //*(float)mySeisView.getSampleInt();
    int trace1, trace2;
    float time1, time2;

    if( traceOld > traceNew ) {
      trace1 = traceNew;
      trace2 = traceOld;
      time1 = timeNew;
      time2 = timeOld;
    }
    else {
      trace1 = traceOld;
      trace2 = traceNew;
      time1 = timeOld;
      time2 = timeNew;
    }
    if( trace1 < 0 ) trace1 = 0;
    if( trace2 >= picks.length) trace2 = picks.length-1;
    int nTraces = trace2 - trace1 + 1;
    float inc = 0.0f;
    if( trace1 != trace2 ) {
      inc = (time2 - time1) / (float) (trace2 - trace1);
    }
//    double[] timePicks = new double[nTraces];
    for( int i = 0; i < nTraces; i++ ) {
      int currentTrace = trace1 + i;
      float time = time1 + inc * (i);
      if( pickMode == csPickOverlay.MODE_NONE ) {
        picks[currentTrace] = time;
      }
      else {
        picks[currentTrace] = pickPeakTrough( currentTrace, (int)time, pickMode );
//        picks[currentTrace] = time;
      }
    }
  }
  

//-----------------------------------------------------------
  // Mouse button is clicked in seismic view
  //
  @Override
  public void mouseClicked(MouseEvent e) {
    if( myActiveHorizonIndex < 0 ) return;
    if( !myDoInterpolate ) myPrevPickTrace = -1; // Prevent interpolation
    pickMouseEvent( e, false );
  }
  //-----------------------------------------------------------
  // Mouse button is released in seismic view
  //
  @Override
  public void mouseReleased(MouseEvent e) {
    if( !myDoInterpolate ) myPrevPickTrace = -1; // Prevent interpolation
  }

  @Override
  public void mouseMoved(MouseEvent e) {
    if( !myDoInterpolate ) myPrevPickTrace = -1; // Prevent interpolation
  }
  @Override
  public void mousePressed(MouseEvent e) {
    if( !myDoInterpolate ) myPrevPickTrace = -1; // Prevent interpolation
  }
  @Override
  public void mouseEntered(MouseEvent e) {
    myPrevPickTrace = -1; // Prevent interpolation
  }
  @Override
  public void mouseExited(MouseEvent e) {
    myPrevPickTrace = -1; // Prevent interpolation
  }
  //----------------------------------------------------------------------
  public float pickPeakTrough( int traceIndex, int sampleIndex1, int pickMode ) {
    int sampleIndex2 = sampleIndex1 + 1;
    csISeismicTraceBuffer traceBuffer = mySeisView.getTraceBuffer();
    int numSamples = traceBuffer.numSamples();
    if( sampleIndex1 >= 0 && sampleIndex2 < numSamples ) {
      float[] samples = traceBuffer.samples(traceIndex);

      if( ( pickMode == csPickOverlay.MODE_PEAK && samples[sampleIndex1] > samples[sampleIndex2] ) ||
          ( pickMode == csPickOverlay.MODE_TROUGH && samples[sampleIndex1] < samples[sampleIndex2] ) ) {
        int sPrev = sampleIndex1;
        int sNext = sPrev-1;
        if( pickMode == csPickOverlay.MODE_PEAK ) {
          while( sNext >= 0 && samples[sNext] > samples[sPrev] ) {
            sPrev = sNext;
            sNext = sNext - 1;
          }
        }
        else {
          while( sNext >= 0 && samples[sNext] < samples[sPrev] ) {
            sPrev = sNext;
            sNext = sNext - 1;
          }
        }
        if( sNext >= 0 ) {
          sampleIndex1 = sNext;
        }
        else {
          sampleIndex1 = sPrev;
        }
      }
      else {
        int sPrev = sampleIndex2;
        int sNext = sPrev+1;
        if( pickMode == csPickOverlay.MODE_PEAK ) {
          while( sNext < numSamples && samples[sNext] > samples[sPrev] ) {
            sPrev = sNext;
            sNext = sNext + 1;
          }
        }
        else {
          while( sNext < numSamples && samples[sNext] < samples[sPrev] ) {
            sPrev = sNext;
            sNext = sNext + 1;
          }          
        }
        if( sNext < numSamples ) {
          sampleIndex1 = sPrev-1;
        }
        else {
          sampleIndex1 = sPrev-2;
        }
      }
      float ratio = (float)computeQuadExtremeRatio( samples[sampleIndex1], samples[sampleIndex1+1], samples[sampleIndex1+2] );
      float outIndex1 = sampleIndex1;
      float outIndex3 = (sampleIndex1+2);
      return( ratio*(outIndex3-outIndex1) + outIndex1 );
    }
    else {
      return( sampleIndex1 );
    }
  }
//-------------------------------------------------------
  public double computeQuadExtremeRatio( double y1, double y2, double y3 ) {
    double extremeRatio = 0.5;
    double min = Math.min(Math.min(y1,y2),y3);
    double max = Math.max(Math.max(y1,y2),y3);
    double x1 = Math.abs(y1);
    double x2 = x1 + (max-min);
    double x3 = x1 + 2.0*(max-min);

    double tmp1 = (y2-y1)/(x2-x1);
    double tmp2 = tmp1*(x3-x1) + y1 - y3;
    double tmp3 = (x2+x1)*(x3-x1) + x1*x1 - x3*x3;

    if( Math.abs(tmp3) != 0.0 ) {
      double a = tmp2 / tmp3;
      double b = tmp1 - a * ( x2 + x1 );
      if( Math.abs(a) != 0.0 ) {
        double xExtremum = -0.5 * b / a;
        extremeRatio = (xExtremum-x1)/(x3-x1);
      }
    }
    return extremeRatio;
  }

  @Override
  public void keyPressed( KeyEvent e ) {
    myDoInterpolate = e.isControlDown();
//    System.out.println("Key pressed: " + e.getKeyCode() + " " + myDoInterpolate );
  }
  @Override
  public void keyReleased( KeyEvent e ) {
    myDoInterpolate = false;
//    System.out.println("Key released: " + e.getKeyCode() + " " + myDoInterpolate );
  }

}

