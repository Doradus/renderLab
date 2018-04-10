#pragma once
#ifndef RENDERING_INTERFACE_H
#define RENDERING_INTERFACE_H

class RenderingInterface {
public:
	RenderingInterface();
	virtual ~RenderingInterface();

	virtual void InitRenderer() = 0;

};

#endif