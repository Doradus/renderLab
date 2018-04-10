#pragma once

class RenderingInterface {
public:
	RenderingInterface();
	virtual ~RenderingInterface();

	virtual void InitRenderer() = 0;

};