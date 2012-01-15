#include "ARtag.h"
#include <ARToolKitPlus/TrackerSingleMarker.h>

/*! 
   Set the max ARtag ID that is detectable.
*/
#define ARTAG_MAX_ID 99

using ARToolKitPlus::TrackerSingleMarker;
class ARtagLocalizer
{
public:
	ARtagLocalizer();
	~ARtagLocalizer();
	int initARtagPose(int width, int height, float markerWidth, float x_offset = 0.f, float y_offset = 0.f, float yaw_offset = 0.f, float ffactor = 0.97);
	bool getARtagPose(IplImage * src, IplImage * dst, int camID);
	ARtag * getARtag(int index);
	int getARtagSize();
	void setARtagOffset(float x_offset, float y_offset, float yaw_offset);
	int cleanupARtagPose(void);

	static bool allStop;
	
private:
	int imgwidth;
	int imgheight;
	bool useBCH;
	bool init;
	float xoffset;
	float yoffset;
	float yawoffset;
	float fudge;

	std::vector<ARtag> mytag;
	float patternWidth_;
	float patternCenter_[2];
	TrackerSingleMarker *tracker;
};
