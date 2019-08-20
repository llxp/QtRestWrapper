#ifndef QVARIANT_CAPI_H
#define QVARIANT_CAPI_H

#include <stdint.h>

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;
typedef double qreal;
typedef signed char qint8;         /* 8 bit signed */
typedef unsigned char quint8;      /* 8 bit unsigned */
typedef short qint16;              /* 16 bit signed */
typedef unsigned short quint16;    /* 16 bit unsigned */
typedef int qint32;                /* 32 bit signed */
typedef unsigned int quint32;      /* 32 bit unsigned */
typedef int64_t qint64;            /* 64 bit signed */
typedef uint64_t quint64;  /* 64 bit unsigned */
typedef qint64 qlonglong;
typedef quint64 qulonglong;

struct PrivateShared
{
    inline PrivateShared(void *v) : ptr(v), ref(1) { }
    void *ptr;
    /*_Atomic(int)*/ int ref;
};

#define QT_FOR_EACH_STATIC_PRIMITIVE_TYPE(F)\
    F(Void, 43, void) \
    F(Bool, 1, bool) \
    F(Int, 2, int) \
    F(UInt, 3, uint) \
    F(LongLong, 4, qlonglong) \
    F(ULongLong, 5, qulonglong) \
    F(Double, 6, double) \
    F(Long, 32, long) \
    F(Short, 33, short) \
    F(Char, 34, char) \
    F(ULong, 35, ulong) \
    F(UShort, 36, ushort) \
    F(UChar, 37, uchar) \
    F(Float, 38, float) \
    F(SChar, 40, signed char) \
    F(Nullptr, 51, std::nullptr_t) \
    F(QCborSimpleType, 52, QCborSimpleType) \

#define QT_FOR_EACH_STATIC_CORE_CLASS(F)\
    F(QChar, 7, QChar) \
    F(QString, 10, QString) \
    F(QStringList, 11, QStringList) \
    F(QByteArray, 12, QByteArray) \
    F(QBitArray, 13, QBitArray) \
    F(QDate, 14, QDate) \
    F(QTime, 15, QTime) \
    F(QDateTime, 16, QDateTime) \
    F(QUrl, 17, QUrl) \
    F(QLocale, 18, QLocale) \
    F(QRect, 19, QRect) \
    F(QRectF, 20, QRectF) \
    F(QSize, 21, QSize) \
    F(QSizeF, 22, QSizeF) \
    F(QLine, 23, QLine) \
    F(QLineF, 24, QLineF) \
    F(QPoint, 25, QPoint) \
    F(QPointF, 26, QPointF) \
    F(QRegExp, 27, QRegExp) \
    F(QEasingCurve, 29, QEasingCurve) \
    F(QUuid, 30, QUuid) \
    F(QVariant, 41, QVariant) \
    F(QRegularExpression, 44, QRegularExpression) \
    F(QJsonValue, 45, QJsonValue) \
    F(QJsonObject, 46, QJsonObject) \
    F(QJsonArray, 47, QJsonArray) \
    F(QJsonDocument, 48, QJsonDocument) \
    F(QCborValue, 53, QCborValue) \
    F(QCborArray, 54, QCborArray) \
    F(QCborMap, 55, QCborMap) \
    QT_FOR_EACH_STATIC_ITEMMODEL_CLASS(F)

#define QT_FOR_EACH_STATIC_CORE_TEMPLATE(F)\
    F(QVariantMap, 8, QVariantMap) \
    F(QVariantList, 9, QVariantList) \
    F(QVariantHash, 28, QVariantHash) \
    F(QByteArrayList, 49, QByteArrayList) \

#define QT_FOR_EACH_STATIC_GUI_CLASS(F)\
    F(QFont, 64, QFont) \
    F(QPixmap, 65, QPixmap) \
    F(QBrush, 66, QBrush) \
    F(QColor, 67, QColor) \
    F(QPalette, 68, QPalette) \
    F(QIcon, 69, QIcon) \
    F(QImage, 70, QImage) \
    F(QPolygon, 71, QPolygon) \
    F(QRegion, 72, QRegion) \
    F(QBitmap, 73, QBitmap) \
    F(QCursor, 74, QCursor) \
    F(QKeySequence, 75, QKeySequence) \
    F(QPen, 76, QPen) \
    F(QTextLength, 77, QTextLength) \
    F(QTextFormat, 78, QTextFormat) \
    F(QMatrix, 79, QMatrix) \
    F(QTransform, 80, QTransform) \
    F(QMatrix4x4, 81, QMatrix4x4) \
    F(QVector2D, 82, QVector2D) \
    F(QVector3D, 83, QVector3D) \
    F(QVector4D, 84, QVector4D) \
    F(QQuaternion, 85, QQuaternion) \
    F(QPolygonF, 86, QPolygonF) \


enum Type {
    Invalid = 0,
    Bool = 1,
    Int = 2,
    UInt = 3,
    LongLong = 4,
    ULongLong = 5,
    Double = 6,
    Char = 7,
    Map = 8,
    List = 9,
    String = 10,
    StringList = 11,
    ByteArray = 12,
    BitArray = 13,
    Date = 14,
    Time = 15,
    DateTime = 16,
    Url = 17,
    Locale = 18,
    Rect = 19,
    RectF = 20,
    Size = 21,
    SizeF = 22,
    Line = 23,
    LineF = 24,
    Point = 25,
    PointF = 26,
    RegExp = 27,
    RegularExpression = 44,
    Hash = 28,
    EasingCurve = 29,
    Uuid = 30,
    LastCoreType = 55,

    Font = 64,
    Pixmap = 65,
    Brush = 66,
    Color = 67,
    Palette = 68,
    Icon = 69,
    Image = 70,
    Polygon = 71,
    Region = 72,
    Bitmap = 73,
    Cursor = 74,
    KeySequence = 75,
    Pen = 76,
    TextLength = 77,
    TextFormat = 78,
    Matrix = 79,
    Transform = 80,
    Matrix4x4 = 81,
    Vector2D = 82,
    Vector3D = 83,
    Vector4D = 84,
    Quaternion = 85,
    PolygonF = 86,
    LastGuiType = PolygonF,

    SizePolicy = 121,

    UserType = 1024,
    LastType = 0xffffffff // need this so that gcc >= 3.4 allocates 32 bits for Type
};

struct qvariant_capi
{
    union Data
    {
        char c;
        uchar uc;
        short s;
        signed char sc;
        ushort us;
        int i;
        uint u;
        long l;
        ulong ul;
        bool b;
        double d;
        float f;
        qreal real;
        qlonglong ll;
        qulonglong ull;
        void *obj;
        void *ptr;
        PrivateShared *shared;
    } data;
    uint type : 30;
    uint is_shared : 1;
    uint is_null : 1;
};

#endif // QVARIANT_CAPI_H
