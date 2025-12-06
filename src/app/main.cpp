#include "mainwindow.h"
#include "VerificationPage.h"
#include "CLICommands.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    // CASE 1 — user passed CLI arguments
    if(argc > 1)
    {
        CLICommands cli;

        // Documentation from 'main' branch (Helpful for reference)
        // xml_editor verify -i test.xml
        // xml_editor mini -i in.xml -o out.xml
        // xml_editor format -i in.xml -o out.xml
        // xml_editor json -i test.xml -o test.json
        // xml_editor compress -i sample.xml -o sample.comp
        // xml_editor decompress -i sample.comp -o sample_decompressed.xml
        // xml_editor mutual -i input_file.xml -ids 1,2,3
        // xml_editor draw -i input_file.xml -o output.png

        return cli.handle(argc, argv);
    }
    else
    {
        // CASE 2 — no arguments → launch GUI
        // We need this initialization from the 'About+Verification_Pages' branch
        // so that the code below works.
        QApplication a(argc, argv);

        a.setStyleSheet("QMessageBox { background-color: #2b2b2b; } QLabel { color: white; }");
        
        // 1. Create and show the Main Window
        MainWindow w;
        w.show();

        // 2. Create and show the Verification Window alongside it
        VerificationPage v;
        v.show();

        return a.exec();
    }
}