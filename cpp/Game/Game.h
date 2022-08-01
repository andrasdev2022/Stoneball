#ifndef __GAME_H__
#define __GAME_H__

#include "GameInterface.h"
#include "EmotesBubble.h"

class Game : public GameInterface
{
public:
    void wakeUp() noexcept override;
    [[nodiscard]] WorldUPtr& latestWorld() noexcept override {return worldList_.rbegin()->second;}
    [[nodiscard]] std::map<double, WorldUPtr>& worldList() noexcept override {return worldList_;}
    [[nodiscard]] size_t worldListSize() noexcept override {return worldList_.size();}
    [[nodiscard]] std::vector<Button*>& buttons() noexcept override {return buttons_;}
    void initButtons() noexcept override;
    void changeButtonsAccordingToPlaymode() noexcept override;
    [[nodiscard]] Button* nextTurnButton() noexcept override {return nextTurnButton_.get();}
    void updateWorldOfButtons(WorldUPtr& world) noexcept override;
    void resetWorldTouchEvent() noexcept override;
    WorldTouchEvent* worldTouchEvent() noexcept override {return worldTouchEvent_.get();}
    bool showNextTurnButton() noexcept override;
    Button* hideNextTurnButton() noexcept override;
    void changeNextTurnButtonLabel(TranslateInterface::Key key) noexcept override;
    void hideArrowButtons() noexcept override;

    [[nodiscard]] Button* arrowButtonRight() noexcept override {return arrowButtonRight_.get();}
    [[nodiscard]] Button* arrowButtonUp() noexcept override {return arrowButtonUp_.get();}
    [[nodiscard]] Button* arrowButtonDown() noexcept override {return arrowButtonDown_.get();}
    [[nodiscard]] Button* arrowButtonLeft() noexcept override {return arrowButtonLeft_.get();}
    [[nodiscard]] EmotesBubble* emotesBubble() noexcept override {return emotesBubble_.get();}

protected:
    virtual void updateANewCopyOfTheLastestWorld(double now) noexcept;

    std::map<double, WorldUPtr> worldList_;
    std::vector<Button*> buttons_;
    ButtonUPtr arrowButtonRight_;
    ButtonUPtr arrowButtonUp_;
    ButtonUPtr arrowButtonDown_;
    ButtonUPtr arrowButtonLeft_;
    ButtonUPtr nextTurnButton_;
    WorldTouchEventUPtr worldTouchEvent_;
    EmotesBubbleUPtr emotesBubble_;


    static constexpr int WORLDLIST_MAX_SIZE = 40;
};

#endif // __GAME_H__
