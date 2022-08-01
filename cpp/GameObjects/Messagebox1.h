#ifndef __MESSAGEBOX1_H__
#define __MESSAGEBOX1_H__

#include "BlueTriangle.h"
#include "TranslateInterface.h"

class Messagebox1 : public BlueTriangle
{
public:
    Messagebox1(double timeoutMs, TranslateInterface::Key messageKey) noexcept;
    Messagebox1(double timeoutMs, TranslateInterface::Key messageKey, std::wstring message) noexcept;

    BlueTriangle* copy() const noexcept override;

    // GameObject
    void update(double now) noexcept override;
    void draw(Screen* screen) noexcept override;
    double z() const noexcept override;

    TranslateInterface::Key message() const {return message_;}
    void setMessage(TranslateInterface::Key key) {message_ = key;}

    bool showCountDown() const noexcept {return showCountDown_;}
    void setShowCountDown(bool value) noexcept {showCountDown_ = value;}
    void drawCountDown(Screen* screen) noexcept;

    double spareTime() const noexcept {return spareTime_;}
    void setSpareTime(double spareTime) noexcept {spareTime_ = spareTime;}

    void setMessageString(const std::wstring str) {message2_ = str;}

    // from BlueTriangle
    Messagebox1* messagebox1() noexcept override {return this;}
    const Messagebox1* messagebox1() const noexcept override {return this;}

    // SerializeInterface
    bool addData(Serialize*) override;
    // copy data into the UDPMessageData, it will be synchronized by SnapshotSyncher
    // and MultiplayerWorld
    static const uint8_t* buildFrom(UDPMessageData*, const uint8_t*);

private:
    double timeoutMs_{};
    double startTime_{};
    TranslateInterface::Key message_{TranslateInterface::Key::LAST_ONE};
    std::wstring message2_;
    bool showCountDown_{};
    double spareTime_{};
};

using Messagebox1UPtr = std::unique_ptr<Messagebox1>;

#endif // __MESSAGEBOX1_H__
