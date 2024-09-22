#include "circuiteditor.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

      CircuitEditor editor;
      editor.show();

      return a.exec();
}
