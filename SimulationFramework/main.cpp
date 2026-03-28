#include <QApplication>
#include <QColorSpace>
#include <QCommandLineParser>
#include <QSurfaceFormat>

#include <QComboBox>
#include <QDockWidget>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QPushButton>
#include <QTabWidget>
#include <QLabel>

#include "GLMainWindow.hpp"
#include "ExampleRenderer.hpp"

#include "Prog1_1Simulation.hpp"
#include "Prog1_2Renderer.hpp"

using namespace Eigen;

#ifdef _WIN32
// always use (proper) hardwarOSe acceleration if available, since Intel's iGPUs have extremely buggy OpenGL support
extern "C"
{
	__declspec(dllexport) DWORD NvOptimusEnablement = 1;
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}
#endif

void addExampleTab(GLMainWindow * mainWindow, QTabWidget * tabWidget)
{

    //select tab widget
    auto current = tabWidget->count();
    //add a tab
    auto tab = new QWidget{tabWidget};
    tabWidget->addTab(tab,"Options");
    //get layout of the tab
    auto layout = new QFormLayout{tab};

    //manipulate the info within the tab
    layout->addRow("",new QLabel("Equation type:"));

    auto rq_type =new QComboBox(tab);
    rq_type->addItem("Explicit");
    rq_type->addItem("Implicit");
    rq_type->addItem("Verlet");
    rq_type->setCurrentIndex(1);
    layout->addRow("Equation",rq_type);

    //axis controls
    layout->addRow("",new QLabel("initial values"));
    auto x_r = new QDoubleSpinBox(tab);
    layout->addRow("r(x)",x_r);

    auto y_r = new QDoubleSpinBox(tab);
    layout->addRow("r(y)",y_r);
    y_r->setValue(1.0);

    auto z_r = new QDoubleSpinBox(tab);
    layout->addRow("r(z)",z_r);

    auto x_dr = new QDoubleSpinBox(tab);
    layout->addRow("dr(x)",x_dr);
    x_dr->setValue(2.0);

    auto y_dr = new QDoubleSpinBox(tab);
    layout->addRow("dr(y)",y_dr);

    auto z_dr = new QDoubleSpinBox(tab);
    layout->addRow("dr(z)",z_dr);

    layout->addRow("",new QLabel("Simulation"));
    auto f_dt = new QDoubleSpinBox(tab);

    f_dt->setMaximum(1.0);
    f_dt->setDecimals(4);
    //system time
    f_dt->setSingleStep(0.01);
    f_dt->setValue(0.05);
    layout->addRow("dt function",f_dt);

    auto grav =new QDoubleSpinBox(tab);
    grav->setValue(9.0);
    layout->addRow("Gravity",grav);

    //reset button
    auto apply_button = new QPushButton("Apply");
    layout->addRow("",apply_button);

    QObject::connect(apply_button,&QPushButton::clicked, [=](){

        Prog1_1Simulation::Integration integration;

        switch (rq_type->currentIndex()) {

        case 1:

            integration = Prog1_1Simulation::Integration::Implicit;
            break;

        case 2:
            integration = Prog1_1Simulation::Integration::Verlet;
            break;

        default:
            integration = Prog1_1Simulation::Integration::Explicit;
            break;
        }
        auto render = (Prog1_1Simulation*) mainWindow->getRenderer();
        render->reset(integration,
                      Vector3d{x_r->value(),y_r->value(),z_r->value()},
                      Vector3d{x_dr->value(),y_dr->value(),z_dr->value()},
                      grav->value(),
                      f_dt->value());
    });

    tab->setMinimumWidth(150);



    QWidget::connect(tabWidget, &QTabWidget::currentChanged, [=] (int index) {
		if(index != current)
			return;


        mainWindow->setRendererFactory(
            [] (QObject * parent) {
				// ExampleRenderer contains our OpenGL code
				return new ExampleRenderer{parent};
            }
        );
    });
}


void addSimTwoTab(GLMainWindow * mainWindow, QTabWidget * tabWidget){

    auto n_body_tab = new QWidget(tabWidget);

    auto layout = new QFormLayout(n_body_tab);

    //add options

    auto n_bodies = new QDoubleSpinBox(n_body_tab);
    n_bodies->setMinimum(0);
    n_bodies->setMaximum(10);
    n_bodies->setValue(0);
    n_bodies->setSingleStep(1);
    n_bodies->setDecimals(0);
    layout->addRow("Number of Objects",n_bodies);

    //select body to manipulate
    auto current_body = new QComboBox(n_body_tab);
    layout->addRow("Current object",current_body);


    //connect the option selection with the number of Object to simulate

    auto* bodies = new std::vector<std::tuple<Eigen::Vector3d , Eigen::Vector3d, double>>();
    QObject::connect(n_bodies, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [=] (double value){

        bodies->resize((int) value);
        for(int i = 0 ; i < value;i++){
            (*bodies)[i] = std::make_tuple(Eigen::Vector3d::Zero(),Eigen::Vector3d::Zero(),1.0);
        }
        current_body->clear();
        for(int i = 0 ; i < value; i++){
            current_body->addItem(QString::number(i));
        }
    });

    //add lables for body properties

    auto properties_label = new QLabel("Properties of current body",n_body_tab);
    layout->addRow(properties_label);

    // add spinners for the 3D position and velocity and the mass of the current body
    auto position_x = new QDoubleSpinBox{n_body_tab};
    position_x->setMinimum(-10);
    position_x->setMaximum(10);
    auto position_y = new QDoubleSpinBox{n_body_tab};
    position_y->setMinimum(-10);
    position_y->setMaximum(10);
    auto position_z = new QDoubleSpinBox{n_body_tab};
    position_z->setMinimum(-10);
    position_z->setMaximum(10);
    auto velocity_x = new QDoubleSpinBox{n_body_tab};
    velocity_x->setMinimum(-10);
    velocity_x->setMaximum(10);
    auto velocity_y = new QDoubleSpinBox{n_body_tab};
    velocity_y->setMinimum(-10);
    velocity_y->setMaximum(10);
    auto velocity_z = new QDoubleSpinBox{n_body_tab};
    velocity_z->setMinimum(-10);
    velocity_z->setMaximum(10);
    auto mass = new QDoubleSpinBox{n_body_tab};
    layout->addRow("Position x", position_x);
    layout->addRow("Position y", position_y);
    layout->addRow("Position z", position_z);
    layout->addRow("Velocity x", velocity_x);
    layout->addRow("Velocity y", velocity_y);
    layout->addRow("Velocity z", velocity_z);
    layout->addRow("Mass", mass);

    // connect the current body dropdown to the spinners, so that the spinners are updated when the current body changes
    QObject::connect(current_body, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index) {
        if (index >= 0) {
            auto& body = (*bodies)[index];
            position_x->setValue(std::get<0>(body)[0]);
            position_y->setValue(std::get<0>(body)[1]);
            position_z->setValue(std::get<0>(body)[2]);
            velocity_x->setValue(std::get<1>(body)[0]);
            velocity_y->setValue(std::get<1>(body)[1]);
            velocity_z->setValue(std::get<1>(body)[2]);
            mass->setValue(std::get<2>(body));
        }
    });

    // add a button to set the current body's properties
    auto set_button = new QPushButton{"Set", n_body_tab};
    QObject::connect(set_button, &QPushButton::clicked, [=]() {
        auto index = current_body->currentIndex();
        auto& body = (*bodies)[index];
        std::get<0>(body) = Eigen::Vector3d{position_x->value(), position_y->value(), position_z->value()};
        std::get<1>(body) = Eigen::Vector3d{velocity_x->value(), velocity_y->value(), velocity_z->value()};
        std::get<2>(body) = mass->value();
    });
    layout->addRow(set_button);

    auto SimulationPropertiesLabel = new QLabel("Simulation Properties",n_body_tab);
    auto grav = new QDoubleSpinBox(n_body_tab);
    auto dtime = new QDoubleSpinBox(n_body_tab);
    dtime->setMinimum(0.000);
    dtime->setMaximum(1);
    dtime->setValue(0.01);
    dtime->setSingleStep(0.001);
    dtime->setDecimals(3);
    layout->addRow(SimulationPropertiesLabel);
    layout->addRow("Gravity",grav);
    layout->addRow("dtime",dtime);

    auto reset_button = new QPushButton("Reset",n_body_tab);
    QObject::connect(reset_button, &QPushButton::clicked, [=](){
        auto r = (Prog1_2Renderer*) mainWindow->getRenderer();
    r->getSimulation()->reset(
                *bodies,
                grav->value(),
                dtime->value());
    });

    tabWidget->addTab(n_body_tab,"Object controll");
    tabWidget->setMinimumWidth(300);



    QWidget::connect(tabWidget, &QTabWidget::currentChanged, [=] (int index) {
        if(index != index)
            return;


        mainWindow->setRendererFactory(
            [] (QObject * parent) {
                // ExampleRenderer contains our OpenGL code
                return new ExampleRenderer{parent};
            }
            );
    });
}

void addSimulationControls(GLMainWindow * mainWindow)
{
	auto dock = new QDockWidget("Simulation controls", mainWindow);
	dock->setAllowedAreas(Qt::DockWidgetArea::RightDockWidgetArea);

	auto tabWidget = new QTabWidget(dock);
	tabWidget->setTabPosition(QTabWidget::TabPosition::West);
	dock->setWidget(tabWidget);

    //addExampleTab(mainWindow, tabWidget);
    addSimTwoTab(mainWindow, tabWidget);

	mainWindow->addDockWidget(Qt::DockWidgetArea::RightDockWidgetArea, dock);
}

int main(int argc, char ** argv)
{
	// set up OpenGL surface format
	auto surfaceFormat = QSurfaceFormat::defaultFormat();
	surfaceFormat.setVersion(3, 3);
	surfaceFormat.setProfile(QSurfaceFormat::CoreProfile);
	surfaceFormat.setOption(QSurfaceFormat::DebugContext);
	surfaceFormat.setColorSpace(QColorSpace::NamedColorSpace::SRgb);
	surfaceFormat.setSamples(4);
	QSurfaceFormat::setDefaultFormat(surfaceFormat);

	// set up a Qt application
	using App = QApplication;
	App::setAttribute(Qt::AA_UseDesktopOpenGL);
	App::setApplicationName("SimulationFramework");
	App::setApplicationDisplayName(App::translate("main", "Simulation Framework"));
	App::setApplicationVersion("1.0");
	App app(argc, argv);

	// configure command line parser
	QCommandLineParser parser;
	parser.setApplicationDescription(App::translate("main", "Simulation framework for the TU Darmstadt lecture on physically based simulation and animation."));
	parser.addHelpOption();
	parser.addVersionOption();

	// provide a flag for OpenGL debugging (outputs can be viewed with debugger!)
	QCommandLineOption debugGLOption({ "g", "debug-gl" }, App::translate("main", "Enable OpenGL debug logging"));
	parser.addOption(debugGLOption);

	// parse command line
	parser.process(app);

	// create main window. modify GLMainWindow.ui to add widgets etc.
	GLMainWindow widget;

	// enable OpenGL error logging (look at debugger output!) if flag is passed
	if(parser.isSet(debugGLOption))
	{
		widget.setOpenGLLoggingSynchronous(true);
		widget.setOpenGLLoggingEnabled(true);
	}

	// set up which renderer to use. factory to create renderer when OpenGL context exists
	widget.setRendererFactory(
		[] (QObject * parent) {
			// ExampleRenderer contains our OpenGL code
            //return new ExampleRenderer{parent};
            return new Prog1_2Renderer{parent};
		}
	);



	addSimulationControls(&widget);

	// show the main window
	widget.show();

	// run the event loop (do not write your own!)
	return app.exec();
}

