#ifndef TERMPROJ_MIRROR_H
#define TERMPROJ_MIRROR_H


#include <vector>
#include <string>
#include "widgets/Widget.h"
#include <iostream>
#include <stdio.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <stropts.h>
#include <unistd.h>

class Mirror {
public:
    std::vector<Widget*> selectedWidgets;

    Mirror();
    Mirror(std::string configFilename);
    ~Mirror();

    void run();

private:

    bool exit = false;
    std::string name;
    std::vector<std::string> const allWidgets = {"Weather", "Stock", "Quote"};
    nlohmann::json config;
    nlohmann::json data = {{"widgets", nlohmann::json::array()}};


    void configure();

    void configure(std::string configFileName);

    void changeName();

    /*************
     * This method will display main options:
     *       Add a widget
     *       Edit a selected widget
     *       Delete a configured widget
     *       Show configured widgets
     *       Exit
     */
    int displayMainOptions();


    /*****************************
     * Display a list of selected/configured widgets
     * use this when user wants to  delete or edit
     */
    void displayExistingWidgets();


    /********************************
     * Display a list of widgets able to be added
     * For example, some widgets might be able to be
     * added more than once, but others only once,
     * so only display the correct widgets;
     */
    void displayAddableWidgets();


    /*************************
     * shows the available widgets and asks for a choice
     */
    void addWidget();


    /**********************************
     * Calls the configure method of the selected widget
     * and then adds it to the widgets array
     * @param widgetName
     */
    void addWidget(std::string widgetName);



    /*****************************
     * Basically, get the json results of each selectedWidget
     * and combine to make a big json object
     * foreach widget in selectedWidgets
     *                  widget.refreshData()
     *
     * @return json results of ALL selectedwidgets
     */
    nlohmann::json getData();


    /*****************************************
     * This uses the getData() return to
     * write the html file that will be displayed
     * on the system
     *
     */
    void publishData();

    void updateDataWidget(nlohmann::json widgetData)
    {
        bool found = false;
        for (auto& widget : data["widgets"])
        {
            if (widget["name"].get<std::string>() == widgetData["name"].get<std::string>())
            {
                found = true;
                widget = widgetData;
            }
        }

        if (!found)
            data["widgets"].push_back(widgetData);
    }

    /**
     Linux (POSIX) implementation of _kbhit().
     Morgan McGuire, morgan@cs.brown.edu
     via www.flipcode.com/archives/_kbhit_for_Linux.shtml

     This returns a nonzero integer if keyboard has been hit
     USAGE:
        while (!keyboard_hit()){ do something }
     */

    int keyboard_hit() {
        //
        fflush(stdout);

        static const int STDIN = 0;
        static bool initialized = false;

        if (! initialized) {
            // Use termios to turn off line buffering
            termios term;
            tcgetattr(STDIN, &term);
            term.c_lflag &= ~ICANON;
            tcsetattr(STDIN, TCSANOW, &term);
            setbuf(stdin, NULL);
            initialized = true;
        }

        int bytesWaiting;
        ioctl(STDIN, FIONREAD, &bytesWaiting);
        return bytesWaiting;
    }

    void clearCin();

    void exitMirror();
};


#endif //TERMPROJ_MIRROR_H
