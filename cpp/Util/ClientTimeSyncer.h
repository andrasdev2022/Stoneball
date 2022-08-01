#ifndef __CLIENTTIMESYNCER_H__
#define __CLIENTTIMESYNCER_H__

#include <stddef.h>

class ClientTimeSyncer
{
public:
    static const size_t snapshotStep_;
    static const double interpolationTime_;

    void onServerUpdate(double snapshotTimeStamp);

    double now();

    void trace();
private:
    double totalDrift_{};
    double clientTime_{};
    double prevSnapshotTimeStamp_{};
};

#endif // __CLIENTTIMESYNCER_H__
