#include "abstracttool.h"
#include "converttool.h"
#include "extracttool.h"
#include "exception.h"
#include "mainparser.h"
#include "showtool.h"
#include "toolparser.h"

#include <QtCore/QDebug>
#include <QtGui/QGuiApplication>

#include <ImageIO>

#include <map>
#include <memory>

using namespace ImageTool;
using ToolsMap = std::map<QByteArray, std::unique_ptr<AbstractTool>>;
using DescriptionMap = MainParser::DescriptionMap;

static ToolsMap createTools()
{
    std::unique_ptr<AbstractTool> showTool(new ShowTool);
    std::unique_ptr<AbstractTool> convertTool(new ConvertTool);
    std::unique_ptr<AbstractTool> extractTool(new ExtractTool);
    ToolsMap result;
    result[showTool->id()] = std::move(showTool);
    result[convertTool->id()] = std::move(convertTool);
    result[extractTool->id()] = std::move(extractTool);
    return result;
}

static MainParser::DescriptionMap getDescriptions(const ToolsMap &map)
{
    MainParser::DescriptionMap result;
    for (const auto &item: map)
        result[item.first] = item.second->decription();
    return result;
}

int main(int argc, char *argv[])
{
    try {
        QCoreApplication app(argc, argv);
        app.setApplicationName("imagetool");
        app.setApplicationVersion("1.0");
        app.addLibraryPath(app.applicationDirPath() + ImageIO::pluginsDirPath());

        const auto tools = createTools();

        MainParser parser(getDescriptions(tools));
        parser.process(app.arguments());

        const auto toolName = parser.toolName();
        const auto it = tools.find(toolName.toLatin1());
        if (it == tools.end()) {
            parser.showError(app.translate("ImageTool", "Unknown command %1").arg(toolName));
            parser.showHelp(EXIT_FAILURE);
        }

        return it->second->run(parser.arguments());
    } catch (const ExitException &ex) {
        return ex.code();
    } catch (const RuntimeError &ex) {
        MainParser::showError(ex.message());
        return 1;
    } catch (const std::exception &ex) {
        MainParser::showError(QString::fromLatin1(ex.what()));
        return 1;
    }
}

