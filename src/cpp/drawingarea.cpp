#include "opencv2/imgproc.hpp"
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"

#include "drawingarea.hpp"

CameraDrawingArea::CameraDrawingArea():
//videoCapture(0) 
videoCapture(gstreamer_pipeline(1920,1080,1920,1080,30,0,0))
{

	// Lets refresh drawing area very now and then.
	everyNowAndThenConnection = Glib::signal_timeout().connect(sigc::mem_fun(*this, &CameraDrawingArea::everyNowAndThen), 33);
	gtk_widget_set_double_buffered(this, false);
}

CameraDrawingArea::~CameraDrawingArea() {
	everyNowAndThenConnection.disconnect();
}

/**
 * Every now and then, we invalidate the whole Widget rectangle,
 * forcing a complete refresh.
 */
bool CameraDrawingArea::everyNowAndThen() {
	BOOST_LOG_TRIVIAL(trace) << "everyNowAndThen called";
	auto win = get_window();
	if (win) {
		Gdk::Rectangle r(0, 0, width, height);
		win->invalidate_rect(r, false);
	}
	
	// Don't stop calling me:
	return true;
}

/**
 * Called every time the widget has its allocation changed.
 */
void CameraDrawingArea::on_size_allocate (Gtk::Allocation& allocation) {
	// Call the parent to do whatever needs to be done:
	DrawingArea::on_size_allocate(allocation);
	
	// Remember the new allocated size for resizing operation:
	width = allocation.get_width();
	height = allocation.get_height();
}

/**
 * Called every time the widget needs to be redrawn.
 * This happens when the Widget got resized, or obscured by
 * another object, or every now and then.
 */
bool CameraDrawingArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr) {
	BOOST_LOG_TRIVIAL(trace) << "on_draw called";
	
	// Prevent the drawing if size is 0:
	if (width == 0 || height == 0) {
		return true;
	}

	// Capture one image from camera:
	auto t1 = std::chrono::high_resolution_clock::now();
	videoCapture.read(webcam);
	auto t2 = std::chrono::high_resolution_clock::now();
	auto d = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
	BOOST_LOG_TRIVIAL(trace) << "capture took " << d.count() << " ms";
	
	// Resize it to the allocated size of the Widget.
	resize(webcam, output, cv::Size(width, height), 0, 0, cv::INTER_LINEAR);

	// Initializes a pixbuf sharing the same data as the mat:
	Glib::RefPtr<Gdk::Pixbuf> pixbuf =
		Gdk::Pixbuf::create_from_data((guint8*)output.data,
									  Gdk::COLORSPACE_RGB,
									  false,
									  8,
									  output.cols,
									  output.rows,
									  (int) output.step);

	// Display
	Gdk::Cairo::set_source_pixbuf(cr, pixbuf);
	cr->paint();
	
	// Don't stop calling me.
	return true;
}

std::string CameraDrawingArea::gstreamer_pipeline (int capture_width, int capture_height, int display_width, int display_height, int framerate, int flip_method, int sensor_id)
{
    return "nvarguscamerasrc sensor-id=" + std::to_string(sensor_id) +  " ! video/x-raw(memory:NVMM), width=(int)" + std::to_string(capture_width) + ", height=(int)" +
           std::to_string(capture_height) + ", format=(string)NV12, framerate=(fraction)" + std::to_string(framerate) +
           "/1 ! nvvidconv flip-method=" + std::to_string(flip_method) + " ! video/x-raw, width=(int)" + std::to_string(display_width) + ", height=(int)" +
           std::to_string(display_height) + ", format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink  drop=true";
}