#include <QCoreApplication>
#include <fmt/core.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    fmt::print("Goodbye World\n");

    return a.exec();
}
