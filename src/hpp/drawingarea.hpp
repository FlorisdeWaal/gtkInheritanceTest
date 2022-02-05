#ifndef CAMERA_DRAWING_AREA_H
#define CAMERA_DRAWING_AREA_H

#include <opencv2/highgui.hpp>
#include <gtkmm.h>
#include <boost/log/trivial.hpp>
#include <chrono>

class CameraDrawingArea :
public Gtk::DrawingArea {
public:
    CameraDrawingArea();
    virtual ~CameraDrawingArea();
    
protected:
    bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
	void on_size_allocate (Gtk::Allocation& allocation) override;
	
	bool everyNowAndThen();

private:
	sigc::connection everyNowAndThenConnection;
	cv::VideoCapture videoCapture;
	cv::Mat webcam;
	cv::Mat output;
	int width, height;
	std::string gstreamer_pipeline(int capture_width, int capture_height, int display_width, int display_height, int framerate, int flip_method, int sensor_id);
	std::chrono::milliseconds duration;
};
#endif
