#ifndef __MATCHRESULTS_H__
#define __MATCHRESULTS_H__

#include "BlueTriangle.h"
#include <vector>
#include <memory>

enum BitmapId : int;

class MatchResults : public BlueTriangle
{
public:
    MatchResults(uint8_t teamNum);

    BlueTriangle* copy() const noexcept override;

    virtual void setScores(uint8_t red, uint8_t gray, uint8_t yellow, uint8_t green) noexcept;
    virtual void setElapsedGameTimes(uint16_t red, uint16_t gray, uint16_t yellow, uint16_t green) noexcept;
    virtual uint8_t winnerTeam() noexcept;
    virtual void setLooserTeam(uint8_t ) noexcept;

    // GameObject
    void update(double now) noexcept override;
    void draw(Screen* screen) noexcept override;
    double z() const noexcept override;

    // from BlueTriangle
    MatchResults* matchResults() noexcept override {return this;}
    const MatchResults* matchResults() const noexcept override {return this;}

    // SerializeInterface
    bool addData(Serialize*) override;
    // copy data into the UDPMessageData, it will be synchronized by SnapshotSyncher
    // and MultiplayerWorld
    static const uint8_t* buildFrom(UDPMessageData*, const uint8_t*);

    // for MultiplayerWorld and snapshotsyncer
    virtual void setTeamNum(uint8_t teamNum) noexcept;
    virtual void setElapsedTimes(uint64_t elapsedTimes) noexcept;
    virtual void setScores(uint32_t scores) noexcept;

private:
    void updateTeamDatas() noexcept;
    void drawLabelCentered(Screen* screen, BitmapId bitmapId, int y) noexcept;
    void drawLabel(Screen* screen, BitmapId bitmapId, int x, int y) noexcept;
    std::wstring prepareTimeString(uint16_t elapsedGameTime) noexcept;
    void initAnimation() noexcept;
    void drawTableLabels(Screen* screen) noexcept;
    void drawKnights(Screen* screen) noexcept;
    void saveTable2Cache(Screen* screen) noexcept;
    void drawAnimation() noexcept;

    uint8_t teamNum_{};
    uint8_t redScore_{};
    uint8_t grayScore_{};
    uint8_t yellowScore_{};
    uint8_t greenScore_{};
    uint16_t redElapsedTime_{};
    uint16_t grayElapsedTime_{};
    uint16_t yellowElapsedTime_{};
    uint16_t greenElapsedTime_{};
    double tableX_{};
    double tableY_{};
    int scoreLabelWidth_{};
    int timeLabelWidth_{};

    struct TeamData {
        uint8_t teamId{};
        uint8_t score{};
        uint16_t elapsedTime{};
        static bool greater(const TeamData& a, const TeamData& b) {
            if(a.score > b.score) return true;
            if(a.score < b.score) return false;
            if(a.elapsedTime > b.elapsedTime) return true;
            return false;
        }
    };
    std::vector<TeamData> teamDatas_;
    double appearenceTime_{};
    int fireworksCount_{};
    uint8_t looserTeam_{255};
};

using MatchResultsUPtr = std::unique_ptr<MatchResults>;

#endif // __MATCHRESULTS_H__
