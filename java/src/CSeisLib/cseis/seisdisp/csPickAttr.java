/* Copyright (c) Colorado School of Mines, 2013.*/
/* All rights reserved.                       */


package cseis.seisdisp;

import java.awt.Color;

public class csPickAttr {
  private final int idNumber;
  public String name;
  public Color color;
  public int mode;
  private csPickAttr() {
    idNumber = -1;
  }
  public csPickAttr( int id ) {
    idNumber = id;
    name = "";
    color = Color.black;
    mode = csPickOverlay.MODE_PEAK;
  }
  public csPickAttr( int id, String name_in, Color color_in, int mode_in ) {
    idNumber = id;
    name  = name_in;
    color = color_in;
    mode  = mode_in;
  }
  public int getID() {
    return idNumber;
  }
}
