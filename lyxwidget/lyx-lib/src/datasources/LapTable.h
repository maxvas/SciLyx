#ifndef LAPTABLE_H
#define LAPTABLE_H

#include <QVariant>

class LapTable : public QObject
{
    Q_OBJECT
public:
    LapTable(int columnsCoout, int rowsCount)
        :_columnsCount(columnsCoout), _rowsCount(rowsCount)
    {
        data = new QVariant*[rowsCount];
        for (int i=0; i<rowsCount; i++)
        {
            data[i] = new QVariant[columnsCoout];
        }
    }
    ~LapTable()
    {
        clear();
    }
    void clear()
    {
        if (!data)
        {
            return;
        }
        //TODO
//        for (int i=0; i<_rowsCount; i++)
//        {
//            delete data[i];
//        }
        delete data;
    }

    QVariant **data;
    int columnsCount()
    {
        return _columnsCount;
    }
    int rowsCount()
    {
        return _rowsCount;
    }

private:
    int _columnsCount;
    int _rowsCount;

};
#endif // LAPTABLE_H
