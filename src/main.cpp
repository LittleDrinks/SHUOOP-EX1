#include "app/application.hpp"

int main() {
    Application& app = Application::getInstance();
    app.loadData();
    app.showUI();
    app.saveData();
    return 0;
}