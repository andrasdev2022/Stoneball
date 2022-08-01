#include "ClientTimeSyncer.h"
#include "Math_.h"

#ifndef WIN32
	#include <sys/time.h>
#else
    #include "UtilWinDefs.h"
#endif

const size_t ClientTimeSyncer::snapshotStep_ = 50;
const double ClientTimeSyncer::interpolationTime_ = 150.;

void ClientTimeSyncer::onServerUpdate(double snapshotTimeStamp) {
    if(prevSnapshotTimeStamp_ > snapshotTimeStamp) return;
    prevSnapshotTimeStamp_ = snapshotTimeStamp;
    double min = snapshotTimeStamp + interpolationTime_ / 5.;
    double max = snapshotTimeStamp + 4 * interpolationTime_ / 5.;
    double n = now();
    //debug("DBG ClientTimeSyncer::onServerUpdate: n - snapshotTimeStamp: %f", n - snapshotTimeStamp);
    if(n < min) {
        totalDrift_ += (min - n) / 2.;
        //debug("DBG ClientTimeSyncer::onServerUpdate: totalDrift_ correction: %f", (min - n) / 2.);
    } else if(n > max) {
        totalDrift_ -= (n - max) / 2.;
        //debug("DBG ClientTimeSyncer::onServerUpdate: totalDrift_ correction: %f", (n - max) / 2.);
    }

    totalDrift_ -= (n - snapshotTimeStamp - interpolationTime_ / 2.) / 10.;
}

double ClientTimeSyncer::now() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    clientTime_ = tv.tv_sec * 1000. + tv.tv_usec/1000.;
    return clientTime_ + totalDrift_;
}

void ClientTimeSyncer::trace() {
    //debug("DBG ClientTimeSyncer::trace totalDrift_: %f", totalDrift_);
}
