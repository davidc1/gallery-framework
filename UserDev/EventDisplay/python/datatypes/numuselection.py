from database import recoBase
from ROOT import evd, TVector3
from pyqtgraph.Qt import QtGui, QtCore
import pyqtgraph as pg
import math as mt
from track import polyLine
from shower import shower_polygon

class numuselection(recoBase):
    """docstring for numuselection"""
    def __init__(self):
        super(numuselection, self).__init__()
        self._productName = 'numuselection'
        self._process = evd.DrawNumuSelection()
        self._brush = (0, 0, 0)
        self.init()

         

    # this is the function that actually draws the numu selection output.
    def drawObjects(self, view_manager):

        geom = view_manager._geometry
        for view in view_manager.getViewPorts():
            thisPlane = view.plane()
            self._drawnObjects.append([])
            # First get the hit information:
            numus = self._process.getDataByPlane(thisPlane)

            for i in xrange(len(numus)):


                # Draw the vertex:

                vertex = numus[i].vertex()
                # Draws a circle at (x,y,radius = 0.5cm)
                radBigW = 3 / view_manager._geometry.wire2cm()
                radBigT = (3) / view_manager._geometry.time2cm()

                offset = view_manager._geometry.offset(
                    thisPlane) / view_manager._geometry.time2cm()

                sW = vertex.w / view_manager._geometry.wire2cm()
                sT = vertex.t / view_manager._geometry.time2cm() + offset

                r = QtGui.QGraphicsEllipseItem(
                    sW-radBigW, sT-radBigT, 2*radBigW, 2*radBigT)


                r.setPen(pg.mkPen(None))
                r.setBrush(pg.mkColor(139,0,139, 128))
                self._drawnObjects[thisPlane].append(r)
                view._view.addItem(r)


                # Draw all the tracks:
                tracks = numus[i].tracks()
                for j in xrange(len(numus[i].tracks())):
                    track = tracks[j]
                    # construct a polygon for this track:
                    points = []
                    # Remeber - everything is in cm, but the display is in
                    # wire/time!
                    for pair in track.track():
                        x = pair.first / geom.wire2cm()
                        y = pair.second / geom.time2cm() + offset
                        points.append(QtCore.QPointF(x, y))

                    thisPoly = polyLine(points)

                    #Change the color here:
                    if j == numus[i].muon_index():
                        # Do something special with the muon
                        print '\t the muon!'
                        pen = pg.mkPen((238,130,238), width=2)
                    else:
                        print '\t other tracks!'
                        pen = pg.mkPen((139,0,139), width=2)

                    thisPoly.setPen(pen)
                    # polyLine.draw(view._view)
                
                    view._view.addItem(thisPoly)

                    self._drawnObjects[view.plane()].append(thisPoly)

                # showers
                showers = numus[i].showers()

                i_color = 0

                for i in xrange(len(showers)):
                
                    shower = showers[i]

                    color = (252, 127, 0, 100)

                    # construct a polygon for this shower:
                    points = []
                    # Remember - everything is in cm, but the display is in
                    # wire/time!
                    geom = view_manager._geometry
                    offset = geom.offset(view.plane()) / geom.time2cm()
                    x = shower.startPoint().w / geom.wire2cm()
                    y = shower.startPoint().t / geom.time2cm() + offset

                    points.append(QtCore.QPoint(x, y))
                    # next connect the two points at the end of the shower to make
                    # a cone
                    #
                    # We need the vector that's perpendicular to the axis, to make the cone.
                    # Use 3D vectors to allow the cross product:
                    zAxis = TVector3(0, 0, 1)
                    showerAxis = TVector3(shower.endPoint().w - shower.startPoint().w,
                                          shower.endPoint().t -
                                          shower.startPoint().t,
                                          0.0)
                    perpAxis = zAxis.Cross(showerAxis)
                    
                    length = showerAxis.Mag() * mt.tan(shower.openingAngle()/2)
                    perpAxis *= length / perpAxis.Mag()
                    
                    
                    x1, y1 = shower.endPoint().w + perpAxis.X(), shower.endPoint().t + \
                        perpAxis.Y()
                    x2, y2 = shower.endPoint().w - perpAxis.X(), shower.endPoint().t - \
                        perpAxis.Y()
                    
                    # Scale everything to wire/time:
                    x1 /= geom.wire2cm()
                    y1 /= geom.time2cm() 
                    x2 /= geom.wire2cm()
                    y2 /= geom.time2cm() 
                    
                    y1 += offset
                    y2 += offset
                    
                    points.append(QtCore.QPoint(x1, y1))
                    points.append(QtCore.QPoint(x2, y2))
                    
                    
                    thisPolyF = QtGui.QPolygonF(points)
                
                    self.shower_poly = QtGui.QGraphicsPolygonItem( thisPolyF )
                    self.shower_poly = shower_polygon( thisPolyF )
                    
                    self.shower_poly.setPen(pg.mkPen(None))
                    self.shower_poly.setBrush(pg.mkColor(color))
                    
                    view._view.addItem(self.shower_poly)
                    self._drawnObjects[view.plane()].append(self.shower_poly)
