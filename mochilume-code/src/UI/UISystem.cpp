#include "UISystem.h"

UISystem* UISystem::_instance = nullptr;

UISystem::UISystem() {}

UISystem::~UISystem() {
    clear();
}

UISystem* UISystem::getInstance() {
    if (_instance == nullptr) {
        _instance = new UISystem();
    }
    return _instance;
}

void UISystem::pushUIList(UIList* list) {
    if (list) {
        uiLists.push_back(list);
    }
}

void UISystem::popUIList() {
    if (!uiLists.empty()) {
        uiLists.pop_back();
    }
}

UIList* UISystem::getTopUIList() const {
    if (!uiLists.empty()) {
        return uiLists.back();
    }
    return nullptr;
}

void UISystem::renderAll(Adafruit_GFX* gfx) {
    if (!gfx || uiLists.empty()) {
        return;
    }
    
    std::vector<UIList*> sortedLists = uiLists;
    std::sort(sortedLists.begin(), sortedLists.end(), 
        [](UIList* a, UIList* b) { return a->getZIndex() < b->getZIndex(); });
    
    for (auto list : sortedLists) {
        list->render(gfx);
    }
}

void UISystem::handleNextButton() {
    UIList* topList = getTopUIList();
    if (topList) {
        topList->nextOption();
    }
}

void UISystem::handlePrevButton() {
    UIList* topList = getTopUIList();
    if (topList) {
        topList->prevOption();
    }
}

void UISystem::handleOKButton() {
    UIList* topList = getTopUIList();
    if (topList) {
        topList->selectCurrent();
    }
}

void UISystem::clear() {
    for (auto list : uiLists) {
        delete list;
    }
    uiLists.clear();
}
