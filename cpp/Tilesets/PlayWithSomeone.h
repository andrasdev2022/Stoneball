#ifndef __PLAYWITHSOMEONE_H__
#define __PLAYWITHSOMEONE_H__

#include <vector>
#include <inttypes.h>
#include <string>
#include "Tileset.h"
#include "TilesetCardHandler.h"

class Knight;

class PlayWithSomeone : virtual public Tileset, public TilesetCardHandler
{
public:
    PlayWithSomeone(int teamCount);

    enum class Team {
        RED_TEAM,
        DARK_TEAM,
        BLONDE_FEMALE_TEAM,
        GREEN_FEMALE_TEAM,
        LAST_ONE
    };

    virtual bool walkHistoryRuleCheck() const noexcept override;

    void drawTexts() noexcept override;
    void followTheKnight() noexcept override;
    bool isBlockingTile(int x, int y, VectorD direction) const noexcept override;
    [[nodiscard]] bool isKnightInActiveTeam(Knight*) const noexcept override;
    void setActiveTeam(GameObjectInterface::Type type) noexcept override;
    void setActiveTeamId(unsigned char teamId) noexcept override;
    unsigned char activeTeamId() const noexcept override;
    void calcTileMapRect() override;
    [[nodiscard]] bool isThereActiveTeam() const noexcept override {return activeTeam_ != Team::LAST_ONE;}

    virtual TilesetCardHandler* tilesetCardHandler() noexcept override {return this;}
    virtual const TilesetCardHandler* tilesetCardHandler() const noexcept override {return this;}
    virtual PlayWithSomeone* playWithSomeone() noexcept override {return this;}

    uint8_t winnerTeam() const noexcept override {return winnerTeam_;}
    void setWinnerTeam(uint8_t team) noexcept override;
    bool gameOver() const noexcept override {return winnerTeam_ != WINNER_UNKNOWN;}
    void setQuitters(const std::vector<uint8_t>& quitters) noexcept override {quitters_ = quitters;}
    virtual const std::vector<uint8_t>& quitters() const noexcept {return quitters_;}
    void resetPassCount() noexcept override {passCount_ = 0;}
    void setLastTurn() noexcept override {lastTurn_ = true;}
#ifdef __SERVERAPP__
    [[nodiscard]] bool playersJoined() const noexcept override {return playersJoined_;}
#endif

protected:
    virtual std::vector<int> calcScore() noexcept;
    virtual std::wstring prepareTimeString(int playerIndex) noexcept;
    uint16_t elapsedGameTime(uint8_t team) noexcept override;
    void setElapsedGameTime(uint8_t team, uint16_t value) noexcept override;
    void setTeamName(uint8_t team, std::wstring name) noexcept override;
    virtual void resetWatch() noexcept;
    void updateElapsedTime(double now) noexcept override;
    virtual void correctTimestamp() noexcept;
    virtual void stopServer() noexcept;
    virtual bool testSuddenDeath() noexcept;
    void nextTurn() noexcept override;
    void announceWinner(int looserIndex) noexcept override;
    void teamCountCheck() noexcept override;
    std::wstring generateYourTeamText() const noexcept;
    void generateRedTeamText(const std::vector<int>& teams) noexcept;
    void generateGrayTeamText(const std::vector<int>& teams) noexcept;
    void generateBlondeTeamText(const std::vector<int>& teams) noexcept;
    void generateGreenTeamText(const std::vector<int>& teams) noexcept;
    virtual void setBouncingStoneId(uint32_t id) noexcept {bouncingStoneId_ = id;}
    [[nodiscard]] virtual int textWidth() const noexcept {return textWidth_;}
    void showNextTurnMessageBox() noexcept;

    Team teamCount_;
    std::vector<double> elapsedGameTime_;
    Team activeTeam_;
    std::wstring textRedTeam_;
    std::wstring textGrayTeam_;
    std::wstring textBlondeTeam_;
    std::wstring textGreenTeam_;
    std::vector<double> latestTurnTimeStamp_;
    unsigned passCount_{};
    uint8_t winnerTeam_{WINNER_UNKNOWN};
#ifdef __SERVERAPP__
    bool playersJoined_{};
#endif
    std::vector<uint8_t> quitters_;
    bool lastTurn_{};
    std::vector<std::wstring> teamNames_;
    uint32_t bouncingStoneId_{0xffffffff};

    int textWidth_{};
};

#endif // __PLAYWITHSOMEONE_H__
