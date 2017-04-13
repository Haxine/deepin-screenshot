#include "calculaterect.h"

const int padding = 2;
const int ROTATEPOINT_PADDING = 30;
const int TANT_EDGEVALUE = 0.78539;
const int TANT2_EDGEVALUE = 2.35619;
const int MIN_PADDING = 3;
const qreal SLOPE = 0.5522848;
/* get a rect by diagPoints */
QRect diagPointsRect(DiagPoints diagPoints) {
    return QRect(std::min(diagPoints.masterPoint.x(), diagPoints.deputyPoint.x()),
                 std::min(diagPoints.masterPoint.y(), diagPoints.deputyPoint.y()),
                 std::abs(diagPoints.masterPoint.x() - diagPoints.deputyPoint.x()),
                 std::abs(diagPoints.masterPoint.y() - diagPoints.deputyPoint.y()));
}

/* judge whether the point1 is on the point2 or not */
bool pointClickIn(QPointF point2, QPointF point1, int padding) {
    if (point2.x() >= point1.x() - padding && point2.x() <= point1.x() + padding &&
            point2.y() >= point1.y() - padding && point2.y() <= point1.y() + padding) {
        return true;
    } else {
        return false;
    }
}

/* judge whether the point3 is on the segment*/
bool pointOnLine(QPointF point1, QPointF point2, QPointF point3) {
    if (point1.x() == point2.x()) {
           if (point3.x() >= point1.x() - padding && point3.x() <= point1.x() + padding &&
           point3.y() >= std::min(point1.y(), point2.y()) - padding && point3.y() <= std::max(point1.y(), point2.y()) + padding) {
               return true;
           }
       } else {
           qreal k =  (point2.y() - point1.y()) / (point2.x() - point1.x());
           qreal b = point1.y() - point1.x()*k;

           if ( point3.x() >= std::min(point1.x(), point2.x()) -padding && point3.x() <= std::max(point1.x(), point2.x() + padding)
           && point3.y() >= k * point3.x() + b - padding && point3.y() <= k * point3.x() + b + padding) {
               return true;
           }
       }
       return false;
}

/* To determine whether a point on the rectangle*/
bool pointOnRect(DiagPoints diagPoints, QPointF pos) {
    QPointF point1 = diagPoints.masterPoint;
    QPointF point3 = diagPoints.deputyPoint;
    QPointF point2 = QPointF(diagPoints.masterPoint.x(), diagPoints.deputyPoint.y());
    QPointF point4 = QPointF(diagPoints.deputyPoint.x(), diagPoints.masterPoint.y());

    if (pointOnLine(point1, point2, pos) || pointOnLine(point1, point4, pos) ||
            pointOnLine(point2, point3, pos) || pointOnLine(point3, point4, pos)) {
        return true;
    } else {
        return false;
    }
}

/* get the point who splid a distance on a line */
QPointF pointSplid(QPointF point1, QPointF point2, qreal padding) {
    if (point1.x() == point2.x()) {
        return QPointF(0, padding);
    } else {
        qreal tmpX = padding*std::cos(std::atan2(std::abs(point1.y() - point2.y()), std::abs(point1.x() - point2.x())));
        qreal tmpY = padding*std::sin(std::atan2(std::abs(point1.y() - point2.y()), std::abs(point1.x() - point2.x())));
        return QPointF(tmpX, tmpY);
    }
}

/* get the rotate point by four points in a rectangle*/
QPointF getRotatePoint(QPointF point1, QPointF point2, QPointF point3, QPointF point4) {
    QPointF leftPoint = QPointF(0, 0);
    QPointF rightPoint = QPointF(0, 0);
    QPointF rotatePoint = QPointF(0, 0);

    QPointF leftSplidPoint = pointSplid(point1, point2, ROTATEPOINT_PADDING);
    QPointF rightSplidPoint = pointSplid(point3, point4, ROTATEPOINT_PADDING);

    /* first position*/
    if (point2.x() - point4.x() <= 0 && point2.y() - point4.y() >= 0) {
        leftPoint = QPointF(point1.x() - leftSplidPoint.x(), point1.y() - leftSplidPoint.y());
        rightPoint = QPointF(point3.x() - rightSplidPoint.x(), point3.y() - rightSplidPoint.y());
        rotatePoint = QPointF((leftPoint.x() + rightPoint.x())/2, (leftPoint.y() + rightPoint.y())/2);
        return rotatePoint;
    }

    /* second position*/
    if (point2.x() - point4.x() > 0 && point2.y() - point4.y() > 0) {
        leftPoint = QPointF(point1.x() - leftSplidPoint.x(), point1.y() + leftSplidPoint.y());
        rightPoint = QPointF(point3.x() - rightSplidPoint.x(), point3.y() + rightSplidPoint.y());
        rotatePoint = QPointF((leftPoint.x() + rightPoint.x())/2, (leftPoint.y() + rightPoint.y())/2);
        return rotatePoint;
    }

    /* third position*/
    if (point2.x() - point4.x() < 0 && point2.y() - point4.y() < 0) {
        leftPoint = QPointF(point1.x() + leftSplidPoint.x(), point1.y() - leftSplidPoint.y());
        rightPoint = QPointF(point3.x() + rightSplidPoint.x(), point3.y() - rightSplidPoint.y());
        rotatePoint = QPointF((leftPoint.x() + rightPoint.x())/2, (leftPoint.y() + rightPoint.y())/2);
        return rotatePoint;
    }

    /* fourth position*/
    if (point2.x() - point4.x() > 0 && point2.y() - point4.y() < 0) {
        leftPoint = QPointF(point1.x() + leftSplidPoint.x(), point1.y() + leftSplidPoint.y());
        rightPoint = QPointF(point3.x() + rightSplidPoint.x(), point3.y() + rightSplidPoint.y());
        rotatePoint = QPointF((leftPoint.x() + rightPoint.x())/2, (leftPoint.y() + rightPoint.y())/2);
        return rotatePoint;
    }

    return rotatePoint;
}

FourPoints fourPointsOnRect(DiagPoints diagPoints) {
    FourPoints  fourPoints;
    QPointF point1 = QPointF(std::min(diagPoints.masterPoint.x(), diagPoints.deputyPoint.x()),
                           std::min(diagPoints.masterPoint.y(), diagPoints.deputyPoint.y()));
    QPointF point2 = QPointF(std::min(diagPoints.masterPoint.x(), diagPoints.deputyPoint.x()),
                           std::max(diagPoints.masterPoint.y(), diagPoints.deputyPoint.y()));
    QPointF point3 = QPointF(std::max(diagPoints.masterPoint.x(), diagPoints.deputyPoint.x()),
                           std::min(diagPoints.masterPoint.y(), diagPoints.deputyPoint.y()));
    QPointF point4 = QPointF(std::max(diagPoints.masterPoint.x(), diagPoints.deputyPoint.x()),
                           std::max(diagPoints.masterPoint.y(), diagPoints.deputyPoint.y()));

    fourPoints.point1 = point1;
    fourPoints.point2 = point2;
    fourPoints.point3 = point3;
    fourPoints.point4 = point4;
    return fourPoints;
}

FourPoints getAnotherFPoints(QPointF point1, QPointF point2,
                             QPointF point3, QPointF point4) {
FourPoints  otherFPoints;
otherFPoints.point1 = QPoint((point1.x() + point2.x())/2, (point1.y() + point2.y())/2);
otherFPoints.point2 = QPoint((point1.x() + point3.x())/2, (point1.y() + point3.y())/2);
otherFPoints.point3 = QPoint((point3.x() + point4.x())/2, (point3.y() + point4.y())/2);
otherFPoints.point4 = QPoint((point2.x() + point4.x())/2, (point2.y() + point4.y())/2);
return otherFPoints;
}
/*
 *  this function is get the angle of the mouse'moving*/
/* the angle in point3 */

qreal calculateAngle(QPointF point1, QPointF point2, QPointF point3) {
    if (point1 == point2) {
        return 0;
    }

    qreal a = std::pow(point1.x() - point3.x(), 2) + std::pow(point1.y() - point3.y(), 2);
    qreal b = std::pow(point2.x() - point3.x(), 2) + std::pow(point2.y() - point3.y(), 2);
    qreal c = std::pow(point1.x() - point2.x(), 2) + std::pow(point1.y() - point2.y(), 2);

    qreal angle =std::cos(( a + b - c)/(2*std::sqrt(a)*std::sqrt(b)));
    if (point1.x() <= point3.x() && point1.y() < point3.y()) {
        if (point2.x() < point1.x() || point2.y() > point1.y()) {
            angle = - angle;
        }
    }
    if (point1.x() < point3.x() && point1.y() >= point3.y()) {
        if (point2.x() > point1.x() || point2.y() > point1.y()) {
            angle = - angle;
        }
    }
    if (point1.x() >= point3.x() && point1.y() > point3.y()) {
        if (point2.x() > point1.x() || point2.y() < point1.y()) {
            angle = - angle;
        }
    }
    if (point1.x() > point3.x() && point1.y() <= point3.y()) {
        if (point2.x() < point1.x() || point2.y() < point1.y()) {
            angle = - angle;
        }
    }
    return angle;
}

/* point2 is the rotatePoint, point1 is the centerPoint, point3 is point2 who rotate */
QPointF pointRotate(QPointF point1, QPointF point2, qreal angle) {
    QPointF middlePoint = QPointF(point2.x() - point1.x(), point2.y() - point1.y());
    QPointF tmpPoint = QPointF(middlePoint.x() * std::cos(angle) - middlePoint.y() * std::sin(angle),
                             middlePoint.x() * std::sin(angle) + middlePoint.y() * std::cos(angle));
    QPointF point3 = QPointF(tmpPoint.x() + point1.x(), tmpPoint.y() + point1.y());

    return point3;
}

/* the distance from a point(point3) to a line(point1, point2) */
qreal pointToLineDistance(QPointF point1, QPointF point2, QPointF point3) {
    if (point1.x() == point2.x()) {
        return std::abs(point3.x() - point1.x());
    } else {
        qreal k = (point1.y() - point2.y())/(point1.x() - point2.x());
        qreal b = point1.y() - point1.x() * k;
        return std::abs(point3.x() * k + b - point3.y()) / std::sqrt(std::pow(k, 2) + 1);
    }
}

/* judge the direction of point3 of line(point1, point2) */
qreal pointLineDir(QPointF point1, QPointF point2, QPointF point3) {
    if (point1.x() == point2.x()) {
        if (point3.x() <= point1.x()) {
            return -1;
        } else {
            return 1;
        }
    } else {
        qreal k = (point1.y() - point2.y()) / (point1.x() - point2.x());
        qreal b = point1.y() - point1.x() * k;
        if (point3.y() <= point3.x() * k + b) {
            return -1;
        } else {
            return 1;
        }
    }
 }

/* calculate the control point of the beizer */
QPointF getControlPoint(QPointF point1, QPointF point2, bool direction)  {
    qreal k1 = SLOPE;
    qreal k2 = 1 - SLOPE;
    qreal k3;

    if (direction) {
        k3 = k2/k1;
    } else {
        k3 = k1/k2;
    }
    QPointF resultPoint = QPoint((point1.x() + k3*point2.x())/(1+k3),
                                                         (point1.y() + k3*point2.y())/(1+k3));
    return resultPoint;
}

/* get eight control points */
QList<QPointF> getEightControlPoint(FourPoints rectFPoints) {
    FourPoints anotherFPoints = getAnotherFPoints(rectFPoints.point1, rectFPoints.point2,
                                                  rectFPoints.point3, rectFPoints.point4);

    QList<QPointF> resultPointList;
    resultPointList.append(getControlPoint(rectFPoints.point1, anotherFPoints.point1, true));
    resultPointList.append(getControlPoint(rectFPoints.point1, anotherFPoints.point2, true));
    resultPointList.append(getControlPoint(anotherFPoints.point1, rectFPoints.point2, false));
    resultPointList.append(getControlPoint(rectFPoints.point2, anotherFPoints.point4, true));
    resultPointList.append(getControlPoint(anotherFPoints.point2, rectFPoints.point3, false));
    resultPointList.append(getControlPoint(rectFPoints.point3, anotherFPoints.point3, true));
    resultPointList.append(getControlPoint(anotherFPoints.point3, rectFPoints.point4, false));
    resultPointList.append(getControlPoint(rectFPoints.point4, anotherFPoints.point4, true));
    qDebug() << resultPointList;
    return resultPointList;
}

/* judge whether the clickOnPoint is on the bezier */
/* 0 <= pos.x() <= 1*/
bool pointOnBezier(QPointF point1, QPointF point2, QPointF point3, QPointF point4, QPointF pos) {
    for (qreal t = 0; t <= 1; t = t + 0.1) {
        qreal bx = point1.x()*(1-t)*std::pow(1-t, 2) + 3*point1.x()*t*std::pow(1-t, 2)
                + 3*point2.x()*std::pow(t, 2)*(1-t) + point3.x()*t*std::pow(t, 2);
        qreal by = point1.y()*(1-t)*std::pow(1-t, 2) + 3*point1.y()*t*std::pow(1-t, 2)
                + 3*point2.y()*std::pow(t, 2)*(1-t) + point3.y()*t*std::pow(t, 2);
        if (pos.x() >= bx - MIN_PADDING && pos.x() <= bx + MIN_PADDING &&
                pos.y() >= by - MIN_PADDING && pos.y() <= by + MIN_PADDING) {
            return true;
        }
    }
    return false;
}

/* judge whether the clickOnPoint is on the ellipse */
bool pointOnEllipse(FourPoints rectFPoints, QPointF pos) {
    FourPoints anotherFPoints = getAnotherFPoints(rectFPoints.point1, rectFPoints.point2,
                                                  rectFPoints.point3, rectFPoints.point4);
    QList<QPointF> controlPointList;
    controlPointList.append(getControlPoint(rectFPoints.point1, anotherFPoints.point1, true));
    controlPointList.append(getControlPoint(rectFPoints.point1, anotherFPoints.point2, true));
    controlPointList.append(getControlPoint(anotherFPoints.point1, rectFPoints.point2, false));
    controlPointList.append(getControlPoint(rectFPoints.point2, anotherFPoints.point4, true));
    controlPointList.append(getControlPoint(anotherFPoints.point1, rectFPoints.point3, true));
    controlPointList.append(getControlPoint(rectFPoints.point3, anotherFPoints.point3, true));
    controlPointList.append(getControlPoint(anotherFPoints.point3, rectFPoints.point4, true));
    controlPointList.append(getControlPoint(rectFPoints.point4, anotherFPoints.point4, true));

    if (pointOnBezier(anotherFPoints.point1, controlPointList[0], controlPointList[1],
        anotherFPoints.point2, pos) || pointOnBezier(anotherFPoints.point2, controlPointList[4],
      controlPointList[5], anotherFPoints.point3, pos) || pointOnBezier(anotherFPoints.point3,
      controlPointList[6], controlPointList[7], anotherFPoints.point1, pos) || pointOnBezier(
      anotherFPoints.point4, controlPointList[3], controlPointList[2], anotherFPoints.point3, pos)) {
        return true;
    } else {
        return false;
    }
}

/* handle resize of eight points in rectangle */
FourPoints resizePointPosition(QPointF point1, QPointF point2, QPointF point3, QPointF point4,
                          QPointF pos, int key,  bool isShift) {
    FourPoints resizeFPoints;
    resizeFPoints.point1 = point1;
    resizeFPoints.point2 = point2;
    resizeFPoints.point3 = point3;
    resizeFPoints.point4 = point4;
    if (point1.x() - point2.x() < 0 && point1.y() - point2.y() < 0 &&
    point1.x() - point3.x() < 0 && point1.y() - point3.y() > 0) {
        switch (key) {
        case 0: { resizeFPoints = point1Resize1(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
        case 1: { resizeFPoints = point2Resize1(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
        case 2: { resizeFPoints = point3Resize1(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
        case 3: { resizeFPoints = point4Resize1(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
        case 4: { resizeFPoints = point5Resize1(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
        case 5: { resizeFPoints = point6Resize1(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
        case 6: { resizeFPoints = point7Resize1(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
        case 7: { resizeFPoints = point8Resize1(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
        }
    }
    if (point1.x() - point2.x() < 0 && point1.y() - point2.y() > 0 &&
    point1.x() - point3.x() > 0 && point1.y() - point3.y() > 0) {
        switch(key) {
        case 0: { resizeFPoints = point1Resize2(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
        case 1: { resizeFPoints = point2Resize2(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
        case 2: { resizeFPoints = point3Resize2(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
        case 3: { resizeFPoints = point4Resize2(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
        case 4: { resizeFPoints = point5Resize2(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
        case 5: { resizeFPoints = point6Resize2(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
        case 6: { resizeFPoints = point7Resize2(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
        case 7: { resizeFPoints = point8Resize2(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
        }
    }
    if (point1.x() - point2.x() > 0 && point1.y() - point2.y() < 0 &&
    point1.x() - point3.x() < 0 && point1.y() - point3.y()) {
        switch(key) {
        case 0: { resizeFPoints = point1Resize3(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
        case 1: { resizeFPoints = point2Resize3(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
        case 2: { resizeFPoints = point3Resize3(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
        case 3: { resizeFPoints = point4Resize3(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
        case 4: { resizeFPoints = point5Resize3(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
        case 5: { resizeFPoints = point6Resize3(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
        case 6: { resizeFPoints = point7Resize3(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
        case 7: { resizeFPoints = point8Resize3(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
        }
    }
    if (point1.x() - point2.x() > 0 && point1.y() - point2.y() > 0 &&
    point1.x() - point3.x() > 0 && point1.y() - point3.y() < 0) {
        switch(key) {
        case 0: { resizeFPoints = point1Resize4(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
        case 1: { resizeFPoints = point2Resize4(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
        case 2: { resizeFPoints = point3Resize4(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
        case 3: { resizeFPoints = point4Resize4(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
        case 4: { resizeFPoints = point5Resize4(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
        case 5: { resizeFPoints = point6Resize4(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
        case 6: { resizeFPoints = point7Resize4(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
        case 7: { resizeFPoints = point8Resize4(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
        }
    }
    if (point1.x() == point2.x() && point1.y() < point2.y() &&
    point1.x() < point3.x() && point1.y() == point3.y()) {
        switch(key) {
        case 0: { resizeFPoints = point1Resize5(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
        case 1: { resizeFPoints = point2Resize5(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
        case 2: { resizeFPoints = point3Resize5(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
        case 3: { resizeFPoints = point4Resize5(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
        case 4: { resizeFPoints = point5Resize5(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
        case 5: { resizeFPoints = point6Resize5(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
        case 6: { resizeFPoints = point7Resize5(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
        case 7: { resizeFPoints = point8Resize5(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
        }
    }
    if (point1.x() < point2.x() && point1.y() == point2.y() &&
    point1.x() == point3.x() && point1.y() < point3.y()) {
        switch(key) {
        case 0: { resizeFPoints = point1Resize6(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
        case 1: { resizeFPoints = point2Resize6(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
        case 2: { resizeFPoints = point3Resize6(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
        case 3: { resizeFPoints = point4Resize6(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
        case 4: { resizeFPoints = point5Resize6(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
        case 5: { resizeFPoints = point6Resize6(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
        case 6: { resizeFPoints = point7Resize6(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
        case 7: { resizeFPoints = point8Resize6(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
        }
    }
    if (point1.x() < point2.x() && point1.y() == point2.y() &&
    point1.x() == point3.x() && point1.y() > point3.y()) {
        switch(key) {
        case 0: { resizeFPoints = point1Resize7(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
        case 1: { resizeFPoints = point2Resize7(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
        case 2: { resizeFPoints = point3Resize7(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
        case 3: { resizeFPoints = point4Resize7(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
        case 4: { resizeFPoints = point5Resize7(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
        case 5: { resizeFPoints = point6Resize7(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
        case 6: { resizeFPoints = point7Resize7(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
        case 7: { resizeFPoints = point8Resize7(point1, point2, point3, point4, pos, isShift); return resizeFPoints;}
        }
    }
    return resizeFPoints;
}

/* first point1 */
/* point1 in the first position*/
FourPoints point1Resize1(QPointF point1, QPointF point2, QPointF point3,
                         QPointF point4, QPointF pos, bool isShift) {
    FourPoints newResizeFPoint;
    newResizeFPoint.point1 = point1;
    newResizeFPoint.point2 = point2;
    newResizeFPoint.point3 = point3;
    newResizeFPoint.point4 = point4;
    if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) >= TANT_EDGEVALUE &&
    (pos.y() + MIN_PADDING > point2.y() || pos.x() + MIN_PADDING > point3.x() ||
     pointLineDir(point3, point4, pos) == -1 || pointLineDir(point2, point4, pos) == 1)) {
        return newResizeFPoint;
    } else if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) < TANT_EDGEVALUE &&
      (pos.x() + MIN_PADDING > point2.x() || pos.y() - MIN_PADDING < point3.y() ||
       pointLineDir(point3, point4, pos) == -1 || pointLineDir(point2, point4, pos) == 1)) {
        return newResizeFPoint;
    }  else {
        if (pointToLineDistance(point4, point2, pos) < MIN_PADDING ||
                pointToLineDistance(point3, point4, pos) < MIN_PADDING) {
            return newResizeFPoint;
        } else {
            if (!isShift) {
                if (pointLineDir(point1, point3, pos) == -1 && pointLineDir(point1, point2, pos) == 1) {
                    qreal distance= pointToLineDistance(point1, point2, pos);
                    QPointF add = pointSplid(point2, point4, distance);
                    point2 = QPointF(point2.x() - add.x(), point2.y() + add.y());
                    distance = pointToLineDistance(point1, point3, pos);
                    add = pointSplid(point3, point4, distance);
                    point3 = QPointF(point3.x() - add.x(), point3.y() - add.y());
                    point1 = pos;
                }
                if (pointLineDir(point1, point3, pos) == -1 && pointLineDir(point1, point2, pos) == -1) {
                    qreal distance= pointToLineDistance(point1, point2, pos);
                    QPointF add = pointSplid(point2, point4, distance);
                    point2 = QPointF(point2.x() + add.x(), point2.y() - add.y());
                    distance = pointToLineDistance(point1, point3, pos);
                    add = pointSplid(point3, point4, distance);
                    point3 = QPointF(point3.x() - add.x(), point3.y() - add.y());
                    point1 = pos;
                }
                if (pointLineDir(point1, point3, pos) == 1 && pointLineDir(point1, point2, pos) == -1) {
                    qreal distance= pointToLineDistance(point1, point2, pos);
                    QPointF add = pointSplid(point2, point4, distance);
                    point2 = QPointF(point2.x() + add.x(), point2.y() - add.y());
                    distance = pointToLineDistance(point1, point3, pos);
                    add = pointSplid(point3, point4, distance);
                    point3 = QPointF(point3.x() + add.x(), point3.y() + add.y());
                    point1 = pos;
                }
                if (pointLineDir(point1, point3, pos) == 1 && pointLineDir(point1, point2, pos) == 1) {
                    qreal distance= pointToLineDistance(point1, point2, pos);
                    QPointF add = pointSplid(point2, point4, distance);
                    point2 = QPointF(point2.x() - add.x(), point2.y() + add.y());
                    distance = pointToLineDistance(point1, point3, pos);
                    add = pointSplid(point3, point4, distance);
                    point3 = QPointF(point3.x() + add.x(), point3.y() + add.y());
                    point1 = pos;
                }
                newResizeFPoint.point1 = point1;
                newResizeFPoint.point2 = point2;
                newResizeFPoint.point3 = point3;
                newResizeFPoint.point4 = point4;
                return newResizeFPoint;
            } else {
                 qreal distance1 = pointToLineDistance(point1, point2, pos);
                 qreal distance2 = pointToLineDistance(point1, point3, pos);
                 qreal distance = std::min(distance1, distance2);
                if (pointLineDir(point1, point3, pos) == 1 && pointLineDir(point1, point2, pos) == -1) {         
                    QPointF add = pointSplid(point2, point4, distance);
                    point2 = QPointF(point2.x() + add.x(), point2.y() - add.y());
                    add = pointSplid(point3, point4, distance);
                    point3 = QPointF(point3.x() + add.x(), point3.y() + add.y());
                    point1 = QPointF(point2.x() + point3.x() - point4.x(), point2.y() + point3.y() - point4.y());
                } else {
                    QPointF add = pointSplid(point2, point4, distance);
                    point2 = QPointF(point2.x() - add.x(), point2.y() + add.y());
                    add = pointSplid(point3, point4, distance);
                    point3 = QPointF(point3.x() - add.x(), point3.y() - add.y());
                    point1 = QPointF(point2.x() + point3.x() - point4.x(), point2.y() + point3.y() - point4.y());
                }
                newResizeFPoint.point1 = point1;
                newResizeFPoint.point2 = point2;
                newResizeFPoint.point3 = point3;
                newResizeFPoint.point4 = point4;
                return newResizeFPoint;
            }
            return newResizeFPoint;
        }
    }
}

/* point1 in the second position */
FourPoints point1Resize2(QPointF point1, QPointF point2, QPointF point3,
                         QPointF point4, QPointF pos, bool isShift) {
    FourPoints newResizeFPoints;
    newResizeFPoints.point1 = point1;
    newResizeFPoints.point2 = point2;
    newResizeFPoints.point3 = point3;
    newResizeFPoints.point4 = point4;
    if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) <= -TANT_EDGEVALUE
    && (pos.y() - MIN_PADDING < point2.y() || pos.x() - MIN_PADDING < point3.x() ||
        pointLineDir(point3, point4, pos) == -1 || pointLineDir(point2, point4, pos) == -1)) {
        return newResizeFPoints;
    } else if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) > -TANT_EDGEVALUE
    && (pos.y() - MIN_PADDING < point3.y() || pos.x() + MIN_PADDING > point2.x() ||
    pointLineDir(point3, point4, pos) == -1 || pointLineDir(point2, point4, pos) == -1))  {
        return newResizeFPoints;
    } else {
        if (!isShift) {
            if (pointLineDir(point1, point3, pos) == 1 && pointLineDir(point1, point2, pos) == 1) {
                qreal distance = pointToLineDistance(point1, point2, pos);
                QPointF add = pointSplid(point2, point4, distance);
                point2 = QPointF(point2.x() + add.x(), point2.y() + add.y());
                distance = pointToLineDistance(point1, point3, pos);
                add = pointSplid(point3, point4, distance);
                point3 = QPointF(point3.x() - add.x(), point3.y() + add.y());
                point1 = pos;
            }
            if (pointLineDir(point1, point3, pos) == 1 && pointLineDir(point1, point2, pos) == -1) {
                qreal distance = pointToLineDistance(point1, point2, pos);
                QPointF add = pointSplid(point2, point4, distance);
                point2 = QPointF(point2.x() - add.x(), point2.y() - add.y());
                distance = pointToLineDistance(point1, point3, pos);
                add = pointSplid(point3, point4, distance);
                point3 = QPointF(point3.x() - add.x(), point3.y() + add.y());
                point1 = pos;
            }
            if (pointLineDir(point1, point3, pos) == -1 && pointLineDir(point1, point2, pos) == -1) {
                qreal distance = pointToLineDistance(point1, point2, pos);
                QPointF add = pointSplid(point2, point4, distance);
                point2 = QPointF(point2.x() - add.x(), point2.y() - add.y());
                distance = pointToLineDistance(point1, point3, pos);
                add = pointSplid(point3, point4, distance);
                point3 = QPointF(point3.x() + add.x(), point3.y() - add.y());
                point1 = pos;
            }
            if (pointLineDir(point1, point3, pos) == -1 && pointLineDir(point1, point2, pos) == 1) {
                qreal distance = pointToLineDistance(point1, point2, pos);
                QPointF add = pointSplid(point2, point4, distance);
                point2 = QPointF(point2.x() + add.x(), point2.y() + add.y());
                distance = pointToLineDistance(point1, point3, pos);
                add = pointSplid(point3, point4, distance);
                point3 = QPointF(point3.x() + add.x(), point3.y() - add.y());
                point1 = pos;
            }
            newResizeFPoints.point1 = point1;
            newResizeFPoints.point2 = point2;
            newResizeFPoints.point3 = point3;
            newResizeFPoints.point4 = point4;
            return newResizeFPoints;
        } else {
            qreal distance1 = pointToLineDistance(point1, point2, pos);
            qreal distance2 = pointToLineDistance(point1, point3, pos);
            qreal distance = std::min(distance1, distance2);
            if (pointLineDir(point1, point2, pos) == -1 && pointLineDir(point1, point2, pos) == -1) {
                QPointF add = pointSplid(point2, point4, distance);
                point2 = QPointF(point2.x() - add.x(), point2.y() - add.y());
                add = pointSplid(point3, point4, distance);
                point3 = QPointF(point3.x() + add.x(), point3.y() - add.y());
                point1 = QPointF(point2.x() + point3.x() - point4.x(), point2.y() + point3.y() - point4.y());
            } else {
                QPointF add = pointSplid(point2, point4, distance);
                point2 = QPointF(point2.x() + add.x(), point2.y() + add.y());
                add = pointSplid(point3, point4, distance);
                point3 = QPointF(point3.x() - add.x(), point3.y() + add.y());
                point1 = QPointF(point2.x() + point3.x() - point4.x(), point2.y() + point3.y() - point4.y());
            }
            newResizeFPoints.point1 = point1;
            newResizeFPoints.point2 = point2;
            newResizeFPoints.point3 = point3;
            newResizeFPoints.point4 = point4;
            return newResizeFPoints;
        }
    }
    return newResizeFPoints;
}

/* point1 in the third position*/
FourPoints point1Resize3(QPointF point1, QPointF point2, QPointF point3,
                         QPointF point4, QPointF pos, bool isShift) {
    FourPoints newResizeFPoints;
    newResizeFPoints.point1 = point1;
    newResizeFPoints.point2 = point2;
    newResizeFPoints.point3 = point3;
    newResizeFPoints.point4 = point4;
    if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) >= TANT2_EDGEVALUE
    && (pos.x() - MIN_PADDING < point2.x() || pos.y() + MIN_PADDING > point3.y() ||
        pointLineDir(point3, point4, pos) == 1 || pointLineDir(point2, point4, pos) == 1)) {
        return newResizeFPoints;
    } else if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) < TANT2_EDGEVALUE
    && (pos.x() + MIN_PADDING > point3.x() || pos.y() + MIN_PADDING > point2.y() ||
       pointLineDir(point3, point4, pos) == 1 || pointLineDir(point2, point4, pos) == 1)) {
        return newResizeFPoints;
    } else {
        if (pointToLineDistance(point4, point2, pos) < MIN_PADDING || pointToLineDistance(point3,
            point4, pos) < MIN_PADDING) {
            return newResizeFPoints;
        } else {
            if (!isShift) {
                if (pointLineDir(point1, point3, pos) == -1 && pointLineDir(point1, point2, pos) == -1) {
                    qreal distance = pointToLineDistance(point1, point2, pos);
                    QPointF add = pointSplid(point2, point4, distance);
                    point2 = QPointF(point2.x() - add.x(), point2.y() - add.y());
                    distance = pointToLineDistance(point1, point3, pos);
                    add = pointSplid(point3, point4, distance);
                    point3 = QPointF(point3.x() + add.x(), point3.y() - add.y());
                    point1 = pos;
                }
                if (pointLineDir(point1, point3, pos) == -1 && pointLineDir(point1, point2, pos) == 1) {
                    qreal distance = pointToLineDistance(point1, point2, pos);
                    QPointF add = pointSplid(point2, point4, distance);
                    point2 = QPointF(point2.x() + add.x(), point2.y() + add.y());
                    distance = pointToLineDistance(point1, point3, pos);
                    add = pointSplid(point3, point4, distance);
                    point3 = QPointF(point3.x() + add.x(), point3.y() - add.y());
                    point1 = pos;
                }
                if (pointLineDir(point1, point3, pos) == 1 && pointLineDir(point1, point2, pos) == 1) {
                    qreal distance = pointToLineDistance(point1, point2, pos);
                    QPointF add = pointSplid(point2, point4, distance);
                    point2 = QPointF(point2.x() + add.x(), point2.y() + add.y());
                    distance = pointToLineDistance(point1, point3, pos);
                    add = pointSplid(point3, point4, distance);
                    point3 = QPointF(point3.x() - add.x(), point3.y() + add.y());
                    point1 = pos;
                }
                if (pointLineDir(point1, point3, pos) == 1 && pointLineDir(point1, point2, pos) == -1) {
                    qreal distance = pointToLineDistance(point1, point2, pos);
                    QPointF add = pointSplid(point2, point4, distance);
                    point2 = QPointF(point2.x() - add.x(), point2.y() - add.y());
                    distance = pointToLineDistance(point1, point3, pos);
                    add = pointSplid(point3, point4, distance);
                    point3 = QPointF(point3.x() - add.x(), point3.y() + add.y());
                    point1 = pos;
                }
                newResizeFPoints.point1 = point1;
                newResizeFPoints.point2 = point2;
                newResizeFPoints.point3 = point3;
                newResizeFPoints.point4 = point4;
                return newResizeFPoints;
            } else {
                qreal distance1 = pointToLineDistance(point1, point2, pos);
                qreal distance2 = pointToLineDistance(point1, point3, pos);
                qreal distance = std::min(distance1, distance2);
                if (pointLineDir(point1, point3, pos) == 1 && pointLineDir(point1, point2, pos) == 1) {
                    QPointF add = pointSplid(point2, point4, distance);
                    point2 = QPointF(point2.x() + add.x(), point2.y() + add.y());
                    add = pointSplid(point3, point4, distance);
                    point3 = QPointF(point3.x() - add.x(), point3.y() + add.y());
                } else {
                    QPointF add = pointSplid(point2, point4, distance);
                    point2 = QPointF(point2.x() - add.x(), point2.y() - add.y());
                    add = pointSplid(point3, point4, distance);
                    point3 = QPointF(point3.x() + add.x(), point3.y() - add.y());
                }
                point1 = QPointF(point2.x() + point3.x() - point4.x(), point2.y() + point3.y() - point4.y());
                newResizeFPoints.point1 = point1;
                newResizeFPoints.point2 = point2;
                newResizeFPoints.point3 = point3;
                newResizeFPoints.point4 = point4;
                return newResizeFPoints;
            }
        }
        return newResizeFPoints;
    }
}

/* point1 in the fourth position */
FourPoints point1Resize4(QPointF point1, QPointF point2, QPointF point3,
                         QPointF point4, QPointF pos, bool isShift) {
    FourPoints newResizeFPoints;
    newResizeFPoints.point1 = point1;
    newResizeFPoints.point2 = point2;
    newResizeFPoints.point3 = point3;
    newResizeFPoints.point4 = point4;

    if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) >= -TANT2_EDGEVALUE
    && (pos.y() - MIN_PADDING < point2.y() || pos.x() - MIN_PADDING < point3.x() ||
    pointLineDir(point3, point4, pos) == 1 || pointLineDir(point2, point4, pos) == -1)) {
        return newResizeFPoints;
    } else if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) < - TANT2_EDGEVALUE
    && (pos.y() + MIN_PADDING > point3.y() || pos.x() - MIN_PADDING < point2.x() ||
    pointLineDir(point3, point4, pos) == 1 || pointLineDir(point2, point4, pos) == -1)) {
        return newResizeFPoints;
    } else {
        if (pointToLineDistance(point4, point2, pos) < MIN_PADDING ||
                pointToLineDistance(point3, point4, pos) < MIN_PADDING) {
            return newResizeFPoints;
        } else {
            if (!isShift) {
                if (pointLineDir(point1, point3, pos) == 1 && pointLineDir(point1, point2, pos) == -1) {
                    qreal distance = pointToLineDistance(point1, point2, pos);
                    QPointF add  = pointSplid(point2, point4, distance);
                    point2 = QPointF(point2.x() + add.x(), point2.y() - add.y());
                    distance  = pointToLineDistance(point1, point3, pos);
                    add = pointSplid(point3, point4, distance);
                    point3 = QPointF(point3.x() + add.x(), point3.y() + add.y());
                    point1 = pos;
                }
                if (pointLineDir(point1, point3, pos) == -1 && pointLineDir(point1, point2, pos) == -1) {
                    qreal distance = pointToLineDistance(point1, point2, pos);
                    QPointF add  = pointSplid(point2, point4, distance);
                    point2 = QPointF(point2.x() + add.x(), point2.y() - add.y());
                    distance  = pointToLineDistance(point1, point3, pos);
                    add = pointSplid(point3, point4, distance);
                    point3 = QPointF(point3.x() - add.x(), point3.y() - add.y());
                    point1 = pos;
                }
                if (pointLineDir(point1, point3, pos) == -1 && pointLineDir(point1, point2, pos) == 1) {
                    qreal distance = pointToLineDistance(point1, point2, pos);
                    QPointF add  = pointSplid(point2, point4, distance);
                    point2 = QPointF(point2.x() - add.x(), point2.y() + add.y());
                    distance  = pointToLineDistance(point1, point3, pos);
                    add = pointSplid(point3, point4, distance);
                    point3 = QPointF(point3.x() - add.x(), point3.y() - add.y());
                    point1 = pos;
                }
                if (pointLineDir(point1, point3, pos) == 1 && pointLineDir(point1, point2, pos) == 1) {
                    qreal distance = pointToLineDistance(point1, point2, pos);
                    QPointF add  = pointSplid(point2, point4, distance);
                    point2 = QPointF(point2.x() - add.x(), point2.y() + add.y());
                    distance  = pointToLineDistance(point1, point3, pos);
                    add = pointSplid(point3, point4, distance);
                    point3 = QPointF(point3.x() + add.x(), point3.y() + add.y());
                    point1 = pos;
                }
                newResizeFPoints.point1 =  point1;
                newResizeFPoints.point2 =  point2;
                newResizeFPoints.point3 =  point3;
                newResizeFPoints.point4 =  point4;
                return newResizeFPoints;
            } else {
                qreal distance1 = pointToLineDistance(point1, point2, pos);
                qreal distance2 = pointToLineDistance(point1, point3, pos);
                qreal distance = std::min(distance1, distance2);
               if (pointLineDir(point1, point3, pos) == -1 &&pointLineDir(point1, point2, pos) == 1) {
                   QPointF add = pointSplid(point2, point4, distance);
                   point2 = QPointF(point2.x() - add.x(), point2.y() + add.y());
                   add = pointSplid(point3, point4, distance);
                   point3 = QPointF(point3.x() - add.x(), point3.y() - add.y());
               } else {
                   QPointF add = pointSplid(point2, point4, distance);
                   point2 = QPointF(point2.x() + add.x(), point2.y() - add.y());
                   add = pointSplid(point3, point4, distance);
                   point3 = QPointF(point3.x() + add.x(), point3.y() + add.y());
               }
               point1 = QPointF(point2.x() + point3.x() - point4.x(),
                               point2.y() + point3.y() - point4.y());
               newResizeFPoints.point1 = point1;
               newResizeFPoints.point2 = point2;
               newResizeFPoints.point3 = point3;
               newResizeFPoints.point4 = point4;
               return newResizeFPoints;
            }
        }
    }
    return newResizeFPoints;
}

/* point1 in the fifth position */
FourPoints point1Resize5(QPointF point1, QPointF point2, QPointF point3,
                         QPointF point4, QPointF pos, bool isShift) {
    FourPoints newResizeFPoints;
    newResizeFPoints.point1 = point1;
    newResizeFPoints.point2 = point2;
    newResizeFPoints.point3 = point3;
    newResizeFPoints.point4 = point4;
    if (pos.y() + MIN_PADDING > point2.y() || pos.x() + MIN_PADDING > point3.x()) {
        return newResizeFPoints;
    } else {
        qreal distanceLeft = pointToLineDistance(point1, point2, pos);
        QPointF addLeft = pointSplid(point2, point4, distanceLeft);
        qreal distanceRight = pointToLineDistance(point1, point3, pos);
        QPointF addRight = pointSplid(point3, point4, distanceRight);
        if (isShift) {
            if (pointLineDir(point1, point3, pos) == -1 && pointLineDir(point1, point2, pos) == -1) {
                point2 = QPointF(point2.x() - addLeft.x(), point2.y() + addLeft.y());
                point3 = QPointF(point3.x() - addRight.x(), point3.y() - addRight.y());
                point1 = pos;
            }
            if (pointLineDir(point1, point3, pos) == -1 && pointLineDir(point1, point2, pos) == 1) {
                point2 = QPointF(point2.x() + addLeft.x(), point2.y() - addLeft.y());
                point3 = QPointF(point3.x() - addRight.x(), point3.y() - addRight.y());
                point1 = pos;
            }
            if (pointLineDir(point1, point3, pos) == 1 && pointLineDir(point1, point2, pos) == 1) {
                point2 = QPointF(point2.x() + addLeft.x(), point2.y() - addLeft.y());
                point3 = QPointF(point3.x() + addRight.x(), point3.y() + addRight.y());
                point1 = pos;
            }
            if (pointLineDir(point1, point3, pos) == 1 && pointLineDir(point1, point2, pos) == -1) {
                point2 = QPointF(point2.x() + addLeft.x(), point2.y() - addLeft.y());
                point3 = QPointF(point3.x() - addRight.x(), point3.y() - addRight.y());
                point1 = pos;
            }
        } else {
            qreal distance = std::min(distanceLeft, distanceRight);
            QPointF addLeft = pointSplid(point2, point4, distance);
            QPointF addRight = pointSplid(point3, point4, distance);
            if (pointLineDir(point1, point3, pos) == 1 && pointLineDir(point1, point2, pos) == 1) {
                point2 = QPointF(point2.x() + addLeft.x(), point2.y() +  addLeft.y());
                point3 = QPointF(point3.x() + addRight.x(), point3.y() + addRight.y());
            } else {
                point2 = QPointF(point2.x() - addLeft.x(), point2.y() -  addLeft.y());
                point3 = QPointF(point3.x() - addRight.x(), point3.y() - addRight.y());
            }
            point1 = QPointF(point2.x() + point3.x() - point4.x(), point2.y() + point3.y() - point4.y());
            newResizeFPoints.point1 = point1;
            newResizeFPoints.point2 = point2;
            newResizeFPoints.point3 = point3;
            newResizeFPoints.point4 = point4;
            return newResizeFPoints;
        }
    }
    return newResizeFPoints;
}

/* point1 in the sixth position */
FourPoints point1Resize6(QPointF point1, QPointF point2, QPointF point3,
                         QPointF point4, QPointF pos, bool isShift) {
    Q_UNUSED(pos);
    Q_UNUSED(isShift);
    FourPoints newResizeFPoints;
    newResizeFPoints.point1 = point1;
    newResizeFPoints.point2 = point2;
    newResizeFPoints.point3 = point3;
    newResizeFPoints.point4 = point4;
    return newResizeFPoints;
}

/* point1 in the seventh position */
FourPoints point1Resize7(QPointF point1, QPointF point2, QPointF point3,
                         QPointF point4, QPointF pos, bool isShift) {
    Q_UNUSED(pos);
    Q_UNUSED(isShift);
    FourPoints newResizeFPoints;
    newResizeFPoints.point1 = point1;
    newResizeFPoints.point2 = point2;
    newResizeFPoints.point3 = point3;
    newResizeFPoints.point4 = point4;
    return newResizeFPoints;
}

/***************** second point2 *******************/
/* point2 in the first position */
FourPoints point2Resize1(QPointF point1, QPointF point2, QPointF point3,
                         QPointF point4, QPointF pos, bool isShift) {
    FourPoints newResizeFPoints;
    newResizeFPoints.point1 = point1;
    newResizeFPoints.point2 = point2;
    newResizeFPoints.point3 = point3;
    newResizeFPoints.point4 = point4;

    if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) >= TANT_EDGEVALUE
    && (pos.x() + MIN_PADDING > point4.x() || pos.y() - MIN_PADDING < point1.y() ||
    pointLineDir(point1, point3, pos) == -1 || pointLineDir(point3, point4, pos) == -1)) {
        return newResizeFPoints;
    } else if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) < TANT_EDGEVALUE
    && (pos.x() - MIN_PADDING < point1.x() || pos.y() - MIN_PADDING < point4.y() ||
        pointLineDir(point1, point3, pos) == -1 || pointLineDir(point3, point4, pos) == -1)) {
        return newResizeFPoints;
    } else {
        if (pointToLineDistance(point1, point3, pos) < MIN_PADDING ||
                pointToLineDistance(point3, point4, pos) < MIN_PADDING) {
            return newResizeFPoints;
        } else {
            if (!isShift) {
                if (pointLineDir(point1, point2, pos) == 1 && pointLineDir(point2, point4, pos) == 1) {
                    qreal distance = pointToLineDistance(point1, point2, pos);
                    QPointF add = pointSplid(point1, point3, distance);
                    point1 = QPointF(point1.x() - add.x(), point1.y() + add.y());
                    distance = pointToLineDistance(point2, point4, pos);
                    add = pointSplid(point3, point4, distance);
                    point4 = QPointF(point4.x() + add.x(), point4.y() + add.y());
                    point2 = pos;
                }
                if (pointLineDir(point1, point2, pos) == 1 && pointLineDir(point2, point4, pos) == -1) {
                    qreal distance = pointToLineDistance(point1, point2, pos);
                    QPointF add = pointSplid(point1, point3, distance);
                    point1 = QPointF(point1.x() - add.x(), point1.y() + add.y());
                    distance = pointToLineDistance(point2, point4, pos);
                    add = pointSplid(point3, point4, distance);
                    point4 = QPointF(point4.x() - add.x(), point4.y() - add.y());
                    point2 = pos;
                }
                if (pointLineDir(point1, point2, pos) == -1 && pointLineDir(point2,point4, pos) == -1) {
                    qreal distance = pointToLineDistance(point1, point2, pos);
                    QPointF add = pointSplid(point1, point3, distance);
                    point1 = QPointF(point1.x() + add.x(), point1.y() - add.y());
                    distance = pointToLineDistance(point2, point4, pos);
                    add = pointSplid(point3, point4, distance);
                    point4 = QPointF(point4.x() - add.x(), point4.y() - add.y());
                    point2 = pos;
                }
                if (pointLineDir(point1, point2, pos) == -1 && pointLineDir(point2, point4, pos) == 1) {
                    qreal distance = pointToLineDistance(point1, point2, pos);
                    QPointF add = pointSplid(point1, point3, distance);
                    point1 = QPointF(point1.x() + add.x(), point1.y() - add.y());
                    distance = pointToLineDistance(point2, point4, pos);
                    add = pointSplid(point3, point4, distance);
                    point4 = QPointF(point4.x() + add.x(), point4.y() + add.y());
                    point2 = pos;
                }
                newResizeFPoints.point1 = point1;
                newResizeFPoints.point2 = point2;
                newResizeFPoints.point3 = point3;
                newResizeFPoints.point4 = point4;
                return newResizeFPoints;
            } else {
                qreal distance1 = pointToLineDistance(point1, point2, pos);
                qreal distance2 = pointToLineDistance(point2, point4, pos);
                qreal distance = std::min(distance1, distance2);
                if (pointLineDir(point1, point2, pos) == -1 && pointLineDir(point2, point4, pos) == -1) {
                    QPointF add = pointSplid(point1, point3, distance);
                    point1 = QPointF(point1.x() + add.x(), point1.y() - add.y());
                    add = pointSplid(point3, point4, distance);
                    point4 = QPointF(point4.x() - add.x(), point4.y() - add.y());
                    point2 = QPointF(point1.x() + point4.x() - point3.x(), point1.y() + point4.y() - point3.y());
                } else {
                    QPointF add = pointSplid(point1, point3, distance);
                    point1 = QPointF(point1.x() + add.x(), point1.y() - add.y());
                    add = pointSplid(point3, point4, distance);
                    point4 = QPointF(point4.x() - add.x(), point4.y() - add.y());
                    point2 = QPointF(point1.x() + point4.x() - point3.x(), point1.y() + point4.y() - point3.y());
                }
                newResizeFPoints.point1 = point1;
                newResizeFPoints.point2 = point2;
                newResizeFPoints.point3 = point3;
                newResizeFPoints.point4 = point4;
                return newResizeFPoints;
            }
        }
    }
    return newResizeFPoints;
}

/* point2 in the second position */
FourPoints point2Resize2(QPointF point1, QPointF point2, QPointF point3,
                         QPointF point4, QPointF pos, bool isShift) {
    FourPoints newResizeFPoints;
    newResizeFPoints.point1 = point1;
    newResizeFPoints.point2 = point2;
    newResizeFPoints.point3 = point3;
    newResizeFPoints.point4 = point4;

    if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) <= -TANT_EDGEVALUE
    && (pos.y() + MIN_PADDING > point1.x() || pos.x() - MIN_PADDING < point4.x() ||
    pointLineDir(point1, point3, pos) == 1 || pointLineDir(point3, point4, pos) == -1)) {
        return newResizeFPoints;
    } else if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) > -TANT_EDGEVALUE
    && (pos.y() - MIN_PADDING < point4.y() || pos.x() - MIN_PADDING < point1.x() ||
        pointLineDir(point1, point3, pos) == 1 || pointLineDir(point3, point4, pos) == -1)) {
        return newResizeFPoints;
    } else {
        if (pointToLineDistance(point1, point3, pos) < MIN_PADDING ||
                pointToLineDistance(point3, point4, pos) < MIN_PADDING) {
            return newResizeFPoints;
        } else {         
            if (!isShift) {
                if (pointLineDir(point1, point2, pos) == 1 && pointLineDir(point2, point4, pos) == -1) {
                    qreal distance = pointToLineDistance(point1, point2, pos);
                    QPointF add= pointSplid(point1, point3, distance);
                    point1 = QPointF(point1.x() + add.x(), point1.y() + add.y());
                    distance = pointToLineDistance(point2, point4, pos);
                    add = pointSplid(point3, point4, distance);
                    point4 = QPointF(point4.x() + add.x(), point4.y() - add.y());
                    point2 = pos;
                }
                if (pointLineDir(point1, point2, pos) == 1 && pointLineDir(point2, point4, pos) == 1) {
                    qreal distance = pointToLineDistance(point1, point2, pos);
                    QPointF add= pointSplid(point1, point3, distance);
                    point1 = QPointF(point1.x() + add.x(), point1.y() + add.y());
                    distance = pointToLineDistance(point2, point4, pos);
                    add = pointSplid(point3, point4, distance);
                    point4 = QPointF(point4.x() - add.x(), point4.y() + add.y());
                    point2 = pos;
                }
                if (pointLineDir(point1, point2, pos) == -1 && pointLineDir(point2, point4, pos) == 1) {
                    qreal distance = pointToLineDistance(point1, point2, pos);
                    QPointF add= pointSplid(point1, point3, distance);
                    point1 = QPointF(point1.x() - add.x(), point1.y() - add.y());
                    distance = pointToLineDistance(point2, point4, pos);
                    add = pointSplid(point3, point4, distance);
                    point4 = QPointF(point4.x() - add.x(), point4.y() + add.y());
                    point2 = pos;
                }
                if (pointLineDir(point1, point2, pos) == -1 && pointLineDir(point2, point4, pos) == -1) {
                    qreal distance = pointToLineDistance(point1, point2, pos);
                    QPointF add= pointSplid(point1, point3, distance);
                    point1 = QPointF(point1.x() - add.x(), point1.y() - add.y());
                    distance = pointToLineDistance(point2, point4, pos);
                    add = pointSplid(point3, point4, distance);
                    point4 = QPointF(point4.x() + add.x(), point4.y() - add.y());
                    point2 = pos;
                }
                newResizeFPoints.point1 = point1;
                newResizeFPoints.point2 = point2;
                newResizeFPoints.point3 = point3;
                newResizeFPoints.point4 = point4;
                return newResizeFPoints;
            } else {
                qreal distance1 = pointToLineDistance(point1, point2, pos);
                qreal distance2 = pointToLineDistance(point2, point4, pos);
                qreal distance = std::min(distance1, distance2);
                if (pointLineDir(point1, point2, pos) == -1 && pointLineDir(point2, point4, pos) == 1) {
                    QPointF add = pointSplid(point1, point3, distance);
                    point1 = QPointF(point1.x() - add.x(), point1.y() - add.y());
                    add = pointSplid(point3, point4, distance);
                    point4 = QPointF(point4.x() - add.x(), point4.y() + add.y());
                } else {
                    QPointF add = pointSplid(point1, point3, distance);
                    point1 = QPointF(point1.x() + add.x(), point1.y() + add.y());
                    add = pointSplid(point3, point4, distance);
                    point4 = QPointF(point4.x() + add.x(), point4.y() - add.y());
                }
                point2 = QPointF(point1.x() + point4.x() - point3.x(), point1.y() + point4.y() - point3.y());
                newResizeFPoints.point1 = point1;
                newResizeFPoints.point2 = point2;
                newResizeFPoints.point3 = point3;
                newResizeFPoints.point4 = point4;
                return newResizeFPoints;
            }
        }
    }
    return newResizeFPoints;
}

/* point2 in the third position */
FourPoints point2Resize3(QPointF point1, QPointF point2, QPointF point3,
                         QPointF point4, QPointF pos, bool isShift) {
    FourPoints newResizeFPoints;
    newResizeFPoints.point1 = point1;
    newResizeFPoints.point2 = point2;
    newResizeFPoints.point3 = point3;
    newResizeFPoints.point4 = point4;

    if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) >= TANT2_EDGEVALUE
    && (pos.y() + MIN_PADDING > point4.y() || pos.x() + MIN_PADDING > point1.x() ||
    pointLineDir(point1, point3, pos) == -1 || pointLineDir(point3, point4, pos) == 1)) {
        return newResizeFPoints;
    } else if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) < TANT2_EDGEVALUE
    && (pos.y() - MIN_PADDING < point1.y() || pos.x() + MIN_PADDING > point4.x() ||
        pointLineDir(point1, point3, pos) == -1 || pointLineDir(point3, point4, pos) == 1)) {
        return newResizeFPoints;
    } else {
        if (pointToLineDistance(point1, point3, pos) < MIN_PADDING ||
                pointToLineDistance(point3, point4, pos) < MIN_PADDING) {
            return newResizeFPoints;
        } else {
            if (!isShift) {
                if (pointLineDir(point1, point2, pos) == -1 && pointLineDir(point2, point4, pos) == 1) {
                    qreal distance = pointToLineDistance(point1, point2, pos);
                    QPointF add = pointSplid(point1, point3, distance);
                    point1 = QPointF(point1.x() - add.x(), point1.y() - add.y());
                    distance = pointToLineDistance(point2, point4, pos);
                    add = pointSplid(point3, point4, distance);
                    point4 = QPointF(point4.x() - add.x(), point4.y() + add.y());
                    point2 = pos;
                }
                if (pointLineDir(point1, point2, pos) == -1 && pointLineDir(point2, point4, pos) == -1) {
                    qreal distance = pointToLineDistance(point1, point2, pos);
                    QPointF add = pointSplid(point1, point3, distance);
                    point1 = QPointF(point1.x() - add.x(), point1.y() - add.y());
                    distance = pointToLineDistance(point2, point4, pos);
                    add = pointSplid(point3, point4, distance);
                    point4 = QPointF(point4.x() + add.x(), point4.y() - add.y());
                    point2 = pos;
                }
                if (pointLineDir(point1, point2, pos) == 1 && pointLineDir(point2, point4, pos) == -1) {
                    qreal distance = pointToLineDistance(point1, point2, pos);
                    QPointF add = pointSplid(point1, point3, distance);
                    point1 = QPointF(point1.x() + add.x(), point1.y() + add.y());
                    distance = pointToLineDistance(point2, point4, pos);
                    add = pointSplid(point3, point4, distance);
                    point4 = QPointF(point4.x() + add.x(), point4.y() - add.y());
                    point2 = pos;
                }
                if (pointLineDir(point1, point2, pos) == 1&& pointLineDir(point2, point4, pos) == 1) {
                    qreal distance = pointToLineDistance(point1, point2, pos);
                    QPointF add = pointSplid(point1, point3, distance);
                    point1 = QPointF(point1.x() + add.x(), point1.y() + add.y());
                    distance = pointToLineDistance(point2, point4, pos);
                    add = pointSplid(point3, point4, distance);
                    point4 = QPointF(point4.x() - add.x(), point4.y() + add.y());
                    point2 = pos;
                }
                newResizeFPoints.point1 = point1;
                newResizeFPoints.point2 = point2;
                newResizeFPoints.point3 = point3;
                newResizeFPoints.point4 = point4;
                return newResizeFPoints;
            } else {
                qreal distance1 = pointToLineDistance(point1, point2, pos);
                qreal distance2 = pointToLineDistance(point2, point4, pos);
                qreal distance = std::min(distance1, distance2);
                if (pointLineDir(point1, point2, pos) == 1 && pointLineDir(point2, point4, pos) == -1) {
                    QPointF add = pointSplid(point1, point3, distance);
                    point1 = QPointF(point1.x() + add.x(), point1.y() + add.y());
                    add = pointSplid(point3, point4, distance);
                    point4 = QPointF(point4.x() + add.x(), point4.y() - add.y());
                } else {
                    QPointF add = pointSplid(point1, point3, distance);
                    point1 = QPointF(point1.x() - add.x(), point1.y() - add.y());
                    add = pointSplid(point3, point4, distance);
                    point4 = QPointF(point4.x() - add.x(), point4.y() + add.y());
                }
                point2 = QPointF(point1.x() + point4.x() - point3.x(), point1.y() + point4.y() - point3.y());
                newResizeFPoints.point1 = point1;
                newResizeFPoints.point2 = point2;
                newResizeFPoints.point3 = point3;
                newResizeFPoints.point4 = point4;
                return newResizeFPoints;
            }
        }
    }
    return newResizeFPoints;
}

/* point2 in the fourth position */
FourPoints point2Resize4(QPointF point1, QPointF point2, QPointF point3,
                         QPointF point4, QPointF pos, bool isShift) {
    FourPoints newResizeFPoints;
    newResizeFPoints.point1 = point1;
    newResizeFPoints.point2 = point2;
    newResizeFPoints.point3 = point3;
    newResizeFPoints.point4 = point4;

    if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) >= -TANT2_EDGEVALUE
    && (pos.y() + MIN_PADDING > point1.y() || pos.x() - MIN_PADDING < point4.x() ||
    pointLineDir(point1, point3, pos) == 1 || pointLineDir(point3, point4, pos) == 1)) {
        return newResizeFPoints;
    } else if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) < -TANT2_EDGEVALUE
    && (pos.y() + MIN_PADDING > point4.y() || pos.x() + MIN_PADDING > point1.x() ||
        pointLineDir(point1, point3, pos) == 1 || pointLineDir(point3, point4, pos) == 1)) {
        return newResizeFPoints;
    } else {
        if (pointToLineDistance(point1, point3, pos) < MIN_PADDING ||
                pointToLineDistance(point3, point4, pos) < MIN_PADDING) {
            return newResizeFPoints;
        } else {
            if (!isShift) {
                if (pointLineDir(point1, point2, pos) == 1 && pointLineDir(point2, point4, pos) == 1) {
                    qreal distance = pointToLineDistance(point1, point2, pos);
                    QPointF add = pointSplid(point1, point3, distance);
                    point1 = QPointF(point1.x() - add.x(), point1.y() + add.y());
                    distance = pointToLineDistance(point2, point4, pos);
                    add = pointSplid(point3, point4, distance);
                    point4 = QPointF(point4.x() + add.x(), point4.y() + add.y());
                    point2 = pos;
                }
                if (pointLineDir(point1, point2, pos) == 1 && pointLineDir(point2, point4, pos) == -1) {
                    qreal distance = pointToLineDistance(point1, point2, pos);
                    QPointF add = pointSplid(point1, point3, distance);
                    point1 = QPointF(point1.x() - add.x(), point1.y() + add.y());
                    distance = pointToLineDistance(point2, point4, pos);
                    add = pointSplid(point3, point4, distance);
                    point4 = QPointF(point4.x() - add.x(), point4.y() - add.y());
                    point2 = pos;
                }
                if (pointLineDir(point1, point2, pos) == -1 && pointLineDir(point2, point4, pos) == 1) {
                    qreal distance = pointToLineDistance(point1, point2, pos);
                    QPointF add = pointSplid(point1, point3, distance);
                    point1 = QPointF(point1.x() + add.x(), point1.y() - add.y());
                    distance = pointToLineDistance(point2, point4, pos);
                    add = pointSplid(point3, point4, distance);
                    point4 = QPointF(point4.x() + add.x(), point4.y() + add.y());
                    point2 = pos;
                }
                if (pointLineDir(point1, point2, pos) == -1 && pointLineDir(point2, point4, pos) == -1) {
                    qreal distance = pointToLineDistance(point1, point2, pos);
                    QPointF add = pointSplid(point1, point3, distance);
                    point1 = QPointF(point1.x() + add.x(), point1.y() - add.y());
                    distance = pointToLineDistance(point2, point4, pos);
                    add = pointSplid(point3, point4, distance);
                    point4 = QPointF(point4.x() - add.x(), point4.y() - add.y());
                    point2 = pos;
                }
                newResizeFPoints.point1 = point1;
                newResizeFPoints.point2 = point2;
                newResizeFPoints.point3 = point3;
                newResizeFPoints.point4 = point4;
                return newResizeFPoints;
            } else {
                qreal distance1 = pointToLineDistance(point1, point2, pos);
                qreal distance2 = pointToLineDistance(point2, point4, pos);
                qreal distance = std::min(distance1, distance2);
                if (pointLineDir(point1, point2, pos) == 1 && pointLineDir(point2, point4, pos) == 1) {
                    QPointF add = pointSplid(point1, point3, distance);
                    point1 = QPointF(point1.x() - add.x(), point1.y() + add.y());
                    add = pointSplid(point3, point4, distance);
                    point4 = QPointF(point4.x() + add.x(), point4.y() + add.y());
                } else {
                    QPointF add = pointSplid(point1, point3, distance);
                    point1 = QPointF(point1.x() + add.x(), point1.y() - add.y());
                    add = pointSplid(point3, point4, distance);
                    point4 = QPointF(point4.x() - add.x(), point4.y() - add.y());
                }
                point2 = QPointF(point1.x() + point4.x() - point3.x(),
                                point1.y() + point4.y() - point3.y());
                newResizeFPoints.point1 = point1;
                newResizeFPoints.point2 = point2;
                newResizeFPoints.point3 = point3;
                newResizeFPoints.point4 = point4;
                return newResizeFPoints;
            }
        }
    }
    return newResizeFPoints;
}

/* point2 in the fifth position*/
FourPoints point2Resize5(QPointF point1, QPointF point2, QPointF point3,
                         QPointF point4, QPointF pos, bool isShift) {
    FourPoints newResizeFPoints;
    newResizeFPoints.point1 = point1;
    newResizeFPoints.point2 = point2;
    newResizeFPoints.point3 = point3;
    newResizeFPoints.point4 = point4;

    if (pos.y() - MIN_PADDING < point1.y() || pos.x() + MIN_PADDING > point4.x()) {
        return newResizeFPoints;
    } else {
        if (!isShift) {
            point1.setX(pos.x());
            point4.setY(pos.y());
            point2 = pos;
            newResizeFPoints.point1 = point1;
            newResizeFPoints.point2 = point2;
            newResizeFPoints.point3 = point3;
            newResizeFPoints.point4 = point4;
            return newResizeFPoints;
        } else {
            qreal distance1 = pointToLineDistance(point1, point2, pos);
            qreal distance2 = pointToLineDistance(point2, point4, pos);
            qreal distance = std::min(distance1, distance2);
            if (pointLineDir(point1, point2, pos) == 1 && pointLineDir(point2, point4, pos) == -1) {
                QPointF add = pointSplid(point1, point3, distance);
                point1 = QPointF(point1.x() + add.x(), point1.y() - add.y());
                add = pointSplid(point3, point4, distance);
                point4 = QPointF(point4.x() - add.x(), point4.y() - add.y());
            } else {
                QPointF add = pointSplid(point1, point3, distance);
                point1 = QPointF(point1.x() - add.x(), point1.y() + add.y());
                add = pointSplid(point3, point4, distance);
                point4 = QPointF(point4.x() + add.x(), point4.y() + add.y());
            }
            point2 = QPointF(point1.x() + point4.x() - point3.x(),
                             point1.y() + point4.y() - point3.y());
            newResizeFPoints.point1 = point1;
            newResizeFPoints.point2 = point2;
            newResizeFPoints.point3 = point3;
            newResizeFPoints.point4 = point4;
            return newResizeFPoints;
        }
    }
    return newResizeFPoints;
}

/* point2 in the sixth position */
FourPoints point2Resize6(QPointF point1, QPointF point2, QPointF point3,
                         QPointF point4, QPointF pos, bool isShift) {
    Q_UNUSED(pos);
    Q_UNUSED(isShift);
    FourPoints newResizeFPoints;
    newResizeFPoints.point1 = point1;
    newResizeFPoints.point2 = point2;
    newResizeFPoints.point3 = point3;
    newResizeFPoints.point4 = point4;
    return newResizeFPoints;
}

/* point2 in the seventh position */
FourPoints point2Resize7(QPointF point1, QPointF point2, QPointF point3,
                         QPointF point4, QPointF pos, bool isShift) {
    Q_UNUSED(pos);
    Q_UNUSED(isShift);
    FourPoints newResizeFPoints;
    newResizeFPoints.point1 = point1;
    newResizeFPoints.point2 = point2;
    newResizeFPoints.point3 = point3;
    newResizeFPoints.point4 = point4;
    return newResizeFPoints;
}

/***************** third point3 *******************/
/* point3 in the first position */
FourPoints point3Resize1(QPointF point1, QPointF point2, QPointF point3,
                         QPointF point4, QPointF pos, bool isShift) {
    FourPoints newResizeFPoints;
    newResizeFPoints.point1 = point1;
    newResizeFPoints.point2 = point2;
    newResizeFPoints.point3 = point3;
    newResizeFPoints.point4 = point4;
    if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) >= TANT_EDGEVALUE &&
    (pos.x() - MIN_PADDING < point1.x() || pos.y() + MIN_PADDING > point4.y() ||
     pointLineDir(point1, point2, pos) == 1 || pointLineDir(point2, point4, pos) == 1)) {
        return newResizeFPoints;
    } else if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) < TANT_EDGEVALUE &&
     (pos.x() + MIN_PADDING > point4.x() || pos.y() + MIN_PADDING > point1.y() ||
      pointLineDir(point1, point2, pos) == 1 || pointLineDir(point2, point4, pos) == 1)) {
        return newResizeFPoints;
    } else {
        if (pointToLineDistance(point2, point4, pos) < MIN_PADDING ||
                pointToLineDistance(point1, point2, pos) < MIN_PADDING) {
            return newResizeFPoints;
        } else {
            if (!isShift) {
                if (pointLineDir(point1, point3, pos) == -1 && pointLineDir(point3, point4, pos) == -1) {
                    qreal distance = pointToLineDistance(point3, point4, pos);
                    QPointF add = pointSplid(point2, point4, distance);
                    point4 = QPointF(point4.x() + add.x(), point4.y() - add.y());
                    distance = pointToLineDistance(point1, point3, pos);
                    add = pointSplid(point1, point2, distance);
                    point1 = QPointF(point1.x() - add.x(), point1.y() - add.y());
                    point3 = pos;
                }
                if (pointLineDir(point1, point3, pos) == -1 && pointLineDir(point3, point4, pos) == 1) {
                    qreal distance = pointToLineDistance(point3, point4, pos);
                    QPointF add = pointSplid(point2, point4, distance);
                    point4 = QPointF(point4.x() - add.x(), point4.y() + add.y());
                    distance = pointToLineDistance(point1, point3, pos);
                    add = pointSplid(point1, point2, distance);
                    point1 = QPointF(point1.x() - add.x(), point1.y() - add.y());
                    point3 = pos;
                }
                if (pointLineDir(point1, point3, pos) == 1 && pointLineDir(point3, point4, pos) == 1) {
                    qreal distance = pointToLineDistance(point3, point4, pos);
                    QPointF add = pointSplid(point2, point4, distance);
                    point4 = QPointF(point4.x() - add.x(), point4.y() + add.y());
                    distance = pointToLineDistance(point1, point3, pos);
                    add = pointSplid(point1, point2, distance);
                    point1 = QPointF(point1.x() + add.x(), point1.y() + add.y());
                    point3 = pos;
                }
                if (pointLineDir(point1, point3, pos) == 1 && pointLineDir(point3, point4, pos) == -1) {
                    qreal distance = pointToLineDistance(point3, point4, pos);
                    QPointF add = pointSplid(point2, point4, distance);
                    point4 = QPointF(point4.x() + add.x(), point4.y() - add.y());
                    distance = pointToLineDistance(point1, point3, pos);
                    add = pointSplid(point1, point2, distance);
                    point1 = QPointF(point1.x() + add.x(), point1.y() + add.y());
                    point3 = pos;
                }
                newResizeFPoints.point1 = point1;
                newResizeFPoints.point2 = point2;
                newResizeFPoints.point3 = point3;
                newResizeFPoints.point4 = point4;
                return newResizeFPoints;
            } else {
                qreal distance1 = pointToLineDistance(point1, point3, pos);
                qreal distance2 = pointToLineDistance(point3, point4, pos);
                qreal distance = std::min(distance1, distance2);
                if (pointLineDir(point1, point3, pos) == 1 && pointLineDir(point3, point4, pos) == 1) {
                    QPointF add = pointSplid(point1, point2, distance);
                    point1 = QPointF(point1.x() + add.x(), point1.y() + add.y());
                    add = pointSplid(point2, point4, distance);
                    point4 = QPointF(point4.x() - add.x(), point4.y() + add.y());
                    point3 = QPointF(point1.x() + point4.x() - point2.x(), point1.y() + point4.y() - point2.y());
                } else {
                    QPointF add = pointSplid(point1, point2, distance);
                    point1 = QPointF(point1.x() - add.x(), point1.y() - add.y());
                    add = pointSplid(point2, point4, distance);
                    point4 = QPointF(point4.x() + add.x(), point4.y() - add.y());
                    point3 = QPointF(point1.x() + point4.x() - point2.x(), point1.y() + point4.y() - point2.y());
                    point3 = QPointF(point1.x() + point4.x() - point2.x(), point1.y() + point4.y() - point2.y());
                }
                newResizeFPoints.point1 =  point1;
                newResizeFPoints.point2 = point2;
                newResizeFPoints.point3 = point3;
                newResizeFPoints.point4 = point4;
                return newResizeFPoints;
            }
        }
    }
    return newResizeFPoints;
}

/* point3 in the second position */
FourPoints point3Resize2(QPointF point1, QPointF point2, QPointF point3,
                         QPointF point4, QPointF pos, bool isShift) {
    FourPoints newResizeFPoints;
    newResizeFPoints.point1 = point1;
    newResizeFPoints.point2 = point2;
    newResizeFPoints.point3 = point3;
    newResizeFPoints.point4 = point4;
    if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) <= -TANT_EDGEVALUE &&
    (pos.x() + MIN_PADDING > point1.x() || pos.y() - MIN_PADDING < point4.y() ||
     pointLineDir(point1, point2, pos) == 1 || pointLineDir(point2, point4, pos) == -1)) {
        return newResizeFPoints;
    } else if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) > -TANT_EDGEVALUE &&
     (pos.x() + MIN_PADDING > point4.x() || pos.y() + MIN_PADDING > point1.y() ||
      pointLineDir(point1, point2, pos) == 1 || pointLineDir(point2, point4, pos) == -1)) {
        return newResizeFPoints;
    } else {
        if (pointToLineDistance(point2, point4, pos) < MIN_PADDING ||
                pointToLineDistance(point1, point2, pos) < MIN_PADDING) {
            return newResizeFPoints;
        } else {     
            if (!isShift) {
                if (pointLineDir(point1, point3, pos) == 1 && pointLineDir(point3, point4, pos) == -1) {
                    qreal distance = pointToLineDistance(point3, point4, pos);
                    QPointF add = pointSplid(point2, point4, distance);
                    point4 = QPointF(point4.x() - add.x(), point4.y() - add.y());
                    distance = pointToLineDistance(point1, point3, pos);
                    add = pointSplid(point1, point2, distance);
                    point1 = QPointF(point1.x() - add.x(), point1.y() + add.y());
                    point3 = pos;
                }
                if (pointLineDir(point1, point3, pos) == -1 && pointLineDir(point3, point4, pos) == -1) {
                    qreal distance = pointToLineDistance(point3, point4, pos);
                    QPointF add = pointSplid(point2, point4, distance);
                    point4 = QPointF(point4.x() - add.x(), point4.y() - add.y());
                    distance = pointToLineDistance(point1, point3, pos);
                    add = pointSplid(point1, point2, distance);
                    point1 = QPointF(point1.x() + add.x(), point1.y() - add.y());
                    point3 = pos;
                }
                if (pointLineDir(point1, point3, pos) == -1 && pointLineDir(point3, point4, pos) == 1) {
                    qreal distance = pointToLineDistance(point3, point4, pos);
                    QPointF add = pointSplid(point2, point4, distance);
                    point4 = QPointF(point4.x() + add.x(), point4.y() + add.y());
                    distance = pointToLineDistance(point1, point3, pos);
                    add = pointSplid(point1, point2, distance);
                    point1 = QPointF(point1.x() + add.x(), point1.y() - add.y());
                    point3 = pos;
                }
                if (pointLineDir(point1, point3, pos) == 1 && pointLineDir(point3, point4, pos) == 1) {
                    qreal distance = pointToLineDistance(point3, point4, pos);
                    QPointF add = pointSplid(point2, point4, distance);
                    point4 = QPointF(point4.x() + add.x(), point4.y() + add.y());
                    distance = pointToLineDistance(point1, point3, pos);
                    add = pointSplid(point1, point2, distance);
                    point1 = QPointF(point1.x() - add.x(), point1.y() + add.y());
                    point3 = pos;
                }
                newResizeFPoints.point1 = point1;
                newResizeFPoints.point2 = point2;
                newResizeFPoints.point3 = point3;
                newResizeFPoints.point4 = point4;
                return newResizeFPoints;
            } else {
                qreal distance1 = pointToLineDistance(point1, point3, pos);
                qreal distance2 = pointToLineDistance(point3, point4, pos);
                qreal distance = std::min(distance1, distance2);
                if (pointLineDir(point1, point3, pos) == -1 && pointLineDir(point3, point4, pos) == 1) {
                    QPointF add = pointSplid(point1, point2, distance);
                    point1 = QPointF(point1.x() + add.x(), point2.y() - add.y());
                    add = pointSplid(point2, point4, distance);
                    point4 = QPointF(point4.x() + add.x(), point4.y() + add.y());
                } else {
                    QPointF add = pointSplid(point1, point2, distance);
                    point1 = QPointF(point1.x() + add.x(), point2.y() - add.y());
                    add = pointSplid(point2, point4, distance);
                    point4 = QPointF(point4.x() + add.x(), point4.y() + add.y());
                }
                point3 = QPointF(point1.x() + point4.x() - point2.x(), point1.y() + point4.y() - point2.y());
                newResizeFPoints.point1 =  point1;
                newResizeFPoints.point2 = point2;
                newResizeFPoints.point3 = point3;
                newResizeFPoints.point4 = point4;
                return newResizeFPoints;
            }
        }
    }
    return newResizeFPoints;
}

/* point3 in the third position */
FourPoints point3Resize3(QPointF point1, QPointF point2, QPointF point3,
                         QPointF point4, QPointF pos, bool isShift) {
    FourPoints newResizeFPoints;
    newResizeFPoints.point1 = point1;
    newResizeFPoints.point2 = point2;
    newResizeFPoints.point3 = point3;
    newResizeFPoints.point4 = point4;

    if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) >= TANT2_EDGEVALUE &&
    (pos.x() - MIN_PADDING < point4.x() || pos.y() - MIN_PADDING < point1.y() ||
     pointLineDir(point1, point2, pos) == -1 || pointLineDir(point2, point4, pos) == 1)) {
        return newResizeFPoints;
    } else if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) < TANT2_EDGEVALUE &&
     (pos.x() - MIN_PADDING < point1.x() || pos.y() + MIN_PADDING > point4.y() ||
      pointLineDir(point1, point2, pos) == -1 || pointLineDir(point2, point4, pos) == 1)) {
        return newResizeFPoints;
    } else {
        if (pointToLineDistance(point2, point4, pos) < MIN_PADDING ||
                pointToLineDistance(point1, point2, pos) < MIN_PADDING) {
            return newResizeFPoints;
        } else {
            if (!isShift) {
                if (pointLineDir(point1, point3, pos) == -1 && pointLineDir(point3, point4, pos) == 1) {
                    qreal distance = pointToLineDistance(point3, point4, pos);
                    QPointF add = pointSplid(point2, point4, distance);
                    point4 = QPointF(point4.x() + add.x(), point4.y() + add.y());
                    distance = pointToLineDistance(point1, point3, pos);
                    add = pointSplid(point1, point2, distance);
                    point1 = QPointF(point1.x() + add.x(), point1.y() - add.y());
                    point3 = pos;
                }
                if (pointLineDir(point1, point3, pos) == -1 && pointLineDir(point3, point4, pos) == -1) {
                    qreal distance = pointToLineDistance(point3, point4, pos);
                    QPointF add = pointSplid(point2, point4, distance);
                    point4 = QPointF(point4.x() - add.x(), point4.y() - add.y());
                    distance = pointToLineDistance(point1, point3, pos);
                    add = pointSplid(point1, point2, distance);
                    point1 = QPointF(point1.x() + add.x(), point1.y() - add.y());
                    point3 = pos;
                }
                if (pointLineDir(point1, point3, pos) == 1 && pointLineDir(point3, point4, pos) == -1) {
                    qreal distance = pointToLineDistance(point3, point4, pos);
                    QPointF add = pointSplid(point2, point4, distance);
                    point4 = QPointF(point4.x() - add.x(), point4.y() - add.y());
                    distance = pointToLineDistance(point1, point3, pos);
                    add = pointSplid(point1, point2, distance);
                    point1 = QPointF(point1.x() - add.x(), point1.y() + add.y());
                    point3 = pos;
                }
                if (pointLineDir(point1, point3, pos) == 1 && pointLineDir(point3, point4, pos) == 1) {
                    qreal distance = pointToLineDistance(point3, point4, pos);
                    QPointF add = pointSplid(point2, point4, distance);
                    point4 = QPointF(point4.x() + add.x(), point4.y() + add.y());
                    distance = pointToLineDistance(point1, point3, pos);
                    add = pointSplid(point1, point2, distance);
                    point1 = QPointF(point1.x() - add.x(), point1.y() + add.y());
                    point3 = pos;
                }
                newResizeFPoints.point1 = point1;
                newResizeFPoints.point2 = point2;
                newResizeFPoints.point3 = point3;
                newResizeFPoints.point4 = point4;
                return newResizeFPoints;
            } else {
                qreal distance1 = pointToLineDistance(point1, point3, pos);
                qreal distance2 = pointToLineDistance(point3, point4, pos);
                qreal distance = std::min(distance1, distance2);
                if (pointLineDir(point1, point3, pos) == 1 && pointLineDir(point3, point4, pos) == -1) {
                    QPointF add = pointSplid(point1, point2, distance);
                    point1 = QPointF(point1.x() - add.x(), point2.y() + add.y());
                    add = pointSplid(point2, point4, distance);
                    point4 = QPointF(point4.x() - add.x(), point4.y() - add.y());
                } else {
                    QPointF add = pointSplid(point1, point2, distance);
                    point1 = QPointF(point1.x() + add.x(), point2.y() - add.y());
                    add = pointSplid(point2, point4, distance);
                    point4 = QPointF(point4.x() + add.x(), point4.y() + add.y());
                }
                point3 = QPointF(point1.x() + point4.x() - point2.x(), point1.y() + point4.y() - point2.y());
                newResizeFPoints.point1 =  point1;
                newResizeFPoints.point2 = point2;
                newResizeFPoints.point3 = point3;
                newResizeFPoints.point4 = point4;
                return newResizeFPoints;
            }
        }
    }
    return newResizeFPoints;
}

/* point3 in the fourth position */
FourPoints point3Resize4(QPointF point1, QPointF point2, QPointF point3,
                         QPointF point4, QPointF pos, bool isShift) {
    FourPoints newResizeFPoints;
    newResizeFPoints.point1 = point1;
    newResizeFPoints.point2 = point2;
    newResizeFPoints.point3 = point3;
    newResizeFPoints.point4 = point4;

    if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) >= -TANT2_EDGEVALUE &&
    (pos.y() - MIN_PADDING < point4.y() || pos.x() + MIN_PADDING > point1.x() ||
     pointLineDir(point1, point2, pos) == -1 || pointLineDir(point2, point4, pos) == -1)) {
        return newResizeFPoints;
    } else if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) < -TANT2_EDGEVALUE &&
     (pos.y() - MIN_PADDING < point1.y() || pos.x() - MIN_PADDING < point4.x() ||
      pointLineDir(point1, point2, pos) == -1 || pointLineDir(point2, point4, pos) == -1)) {
        return newResizeFPoints;
    } else {
        if (pointToLineDistance(point2, point4, pos) < MIN_PADDING ||
                pointToLineDistance(point1, point2, pos) < MIN_PADDING) {
            return newResizeFPoints;
        } else {
            if (!isShift) {
                if (pointLineDir(point1, point3, pos) == 1 && pointLineDir(point3, point4, pos) == 1) {
                    qreal distance = pointToLineDistance(point3, point4, pos);
                    QPointF add = pointSplid(point2, point4, distance);
                    point4 = QPointF(point4.x() - add.x(), point4.y() + add.y());
                    distance = pointToLineDistance(point1, point3, pos);
                    add  = pointSplid(point1, point2, distance);
                    point1 = QPointF(point1.x() + add.x(), point1.y() + add.y());
                    point3 = pos;
                }
                if (pointLineDir(point1, point3, pos) == 1 && pointLineDir(point3, point4, pos) == -1) {
                    qreal distance = pointToLineDistance(point3, point4, pos);
                    QPointF add = pointSplid(point2, point4, distance);
                    point4 = QPointF(point4.x() + add.x(), point4.y() - add.y());
                    distance = pointToLineDistance(point1, point3, pos);
                    add  = pointSplid(point1, point2, distance);
                    point1 = QPointF(point1.x() + add.x(), point1.y() + add.y());
                    point3 = pos;
                }
                if (pointLineDir(point1, point3, pos) == -1 && pointLineDir(point3, point4, pos) == -1) {
                    qreal distance = pointToLineDistance(point3, point4, pos);
                    QPointF add = pointSplid(point2, point4, distance);
                    point4 = QPointF(point4.x() + add.x(), point4.y() - add.y());
                    distance = pointToLineDistance(point1, point3, pos);
                    add  = pointSplid(point1, point2, distance);
                    point1 = QPointF(point1.x() - add.x(), point1.y() - add.y());
                    point3 = pos;
                }
                if (pointLineDir(point1, point3, pos) == -1 && pointLineDir(point3, point4, pos) == 1) {
                    qreal distance = pointToLineDistance(point3, point4, pos);
                    QPointF add = pointSplid(point2, point4, distance);
                    point4 = QPointF(point4.x() - add.x(), point4.y() + add.y());
                    distance = pointToLineDistance(point1, point3, pos);
                    add  = pointSplid(point1, point2, distance);
                    point1 = QPointF(point1.x() - add.x(), point1.y() - add.y());
                    point3 = pos;
                }
                newResizeFPoints.point1 = point1;
                newResizeFPoints.point2 = point2;
                newResizeFPoints.point3 = point3;
                newResizeFPoints.point4 = point4;
                return newResizeFPoints;
            } else {
                qreal distance1 = pointToLineDistance(point1, point3, pos);
                qreal distance2 = pointToLineDistance(point3, point4, pos);
                qreal distance = std::min(distance1, distance2);

                if (pointLineDir(point1, point3, pos) == -1 && pointLineDir(point3, point4, pos) == -1) {
                    QPointF add = pointSplid(point1, point2, distance);
                    point1 = QPointF(point1.x() - add.x(), point2.y() - add.y());
                    add = pointSplid(point1, point2, distance);
                    point4 = QPointF(point4.x() + add.x(), point4.y() - add.y());
                } else {
                    QPointF add = pointSplid(point1, point2, distance);
                    point1 = QPointF(point1.x() + add.x(), point2.y() + add.y());
                    add = pointSplid(point1, point2, distance);
                    point4 = QPointF(point4.x() - add.x(), point4.y() + add.y());
                }
                point3 = QPointF(point1.x() + point4.x() - point2.x(), point1.y() + point4.y() - point2.y());
                newResizeFPoints.point1 =  point1;
                newResizeFPoints.point2 = point2;
                newResizeFPoints.point3 = point3;
                newResizeFPoints.point4 = point4;
                return newResizeFPoints;
            }
        }
    }
    return newResizeFPoints;
}

/* point3 in the fifth position */
FourPoints point3Resize5(QPointF point1, QPointF point2, QPointF point3,
                         QPointF point4, QPointF pos, bool isShift) {
    FourPoints newResizeFPoints;
    newResizeFPoints.point1 =  point1;
    newResizeFPoints.point2 = point2;
    newResizeFPoints.point3 = point3;
    newResizeFPoints.point4 = point4;

    if (pos.y() + MIN_PADDING > point4.y() || pos.x() - MIN_PADDING < point1.x()) {
        return newResizeFPoints;
    } else {
        if (!isShift) {
            point4.setX(pos.x());
            point1.setY(pos.y());
            point3 = pos;

            newResizeFPoints.point1 = point1;
            newResizeFPoints.point2 = point2;
            newResizeFPoints.point3 = point3;
            newResizeFPoints.point4 = point4;
            return newResizeFPoints;
        } else {
            qreal distance1 = pointToLineDistance(point1, point3,  pos);
            qreal distance2 = pointToLineDistance(point3, point4, pos);
            qreal distance = std::min(distance1, distance2);

            if (pointLineDir(point1, point3, pos) == 1 && pointLineDir(point3, point4, pos) == -1) {
                QPointF add = pointSplid(point1, point2, distance);
                point1 = QPointF(point1.x() - add.x(), point1.y() + add.y());
                add = pointSplid(point2, point4, distance);
                point4 = QPointF(point4.x() - add.x(), point4.y() - add.y());
            } else {
                QPointF add = pointSplid(point1, point2, distance);
                point1 = QPointF(point1.x() + add.x(), point1.y() - add.y());
                add = pointSplid(point2, point4, distance);
                point4 = QPointF(point4.x() + add.x(), point4.y() + add.y());
            }
            point3 = QPointF(point1.x() + point4.x() - point2.x(), point1.y() + point4.y() - point2.y());
            newResizeFPoints.point1 = point1;
            newResizeFPoints.point2 = point2;
            newResizeFPoints.point3 = point3;
            newResizeFPoints.point4 = point4;
            return newResizeFPoints;
        }
    }
    return newResizeFPoints;
}
/* point3 in the sixth position */
FourPoints point3Resize6(QPointF point1, QPointF point2, QPointF point3,
                         QPointF point4, QPointF pos, bool isShift) {
    Q_UNUSED(pos);
    Q_UNUSED(isShift);
    FourPoints newResizeFPoints;
    newResizeFPoints.point1 =  point1;
    newResizeFPoints.point2 = point2;
    newResizeFPoints.point3 = point3;
    newResizeFPoints.point4 = point4;
    return newResizeFPoints;
}
/* point3 in the seventh position */
FourPoints point3Resize7(QPointF point1, QPointF point2, QPointF point3,
                         QPointF point4, QPointF pos, bool isShift) {
    Q_UNUSED(pos);
    Q_UNUSED(isShift);
    FourPoints newResizeFPoints;
    newResizeFPoints.point1 =  point1;
    newResizeFPoints.point2 = point2;
    newResizeFPoints.point3 = point3;
    newResizeFPoints.point4 = point4;
    return newResizeFPoints;
}

/***************** fourth point4 *******************/
/* point4 in the first position */
FourPoints point4Resize1(QPointF point1, QPointF point2, QPointF point3,
                         QPointF point4, QPointF pos, bool isShift) {
    FourPoints newResizeFPoints;
    newResizeFPoints.point1 = point1;
    newResizeFPoints.point2 = point2;
    newResizeFPoints.point3 = point3;
    newResizeFPoints.point4 = point4;
    if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) >= TANT_EDGEVALUE &&
    (pos.x() - MIN_PADDING < point2.x() || pos.y() - MIN_PADDING < point3.y() ||
     pointLineDir(point1, point2, pos) == 1 || pointLineDir(point1, point3, pos) == -1)) {
        return newResizeFPoints;
    } else if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) < TANT_EDGEVALUE &&
     (pos.x() - MIN_PADDING < point3.x() || pos.y() + MIN_PADDING > point2.y() ||
      pointLineDir(point1, point2, pos) == 1 || pointLineDir(point1, point3, pos) == -1)) {
        return newResizeFPoints;
    } else {
        if (pointToLineDistance(point1, point2, pos) < MIN_PADDING ||
                pointToLineDistance(point1, point3, pos) < MIN_PADDING) {
            return newResizeFPoints;
        } else {
            if (!isShift) {
                if (pointLineDir(point3, point4, pos) == -1 && pointLineDir(point2, point4, pos) == 1) {
                    qreal distance = pointToLineDistance(point2, point4, pos);
                    QPointF add = pointSplid(point1, point2, distance);
                    point2 = QPointF(point2.x() + add.x(), point2.y() + add.y());
                    distance = pointToLineDistance(point3, point4, pos);
                    add = pointSplid(point1, point3, distance);
                    point3 = QPointF(point3.x() + add.x(), point3.y() - add.y());
                    point4 = pos;
                }
                if (pointLineDir(point3, point4, pos) == -1 && pointLineDir(point2, point4, pos) == -1) {
                    qreal distance = pointToLineDistance(point2, point4, pos);
                    QPointF add = pointSplid(point1, point2, distance);
                    point2 = QPointF(point2.x() - add.x(), point2.y() - add.y());
                    distance = pointToLineDistance(point3, point4, pos);
                    add = pointSplid(point1, point3, distance);
                    point3 = QPointF(point3.x() + add.x(), point3.y() - add.y());
                    point4 = pos;
                }
                if (pointLineDir(point3, point4, pos) == 1 && pointLineDir(point2, point4, pos) == -1) {
                    qreal distance = pointToLineDistance(point2, point4, pos);
                    QPointF add = pointSplid(point1, point2, distance);
                    point2 = QPointF(point2.x() - add.x(), point2.y() - add.y());
                    distance = pointToLineDistance(point3, point4, pos);
                    add = pointSplid(point1, point3, distance);
                    point3 = QPointF(point3.x() - add.x(), point3.y() + add.y());
                    point4 = pos;
                }
                if (pointLineDir(point3, point4, pos) == 1 && pointLineDir(point2, point4, pos) == 1) {
                    qreal distance = pointToLineDistance(point2, point4, pos);
                    QPointF add = pointSplid(point1, point2, distance);
                    point2 = QPointF(point2.x() + add.x(), point2.y() + add.y());
                    distance = pointToLineDistance(point3, point4, pos);
                    add = pointSplid(point1, point3, distance);
                    point3 = QPointF(point3.x() - add.x(), point3.y() + add.y());
                    point4 = pos;
                }
                newResizeFPoints.point1 = point1;
                newResizeFPoints.point2 = point2;
                newResizeFPoints.point3 = point3;
                newResizeFPoints.point4 = point4;
                return newResizeFPoints;
            } else {
                qreal distance1 = pointToLineDistance(point2, point4, pos);
                qreal distance2 = pointToLineDistance(point3, point4, pos);
                qreal distance =  std::min(distance1, distance2);
                if (pointLineDir(point2, point4, pos) == 1 && pointLineDir(point3, point4, pos) == -1) {
                    QPointF add = pointSplid(point1, point2, distance);
                    point2 = QPointF(point2.x() + add.x(), point2.y() + add.y());
                    add = pointSplid(point1, point3, distance);
                    point3 = QPointF(point3.x() + add.x(), point3.y() - add.y());
                    point4 = QPointF(point2.x() + point3.x() - point1.x(), point2.y() + point3.y() - point1.y());
                } else {
                    QPointF add = pointSplid(point1, point2, distance);
                    point2 = QPointF(point2.x() - add.x(), point2.y() - add.y());
                    add = pointSplid(point1, point3, distance);
                    point3 = QPointF(point3.x() - add.x(), point3.y() + add.y());
                    point4 = QPointF(point2.x() + point3.x() - point1.x(), point2.y() + point3.y() - point1.y());
                }
                point4 = QPointF(point2.x() + point3.x() - point1.x(), point2.y() + point3.y() - point2.y());
                newResizeFPoints.point1 =  point1;
                newResizeFPoints.point2 = point2;
                newResizeFPoints.point3 = point3;
                newResizeFPoints.point4 = point4;
                return newResizeFPoints;
            }
        }
    }
    return newResizeFPoints;
}

/* point4 in the second position */
FourPoints point4Resize2(QPointF point1, QPointF point2, QPointF point3,
                         QPointF point4, QPointF pos, bool isShift) {
    FourPoints newResizeFPoints;
    newResizeFPoints.point1 = point1;
    newResizeFPoints.point2 = point2;
    newResizeFPoints.point3 = point3;
    newResizeFPoints.point4 = point4;
    if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) <= -TANT_EDGEVALUE &&
    (pos.y() + MIN_PADDING > point3.y() || pos.x() + MIN_PADDING > point2.x() ||
     pointLineDir(point1, point2, pos) == 1 || pointLineDir(point1, point3, pos) == 1)) {
        return newResizeFPoints;
    } else if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) > -TANT_EDGEVALUE &&
     (pos.y() + MIN_PADDING > point2.y() || pos.x() - MIN_PADDING < point3.x() ||
      pointLineDir(point1, point2, pos) == 1 || pointLineDir(point1, point3, pos) == 1)) {
        return newResizeFPoints;
    } else {
        if (pointToLineDistance(point1, point2, pos) < MIN_PADDING ||
                pointToLineDistance(point1, point3, pos) < MIN_PADDING) {
            return newResizeFPoints;
        } else {  
            if (!isShift) {
                if (pointLineDir(point3, point4, pos) == 1 && pointLineDir(point2, point4, pos) == 1) {
                    qreal distance = pointToLineDistance(point2, point4, pos);
                    QPointF add = pointSplid(point1, point2, distance);
                    point2 = QPointF(point2.x() - add.x(), point2.y() + add.y());
                    distance = pointToLineDistance(point3, point4, pos);
                    add = pointSplid(point1, point3, distance);
                    point3 = QPointF(point3.x() + add.x(), point3.y() + add.y());
                    point4 = pos;
                }
                if (pointLineDir(point3, point4, pos) == -1 && pointLineDir(point2, point4, pos) == 1) {
                    qreal distance = pointToLineDistance(point2, point4, pos);
                    QPointF add = pointSplid(point1, point2, distance);
                    point2 = QPointF(point2.x() - add.x(), point2.y() + add.y());
                    distance = pointToLineDistance(point3, point4, pos);
                    add = pointSplid(point1, point3, distance);
                    point3 = QPointF(point3.x() - add.x(), point3.y() - add.y());
                    point4 = pos;
                }
                if (pointLineDir(point3, point4, pos) == -1 && pointLineDir(point2, point4, pos) == -1) {
                    qreal distance = pointToLineDistance(point2, point4, pos);
                    QPointF add = pointSplid(point1, point2, distance);
                    point2 = QPointF(point2.x() + add.x(), point2.y() - add.y());
                    distance = pointToLineDistance(point3, point4, pos);
                    add = pointSplid(point1, point3, distance);
                    point3 = QPointF(point3.x() - add.x(), point3.y() - add.y());
                    point4 = pos;
                }
                if (pointLineDir(point3, point4, pos) == 1 && pointLineDir(point2, point4, pos) == -1) {
                    qreal distance = pointToLineDistance(point2, point4, pos);
                    QPointF add = pointSplid(point1, point2, distance);
                    point2 = QPointF(point2.x() + add.x(), point2.y() - add.y());
                    distance = pointToLineDistance(point3, point4, pos);
                    add = pointSplid(point1, point3, distance);
                    point3 = QPointF(point3.x() + add.x(), point3.y() + add.y());
                    point4 = pos;
                }
                newResizeFPoints.point1 = point1;
                newResizeFPoints.point2 = point2;
                newResizeFPoints.point3 = point3;
                newResizeFPoints.point4 = point4;
                return newResizeFPoints;
            } else {
                qreal distance1 = pointToLineDistance(point2, point4, pos);
                qreal distance2 = pointToLineDistance(point3, point4, pos);
                qreal distance = std::min(distance1, distance2);
                if (pointLineDir(point2, point4, pos) == -1 && pointLineDir(point3, point4, pos) == -1) {
                    QPointF add = pointSplid(point1, point2, distance);
                    point2 = QPointF(point2.x() + add.x(), point2.y() - add.y());
                    add = pointSplid(point1, point3, distance);
                    point3 = QPointF(point3.x() - add.x(), point3.y() - add.y());
                } else {
                    QPointF add = pointSplid(point1, point2, distance);
                    point2 = QPointF(point2.x() - add.x(), point2.y() + add.y());
                    add = pointSplid(point1, point3, distance);
                    point3 = QPointF(point3.x() + add.x(), point3.y() + add.y());
                }
                point4 = QPointF(point2.x() + point3.x() - point1.x(), point2.y() + point3.y() - point2.y());
                newResizeFPoints.point1 =  point1;
                newResizeFPoints.point2 = point2;
                newResizeFPoints.point3 = point3;
                newResizeFPoints.point4 = point4;
                return newResizeFPoints;
            }
        }
    }
    return newResizeFPoints;
}

/* point4 in the third position */
FourPoints point4Resize3(QPointF point1, QPointF point2, QPointF point3,
                         QPointF point4, QPointF pos, bool isShift) {
    FourPoints newResizeFPoints;
    newResizeFPoints.point1 = point1;
    newResizeFPoints.point2 = point2;
    newResizeFPoints.point3 = point3;
    newResizeFPoints.point4 = point4;

    if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) >= TANT2_EDGEVALUE &&
    (pos.y() - MIN_PADDING < point2.y() || pos.x() + MIN_PADDING > point3.x() ||
     pointLineDir(point1, point2, pos) == -1 || pointLineDir(point1, point3, pos) == -1)) {
        return newResizeFPoints;
    } else if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) < TANT2_EDGEVALUE &&
     (pos.y() - MIN_PADDING < point3.y() || pos.x() - MIN_PADDING < point2.x() ||
      pointLineDir(point1, point2, pos) == -1 || pointLineDir(point1, point3, pos) == -1)) {
        return newResizeFPoints;
    } else {
        if (pointToLineDistance(point1, point2, pos) < MIN_PADDING ||
                pointToLineDistance(point1, point3, pos) < MIN_PADDING) {
            return newResizeFPoints;
        } else {
            if (!isShift) {
                if (pointLineDir(point3, point4, pos) == 1 && pointLineDir(point2, point4, pos) == 1) {
                    qreal distance = pointToLineDistance(point2, point4, pos);
                    QPointF add = pointSplid(point1, point2, distance);
                    point2 = QPointF(point2.x() - add.x(), point2.y() + add.y());
                    distance = pointToLineDistance(point3, point4, pos);
                    add = pointSplid(point1, point3, distance);
                    point3 = QPointF(point3.x() + add.x(), point3.y() + add.y());
                    point4 = pos;
                }
                if (pointLineDir(point3, point4, pos) == -1 && pointLineDir(point2, point4, pos) == 1) {
                    qreal distance = pointToLineDistance(point2, point4, pos);
                    QPointF add = pointSplid(point1, point2, distance);
                    point2 = QPointF(point2.x() - add.x(), point2.y() + add.y());
                    distance = pointToLineDistance(point3, point4, pos);
                    add = pointSplid(point1, point3, distance);
                    point3 = QPointF(point3.x() - add.x(), point3.y() - add.y());
                    point4 = pos;
                }
                if (pointLineDir(point3, point4, pos) == -1 && pointLineDir(point2, point4, pos) == -1) {
                    qreal distance = pointToLineDistance(point2, point4, pos);
                    QPointF add = pointSplid(point1, point2, distance);
                    point2 = QPointF(point2.x() + add.x(), point2.y() - add.y());
                    distance = pointToLineDistance(point3, point4, pos);
                    add = pointSplid(point1, point3, distance);
                    point3 = QPointF(point3.x() - add.x(), point3.y() - add.y());
                    point4 = pos;
                }
                if (pointLineDir(point3, point4, pos) == 1 && pointLineDir(point2, point4, pos) == -1) {
                    qreal distance = pointToLineDistance(point2, point4, pos);
                    QPointF add = pointSplid(point1, point2, distance);
                    point2 = QPointF(point2.x() + add.x(), point2.y() - add.y());
                    distance = pointToLineDistance(point3, point4, pos);
                    add = pointSplid(point1, point3, distance);
                    point3 = QPointF(point3.x() + add.x(), point3.y() + add.y());
                    point4 = pos;
                }
                newResizeFPoints.point1 = point1;
                newResizeFPoints.point2 = point2;
                newResizeFPoints.point3 = point3;
                newResizeFPoints.point4 = point4;
                return newResizeFPoints;
            } else {
                qreal distance1 = pointToLineDistance(point2, point4, pos);
                qreal distance2 = pointToLineDistance(point3, point4, pos);
                qreal distance = std::min(distance1, distance2);
                if (pointLineDir(point2, point4, pos) == 1 && pointLineDir(point3, point4, pos) == 1) {
                    QPointF add = pointSplid(point1, point2, distance);
                    point2 = QPointF(point2.x() - add.x(), point2.y() + add.y());
                    add = pointSplid(point1, point3, distance);
                    point3 = QPointF(point3.x() + add.x(), point3.y() + add.y());
                } else {
                    QPointF add = pointSplid(point1, point2, distance);
                    point2 = QPointF(point2.x() + add.x(), point2.y() - add.y());
                    add = pointSplid(point1, point3, distance);
                    point3 = QPointF(point3.x() - add.x(), point3.y() - add.y());
                }
                point4 = QPointF(point2.x() + point3.x() - point1.x(), point2.y() + point3.y() - point2.y());
                newResizeFPoints.point1 =  point1;
                newResizeFPoints.point2 = point2;
                newResizeFPoints.point3 = point3;
                newResizeFPoints.point4 = point4;
                return newResizeFPoints;
            }
        }
    }
    return newResizeFPoints;
}

/* point4 in the third position */
FourPoints point4Resize4(QPointF point1, QPointF point2, QPointF point3,
                         QPointF point4, QPointF pos, bool isShift) {
    FourPoints newResizeFPoints;
    newResizeFPoints.point1 = point1;
    newResizeFPoints.point2 = point2;
    newResizeFPoints.point3 = point3;
    newResizeFPoints.point4 = point4;

    if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) >= -TANT2_EDGEVALUE &&
    (pos.x() + MIN_PADDING > point2.x() || pos.y() + MIN_PADDING > point3.y() ||
     pointLineDir(point1, point2, pos) == -1 || pointLineDir(point1, point3, pos) == 1)) {
        return newResizeFPoints;
    } else if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) < -TANT2_EDGEVALUE &&
     (pos.x() + MIN_PADDING > point3.x() || pos.y() - MIN_PADDING < point2.y() ||
      pointLineDir(point1, point2, pos) == -1 || pointLineDir(point1, point3, pos) == 1)) {
        return newResizeFPoints;
    } else {
        if (pointToLineDistance(point1, point2, pos) < MIN_PADDING ||
                pointToLineDistance(point1, point3, pos) < MIN_PADDING) {
            return newResizeFPoints;
        } else {
            if (!isShift) {
                if (pointLineDir(point3, point4, pos) == 1 && pointLineDir(point2, point4, pos) == -1) {
                    qreal distance = pointToLineDistance(point2, point4, pos);
                    QPointF add = pointSplid(point1, point2, distance);
                    point2 = QPointF(point2.x() - add.x(), point2.y() - add.y());
                    distance = pointToLineDistance(point3, point4, pos);
                    add = pointSplid(point1, point3, distance);
                    point3 = QPointF(point3.x() - add.x(), point3.y() + add.y());
                    point4 = pos;
                }
                if (pointLineDir(point3, point4, pos) == -1 && pointLineDir(point2, point4, pos) == -1) {
                    qreal distance = pointToLineDistance(point2, point4, pos);
                    QPointF add = pointSplid(point1, point2, distance);
                    point2 = QPointF(point2.x() - add.x(), point2.y() - add.y());
                    distance = pointToLineDistance(point3, point4, pos);
                    add = pointSplid(point1, point3, distance);
                    point3 = QPointF(point3.x() + add.x(), point3.y() - add.y());
                    point4 = pos;
                }
                if (pointLineDir(point3, point4, pos) == -1 && pointLineDir(point2, point4, pos) == 1) {
                    qreal distance = pointToLineDistance(point2, point4, pos);
                    QPointF add = pointSplid(point1, point2, distance);
                    point2 = QPointF(point2.x() + add.x(), point2.y() + add.y());
                    distance = pointToLineDistance(point3, point4, pos);
                    add = pointSplid(point1, point3, distance);
                    point3 = QPointF(point3.x() + add.x(), point3.y() - add.y());
                    point4 = pos;
                }
                if (pointLineDir(point3, point4, pos) == 1 && pointLineDir(point2, point4, pos) == 1) {
                    qreal distance = pointToLineDistance(point2, point4, pos);
                    QPointF add = pointSplid(point1, point2, distance);
                    point2 = QPointF(point2.x() + add.x(), point2.y() + add.y());
                    distance = pointToLineDistance(point3, point4, pos);
                    add = pointSplid(point1, point3, distance);
                    point3 = QPointF(point3.x() - add.x(), point3.y() + add.y());
                    point4 = pos;
                }
                newResizeFPoints.point1 = point1;
                newResizeFPoints.point2 = point2;
                newResizeFPoints.point3 = point3;
                newResizeFPoints.point4 = point4;
                return newResizeFPoints;
            } else {
                qreal distance1 = pointToLineDistance(point2,  point4, pos);
                qreal distance2 = pointToLineDistance(point3, point4, pos);
                qreal distance = std::min(distance1, distance2);
                if (pointLineDir(point2, point4, pos) == -1 && pointLineDir(point3, point4, pos) == 1) {
                    QPointF add = pointSplid(point1, point2, distance);
                    point2 = QPointF(point2.x() - add.x(), point2.y() - add.y());
                    add = pointSplid(point1, point3, distance);
                    point3 = QPointF(point3.x() - add.x(), point3.y() + add.y());
                } else {
                    QPointF add = pointSplid(point1, point2, distance);
                    point2 = QPointF(point2.x() + add.x(), point2.y() + add.y());
                    add = pointSplid(point1, point3, distance);
                    point3 = QPointF(point3.x() + add.x(), point3.y() - add.y());
                }
                point4 = QPointF(point2.x() + point3.x() - point1.x(), point2.y() + point3.y() - point2.y());
                newResizeFPoints.point1 =  point1;
                newResizeFPoints.point2 = point2;
                newResizeFPoints.point3 = point3;
                newResizeFPoints.point4 = point4;
                return newResizeFPoints;
            }
        }
    }
    return newResizeFPoints;
}

/* point4 in the fifth position */
FourPoints point4Resize5(QPointF point1, QPointF point2, QPointF point3,
                         QPointF point4, QPointF pos, bool isShift) {
    FourPoints newResizeFPoints;
    newResizeFPoints.point1 =  point1;
    newResizeFPoints.point2 = point2;
    newResizeFPoints.point3 = point3;
    newResizeFPoints.point4 = point4;
    if (pos.y() - MIN_PADDING < point3.y() || pos.x() - MIN_PADDING < point2.x()) {
        return newResizeFPoints;
    } else {
        if (!isShift) {
            point2.setY(pos.y());
            point3.setX(pos.x());
            point4 = pos;

            newResizeFPoints.point1 = point1;
            newResizeFPoints.point2 = point2;
            newResizeFPoints.point3 = point3;
            newResizeFPoints.point4 = point4;
            return newResizeFPoints;
        }
    }
    return newResizeFPoints;
}
/* point4 in the sixth position */
FourPoints point4Resize6(QPointF point1, QPointF point2, QPointF point3,
                         QPointF point4, QPointF pos, bool isShift) {
    Q_UNUSED(pos);
    Q_UNUSED(isShift);
    FourPoints newResizeFPoints;
    newResizeFPoints.point1 =  point1;
    newResizeFPoints.point2 = point2;
    newResizeFPoints.point3 = point3;
    newResizeFPoints.point4 = point4;
    return newResizeFPoints;
}
/* point4 in the seventh position */
FourPoints point4Resize7(QPointF point1, QPointF point2, QPointF point3,
                         QPointF point4, QPointF pos, bool isShift) {
    Q_UNUSED(pos);
    Q_UNUSED(isShift);
    FourPoints newResizeFPoints;
    newResizeFPoints.point1 =  point1;
    newResizeFPoints.point2 = point2;
    newResizeFPoints.point3 = point3;
    newResizeFPoints.point4 = point4;
    return newResizeFPoints;
}

/********************** fifth point5 ************************/
/* point5 in the first position */
FourPoints point5Resize1(QPointF point1, QPointF point2, QPointF point3,
                         QPointF point4, QPointF pos, bool isShift) {
    FourPoints newResizeFPoints;
    newResizeFPoints.point1 = point1;
    newResizeFPoints.point2 = point2;
    newResizeFPoints.point3 = point3;
    newResizeFPoints.point4 = point4;
    QPointF point7 = QPointF((point3.x() + point4.x())/2, (point3.y() + point4.y())/2);

    if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) >= TANT_EDGEVALUE &&
            (pos.x() + MIN_PADDING > point7.x()  || pointLineDir(point3, point4, pos) == -1 ||
             pointLineDir(point2, point4, pos) == 1)) {
        return newResizeFPoints;
    } else if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) < TANT_EDGEVALUE &&
               (pos.y() - MIN_PADDING < point7.y() ||pointLineDir(point3, point4, pos) == -1 ||
                pointLineDir(point2, point4, pos) == 1)) {
        return newResizeFPoints;
    } else {
        if (pointToLineDistance(point3, point4, pos) < MIN_PADDING) {
            return newResizeFPoints;
        } else {
            if (!isShift) {
                if (pointLineDir(point1, point2, pos) == 1) {
                    qreal distance = pointToLineDistance(point1, point2, pos);
                    QPointF add = pointSplid(point1, point3, distance);
                    point1 = QPointF(point1.x() - add.x(), point1.y() + add.y());
                    add = pointSplid(point2, point4, distance);
                    point2 = QPointF(point2.x() - add.x(), point2.y() + add.y());
                } else {
                    qreal distance = pointToLineDistance(point1, point2, pos);
                    QPointF add = pointSplid(point1, point3, distance);
                    point1 = QPointF(point1.x() + add.x(), point1.y() - add.y());
                    add = pointSplid(point2, point4, distance);
                    point2 = QPointF(point2.x() + add.x(), point2.y() - add.y());
                }
                newResizeFPoints.point1 = point1;
                newResizeFPoints.point2 = point2;
                newResizeFPoints.point3 = point3;
                newResizeFPoints.point4 = point4;
                return newResizeFPoints;
            } else {
                qreal distance = pointToLineDistance(point1,  point2, pos);
                if (pointLineDir(point1, point2, pos) == -1) {
                    QPointF add = pointSplid(point2, point4, distance);
                    point2 = QPointF(point2.x() + add.x(), point2.y() - add.y());
                    add = pointSplid(point3, point4, distance);
                    point3 = QPointF(point3.x() + add.x(), point3.y() + add.y());
                } else {
                    QPointF add = pointSplid(point2, point4, distance);
                    point2 = QPointF(point2.x() - add.x(), point2.y() + add.y());
                    add = pointSplid(point3, point4, distance);
                    point3 = QPointF(point3.x() - add.x(), point3.y() - add.y());
                }
                point1 = QPointF(point2.x() + point3.x() - point4.x(), point2.y() + point3.y() - point4.y());
                newResizeFPoints.point1 =  point1;
                newResizeFPoints.point2 = point2;
                newResizeFPoints.point3 = point3;
                newResizeFPoints.point4 = point4;
                return newResizeFPoints;
            }
        }
    }
    return newResizeFPoints;
}

/* point5 in the second position */
FourPoints point5Resize2(QPointF point1, QPointF point2, QPointF point3,
                         QPointF point4, QPointF pos, bool isShift) {
    FourPoints newResizeFPoints;
    newResizeFPoints.point1 = point1;
    newResizeFPoints.point2 = point2;
    newResizeFPoints.point3 = point3;
    newResizeFPoints.point4 = point4;
    QPointF point7 = QPointF((point3.x() + point4.x())/2, (point3.y() + point4.y())/2);

    if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) <= -TANT_EDGEVALUE &&
            (pos.x() - MIN_PADDING < point7.x()  || pointLineDir(point3, point4, pos) == -1 ||
             pointLineDir(point2, point4, pos) == -1)) {
        return newResizeFPoints;
    } else if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) > -TANT_EDGEVALUE &&
               (pos.y() - MIN_PADDING < point7.y() ||pointLineDir(point3, point4, pos) == -1 ||
                pointLineDir(point2, point4, pos) == -1)) {
        return newResizeFPoints;
    } else {
        if (pointToLineDistance(point3, point4, pos) < MIN_PADDING) {
            return newResizeFPoints;
        } else {
            if (!isShift) {
                if (pointLineDir(point1, point2, pos) == 1) {
                    qreal distance = pointToLineDistance(point1, point2, pos);
                    QPointF add = pointSplid(point1, point3, distance);
                    point1 = QPointF(point1.x() + add.x(), point1.y() + add.y());
                    add = pointSplid(point2, point4, distance);
                    point2 = QPointF(point2.x() + add.x(), point2.y() + add.y());
                } else {
                    qreal distance = pointToLineDistance(point1, point2, pos);
                    QPointF add = pointSplid(point1, point3, distance);
                    point1 = QPointF(point1.x() - add.x(), point1.y() - add.y());
                    add = pointSplid(point2, point4, distance);
                    point2 = QPointF(point2.x() - add.x(), point2.y() - add.y());
                }
                    newResizeFPoints.point1 = point1;
                    newResizeFPoints.point2 = point2;
                    newResizeFPoints.point3 = point3;
                    newResizeFPoints.point4 = point4;
                    return newResizeFPoints;
                } else {
                    qreal distance = pointToLineDistance(point1,  point2, pos);
                    if (pointLineDir(point1, point2, pos) == -1) {
                        QPointF add = pointSplid(point2, point4, distance);
                        point2 = QPointF(point2.x() - add.x(), point2.y() - add.y());
                        add = pointSplid(point3, point4, distance);
                        point3 = QPointF(point3.x() + add.x(), point3.y() - add.y());
                    } else {
                        QPointF add = pointSplid(point2, point4, distance);
                        point2 = QPointF(point2.x() + add.x(), point2.y() + add.y());
                        add = pointSplid(point3, point4, distance);
                        point3 = QPointF(point3.x() - add.x(), point3.y() + add.y());
                    }
                    point1 = QPointF(point2.x() + point3.x() - point4.x(), point2.y() + point3.y() - point4.y());
                    newResizeFPoints.point1 =  point1;
                    newResizeFPoints.point2 = point2;
                    newResizeFPoints.point3 = point3;
                    newResizeFPoints.point4 = point4;
                    return newResizeFPoints;
                }
            }
    }
    return newResizeFPoints;
}

/* point5 in the  third position */
FourPoints point5Resize3(QPointF point1, QPointF point2, QPointF point3,
                         QPointF point4, QPointF pos, bool isShift) {
    FourPoints newResizeFPoints;
    newResizeFPoints.point1 = point1;
    newResizeFPoints.point2 = point2;
    newResizeFPoints.point3 = point3;
    newResizeFPoints.point4 = point4;
    QPointF point7 = QPointF((point3.x() + point4.x())/2, (point3.y() + point4.y())/2);

    if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) >= TANT2_EDGEVALUE &&
            (pos.y() + MIN_PADDING > point7.y()  || pointLineDir(point3, point4, pos) == 1 ||
             pointLineDir(point2, point4, pos) == 1)) {
        return newResizeFPoints;
    } else if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x()))  < TANT2_EDGEVALUE &&
               (pos.x() + MIN_PADDING > point7.x() ||pointLineDir(point3, point4, pos) == 1 ||
                pointLineDir(point2, point4, pos) == 1)) {
        return newResizeFPoints;
    } else {
        if (pointToLineDistance(point3, point4, pos) < MIN_PADDING) {
            return newResizeFPoints;
        } else {
            if (!isShift) {
                if (pointLineDir(point1, point2, pos) == 1) {
                    qreal distance = pointToLineDistance(point1, point2, pos);
                    QPointF add = pointSplid(point1, point3, distance);
                    point1 = QPointF(point1.x() + add.x(), point1.y() + add.y());
                    add = pointSplid(point2, point4, distance);
                    point2 = QPointF(point2.x() + add.x(), point2.y() + add.y());
                } else {
                    qreal distance = pointToLineDistance(point1, point2, pos);
                    QPointF add = pointSplid(point1, point3, distance);
                    point1 = QPointF(point1.x() - add.x(), point1.y() - add.y());
                    add = pointSplid(point2, point4, distance);
                    point2 = QPointF(point2.x() - add.x(), point2.y() - add.y());
                }
                    newResizeFPoints.point1 = point1;
                    newResizeFPoints.point2 = point2;
                    newResizeFPoints.point3 = point3;
                    newResizeFPoints.point4 = point4;
                    return newResizeFPoints;
                } else {
                    qreal distance = pointToLineDistance(point1,  point2, pos);
                    if (pointLineDir(point1, point2, pos) == 1) {
                        QPointF add = pointSplid(point2, point4, distance);
                        point2 = QPointF(point2.x() + add.x(), point2.y() + add.y());
                        add = pointSplid(point3, point4, distance);
                        point3 = QPointF(point3.x() - add.x(), point3.y() + add.y());
                    } else {
                        QPointF add = pointSplid(point2, point4, distance);
                        point2 = QPointF(point2.x() - add.x(), point2.y() - add.y());
                        add = pointSplid(point3, point4, distance);
                        point3 = QPointF(point3.x() + add.x(), point3.y() - add.y());
                    }
                    point1 = QPointF(point2.x() + point3.x() - point4.x(), point2.y() + point3.y() - point4.y());
                    newResizeFPoints.point1 =  point1;
                    newResizeFPoints.point2 = point2;
                    newResizeFPoints.point3 = point3;
                    newResizeFPoints.point4 = point4;
                    return newResizeFPoints;
                }
            }
    }
    return newResizeFPoints;
}

/* point5 in the  fourth position */
FourPoints point5Resize4(QPointF point1, QPointF point2, QPointF point3,
                         QPointF point4, QPointF pos, bool isShift) {
    FourPoints newResizeFPoints;
    newResizeFPoints.point1 = point1;
    newResizeFPoints.point2 = point2;
    newResizeFPoints.point3 = point3;
    newResizeFPoints.point4 = point4;
    QPointF point7 = QPointF((point3.x() + point4.x())/2, (point3.y() + point4.y())/2);

    if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) >= -TANT2_EDGEVALUE &&
            (pos.x() - MIN_PADDING< point7.x()  || pointLineDir(point3, point4, pos) == 1 ||
             pointLineDir(point2, point4, pos) == -1)) {
        return newResizeFPoints;
    } else if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x()))  < -TANT2_EDGEVALUE &&
               (pos.y() + MIN_PADDING > point7.y() ||pointLineDir(point3, point4, pos) == 1 ||
                pointLineDir(point2, point4, pos) == -1)) {
        return newResizeFPoints;
    } else {
        if (pointToLineDistance(point3, point4, pos) < MIN_PADDING) {
            return newResizeFPoints;
        } else {
            if (!isShift) {
                if (pointLineDir(point1, point2, pos) == -1) {
                    qreal distance = pointToLineDistance(point1, point2, pos);
                    QPointF add = pointSplid(point1, point3, distance);
                    point1 = QPointF(point1.x() + add.x(), point1.y() - add.y());
                    add = pointSplid(point2, point4, distance);
                    point2 = QPointF(point2.x() + add.x(), point2.y() - add.y());
                } else {
                    qreal distance = pointToLineDistance(point1, point2, pos);
                    QPointF add = pointSplid(point1, point3, distance);
                    point1 = QPointF(point1.x() - add.x(), point1.y() + add.y());
                    add = pointSplid(point2, point4, distance);
                    point2 = QPointF(point2.x() - add.x(), point2.y() + add.y());
                }
                    newResizeFPoints.point1 = point1;
                    newResizeFPoints.point2 = point2;
                    newResizeFPoints.point3 = point3;
                    newResizeFPoints.point4 = point4;
                    return newResizeFPoints;
                } else {
                    qreal distance = pointToLineDistance(point1,  point2, pos);
                    if (pointLineDir(point1, point2, pos) == 1) {
                        QPointF add = pointSplid(point2, point4, distance);
                        point2 = QPointF(point2.x() - add.x(), point2.y() + add.y());
                        add = pointSplid(point3, point4, distance);
                        point3 = QPointF(point3.x() - add.x(), point3.y() - add.y());
                    } else {
                        QPointF add = pointSplid(point2, point4, distance);
                        point2 = QPointF(point2.x() + add.x(), point2.y() - add.y());
                        add = pointSplid(point3, point4, distance);
                        point3 = QPointF(point3.x() + add.x(), point3.y() + add.y());
                    }
                    point1 = QPointF(point2.x() + point3.x() - point4.x(), point2.y() + point3.y() - point4.y());
                    newResizeFPoints.point1 =  point1;
                    newResizeFPoints.point2 = point2;
                    newResizeFPoints.point3 = point3;
                    newResizeFPoints.point4 = point4;
                    return newResizeFPoints;
                }
            }
        }

    return newResizeFPoints;
}

/* point5 in the fifth position */
FourPoints point5Resize5(QPointF point1, QPointF point2, QPointF point3,
                         QPointF point4, QPointF pos, bool isShift) {
    FourPoints newResizeFPoints;
    newResizeFPoints.point1 = point1;
    newResizeFPoints.point2 = point2;
    newResizeFPoints.point3 = point3;
    newResizeFPoints.point4 = point4;
    QPointF point7 = QPointF((point3.x() + point4.x())/2, (point3.y() + point4.y())/2);

    if (pos.x() + MIN_PADDING > point7.x()) {
        return newResizeFPoints;
    } else {
        if (!isShift) {
            if (pointLineDir(point1, point2, pos) == -1) {
                qreal distance = pointToLineDistance(point1, point2, pos);
                QPointF add = pointSplid(point1, point3, distance);
                point1 = QPointF(point1.x() - add.x(), point1.y() + add.y());
                add = pointSplid(point2, point4, distance);
                point2 = QPointF(point2.x() - add.x(), point2.y() + add.y());
            } else {
                qreal distance = pointToLineDistance(point1, point2, pos);
                QPointF add = pointSplid(point1, point3, distance);
                point1 = QPointF(point1.x() + add.x(), point1.y() - add.y());
                add = pointSplid(point2, point4, distance);
                point2 = QPointF(point2.x() + add.x(), point2.y() - add.y());
            }
            newResizeFPoints.point1 = point1;
            newResizeFPoints.point2 = point2;
            newResizeFPoints.point3 = point3;
            newResizeFPoints.point4 = point4;
            return newResizeFPoints;
        } else {
            qreal distance = pointToLineDistance(point1,  point2, pos);
            if (pointLineDir(point1, point2, pos) == 1) {
                QPointF add = pointSplid(point2, point4, distance);
                point2 = QPointF(point2.x() + add.x(), point2.y() - add.y());
                add = pointSplid(point3, point4, distance);
                point3 = QPointF(point3.x() + add.x(), point3.y() + add.y());
            } else {
                QPointF add = pointSplid(point2, point4, distance);
                point2 = QPointF(point2.x() - add.x(), point2.y() + add.y());
                add = pointSplid(point3, point4, distance);
                point3 = QPointF(point3.x() - add.x(), point3.y() - add.y());
            }
            point1 = QPointF(point2.x() + point3.x() - point4.x(), point2.y() + point3.y() - point4.y());
            newResizeFPoints.point1 =  point1;
            newResizeFPoints.point2 = point2;
            newResizeFPoints.point3 = point3;
            newResizeFPoints.point4 = point4;
            return newResizeFPoints;
        }
    }
    return newResizeFPoints;
}

/* point5 in the sixth position */
FourPoints point5Resize6(QPointF point1, QPointF point2, QPointF point3,
                         QPointF point4, QPointF pos, bool isShift) {
    Q_UNUSED(pos);
    Q_UNUSED(isShift);
    FourPoints newResizeFPoints;
    newResizeFPoints.point1 =  point1;
    newResizeFPoints.point2 = point2;
    newResizeFPoints.point3 = point3;
    newResizeFPoints.point4 = point4;
    return newResizeFPoints;
}

/* point5 in the seventh position */
FourPoints point5Resize7(QPointF point1, QPointF point2, QPointF point3,
                         QPointF point4, QPointF pos, bool isShift) {
    Q_UNUSED(pos);
    Q_UNUSED(isShift);
    FourPoints newResizeFPoints;
    newResizeFPoints.point1 =  point1;
    newResizeFPoints.point2 = point2;
    newResizeFPoints.point3 = point3;
    newResizeFPoints.point4 = point4;
    return newResizeFPoints;
}

/********************** sixth point6 ************************/
/* point6 in the first position */
FourPoints point6Resize1(QPointF point1, QPointF point2, QPointF point3,
                         QPointF point4, QPointF pos, bool isShift) {
    FourPoints newResizeFPoints;
    newResizeFPoints.point1 = point1;
    newResizeFPoints.point2 = point2;
    newResizeFPoints.point3 = point3;
    newResizeFPoints.point4 = point4;
    QPointF point8 = QPointF((point2.x() + point4.x())/2, (point2.y() + point4.y())/2);

    if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) >= TANT_EDGEVALUE &&
            (pos.y() + MIN_PADDING > point8.x()  || pointLineDir(point3, point4, pos) == -1 ||
             pointLineDir(point2, point4, pos) == 1)) {
        return newResizeFPoints;
    } else if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) < TANT_EDGEVALUE &&
               (pos.x() + MIN_PADDING > point8.x() ||pointLineDir(point3, point4, pos) == -1 ||
                pointLineDir(point2, point4, pos) == 1)) {
        return newResizeFPoints;
    } else {
        if (pointToLineDistance(point2, point4, pos) < MIN_PADDING) {
            return newResizeFPoints;
        } else {
            if (!isShift) {
                if (pos.x()  >= (point2.x() + point4.x())/2 || pos.y() >= (point2.y() + point4.y())/2) {
                    return newResizeFPoints;
                } else {
                    if (pointLineDir(point1, point3, pos) == 1) {
                        qreal distance = pointToLineDistance(point1, point3, pos);
                        QPointF add = pointSplid(point1, point2, distance);
                        point1 = QPointF(point1.x() + add.x(), point1.y() + add.y());
                        add = pointSplid(point3, point4, distance);
                        point3 = QPointF(point3.x() + add.x(), point3.y() + add.y());
                    } else {
                        qreal distance = pointToLineDistance(point1, point3, pos);
                        QPointF add = pointSplid(point1, point2, distance);
                        point1 = QPointF(point1.x() - add.x(), point1.y() - add.y());
                        add = pointSplid(point3, point4, distance);
                        point3 = QPointF(point3.x() - add.x(), point3.y() - add.y());
                    }
                }
                newResizeFPoints.point1 = point1;
                newResizeFPoints.point2 = point2;
                newResizeFPoints.point3 = point3;
                newResizeFPoints.point4 = point4;
                return newResizeFPoints;
            } else {
                qreal distance = pointToLineDistance(point1,  point3, pos);
                if (pointLineDir(point1, point3, pos) == 1) {
                    QPointF add = pointSplid(point1, point2, distance);
                    point1 = QPointF(point1.x() + add.x(), point1.y() - add.y());
                    add = pointSplid(point2, point4, distance);
                    point4 = QPointF(point4.x() - add.x(), point4.y() + add.y());
                } else {
                    QPointF add = pointSplid(point1, point2, distance);
                    point1 = QPointF(point1.x() - add.x(), point1.y() - add.y());
                    add = pointSplid(point2, point4, distance);
                    point4 = QPointF(point4.x() + add.x(), point4.y() - add.y());
                }
                point3 = QPointF(point1.x() + point4.x() - point2.x(), point1.y() + point4.y() - point2.y());
                newResizeFPoints.point1 =  point1;
                newResizeFPoints.point2 = point2;
                newResizeFPoints.point3 = point3;
                newResizeFPoints.point4 = point4;
                return newResizeFPoints;
            }
        }
    }
    return newResizeFPoints;
}
/* point6 in the second position */
FourPoints point6Resize2(QPointF point1, QPointF point2, QPointF point3,
                         QPointF point4, QPointF pos, bool isShift) {
    FourPoints newResizeFPoints;
    newResizeFPoints.point1 = point1;
    newResizeFPoints.point2 = point2;
    newResizeFPoints.point3 = point3;
    newResizeFPoints.point4 = point4;
    QPointF point8 = QPointF((point2.x() + point4.x())/2, (point2.y() + point4.y())/2);

    if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) <= -TANT_EDGEVALUE &&
            (pos.y() - MIN_PADDING < point8.y()  || pointLineDir(point3, point4, pos) == -1 ||
             pointLineDir(point2, point4, pos) == -1)) {
        return newResizeFPoints;
    } else if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) > -TANT_EDGEVALUE &&
               (pos.x() + MIN_PADDING > point8.x() ||pointLineDir(point3, point4, pos) == -1 ||
                pointLineDir(point2, point4, pos) == -1)) {
        return newResizeFPoints;
    } else {
        if (pointToLineDistance(point2, point4, pos) < MIN_PADDING) {
            return newResizeFPoints;
        } else {
            if (!isShift) {
                if (pointLineDir(point1, point3, pos) == 1) {
                    qreal distance = pointToLineDistance(point1, point3, pos);
                    QPointF add = pointSplid(point1, point2, distance);
                    point1 = QPointF(point1.x() - add.x(), point1.y() + add.y());
                    add = pointSplid(point3, point4, distance);
                    point3 = QPointF(point3.x() - add.x(), point3.y() + add.y());
                } else {
                    qreal distance = pointToLineDistance(point1, point3, pos);
                    QPointF add = pointSplid(point1, point2, distance);
                    point1 = QPointF(point1.x() + add.x(), point1.y() - add.y());
                    add = pointSplid(point3, point4, distance);
                    point3 = QPointF(point3.x() + add.x(), point3.y() - add.y());
                }
                newResizeFPoints.point1 = point1;
                newResizeFPoints.point2 = point2;
                newResizeFPoints.point3 = point3;
                newResizeFPoints.point4 = point4;
                return newResizeFPoints;
            } else {
                qreal distance = pointToLineDistance(point1,  point3, pos);
                if (pointLineDir(point1, point3, pos) == -1) {
                    QPointF add = pointSplid(point1, point2, distance);
                    point1 = QPointF(point1.x() + add.x(), point1.y() - add.y());
                    add = pointSplid(point2, point4, distance);
                    point4 = QPointF(point4.x() + add.x(), point4.y() + add.y());
                } else {
                    QPointF add = pointSplid(point1, point2, distance);
                    point1 = QPointF(point1.x() - add.x(), point1.y() + add.y());
                    add = pointSplid(point2, point4, distance);
                    point4 = QPointF(point4.x() - add.x(), point4.y() - add.y());
                }
                point3 = QPointF(point1.x() + point4.x() - point2.x(), point1.y() + point4.y() - point2.y());
                newResizeFPoints.point1 =  point1;
                newResizeFPoints.point2 = point2;
                newResizeFPoints.point3 = point3;
                newResizeFPoints.point4 = point4;
                return newResizeFPoints;
            }
        }
    }
    return newResizeFPoints;
}
/* point6 in the third position */
FourPoints point6Resize3(QPointF point1, QPointF point2, QPointF point3,
                         QPointF point4, QPointF pos, bool isShift) {
    FourPoints newResizeFPoints;
    newResizeFPoints.point1 = point1;
    newResizeFPoints.point2 = point2;
    newResizeFPoints.point3 = point3;
    newResizeFPoints.point4 = point4;
    QPointF point8 = QPointF((point2.x() + point4.x())/2, (point2.y() + point4.y())/2);

    if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) >= TANT2_EDGEVALUE &&
            (pos.x() - MIN_PADDING < point8.x()  || pointLineDir(point3, point4, pos) == 1 ||
             pointLineDir(point2, point4, pos) == 1)) {
        return newResizeFPoints;
    } else if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) < TANT2_EDGEVALUE &&
               (pos.y() + MIN_PADDING > point8.y() ||pointLineDir(point3, point4, pos) == 1 ||
                pointLineDir(point2, point4, pos) == 1)) {
        return newResizeFPoints;
    } else {
        if (pointToLineDistance(point2, point4, pos) < MIN_PADDING) {
            return newResizeFPoints;
        } else {
            if (!isShift) {
                if (pointLineDir(point1, point3, pos) == -1) {
                    qreal distance = pointToLineDistance(point1, point3, pos);
                    QPointF add = pointSplid(point1, point2, distance);
                    point1 = QPointF(point1.x() + add.x(), point1.y() - add.y());
                    add = pointSplid(point3, point4, distance);
                    point3 = QPointF(point3.x() + add.x(), point3.y() - add.y());
                } else {
                    qreal distance = pointToLineDistance(point1, point3, pos);
                    QPointF add = pointSplid(point1, point2, distance);
                    point1 = QPointF(point1.x() - add.x(), point1.y() + add.y());
                    add = pointSplid(point3, point4, distance);
                    point3 = QPointF(point3.x() - add.x(), point3.y() + add.y());
                }
                newResizeFPoints.point1 = point1;
                newResizeFPoints.point2 = point2;
                newResizeFPoints.point3 = point3;
                newResizeFPoints.point4 = point4;
                return newResizeFPoints;
            } else {
                qreal distance = pointToLineDistance(point1,  point3, pos);
                if (pointLineDir(point1, point3, pos) == 1) {
                    QPointF add = pointSplid(point1, point2, distance);
                    point1 = QPointF(point1.x() - add.x(), point1.y() + add.y());
                    add = pointSplid(point2, point4, distance);
                    point4 = QPointF(point4.x() - add.x(), point4.y() - add.y());
                } else {
                    QPointF add = pointSplid(point1, point2, distance);
                    point1 = QPointF(point1.x() + add.x(), point1.y() - add.y());
                    add = pointSplid(point2, point4, distance);
                    point4 = QPointF(point4.x() + add.x(), point4.y() + add.y());
                }
                point3 = QPointF(point1.x() + point4.x() - point2.x(), point1.y() + point4.y() - point2.y());
                newResizeFPoints.point1 =  point1;
                newResizeFPoints.point2 = point2;
                newResizeFPoints.point3 = point3;
                newResizeFPoints.point4 = point4;
                return newResizeFPoints;
            }
        }
    }
    return newResizeFPoints;
}
/* point6 in the fourth position */
FourPoints point6Resize4(QPointF point1, QPointF point2, QPointF point3,
                         QPointF point4, QPointF pos, bool isShift) {
    FourPoints newResizeFPoints;
    newResizeFPoints.point1 = point1;
    newResizeFPoints.point2 = point2;
    newResizeFPoints.point3 = point3;
    newResizeFPoints.point4 = point4;
    QPointF point8 = QPointF((point2.x() + point4.x())/2, (point2.y() + point4.y())/2);

    if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) >= -TANT2_EDGEVALUE &&
            (pos.y() - MIN_PADDING < point8.y()  || pointLineDir(point3, point4, pos) == 1 ||
             pointLineDir(point2, point4, pos) == -1)) {
        return newResizeFPoints;
    } else if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) < -TANT2_EDGEVALUE &&
               (pos.x() - MIN_PADDING < point8.x() ||pointLineDir(point3, point4, pos) == 1 ||
                pointLineDir(point2, point4, pos) == -1)) {
        return newResizeFPoints;
    } else {
        if (pointToLineDistance(point2, point4, pos) < MIN_PADDING) {
            return newResizeFPoints;
        } else {
            if (!isShift) {
                if (pointLineDir(point1, point3, pos) == -1) {
                    qreal distance = pointToLineDistance(point1, point3, pos);
                    QPointF add = pointSplid(point1, point2, distance);
                    point1 = QPointF(point1.x() - add.x(), point1.y() - add.y());
                    add = pointSplid(point3, point4, distance);
                    point3 = QPointF(point3.x() - add.x(), point3.y() - add.y());
                } else {
                    qreal distance = pointToLineDistance(point1, point3, pos);
                    QPointF add = pointSplid(point1, point2, distance);
                    point1 = QPointF(point1.x() + add.x(), point1.y() + add.y());
                    add = pointSplid(point3, point4, distance);
                    point3 = QPointF(point3.x() + add.x(), point3.y() + add.y());
                }
                newResizeFPoints.point1 = point1;
                newResizeFPoints.point2 = point2;
                newResizeFPoints.point3 = point3;
                newResizeFPoints.point4 = point4;
                return newResizeFPoints;
            } else {
                qreal distance = pointToLineDistance(point1,  point3, pos);
                if (pointLineDir(point1, point3, pos) == -1) {
                    QPointF add = pointSplid(point1, point2, distance);
                    point1 = QPointF(point1.x() - add.x(), point1.y() - add.y());
                    add = pointSplid(point2, point4, distance);
                    point4 = QPointF(point4.x() + add.x(), point4.y() - add.y());
                } else {
                    QPointF add = pointSplid(point1, point2, distance);
                    point1 = QPointF(point1.x() + add.x(), point1.y() + add.y());
                    add = pointSplid(point2, point4, distance);
                    point4 = QPointF(point4.x() - add.x(), point4.y() + add.y());
                }
                point3 = QPointF(point1.x() + point4.x() - point2.x(), point1.y() + point4.y() - point2.y());
                newResizeFPoints.point1 =  point1;
                newResizeFPoints.point2 = point2;
                newResizeFPoints.point3 = point3;
                newResizeFPoints.point4 = point4;
                return newResizeFPoints;
            }
        }
    }
    return newResizeFPoints;
}
/* point6 in the fifth position */
FourPoints point6Resize5(QPointF point1, QPointF point2, QPointF point3,
                         QPointF point4, QPointF pos, bool isShift) {
    FourPoints newResizeFPoints;
    newResizeFPoints.point1 = point1;
    newResizeFPoints.point2 = point2;
    newResizeFPoints.point3 = point3;
    newResizeFPoints.point4 = point4;
    QPointF point8 = QPointF((point2.x() + point4.x())/2, (point2.y() + point4.y())/2);

    if (pos.y() + MIN_PADDING > point8.y()) {
        return newResizeFPoints;
    } else {
        if (!isShift) {
            if (pointLineDir(point1, point3, pos) == -1) {
                qreal distance = pointToLineDistance(point1, point3, pos);
                QPointF add = pointSplid(point1, point2, distance);
                point1 = QPointF(point1.x() - add.x(), point1.y() - add.y());
                add = pointSplid(point3, point4, distance);
                point3 = QPointF(point3.x() - add.x(), point3.y() - add.y());
            } else {
                qreal distance = pointToLineDistance(point1, point3, pos);
                QPointF add = pointSplid(point1, point2, distance);
                point1 = QPointF(point1.x() + add.x(), point1.y() + add.y());
                add = pointSplid(point3, point4, distance);
                point3 = QPointF(point3.x() + add.x(), point3.y() + add.y());
            }
            newResizeFPoints.point1 = point1;
            newResizeFPoints.point2 = point2;
            newResizeFPoints.point3 = point3;
            newResizeFPoints.point4 = point4;
            return newResizeFPoints;
        } else {
            qreal distance = pointToLineDistance(point1,  point3, pos);
            if (pointLineDir(point1, point3, pos) == 1) {
                QPointF add = pointSplid(point1, point2, distance);
                point1 = QPointF(point1.x() + add.x(), point1.y() + add.y());
                add = pointSplid(point2, point4, distance);
                point4 = QPointF(point4.x() - add.x(), point4.y() + add.y());
            } else {
                QPointF add = pointSplid(point1, point2, distance);
                point1 = QPointF(point1.x() - add.x(), point1.y() + add.y());
                add = pointSplid(point2, point4, distance);
                point4 = QPointF(point3.x() + add.x(), point4.y() - add.y());
            }
            point1 = QPointF(point2.x() + point3.x() - point4.x(), point2.y() + point3.y() - point4.y());
            newResizeFPoints.point1 =  point1;
            newResizeFPoints.point2 = point2;
            newResizeFPoints.point3 = point3;
            newResizeFPoints.point4 = point4;
            return newResizeFPoints;
        }
    }
    return newResizeFPoints;
}
/* point6 in the sixth position */
FourPoints point6Resize6(QPointF point1, QPointF point2, QPointF point3,
                         QPointF point4, QPointF pos, bool isShift) {
    Q_UNUSED(pos);
    Q_UNUSED(isShift);
    FourPoints newResizeFPoints;
    newResizeFPoints.point1 =  point1;
    newResizeFPoints.point2 = point2;
    newResizeFPoints.point3 = point3;
    newResizeFPoints.point4 = point4;
    return newResizeFPoints;
}
/* point6 in the seventh position */
FourPoints point6Resize7(QPointF point1, QPointF point2, QPointF point3,
                         QPointF point4, QPointF pos, bool isShift) {
    Q_UNUSED(pos);
    Q_UNUSED(isShift);
    FourPoints newResizeFPoints;
    newResizeFPoints.point1 =  point1;
    newResizeFPoints.point2 = point2;
    newResizeFPoints.point3 = point3;
    newResizeFPoints.point4 = point4;
    return newResizeFPoints;
}

/********************** seventh point7 ************************/
/* point7 in the first position */
FourPoints point7Resize1(QPointF point1, QPointF point2, QPointF point3,
                         QPointF point4, QPointF pos, bool isShift) {
    FourPoints newResizeFPoints;
    newResizeFPoints.point1 = point1;
    newResizeFPoints.point2 = point2;
    newResizeFPoints.point3 = point3;
    newResizeFPoints.point4 = point4;
    QPointF point5 = QPointF((point2.x() + point1.x())/2, (point2.y() + point1.y())/2);

    if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) >= TANT_EDGEVALUE &&
            (pos.x() - MIN_PADDING < point5.x()  || pointLineDir(point1, point2, pos) == 1 ||
             pointLineDir(point2, point4, pos) == 1)) {
        return newResizeFPoints;
    } else if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) < TANT_EDGEVALUE &&
               (pos.y() + MIN_PADDING > point5.y() ||pointLineDir(point1, point2, pos) == 1 ||
                pointLineDir(point2, point4, pos) == 1)) {
        return newResizeFPoints;
    } else {
        if (pointToLineDistance(point2, point1, pos) < MIN_PADDING) {
            return newResizeFPoints;
        } else {
            if (!isShift) {
                if (pos.x()  < (point1.x() + point2.x())/2 || pos.y() > (point1.y() + point2.y())/2) {
                    return newResizeFPoints;
                } else {
                    if (pointLineDir(point3, point4, pos) == -1) {
                        qreal distance = pointToLineDistance(point3, point4, pos);
                        QPointF add = pointSplid(point1, point3, distance);
                        point3 = QPointF(point3.x() + add.x(), point3.y() - add.y());
                        add = pointSplid(point2, point4, distance);
                        point4 = QPointF(point4.x() + add.x(), point4.y() - add.y());
                    } else {
                        qreal distance = pointToLineDistance(point3, point4, pos);
                        QPointF add = pointSplid(point1, point3, distance);
                        point3 = QPointF(point3.x() - add.x(), point3.y() + add.y());
                        add = pointSplid(point2, point4, distance);
                        point4 = QPointF(point4.x() - add.x(), point4.y() + add.y());
                    }
                }
                newResizeFPoints.point1 = point1;
                newResizeFPoints.point2 = point2;
                newResizeFPoints.point3 = point3;
                newResizeFPoints.point4 = point4;
                return newResizeFPoints;
            } else {
                qreal distance = pointToLineDistance(point3,  point4, pos);
                if (pointLineDir(point3, point4, pos) == -1) {
                    QPointF add = pointSplid(point1, point2, distance);
                    point2 = QPointF(point2.x() - add.x(), point2.y() - add.y());
                    add = pointSplid(point1, point3, distance);
                    point3 = QPointF(point3.x() - add.x(), point3.y() + add.y());
                } else {
                    QPointF add = pointSplid(point1, point2, distance);
                    point2 = QPointF(point2.x() + add.x(), point2.y() + add.y());
                    add = pointSplid(point1, point3, distance);
                    point3 = QPointF(point3.x() + add.x(), point3.y() - add.y());
                }
                point3 = QPointF(point1.x() + point4.x() - point2.x(), point1.y() + point4.y() - point2.y());
                newResizeFPoints.point1 =  point1;
                newResizeFPoints.point2 = point2;
                newResizeFPoints.point3 = point3;
                newResizeFPoints.point4 = point4;
                return newResizeFPoints;
            }
        }
    }
    return newResizeFPoints;
}
/* point7 in the second position */
FourPoints point7Resize2(QPointF point1, QPointF point2, QPointF point3,
                         QPointF point4, QPointF pos, bool isShift) {
    FourPoints newResizeFPoints;
    newResizeFPoints.point1 = point1;
    newResizeFPoints.point2 = point2;
    newResizeFPoints.point3 = point3;
    newResizeFPoints.point4 = point4;
    QPointF point5 = QPointF((point2.x() + point1.x())/2, (point2.y() + point1.y())/2);

    if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) <= -TANT_EDGEVALUE &&
            (pos.x() + MIN_PADDING > point5.x()  || pointLineDir(point1, point2, pos) == 1 ||
             pointLineDir(point2, point4, pos) == -1)) {
        return newResizeFPoints;
    } else if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) > -TANT_EDGEVALUE &&
               (pos.y() + MIN_PADDING > point5.y() ||pointLineDir(point1, point2, pos) == 1 ||
                pointLineDir(point2, point4, pos) == -1)) {
        return newResizeFPoints;
    } else {
        if (pointToLineDistance(point2, point1, pos) < MIN_PADDING) {
            return newResizeFPoints;
        } else {
            if (!isShift) {
                if (pointLineDir(point3, point4, pos) == -1) {
                    qreal distance = pointToLineDistance(point3, point4, pos);
                    QPointF add = pointSplid(point1, point3, distance);
                    point3 = QPointF(point3.x() - add.x(), point3.y() - add.y());
                    add = pointSplid(point2, point4, distance);
                    point4 = QPointF(point4.x() - add.x(), point4.y() - add.y());
                } else {
                    qreal distance = pointToLineDistance(point3, point4, pos);
                    QPointF add = pointSplid(point1, point3, distance);
                    point3 = QPointF(point3.x() + add.x(), point3.y() + add.y());
                    add = pointSplid(point2, point4, distance);
                    point4 = QPointF(point4.x() + add.x(), point4.y() + add.y());
                }
                newResizeFPoints.point1 = point1;
                newResizeFPoints.point2 = point2;
                newResizeFPoints.point3 = point3;
                newResizeFPoints.point4 = point4;
                return newResizeFPoints;
            } else {
                qreal distance = pointToLineDistance(point1,  point3, pos);
                if (pointLineDir(point3, point4, pos) == 1) {
                    QPointF add = pointSplid(point1, point2, distance);
                    point2 = QPointF(point2.x() - add.x(), point2.y() + add.y());
                    add = pointSplid(point1, point3, distance);
                    point3 = QPointF(point3.x() + add.x(), point3.y() + add.y());
                } else {
                    QPointF add = pointSplid(point1, point2, distance);
                    point2 = QPointF(point2.x() + add.x(), point2.y() - add.y());
                    add = pointSplid(point1, point3, distance);
                    point3 = QPointF(point3.x() - add.x(), point3.y() - add.y());
                }
                point4 = QPointF(point2.x() + point3.x() - point1.x(), point2.y() + point3.y() - point1.y());
                newResizeFPoints.point1 =  point1;
                newResizeFPoints.point2 = point2;
                newResizeFPoints.point3 = point3;
                newResizeFPoints.point4 = point4;
                return newResizeFPoints;
            }
        }
    }
    return newResizeFPoints;
}
/* point7 in the third position */
FourPoints point7Resize3(QPointF point1, QPointF point2, QPointF point3,
                         QPointF point4, QPointF pos, bool isShift) {
    FourPoints newResizeFPoints;
    newResizeFPoints.point1 = point1;
    newResizeFPoints.point2 = point2;
    newResizeFPoints.point3 = point3;
    newResizeFPoints.point4 = point4;
    QPointF point5 = QPointF((point2.x() + point1.x())/2, (point2.y() + point1.y())/2);

    if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) >= TANT2_EDGEVALUE &&
            (pos.y() - MIN_PADDING < point5.y()|| pointLineDir(point1, point2, pos) == -1 ||
             pointLineDir(point2, point4, pos) == 1)) {
        return newResizeFPoints;
    } else if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) < TANT2_EDGEVALUE &&
               (pos.x() - MIN_PADDING < point5.x() ||pointLineDir(point1, point2, pos) == -1 ||
                pointLineDir(point2, point4, pos) == 1)) {
        return newResizeFPoints;
    } else {
        if (pointToLineDistance(point2, point1, pos) < MIN_PADDING) {
            return newResizeFPoints;
        } else {
            if (!isShift) {
                if (pointLineDir(point3, point4, pos) == 1) {
                    qreal distance = pointToLineDistance(point3, point4, pos);
                    QPointF add = pointSplid(point1, point3, distance);
                    point3 = QPointF(point3.x() + add.x(), point3.y() + add.y());
                    add = pointSplid(point2, point4, distance);
                    point4 = QPointF(point4.x() + add.x(), point4.y() + add.y());
                } else {
                    qreal distance = pointToLineDistance(point3, point4, pos);
                    QPointF add = pointSplid(point1, point3, distance);
                    point3 = QPointF(point3.x() - add.x(), point3.y() - add.y());
                    add = pointSplid(point2, point4, distance);
                    point4 = QPointF(point4.x() - add.x(), point4.y() - add.y());
                }
                newResizeFPoints.point1 = point1;
                newResizeFPoints.point2 = point2;
                newResizeFPoints.point3 = point3;
                newResizeFPoints.point4 = point4;
                return newResizeFPoints;
            } else {
                qreal distance = pointToLineDistance(point3,  point4, pos);
                if (pointLineDir(point3, point4, pos) == -1) {
                    QPointF add = pointSplid(point1, point2, distance);
                    point2 = QPointF(point2.x() + add.x(), point2.y() - add.y());
                    add = pointSplid(point1, point3, distance);
                    point3 = QPointF(point3.x() - add.x(), point3.y() - add.y());
                } else {
                    QPointF add = pointSplid(point1, point2, distance);
                    point2 = QPointF(point2.x() - add.x(), point2.y() + add.y());
                    add = pointSplid(point1, point3, distance);
                    point3 = QPointF(point3.x() + add.x(), point3.y() + add.y());
                }
                point4 = QPointF(point2.x() + point3.x() - point1.x(), point2.y() + point3.y() - point1.y());
                newResizeFPoints.point1 =  point1;
                newResizeFPoints.point2 = point2;
                newResizeFPoints.point3 = point3;
                newResizeFPoints.point4 = point4;
                return newResizeFPoints;
            }
        }
    }
    return newResizeFPoints;
}
/* point7 in the fourth position */
FourPoints point7Resize4(QPointF point1, QPointF point2, QPointF point3,
                         QPointF point4, QPointF pos, bool isShift) {
    FourPoints newResizeFPoints;
    newResizeFPoints.point1 = point1;
    newResizeFPoints.point2 = point2;
    newResizeFPoints.point3 = point3;
    newResizeFPoints.point4 = point4;
    QPointF point5 = QPointF((point2.x() + point4.x())/2, (point2.y() + point4.y())/2);

    if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) >= -TANT2_EDGEVALUE &&
            (pos.x() + MIN_PADDING > point5.x()  || pointLineDir(point1, point2, pos) == -1 ||
             pointLineDir(point2, point4, pos) == -1)) {
        return newResizeFPoints;
    } else if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) < -TANT2_EDGEVALUE &&
               (pos.y() - MIN_PADDING < point5.y() ||pointLineDir(point1, point2, pos) == -1 ||
                pointLineDir(point2, point4, pos) == -1)) {
        return newResizeFPoints;
    } else {
        if (pointToLineDistance(point2, point1, pos) < MIN_PADDING) {
            return newResizeFPoints;
        } else {
            if (!isShift) {
                if (pointLineDir(point3, point4, pos) == -1) {
                    qreal distance = pointToLineDistance(point3, point4, pos);
                    QPointF add = pointSplid(point1, point3, distance);
                    point3 = QPointF(point3.x() + add.x(), point3.y() - add.y());
                    add = pointSplid(point2, point4, distance);
                    point4 = QPointF(point4.x() + add.x(), point4.y() - add.y());
                } else {
                    qreal distance = pointToLineDistance(point3, point4, pos);
                    QPointF add = pointSplid(point1, point3, distance);
                    point3 = QPointF(point3.x() - add.x(), point3.y() + add.y());
                    add = pointSplid(point2, point4, distance);
                    point4 = QPointF(point4.x() - add.x(), point4.y() + add.y());
                }
                newResizeFPoints.point1 = point1;
                newResizeFPoints.point2 = point2;
                newResizeFPoints.point3 = point3;
                newResizeFPoints.point4 = point4;
                return newResizeFPoints;
            } else {
                qreal distance = pointToLineDistance(point3,  point4, pos);
                if (pointLineDir(point3, point4, pos) == -1) {
                    QPointF add = pointSplid(point1, point2, distance);
                    point2 = QPointF(point2.x() + add.x(), point2.y() + add.y());
                    add = pointSplid(point1, point3, distance);
                    point3 = QPointF(point3.x() + add.x(), point3.y() - add.y());
                } else {
                    QPointF add = pointSplid(point1, point2, distance);
                    point2 = QPointF(point2.x() - add.x(), point2.y() - add.y());
                    add = pointSplid(point1, point3, distance);
                    point3 = QPointF(point3.x() - add.x(), point3.y() + add.y());
                }
                point4 = QPointF(point2.x() + point3.x() - point1.x(), point2.y() + point3.y() - point1.y());
                newResizeFPoints.point1 =  point1;
                newResizeFPoints.point2 = point2;
                newResizeFPoints.point3 = point3;
                newResizeFPoints.point4 = point4;
                return newResizeFPoints;
            }
        }
    }
    return newResizeFPoints;
}
/* point7 in the fifth position */
FourPoints point7Resize5(QPointF point1, QPointF point2, QPointF point3,
                         QPointF point4, QPointF pos, bool isShift) {
    FourPoints newResizeFPoints;
    newResizeFPoints.point1 = point1;
    newResizeFPoints.point2 = point2;
    newResizeFPoints.point3 = point3;
    newResizeFPoints.point4 = point4;
    QPointF point5 = QPointF((point2.x() + point1.x())/2, (point2.y() + point1.y())/2);

    if (pos.x() - MIN_PADDING < point5.x()) {
        return newResizeFPoints;
    } else {
        if (!isShift) {
            if (pointLineDir(point3, point4, pos) == 1) {
                qreal distance = pointToLineDistance(point3, point4, pos);
                QPointF add = pointSplid(point1, point3, distance);
                point3 = QPointF(point3.x() + add.x(), point3.y() - add.y());
                add = pointSplid(point2, point4, distance);
                point4 = QPointF(point4.x() + add.x(), point4.y() - add.y());
            } else {
                qreal distance = pointToLineDistance(point3, point4, pos);
                QPointF add = pointSplid(point1, point3, distance);
                point3 = QPointF(point3.x() - add.x(), point3.y() + add.y());
                add = pointSplid(point2, point4, distance);
                point4 = QPointF(point4.x() - add.x(), point4.y() + add.y());
            }
            newResizeFPoints.point1 = point1;
            newResizeFPoints.point2 = point2;
            newResizeFPoints.point3 = point3;
            newResizeFPoints.point4 = point4;
            return newResizeFPoints;
        } else {
            qreal distance = pointToLineDistance(point3,  point4, pos);
            if (pointLineDir(point3, point4, pos) == -1) {
                QPointF add = pointSplid(point1, point2, distance);
                point2 = QPointF(point2.x() - add.x(), point2.y() - add.y());
                add = pointSplid(point1, point3, distance);
                point3 = QPointF(point3.x() - add.x(), point3.y() + add.y());
            } else {
                QPointF add = pointSplid(point1, point2, distance);
                point2 = QPointF(point2.x() + add.x(), point2.y() + add.y());
                add = pointSplid(point1, point3, distance);
                point3 = QPointF(point3.x() + add.x(), point3.y() - add.y());
            }
            point4 = QPointF(point2.x() + point3.x() - point1.x(), point2.y() + point3.y() - point1.y());
            newResizeFPoints.point1 =  point1;
            newResizeFPoints.point2 = point2;
            newResizeFPoints.point3 = point3;
            newResizeFPoints.point4 = point4;
            return newResizeFPoints;
        }
    }
    return newResizeFPoints;
}
/* point7 in the sixth position */
FourPoints point7Resize6(QPointF point1, QPointF point2, QPointF point3,
                         QPointF point4, QPointF pos, bool isShift) {
    Q_UNUSED(pos);
    Q_UNUSED(isShift);
    FourPoints newResizeFPoints;
    newResizeFPoints.point1 =  point1;
    newResizeFPoints.point2 = point2;
    newResizeFPoints.point3 = point3;
    newResizeFPoints.point4 = point4;
    return newResizeFPoints;
}
/* point6 in the seventh position */
FourPoints point7Resize7(QPointF point1, QPointF point2, QPointF point3,
                         QPointF point4, QPointF pos, bool isShift) {
    Q_UNUSED(pos);
    Q_UNUSED(isShift);
    FourPoints newResizeFPoints;
    newResizeFPoints.point1 =  point1;
    newResizeFPoints.point2 = point2;
    newResizeFPoints.point3 = point3;
    newResizeFPoints.point4 = point4;
    return newResizeFPoints;
}

/********************** eighth point8 ************************/
/* point8 in the first position */
FourPoints point8Resize1(QPointF point1, QPointF point2, QPointF point3,
                         QPointF point4, QPointF pos, bool isShift) {
    FourPoints newResizeFPoints;
    newResizeFPoints.point1 = point1;
    newResizeFPoints.point2 = point2;
    newResizeFPoints.point3 = point3;
    newResizeFPoints.point4 = point4;
    QPointF point6 = QPointF((point3.x() + point1.x())/2, (point3.y() + point1.y())/2);

    if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) >= TANT_EDGEVALUE &&
            (pos.y() - MIN_PADDING < point6.y()  || pointLineDir(point1, point3, pos) == -1 ||
             pointLineDir(point3, point4, pos) == -1)) {
        return newResizeFPoints;
    } else if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) < TANT_EDGEVALUE &&
               (pos.x() - MIN_PADDING < point6.x() ||pointLineDir(point1, point3, pos) == -1 ||
                pointLineDir(point3, point4, pos) == -1)) {
        return newResizeFPoints;
    } else {
        if (pointToLineDistance(point1, point3, pos) < MIN_PADDING) {
            return newResizeFPoints;
        } else {
            if (pointToLineDistance(point1, point3, pos) < MIN_PADDING) {
                return newResizeFPoints;
            } else {
                if (!isShift) {
                    if (pos.y()  <= (point1.y() + point3.y())/2) {
                        return newResizeFPoints;
                    } else {
                        if (pointLineDir(point2, point4, pos) == 1) {
                            qreal distance = pointToLineDistance(point2, point4, pos);
                            QPointF add = pointSplid(point1, point2, distance);
                            point2 = QPointF(point2.x() + add.x(), point2.y() + add.y());
                            add = pointSplid(point3, point4, distance);
                            point4 = QPointF(point4.x() + add.x(), point4.y() + add.y());
                        } else {
                            qreal distance = pointToLineDistance(point2, point4, pos);
                            QPointF add = pointSplid(point1, point2, distance);
                            point2 = QPointF(point2.x() - add.x(), point2.y() - add.y());
                            add = pointSplid(point3, point4, distance);
                            point4 = QPointF(point4.x() - add.x(), point4.y() - add.y());
                        }
                    }
                    newResizeFPoints.point1 = point1;
                    newResizeFPoints.point2 = point2;
                    newResizeFPoints.point3 = point3;
                    newResizeFPoints.point4 = point4;
                    return newResizeFPoints;
                } else {
                    qreal distance = pointToLineDistance(point2,  point4, pos);
                    if (pointLineDir(point2, point4, pos) == -1) {
                        QPointF add = pointSplid(point1, point3, distance);
                        point1 = QPointF(point1.x() + add.x(), point1.y() - add.y());
                        add = pointSplid(point1, point3, distance);
                        point4 = QPointF(point4.x() - add.x(), point4.y() - add.y());
                    } else {
                        QPointF add = pointSplid(point1, point3, distance);
                        point1 = QPointF(point1.x() - add.x(), point1.y() + add.y());
                        add = pointSplid(point1, point3, distance);
                        point4 = QPointF(point4.x() + add.x(), point4.y() + add.y());
                    }
                    point2 = QPointF(point1.x() + point4.x() - point3.x(), point1.y() + point4.y() - point3.y());
                    newResizeFPoints.point1 =  point1;
                    newResizeFPoints.point2 = point2;
                    newResizeFPoints.point3 = point3;
                    newResizeFPoints.point4 = point4;
                    return newResizeFPoints;
                }
            }
        }
    }
    return newResizeFPoints;
}
/* point8 in the second position */
FourPoints point8Resize2(QPointF point1, QPointF point2, QPointF point3,
                         QPointF point4, QPointF pos, bool isShift) {
    FourPoints newResizeFPoints;
    newResizeFPoints.point1 = point1;
    newResizeFPoints.point2 = point2;
    newResizeFPoints.point3 = point3;
    newResizeFPoints.point4 = point4;
    QPointF point6 = QPointF((point3.x() + point1.x())/2, (point3.y() + point1.y())/2);

    if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) <= -TANT_EDGEVALUE &&
            (pos.y() + MIN_PADDING > point6.y()  || pointLineDir(point1, point3, pos) == 1 ||
             pointLineDir(point3, point4, pos) == -1)) {
        return newResizeFPoints;
    } else if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) > -TANT_EDGEVALUE &&
               (pos.x() - MIN_PADDING < point6.x() ||pointLineDir(point1, point3, pos) == 1 ||
                pointLineDir(point3, point4, pos) == -1)) {
        return newResizeFPoints;
    } else {
        if (pointToLineDistance(point1, point3, pos) < MIN_PADDING) {
            return newResizeFPoints;
        } else {
            if (!isShift) {
                if (pointLineDir(point2, point4, pos) == -1) {
                    qreal distance = pointToLineDistance(point2, point4, pos);
                    QPointF add = pointSplid(point1, point2, distance);
                    point2 = QPointF(point2.x() + add.x(), point2.y() - add.y());
                    add = pointSplid(point3, point4, distance);
                    point4 = QPointF(point4.x() + add.x(), point4.y() - add.y());
                } else {
                    qreal distance = pointToLineDistance(point2, point4, pos);
                    QPointF add = pointSplid(point1, point2, distance);
                    point2 = QPointF(point2.x() - add.x(), point2.y() + add.y());
                    add = pointSplid(point3, point4, distance);
                    point4 = QPointF(point4.x() - add.x(), point4.y() + add.y());
                }
                newResizeFPoints.point1 = point1;
                newResizeFPoints.point2 = point2;
                newResizeFPoints.point3 = point3;
                newResizeFPoints.point4 = point4;
                return newResizeFPoints;
            } else {
                qreal distance = pointToLineDistance(point2,  point4, pos);
                if (pointLineDir(point2, point4, pos) == 1) {
                    QPointF add = pointSplid(point1, point2, distance);
                    point1 = QPointF(point1.x() - add.x(), point1.y() - add.y());
                    add = pointSplid(point3, point4, distance);
                    point4 = QPointF(point4.x() - add.x(), point4.y() + add.y());
                } else {
                    QPointF add = pointSplid(point1, point2, distance);
                    point1 = QPointF(point1.x() + add.x(), point1.y() + add.y());
                    add = pointSplid(point3, point4, distance);
                    point4 = QPointF(point4.x() + add.x(), point4.y() - add.y());
                }
                point2 = QPointF(point1.x() + point4.x() - point3.x(), point1.y() + point4.y() - point3.y());
                newResizeFPoints.point1 =  point1;
                newResizeFPoints.point2 = point2;
                newResizeFPoints.point3 = point3;
                newResizeFPoints.point4 = point4;
                return newResizeFPoints;
            }
        }
    }
    return newResizeFPoints;
}
/* point8 in the third position */
FourPoints point8Resize3(QPointF point1, QPointF point2, QPointF point3,
                         QPointF point4, QPointF pos, bool isShift) {
    FourPoints newResizeFPoints;
    newResizeFPoints.point1 = point1;
    newResizeFPoints.point2 = point2;
    newResizeFPoints.point3 = point3;
    newResizeFPoints.point4 = point4;
    QPointF point6 = QPointF((point3.x() + point1.x())/2, (point3.y() + point1.y())/2);

    if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) >= TANT2_EDGEVALUE &&
            (pos.x() + MIN_PADDING > point6.x()  || pointLineDir(point1, point3, pos) == -1 ||
             pointLineDir(point3, point4, pos) == 1)) {
        return newResizeFPoints;
    } else if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) < TANT2_EDGEVALUE &&
               (pos.y() - MIN_PADDING < point6.y() ||pointLineDir(point1, point3, pos) == -1 ||
                pointLineDir(point3, point4, pos) == 1)) {
        return newResizeFPoints;
    } else {
        if (pointToLineDistance(point1, point3, pos) < MIN_PADDING) {
            return newResizeFPoints;
        } else {
            if (!isShift) {
                if (pointLineDir(point2, point4, pos) == -1) {
                    qreal distance = pointToLineDistance(point2, point4, pos);
                    QPointF add = pointSplid(point1, point2, distance);
                    point2 = QPointF(point2.x() + add.x(), point2.y() - add.y());
                    add = pointSplid(point3, point4, distance);
                    point4 = QPointF(point4.x() + add.x(), point4.y() - add.y());
                } else {
                    qreal distance = pointToLineDistance(point2, point4, pos);
                    QPointF add = pointSplid(point1, point2, distance);
                    point2 = QPointF(point2.x() - add.x(), point2.y() + add.y());
                    add = pointSplid(point3, point4, distance);
                    point4 = QPointF(point4.x() - add.x(), point4.y() + add.y());
                }
                newResizeFPoints.point1 = point1;
                newResizeFPoints.point2 = point2;
                newResizeFPoints.point3 = point3;
                newResizeFPoints.point4 = point4;
                return newResizeFPoints;
            } else {
                qreal distance = pointToLineDistance(point2,  point4, pos);
                if (pointLineDir(point2, point4, pos) == 1) {
                    QPointF add = pointSplid(point1, point3, distance);
                    point1 = QPointF(point1.x() - add.x(), point1.y() - add.y());
                    add = pointSplid(point2, point4, distance);
                    point4 = QPointF(point4.x() - add.x(), point4.y() + add.y());
                } else {
                    QPointF add = pointSplid(point1, point2, distance);
                    point1 = QPointF(point1.x() + add.x(), point1.y() + add.y());
                    add = pointSplid(point2, point4, distance);
                    point4 = QPointF(point4.x() + add.x(), point4.y() - add.y());
                }
                point2 = QPointF(point1.x() + point4.x() - point3.x(), point1.y() + point4.y() - point3.y());
                newResizeFPoints.point1 =  point1;
                newResizeFPoints.point2 = point2;
                newResizeFPoints.point3 = point3;
                newResizeFPoints.point4 = point4;
                return newResizeFPoints;
            }
        }
    }
    return newResizeFPoints;
}
/* point8 in the fourth position */
FourPoints point8Resize4(QPointF point1, QPointF point2, QPointF point3,
                         QPointF point4, QPointF pos, bool isShift) {
    FourPoints newResizeFPoints;
    newResizeFPoints.point1 = point1;
    newResizeFPoints.point2 = point2;
    newResizeFPoints.point3 = point3;
    newResizeFPoints.point4 = point4;
    QPointF point6 = QPointF((point3.x() + point1.x())/2, (point3.y() + point1.y())/2);

    if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) >= -TANT2_EDGEVALUE &&
            (pos.y() + MIN_PADDING > point6.y()  || pointLineDir(point1, point3, pos) == 1 ||
             pointLineDir(point3, point4, pos) == 1)) {
        return newResizeFPoints;
    } else if (std::atan2((point2.y() - point1.y()), (point2.x() - point1.x())) < -TANT2_EDGEVALUE &&
               (pos.x() + MIN_PADDING > point6.x() ||pointLineDir(point1, point3, pos) == 1 ||
                pointLineDir(point3, point4, pos) == 1)) {
        return newResizeFPoints;
    } else {
        if (pointToLineDistance(point3, point1, pos) < MIN_PADDING) {
            return newResizeFPoints;
        } else {
            if (!isShift) {
                if (pointLineDir(point2, point4, pos) == -1) {
                    qreal distance = pointToLineDistance(point2, point4, pos);
                    QPointF add = pointSplid(point1, point2, distance);
                    point2 = QPointF(point2.x() - add.x(), point2.y() - add.y());
                    add = pointSplid(point3, point4, distance);
                    point4 = QPointF(point4.x() - add.x(), point4.y() - add.y());
                } else {
                    qreal distance = pointToLineDistance(point2, point4, pos);
                    QPointF add = pointSplid(point1, point2, distance);
                    point2 = QPointF(point2.x() + add.x(), point2.y() + add.y());
                    add = pointSplid(point3, point4, distance);
                    point4 = QPointF(point4.x() + add.x(), point4.y() + add.y());
                }
                newResizeFPoints.point1 = point1;
                newResizeFPoints.point2 = point2;
                newResizeFPoints.point3 = point3;
                newResizeFPoints.point4 = point4;
                return newResizeFPoints;
            } else {
                qreal distance = pointToLineDistance(point2,  point4, pos);
                if (pointLineDir(point2, point4, pos) == 1) {
                    QPointF add = pointSplid(point1, point3, distance);
                    point1 = QPointF(point1.x() - add.x(), point1.y() + add.y());
                    add = pointSplid(point1, point3, distance);
                    point4 = QPointF(point4.x() + add.x(), point4.y() + add.y());
                } else {
                    QPointF add = pointSplid(point1, point3, distance);
                    point1 = QPointF(point1.x() + add.x(), point1.y() - add.y());
                    add = pointSplid(point1, point3, distance);
                    point4 = QPointF(point4.x() - add.x(), point4.y() - add.y());
                }
                point2 = QPointF(point1.x() + point4.x() - point3.x(), point1.y() + point4.y() - point3.y());
                newResizeFPoints.point1 =  point1;
                newResizeFPoints.point2 = point2;
                newResizeFPoints.point3 = point3;
                newResizeFPoints.point4 = point4;
                return newResizeFPoints;
            }
        }
    }
    return newResizeFPoints;
}
/* point8 in the fifth position */
FourPoints point8Resize5(QPointF point1, QPointF point2, QPointF point3,
                         QPointF point4, QPointF pos, bool isShift) {
    FourPoints newResizeFPoints;
    newResizeFPoints.point1 = point1;
    newResizeFPoints.point2 = point2;
    newResizeFPoints.point3 = point3;
    newResizeFPoints.point4 = point4;
    QPointF point6 = QPointF((point3.x() + point1.x())/2, (point3.y() + point1.y())/2);

    if (pos.y() - MIN_PADDING < point6.y()) {
        return newResizeFPoints;
    } else {
        if (!isShift) {
            if (pointLineDir(point2, point4, pos) == 1) {
                qreal distance = pointToLineDistance(point2, point4, pos);
                QPointF add = pointSplid(point1, point2, distance);
                point2 = QPointF(point2.x() + add.x(), point2.y() + add.y());
                add = pointSplid(point3, point4, distance);
                point4 = QPointF(point4.x() + add.x(), point4.y() + add.y());
            } else {
                qreal distance = pointToLineDistance(point2, point4, pos);
                QPointF add = pointSplid(point1, point2, distance);
                point2 = QPointF(point2.x() - add.x(), point2.y() - add.y());
                add = pointSplid(point3, point4, distance);
                point4 = QPointF(point4.x() - add.x(), point4.y() - add.y());
            }
            newResizeFPoints.point1 = point1;
            newResizeFPoints.point2 = point2;
            newResizeFPoints.point3 = point3;
            newResizeFPoints.point4 = point4;
            return newResizeFPoints;
        } else {
            qreal distance = pointToLineDistance(point2,  point4, pos);
            if (pointLineDir(point2, point4, pos) == -1) {
                QPointF add = pointSplid(point1, point3, distance);
                point1 = QPointF(point1.x() + add.x(), point1.y() - add.y());
                add = pointSplid(point3, point4, distance);
                point4 = QPointF(point4.x() - add.x(), point4.y() - add.y());
            } else {
                QPointF add = pointSplid(point1, point3, distance);
                point1 = QPointF(point1.x() - add.x(), point1.y() + add.y());
                add = pointSplid(point3, point4, distance);
                point4 = QPointF(point4.x() + add.x(), point4.y() + add.y());
            }
            point2 = QPointF(point1.x() + point4.x() - point3.x(), point1.y() + point4.y() - point3.y());
            newResizeFPoints.point1 =  point1;
            newResizeFPoints.point2 = point2;
            newResizeFPoints.point3 = point3;
            newResizeFPoints.point4 = point4;
            return newResizeFPoints;
        }
    }
    return newResizeFPoints;
}
/* point8 in the sixth position */
FourPoints point8Resize6(QPointF point1, QPointF point2, QPointF point3,
                         QPointF point4, QPointF pos, bool isShift) {
    Q_UNUSED(pos);
    Q_UNUSED(isShift);
    FourPoints newResizeFPoints;
    newResizeFPoints.point1 =  point1;
    newResizeFPoints.point2 = point2;
    newResizeFPoints.point3 = point3;
    newResizeFPoints.point4 = point4;
    return newResizeFPoints;
}
/* point8 in the seventh position */
FourPoints point8Resize7(QPointF point1, QPointF point2, QPointF point3,
                         QPointF point4, QPointF pos, bool isShift) {
    Q_UNUSED(pos);
    Q_UNUSED(isShift);
    FourPoints newResizeFPoints;
    newResizeFPoints.point1 =  point1;
    newResizeFPoints.point2 = point2;
    newResizeFPoints.point3 = point3;
    newResizeFPoints.point4 = point4;
    return newResizeFPoints;
}