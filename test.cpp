#include "test.h"

IMPLEMENT_APP(MyApp);

bool MyApp::OnInit() {
    GUI *frame = new GUI();
    frame->Show( true );
    return true;
}

using namespace std;

void getUserInputPhrase(std::string &phraseToHide) {
    cin.ignore (std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Phrase to hide:\n";
    std::getline(std::cin, phraseToHide);
}

void getUserInputPassword(std::string &password) {
    std::cout << "Enter your password: ";
    std::cin >> password;
}

void getUserMenuOption(std::string &userInput) {
    std::cout << "Choose what you want to do:\n";
    std::cout << "1) Hide text in Picture\n";
    std::cout << "2) Get text from Picture\n";
    std::cout << "3) Hide file in Picture\n";
    std::cout << "4) Unhide file in Picture\n";
    std::cout << "Your choice: ";
    std::cin >> userInput;
}

void hide() {
    SteganoHide myHide;
    try {
        myHide.loadPicture(HIDE_FILE);
    } catch (SteganoException &stegoException) {
        std::cerr << stegoException.what() << std::endl;
        exit(0);
    }

    std::string phraseToHide, password;
    getUserInputPhrase(phraseToHide);
    getUserInputPassword(password);

    try {
        myHide.hidePhrase(phraseToHide, password);
    } catch (SteganoException &stegoException) {
        std::cerr << stegoException.what() << std::endl;
        exit(0);
    }
    myHide.saveChangesToDisk();
    std::cout << "Text successful hidden" << std::endl;
}

void unhide() {
    SteganoUnhide stegUnhide;
    try {
        stegUnhide.loadPicture(STEGANO_FILE);
    } catch (SteganoException &stegoException) {
        std::cerr << stegoException.what() << std::endl;
        exit(0);
    }

    std::string password;
    getUserInputPassword(password);
    std::cout << "The hidden text is:\n--------------------\n" << stegUnhide.unhidePhrase(password).str() << "\n-------------------------" << std::endl;
}

void hideFile() {
    std::string fileToHide, containerFile;
    cin.ignore (std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Enter the file you want to hide: ";
    std::getline(std::cin, fileToHide);
    //cin.ignore (std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Enter the container file: ";
    std::getline(std::cin, containerFile);

    std::ifstream inputFileStream(fileToHide, std::ifstream::in | std::ifstream::binary);

    SteganoHide myHide;
    try {
        myHide.loadPicture(containerFile);
    } catch (SteganoException &loadError) {
        std::cerr << loadError.what() << std::endl;
        exit(0);
    }

    try {
        std::thread showPercentThread([&]() {
            unsigned char doneStateInPercent = myHide.getDoneStateInPercent();
            while(doneStateInPercent < 100) {
                // need to flush that the cursor is always at the end of the line.
                std::cout.flush();
                std::cout << "\r" << (int)doneStateInPercent << " / 100%";
                doneStateInPercent = myHide.getDoneStateInPercent();
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            }
        });
        showPercentThread.detach();
        myHide.hideFile(inputFileStream, "empty");
    } catch (SteganoException &stegoException) {
        std::cerr << stegoException.what() << std::endl;
        exit(0);
    }

    myHide.saveChangesToDisk();
    inputFileStream.close();
    std::cout << "\nHiding successful!" << std::endl;
}

void unhideFile() {
    std::string outputFile, containerFile;
    cin.ignore (std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Enter the container file: ";
    std::getline(std::cin, containerFile);
    //cin.ignore (std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Enter a new name for the extracted file: ";
    std::getline(std::cin, outputFile);


    SteganoUnhide unHide;
    try {
        unHide.loadPicture(containerFile);
    } catch (SteganoException &loadError) {
        std::cerr << loadError.what() << std::endl;
        exit(0);
    }


    std::ofstream outputFileStream(outputFile, std::ofstream::out | std::ofstream::binary);
    try {
        outputFileStream << unHide.unhidePhrase("empty").rdbuf();
    } catch (SteganoException &stegoException) {
        std::cerr << stegoException.what() << std::endl;
        exit(0);
    }
    outputFileStream.close();
}

/*int main(int argc,char **argv)
{
 //   MyApp myApp;
    Magick::InitializeMagick(*argv);

    std::string menuOption;
    unsigned char userInput;
    getUserMenuOption(menuOption);

    try {
       userInput = std::stoi(menuOption);
    } catch(std::invalid_argument const &inv_arg) {
        std::cerr << "You entered something wrong." << std::endl;
        exit(0);
    }

    switch(userInput) {
    case 1:
        hide();
        break;
    case 2:
        unhide();
        break;
    case 3:
        hideFile();
        break;
    case 4:
        unhideFile();
        break;
    }

    return 0;
}*/

