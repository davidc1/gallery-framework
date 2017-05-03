from database import recoBase
from pyqtgraph.Qt import QtGui, QtCore
from ROOT import evd, larutil
import pyqtgraph as pg


class mctruth(recoBase):

    def __init__(self):
        super(mctruth, self).__init__()
        self._productName = 'mctruth'
        self._process = evd.DrawMCTruth()
        self.init()

    def getLabel(self):
        info = self._process.getData()
        # Determine type of incoming neutrino:
        inneut = {
        12 : "nue",
        -12 : "nuebar",
        14 : "numu",
        -14 : "numubar"
        }.get(info.incoming_lepton_pdg())

        # print info.target_pdg()

        # target = {
        # 2212 : "p",
        # 2112 : "n",
        # 1000180400 : "Ar"
        # }.get(info.target_pdg())

        return inneut + " + " 

    def drawObjects(self, view_manager):
        geom = view_manager._geometry

        # Just draw the vertex to start:
        info = self._process.getData()
        vertex = info.vertex()

        for view in view_manager.getViewPorts():
            self._drawnObjects.append([])

            offset = geom.offset(view.plane())

            vertexPoint = larutil.GeometryHelper.GetME().Point_3Dto2D(
                vertex, view.plane())

            # print "VertexPoint in plane {plane}: ({w},{t})".format(
            #     plane=view.plane(),
            #     w=vertexPoint.w,
            #     t=vertexPoint.t)
            
            points = self.makeCross(vertexPoint.w/geom.wire2cm(),
                                    (vertexPoint.t + offset )/geom.time2cm(),
                                    shortDistX=0.05/geom.wire2cm(),
                                    longDistX=1.0/geom.wire2cm(),
                                    shortDistY=0.05/geom.time2cm(),
                                    longDistY=1.0/geom.time2cm(),
                                    )

            thisPolyF = QtGui.QPolygonF(points)
            thisPoly = QtGui.QGraphicsPolygonItem(thisPolyF)
            thisPoly.setBrush(pg.mkColor((200,200,200,200)))

            self._drawnObjects[view.plane()].append(thisPoly)
            view._view.addItem(thisPoly)
            # #   # get the showers from the process:
            # tracks = self._process.getDataByPlane(view.plane())
            # offset = geom.offset(view.plane()) / geom.time2cm()

            # for i in xrange(len(tracks)):
            #     track = tracks[i]
            #     # construct a polygon for this track:
            #     points = []
            #     # Remeber - everything is in cm, but the display is in
            #     # wire/time!
            #     for pair in track.track():
            #         x = pair.first / geom.wire2cm()
            #         y = pair.second / geom.time2cm() + offset
            #         points.append(QtCore.QPointF(x, y))

            #     # self._drawnObjects[view.plane()].append(thisPoly)

            #     thisPoly = polyLine(points)
            #     pen = pg.mkPen((0,0,0), width=2)
            #     thisPoly.setPen(pen)
            #     # polyLine.draw(view._view)

            #     view._view.addItem(thisPoly)

            #     self._drawnObjects[view.plane()].append(thisPoly)

    def makeCross(self, startX, startY,
                  shortDistX, longDistX,
                  shortDistY, longDistY):
        # Let's draw an X to mark the vertex in each plane.
        points = []
        # Start by filling in points to mark the X:
        points.append(QtCore.QPointF(startX + shortDistX,
                                     startY))
        points.append(QtCore.QPointF(startX + shortDistX + longDistX,
                                     startY - longDistY))
        points.append(QtCore.QPointF(startX + longDistX,
                                     startY - shortDistY - longDistY))

        points.append(QtCore.QPointF(startX,
                                     startY - shortDistY))
        points.append(QtCore.QPointF(startX - longDistX,
                                     startY - longDistY - shortDistY))
        points.append(QtCore.QPointF(startX - longDistX - shortDistX,
                                     startY - longDistY))

        points.append(QtCore.QPointF(startX - shortDistX,
                                     startY))
        points.append(QtCore.QPointF(startX - shortDistX - longDistX,
                                     startY + longDistY))
        points.append(QtCore.QPointF(startX - longDistX,
                                     startY + shortDistY + longDistY))

        points.append(QtCore.QPointF(startX,
                                     startY + shortDistY))
        points.append(QtCore.QPointF(startX + longDistX,
                                     startY + shortDistY + longDistY))
        points.append(QtCore.QPointF(startX + longDistX + shortDistX,
                                     startY + longDistY))

        return points



from database import recoBase3D

try:
    import pyqtgraph.opengl as gl
    import numpy as np
    class mctruth3D(recoBase3D):

        def __init__(self):
            super(mctruth3D, self).__init__()
            self._productName = 'mctruth3D'
            self._process = evd.DrawMCTruth3D()
            self.init()

        def getLabel(self):
            info = self._process.getData()
            # Determine type of incoming neutrino:
            inneut = {
            12 : "nue",
            -12 : "nuebar",
            14 : "numu",
            -14 : "numubar"
            }.get(info.incoming_lepton_pdg())

            # print info.target_pdg()

            # target = {
            # 2212 : "p",
            # 2112 : "n",
            # 1000180400 : "Ar"
            # }.get(info.target_pdg())

            return inneut + " + "

        def drawObjects(self, view_manager):
            geom = view_manager._geometry
            view = view_manager.getView()

            self
            mcts = self._process.getData()

            for mct in mcts:

                # Stupid thing right now:
                # make a 3 lines around the vertex

                xline = np.ndarray((2, 3))
                yline = np.ndarray((2, 3))
                zline = np.ndarray((2, 3))

                length = 5

                for line in xline, yline, zline:
                    for point in line:
                        point[0] = mct.NuX()
                        point[1] = mct.NuY()
                        point[2] = mct.NuZ()

                xline[0][0] += length/2
                xline[1][0] -= length/2
                yline[0][1] += length/2
                yline[1][1] -= length/2
                zline[0][2] += length/2
                zline[1][2] -= length/2

                # Make the 3 lines for the vertex:

                # pts = np.vstack([x, y, z]).transpose()
                # pen = pg.mkPen((255, 0, 0), width=2)
                xglline = gl.GLLinePlotItem(pos=xline, width=3,
                                            color=(0., 0.690, 0.545, 1.0))
                yglline = gl.GLLinePlotItem(pos=yline, width=3,
                                            color=(0., 0.690, 0.545, 1.0))
                zglline = gl.GLLinePlotItem(pos=zline, width=3,
                                            color=(0., 0.690, 0.545, 1.0))
                view.addItem(xglline)
                view.addItem(yglline)
                view.addItem(zglline)
                self._drawnObjects.append(xglline)
                self._drawnObjects.append(yglline)
                self._drawnObjects.append(zglline)

except Exception, e:
    pass

