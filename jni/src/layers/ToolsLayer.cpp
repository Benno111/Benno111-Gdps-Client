#include <gd.h>
#include <Logger.h>
#include <sstream>
#include "cocos2d.h"
#include "ToolsLayer.h"
#include "StorageExporter.h"
#include <cocos2dx_bak/extensions/GUI/CCControlExtension/CCScale9Sprite.h>

bool ToolsLayer::init()
{

    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();

    auto bg = CCSprite::create("GJ_gradientBG.png");

    bg->setPosition(CCPoint(0, winSize.height / 2));
    bg->setScaleX(winSize.width);
    bg->setScaleY(winSize.height / bg->getContentSize().height);

    bg->setColor(ccc3(0, 0x66, 0xFF));

    this->addChild(bg, -2);

    auto misc = CCMenu::create();
    this->addChild(misc, 50);

    auto m_pButtonMenu = CCMenu::create();
    this->addChild(m_pButtonMenu);

    auto arrow = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
    auto backBtn = CCMenuItemSpriteExtra::create(
        arrow,
        arrow,
        this,
        menu_selector(ToolsLayer::onBack));
    backBtn->setPosition((-winSize.width / 2) + 25.0f, (winSize.height / 2) - 25.0f);
    misc->addChild(backBtn);

    auto background = cocos2d::extension::CCScale9Sprite::create("GJ_square01.png");
    background->setContentSize(cocos2d::CCSize(380.0f, 240.0f));
    background->setPosition(winSize.width / 2, winSize.height / 2);
    this->addChild(background);

    auto list = GJListLayer::create(NULL, "Tools", ccc4(255, 0, 0, 0), 356, 220);

    list->setPosition(ccp(
        winSize.width / 2 - list->getContentSize().width / 2,
        winSize.height / 2 - list->getContentSize().height / 2));
    this->addChild(list);

    float scale = 0.85;

    auto tool = CCSprite::createWithSpriteFrameName("GJ_usernameBtn_001.png");
    auto toolBtn = CCMenuItemSpriteExtra::create(
        tool,
        tool,
        this,
        menu_selector(ToolsLayer::onUsername));
    toolBtn->setPosition((CCMIDX / 50), (CCMIDY / 5) + 18.5); // idk what i did here lol
    tool->setScale(scale);
    misc->addChild(toolBtn, 50);

    auto referenceX = toolBtn->getPositionX();
    auto referenceY = toolBtn->getPositionY();

    auto tool2 = CCSprite::createWithSpriteFrameName("GJ_uploadMusicBtn_001.png");
    auto toolBtn2 = CCMenuItemSpriteExtra::create(
        tool2,
        tool2,
        this,
        menu_selector(ToolsLayer::onMusic));

    toolBtn2->setPosition(referenceX - 100, referenceY);
    tool2->setScale(scale);
    misc->addChild(toolBtn2, 50);

    auto tool3 = CCSprite::createWithSpriteFrameName("GJ_requestBtn_001.png");
    auto toolBtn3 = CCMenuItemSpriteExtra::create(
        tool3,
        tool3,
        this,
        menu_selector(ToolsLayer::onLevelRequest));

    toolBtn3->setPosition(referenceX + 100, referenceY);
    tool3->setScale(scale);
    misc->addChild(toolBtn3);

    auto tool4 = CCSprite::createWithSpriteFrameName("GJ_passwordBtn_001.png");
    auto toolBtn4 = CCMenuItemSpriteExtra::create(
        tool4,
        tool4,
        this,
        menu_selector(ToolsLayer::onPassword));

    toolBtn4->setPosition(referenceX, referenceY - 100);
    tool4->setScale(scale);
    misc->addChild(toolBtn4);

    auto tool5 = CCSprite::createWithSpriteFrameName("GJ_unlistedBtn_001.png");
    auto toolBtn5 = CCMenuItemSpriteExtra::create(
        tool5,
        tool5,
        this,
        menu_selector(ToolsLayer::onUnlisted));

    toolBtn5->setPosition(referenceX - 100, referenceY - 100);
    tool5->setScale(scale);
    misc->addChild(toolBtn5);

    auto tool6 = CCSprite::createWithSpriteFrameName("GJ_gdbrowsertBtn_001.png");
    auto toolBtn6 = CCMenuItemSpriteExtra::create(
        tool6,
        tool6,
        this,
        menu_selector(ToolsLayer::onGDBrowser));

    toolBtn6->setPosition(referenceX + 100, referenceY - 100);
    tool6->setScale(scale);
    misc->addChild(toolBtn6);

    auto copySprite = ButtonSprite::create("Copy Files", 90, 10, 10, 5);
    auto copyButton = CCMenuItemSpriteExtra::create(
        copySprite,
        copySprite,
        this,
        menu_selector(ToolsLayer::onCopyGameFiles));
    copyButton->setPosition(referenceX, referenceY - 50);
    misc->addChild(copyButton, 50);

    // this->addChild(m);

    this->setTouchEnabled(true);
    this->setKeypadEnabled(true);

    return true;
}

void ToolsLayer::onCopyGameFiles(CCObject *)
{
    const unsigned copied = StorageExporter::copySaveFiles();
    const char *message = copied == 2
        ? "CCGameManager.dat and CCLocalLevels.dat were copied to /sdcard/Benno111GDPS/GameFiles."
        : copied == 1
            ? "One save file was copied. The other file was missing or could not be read."
            : "No save files were copied. Check that the files exist and storage permission is granted.";
    FLAlertLayer::create(nullptr, copied ? "Files Copied" : "Copy Failed",
                         message, "OK", nullptr, 400, false, 300)->show();
}
