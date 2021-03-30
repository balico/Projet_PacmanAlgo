#include <time.h>
#include "pacmanwindow.h"

#ifdef WIN32
// Mandatory if linked with static Qt
// Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin);
#endif

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    srand(time(0));

    PacmanWindow wnd;
    wnd.show();

    return app.exec();
}
