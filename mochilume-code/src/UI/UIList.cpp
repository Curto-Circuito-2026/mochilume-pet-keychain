#include "UIList.h"

UIList::UIList(uint8_t zIndex) 
    : curOptionIndex(0), zIndex(zIndex) {}

UIList::~UIList() {
    clear();
}

void UIList::addOption(UIOption* option) {
    if (option) {
        options.push_back(option);
    }
}

void UIList::addOption(const char* label, UICallback callback) {
    options.push_back(new UIOption(label, callback));
}

void UIList::removeOption(uint8_t index) {
    if (index < options.size()) {
        delete options[index];
        options.erase(options.begin() + index);
        
        if (curOptionIndex >= options.size() && curOptionIndex > 0) {
            curOptionIndex--;
        }
    }
}

void UIList::clear() {
    for (auto option : options) {
        delete option;
    }
    options.clear();
    curOptionIndex = 0;
}

void UIList::setCurOptionIndex(uint8_t index) {
    if (index < options.size()) {
        curOptionIndex = index;
    }
}

UIOption* UIList::getCurOption() const {
    if (curOptionIndex < options.size()) {
        return options[curOptionIndex];
    }
    return nullptr;
}

UIOption* UIList::getOption(uint8_t index) const {
    if (index < options.size()) {
        return options[index];
    }
    return nullptr;
}

void UIList::nextOption() {
    if (options.size() > 0) {
        curOptionIndex = (curOptionIndex + 1) % options.size();
    }
}

void UIList::prevOption() {
    if (options.size() > 0) {
        if (curOptionIndex == 0) {
            curOptionIndex = options.size() - 1;
        } else {
            curOptionIndex--;
        }
    }
}

void UIList::selectCurrent() {
    UIOption* current = getCurOption();
    if (current) {
        current->execute();
    }
}
