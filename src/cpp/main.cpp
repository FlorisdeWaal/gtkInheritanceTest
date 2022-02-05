#include "window.hpp"

int main (int argc, char *argv[]) {
	auto app = Gtk::Application::create(argc, argv, "ch.agl-developpement.cpp-tutorial.raspberry-cpp-gtk-opencv");
	MainWindow mainWindow(1920, 1080);
	//mainWindow.add_tick_callback(frame_clock, null);
	//MainWindow* mwptr = &mainWindow;
	//gtk_widget_add_tick_callback(mwptr);
	return app->run(mainWindow);	
}
