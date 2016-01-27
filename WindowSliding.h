#pragma once

class CWindowSliding
{
public:
	CWindowSliding(void);
	int calSWAverage(float* pfData, float* pfAve, int indexI, int indexF, int nDelta, int nWS);
public:
	~CWindowSliding(void);
};
