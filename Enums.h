#ifndef ENUMS_H
#define ENUMS_H
#include <QObject>
class Enums : public QObject
{
    Q_OBJECT
public:
    enum class GSTError : int{
        EOS = 1,
        APPSINK_NULL = 2,
        PIPELINE_NULL = 3,
        PIPELINESTR_EMPTY = 4,
        BUFFER_NULL = 5,
        IMAGE_NULL = 6,
        UNKNOWN
    };
};

#endif // ENUMS_H
