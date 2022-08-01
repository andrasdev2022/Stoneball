#include "GameObject.h"
#include "stoneball.h"
#include "GameObjectDescriptor.h"
#include "MediaData.h"
#include "Global.h"
#include "Bitmap565.h"
#include "Screen.h"
#include "Application.h"
#include <string.h>
#include <math.h>
#include "Size.h"
#include "Environment.h"
#include "Defines.h"
#include "Memory.h"
#include "UDPMessageData.h"
#include "IdGenerator.h"

using namespace Bitmap565;


GameObject::GameObject(double x, double bottomY, Type type) noexcept
: x_(x)
, y_(bottomY)
, direction_(0., 0.)
, type_(type)
, id_(IdGenerator::generateId())
, stateChangeAvailable_(true)
{
}

void GameObject::init() noexcept {
    width_ = bitmapWidth(god().skinMin());
    height_ = bitmapHeight(god().skinMin());
    velocity_ = god().velocity();
    animTimeStep_ = god().animTimeStep();
    frameIndex_ = god().skinStart();
    skinMin_ = god().skinMin();
    skinMax_ = god().skinMax();
    y_ -= contentBottom() - y_;
}

// srcBitmap: e.g. chibi1.png
// dstBitmap: e.g. canvas bitmap
// srcX, srcY: position on chibi1.png
// srcWidth, srcHeight: size of chibi1.png
// this->width, this->height: size of 1(!) chibi object
// this->x_, this->y_: pixel position on dst bitmap, e.g. background or canvas
/*
void GameObject::copyImg(const uint16_t* srcBitmap, int srcX, int srcY, int srcWidth, int srcHeight,
                        AndroidBitmapInfo* dstInfo, uint16_t* dstBitmap) const
{
    int convertedY = (int)this->y_;
    int startY = 0;
    if(convertedY < 0) {
        startY = -convertedY;
        convertedY = 0;
    }

    // first line on the target bitmap
    const uint16_t*  srcLine = srcBitmap + srcWidth * (srcY + startY) + srcX;
    uint16_t*  dstLine = ((uint16_t*)((char*)dstBitmap + dstInfo->stride * convertedY)) + (int)this->x_;

    for(int Y = startY; Y < this->height_; ++Y) {
        if(Y + convertedY < dstInfo->height) {
            for(int X = 0; X < this->width_; ++X) {
                if(X + (int)this->x_ >= 0 && X + (int)this->x_ < dstInfo->width && srcLine[X] != *srcBitmap) {
                    dstLine[X] = srcLine[X];
                }
            }
        }
        srcLine += srcWidth;
        dstLine = (uint16_t*)((char*)dstLine + dstInfo->stride);
    }
}
*/

void GameObject::copyImgRareMatrix(const MediaData* frameData, AndroidBitmapInfo* dstInfo, uint16_t* dstBitmap) const noexcept {
    const Rect& contentRect = frameData->contentRect();
    if((int)y_ + contentRect.y + contentRect.height <= 0) return;
    if((int)y_ + contentRect.y >= (int)dstInfo->height) return;
    if((int)x_ + contentRect.x + contentRect.width <= 0)  return;
    if((int)x_ + contentRect.x >= (int)dstInfo->width) return;

    //LOGI("GameObject::copyImgRareMatrix contentRect.y: %d contentRect.width: %d, contentRect.height: %d\n", contentRect.y, contentRect.width, contentRect.height);
    const uint16_t* rareMatrix = frameData->data();

    uint32_t size = ((uint32_t)rareMatrix[0] | ((uint32_t)rareMatrix[1] << 16));

    //if(type_ == Type::GOT_ATTACKBUTTON1) {
    //    LOGI("Size: %u", size);
    //}

    const uint16_t* rareMatrixEnd = rareMatrix + size;
    ++rareMatrix;++rareMatrix;
    //int drawnLines = 0;
    int lastY = -1;
    //int sizeSum = 0;
    while(rareMatrixEnd > rareMatrix) {
        int x = *rareMatrix++;
        int y = *rareMatrix++;
        int length = *rareMatrix++;
        int jump = length;
        //if(type_ == Type::GOT_ATTACKBUTTON1) {
        //    LOGI("lastY: %d, Jump: %d", lastY, jump);
        //}

        if((int)y_ + y >= (int)dstInfo->height) {
            //if(type_ == Type::GOT_ATTACKBUTTON1) LOGI("GameObject::copyImgRareMatrix dbg0\n");
            break;
        }
        if((int)y_ + y < 0) {
            rareMatrix += jump;
            //if(type_ == Type::GOT_ATTACKBUTTON1) LOGI("GameObject::copyImgRareMatrix dbg1\n");
            continue;
        }

        if((int)x_ + x >= (int)dstInfo->width) {
            //goto next;
            rareMatrix += jump;
            //if(type_ == Type::GOT_ATTACKBUTTON1) LOGI("GameObject::copyImgRareMatrix dbg2\n");
            continue;
        } else if((int)x_ + x + length > (int)dstInfo->width) {
            length = dstInfo->width - (int)x_ - x;
            //if(type_ == Type::GOT_ATTACKBUTTON1) LOGI("GameObject::copyImgRareMatrix dbg3\n");
        } else if((int)x_ + x < 0) {
            if(length <= -((int)x_ + x)) {
                rareMatrix += jump;
                //if(type_ == Type::GOT_ATTACKBUTTON1) LOGI("GameObject::copyImgRareMatrix dbg4\n");
                continue;
            }
            //if(type_ == Type::GOT_ATTACKBUTTON1) LOGI("GameObject::copyImgRareMatrix dbg5\n");
            rareMatrix += -((int)x_ + x); // move forward
            length += (int)x_ + x; // decrease
            jump += (int)x_ + x; // decrease
            x = -(int)x_;
        }

        if(skipEveryNthLine > 0) {
            if(lastY != (int)y_ + y) {
                ++lineCounter;
                lastY = (int)y_ + y;
            }
            if((lineCounter % skipEveryNthLine) != 0) {
                uint16_t*  dstPos = ((uint16_t*)((char*)dstBitmap + dstInfo->stride * ((int)y_ + y))) + (int)x_ + x;
                memcpy(dstPos, rareMatrix, length * sizeof(uint16_t));
            }
        } else {
            uint16_t*  dstPos = ((uint16_t*)((char*)dstBitmap + dstInfo->stride * ((int)y_ + y))) + (int)x_ + x;
            memcpy(dstPos, rareMatrix, length * sizeof(uint16_t));
        }
        rareMatrix += jump;
        //sizeSum += jump + 3 * sizeof(uint16_t);
        //if(lastY != y) {
        //    ++drawnLines;
        //    lastY = y;
        //}
    }
    //if(type_ == Type::GOT_ATTACKBUTTON1) {
    //    LOGI("GameObject::copyImgRareMatrix drawnLines: %d, sizeSum: %d\n", drawnLines, sizeSum);
    //}
}

void GameObject::copyImg(const MediaData* frameData, AndroidBitmapInfo* dstInfo, uint16_t* dstBitmap) const noexcept {
    //LOGI("GameObject::copyImg frameData->type(): %d\n", frameData->type());
    if(frameData->type() == MediaData::Type::RAREMATRIX) {
        copyImgRareMatrix(frameData, dstInfo, dstBitmap);
        return;
    }

    const Rect& contentRect = frameData->contentRect();

    if((int)y_ + contentRect.y + contentRect.height <= 0) {
        return;
    }
    const uint16_t transparentPixel = 0;

    int convertedY = (int)y_ + contentRect.y; // y position on the screen
    int startY = 0;  // y position on the sprite
    if(convertedY < 0) {
        startY = -convertedY;
        convertedY = 0;
    }

    int srcPos = frameData->width() * (contentRect.y + startY) + contentRect.x;
    const uint16_t*  srcLine = frameData->data() + srcPos;
    uint16_t*  dstLine = ((uint16_t*)((char*)dstBitmap + dstInfo->stride * convertedY)) + (int)this->x_ + contentRect.x;

    for(int Y = 0; Y < contentRect.height - startY; ++Y) {
        if(convertedY + Y < (int)dstInfo->height) {
            for(int X = 0; X < contentRect.width; ++X) {
                if(srcLine[X] != transparentPixel && X + (int)this->x_ + contentRect.x >= 0 && X + (int)this->x_ + contentRect.x < (int)dstInfo->width) {
                    if(transparency_) {
                        dstLine[X] = fastMix565(srcLine[X], dstLine[X], transparency_);
                        //dstLine[X] = alphaBlendRGB565(srcLine[X], dstLine[X], transparency_);
                    } else {
                        dstLine[X] = srcLine[X];
                    }
                }
            }
        }
        srcLine += frameData->width();
        dstLine = (uint16_t*)((char*)dstLine + dstInfo->stride);
    }
}

void GameObject::drawHPBar(AndroidBitmapInfo* dstInfo, uint16_t* dstBitmap) const noexcept {
    if((int)y_ < 0 || hitPoints_ < 0 || hitPoints_ == maxHitPoints_) {
        return;
    }
    const int barHeight = 6;
    int convertedY = (int)y_ - barHeight;
    int startY = 0;
    if(convertedY < 0) {
        startY = -convertedY;
        convertedY = 0;
    }
    int fullLength = 70_ZRD;
    int greenLength = fullLength * hitPoints_ / maxHitPoints_;
    int startX = (width_ - fullLength) / 2;
    uint16_t*  dstLine = ((uint16_t*)((char*)dstBitmap + dstInfo->stride * convertedY)) + (int)this->x_ + startX;

    const uint16_t black = make565(0, 0, 0);
    const uint16_t green = make565(78, 131, 78);

    for(int Y = 0; Y < barHeight - startY; ++Y) {
        if(convertedY + Y < (int)dstInfo->height) {
            for(int X = 0; X < fullLength; ++X) {
                if(X + (int)this->x_ + startX >= 0 && X + (int)this->x_ + startX < (int)dstInfo->width) {
                    if(X == 0 || X == fullLength -1 || Y == 0 || Y == barHeight -1) {
                        dstLine[X] = black;
                        continue;
                    }
                    if(X <= greenLength) {
                        dstLine[X] = green;
                    }
                }
            }
        }
        dstLine = (uint16_t*)((char*)dstLine + dstInfo->stride);
    }
}

void GameObject::updateFrame(double now, int minFrameIndex, int maxFrameIndex) noexcept {
    // animation
    if(lastAnimFrameTmps < 1e-4) {
        lastAnimFrameTmps = now;
    }
    while(now - lastAnimFrameTmps > animTimeStep_) {
        frameIndex_ = (frameIndex_ + 1) % (maxFrameIndex + 1);
        if(frameIndex_ == 0) frameIndex_ = minFrameIndex;
        lastAnimFrameTmps += animTimeStep_;
    }
}

void GameObject::draw(AndroidBitmapInfo* info, void* pixels) noexcept {
    copyImg(frameData(), info, (uint16_t*)pixels);
    drawHPBar(info, (uint16_t*)pixels);
#ifdef __TEST__
    drawColliderShape(info, (uint16_t*)pixels);
#endif
}

void GameObject::walkOrMove(double now) noexcept {
    // moving
    SizeI sizeI(0,0);
    if(Application::environment()) {
        sizeI = Application::environment()->gameMap()->size();
    } else {
        sizeI = SizeI(screenWidth(), screenHeight());
    }

    const int maxWidth = sizeI.width;
    const int maxHeight = sizeI.height;

    if(lastUpdateTmps < 1e-4) {
        lastUpdateTmps = now;
    }

    double distance = velocity_ * (now - lastUpdateTmps);
    x_ += distance * direction_.x;
    y_ += distance * direction_.y;
    lastUpdateTmps = now;

    useBorderRestrictions(maxWidth, maxHeight);
}

void GameObject::jump(double now, double jumpTime, double maxHeightOfJump, double correction, double& startTimeOfJump, double& yPosAtStart) noexcept {
    SizeI sizeI(0, 0);
    if(Application::environment()) {
        sizeI = Application::environment()->gameMap()->size();
    } else {
        sizeI = SizeI(screenWidth(), screenHeight());
    }
    const int maxWidth = sizeI.width;
    // TODO: maxheight should be the gamemapheight
    //const int maxHeight = 100000;
    const int maxHeight = sizeI.height;

    if(lastUpdateTmps < 1e-4) {
        lastUpdateTmps = now;
    }

    if(startTimeOfJump < 0.) {
        startTimeOfJump = now;
        yPosAtStart = this->y_;
    }

    if(frameIndex_ == skinMin_) {
        // synhro with the frame player
        //startTimeOfJump = now;
        //lastUpdateTmps = now;
        //this->y_ = yPosAtStart;
        //LOGI("DBG jump reset of startTimeOfJump");
    }

    if(lastUpdateTmps - startTimeOfJump < -1e-5) {
        //LOGI("DBG GameObject::jump lastUpdateTmps < startTimeOfJump!!!!1");
    }

    if(now - startTimeOfJump > jumpTime) {
        //LOGI("DBG GameObject::jump we should have fell down: %f", now - startTimeOfJump);
        //startTimeOfJump = now;
        //this->y_ = yPosAtStart;
    }
    double heightOfJump = maxHeightOfJump * sin(M_PI / jumpTime * (now - startTimeOfJump + 1e-7));
    if(heightOfJump < 1e-7) heightOfJump = -heightOfJump ;

    double distance = velocity_ * (now - lastUpdateTmps + 1e-7);
    x_ += distance * direction_.x;
    y_ = yPosAtStart - heightOfJump + correction;
    lastUpdateTmps = now;

    if(lastUpdateTmps - startTimeOfJump < -1e-5) {
        //LOGI("DBG jump lastUpdateTmps < startTimeOfJump!!!!2: %f, %f, %f", lastUpdateTmps - startTimeOfJump, lastUpdateTmps, startTimeOfJump);
    }
    useBorderRestrictions(maxWidth, maxHeight);
}

void GameObject::useBorderRestrictions(const int bgWidth, const int bgHeight) noexcept {
    if( x_ >= bgWidth - width_ / 2) {
        x_ = bgWidth - width_ / 2;
    }

    if( y_ >= bgHeight - height_ / 2) {
        y_ = bgHeight - height_ / 2;
    }

    if(x_ < -width_ / 2) {
        x_ = -width_ / 2;
    } else if(x_ < 0_ZRW - width_ / 2) {
        // Ha a mobil képernyő hosszú, akkor rögzített képarányú kép középre igazítva jelenik meg.
        // Le kell korlátozni a jobb oldalt az objectek mozgását. De ez nem jól működik tileset esetén
        //x_ = 0_ZRW - width_ / 2;
    }

    if(y_ < -height_ / 2) {
        y_ = -height_ / 2;
    }
}

void GameObject::yCorrectionByGravity(double newY, bool collision) noexcept {
    Unused(0, collision);
    y_ = newY;
    // TODO: maxheight should be tha gamemapheight
    const int maxYPos = 100000;
    //const int maxYPos = screenPosY() + screenHeight();
    if( y_ >= maxYPos + height_ / 2) {
        y_ = maxYPos - height_ / 2;
    }
}

const MediaData* GameObject::frameData() const noexcept {
    return bitmapData(frameIndex_, flipped());
}

double GameObject::contentBottom() const noexcept {
    if(frameIndex_ == -1) return y_;
    int maxHeight = 0;
    for(int i = god().skinStart(); i <= god().skinMax(); ++i) {
        const Rect& rect = bitmapData(i, flipped())->contentRect();
        if(maxHeight < rect.y + rect.height) {
            maxHeight = rect.y + rect.height;
        }
    }
    return y_ + maxHeight;
}


void GameObject::draw(Screen* screen) noexcept {
    //LOGI("DBG GameObject::draw begin %d", type_);
    const MediaData* fData = frameData();
    const Rect& contentRect = fData->contentRect();

    //LOGI("DBG GameObject::draw fData->type(): %d", static_cast<int>(fData->type()));

    y_ = y();
    x_ = x();
    width_ = width();
    height_ = height();

    Rect rect(x_ + contentRect.x, y_ + contentRect.y, contentRect.width, contentRect.height);

    // frame is under the screen
    if(rect.y >= screen->y() + screen->height()) return;
    // frame right to the screen
    if(rect.x >= screen->x() + screen->width()) return;
    // frame is above the screen
    if(rect.y + rect.height <= screen->y()) return;
    // frame left to the screen
    if(rect.x + rect.width <= screen->x()) return;
    //LOGI("DBG GameObject::draw 1");
    if(fData->type() == MediaData::Type::RAREMATRIX) {
        //LOGI("DBG GameObject::draw rarematrix");
        copyIntersectionRarematrix(screen->content(), screen->rect(), screen->rect().width, fData->data(), Rect(x_, y_, width_, height_));
    } else {
        //LOGI("DBG GameObject::draw 12");
        uint16_t* startSrcPos = fData->data() + contentRect.y * fData->width() + contentRect.x;
        copyIntersection(screen->content(), screen->rect(), screen->rect().width, startSrcPos, rect, fData->width(), false,
                         transparency_);
    }
    //LOGI("DBG GameObject::draw 2");
    drawHPBar(screen);
#ifdef __TEST__
    drawColliderShape(screen);
#endif
    //LOGI("DBG GameObject::draw end %d", type_);
}

void GameObject::drawHPBar(Screen* screen) noexcept {
    if((int)y_ < screen->y() || hitPoints_ < 0 || hitPoints_ == maxHitPoints_) {
        return;
    }

    const int barHeight = 6;
    int convertedY = (int)y_ - screen->y() - barHeight;
    int startY = 0;
    if(convertedY < 0) {
        startY = -convertedY;
        convertedY = 0;
    }
    int fullLength = 70_ZRD;
    int greenLength = fullLength * hitPoints_ / maxHitPoints_;
    int startX = (width_ - fullLength) / 2;
    uint16_t*  dstLine = screen->content() + convertedY * screen->width() + ((int)this->x_ - screen->x()) + startX;

    const uint16_t black = make565(0, 0, 0);
    const uint16_t green = make565(78, 131, 78);
    for(int Y = 0; Y < barHeight - startY; ++Y) {
        if(convertedY + Y < (int)screen->height()) {
            for(int X = 0; X < fullLength; ++X) {
                if(X + (int)(this->x_ - screen->x()) + startX >= 0 && X + (int)(this->x_ - screen->x()) + startX < (int)screen->width()) {
                    if(X == 0 || X == fullLength -1 || Y == 0 || Y == barHeight -1) {
                        dstLine[X] = black;
                        continue;
                    }
                    if(X <= greenLength) {
                        dstLine[X] = green;
                    }
                }
            }
        }
        dstLine += screen->width();
    }
}

// TODO: implement
void GameObject::drawColliderShape(AndroidBitmapInfo* dstInfo, uint16_t* dstBitmap) const noexcept {
    Unused(0, dstInfo, dstBitmap);
    if(!collisionData_) return;
}

void GameObject::drawColliderShape(Screen* screen) noexcept {
    if((int)y_ < screen->y() || !collisionData_) {
        return;
    }

    Rect collisionRect = collisionData_->rect();
    const int barHeight = collisionRect.height;
    int convertedY = (int)y_ - screen->y() + collisionRect.y;
    int startY = 0;
    if(convertedY < 0) {
        startY = -convertedY;
        convertedY = 0;
    }
    int fullLength = collisionRect.width;
    int startX = collisionRect.x;
    uint16_t*  dstLine = screen->content() + convertedY * screen->width() + ((int)this->x_ - screen->x()) + startX;

    const uint16_t red = make565(218, 0, 0);
    for(int Y = 0; Y < barHeight - startY; ++Y) {
        if(convertedY + Y < (int)screen->height()) {
            for(int X = 0; X < fullLength; ++X) {
                if(X + (int)(this->x_ - screen->x()) + startX >= 0 && X + (int)(this->x_ - screen->x()) + startX < (int)screen->width()) {
                    if(X == 0 || X == fullLength -1 || Y == 0 || Y == barHeight -1) {
                        dstLine[X] = red;
                        continue;
                    }
                }
            }
        }
        dstLine += screen->width();
    }
}

void GameObject::wakeUp(double now) noexcept {
    lastAnimFrameTmps = now;
    lastUpdateTmps = now;
}

std::string GameObject::str(Type t) noexcept {
    switch(t) {
        case Type::GOT_KNIGHT_RED: return "GOT_KNIGHT_RED"; break;
        case Type::GOT_KNIGHT_DARK: return "GOT_KNIGHT_DARK"; break;
        case Type::GOT_KNIGHT_BLONDE_FEMALE: return "GOT_KNIGHT_BLONDE_FEMALE"; break;
        case Type::GOT_KNIGHT_GREEN_FEMALE: return "GOT_KNIGHT_GREEN_FEMALE"; break;
        case Type::GOT_CONTROLLERDISK1: return "GOT_CONTROLLERDISK1"; break;
        case Type::GOT_CONTROLLERDISK2: return "GOT_CONTROLLERDISK2"; break;
        case Type::GOT_ARROW_RIGHT: return "GOT_ARROW_RIGHT"; break;
        case Type::GOT_ARROW_UP: return "GOT_ARROW_UP"; break;
        case Type::GOT_ARROW_DOWN: return "GOT_ARROW_DOWN"; break;
        case Type::GOT_ARROW_LEFT: return "GOT_ARROW_LEFT"; break;
        case Type::GOT_GREEN_GEM1: return "GOT_GREEN_GEM1"; break;
        case Type::GOT_BLUE_GEM1: return "GOT_BLUE_GEM1"; break;
        case Type::GOT_BULLET: return "GOT_BULLET"; break;
        case Type::GOT_PARTICLE: return "GOT_PARTICLE"; break;
        case Type::GOT_ROLLINGSTONE: return "GOT_ROLLINGSTONE"; break;
        case Type::GOT_RPGBGELEMENT: return "GOT_RPGBGELEMENT"; break;
        case Type::GOT_REFRESH: return "GOT_REFRESH"; break;
        case Type::GOT_UNDO: return "GOT_UNDO"; break;
        case Type::GOT_BACK: return "GOT_BACK"; break;
        case Type::GOT_NEXT_TURN: return "GOT_NEXT_TURN"; break;
        case Type::GOT_BLUE_TRIANGLE: return "GOT_BLUE_TRIANGLE"; break;
        case Type::GOT_RED_BOUNCING_STONE: return "GOT_RED_BOUNCING_STONE"; break;
        case Type::GOT_GRAY_BOUNCING_STONE: return "GOT_GRAY_BOUNCING_STONE"; break;
        case Type::GOT_YELLOW_BOUNCING_STONE: return "GOT_YELLOW_BOUNCING_STONE"; break;
        case Type::GOT_GREEN_BOUNCING_STONE: return "GOT_GREEN_BOUNCING_STONE"; break;
        case Type::GOT_MESSAGEBOX: return "GOT_MESSAGEBOX"; break;
        case Type::GOT_LISTBOX: return "GOT_LISTBOX"; break;
        case Type::GOT_SELECTIONBOX: return "GOT_SELECTIONBOX"; break;
        case Type::GOT_DECK_OF_CARDS: return "GOT_DECK_OF_CARDS"; break;
        case Type::GOT_CARD_PULL: return "GOT_CARD_PULL"; break;
        case Type::GOT_CARD_PUSH_PLAYER: return "GOT_CARD_PUSH_PLAYER"; break;
        case Type::GOT_CARD_TELEPORT: return "GOT_CARD_TELEPORT"; break;
        case Type::GOT_QUESTIONMARK: return "GOT_QUESTIONMARK"; break;
        case Type::GOT_PASS: return "GOT_PASS"; break;
        case Type::GOT_SLIDER: return "GOT_SLIDER"; break;
        case Type::GOT_MATCH_RESULTS: return "GOT_MATCH_RESULTS"; break;
        case Type::GOT_MESSAGEBOXYESNO: return "GOT_MESSAGEBOXYESNO"; break;
        case Type::GOT_FIREWORK_LONG_BLUE: return "GOT_FIREWORK_LONG_BLUE"; break;
        case Type::GOT_FIREWORK_LONG_ORANGE: return "GOT_FIREWORK_LONG_ORANGE"; break;
        case Type::GOT_FIREWORK_LONG_GREEN: return "GOT_FIREWORK_LONG_GREEN"; break;
        case Type::GOT_TROPHY_NR1: return "GOT_TROPHY_NR1"; break;
        case Type::GOT_TROPHY_NR2: return "GOT_TROPHY_NR2"; break;
        case Type::GOT_TROPHY_NR3: return "GOT_TROPHY_NR3"; break;
        case Type::GOT_EMOTE_BUTTON: return "GOT_EMOTE_BUTTON"; break;
        case Type::GOT_EMOTES_BUBBLE: return "GOT_EMOTES_BUBBLE"; break;
        case Type::GOT_EMOTE_FIGURE: return "GOT_EMOTE_FIGURE"; break;
        case Type::GOT_MESSAGEBOXTERMSOFUSE: return "GOT_MESSAGEBOXTERMSOFUSE"; break;
        case Type::COUNT: return "COUNT"; break;
    }
    return "";
}

std::string GameObject::str(Event e) noexcept {
    switch(e) {
        case Event::ACTION_DOWN: return "ACTION_DOWN"; break;
        case Event::ACTION_UP: return "ACTION_UP"; break;
        case Event::ACTION_MOVE: return "ACTION_MOVE"; break;
        case Event::ACTION_CANCEL: return "ACTION_CANCEL"; break;
        case Event::ACTION_OUTSIDE: return "ACTION_OUTSIDE"; break;
        case Event::ACTION_POINTER_DOWN: return "ACTION_POINTER_DOWN"; break;
        case Event::ACTION_POINTER_UP: return "ACTION_POINTER_UP"; break;
        case Event::ACTION_MASKED_POINTER_DOWN_1: return "ACTION_MASKED_POINTER_DOWN_1"; break;
        case Event::ACTION_MASKED_POINTER_DOWN_2: return "ACTION_MASKED_POINTER_DOWN_2"; break;
        case Event::ACTION_MASKED_POINTER_UP_1: return "ACTION_MASKED_POINTER_UP_1"; break;
        case Event::ACTION_MASKED_POINTER_UP_2: return "ACTION_MASKED_POINTER_UP_2"; break;
    }
    return "";
}

double GameObject::z() const noexcept {
    //return y() + contentBottom();
    // Remeber the case of lake and the tree
    return y() + height();
}

bool GameObject::addData(Serialize* serialize) {
    const Rect& contentRect = frameData()->contentRect();
    //msgData.contentBottom = y() + contentRect.y + contentRect.height;
    //msgData.frameIndex = gameObject->frameIndex();
    //msgData.skinMin = gameObject->skinMin();
    //msgData.skinMax = gameObject->skinMax();
    //msgData.gameObjectType = static_cast<unsigned char>(gameObject->type());
    //msgData.flipped = gameObject->flipped();
    //msgData.hitPoints = gameObject->hitPoints();
    //msgData.maxHitPoints = gameObject->maxHitPoints();
    //msgData.dead = gameObject->isDead();

    return serialize->add((uint32_t)id()) != Serialize::SERIALIZE_ERROR &&
           serialize->add(x()) != Serialize::SERIALIZE_ERROR &&
           serialize->add(y() + contentRect.y + contentRect.height) != Serialize::SERIALIZE_ERROR &&
           serialize->add((uint16_t)frameIndex()) != Serialize::SERIALIZE_ERROR &&
           serialize->add((uint16_t)skinMin()) != Serialize::SERIALIZE_ERROR &&
           serialize->add((uint16_t)skinMax()) != Serialize::SERIALIZE_ERROR &&
           serialize->add((uint8_t)type()) != Serialize::SERIALIZE_ERROR &&
           serialize->add((uint8_t)flipped()) != Serialize::SERIALIZE_ERROR &&
           serialize->add((uint8_t)isDead()) != Serialize::SERIALIZE_ERROR
            ;
}

// copy data into the UDPMessageData, it will be synchronized by SnapshotSyncher
// and MultiplayerWorld
const uint8_t* GameObject::buildFrom(UDPMessageData* messageData, const uint8_t* src) {
    src = Memory::copy(messageData->id, src);
    src = Memory::copy(messageData->x, src);
    src = Memory::copy(messageData->contentBottom, src);
    src = Memory::copy(messageData->frameIndex, src);
    src = Memory::copy(messageData->skinMin, src);
    src = Memory::copy(messageData->skinMax, src);
    //src = Memory::copy(messageData->state, src);
    src = Memory::copy(messageData->gameObjectType, src);
    src = Memory::copy(messageData->flipped, src);
    //src = Memory::copy(messageData->hitPoints, src);
    //src = Memory::copy(messageData->maxHitPoints, src);
    src = Memory::copy(messageData->dead, src);
    return src;
}
