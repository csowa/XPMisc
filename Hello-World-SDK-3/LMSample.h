#pragma once
#include <sstream>
#include "Leap.h"

using namespace std;
using namespace Leap;

class SampleListener : public Listener {
public:
	static ostringstream ShowFrame(const Controller&);

	virtual void onInit(const Controller&);
	virtual void onConnect(const Controller&);
	virtual void onDisconnect(const Controller&);
	virtual void onExit(const Controller&);
	virtual void onFrame(const Controller&);
	virtual void onFocusGained(const Controller&);
	virtual void onFocusLost(const Controller&);
	virtual void onDeviceChange(const Controller&);
	virtual void onServiceConnect(const Controller&);
	virtual void onServiceDisconnect(const Controller&);

	static Controller& m_controller();
};
