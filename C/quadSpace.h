#pragma once

#include <stdint.h>

#define POW_2(pow) (1 << (pow))
#define POW_4(pow) (1 << ((pow) << 1))

#define QUAD_SPACE_DIMENSIONS       POW_2(7)
#define QUAD_SPACE_NODE_COUNT       ((POW_4(8) - 1) / 3)
#define QUAD_SPACE_SCALE_BIT(scale) (1 << (scale))

struct quadSpace {
	uint8_t *nodes;
};

typedef struct {
	uint8_t atomX;
	uint8_t atomY;
	uint8_t valid:1;
	uint16_t node:15;
} quadSpacePosition;

void quadSpaceInitialize(struct quadSpace *quad);
void quadSpaceFree(struct quadSpace *quad);

quadSpacePosition quadSpaceQuery(struct quadSpace *quad, uint8_t scale);
void quadSpaceRelease(struct quadSpace *quad, quadSpacePosition position);
uint8_t quadSpaceLevel(struct quadSpace *quad, quadSpacePosition position);