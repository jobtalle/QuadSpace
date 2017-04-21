#include "quadSpace.h"

#include <stdlib.h>
#include <string.h>
#include <math.h>

void quadSpaceInitialize(struct quadSpace *quad)
{
	quad->nodes = malloc(QUAD_SPACE_NODE_COUNT);
	memset(quad->nodes, 0xFF, QUAD_SPACE_NODE_COUNT);
}

void quadSpaceFree(struct quadSpace *quad)
{
	free(quad->nodes);
}

static uint32_t quadSpaceNodeChildren(uint32_t node)
{
	return (node << 2) + 1;
}

static uint32_t quadSpaceNodeParent(uint32_t node)
{
	return (node - 1) >> 2;
}

static void quadSpaceValidateParent(struct quadSpace *quad, uint32_t child, uint32_t scale)
{
	uint32_t node = quadSpaceNodeParent(child);
	uint32_t children = quadSpaceNodeChildren(node);
	uint8_t newNode =
		quad->nodes[children] |
		quad->nodes[children + 1] |
		quad->nodes[children + 2] |
		quad->nodes[children + 3];

	if(!(
		quad->nodes[children] &
		quad->nodes[children + 1] &
		quad->nodes[children + 2] &
		quad->nodes[children + 3] &
		QUAD_SPACE_SCALE_BIT(scale - 1))) {
		int8_t i;

		for(i = scale - 1; i >= 0; --i) {
			newNode &= ~QUAD_SPACE_SCALE_BIT(i);
		}
	}

	if(quad->nodes[node] != newNode) {
		quad->nodes[node] = newNode;

		if(node != 0) quadSpaceValidateParent(quad, node, scale - 1);
	}
}

quadSpacePosition quadSpaceQuery(struct quadSpace *quad, uint8_t scale)
{
	quadSpacePosition position;
	uint32_t node = 0;
	uint8_t currentScale = 0;

	if(!(quad->nodes[node] & QUAD_SPACE_SCALE_BIT(scale))) {
		position.valid = 0;
		return position;
	}
	else {
		position.valid = 1;
	}

	position.atomX = position.atomY = 0;

	while(currentScale != scale) {
		uint32_t children = quadSpaceNodeChildren(node);
		uint32_t atomStride = POW_2(6 - currentScale);
		uint8_t quarter;

		for(quarter = 0; quarter < 4; ++quarter) {
			if(quad->nodes[children + quarter] & QUAD_SPACE_SCALE_BIT(scale)) break;
		}

		if(quarter == 1 || quarter == 3) position.atomX += atomStride;
		if(quarter > 1) position.atomY += atomStride;

		node = children + quarter;
		++currentScale;
	}

	quad->nodes[node] = 0;
	if(scale != 0) quadSpaceValidateParent(quad, node, scale);

	position.node = node;

	return position;
}

void quadSpaceRelease(struct quadSpace *quad, quadSpacePosition position)
{
	uint8_t level = quadSpaceLevel(position);

	quad->nodes[position.node] = 0xFF;
	if(level != 0) quadSpaceValidateParent(quad, position.node, level);
}

uint8_t quadSpaceLevel(quadSpacePosition position)
{
	return uint8_t(log(3 * (position.node + 1) + 1) / log(4) - 1);
}
