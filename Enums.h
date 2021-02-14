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
        APPSRC_NULL = 3,
        PIPELINE_NULL = 4,
        PIPELINESTR_EMPTY = 5,
        BUFFER_NULL = 6,
        IMAGE_NULL = 7,
        PUSH_BUFFER = 8,
        UNKNOWN
    };
};

#endif // ENUMS_H
