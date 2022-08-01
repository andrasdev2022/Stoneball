#include "Application.h"
#include "Button.h"
#include "TouchEventHandler.h"
#include "Screen.h"
#include "WorldTouchEvent.h"
#include "MenuTouchEvent.h"
#include "stopwatch.h"
#include "UDP.h"
#include "ClientTimeSyncer.h"
#include "Environment.h"
#include "EnumClassOp.h"
#include "StringUtil.h"
#include <string.h>
#include "TranslatorFactory.h"
#include "FontFactory.h"
#include "SingleplayerWorld.h"
#include "MultiplayerWorld.h"
#include "BitmapIds.h"
#include "Defines.h"
#include "Algorithms.h"
#include "Messagebox1.h"
#include "UDPMessageFactory.h"
#include "MenuBuilder.h"
#include "AnimatedKnightsForMenu.h"
#include "SettingsMenu.h"
#include "SoundEffect.h"
#include "MediaData.h"
#include "MessageboxYesNo.h"
#include "GameInterface.h"
#include "GameFactory.h"
#include "UDPServerInterface.h"
#include "EmotesBubble.h"

using namespace Algorithms;

/*
class Room::Members {
public:

};

Room::Room() {
    members_ = std::make_shared<Room::Members>();
}

namespace ThreadLocal {
    THREAD_LOCAL std::unique_ptr<Room> room_;
    std::unique_ptr<Room>& room() {return room_;}
}
*/

namespace Application {

THREAD_LOCAL GameInterfaceUPtr game_;
THREAD_LOCAL TouchEventHandler touchEventHandler_;
THREAD_LOCAL ButtonUPtr refreshButton_;
THREAD_LOCAL ButtonUPtr backButton_;
THREAD_LOCAL ButtonUPtr undoButton_;
THREAD_LOCAL ButtonUPtr emotesButton_;

THREAD_LOCAL ScreenUPtr screen_;
THREAD_LOCAL MenuTouchEventUPtr menuTouchEvent_;
THREAD_LOCAL StopWatch stopWatch;
THREAD_LOCAL double now__{};
THREAD_LOCAL PlayerMode playerMode_ = PlayerMode::UNDEFINED;
THREAD_LOCAL Avatar avatar_ = Avatar::UNDEFINED;
THREAD_LOCAL MenuInterfaceSPtr activeMenuI_;
THREAD_LOCAL LevelFactory::GameLevel gameLevel_ = LevelFactory::GameLevel::LEVEL_1;
THREAD_LOCAL TranslateInterface::Language language_ = TranslateInterface::Language::ENGLISH;
THREAD_LOCAL TranslateInterfaceUPtr translator_;
THREAD_LOCAL FontInterfaceUPtr font_;
THREAD_LOCAL FontInterfaceUPtr font8px_;
THREAD_LOCAL FontInterfaceUPtr font12px_;
THREAD_LOCAL FontInterfaceUPtr font14px_;
THREAD_LOCAL FontInterfaceUPtr font24px_;
THREAD_LOCAL FontInterfaceUPtr font30px_;
THREAD_LOCAL bool freezUpdate_ = false;
THREAD_LOCAL PlayMode playMode_ = PlayMode::REALTIME;

THREAD_LOCAL EnvironmentUPtr environment_;
THREAD_LOCAL std::vector<BlueTriangleUPtr> messageboxes_;
THREAD_LOCAL std::vector<BlueTriangleUPtr> widgets_;

THREAD_LOCAL std::wstring roomName_;

THREAD_LOCAL int playTime_ = 5; // 5 minutes
THREAD_LOCAL int musicVolume_ = 50;
THREAD_LOCAL int effectVolume_ = 100;
THREAD_LOCAL bool settingMustBeSaved_ = false;
extern THREAD_LOCAL std::string serverHost_;
extern THREAD_LOCAL int serverPort_;
THREAD_LOCAL int resolutionIndex_{};
THREAD_LOCAL bool privacyPolicyAccepted_{};

[[nodiscard]] bool& privacyPolicyAccepted() noexcept {return privacyPolicyAccepted_;}
[[nodiscard]] int musicVolume() noexcept {return musicVolume_;}
[[nodiscard]] int effectVolume() noexcept {return effectVolume_;}

void setMusicVolume(int volume) noexcept {musicVolume_ = volume;}
void setEffectVolume(int volume) noexcept {effectVolume_ = volume;}

[[nodiscard]] Screen* screen() noexcept {return screen_.get();}
[[nodiscard]] TouchEventHandler* touchEventHandler() noexcept {return &touchEventHandler_;}
//[[nodiscard]] std::map<double, WorldUPtr>& worldList() noexcept {return game_->worldList();}
[[nodiscard]] GameInterfaceUPtr& game() noexcept {return game_;}
[[nodiscard]] PlayerMode playerMode() noexcept {return playerMode_;}
void setPlayerMode(PlayerMode pm) noexcept {playerMode_ = pm;}
[[nodiscard]] Avatar avatar() noexcept {return avatar_;}
[[nodiscard]] MenuInterface::MenuType activeMenu() noexcept {return activeMenuI_->menuType();}
[[nodiscard]] LevelFactory::GameLevel gameLevel() noexcept {return gameLevel_;}
[[nodiscard]] MenuTouchEvent* menuTouchEvent() noexcept {return menuTouchEvent_.get();}
[[nodiscard]] Environment* environment() noexcept {return environment_.get();}
[[nodiscard]] Tileset* tileset() noexcept {return environment_->tileset();}
[[nodiscard]] WorldUPtr& latestWorld() noexcept {return game_->latestWorld();}
void setGameLevel(LevelFactory::GameLevel gameLevel) noexcept {gameLevel_ = gameLevel;}
std::vector<BlueTriangleUPtr>& widgets() noexcept {return widgets_;}
int levelStatus_[static_cast<int>(LevelFactory::GameLevel::LEVEL_PLAY_WITH_FRIEND_1)]{};

void nextLevel() noexcept;
void initTranslator() noexcept;
void initFont() noexcept;
void initWorldEnvironment() noexcept;
void initTouchEventHandler() noexcept;
void processServerMessages(bool findGame) noexcept;
void clearServerMessages() noexcept;

[[nodiscard]] TranslateInterface::Language language() noexcept {return language_;}
void setLanguage(TranslateInterface::Language lang) noexcept {
    if(language_ != lang) {
        language_ = lang;
        translator_ = nullptr;
        initTranslator();
        font_ = nullptr;
        font8px_ = nullptr;
        font12px_ = nullptr;
        font14px_ = nullptr;
        font24px_ = nullptr;
        font30px_ = nullptr;
        initFont();
    }
}
const TranslateInterfaceUPtr& translator() noexcept {return translator_;}
const FontInterfaceUPtr& font() noexcept {return font_;}
const FontInterfaceUPtr& font8px() noexcept {return font8px_;}
const FontInterfaceUPtr& font12px() noexcept {return font12px_;}
const FontInterfaceUPtr& font14px() noexcept {return font14px_;}
const FontInterfaceUPtr& font24px() noexcept {return font24px_;}
const FontInterfaceUPtr& font30px() noexcept {return font30px_;}
void freezUpdate() noexcept {freezUpdate_ = true;}
[[nodiscard]] PlayMode playMode() noexcept {return playMode_;}

void setPlayMode(PlayMode p) noexcept {
    playMode_ = p;
    game_->changeButtonsAccordingToPlaymode();
}

[[nodiscard]] GameObjectInterface::Type Avatar2GOT(Avatar avatar) noexcept {
    switch(avatar) {
    case Avatar::RED: return GameObjectInterface::Type::GOT_KNIGHT_RED; break;
    case Avatar::DARK: return GameObjectInterface::Type::GOT_KNIGHT_DARK; break;
    case Avatar::BLONDE_FEMALE: return GameObjectInterface::Type::GOT_KNIGHT_BLONDE_FEMALE; break;
    case Avatar::GREEN_FEMALE: return GameObjectInterface::Type::GOT_KNIGHT_GREEN_FEMALE; break;
    case Avatar::UNDEFINED: return GameObjectInterface::Type::COUNT; break;
    }
    return GameObjectInterface::Type::COUNT;
}

[[nodiscard]] Avatar GOT2Avatar(GameObjectInterface::Type got) noexcept {
    switch(got) {
    case GameObjectInterface::Type::GOT_KNIGHT_RED: return Avatar::RED; break;
    case GameObjectInterface::Type::GOT_KNIGHT_DARK: return Avatar::DARK; break;
    case GameObjectInterface::Type::GOT_KNIGHT_BLONDE_FEMALE: return Avatar::BLONDE_FEMALE; break;
    case GameObjectInterface::Type::GOT_KNIGHT_GREEN_FEMALE: return Avatar::GREEN_FEMALE; break;
    default: break;
    }
    return Avatar::UNDEFINED;
}

void updateTeamByAvatar() noexcept {
    if(!environment_ || !environment_->tileset()) return;
    environment_->tileset()->setActiveTeam(Avatar2GOT(avatar_));
}

void setAvatar(Avatar a) noexcept {
    avatar_ = a;
}

void deleteDeadMessageboxesAndWidgets() noexcept {
    for(auto it = messageboxes_.begin(); it != messageboxes_.end();) {
        if((*it)->isDead()) {
            it = messageboxes_.erase(it);
        } else {
            ++it;
        }
    }
    for(auto it = widgets_.begin(); it != widgets_.end();) {
        if((*it)->isDead()) {
            it = widgets_.erase(it);
        } else {
            ++it;
        }
    }
}

void initButtons() noexcept {
    game_->initButtons();

    backButton_ = std::make_unique<Button>(0_ZRD, 32_ZRD, GameObject::Type::GOT_BACK);
    refreshButton_ = std::make_unique<Button>(32_ZRD, 32_ZRD, GameObject::Type::GOT_REFRESH);
    undoButton_ = std::make_unique<Button>(64_ZRD, 32_ZRD, GameObject::Type::GOT_UNDO);
    emotesButton_ = std::make_unique<Button>(screenWidth() - 48_ZRD, screenHeight() - 192_ZRD, GameObject::Type::GOT_EMOTE_BUTTON);

    if(anyOf(Application::activeMenu(), MenuInterface::MenuType::SINGLEPLAYER_RUNNIG, MenuInterface::MenuType::MULTIPLAYER_ON_ONE_DEVICE_RUNNING, MenuInterface::MenuType::MULTIPLAYER_FIND_GAME)) {
    game_->buttons().push_back(refreshButton_.get());
        if(Application::activeMenu() == MenuInterface::MenuType::SINGLEPLAYER_RUNNIG) {
            game_->buttons().push_back(undoButton_.get());
        }
    } else if(anyOf(Application::activeMenu(), MenuInterface::MenuType::MULTIPLAYER_HOST_RUNNING, MenuInterface::MenuType::MULTIPLAYER_JOINED_RUNNING)) {
        game_->buttons().push_back(emotesButton_.get());
    }
    game_->buttons().push_back(backButton_.get());
}

void initScreen() noexcept {
    if(!screen_) {
        screen_ = std::make_unique<Screen>(0, 0, screenWidth(), screenHeight());
        setScreenPos(0, 0);
    }
}

void initMenu() noexcept {
    if(!activeMenuI_) {
        activeMenuI_ = MenuBuilder::buildMainMenu();
    }
}

void setActiveMenu(MenuInterface::MenuType t) noexcept {
    initMenu();
    activeMenuI_ = activeMenuI_->findMenu(t);
}

void initWorldEnvironment(WorldUPtr world) noexcept {
    LOGI("initWorldEnvironment");

    touchEventHandler_.clear();

    environment_ = std::make_unique<Environment>();
    environment_->init(world, LevelFactory::create(gameLevel_));

    world->init();
    // A knight must be in idle state (instead of sit) in multiplayer version?
    // the activeKnightId must be filled?
    updateTeamByAvatar();
    debug("avatar: %d, active team: %d", (int)avatar_, (int)environment_->tileset()->activeTeamId());
    game_->updateWorldOfButtons(world);
    game_->worldList().emplace(0, std::move(world));
    initTouchEventHandler();
}

void initTouchEventHandler() noexcept {
    std::for_each(game_->buttons().begin(), game_->buttons().end(), [](Button*b){touchEventHandler_.add(b);});
    touchEventHandler_.add(game_->emotesBubble());
    touchEventHandler_.add(game_->worldTouchEvent());
}

void addMandatoryBackButton() {
    if(!backButton_) {
        backButton_ = std::make_unique<Button>(0_ZRD, 32_ZRD, GameObject::Type::GOT_BACK);
    }
    touchEventHandler_.remove(backButton_.get());
    touchEventHandler_.add(backButton_.get());
}

void drawMenu(AndroidBitmapInfo* info, void* pixels) noexcept {
    if(!activeMenuI_ || !activeMenuI_->draw(info, pixels)) {return;}
    if(activeMenuI_->menuType() != MenuInterface::MenuType::MAIN) {
        addMandatoryBackButton();
        backButton_->draw(info, pixels);
        if(activeMenuI_->menuType() == MenuInterface::MenuType::MULTIPLAYER_FIND_GAME) {
            if(!refreshButton_) {
                refreshButton_ = std::make_unique<Button>(32_ZRD, 32_ZRD, GameObject::Type::GOT_REFRESH);
            }
            touchEventHandler_.remove(refreshButton_.get());
            touchEventHandler_.add(refreshButton_.get());
            refreshButton_->draw(info, pixels);
        }
    }
}

void updateSingleplayerWorld() noexcept {
    if(freezUpdate_) return;

    if(!game_ || !game_->singleplayerGame()) {
        game_ = GameFactory::create(GameFactory::Type::SINGLEPLAYER);
    }

    if(!game_->buttons().size()) {
        initButtons();
        initWorldEnvironment(std::make_unique<SingleplayerWorld>());
    }

    stopWatch.start("update");
    game_->updateWorld(now());
    stopWatch.saveDuration("update");
}

void updateMultiplayerWorld() noexcept {

    if(!game_ || game_->singleplayerGame()) {
        game_ = GameFactory::create(GameFactory::Type::MULTIPLAYER);
    }

    if(!game_->buttons().size()) {
        initButtons();
        initWorldEnvironment(std::make_unique<MultiplayerWorld>());
        latestWorld()->update(now());
        return;
    }
    game_->updateWorld(now());
}

void initTranslator() noexcept {
    if(!translator_) {
        translator_ = TranslatorFactory::create(language_);
    }
}

void initFont() noexcept {
    if(font_) {return; }
    font_ = FontFactory::create(language_, FontInterface::Size::PIXEL_16);
    font8px_ = FontFactory::create(language_, FontInterface::Size::PIXEL_8);
    font12px_ = FontFactory::create(language_, FontInterface::Size::PIXEL_12);
    font14px_ = FontFactory::create(language_, FontInterface::Size::PIXEL_14);
    font24px_ = FontFactory::create(language_, FontInterface::Size::PIXEL_24);
    font30px_ = FontFactory::create(language_, FontInterface::Size::PIXEL_30);
}

void updateWorld(double now) noexcept {
    initScreen();
    initTranslator();
    initFont();
    initMenu();
    //LOGI("DBG updateWorld 1");
/*  The too slow fps should be handled
    if(now_ > 1e-7 && now - now_ > 50.) {
        now_ = now_ + 50.;
        debug("WARNING: 50 ms correction");
    } else {
        now_ = now;
    }
*/
    now__ = now;
    //LOGI("DBG updateWorld 2");

    if(playerMode_ == PlayerMode::UNDEFINED) {
	//LOGI("DBG updateWorld 22");
        if(!menuTouchEvent_) {
            menuTouchEvent_ = std::make_unique<MenuTouchEvent>();
            touchEventHandler_.add(menuTouchEvent_.get());
        }

        if(activeMenuI_->menuType() == MenuInterface::MenuType::SETTINGS) {
            activeMenuI_->update(now);
            return;
        }

        if(activeMenuI_->menuType() == MenuInterface::MenuType::MAIN) {
            activeMenuI_->update(now);
        }
    } else if(playerMode_ == PlayerMode::MULTIPLAYER) {
        if(activeMenuI_->menuType() == MenuInterface::MenuType::MULTIPLAYER_ON_ONE_DEVICE_RUNNING) {
            updateSingleplayerWorld();
        } else if(anyOf(activeMenuI_->menuType(), MenuInterface::MenuType::MULTIPLAYER_CREATE_GAME,
                        MenuInterface::MenuType::MULTIPLAYER_ONE_DEVICE_GAME_PARAMETERS,
                        MenuInterface::MenuType::ONLINE)) {
            activeMenuI_->update(now);
            //return;
        } else if(activeMenuI_->menuType() == MenuInterface::MenuType::MULTIPLAYER_FIND_GAME) {
#ifndef __SERVERAPP__
            processServerMessages(true);
            activeMenuI_->update(now);
#endif
        } else if(activeMenuI_->menuType() == MenuInterface::MenuType::MULTIPLAYER_HOST_RUNNING || activeMenuI_->menuType() == MenuInterface::MenuType::MULTIPLAYER_JOINED_RUNNING) {
            // at least a world should be created before the first server message arrives
            updateMultiplayerWorld();
#ifndef __SERVERAPP__
            // the snapshotsync clear the game over messageboxes
            if(environment_ && tileset() && tileset()->winnerTeam() != Tileset::WINNER_UNKNOWN) {
                clearServerMessages();
            } else {
                processServerMessages(false);
            }
#endif
        }
    } else if(playerMode_ == PlayerMode::SINGLEPLAYER) {
        if(activeMenuI_->menuType() == MenuInterface::MenuType::LEVELS && menuTouchEvent_) {
            activeMenuI_->update(Application::now());
        } else if(activeMenuI_->menuType() == MenuInterface::MenuType::SINGLEPLAYER_RUNNIG) {
            updateSingleplayerWorld();
        }
    }
    for(auto& msg : messageboxes_) {msg->update(Application::now());}
    for(auto& msg : widgets_) {msg->update(Application::now());}
    deleteDeadMessageboxesAndWidgets();
}

void drawSingleplayerWorld(AndroidBitmapInfo* info, void* pixels) noexcept {
    stopWatch.start("draw");
    static double firstRun = now_ms();

    game_->drawWorld(info, pixels);

    stopWatch.saveDuration("draw");

    #define WATCH_DURATION 10e+6
    static bool once = false;
    static int frameCount = 0;
    ++frameCount;
    if(!once && (stopWatch.duration("draw") > WATCH_DURATION || stopWatch.duration("update") > WATCH_DURATION)) {
        using namespace StringUtil;
        once = true;
        double now = now_ms();
        debug("screen resolution: %dx%d (widthxheight)", screenWidth(), screenHeight());
        debug("In 10s drawing time:\n-----------------------------------------------");
        std::string formattedStr = group(frameCount);
        debug("frame count: %s", formattedStr.c_str());
        debug("elapsed time %f millisec", now - firstRun);
        debug("speed: %f fps", frameCount * 1000. / (now - firstRun));
        formattedStr = group(stopWatch.duration("update"));
        debug("full update time: %s microseconds", formattedStr.c_str());
        formattedStr = group(stopWatch.duration("draw"));
        debug("full draw time: %s microseconds", formattedStr.c_str());
    }
}

void drawMultiplayerWorld([[maybe_unused]]AndroidBitmapInfo* info, [[maybe_unused]]void* pixels) noexcept {
    game_->drawWorld(info, pixels);
}

void drawMessageboxesAndWidgets([[maybe_unused]]AndroidBitmapInfo* info, [[maybe_unused]]void* pixels) {
    if(!screen()) return;

    struct Less {
        Less(){}
        Less(GameObject* item, bool messagebox) : item_(item), messagebox_(messagebox) {

        }

        bool operator()(const Less& a, const Less& b) const {
            return a.item_->z() < b.item_->z();
        }
        GameObject* item_;
        bool messagebox_;
    };
    std::multiset<Less, Less> sorter;
    std::for_each(messageboxes_.begin(), messageboxes_.end(), [&sorter](const auto& go){sorter.emplace(go.get(), true);});
    std::for_each(widgets_.begin(), widgets_.end(), [&sorter](const auto& go){sorter.emplace(go.get(), false);});

    if(!screen()->content()) {
        screen()->setContentPtr((uint16_t*)pixels);
    }

    for(auto& l : sorter) {
        l.item_->draw(screen());
    }
}

void drawWorld(AndroidBitmapInfo* info, void* pixels) noexcept {
    if(playerMode_ == PlayerMode::UNDEFINED) {
        drawMenu(info, pixels);
    } else if(playerMode_ == PlayerMode::MULTIPLAYER) {
        if((avatar_ == Avatar::UNDEFINED && activeMenuI_->menuType() != MenuInterface::MenuType::MULTIPLAYER_ON_ONE_DEVICE_RUNNING) ||
        anyOf(activeMenuI_->menuType(), MenuInterface::MenuType::MULTIPLAYER_CREATE_GAME,
              MenuInterface::MenuType::MULTIPLAYER_FIND_GAME,
              MenuInterface::MenuType::MULTIPLAYER_ONE_DEVICE_GAME_PARAMETERS,
              MenuInterface::MenuType::ONLINE)) {
            drawMenu(info, pixels);
        } else {
            if(activeMenuI_->menuType() == MenuInterface::MenuType::MULTIPLAYER_ON_ONE_DEVICE_RUNNING) {
                drawSingleplayerWorld(info, pixels);
            } else {
                drawMultiplayerWorld(info, pixels);
            }
        }
    } else if(playerMode_ == PlayerMode::SINGLEPLAYER) {
        if(activeMenuI_->menuType() == MenuInterface::MenuType::LEVELS) {
            drawMenu(info, pixels);
        } else if(activeMenuI_->menuType() == MenuInterface::MenuType::SINGLEPLAYER_RUNNIG) {
            drawSingleplayerWorld(info, pixels);
        }
    }
    drawMessageboxesAndWidgets(info, pixels);
}

void wakeUp() noexcept {
    if(game_) game_->wakeUp();
}

double now() noexcept {
    return now__;
}

void stop() noexcept {
    debug("DBG Application stop");
    if(udpServer()) {
        debug("DBG Application stop 1");
        udpServer()->stop();
        debug("DBG Application stop 2");
        udpServer()->join();
        debug("DBG Application stop 3");
        if(!udpServer()->isRunning()) {
            debug("DBG UDPServer stopped and joined");
        }
    }
}

bool leaveTheGameConfirmed{};

void messageboxYesNoCallback1([[maybe_unused]]MessageboxYesNo* messagebox, MessageboxYesNo::Button value) {
    if(value == MessageboxYesNo::Button::YES) {
        leaveTheGameConfirmed = true;
        restartLevel();
    } else {
        leaveTheGameConfirmed = false;
        if(latestWorld()->areEnemiesDead() && latestWorld()->singleplayer()) {
            latestWorld()->singleplayer()->redrawLevelCompletedPanel();
        }
    }
}

void restartLevel() noexcept {
    if(!leaveTheGameConfirmed) {
        // the messagebox is handled on client side, the server does not need to know about it
        MessageboxYesNo* messagebox;
        if(activeMenuI_->menuType() == MenuInterface::MenuType::SINGLEPLAYER_RUNNIG) {
            messagebox = new MessageboxYesNo(MessageboxYesNo::Type::RELOAD_PUZZLE);
        } else {
            messagebox = new MessageboxYesNo(MessageboxYesNo::Type::RESTART_MATCH);
        }
        messagebox->add(messageboxYesNoCallback1);
        widgets_.emplace_back(messagebox);
        touchEventHandler_.insertFirst(messagebox);
        return;
    }
    leaveTheGameConfirmed = false;

    game_->worldList().clear();
    initWorldEnvironment(std::make_unique<SingleplayerWorld>());
    destroyMessageBoxes();
    destroyWidgets();
}

void messageboxYesNoCallback2([[maybe_unused]]MessageboxYesNo* messagebox, MessageboxYesNo::Button value) {
    if(value == MessageboxYesNo::Button::YES) {
        leaveTheGameConfirmed = true;
        goUpInTheMenu();
    } else {
        leaveTheGameConfirmed = false;
    }
}

void goUpInTheMenu() noexcept {
    debug("DBG 0, menutype: %d", (int)activeMenuI_->menuType());

    if(environment_ && tileset() && !tileset()->gameOver() && !tileset()->viewOnly() &&
    anyOf(activeMenuI_->menuType(), MenuInterface::MenuType::MULTIPLAYER_HOST_RUNNING, MenuInterface::MenuType::MULTIPLAYER_JOINED_RUNNING,
    MenuInterface::MenuType::MULTIPLAYER_ON_ONE_DEVICE_RUNNING, MenuInterface::MenuType::SINGLEPLAYER_RUNNIG) &&
    latestWorld()->status() != World::Status::ST_DEFEAT && latestWorld()->status() != World::Status::ST_VICTORY) {
        if(!leaveTheGameConfirmed) {
            // the messagebox is handled on client side, the server does not need to know about it
            MessageboxYesNo* messagebox;
            if(activeMenuI_->menuType() == MenuInterface::MenuType::SINGLEPLAYER_RUNNIG) {
                messagebox = new MessageboxYesNo(MessageboxYesNo::Type::QUIT_PUZZLE);
            } else {
                messagebox = new MessageboxYesNo(MessageboxYesNo::Type::QUIT_MATCH);
            }
            messagebox->add(messageboxYesNoCallback2);
            widgets_.emplace_back(messagebox);
            touchEventHandler_.insertFirst(messagebox);
            return;
        }
        leaveTheGameConfirmed = false;
    }
    if(screen_) {
        screen_->setX(0);
        screen_->setY(0);
    }
    if(game_) {
        game_->worldList().clear();
    }
    //clearAvatarSelection();
    if(activeMenuI_->menuType() == MenuInterface::MenuType::SINGLEPLAYER_RUNNIG) {
        avatar_ = Avatar::RED;
        playerMode_ = PlayerMode::SINGLEPLAYER;
        setActiveMenu(MenuInterface::MenuType::LEVELS);
        if(game_) {game_->resetWorldTouchEvent();}
        touchEventHandler_.clear();
        touchEventHandler_.add(menuTouchEvent_.get());
        if(SoundEffect::isEmpty()) {
            SoundEffect::play(SoundEffect::Name::STOP_PLAYING_RUN_ON_SAND);
        }
    } else if(anyOf(activeMenuI_->menuType(), MenuInterface::MenuType::ONLINE)) {
        avatar_ = Avatar::UNDEFINED;
        playerMode_ = PlayerMode::MULTIPLAYER;
        setActiveMenu(MenuInterface::MenuType::MULTIPLAYER);
        touchEventHandler_.clear();
        touchEventHandler_.add(menuTouchEvent_.get());
        destroyMessageBoxes();
        destroyWidgets();
    } else if(anyOf(activeMenuI_->menuType(),
                                 MenuInterface::MenuType::MULTIPLAYER_CREATE_GAME,
                                 MenuInterface::MenuType::MULTIPLAYER_FIND_GAME,
                                 MenuInterface::MenuType::MULTIPLAYER_ON_ONE_DEVICE_RUNNING,
                                 MenuInterface::MenuType::MULTIPLAYER_ONE_DEVICE_GAME_PARAMETERS,
                                 MenuInterface::MenuType::MULTIPLAYER_HOST_RUNNING,
                                 MenuInterface::MenuType::MULTIPLAYER_JOINED_RUNNING)) {

        if(anyOf(activeMenuI_->menuType(), MenuInterface::MenuType::MULTIPLAYER_HOST_RUNNING, MenuInterface::MenuType::MULTIPLAYER_JOINED_RUNNING)) {
            // Let's send an exit message
            UDP udp;
            UDPMessageInterfaceUPtr newMsg = UDPMessageFactory::createExitMessage();
            [[maybe_unused]]bool xx = udp.sendMessage2Server(newMsg);
            if(SoundEffect::isEmpty()) {
                SoundEffect::play(SoundEffect::Name::STOP_PLAYING_RUN_ON_SAND);
            }
            serverHost_ = "";
            serverPort_ = 0;
        }
        if(activeMenuI_->menuType() == MenuInterface::MenuType::MULTIPLAYER_ON_ONE_DEVICE_RUNNING) {
            if(SoundEffect::isEmpty()) {
                SoundEffect::play(SoundEffect::Name::STOP_PLAYING_RUN_ON_SAND);
            }
        }
        avatar_ = Avatar::UNDEFINED;
        playerMode_ = PlayerMode::MULTIPLAYER;

        if(anyOf(activeMenuI_->menuType(), MenuInterface::MenuType::MULTIPLAYER_CREATE_GAME,
        MenuInterface::MenuType::MULTIPLAYER_FIND_GAME)) {
            setActiveMenu(MenuInterface::MenuType::ONLINE);
        } else {
            setActiveMenu(MenuInterface::MenuType::MULTIPLAYER);
        }
        if(game_) {game_->resetWorldTouchEvent();}
        touchEventHandler_.clear();
        //menuTouchEvent_.reset(nullptr);
        //menuTouchEvent_ = std::make_unique<MenuTouchEvent>();
        touchEventHandler_.add(menuTouchEvent_.get());
        //touchEventHandler_.add(backButton_.get());
        destroyMessageBoxes();
        destroyWidgets();
        if(activeMenuI_->animatedKnightsForMenu()) {
            activeMenuI_->animatedKnightsForMenu()->sitDown();
        }
    } else if(anyOf(activeMenuI_->menuType(), MenuInterface::MenuType::SETTINGS, MenuInterface::MenuType::ABOUT, MenuInterface::MenuType::MULTIPLAYER,
      MenuInterface::MenuType::LEVELS)) {
        if(activeMenuI_->menuType() == MenuInterface::MenuType::SETTINGS) {
            activeMenuItem()->settingsMenu()->resetVolumes();
        }
        avatar_ = Avatar::UNDEFINED;
        playerMode_ = PlayerMode::UNDEFINED;
        setActiveMenu(MenuInterface::MenuType::MAIN);
        if(game_) {game_->buttons().clear();}
        touchEventHandler_.clear();
        touchEventHandler_.add(menuTouchEvent_.get());
        //touchEventHandler_.add(backButton_.get());
        destroyMessageBoxes();
        destroyWidgets();
    }
}

void nextLevel() noexcept {
    goUpInTheMenu();
    EnumClassOp gl(gameLevel_);
    if(++gl < LevelFactory::GameLevel::LEVEL_PLAY_WITH_FRIEND_1) {
        gameLevel_ = gl;
        setActiveMenu(MenuInterface::MenuType::SINGLEPLAYER_RUNNIG);
        initButtons();
        initWorldEnvironment(std::make_unique<SingleplayerWorld>());
    }
}

void nextTurn() noexcept {
    if(playMode() != PlayMode::TURNBASED) return;
    environment()->tileset()->nextTurn();
}

void showMessageBox(Messagebox1* messagebox) noexcept {
#ifdef __SERVERAPP__
    if(game_ && game_->worldList().size()) {
        latestWorld()->addOtherElement(BlueTriangleUPtr(messagebox));
    }
#else
    messageboxes_.emplace_back(messagebox);
#endif
}

void showWidget(BlueTriangle* widget) noexcept {
    if(game_ && game_->worldList().size()) {
        latestWorld()->addOtherElement(BlueTriangleUPtr(widget));
    } else {
        widgets_.emplace_back(widget);
    }
}

void destroyMessageBoxes() noexcept {
    messageboxes_.clear();
}

void destroyWidgets() noexcept {
    debug("destroyWidgets");
    for(auto& w : widgets_) {
        if(w->observerTouchEvent()) {
            debug("destroyWidgets DBG id: %u", w->observerTouchEvent()->id());
            touchEventHandler_.remove(w->observerTouchEvent());
        }
    }
    widgets_.clear();
}

void showNextTurnButton() noexcept {
    if(game_->showNextTurnButton()) {
        touchEventHandler_.remove(game_->nextTurnButton());
        touchEventHandler_.insertFirst(game_->nextTurnButton());
    }
}

void hideNextTurnButton() noexcept {
    if(auto button = game_->hideNextTurnButton()) {
        touchEventHandler_.remove(button);
    }
}

void hideArrowButtons() noexcept {
    game_->hideArrowButtons();
    touchEventHandler_.remove(game_->arrowButtonRight());
    touchEventHandler_.remove(game_->arrowButtonUp());
    touchEventHandler_.remove(game_->arrowButtonDown());
    touchEventHandler_.remove(game_->arrowButtonLeft());
}

void setPlayTime(int minutes) noexcept {playTime_ = minutes;}
int playTime() noexcept {return playTime_;}

void changeNextTurnButtonLabel(TranslateInterface::Key key) noexcept {
    if(game_) {game_->changeNextTurnButtonLabel(key);}
}

void setRoomName(std::wstring roomName) noexcept {
    roomName_ = roomName;
}

const std::wstring& roomName() noexcept {
    return roomName_;
}

void clearAvatarSelection() noexcept {
    if(activeMenuI_->animatedKnightsForMenu()) {
        activeMenuI_->animatedKnightsForMenu()->clear();
    }
}

MenuInterfaceSPtr activeMenuItem() noexcept {
    return activeMenuI_;
}

void saveSettings() noexcept {
    settingMustBeSaved_ = true;
}

void saveLevelCompleted(int level) noexcept {
    levelStatus_[level] = tileset()->stonePushCounter();
    saveSettings();
}

void setLevelStatuses(int levelStatuses[]) {
    for(int i = 0; i < static_cast<int>(LevelFactory::GameLevel::LEVEL_PLAY_WITH_FRIEND_1); ++i) {
        levelStatus_[i] = levelStatuses[i];
    }
}

int* levelStatuses() noexcept {
    return levelStatus_;
}

bool playersJoined() noexcept {
#ifdef __SERVERAPP__
    if(!environment_ || !environment_->tileset()) return false;
    return environment_->tileset()->playersJoined();
#else
    return false;
#endif
}

THREAD_LOCAL unsigned playerCount_{};
unsigned playerCount() noexcept {
    return playerCount_;
}
void setPlayerCount(unsigned playerCount) noexcept {
    playerCount_ = playerCount;
}

THREAD_LOCAL double deadLineOfRunning_{};
double deadLineOfRunning() noexcept {
    return deadLineOfRunning_;
}

void stopServerAt(double deadLine) noexcept {
    deadLineOfRunning_ = deadLine;
}

THREAD_LOCAL bool furtherPlayersViewOnly_{};
void setFurtherPlayersViewOnly(bool v) noexcept {
    furtherPlayersViewOnly_ = v;
}

bool furtherPlayersViewOnly() noexcept {
    return furtherPlayersViewOnly_;
}

void undoLastStep() noexcept {
    if(environment_ && environment_->tileset() && !latestWorld()->areEnemiesDead()) {
        debug("Application::undoLastStep");
        environment_->tileset()->undoLastStep();
    }
}

THREAD_LOCAL std::wstring profileName_{};
void setProfileName(const wchar_t* profileName) noexcept {
    if(profileName) {
        profileName_ = profileName;
    } else {
        profileName_ = L"";
    }
    profileName_ = StringUtil::trim(profileName_);
}

void setProfileName(const std::wstring& profileName) noexcept {
    profileName_ = profileName;
    profileName_ = StringUtil::trim(profileName_);
}

const std::wstring& profileName() noexcept {return profileName_;}

THREAD_LOCAL unsigned viewerCount_{};
unsigned viewerCount() noexcept {return viewerCount_;}
void setViewerCount(unsigned viewerCount) noexcept {viewerCount_ = viewerCount;}

bool isUndoCompatible() noexcept {
    for(auto& x : latestWorld()->knightList()) {
        if(x.second->isRunning() || x.second->isSliding() || x.second->isDead() || x.second->strategy().get() || Math::abs(x.second->velocity()) > 1e5) {
            //debug("DBG isUndoCompatible(): 1");
            //std::cout << "isRunning: " << x.second->isRunning() <<
            //             ", isDead: " << x.second->isDead() <<
            //             ", strategy: " << x.second->strategy().get() <<
            //             " velocity: " << x.second->velocity() << "\n";
            return false;
        }
    }

    for(auto& x : latestWorld()->rollingStoneList()) {
        if(x.second->isDead() || Math::abs(x.second->velocity()) > 1e5 || x.second->isRolling()) {
            //debug("DBG isUndoCompatible(): 2");
            return false;
        }
    }

    for(auto& x : latestWorld()->rpgBgElementList()) {
        if(Algorithms::anyOf(x.second->skinMin(), NATIVE_BITMAP_HOLE_01, NATIVE_BITMAP_HOLE_RED_01, NATIVE_BITMAP_HOLE_BLUE_01, NATIVE_BITMAP_HOLE_DARK_01)) {
            if(x.second->isDead()) {
                //debug("DBG isUndoCompatible(): 3");
                return false;
            }
        }
    }
    return true;
}

void showEmotesBubble() noexcept {
    if(game_ && game_->emotesBubble()) game_->emotesBubble()->setVisible(true);
}

void hideEmotesBubble() noexcept {
    if(game_ && game_->emotesBubble()) game_->emotesBubble()->setVisible(false);
}

bool isEmotesBubbleVisible() noexcept {
    if(game_ && game_->emotesBubble()) return game_->emotesBubble()->visible();
    return false;
}

void setResolution(int index) noexcept {
    resolutionIndex_ = index;
}

int resolution() noexcept {
    return resolutionIndex_;
}

void resetMenu() noexcept {
    activeMenuI_ = nullptr;
    screen_ = nullptr;
    game_ = nullptr;
    if(language_ != TranslateInterface::Language::ENGLISH) {
        auto lang = language_;
        language_ = TranslateInterface::Language::ENGLISH;
        setLanguage(lang);
    } else {
        language_ = TranslateInterface::Language::HUNGARIAN;
        setLanguage(TranslateInterface::Language::ENGLISH);
    }
    if(backButton_) {
        touchEventHandler_.remove(backButton_.get());
        backButton_ = nullptr;
    }
}
} // namespace Application

// Global.h
const char* getSoundEffect() {
    return SoundEffect::getName();
}

